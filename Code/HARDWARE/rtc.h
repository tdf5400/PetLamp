#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"

// 时间结构体
typedef struct{
	volatile uint8_t hour;
	volatile uint8_t min;
	volatile uint8_t sec;
	
	volatile uint16_t w_year;
	volatile uint8_t  w_month;
	volatile uint8_t  w_day;
	volatile uint8_t  week;
}_calendar_obj;
extern _calendar_obj calendar;	//日历结构体

extern u8 RTC_init(void);
extern void RTC_IRQHandler(void);
extern uint8_t RTC_Set(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);

#endif
