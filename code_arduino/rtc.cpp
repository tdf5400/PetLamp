/**
* @brief RTC模块
* @date  22.04.12
*/

#include "./Inc/rtc.h"
#include <RTClock.h>


/********* 内部函数声明 *********/
static void RTC_IRQHandler(void);
static u8 RTC_Get(void);
static RTClock rtc(RTCSEL_LSE);

_calendar_obj calendar;//时钟结构体 

u8 RTC_init(void){
  /**
  * @brief  实时时钟配置
  * @return 0 - 正常, 其他 - 错误代码
  * @ps 初始化RTC时钟,同时检测时钟是否工作正常，BKP->DR1用于保存是否第一次配置的设置
  */
  rtc.attachSecondsInterrupt(RTC_IRQHandler); // 秒中断
  RTC_Set(8,0,0);
  RTC_Get();//更新时间       
  
  return 0; //ok
}     


u8 RTC_Set(u8 hour, u8 min, u8 sec){
  tm_t ts;
  
  ts.hour = hour;
  ts.minute = min;
  ts.second = sec;
  
  rtc.setTime(rtc.makeTime(ts));
  RTC_Get();//更新时间       
}


static void RTC_IRQHandler(void){
  /**
  * @brief RTC时钟中断
  * @ps 每秒触发一次
  */         
  RTC_Get();//更新时间                        
}


static u8 RTC_Get(void){
  /**
  * @brief 得到当前的时间
  * @return 0 - 成功， 其他 - 错误代码
  */
  tm_t tm;

  rtc.breakTime(rtc.now(), tm);
  
  calendar.hour=tm.hour;      //小时
  calendar.min=tm.minute;  //分钟  
  calendar.sec=tm.second;  //秒钟
  return 0;
}  

 
