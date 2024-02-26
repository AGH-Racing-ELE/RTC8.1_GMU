#ifndef __LINEARBUFFER_H__
#define __LINEARBUFFER_H__

#include <stdint.h>

#define LINEAR_BUFF_DIAG

template<class T, uint16_t SIZE>
class LinearBuffer
{
public:
	volatile T* getReadPtr(void) volatile { return _buffer; }
	volatile T* getWritePtr(void) volatile { return _write_ptr; }
	uint16_t getCount(void)	volatile { return (uint16_t)(_write_ptr - _buffer); }
	uint16_t getFreeSpace(void) volatile { return (uint16_t)(SIZE - getCount()); }
	void clear(void) volatile { _write_ptr = _buffer; }

	bool appendData(T *data, uint16_t size) volatile
	{
		if(size > getFreeSpace()) {
			_overflow = true;
			return false;
		}

		for(uint16_t i = 0; i < size; ++i) {
			*_write_ptr++ = data[i];
		}

#ifdef LINEAR_BUFF_DIAG
		if(getCount() > _max_count) _max_count = getCount();
#endif

		return true;
	}

	bool increaseWritePtr(uint16_t value) volatile
	{
		if(value > getFreeSpace()) {
			_overflow = true;
			return false;
		}

		_write_ptr += value;

#ifdef LINEAR_BUFF_DIAG
		if(getCount() > _max_count) _max_count = getCount();
#endif

		return true;
	}

private:
	volatile T _buffer[SIZE];
	volatile T *_write_ptr = _buffer;
	volatile bool _overflow = false;
#ifdef LINEAR_BUFF_DIAG
	volatile uint16_t _max_count = 0;
#endif
};

#endif //__LINEARBUFFER_H__
