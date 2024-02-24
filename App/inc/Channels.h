#ifndef CHANNELS_H_
#define CHANNELS_H_

#include "ChannelsBase.h"

enum class ChanIndex : uint8_t {
	GearPositionADC,
	Gear,
	UpshiftTime,
	DownshiftTime,
	AirPressureADC,
	AirPressure,
	ResetDetector,
	GearCut,
	CHAN_COUNT
};

class Channels :
		public ChannelsBase<(uint8_t)ChanIndex::CHAN_COUNT>
{
private:
	void processCallback(void);

public:
	Channels() {}
	~Channels() {}

	void init(void);
	static void process(void);

};

extern Channels theChannels;

#endif /* CHANNELS_H_ */
