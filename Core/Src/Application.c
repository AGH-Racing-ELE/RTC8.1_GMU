/*
 * Application.h
 *
 *  Created on: Feb 26, 2024
 *      Author: Admin
 */

#include "Application.h"


Gearbox_t gearbox;
static uint8_t clutch_data;

void AppInit(void)
{
	gearboxInit(&gearbox);
	ADC_Init();
	CAN_Handler_Init();
}

void canRxProcess()
{
	if(CAN_Handler_IsGearUpCommanded())
	{
		disableAutoshift(&gearbox);
		startUpshift(&gearbox);
		return;
	}
	if(CAN_Handler_IsGearDownCommanded())
	{
		disableAutoshift(&gearbox);
		startDownshift(&gearbox);
		return;
	}
	if(CAN_Handler_IsClutchCommanded(&clutch_data))
	{
		if(clutch_data == 255)
		{
			clutch(true);
		}
		else if(clutch_data == 0)
		{
			clutch(false);
		}
		return;
	}
	if(CAN_Handler_IsAutoshiftCommanded())
	{
		enableAutoshift(&gearbox);
		return;
	}

}

void sendCANFrame(void)
{

	CAN_Handler_SendGmu1Frame(gearbox.actual_gear, clutch_data, gearbox.gearPosADC, gearbox._state, 0, 0, gearbox.gear_cut);
	CAN_Handler_SendGmu2Frame(1, 2, 3, 4);
}

void statusLED(void)
{
	HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
}
void AppProcess(void)
{
	uint32_t CANRXtick = HAL_GetTick();
	uint32_t CANSendFrameTick = HAL_GetTick();
	uint32_t StatusLEDTick = HAL_GetTick();

	while(1)
	{

		updateGear(&gearbox);
		processCallback(&gearbox);
		if(gearbox._autoshiftState == AutoshiftEnable)
		{
			autoshiftProcess(&gearbox);
		}
		if((HAL_GetTick() - CANRXtick) > 1)
		{
			canRxProcess();
			CANRXtick = HAL_GetTick();
		}
		if((HAL_GetTick() - CANSendFrameTick) > 2)
		{
			sendCANFrame();
			CANSendFrameTick = HAL_GetTick();
		}
		if((HAL_GetTick() - StatusLEDTick) > 1000)
		{
			statusLED();
			StatusLEDTick = HAL_GetTick();
		}

	}
}

