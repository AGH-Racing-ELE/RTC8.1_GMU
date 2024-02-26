#ifndef CHANNELSBASE_H_
#define CHANNELSBASE_H_

#include <stdint.h>
#include "Timing.h"

enum class ChanIndex : uint8_t;
inline ChanIndex operator+(const ChanIndex &a, const uint32_t &b) { return (ChanIndex)((uint8_t)a + b); }

template<uint8_t CHAN_COUNT>
class ChannelsBase
{
// Types
protected:
	struct Channel {
		uint32_t value = 0;

		char name[20];
		char unit[6];

		uint16_t multipier = 1;
		uint16_t divider = 1;
		uint16_t offset = 0;

		uint16_t timeout_value = 0;
		uint16_t timeout_period = 0;
		uint32_t last_update = 0;
	};

// Members
protected:
	Channel chan_list[CHAN_COUNT];

protected:
	void timeoutCheck(void)
	{
		for(uint8_t i = 0; i < CHAN_COUNT; ++i) {
			if(chan_list[i].timeout_period == 0) continue;
			if(Timing::getElapsedMs(chan_list[i].last_update) > chan_list[i].timeout_period) {
				chan_list[i].value = chan_list[i].timeout_value;
				chan_list[i].last_update = Timing::getMilisecTick();
			}
		}
	}

// Methods
public:
	ChannelsBase() {};
	virtual ~ChannelsBase() {};

	virtual void init(void) {};
	void setValue(ChanIndex chan_idx, uint32_t value)
	{
		chan_list[(uint8_t)chan_idx].value = value;
		chan_list[(uint8_t)chan_idx].last_update = Timing::getMilisecTick();
	}
	void setValue(uint8_t chan_idx, uint32_t value) { setValue((ChanIndex)chan_idx, value); }
	void incrementValue(ChanIndex chan_idx)
	{
		++(chan_list[(uint8_t)chan_idx].value);
		chan_list[(uint8_t)chan_idx].last_update = Timing::getMilisecTick();
	}
	uint32_t getValue(ChanIndex chan_idx) const { return chan_list[(uint8_t)chan_idx].value; }
	uint32_t getValue(uint8_t chan_idx) const { return chan_list[chan_idx].value; }
	uint16_t getMultipier(ChanIndex chan_idx) const { return chan_list[(uint8_t)chan_idx].multipier; }
	uint16_t getDivider(ChanIndex chan_idx) const { return chan_list[(uint8_t)chan_idx].divider; }
	const char * getUnit(ChanIndex chan_idx) const { return chan_list[(uint8_t)chan_idx].unit; }
};

#endif /* CHANNELSBASE_H_ */
