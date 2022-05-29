#ifndef __ADC_H
#define __ADC_H


extern void adc_init(void);
extern unsigned short adc_re_light(void);
extern unsigned short adc_re_vol(void);

#define ADC_LIGHT_LOW 1
#define ADC_LIGHT_MID 2
#define ADC_LIGHT_HIGH 3

#endif
