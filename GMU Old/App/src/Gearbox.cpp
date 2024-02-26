#include "Gearbox.h"
#include "main.h"
#include "stm32f3xx.h"
#include "Channels.h"

Gearbox theGearbox;

const uint16_t Gearbox::_G_MAP[] = { neutral_adc-100, neutral_adc+100, gear1_adc-150, gear1_adc+150, gear2_adc-200, gear2_adc+200, gear3_adc-300, gear3_adc+300, gear4_adc-350, gear4_adc+350};
//const uint16_t Gearbox::_G_MAP[] = {gear4_adc+350, gear4_adc-350,  gear3_adc+300, gear3_adc-300,gear2_adc+200,gear2_adc-200,gear1_adc+150,gear1_adc-150,neutral_adc+100,neutral_adc-100};


void Gearbox::processCallback(void)
{
	switch (_state) {
		case State::Idle:
			break;
		case State::StartUpshift:
			startUpshiftCallback();
			break;
		case State::UpshiftFromNeutral:
			upshiftFromNeutral();
			break;
		case State::UpshiftGearCut:
			upshiftGearCut();
			break;
		case State::UpshiftFinish:
			upshiftFinish();
			break;
		case State::StartDownshift:
			startDownshiftCallback();
			break;
		case State::DownshiftToNeutral:
			downshiftToNeutral();
			break;
		case State::DownshiftRevMatch:
			downshiftRevMatch();
			break;
		case State::DownshiftFinish:
			downshiftFinish();
			break;
		case State::OL_US_GearCut:
			openLoop_upshift_gearCut();
			break;
		case State::OL_US_Finish:
			openLoop_upshift_finish();
			break;
		case State::OL_DS_Clutch:
			openLoop_downshift_clutch();
			break;
		case State::OL_DS_Finish:
			openLoop_downshift_finish();
			break;
		default:
			_state = State::Idle;
			break;
	}
}

void Gearbox::startUpshiftCallback(void)
{
	_upshift_data.shift_start_tick = Timing::getMilisecTick();
	_upshift_data.start_gear = (uint8_t)theChannels.getValue(ChanIndex::Gear);
	_upshift_data.start_gear_sens = (uint16_t)theChannels.getValue(ChanIndex::GearPositionADC);

	//tryb awaryjny jak nie dziala odczyt bieg�w
	if(_upshift_data.start_gear > 5)
	{
		HAL_GPIO_WritePin(GEAR_UP_GPIO_Port, GEAR_UP_Pin, GPIO_PIN_SET);
		_upshift_data.timestamp_tick = Timing::getMilisecTick();
		_state = State::OL_US_GearCut;
		return;
	}

	if(_upshift_data.start_gear == 4)
	{
		_state = State::Idle;
		return;
	}

	HAL_GPIO_WritePin(GEAR_UP_GPIO_Port, GEAR_UP_Pin, GPIO_PIN_SET);
	_upshift_data.timestamp_tick = Timing::getMilisecTick();

	if(_upshift_data.start_gear == 0) {
		_state = State::UpshiftFromNeutral;
	} else {
		_state = State::UpshiftGearCut;
	}
}

void Gearbox::upshiftFromNeutral(void)
{
	bool first_gear_in = (uint8_t)theChannels.getValue(ChanIndex::Gear) == 1;
	bool timeout = Timing::getElapsedMs(_upshift_data.timestamp_tick) > _US_FROM_NEUTRAL_TIMEOUT;
	if(first_gear_in || timeout) {
		HAL_GPIO_WritePin(GEAR_UP_GPIO_Port, GEAR_UP_Pin, GPIO_PIN_RESET);
		theChannels.setValue(ChanIndex::UpshiftTime, (uint16_t)Timing::getElapsedMs(_upshift_data.shift_start_tick));
		_state = State::Idle;
	}
}

