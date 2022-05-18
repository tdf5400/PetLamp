/**
* @brief	时钟显示程序
* @date		2022.04.16
*/
#include "clock.h"
#include "renderer.h"
#include "rtc.h"
#include "keyBoard.h"
#include "delay.h"

// 数字坐标
#define locate_num0 2,0
#define locate_num1 6,0
#define locate_num2 11,0
#define locate_num3 15,0


void clock_set(uint8_t hour, uint8_t min){
	/**
	* @brief 时间设置
	* @param hour - 小时
	* @param min  - 分钟
	*/
	RTC_Set(1970, 1, 1, hour, min, 0);
}

void clock_num_disp(int num0, int num1, int num2, int num3, char cleanFlag){
	/**
	* @brief 时间显示
	* @param num - 数字，一共四位，-1则不显示，数值范围应0~9
	* @param cleanFlag - 是否要清空上次记录的数字，第一次显示时置1即可
	* @ps 默认前两个为大数字，后两个为小数字
	*/
	static uint16_t lastNum = 0;
	uint16_t nowNum = num0 * 1000 + num1 * 100 + num2 * 10 + num3;
	
	if(cleanFlag){
		lastNum = 0;
	}else{
		if(lastNum == nowNum){	// 数字没变，不用更改显示
			return;
		}else{	// 数字改变，则更新记录
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
	* @brief 时间显示
	* @param cleanFlag - 第一次显示时置1即可，是否要清空上次记录时间（用于防止相同数字重复显示）
	*/
	uint8_t hour = calendar.hour;
	uint8_t min = calendar.min;
	
	clock_num_disp(hour/10, hour%10, min/10, min%10, cleanFlag);
}


uint8_t clock_time_optionTask(void){
	/**
	* @brief 时间设置（菜单任务）
	* @return 退出方式，0-设置完毕，1-中途退出
	*/
	uint8_t key = key_scan(0);
	uint8_t key_last = key_select;	// 通过select按键进入此函数
	uint8_t step = 0;
	uint8_t hour = calendar.hour;
	uint8_t min = calendar.min;
	
	while(1){
		key = key_scan(1);
		switch(step){
			case 0:	// 小时设置
				if(key == key_last){	// 防止Mode按住导致跳过
					continue;
				}else{
					key_last = 255;
				}
				clock_num_disp(hour/10, hour%10, -1, -1, 0);
				if(key == key_mode)					{ step++; key_last=key;}			// 确认，同时防止Mode按住
				else if(key == key_select)	{ hour = (hour + 1) % 24;}	// 增加	
				else if(key == key_back)		{ return 1;}	// 返回
				break;
					
			case 1:	// 分钟设置
				if(key == key_last){	// 防止Mode按住导致跳过
					continue;
				}else{
					key_last = 255;
				}
				clock_num_disp(-1, -1, min/10, min%10, 0);
				if(key == key_mode)					{ step++;}			// 确认					
				else if(key == key_select)	{ min = (min + 1) % 60;}	// 增加	
				else if(key == key_back)		{ return 1;}	// 返回
				break;
			case 2:	// 写入时间
				clock_set(hour, min);
				return 0;
		}
//		screen_pwm_task(100);
		delay_ms(100);
	}
}
