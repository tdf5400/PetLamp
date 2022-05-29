/**
* @brief	画面渲染程序
* @date		2022.03.13
*/
#include "./Inc/renderer.h"
#include "./Inc/ws2812.h"
#include <string.h>



/************* 渲染层 *************/
layer_object layer_background = {NULL, NULL};	// 背景图层（由于只有1个对象所以没有用数组）
layer_object layer_1 = {NULL, NULL};			// 图层1

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
				if(object->direct){ 	// →
					screen_drawPic_hFlip(x, y, object->pic);
				}else{					// ←
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
#define screen_select(x, y) (127 - (x*8+y))


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
	ws2812_closeAll();
}

int screen_setPoint(int x, int y, unsigned char *color){
	/**
	* @brief: 点绘制
	* @param: x,y - 坐标（左下角为(0,0)）
	* @param: color - 颜色，长度为3的数组
	*/
	if((x<0 || x>15) || (y<0 || y>7))
		{return 1;}
	
	ws2812_setPixel(screen_select(x, y), color);
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


void screen_drawNum(int x, int y, char num, unsigned char *color, bool bigSizeFlag){
  /**
  * @brief: 图片绘制数字
  * @param: x,y - 坐标（图片右下角）
  * @param: num - 数值(0~9)
  * @param: color - 颜色，若为NULL则默认为白色
  * @param: bigSize - 是否用大字体
  * @ps: 背景部分不会绘制
  */
  unsigned char i, j;
  unsigned char picLength;
  signed char colorCode;
  const object_pic** numTemp;

  // 字体模板选择
  if(bigSizeFlag){
    numTemp = num_b;
  }else{
    numTemp = num_s;
  }    
  picLength = numTemp[num]->w * numTemp[num]->h - 1;

  if(color == NULL){  // 不改变颜色
    screen_drawPic(x, y, numTemp[num]);
  }else{
    for(i=0; i<numTemp[num]->h; i++){
      for(j=0; j<numTemp[num]->w; j++){
        colorCode = numTemp[num]->picData[picLength - i * numTemp[num]->w - j];
        if(colorCode != code_back){ // is not background
          screen_setPoint(x - j, y + i, (unsigned char *)color);        
        }       
      }
    }
  }
}

void screen_show(void){
	/**
	* @brief: 屏幕显示
	*/
  
  
	ws2812_show();
}
