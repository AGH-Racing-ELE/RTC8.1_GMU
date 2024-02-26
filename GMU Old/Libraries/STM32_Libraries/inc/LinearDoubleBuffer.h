#ifndef INC_LINEARDOUBLEBUFFER_H_
#define INC_LINEARDOUBLEBUFFER_H_

#include "stdint.h"
#include "LinearBuffer.h"

template<class T, uint16_t SIZE>
class LinearDoubleBuffer
{
private:
	volatile LinearBuffer<T, SIZE> _buff[2];
	volatile uint8_t _active_buff = 0;
	volatile uint8_t _busy_buff = 1;

private:
	void switchBuffers(void) volatile {
		_busy_buff = _active_buff;
		_active_buff = !_busy_buff;
		_buff[_active_buff].clear();
	}

public:
	LinearDoubleBuffer() {}
	~LinearDoubleBuffer() {}

	volatile T* getReadPtr(void) volatile
	{
		switchBuffers();
		return _buff[_busy_buff].getReadPtr();
	}
	volatile T* getWritePtr(void) volatile { return _buff[_active_buff].getWritePtr(); }
	bool increaseWritePtr(uint16_t value) volatile { return _buff[_active_buff].increaseWritePtr(value); }
	uint16_t getCount(void)	volatile { return _buff[_active_buff].getCount(); }
	uint16_t getFreeSpace(void) volatile { return (uint16_t)(SIZE - _buff[_active_buff].getCount()); }
	void clear(void) volatile { _buff[_active_buff].clear(); }

	bool appendData(T *data, uint16_t size) volatile { return _buff[_active_buff].appendData(data, size); }
};

#endif /* INC_LINEARDOUBLEBUFFER_H_ */
