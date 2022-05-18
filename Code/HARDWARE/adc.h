#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

extern void adc_init(void);
extern uint16_t adc_re(void);
extern uint16_t adc_re_ave(uint8_t times);

#endif
