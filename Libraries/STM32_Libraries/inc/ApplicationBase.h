#ifndef APPLICATIONBASE_H_
#define APPLICATIONBASE_H_

#include <stdint.h>
#include "CircularBuffer.h"
#include "Timing.h"

enum class PerCbIndex : uint8_t;
enum class EvCbIndex : uint8_t;

template<uint8_t PER_CB_COUNT, uint8_t EV_CB_COUNT, uint16_t EV_QU_SIZE>
class ApplicationBase
{
// Types
public:
	typedef void (*PerCallbackType)(void);
	typedef void (*EvCallbackType)(uint8_t);

	struct Event
	{
		uint8_t index;
		uint8_t param;

		void operator=(volatile Event &other) volatile
		{
			index = other.index;
			param = other.param;
		}
	};

// Methods
public:
	ApplicationBase()
	{}

	virtual ~ApplicationBase()
	{}

	virtual void init(void)
	{}

	void start(void)
	{
		while(1) {
			uint32_t tot_cyc_ts = Timing::getCyclesTick();

			processPeriodicCallbacks();
			processEventCallbacks();

			_perf_data.total_cycles += Timing::getElapsedCyc(tot_cyc_ts);
			_perf_data.scaleCycles();
		}
	}

	void setPeriodicCallback(PerCbIndex idx, int16_t period)
	{
		uint8_t i = (uint8_t)idx;
		if(i >= PER_CB_COUNT) return;

		_periodicCallback_list[i].period = period;
	}

	void setPeriodicCallback(PerCbIndex idx, PerCallbackType callback)
	{
		uint8_t i = (uint8_t)idx;
		if(i >= PER_CB_COUNT) return;

		_periodicCallback_list[i].callback = callback;
	}

	void setPeriodicCallback(PerCbIndex idx, int16_t period, PerCallbackType callback)
	{
		uint8_t i = (uint8_t)idx;
		if(i >= PER_CB_COUNT) return;

		_periodicCallback_list[i].period = period;
		_periodicCallback_list[i].callback = callback;
	}

	void setEventCallback(EvCbIndex idx, EvCallbackType callback)
	{
		uint8_t i = (uint8_t)idx;
		if(i >= EV_CB_COUNT) return;

		_eventCallback_list[i].index = i;
		_eventCallback_list[i].callback = callback;
	}

	void raiseEvent(EvCbIndex idx, const uint8_t &param)
	{
		Event e;
		e.index = (uint8_t)idx;
		e.param = param;

		_event_queue.write(&e);
	}

// Types
private:
	struct PeriodicCallback
	{
		uint8_t index = PER_CB_COUNT;
		int16_t period = 0;
		PerCallbackType callback = nullptr;
		uint32_t timestamp = 0;
	};

	struct EventCallback
	{
		uint8_t index = EV_CB_COUNT;
		EvCallbackType callback = nullptr;
	};

public:
	struct PerfData
	{
		uint32_t total_cycles;

		uint32_t total_per_cyc;
		uint32_t total_ev_cyc;

		uint32_t per_cycles[PER_CB_COUNT];
		uint32_t ev_cycles[EV_CB_COUNT];

		void scaleCycles(void)
		{
			if(total_cycles > 500000000) {
				total_cycles >>= 3;
				total_per_cyc >>= 3;
				total_ev_cyc >>= 3;

				for(uint8_t i = 0; i < PER_CB_COUNT; ++i) {
					per_cycles[i] >>= 3;
				}
				for(uint8_t i = 0; i < EV_CB_COUNT; ++i) {
					ev_cycles[i] >>= 3;
				}
			}
		}

		void calcTotalPer(void)
		{
			uint32_t total = 0;
			for(uint8_t i = 0; i < PER_CB_COUNT; ++i) {
				total += per_cycles[i];
			}
			total_per_cyc = total;
		}

		void calcTotalEv(void)
		{
			uint32_t total = 0;
			for(uint8_t i = 0; i < EV_CB_COUNT; ++i) {
				total += ev_cycles[i];
			}
			total_ev_cyc = total;
		}

	};

// Members
private:
	PeriodicCallback _periodicCallback_list[PER_CB_COUNT];
	uint8_t _periodicCallback_count = 0;

	EventCallback _eventCallback_list[EV_CB_COUNT];
	volatile CircularBuffer<Event, EV_QU_SIZE> _event_queue;

public:
	PerfData _perf_data;

// Methods
private:

	void processPeriodicCallbacks(void)
	{
		for(uint8_t i = 0; i < PER_CB_COUNT; ++i) {
			if(_periodicCallback_list[i].period < 0) continue;
			if(_periodicCallback_list[i].callback == nullptr) continue;

			uint32_t curr_timestamp = Timing::getMilisecTick();
			uint32_t elapsed_time = Timing::getElapsedTime(_periodicCallback_list[i].timestamp, curr_timestamp);
			if(elapsed_time >= (uint16_t)_periodicCallback_list[i].period) {
				uint32_t per_ts = Timing::getCyclesTick();
				_periodicCallback_list[i].timestamp = curr_timestamp;
				_periodicCallback_list[i].callback();
				_perf_data.per_cycles[i] += Timing::getElapsedCyc(per_ts);
			}
		}

		_perf_data.calcTotalPer();
	}

	void processEventCallbacks(void)
	{
		uint16_t count = _event_queue.getCount();

		while(count) {
			Event event;
			_event_queue.read(&event);

			if(_eventCallback_list[(uint8_t)event.index].callback != nullptr) {
				uint32_t ev_ts = Timing::getCyclesTick();
				_eventCallback_list[(uint8_t)event.index].callback(event.param);
				_perf_data.ev_cycles[(uint8_t)event.index] += Timing::getElapsedCyc(ev_ts);
			}

			count = _event_queue.getCount();
		}

		_perf_data.calcTotalEv();
	}

};

#endif /* APPLICATIONBASE_H_ */
