#ifndef GEARBOX_H_
#define GEARBOX_H_

#include <stdint.h>

class Gearbox
{
private:
	struct ShiftData {
		uint32_t shift_start_tick = 0;
		uint32_t timestamp_tick = 0;
		uint16_t start_gear_sens = 0;
		uint8_t start_gear = 0;
	};

public:
	enum class State : uint8_t {
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
		OL_DS_Finish,
	};

// Members
private:
	    static const uint16_t neutral_adc = ;
	    static const uint16_t gear1_adc   = 330;
	    static const uint16_t gear2_adc   = 750;
	    static const uint16_t gear3_adc   = 1100;
	    static const uint16_t gear4_adc   = 1300;


	static const uint16_t _G_MAP[];

	static const uint16_t _US_FROM_NEUTRAL_TIMEOUT = 200;
	static const uint16_t _US_ACTUATOR_DELAY = 20; //previous 20
	static const uint16_t _US_FINISH_TIMEOUT = 200;

	static const uint16_t _DS_ACTUATOR_DELAY = 40;    // previous value = 40
	//static const uint16_t _DS_PARTIAL_TIMEOUT = 200;
	static const uint16_t _DS_FINISH_TIMEOUT = 250;  // 250

	static const uint16_t _OL_US_GEARCUT_DELAY = 100;
	static const uint16_t _OL_DS_CLUTCH_DELAY = 100;

	State _state = State::Idle;
	ShiftData _upshift_data;
	ShiftData _downshift_data;

private:
	void processCallback(void);
	void startUpshiftCallback(void);
	void upshiftFromNeutral(void);
	void upshiftGearCut(void);
	void upshiftFinish(void);

	void startDownshiftCallback(void);
	void downshiftToNeutral(void);
	void downshiftRevMatch(void);
	void downshiftFinish(void);

	void openLoop_upshift_gearCut(void);
	void openLoop_upshift_finish(void);

	void openLoop_downshift_clutch(void);
	void openLoop_downshift_finish(void);

// Methods
public:
	void updateGearChannel(void);

	void startUpshift(void);
	void startDownshift(void);

//	void openLoopUpshift(void);
//	void openLoopDownshift(void);
//
//	void closedLoopUpshift(void);
//	void closedLoopDownshift(void);
	void clutch(bool ON);

	State getState(void) { return _state; }
	bool isShiftInProgress(void) { return _state != State::Idle; }

	static void process(void);
};

extern Gearbox theGearbox;

#endif /* GEARBOX_H_ */
