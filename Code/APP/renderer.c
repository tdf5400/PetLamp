/**
* @brief	������Ⱦ����
* @date		2022.03.13
*/

#include "renderer.h"
#include "ws2812.h"
#include <string.h>

/************* ��Ⱦ�� *************/
layer_object layer_background = {NULL, NULL};	// ����ͼ�㣨����ֻ��1����������û�������飩
layer_object layer_1 = {NULL, NULL};	// ͼ��1

void layer_draw(layer_object* layer){
	/**
	* @brief: ��Ⱦ���ͼ
	* @param: layer - layer_object, ��Ⱦ��
	* @param: object - ��Ⱦ����
	*/
	int i;
	int x, y;
	object_AI* object;
	for(i=0; i<2; i++){
		object = layer->objectList[i];
		if(object != NULL){
			// ״̬ͼƬ����
			if(object->action_state != NULL){
				if(object->pic != object->action_state->pic_data[0]) {
					object->pic = object->action_state->pic_data[0];
				}
				else {
					object->pic = object->action_state->pic_data[1];
				}
			}
			
			// ����ͼƬ������ʾ
			if(object->pic != NULL){
				x = object->locate[0];
				y = object->locate[1];
				if(object->direct){ // ��
					screen_drawPic_hFlip(x, y, object->pic);
				}else{							// ��
					screen_drawPic(x, y, object->pic);
				}
			}
			
		}
	}
}


void renderer_layerRendering(void){
	/**
	* @brief: ��Ⱦ����Ⱦ
	*/
//	uint8_t c[3] = color_white;//{0xff, 0x00, 0xa0};
//	uint8_t c1[3] = {0x0f, 0x0f, 0x0f};
//	uint8_t c2[3] = {0x01, 0x01, 0x01};
	
	screen_clear();
	layer_draw(&layer_background);
	layer_draw(&layer_1);
	
//	screen_drawPic(14, 0, &pic_drink_cold);
//	screen_setPoint(15, 7, c);
//	screen_setPoint(15, 6, c1);
//	screen_setPoint(15, 5, c2);
	screen_show();
}

/************* ��Ļ�� *************/
// ��Ļ���ݻ���
// .         ��    ��(127,7)
// .         ��    ��
// .         .    .
// .(0,0)    ��    ��
//          8~15  0~7
static unsigned char screen_cache[128][3];	// ��Ļ����
#define screen_select(x, y) screen_cache[127 - (x*8+y)]


void screen_init(void){
	/**
	* @brief: ��Ⱦģ���ʼ��
	*/
	ws2812_init();
}

void screen_clear(void){
	/**
	* @brief: ����
	* @ps: ����ɫΪ(0,0,0)
	*/
	memset(screen_cache, 0x00, 128*3);
}

int screen_setPoint(int x, int y, unsigned char *color){
	/**
	* @brief: �����
	* @param: x,y - ���꣨���½�Ϊ(0,0)��
	* @param: color - ��ɫ������Ϊ3������
	*/
	if((x<0 || x>15) || (y<0 || y>7))
		{return 1;}
	
	memcpy(screen_select(x, y), color, 3);		
	return 0;
}

void screen_drawPic(int x, int y, const object_pic* pic){
	/**
	* @brief: ͼƬ����
	* @param: x,y - ���꣨ͼƬ���½ǣ�
	* @param: pic - ͼƬ
	* @ps: �������ֲ������
	*/
	unsigned char i, j;
	unsigned char picLength = pic->w * pic->h - 1;
	signed char colorCode;

	for(i=0; i<pic->h; i++){
		for(j=0; j<pic->w; j++){
			colorCode = pic->picData[picLength - i * pic->w - j];
			if(colorCode != code_back){	// is not background
				screen_setPoint(x - j, y + i, (unsigned char *)colorList[colorCode]);
			}				
		}
	}
}

void screen_drawPic_hFlip(int x, int y, const object_pic* pic) {
	/**
	* @brief: ͼƬ����
	* @param: x,y - ���꣨ͼƬ���½ǣ�
	* @param: pic - ͼƬ
	* @ps: ��ɫ���ֲ������
	*/
	unsigned char i, j;
	unsigned char picLength = pic->w * pic->h - 1;
	signed char colorCode;

	for (i = 0; i < pic->h; i++) {
		for (j = 0; j < pic->w; j++) {
			colorCode = pic->picData[picLength - (i+1) * pic->w + (j+1)];
			if(colorCode != code_back){	// is not background
				screen_setPoint(x - j, y + i, (unsigned char *)colorList[colorCode]);
			}		
		}
	}
}

void screen_show(void){
	/**
	* @brief: ��Ļ��ʾ
	*/
	#if !SPI_DMA_SWITCH
	uint8_t i;
	
	#endif	
	
	#if RENDER_PWM_EN
		return;
	
	#else
		// д����Ļ
		#if SPI_DMA_SWITCH
			ws2812_show128(&screen_cache[0][0]);
	
		#else
			for(i=0; i<2; i++)	
				{ws2812_sendData(&screen_cache[i*64][0], 64*3);}
				
		#endif
				
	#endif
}


#if RENDER_PWM_EN
void screen_pwm_task(unsigned int round){
	/**
	*	@brief PWM��Ļ��ʾ������ÿ�����ڵ���һ��
	* @param fre ����Ƶ��
	*/
	static uint16_t cnt = 0;	// ����ֵ
	static uint8_t staFlag = 0;	// ״ֵ̬,0-����1-��
	uint16_t fre = 1000 / round;
	uint16_t thre = fre / 2;		// ��ֵ����ռ�ձ�
	#if !SPI_DMA_SWITCH
	uint8_t i;
	
	#endif
	
	cnt = (cnt + 1) % fre;
	
	if((cnt < thre) && (!staFlag)){	// С����ֵ�ҵ�һ�ν�������ʾ
		// д����Ļ
		#if SPI_DMA_SWITCH
			ws2812_show128(&screen_cache[0][0]);
		
		#else	
			for(i=0; i<2; i++)	
				{ws2812_sendData(&screen_cache[i*64][0], 64*3);}
				
		#endif
				
		staFlag = 1;	// ����״̬
	}else if((cnt < thre) && (staFlag)){
		ws2812_closeAll();	// �رյ���	
		
		staFlag = 0;	// ����״̬
	}
}

#endif




