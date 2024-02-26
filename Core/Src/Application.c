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
	uint8_t clutch_data;

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
	CAN_Handler_SendGmu1Frame(getGear(&gearbox), 0, getADCValue(), (uint8_t)getState(&gearbox),0, 0,getGearCut(&gearbox));
	CAN_Handler_SendGmu2Frame(0, 0, 0, 0);
}

void statusLED(void)
{
	HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
}
void AppProcess(void)
{
	uint8_t CANRXtick = HAL_GetTick();
	uint8_t CANSendFrameTick = HAL_GetTick();
	uint8_t StatusLEDTick = HAL_GetTick();

	while(1)
	{
		if((HAL_GetTick() - CANRXtick) > 1)
		{
			canRxProcess();
			CANRXtick = HAL_GetTick();
		}
		if((HAL_GetTick() - CANSendFrameTick) > 2)
		{
			canRxProcess();
			CANSendFrameTick = HAL_GetTick();
		}
		if((HAL_GetTick() - StatusLEDTick) > 1000)
		{
			canRxProcess();
			StatusLEDTick = HAL_GetTick();
		}
	}
}

