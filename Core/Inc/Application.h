/*
 * Application.h
 *
 *  Created on: Feb 26, 2024
 *      Author: Admin
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Gearbox.h"
#include "ADC_Handler.h"
#include "CAN_Handler.h"

void AppInit(void);
void canRxProcess(void);
void sendCANFrame(void);
void statusLED(void);

#endif /* INC_APPLICATION_H_ */
