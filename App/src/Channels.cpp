#include <Channels.h>
#include "string.h"

Channels theChannels;

void Channels::init(void)
{
	strcpy(chan_list[(uint8_t)ChanIndex::GearPositionADC].name, "GearPosADC");
	strcpy(chan_list[(uint8_t)ChanIndex::Gear].name, "Gear");
	strcpy(chan_list[(uint8_t)ChanIndex::UpshiftTime].name, "UpshiftTime");
	strcpy(chan_list[(uint8_t)ChanIndex::DownshiftTime].name, "DownshiftTime");
	strcpy(chan_list[(uint8_t)ChanIndex::ResetDetector].name, "ResetDetector");
	strcpy(chan_list[(uint8_t)ChanIndex::GearCut].name, "GearCut");
}

void Channels::processCallback(void)
{

}

void Channels::process(void)
{
	theChannels.timeoutCheck();
	theChannels.processCallback();
}
