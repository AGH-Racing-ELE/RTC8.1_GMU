#include "Utilities.h"

uint16_t Utilities::Arrays::getMinValIndex(uint8_t *array, uint16_t size)
{
	uint16_t min_idx = 0;
	uint8_t min_val = array[0];

	for(uint8_t i = 1; i < size; ++i) {

		if(array[i] < min_val) {
			min_val = array[i];
			min_idx = i;
		}

	}

	return min_idx;
}
