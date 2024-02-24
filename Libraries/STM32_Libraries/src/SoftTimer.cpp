#include "SoftTimer.h"
#include "stm32f3xx.h"

void SoftTimer::start(void)
{
	started = true;
	timestamp = HAL_GetTick();
}

void SoftTimer::stop(void)
{
	started = false;
}

uint32_t SoftTimer::getElapsedMs(void)
{
	return HAL_GetTick() - timestamp;
}
