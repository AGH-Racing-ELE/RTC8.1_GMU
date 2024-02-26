#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <stdint.h>
#include <string.h>

#define CIRC_BUFF_DIAG

template<class T, uint16_t SIZE>
	class CircularBuffer
	{
	public:
		uint16_t getSize(void) const volatile
		{
			return SIZE;
		}

		uint16_t getCount(void) volatile
		{
			uint16_t count;

			uint16_t start_cpy = _start_idx;
			uint16_t end_cpy = _end_idx;

			if(end_cpy >= start_cpy) {
				count = (uint16_t)(end_cpy - start_cpy);
			}
			else {
				count = (uint16_t)((_BUFF_SIZE - start_cpy) + end_cpy);
			}

#ifdef CIRC_BUFF_DIAG
			if(count > _max_count)
			_max_count = count;
#endif
			return count;
		}

		bool isEmpty(void) const volatile
		{
			return _start_idx == _end_idx;
		}

		void clear(void) volatile
		{
			_start_idx = _end_idx;
		}

		void remove(uint16_t count) volatile
		{
			while(count--) {
				if(_start_idx == _end_idx) return;
				incrementIdx(&_start_idx);
			}
		}

		bool write(T *value, uint16_t count) volatile
		{
			if(value == nullptr) return false;

			if(count > (SIZE - getCount())) {
				// NOT ENOUGH SPACE
				_overflow = true;
				return false;
			}

			while(count--) {
				_buffer[_end_idx] = *value++;
				incrementIdx(&_end_idx);
			}

			return true;
		}

		bool write(T *value) volatile
		{
			return write(value, 1);
		}

		bool read(T* value, uint16_t count) volatile
		{
			if(value == nullptr) {
				return false;
			} else {
				memset(value, 0, sizeof(T) * count);
			}

			if(count > getCount()) {
				// NOT ENOUGH DATA
				return false;
			}

			while(count--) {
				*value++ = _buffer[_start_idx];
				incrementIdx(&_start_idx);
			}

			return true;
		}

		bool read(T *value) volatile
		{
			return read(value, 1);
		}

		volatile T * getWritePtr(bool increment = true) volatile
		{
			volatile T* ptr;

			if(getCount() < SIZE) {
				ptr = &_buffer[_end_idx];
				if(increment) incrementIdx(&_end_idx);
			}
			else {
				_overflow = true;
				ptr = nullptr;
			}

			return ptr;
		}

		int32_t search(const T &value, uint16_t offset) volatile
		{
			int32_t found_idx_from_offset = -1;
			if(offset < getCount()) {

				volatile uint16_t start_idx_copy = _start_idx;
				uint16_t index = offset;
				for(uint16_t i = 0; i < offset; ++i) {
					incrementIdx(&start_idx_copy);
				}

				while(index < getCount()) {
					if(_buffer[start_idx_copy] == value) {
						found_idx_from_offset = index;
						break;
					}

					incrementIdx(&start_idx_copy);
					++index;
				}
			}

			return found_idx_from_offset;
		}

		void incrementWriteIdx(void) volatile { incrementIdx(&_end_idx); }
		void incrementReadIdx(void) volatile { incrementIdx(&_start_idx); }

		bool isOverflow(void) volatile { return _overflow; }
		void resetOverflow(void) volatile { _overflow = false; }

		volatile T & operator[](uint16_t i) volatile
		{
			uint32_t idx = (_start_idx + i) % _BUFF_SIZE;
			return _buffer[idx];
		}

	private:

		const uint16_t _BUFF_SIZE = SIZE + 1;
		volatile T _buffer[SIZE + 1];
		volatile uint16_t _start_idx = 0;
		volatile uint16_t _end_idx = 0;
		volatile bool _overflow = false;

#ifdef CIRC_BUFF_DIAG
		volatile uint16_t _max_count = 0;
#endif

	private:
		void incrementIdx(volatile uint16_t *i) volatile
		{
			++(*i);
			if(*i >= _BUFF_SIZE) {
				*i = 0;
			}
		}
	};

#endif /* CIRCULARBUFFER_H_ */
