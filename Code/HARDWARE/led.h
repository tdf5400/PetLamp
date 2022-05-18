#ifndef __LED_H
#define __LED_H

#define LED PCout(13)

#define LED_on() 	LED=0
#define LED_off()	LED=1

extern void led_init(void);


#endif
