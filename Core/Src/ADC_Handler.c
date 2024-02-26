/*
 * ADC_Handler.c
 *
 *  Created on: Feb 26, 2024
 *      Author: Admin
 */

#include "ADC_Handler.h"

uint16_t adcValue;

void ADC_Init(void)
{
	HAL_OPAMP_Start(&hopamp1);
	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
	HAL_ADC_Start_IT(&hadc1);
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1)
	{
		adcValue = HAL_ADC_GetValue(hadc);
	}
}

uint16_t getADCValue(void)
{
	return adcValue;
}
