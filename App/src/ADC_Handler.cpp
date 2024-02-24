#include "ADC_Handler.h"
#include "stm32f303xc.h"
#include "opamp.h"
#include "adc.h"
#include "Channels.h"
#include "Gearbox.h"

ADC_Handler theADC;

void ADC_Handler::init(void)
{

	HAL_OPAMP_Start(&hopamp1);
	HAL_OPAMP_Start(&hopamp2);
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	HAL_ADC_Start_IT(&hadc1);
	HAL_ADC_Start_IT(&hadc2);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc == &hadc2) {
		uint32_t ap_adc = HAL_ADC_GetValue(hadc);
		theChannels.setValue(ChanIndex::AirPressureADC, ap_adc);

		uint32_t ap_volt = (ap_adc * 3300 * 5 / (4095 * 3));
		if(ap_volt < 500 || ap_volt > 4500) {
			theChannels.setValue(ChanIndex::AirPressure, 0);
		} else {
			//ap_volt -= 500;
			//theChannels.setValue(ChanIndex::AirPressure, ap_volt * 25 / 1000);
			theChannels.setValue(ChanIndex::AirPressure, (uint8_t)(0.024f*(float)ap_volt-2.0f - 10.0f));
		}
	} else if(hadc == &hadc1) {
		theChannels.setValue(ChanIndex::GearPositionADC, HAL_ADC_GetValue(hadc));
		theGearbox.updateGearChannel();
	}
}
