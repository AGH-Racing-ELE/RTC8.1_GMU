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

	CAN_Handler_SendGmu1Frame(13, 15, getADCValue(), (uint8_t)getState(&gearbox),0, 0,getGearCut(&gearbox));
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

