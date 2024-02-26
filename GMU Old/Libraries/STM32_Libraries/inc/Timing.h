#ifndef INC_TIMING_H_
#define INC_TIMING_H_

#include <stdint.h>

namespace Timing
{
#if defined(STM32F303xC)
	void init(void);
#endif

	uint32_t getMilisecTick(void);
	uint32_t getCyclesTick(void);

	uint32_t getElapsedTime(uint32_t prev_timestamp, uint32_t curr_timestamp, uint32_t max_time = UINT32_MAX);
	uint32_t getElapsedMs(uint32_t prev_timestamp);
	uint32_t getElapsedCyc(uint32_t prev_timestamp);
}



#endif /* INC_TIMING_H_ */
