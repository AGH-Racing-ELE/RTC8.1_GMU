#ifndef CAN_LIB_H
#define CAN_LIB_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"
#include "CAN_value_types.h"

#define CAN_MAX_VALUE_NAME 40
#define CAN_MAX_FRAME_NUM 20


struct can_msg {

	uint8_t	isExtId;
	uint32_t id;
	uint32_t dlc;
	uint8_t data[8];
};

struct can_frame {

	struct can_msg core;
	uint16_t period;
	uint32_t tick_ms;
	uint8_t values_num;
};

struct can_value {

	struct can_frame* pFrame;
	uint8_t name[CAN_MAX_VALUE_NAME];
	uint8_t bit_ofs;
	uint16_t mult;
	uint16_t div;
	uint16_t ofs;
	enum can_value_type type;
};

void CAN_Init(void);
void CAN_InitFrame(struct can_frame* pFrame, uint16_t id, uint16_t period, uint8_t dlc);
void CAN_AddExtId(struct can_frame* pFrame, uint32_t ext_id);
void CAN_InitValue(struct can_frame* pFrame, struct can_value* pCan_value, uint8_t bit_ofs,
		uint16_t mult, uint16_t div, uint16_t ofs, enum can_value_type type);
void CAN_WriteValueFloat(struct can_value* pCan_value, float value);
float CAN_ReadValueFloat(struct can_value* pCan_value);
uint8_t CAN_IsValueActual(struct can_value* pCan_value);
uint8_t CAN_IsFrameActual(struct can_frame* pFrame);
uint8_t CAN_SendFrame(struct can_frame* pFrame);
void CAN_ReceiveCallback(struct can_msg* msg);
void CAN_TxCompleteCallback(void);
void CAN_SetRxCallback(void (*callback)(struct can_frame* frame));

#ifdef __cplusplus
 }
#endif

#endif
