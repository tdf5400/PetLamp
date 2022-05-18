/**
* @brief	ʱ����ʾ����
* @date		2022.04.16
*/
#include "clock.h"
#include "renderer.h"
#include "rtc.h"
#include "keyBoard.h"
#include "delay.h"

// ��������
#define locate_num0 2,0
#define locate_num1 6,0
#define locate_num2 11,0
#define locate_num3 15,0


void clock_set(uint8_t hour, uint8_t min){
	/**
	* @brief ʱ������
	* @param hour - Сʱ
	* @param min  - ����
	*/
	RTC_Set(1970, 1, 1, hour, min, 0);
}

void clock_num_disp(int num0, int num1, int num2, int num3, char cleanFlag){
	/**
	* @brief ʱ����ʾ
	* @param num - ���֣�һ����λ��-1����ʾ����ֵ��ΧӦ0~9
	* @param cleanFlag - �Ƿ�Ҫ����ϴμ�¼�����֣���һ����ʾʱ��1����
	* @ps Ĭ��ǰ����Ϊ�����֣�������ΪС����
	*/
	static uint16_t lastNum = 0;
	uint16_t nowNum = num0 * 1000 + num1 * 100 + num2 * 10 + num3;
	
	if(cleanFlag){
		lastNum = 0;
	}else{
		if(lastNum == nowNum){	// ����û�䣬���ø�����ʾ
			return;
		}else{	// ���ָı䣬����¼�¼
			lastNum = nowNum;
		}
	}
	
	screen_clear();
	
	if(num0 != -1)	screen_drawPic(locate_num0, num_b[num0]);
	if(num1 != -1)	screen_drawPic(locate_num1, num_b[num1]);
	if(num2 != -1)	screen_drawPic(locate_num2, num_s[num2]);
	if(num3 != -1)	screen_drawPic(locate_num3, num_s[num3]);
	
	screen_show();
}

void clock_time_show(char cleanFlag){
	/**
	* @brief ʱ����ʾ
	* @param cleanFlag - ��һ����ʾʱ��1���ɣ��Ƿ�Ҫ����ϴμ�¼ʱ�䣨���ڷ�ֹ��ͬ�����ظ���ʾ��
	*/
	uint8_t hour = calendar.hour;
	uint8_t min = calendar.min;
	
	clock_num_disp(hour/10, hour%10, min/10, min%10, cleanFlag);
}


uint8_t clock_time_optionTask(void){
	/**
	* @brief ʱ�����ã��˵�����
	* @return �˳���ʽ��0-������ϣ�1-��;�˳�
	*/
	uint8_t key = key_scan(0);
	uint8_t key_last = key_select;	// ͨ��select��������˺���
	uint8_t step = 0;
	uint8_t hour = calendar.hour;
	uint8_t min = calendar.min;
	
	while(1){
		key = key_scan(1);
		switch(step){
			case 0:	// Сʱ����
				if(key == key_last){	// ��ֹMode��ס��������
					continue;
				}else{
					key_last = 255;
				}
				clock_num_disp(hour/10, hour%10, -1, -1, 0);
				if(key == key_mode)					{ step++; key_last=key;}			// ȷ�ϣ�ͬʱ��ֹMode��ס
				else if(key == key_select)	{ hour = (hour + 1) % 24;}	// ����	
				else if(key == key_back)		{ return 1;}	// ����
				break;
					
			case 1:	// ��������
				if(key == key_last){	// ��ֹMode��ס��������
					continue;
				}else{
					key_last = 255;
				}
				clock_num_disp(-1, -1, min/10, min%10, 0);
				if(key == key_mode)					{ step++;}			// ȷ��					
				else if(key == key_select)	{ min = (min + 1) % 60;}	// ����	
				else if(key == key_back)		{ return 1;}	// ����
				break;
			case 2:	// д��ʱ��
				clock_set(hour, min);
				return 0;
		}
//		screen_pwm_task(100);
		delay_ms(100);
	}
}
