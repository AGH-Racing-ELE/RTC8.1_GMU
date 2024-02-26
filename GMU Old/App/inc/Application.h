#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdint.h>
#include "ApplicationBase.h"
#include "stm32f3xx.h"

static const uint16_t EV_QU_SIZE = 1024;

enum class PerCbIndex : uint8_t
{
	CanRx = 0u,
	Gearbox,
	Channels,
	SendChann,
	StatusLED,
	Debug,
	PER_CB_COUNT
};

enum class EvCbIndex : uint8_t
{
	GearSensADC = 0u,
	Debug,
	EV_CB_COUNT
};

class Application :
		public ApplicationBase<(uint8_t)PerCbIndex::PER_CB_COUNT, (uint8_t)EvCbIndex::EV_CB_COUNT, EV_QU_SIZE>
{
// Methods
public:
	Application() {};
	~Application() {};

	void init(void);
	void systick_callback(void);

// Methods
private:
	static void canRxProcess(void);
	static void sendChann(void);
	static void statusLED(void);
};

extern Application theApplication;

#endif /* APPLICATION_H_ */
