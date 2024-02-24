#include "Timing.h"

#if defined(STM32F042x6)

#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "tim.h"

#ifndef TIMING_CYC_TIMER
#error "Timing:: cycles timer not defined!"
#endif

#if !defined(TIMING_RES) && TIMING_RES != 16 && TIMING_RES != 32
#error "Timing:: timer resolution not defined!"
#endif

#if TIMING_RES == 16
static volatile uint32_t cycles_count = 0;
#endif

#elif defined(STM32F303xC)

#include "stm32f3xx.h"
#include "core_cm4.h"

#else
#error "Timing: MCU not defined!"
#endif

namespace Timing
{
#if defined(STM32F303xC)
	void init(void)
	{
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CYCCNT = 0;
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	}
#endif

	uint32_t getMilisecTick(void)
	{
		return HAL_GetTick();
	}

	uint32_t getCyclesTick(void)
	{
#if defined(STM32F042x6)
#if TIMING_RES == 16
		return cycles_count + TIMING_CYC_TIMER.Instance->CNT;
#else
		return TIMING_CYC_TIMER->CNT;
#endif
#elif defined(STM32F303xC)
		return DWT->CYCCNT;
#endif
	}

	uint32_t getElapsedTime(uint32_t prev_timestamp, uint32_t curr_timestamp, uint32_t max_time /*= UINT32_MAX*/)
	{
		if(curr_timestamp >= prev_timestamp) {
			return curr_timestamp - prev_timestamp;
		}
		else {
			return max_time - prev_timestamp + 1 + curr_timestamp;
		}
	}

	uint32_t getElapsedMs(uint32_t prev_timestamp)
	{
		return getElapsedTime(prev_timestamp, getMilisecTick());
	}

	uint32_t getElapsedCyc(uint32_t prev_timestamp)
	{
		return getElapsedTime(prev_timestamp, getCyclesTick());
	}

#if TIMING_TIM_RES == 16

void timerCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &TIMING_CYC_TIM) {
		cycles_count += (1 << 16);
	}
}

#endif

}
