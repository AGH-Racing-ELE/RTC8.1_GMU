#ifndef CAN_LIB_HW_H
#define CAN_LIB_HW_H

#include <sys/_stdint.h>

#include "CAN_LIB.h"
#include "can.h"

#define CAN_HANDLE hcan

#define CAN_ARB_LOST_RETR 20

uint32_t CAN_HW_GetTickMs(void);
uint8_t CAN_HW_SendFrame(struct can_frame* pFrame);
void CAN_HW_Init(void);


#endif

