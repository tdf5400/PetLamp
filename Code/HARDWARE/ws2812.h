#ifndef __WS2812_H
#define __WS2812_H

#include "stm32f10x.h"

#define SPI_DMA_SWITCH 1

extern void ws2812_init(void);
extern void ws2812_sendData(uint8_t* data, uint16_t len);
extern void ws2812_freTest(void);


#if SPI_DMA_SWITCH
extern uint8_t DMA_send_buff[128*3*8];
extern void ws2812_show128(uint8_t *dataList);
extern void ws2812_closeAll(void);
#endif	


#endif
