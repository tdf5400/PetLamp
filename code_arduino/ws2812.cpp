	/**
  * @brief 	ws2812����
	* @ps   	ʹ������: A7-MOSI
  */

#include "./Inc/ws2812.h"
#include <WS2812B.h>
#define uint8_t unsigned char

#define NUM_LEDS 128
static WS2812B strip = WS2812B(NUM_LEDS);
uint8_t DMA_send_buff[128*3];


void ws2812_init(void){
	/**
  * @brief 	ws2812 ��ʼ��
  * @param 	None
	* @ps   	ʹ������: A7-MOSI
  */
	strip.begin();
	strip.show();
	ws2812_setBright(40);
}


void ws2812_show(){
	/**
	* @brief ��Ļ��ʾд��
	*/
	strip.show();
}


void ws2812_setPixel(uint8_t index, uint8_t *color){
	/**
	 * @brief ����ĳ���ص�
	 * 
	 */
	strip.setPixelColor(index, strip.Color(color[0], color[1], color[2]));
}


void ws2812_closeAll(void){
	/**
	* @brief �ر����е���
	*/
	strip.clear();
}



void ws2812_setBright(uint8_t bright){
	strip.setBrightness(bright);
}
