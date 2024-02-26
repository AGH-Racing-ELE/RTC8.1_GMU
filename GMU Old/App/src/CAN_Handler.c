#include <CAN_Handler.h>
#include "CAN_LIB.h"

struct can_frame cf_gear_up, cf_gear_down, cf_clutch;
struct can_frame cf_gmu1, cf_gmu2;


void CAN_Handler_Init(){

	CAN_Init();

	CAN_InitFrame(&cf_gear_up, 0x000, 0, 1);
	CAN_InitFrame(&cf_gear_down, 0x001, 0, 1);
	CAN_InitFrame(&cf_clutch, 0x002, 0, 1);
	CAN_InitFrame(&cf_gmu1, 0x080, 10, 8);
	CAN_InitFrame(&cf_gmu2, 0x081, 100, 8);

}

uint8_t CAN_Handler_IsGearUpCommanded(){

	static uint32_t last_tick = 0;

	if(cf_gear_up.tick_ms != last_tick && cf_gear_up.core.data[0] == 255){
		last_tick = cf_gear_up.tick_ms;
		return 1;
	}
	else{
		return 0;
	}


}

uint8_t CAN_Handler_IsGearDownCommanded(){

	static uint32_t last_tick = 0;

	if(cf_gear_down.tick_ms != last_tick && cf_gear_down.core.data[0] == 255){
		last_tick = cf_gear_down.tick_ms;
		return 1;
	}
	else{
		return 0;
	}


}

uint8_t CAN_Handler_IsClutchCommanded(uint8_t* data){

	static uint32_t last_tick = 0;

	if(cf_clutch.tick_ms != last_tick){
		last_tick = cf_clutch.tick_ms;
		*data = cf_clutch.core.data[0];
		return 1;
	}
	else{
		return 0;
	}


}

void CAN_Handler_SendGmu1Frame(uint8_t gear, uint8_t clutch, uint16_t gear_pos_adc, uint8_t state, uint8_t air_preas, uint8_t reset_det, uint8_t gear_cut){

	cf_gmu1.core.data[0] = gear;
	cf_gmu1.core.data[1] = clutch;
	cf_gmu1.core.data[2] = (uint8_t)((gear_pos_adc >> 8) & 0xFF);
	cf_gmu1.core.data[3] = (uint8_t)(gear_pos_adc & 0xFF);
	cf_gmu1.core.data[4] = state;
	cf_gmu1.core.data[5] = air_preas;
	cf_gmu1.core.data[6] = (uint8_t)((gear_cut)&0xFF);
	cf_gmu1.core.data[7] = reset_det;

	CAN_SendFrame(&cf_gmu1);
}

void CAN_Handler_SendGmu2Frame(uint16_t partial_upshift_t, uint16_t partial_downshift_t, uint16_t upshift_t, uint16_t downshift_t){

	cf_gmu2.core.data[0] = (uint8_t)((partial_upshift_t >> 8) & 0xFF);
	cf_gmu2.core.data[1] = (uint8_t)(partial_upshift_t & 0xFF);
	cf_gmu2.core.data[2] = (uint8_t)((partial_downshift_t >> 8) & 0xFF);
	cf_gmu2.core.data[3] = (uint8_t)(partial_downshift_t & 0xFF);
	cf_gmu2.core.data[4] = (uint8_t)((upshift_t >> 8) & 0xFF);
	cf_gmu2.core.data[5] = (uint8_t)(upshift_t & 0xFF);
	cf_gmu2.core.data[6] = (uint8_t)((downshift_t >> 8) & 0xFF);
	cf_gmu2.core.data[7] = (uint8_t)(downshift_t & 0xFF);

	CAN_SendFrame(&cf_gmu2);
}

