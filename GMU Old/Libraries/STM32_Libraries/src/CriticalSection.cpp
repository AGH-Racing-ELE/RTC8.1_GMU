#include "CriticalSection.h"

CriticalSection::CriticalSection(bool start)
{
	started = false;
	if(start) this->start();
}

CriticalSection::CriticalSection() :
		CriticalSection(false)
{
}

CriticalSection::~CriticalSection()
{
	if(started) {
		asm volatile(
				"LDRB R0, %[input]\n\t"
				"MSR PRIMASK, R0;\n\t"
				::[input] "m" (_cpu_sr) : "r0");
	}
}

void CriticalSection::start(void)
{
	asm volatile(
			"MRS   R0, PRIMASK\n\t"
			"CPSID I\n\t"
			"STRB R0, %[output]"
			: [output] "=m" (_cpu_sr) :: "r0", "memory");

	started = true;
}
