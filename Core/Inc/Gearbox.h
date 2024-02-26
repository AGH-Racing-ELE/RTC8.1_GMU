/*
 * Gearbox.h
 *
 *  Created on: Feb 18, 2024
 *      Author: Admin
 */

#ifndef GEARBOX_H_
#define GEARBOX_H_

#include <stdint.h>
#include <stdbool.h>


typedef enum
{
	Idle,
	StartUpshift,
	UpshiftFromNeutral,
	UpshiftGearCut,
	UpshiftFinish,
	StartDownshift,
	DownshiftToNeutral,
	DownshiftRevMatch,
	DownshiftFinish,
	OL_US_GearCut,
	OL_US_Finish,
	OL_DS_Clutch,
	OL_DS_Finish
}GearboxState_t;

typedef struct
{
	uint16_t gearPosADC;
	uint16_t neutral_adc;
	uint16_t gear1_adc;
	uint16_t gear2_adc;
	uint16_t gear3_adc;
	uint16_t gear4_adc;

	//const uint16_t _G_MAP[10];

	uint16_t _US_ACTUATOR_DELAY; //previous 20
	uint16_t _US_FINISH_TIMEOUT;

	uint16_t _DS_ACTUATOR_DELAY;    // previous value = 40
	//static const uint16_t _DS_PARTIAL_TIMEOUT = 200;
	uint16_t _DS_FINISH_TIMEOUT;  // 250

	uint16_t _OL_US_GEARCUT_DELAY;
	uint16_t _OL_DS_CLUTCH_DELAY;

	GearboxState_t _state;

	uint8_t  actual_gear;
	uint32_t shift_start_tick;
	uint32_t timestamp_tick;
	uint16_t start_gear_sens;
	uint8_t start_gear;

	uint8_t gear_cut;

}Gearbox_t;

	void gearboxInit(Gearbox_t* gearbox);

	void updateGearADC(Gearbox_t* gearbox, uint16_t ADCValue);
	void updateGear(Gearbox_t* gearbox);
	uint8_t getGear(Gearbox_t* gerbox);
	uint8_t getGearCut(Gearbox_t* gerbox);

	void processCallback(Gearbox_t* gearbox);
	void startUpshiftCallback(Gearbox_t* gearbox);
	void upshiftFromNeutral(Gearbox_t* gearbox);
	void upshiftGearCut(Gearbox_t* gearbox);
	void upshiftFinish(Gearbox_t* gearbox);

	void startDownshiftCallback(Gearbox_t* gearbox);
	void downshiftToNeutral(Gearbox_t* gearbox);
	void downshiftRevMatch(Gearbox_t* gearbox);
	void downshiftFinish(Gearbox_t* gearbox);

	void openLoop_upshift_gearCut(Gearbox_t* gearbox);
	void openLoop_upshift_finish(Gearbox_t* gearbox);

	void openLoop_downshift_clutch(Gearbox_t* gearbox);
	void openLoop_downshift_finish(Gearbox_t* gearbox);

	void startUpshift(Gearbox_t* gearbox);
	void startDownshift(Gearbox_t* gearbox);


//	void openLoopUpshift(void);
//	void openLoopDownshift(void);
//
//	void closedLoopUpshift(void);
//	void closedLoopDownshift(void);
	void clutch(bool ON);

	GearboxState_t getState(Gearbox_t* gearbox);
	bool isShiftinProgress(Gearbox_t*gearbox);


#endif /* INC_GEARBOX_H_ */