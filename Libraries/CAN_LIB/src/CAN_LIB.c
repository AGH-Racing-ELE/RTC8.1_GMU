#include "CAN_LIB.h"
#include "CAN_LIB_HW.h"
#include "CAN_FIFO.h"
#include <string.h>

uint64_t CAN_ConvertIntToFrameBits(int32_t value, uint8_t bit_ofs, uint8_t length);
uint64_t CAN_ConvertUintToFrameBits(uint32_t value, uint8_t bit_ofs, uint8_t length);
int32_t CAN_ConvertFrameBitsToInt(uint8_t* ptr, uint8_t bit_ofs, uint8_t length);
uint32_t CAN_ConvertFrameBitsToUint(uint8_t* ptr, uint8_t bit_ofs, uint8_t length);


uint32_t CAN_GetTickMs(void);
void (*CAN_RxCallback)(struct can_frame* frame) = NULL;

struct CAN_FIFO FIFO_CAN_TX;

struct can_info {
	
	uint8_t frames_num;
	struct can_frame* pFrames[CAN_MAX_FRAME_NUM];
	uint8_t values_num;
}_can_info;


void CAN_Init(){
	CAN_HW_Init();
	CAN_FIFO_Flush(&FIFO_CAN_TX);
}

void CAN_InitFrame(struct can_frame* pFrame, uint16_t id, uint16_t period, uint8_t dlc) {
	
	pFrame->core.id = id;
	pFrame->period = period;
	pFrame->core.dlc = dlc;
	_can_info.pFrames[_can_info.frames_num] = pFrame;
	_can_info.frames_num++;
}

void CAN_AddExtId(struct can_frame* pFrame, uint32_t ext_id){
	pFrame->core.isExtId = 1;
	pFrame->core.id |= ((ext_id<<14)>>14)<<18;
}

void CAN_InitValue(struct can_frame* pFrame, struct can_value* pCan_value, uint8_t bit_ofs,
					uint16_t mult, uint16_t div, uint16_t ofs, enum can_value_type type) {
	
	pCan_value->pFrame = pFrame;
	pCan_value->bit_ofs = bit_ofs;
	pCan_value->mult = mult;
	pCan_value->div = div;
	pCan_value->ofs = ofs;
	pCan_value->type = type;
	pFrame->values_num++;
	_can_info.values_num++;
}

/* returns index of frame in _can_info or
   returns 255 if didn't found matching frame*/
uint8_t FilterFrame(uint16_t id){
	
	uint8_t i;
	for(i = 0; i < _can_info.frames_num; i++){
		if(_can_info.pFrames[i]->core.id == id){
			return i;
		}
	}
	return 255;
}
/**********************************************/

inline uint32_t CAN_ConvertFrameBitsToUint(uint8_t* ptr, uint8_t bit_ofs, uint8_t length){
	return (uint32_t)((((*(uint64_t*)ptr)>>bit_ofs)<<(64-length))>>(64-length));
}

inline int32_t CAN_ConvertFrameBitsToInt(uint8_t* ptr, uint8_t bit_ofs, uint8_t length){
	return (int32_t)((((*(int64_t*)ptr)>>bit_ofs)<<(64-length))>>(64-length));
}

inline uint64_t CAN_ConvertUintToFrameBits(uint32_t value, uint8_t bit_ofs, uint8_t length){
	return ((((uint64_t)value)<<bit_ofs)<<(64-length-bit_ofs))>>(64-length-bit_ofs);
}

inline uint64_t CAN_ConvertIntToFrameBits(int32_t value, uint8_t bit_ofs, uint8_t length){
	int64_t int_temp;
	int_temp = ((((int64_t)value)<<bit_ofs)<<(64-length-bit_ofs))>>(64-length-bit_ofs);
	return *(uint64_t*)&int_temp;
}



void CAN_WriteValueFloat(struct can_value* pCan_value, float value){
	
	uint64_t* ptr = (uint64_t*)pCan_value->pFrame->core.data;
	uint64_t mask;
	uint8_t type;

	value =  (value - pCan_value->ofs)/pCan_value->mult*pCan_value->div;

	type = pCan_value->type;

	if(type <= 32){
		mask = CAN_ConvertUintToFrameBits(0xffffffff, pCan_value->bit_ofs, type);
		//clear value bits in frame
		*ptr &= ~mask;
		//copy value bits to frame
		*ptr |= CAN_ConvertUintToFrameBits((uint32_t)value, pCan_value->bit_ofs, type);
	}
	else{
		mask = CAN_ConvertUintToFrameBits(0xffffffff, pCan_value->bit_ofs, type-32);
		//clear value bits in frame
		*ptr &= ~mask;
		//copy value bits to frame
		*ptr |= CAN_ConvertIntToFrameBits((int32_t)value, pCan_value->bit_ofs, type-32);
	}
}


float CAN_ReadValueFloat(struct can_value* pCan_value){
	
	uint8_t type;
	float value;
	
	type = pCan_value->type;
	if(type <= 32){
		value = (float)CAN_ConvertFrameBitsToUint(pCan_value->pFrame->core.data, pCan_value->bit_ofs, type);
	}
	else{
		value = (float)CAN_ConvertFrameBitsToInt(pCan_value->pFrame->core.data, pCan_value->bit_ofs, type-32);
	}
	
	value = (value + pCan_value->ofs)*pCan_value->mult/pCan_value->div;
	return value;
}

inline uint32_t CAN_GetTickMs(){
	return CAN_HW_GetTickMs();
}

uint8_t CAN_IsValueActual(struct can_value* pCan_value){
	if(!pCan_value->pFrame->tick_ms) return 0;
	if(CAN_GetTickMs() - pCan_value->pFrame->tick_ms > pCan_value->pFrame->period*4) return 0;
	return 1;
}

uint8_t CAN_IsFrameActual(struct can_frame* pFrame){
	if(!pFrame->tick_ms) return 0;
	if(CAN_GetTickMs() - pFrame->tick_ms > pFrame->period*10) return 0;
	return 1;
}

inline uint8_t CAN_SendFrame(struct can_frame* pFrame) {
	if(!CAN_HW_SendFrame(pFrame)){
		CAN_FIFO_Add(&FIFO_CAN_TX, (uint8_t*)pFrame);
	}
	return 1;
}

void CAN_SetRxCallback(void (*callback)(struct can_frame* frame))
{
	CAN_RxCallback = callback;
}

inline void CAN_ReceiveCallback(struct can_msg* msg){
	uint8_t index;
	index = FilterFrame(msg->id);
	if(index==255) return;
	memcpy(_can_info.pFrames[index]->core.data, msg->data, 8);
	_can_info.pFrames[index]->tick_ms = CAN_GetTickMs();
	if(CAN_RxCallback != NULL) CAN_RxCallback(_can_info.pFrames[index]);
}

void CAN_TxCompleteCallback(){

	uint8_t* pFrame;


	pFrame = CAN_FIFO_Get(&FIFO_CAN_TX);
	if(!pFrame) return;
	CAN_SendFrame((struct can_frame*)pFrame);
}


