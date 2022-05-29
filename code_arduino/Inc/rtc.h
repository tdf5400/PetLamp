#ifndef __RTC_H
#define __RTC_H

#define u8 unsigned char

// ʱ��ṹ��
typedef struct{
	volatile u8 hour;
	volatile u8 min;
	volatile u8 sec;
}_calendar_obj;
extern _calendar_obj calendar;	//�����ṹ��

extern u8 RTC_init(void);
extern u8 RTC_Set(u8 hour, u8 min, u8 sec);

#endif
