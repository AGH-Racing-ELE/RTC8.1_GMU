#include "Application.h"
#include "CAN_Handler.h"
#include "gpio.h"
#include "Channels.h"
#include "Gearbox.h"
#include "ADC_Handler.h"

Application theApplication;

void Application::init(void)
{

	Timing::init();
	theChannels.init();
	theADC.init();
	CAN_Handler_Init();

	theChannels.setValue(ChanIndex::ResetDetector, 255);
	theChannels.setValue(ChanIndex::GearCut, (uint8_t)0);

	setPeriodicCallback(PerCbIndex::CanRx, 1, Application::canRxProcess);
	setPeriodicCallback(PerCbIndex::Gearbox, 1, Gearbox::process);
	setPeriodicCallback(PerCbIndex::Channels,5, Channels::process);
	setPeriodicCallback(PerCbIndex::SendChann,2, Application::sendChann);
	setPeriodicCallback(PerCbIndex::StatusLED, 1000, Application::statusLED);
}

void Application::canRxProcess(void)
{

	uint8_t clutch_data;

	if(CAN_Handler_IsGearUpCommanded()){
		theGearbox.startUpshift();
		return;
	}
	if(CAN_Handler_IsGearDownCommanded()){
		theGearbox.startDownshift();
		return;
	}
	if(CAN_Handler_IsClutchCommanded(&clutch_data)){
		if(clutch_data == 255){
			theGearbox.clutch(true);
		}
		else if(clutch_data == 0){
			theGearbox.clutch(false);
		}
		return;
	}

}

void Application::sendChann(void)
{
	uint16_t adc = (uint16_t)theChannels.getValue(ChanIndex::GearPositionADC);
	uint8_t gear = (uint8_t)(theChannels.getValue(ChanIndex::Gear) & 0xFF);
	uint8_t ap = (uint8_t)(theChannels.getValue(ChanIndex::AirPressure) & 0xFF);
	uint16_t upshift_time = (uint16_t)theChannels.getValue(ChanIndex::UpshiftTime);
	uint16_t downshift_time = (uint16_t)theChannels.getValue(ChanIndex::DownshiftTime);
	uint8_t gear_cut = (uint8_t)theChannels.getValue(ChanIndex::GearCut);

	CAN_Handler_SendGmu1Frame(gear, 0, adc, (uint8_t)theGearbox.getState(), ap, (uint8_t)(theChannels.getValue(ChanIndex::ResetDetector)), (uint8_t)gear_cut);
	CAN_Handler_SendGmu2Frame(0, 0, upshift_time, downshift_time);
}

void Application::statusLED(void)
{
	HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
}

void Application::systick_callback(void)
{
	uint8_t rd = (uint8_t)theChannels.getValue(ChanIndex::ResetDetector);
	if(rd > 0 && (Timing::getMilisecTick() % 10 == 0)) {
		theChannels.setValue(ChanIndex::ResetDetector, --rd);
	}
}
