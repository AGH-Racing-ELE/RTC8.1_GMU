#ifndef INC_CRITICALSECTION_H_
#define INC_CRITICALSECTION_H_

#include <stdint.h>

class CriticalSection
{
public:
	CriticalSection(bool start);
	CriticalSection();
	~CriticalSection();

	void start(void);

private:
	bool started;
	volatile uint8_t _cpu_sr;
};

#endif /* INC_CRITICALSECTION_H_ */
