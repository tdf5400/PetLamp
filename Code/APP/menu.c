/**
* @brief	�˵�ģ��
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

#define CNT_ROUND 100	// cnt����ֵ��ѭ����Χ��0 ~ (CNT_ROUND-1)
#define AI_ROUND  25  // AI��������
#define MENU_ROUND 3  // �˵�����Ŀ��

#define DEBUG_MENU 1
#if DEBUG_MENU
#include <stdio.h>
#define log(...) printf("[DEBUG][MENU]\t"__VA_ARGS__)
#else
#define log(...) ;
#endif



void menu_init(void){
	/**
	* @brief �����ʼ��
	* @ps    ��ʼ����������
	*/
	key_init();			// ����
	screen_init();	// ��Ļ
	dht11_init();		// ��ʪ��
	led_init();			// led
	adc_init();			// adc
	RTC_init();			// RTC
	
	clock_set(8, 0);	// ʱ����Ϊ8��
}


void menu_task(void *args){
	/**
	* @brief �˵�ִ������
	*/
	uint8_t step=0;
	uint16_t cnt=0;
	uint8_t key=0;
	args=args;
	
	while(1){
		key = key_scan(0);
		log("key: %d\r\n", key);
		switch(step){
			case 0:		// ����
					if(!(cnt%AI_ROUND)){		// �����Ը�������
						AI_refresh();								// AI��Ϊ
					}
					
					if(!(cnt%(AI_ROUND/5))){
						renderer_layerRendering();	// ��Ʒ��Ⱦ
					}
					

					if(key == key_mode){	// ����mode�����л���ʾ
						step = (step + 1) % MENU_ROUND;
					}
				break;
			case 1:		// ʱ����ʾ			
				if(key == key_mode){					// ����mode�����л���ʾ
					clock_time_show(1);
					step = (step + 1) % MENU_ROUND;
					break;
				}else if(key == key_select){	// ����select��������ʱ������
						clock_time_optionTask();
				}
				clock_time_show(0);
				break;
			case 2:		// ������ʾ
				city_show(calendar.hour);
				if(key == key_mode){	// ����mode�����л���ʾ
						step = (step + 1) % MENU_ROUND;
				}else if(key == key_select){	// ����select����������ʾ
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








