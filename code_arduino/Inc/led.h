#ifndef __LED_H
#define __LED_H


#define LED_on() 	digitalWrite(PC13, LOW)
#define LED_off()	digitalWrite(PC13, HIGH)

extern void led_init(void);


#endif