void Gearbox::upshiftGearCut(void)
{
	bool delayed = Timing::getElapsedMs(_upshift_data.timestamp_tick) > _US_ACTUATOR_DELAY;
	if(delayed) {
		HAL_GPIO_WritePin(GEAR_CUT_GPIO_Port, GEAR_CUT_Pin, GPIO_PIN_SET);
		_upshift_data.timestamp_tick = Timing::getMilisecTick();
		theChannels.setValue(ChanIndex::GearCut, (uint8_t)255);
		_state = State::UpshiftFinish;
	}
}

void Gearbox::upshiftFinish(void)
{
	bool next_gear_in = theChannels.getValue(ChanIndex::Gear) == (uint16_t)(_upshift_data.start_gear + 1);
	bool timeout = Timing::getElapsedMs(_upshift_data.timestamp_tick) > _US_FINISH_TIMEOUT;
	if(next_gear_in || timeout) {
		theChannels.setValue(ChanIndex::UpshiftTime, (uint16_t)Timing::getElapsedMs(_upshift_data.shift_start_tick));
		HAL_GPIO_WritePin(GEAR_CUT_GPIO_Port, GEAR_CUT_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GEAR_UP_GPIO_Port, GEAR_UP_Pin, GPIO_PIN_RESET);
		theChannels.setValue(ChanIndex::GearCut, (uint8_t)127);
		_state = State::Idle;
	}
}

void Gearbox::startDownshiftCallback(void)
{
	_downshift_data.shift_start_tick = Timing::getMilisecTick();
	_downshift_data.start_gear = (uint8_t)theChannels.getValue(ChanIndex::Gear);
	_downshift_data.start_gear_sens = (uint16_t)theChannels.getValue(ChanIndex::GearPositionADC);

	if(_downshift_data.start_gear > 4)
	{
		HAL_GPIO_WritePin(GEAR_DOWN_GPIO_Port, GEAR_DOWN_Pin, GPIO_PIN_SET);
		_upshift_data.timestamp_tick = Timing::getMilisecTick();
		_state = State::OL_DS_Clutch;
		return;
	}

	if(_downshift_data.start_gear == 0)
	{
		_state = State::Idle;
		return;
	}

	HAL_GPIO_WritePin(GEAR_DOWN_GPIO_Port, GEAR_DOWN_Pin, GPIO_PIN_SET);
	_downshift_data.timestamp_tick = Timing::getMilisecTick();
	if(_downshift_data.start_gear == 1)	_state = State::DownshiftToNeutral;
	else _state = State::DownshiftRevMatch;
}

void Gearbox::downshiftToNeutral(void)
{
	if(Timing::getElapsedMs(_downshift_data.timestamp_tick) > _DS_ACTUATOR_DELAY) {
		HAL_GPIO_WritePin(CLUTCH_GPIO_Port, CLUTCH_Pin, GPIO_PIN_SET);
		_downshift_data.timestamp_tick = Timing::getMilisecTick();
		_state = State::DownshiftFinish;
	}
}

void Gearbox::downshiftRevMatch(void){
	if(Timing::getElapsedMs(_downshift_data.timestamp_tick) > _DS_ACTUATOR_DELAY) {
		HAL_GPIO_WritePin(CLUTCH_GPIO_Port, CLUTCH_Pin, GPIO_PIN_SET);
		_downshift_data.timestamp_tick = Timing::getMilisecTick();
		theChannels.setValue(ChanIndex::GearCut, (uint8_t)0);
		_state = State::DownshiftFinish;
	}
}

void Gearbox::downshiftFinish(void)
{
	bool prev_gear_in = theChannels.getValue(ChanIndex::Gear) == (uint16_t)(_downshift_data.start_gear - 1);
	bool timeout = Timing::getElapsedMs(_downshift_data.timestamp_tick) > _DS_FINISH_TIMEOUT;
	if(prev_gear_in || timeout) {
		//if( timeout) {
		theChannels.setValue(ChanIndex::DownshiftTime, (uint16_t)Timing::getElapsedMs(_downshift_data.shift_start_tick));
		HAL_GPIO_WritePin(CLUTCH_GPIO_Port, CLUTCH_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GEAR_DOWN_GPIO_Port, GEAR_DOWN_Pin, GPIO_PIN_RESET);
		theChannels.setValue(ChanIndex::GearCut, (uint8_t)127);
		_state = State::Idle;
	}
}

