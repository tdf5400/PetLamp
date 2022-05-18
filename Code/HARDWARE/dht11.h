#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

// IO - A0

#define DHT_Dire_I() {GPIOA->CRL &= 0xFFFFFFF0; GPIOA->CRL |= 8;}
#define DHT_Dire_O() {GPIOA->CRL &= 0xFFFFFFF0; GPIOA->CRL |= 3;}

#define DHT_IO_O PAout(0)
#define DHT_IO_I PAin(0)
#define DHT_DAT_Pin GPIO_Pin_0
#define DHT_PORT GPIOA

typedef struct{
	uint8_t temp;
	uint8_t humi;
}DHT11_DataDef;

extern void dht11_init(void);
extern uint8_t dht11_re(DHT11_DataDef* output);

#endif
