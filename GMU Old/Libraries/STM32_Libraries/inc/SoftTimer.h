#ifndef SOFTTIMER_H_
#define SOFTTIMER_H_

#include <stdint.h>

class SoftTimer
{
public:

	void start(void);
	void stop(void);
	bool isStarted(void) { return started; }
	uint32_t getElapsedMs(void);

private:
	bool started = false;
	uint32_t timestamp = 0;

};

#endif /* SOFTTIMER_H_ */
