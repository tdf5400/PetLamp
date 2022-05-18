#include "cityShow.h"

#include "renderer.h"
#include "keyBoard.h"
#include "delay.h"

static const char g_skyObject_locate[12][2] = {   // 要绘制的天体的位置
    {2,0},   {3, 2},  {4, 4},  {5, 5}, {6, 6}, {8, 6}, 
    {10, 6}, {12, 6}, {13, 5}, {14, 4}, {15, 2}, {16, 0}
};
static const char SUN_BEGIN = 6;    // 日间起始点
static const char SUN_END = 18;     // 日间终点

void city_show(char hour){
    /**
     * @brief 显示城市
     * 
     */
    char render_locate_x, render_locate_y;
		screen_clear();
	
    // 渲染日/月
    if((hour >= SUN_BEGIN) && (hour < SUN_END)){  // 日间
        render_locate_x = g_skyObject_locate[hour - SUN_BEGIN][0];
        render_locate_y = g_skyObject_locate[hour - SUN_BEGIN][1];
        screen_drawPic(render_locate_x, render_locate_y, &pic_sun);
    }else{  // 夜间     
				if(hour < SUN_BEGIN){
					render_locate_x = g_skyObject_locate[11 - (SUN_BEGIN - hour)][0];
					render_locate_y = g_skyObject_locate[11 - (SUN_BEGIN - hour)][1];
				}else{
					render_locate_x = g_skyObject_locate[hour - SUN_END][0];
					render_locate_y = g_skyObject_locate[hour - SUN_END][1];
				}
        
        screen_drawPic(render_locate_x, render_locate_y, &pic_moon);
    }

    // 渲染城市
    screen_drawPic(15, 0, &pic_city);
    screen_show();
}


void city_demo(void){
		/**
		 * @brief 功能演示
		 * @param fre 调用频率
		 * @note: 1s动一次，select键退出/进入
		 */
	char hour=6;
	char key;
	int times = 0;
	
	while(1){
		
		if(!times){
			hour = (hour + 1) % 24;
		}
		times = (times + 1) % 10;

		city_show(hour);
//		screen_pwm_task(100);
		delay_ms(100);
		
		
		key = key_scan(0);
		if((key == key_select) || (key == key_back)){
			return;
		}
	}
	
}
