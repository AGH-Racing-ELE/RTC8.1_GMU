#ifndef CAN_HANDLER_H_
#define CAN_HANDLER_H_

#include "stm32f3xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif

void CAN_Handler_Init(void);
uint8_t CAN_Handler_IsGearUpCommanded(void);
uint8_t CAN_Handler_IsGearDownCommanded(void);
uint8_t CAN_Handler_IsClutchCommanded(uint8_t* data);
void CAN_Handler_SendGmu1Frame(uint8_t gear, uint8_t clutch, uint16_t gear_pos_adc, uint8_t state, uint8_t air_preas, uint8_t reset_det, uint8_t gear_cut);
void CAN_Handler_SendGmu2Frame(uint16_t partial_upshift_t, uint16_t partial_downshift_t, uint16_t upshift_t, uint16_t downshift_t);
void CAN_Handler_SendGmu3Frame(uint8_t gear_cut);


#ifdef __cplusplus
}
#endif

#endif /* CAN_HANDLER_H_ */
