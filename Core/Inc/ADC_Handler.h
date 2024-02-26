/*
 * ADC_Hander.h
 *
 *  Created on: Feb 26, 2024
 *      Author: Admin
 */

#ifndef INC_ADC_HANDLER_H_
#define INC_ADC_HANDLER_H_

#include "stm32f303xc.h"
#include "opamp.h"
#include "adc.h"
#include <stdint.h>

void ADC_Init(void);
uint16_t getADCValue(void);

#endif /* INC_ADC_HANDLER_H_ */
