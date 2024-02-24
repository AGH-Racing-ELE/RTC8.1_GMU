#ifndef INC_STMSTUDIOHELPER_H_
#define INC_STMSTUDIOHELPER_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"

typedef struct
{
	volatile uint16_t *s1_adc_ptr;
	volatile uint16_t *s2_adc_ptr;
	volatile uint16_t *s3_adc_ptr;
	volatile uint16_t *s4_adc_ptr;

} SSH_WheelSpeed;

extern volatile SSH_WheelSpeed theSSH_WheelSpeed;

#ifdef __cplusplus
}
#endif
#endif /* INC_STMSTUDIOHELPER_H_ */
