/*
 * FIFO.h
 *
 *  Created on: 30.11.2018
 *      Author: Pawe³
 */


#ifndef CAN_FIFO_H_
#define CAN_FIFO_H_

#include <sys/_stdint.h>
#define CAN_FIFO_SIZE 100

struct CAN_FIFO{

	uint8_t first_element;
	uint8_t last_element;
	uint8_t* data_ptr[CAN_FIFO_SIZE];
	uint8_t empty;
};

void CAN_FIFO_Add(struct CAN_FIFO*, uint8_t*);
uint8_t* CAN_FIFO_Get(struct CAN_FIFO*);
void CAN_FIFO_Flush(struct CAN_FIFO*);

#endif /* FIFO_H_ */
