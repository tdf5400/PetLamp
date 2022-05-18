/**
* @brief	画面渲染程序
* @date		2022.03.13
*/

#include "renderer.h"
#include "ws2812.h"
#include <string.h>

/************* 渲染层 *************/
layer_object layer_background = {NULL, NULL};	// 背景图层（由于只有1个对象所以没有用数组）
layer_object layer_1 = {NULL, NULL};	// 图层1

void layer_draw(layer_object* layer){
	/**
	* @brief: 渲染层绘图
	* @param: layer - layer_object, 渲染层
	* @param: object - 渲染对象
	*/
	int i;
	int x, y;
	object_AI* object;
	for(i=0; i<2; i++){
		object = layer->objectList[i];
		if(object != NULL){
			// 状态图片更新
			if(object->action_state != NULL){
				if(object->pic != object->action_state->pic_data[0]) {
					object->pic = object->action_state->pic_data[0];
				}
				else {
					object->pic = object->action_state->pic_data[1];
				}
			}
			
			// 若有图片，则显示
			if(object->pic != NULL){
				x = object->locate[0];
				y = object->locate[1];
				if(object->direct){ // →
					screen_drawPic_hFlip(x, y, object->pic);
				}else{							// ←
					screen_drawPic(x, y, object->pic);
				}
			}
			
		}
	}
}


void renderer_layerRendering(void){
	/**
	* @brief: 渲染层渲染
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

/************* 屏幕层 *************/
// 屏幕数据缓存
// .         ↓    ↓(127,7)
// .         ↓    ↓
// .         .    .
// .(0,0)    ↓    ↓
//          8~15  0~7
static unsigned char screen_cache[128][3];	// 屏幕缓存
#define screen_select(x, y) screen_cache[127 - (x*8+y)]


void screen_init(void){
	/**
	* @brief: 渲染模块初始化
	*/
	ws2812_init();
}

void screen_clear(void){
	/**
	* @brief: 清屏
	* @ps: 背景色为(0,0,0)
	*/
	memset(screen_cache, 0x00, 128*3);
}

int screen_setPoint(int x, int y, unsigned char *color){
	/**
	* @brief: 点绘制
	* @param: x,y - 坐标（左下角为(0,0)）
	* @param: color - 颜色，长度为3的数组
	*/
	if((x<0 || x>15) || (y<0 || y>7))
		{return 1;}
	
	memcpy(screen_select(x, y), color, 3);		
	return 0;
}

void screen_drawPic(int x, int y, const object_pic* pic){
	/**
	* @brief: 图片绘制
	* @param: x,y - 坐标（图片右下角）
	* @param: pic - 图片
	* @ps: 背景部分不会绘制
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
	* @brief: 图片绘制
	* @param: x,y - 坐标（图片右下角）
	* @param: pic - 图片
	* @ps: 黑色部分不会绘制
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
	* @brief: 屏幕显示
	*/
	#if !SPI_DMA_SWITCH
	uint8_t i;
	
	#endif	
	
	#if RENDER_PWM_EN
		return;
	
	#else
		// 写入屏幕
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
	*	@brief PWM屏幕显示任务，需每个周期调用一次
	* @param fre 调用频率
	*/
	static uint16_t cnt = 0;	// 计数值
	static uint8_t staFlag = 0;	// 状态值,0-暗，1-亮
	uint16_t fre = 1000 / round;
	uint16_t thre = fre / 2;		// 阈值，即占空比
	#if !SPI_DMA_SWITCH
	uint8_t i;
	
	#endif
	
	cnt = (cnt + 1) % fre;
	
	if((cnt < thre) && (!staFlag)){	// 小于阈值且第一次进入则显示
		// 写入屏幕
		#if SPI_DMA_SWITCH
			ws2812_show128(&screen_cache[0][0]);
		
		#else	
			for(i=0; i<2; i++)	
				{ws2812_sendData(&screen_cache[i*64][0], 64*3);}
				
		#endif
				
		staFlag = 1;	// 更新状态
	}else if((cnt < thre) && (staFlag)){
		ws2812_closeAll();	// 关闭灯珠	
		
		staFlag = 0;	// 更新状态
	}
}

#endif




