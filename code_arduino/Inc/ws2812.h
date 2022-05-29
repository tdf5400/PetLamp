#ifndef __WS2812_H
#define __WS2812_H

void ws2812_init(void);
void ws2812_show();
void ws2812_setPixel(unsigned char index, unsigned char *color);
void ws2812_closeAll(void);
void ws2812_setBright(unsigned char bright);






#endif
