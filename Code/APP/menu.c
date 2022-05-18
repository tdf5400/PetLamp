/**
* @brief	菜单模块
* @date		2022.04.16
*/
#include "menu.h"
#include "keyBoard.h"
#include "AI.h"
#include "renderer.h"
#include "clock.h"
#include "cityShow.h"
#include "delay.h"

#include "led.h"
#include "dht11.h"
#include "adc.h"
#include "rtc.h"

#define CNT_ROUND 100	// cnt计数值的循环范围：0 ~ (CNT_ROUND-1)
#define AI_ROUND  25  // AI更新周期
#define MENU_ROUND 3  // 菜单的项目数

#define DEBUG_MENU 1
#if DEBUG_MENU
#include <stdio.h>
#define log(...) printf("[DEBUG][MENU]\t"__VA_ARGS__)
#else
#define log(...) ;
#endif



void menu_init(void){
	/**
	* @brief 外设初始化
	* @ps    初始化所有外设
	*/
	key_init();			// 按键
	screen_init();	// 屏幕
	dht11_init();		// 温湿度
	led_init();			// led
	adc_init();			// adc
	RTC_init();			// RTC
	
	clock_set(8, 0);	// 时间设为8点
}


void menu_task(void *args){
	/**
	* @brief 菜单执行任务
	*/
	uint8_t step=0;
	uint16_t cnt=0;
	uint8_t key=0;
	args=args;
	
	while(1){
		key = key_scan(0);
		log("key: %d\r\n", key);
		switch(step){
			case 0:		// 宠物
					if(!(cnt%AI_ROUND)){		// 周期性更新任务
						AI_refresh();								// AI行为
					}
					
					if(!(cnt%(AI_ROUND/5))){
						renderer_layerRendering();	// 物品渲染
					}
					

					if(key == key_mode){	// 按下mode键，切换显示
						step = (step + 1) % MENU_ROUND;
					}
				break;
			case 1:		// 时间显示			
				if(key == key_mode){					// 按下mode键，切换显示
					clock_time_show(1);
					step = (step + 1) % MENU_ROUND;
					break;
				}else if(key == key_select){	// 按下select键，进入时间设置
						clock_time_optionTask();
				}
				clock_time_show(0);
				break;
			case 2:		// 城市显示
				city_show(calendar.hour);
				if(key == key_mode){	// 按下mode键，切换显示
						step = (step + 1) % MENU_ROUND;
				}else if(key == key_select){	// 按下select键，进入演示
						city_demo();
				}
				break;
		}
//		screen_pwm_task(100);
		
		// led
		if(cnt == 0)				{ LED_on(); }
		else if(cnt == (CNT_ROUND/2)) { LED_off(); }
		
		cnt = (cnt + 1) % CNT_ROUND;	
		delay_ms(100);	
	}
}








