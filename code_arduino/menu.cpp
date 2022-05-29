/**
* @brief  菜单模块
* @date   2022.04.16
*/
#include "./Inc/menu.h"
#include "./Inc/keyBoard.h"
#include "./Inc/AI.h"
#include "./Inc/renderer.h"
#include "./Inc/clock.h"

#include "./Inc/led.h"
#include "./Inc/adc.h"
#include "./Inc/rtc.h"
#include "./Inc/ws2812.h"

#include <Arduino.h>

#define CNT_ROUND 200 // cnt计数值的循环范围：0 ~ (CNT_ROUND-1)
#define AI_ROUND  50  // AI更新周期
#define MENU_ROUND 4  // 菜单的项目数

#define SW_BRIGHT_CTR 1
#define DEBUG_MENU 0
#if DEBUG_MENU
#include <stdio.h>
#define log(...) printf("[DEBUG][MENU]\t"__VA_ARGS__)
#else
#define log(...) ;
#endif


// ********************* 城市显示 *********************
static const char g_skyObject_locate[12][2] = {   // 要绘制的天体的位置
    {2,0},   {3, 2},  {4, 4},  {5, 5}, {6, 6}, {8, 6}, 
    {10, 6}, {12, 6}, {13, 5}, {14, 4}, {15, 2}, {16, 0}
};
static const char SUN_BEGIN = 6;    // 日间起始点
static const char SUN_END = 18;     // 日间终点
extern void city_show(char hour);
extern void city_demo(void);

// ********************* 温湿度显示 *********************
#include <DFRobot_DHT11.h>
extern DFRobot_DHT11 DHT;
extern void sensor_show(void);


void menu_init(void){
  /**
  * @brief 外设初始化
  * @ps    初始化所有外设
  */
  key_init();     // 按键
  screen_init();  // 屏幕
//  dht11_init();   // 温湿度
  led_init();     // led
  adc_init();     // adc
  RTC_init();     // RTC
  
  clock_set(8, 0);  // 时间设为8点
}


void menu_task(void *args){
  /**
  * @brief 菜单执行任务
  */
  uint8_t step=0;
  uint16_t cnt=0;
  uint8_t key=0;
  uint8_t key_last = 0;
  args=args;

  while(1){
    key = key_scan(0);
    log("key: %d\r\n", key);
    switch(step){
      case 0:   // 宠物
          if((cnt%AI_ROUND) == 5){    // 周期性更新任务
            AI_refresh();               // AI行为
          }
          
          if((cnt%(AI_ROUND/4)) == 1){
            renderer_layerRendering();  // 物品渲染
          }          

          if(key == key_mode){  // 按下mode键，切换显示
            cnt = 0;
            step = (step + 1) % MENU_ROUND;
          }
        break;
        
      case 1:   // 时间显示           
        clock_time_show(1);
        
        if(key == key_mode){          // 按下mode键，切换显示
          cnt = 0;
          step = (step + 1) % MENU_ROUND;
        }else if(key == key_select){  // 按下select键，进入时间设置
            clock_time_optionTask();
        }
        break;
        
      case 2:   // 城市显示
        city_show(calendar.hour);
        
        if(key == key_mode){  // 按下mode键，切换显示
            cnt = 0;
            step = (step + 1) % MENU_ROUND;
        }else if(key == key_select){  // 按下select键，进入演示
            city_demo();
        }
        break;
        
      case 3:   // 传感器显示
        sensor_show();

        if(key == key_mode){  // 按下mode键，切换显示
            cnt = 0;
            step = (step + 1) % MENU_ROUND;
        }
        break;
    }

    
    // 亮度调节
    #if SW_BRIGHT_CTR
    if(cnt%(CNT_ROUND/5) == 0){
      unsigned short bright = adc_re_light();
      if(bright == ADC_LIGHT_HIGH){
        ws2812_setBright(65);
      }else if(bright == ADC_LIGHT_MID){
        ws2812_setBright(30);
      }else{
        ws2812_setBright(10);
      }
    }
    #endif


    // led
    if(cnt%(CNT_ROUND) == 0)  { LED_on(); }
    else                        { LED_off(); }
    
    cnt = (cnt + 1) % CNT_ROUND;  
    
    delay(50);  
  }
}



// ********************* 城市显示 *********************
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
    delay(100);
    
    key = key_scan(0);
    if((key == key_select) || (key == key_back)){
      return;
    }
  }
}



// ********************* 温湿度显示 *********************
void sensor_show(void){
  /**
 * @brief 显示温湿度
 * 
 */
  static uint16_t cnt = 0;
  static uint8_t temp = 0;
  static uint8_t humi = 0;

  cnt = (cnt + 1) % 200;

  // 更新数据
  if(cnt%20 == 5){
    DHT.read(DHT11_PIN);
    temp = DHT.temperature;
    humi = DHT.humidity;
  }

  // 显示
  if(cnt < 100){    
    screen_clear();
    screen_drawNum(3, 1, temp/10%10, (unsigned char*)&colorList[code_pinkL][0],false);    
    screen_drawNum(7, 1, temp%10, (unsigned char*)&colorList[code_pinkL][0],false);
    screen_drawPic(14, 1, &pic_sign_temp);
    screen_show();
  }else{
    screen_clear();
    screen_drawNum(3, 1, humi/10%10, (unsigned char*)&colorList[code_pinkL][0], false);    
    screen_drawNum(7, 1, humi%10, (unsigned char*)&colorList[code_pinkL][0], false);
    screen_drawPic(14, 1, &pic_sign_humi);
    screen_show();
  }
}
