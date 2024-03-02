/*
 * Application.h
 *
 *  Created on: Feb 26, 2024
 *      Author: Admin
 */

#include "Application.h"


Gearbox_t gearbox;

void AppInit(void)
{
	gearboxInit(&gearbox);
	ADC_Init();
	CAN_Handler_Init();

}

void canRxProcess()
{
	static uint8_t clutch_data;

	if(CAN_Handler_IsGearUpCommanded())
	{
		startUpshift(&gearbox);
		return;
	}
	if(CAN_Handler_IsGearDownCommanded())
	{
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

}

void sendCANFrame(void)
{

	CAN_Handler_SendGmu1Frame(gearbox.actual_gear, HAL_GPIO_ReadPin(CLUTCH_GPIO_Port, CLUTCH_Pin), gearbox.gearPosADC, gearbox._state, 0, 0, gearbox.gear_cut);
	CAN_Handler_SendGmu2Frame(0, 0, gearbox.upshiftTime, gearbox.downshiftTime);
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
		gearbox.gearPosADC = getADCValue();
		updateGear(&gearbox);
		processCallback(&gearbox);

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