void Gearbox::openLoop_upshift_gearCut(void)
{
	if(Timing::getElapsedMs(_upshift_data.timestamp_tick) > _US_ACTUATOR_DELAY) {
		HAL_GPIO_WritePin(GEAR_CUT_GPIO_Port, GEAR_CUT_Pin, GPIO_PIN_SET);
		_upshift_data.timestamp_tick = Timing::getMilisecTick();
		_state = State::OL_US_Finish;
	}
}

void Gearbox::openLoop_upshift_finish(void)
{
	if(Timing::getElapsedMs(_upshift_data.timestamp_tick) > _OL_US_GEARCUT_DELAY) {
		HAL_GPIO_WritePin(GEAR_CUT_GPIO_Port, GEAR_CUT_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GEAR_UP_GPIO_Port, GEAR_UP_Pin, GPIO_PIN_RESET);
		theChannels.setValue(ChanIndex::UpshiftTime, _US_ACTUATOR_DELAY + _OL_US_GEARCUT_DELAY);
		_state = State::Idle;
	}
}

void Gearbox::openLoop_downshift_clutch(void)
{
	if(Timing::getElapsedMs(_downshift_data.timestamp_tick) > _DS_ACTUATOR_DELAY) {
		HAL_GPIO_WritePin(CLUTCH_GPIO_Port, CLUTCH_Pin, GPIO_PIN_SET);
		_downshift_data.timestamp_tick = Timing::getMilisecTick();
		_state = State::OL_DS_Finish;
	}
}

void Gearbox::openLoop_downshift_finish(void)
{
	if(Timing::getElapsedMs(_downshift_data.timestamp_tick) > _OL_DS_CLUTCH_DELAY) {
		HAL_GPIO_WritePin(CLUTCH_GPIO_Port, CLUTCH_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GEAR_DOWN_GPIO_Port, GEAR_DOWN_Pin, GPIO_PIN_RESET);
		theChannels.setValue(ChanIndex::DownshiftTime, _DS_ACTUATOR_DELAY + _OL_DS_CLUTCH_DELAY);
		_state = State::Idle;
	}
}

void Gearbox::updateGearChannel(void)
{
	uint16_t gear_adc = (uint16_t)theChannels.getValue(ChanIndex::GearPositionADC);

	if( gear_adc >=neutral_adc-150 && gear_adc <= neutral_adc +150) {
		theChannels.setValue(ChanIndex::Gear, 0);
	} else if( gear_adc >=gear1_adc-150 && gear_adc <= gear1_adc +150)  {
		theChannels.setValue(ChanIndex::Gear, 1);
	} else if( gear_adc >=gear2_adc-150 && gear_adc <= gear2_adc +150)  {
		theChannels.setValue(ChanIndex::Gear, 2);
	} else if( (gear_adc >=gear3_adc-150 && gear_adc <= gear3_adc +150)||(gear_adc >=320-150 && gear_adc <= 320 +150))  {
		theChannels.setValue(ChanIndex::Gear, 3);
	} else if( gear_adc >=gear4_adc-150 && gear_adc <= gear4_adc +150)  {
		theChannels.setValue(ChanIndex::Gear, 4);
	} else {
		theChannels.setValue(ChanIndex::Gear, 0xFFFF);
	}
}

void Gearbox::startUpshift(void)
{
	if(_state == State::Idle) {
		_state = State::StartUpshift;
	}
}

void Gearbox::startDownshift(void)
{
	if(_state == State::Idle) {
		_state = State::StartDownshift;
	}
}

void Gearbox::clutch(bool ON)
{
	HAL_GPIO_WritePin(CLUTCH_GPIO_Port, CLUTCH_Pin, ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Gearbox::process(void)
{
	theGearbox.processCallback();
}
