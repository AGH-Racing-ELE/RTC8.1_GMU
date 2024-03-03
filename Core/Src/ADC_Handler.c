/*
 * ADC_Handler.c
 *
 *  Created on: Feb 26, 2024
 *      Author: Admin
 */

#include "ADC_Handler.h"

volatile uint16_t adcValue[10];
uint16_t adcValueMean;

void ADC_Init(void)
{
	HAL_OPAMP_Start(&hopamp1);
	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adcValue,10);
}

uint16_t getADCValue(void)
{
	uint32_t adcValueTmp = 0;
	for(uint8_t i = 0; i>=10; i++)
	{
		adcValueTmp += adcValue[i];
	}
	return adcValueMean = adcValueTmp / 10;
}
