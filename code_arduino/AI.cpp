/**
* @brief	AI行为控制
* @date		2022.03.18
*/

#include "./Inc/AI.h"
#include "./Inc/pic.h"
#include "./Inc/config.h"
#include <stdlib.h>

// devices
#include <DFRobot_DHT11.h>
#include "./Inc/adc.h"
#include "./Inc/rtc.h"

#define uint8_t unsigned char
#define uint16_t unsigned short

DFRobot_DHT11 DHT;

// function declaration
static const object_action* action_choice(object_AI* object, unsigned char WEATHER, unsigned char hour);
static void action_execute(object_AI * object);
static void AI_dev_refresh(void);
static void AI_background_refresh(void);


/***************** 开关 *****************/
#define SWITCH_DEV 1	// 外设，用于天气等状态 
#define DEBUG_AI 0	// 串口输出DEBUG信息
#if DEBUG_AI
#include <stdio.h>
#define log(...) printf("[DEBUG][AI]\t"__VA_ARGS__)
#else
#define log(...) ;
#endif


/***************** 状态值 *****************/
#define ACTION_IDLE  0
#define ACTION_WALK  1
#define ACTION_SIT	 2
#define ACTION_LAY	 3
#define ACTION_SLEEP 4

#define WEATHER_NOR 	0
#define WEATHER_SUN 	1
#define WEATHER_RAIN	2

static struct{
	/**
	* @brief 传感器数据
	*/
	int temp;	// 温度
	int humi;	// 湿度
	
	uint8_t hour;	// 小时
	uint8_t min;	// 分钟
	uint8_t sec;	// 秒

  uint8_t weather;  // 天气
}devData;



/*************** 对象 ***************/
// 背景
#define pic_temp_cold pic_point_blue	// 温度冷
#define pic_temp_hot  pic_point_red	// 温度热
#define pic_humi_dry  pic_point_yellow;       // 湿度干
#define pic_humi_wet  pic_point_green;       // 湿度湿

object_AI background_temp = {{14, 7}, 0, NULL, NULL};	// 温度
object_AI background_humi = {{15, 7}, 0, NULL, NULL};	// 湿度

// 宠物
static const object_action action_idle  = { ACTION_IDLE, {&pic_default_idle, &pic_default_idle} };
static const object_action action_walk  = { ACTION_WALK, {&pic_default_walk_0, &pic_default_walk_1} };
static const object_action action_sit   = { ACTION_SIT,  {&pic_default_sit_0, &pic_default_sit_1} };
static const object_action action_lay   = { ACTION_LAY,  {&pic_default_lay_0, &pic_default_lay_1} };
static const object_action action_sleep = { ACTION_SLEEP, {&pic_default_sleep, &pic_default_sleep} };
static const object_action* action_list[5] = {&action_idle, &action_walk, &action_sit, &action_lay, &action_sleep};

object_AI cat_default = { {12,0}, 0, &action_idle,  &pic_default_idle};


/*************** 函数定义 ***************/
static const float action_probability[3][5] = {
	/**
	* @brief	事件概率
	* @ps		第一维(列)-空闲,移动,坐着,趴着,睡觉;
	*				第二维(行)-晴,雨
	*/
	{0,   0.3, 0.5, 0.2, 0},//0.1},	// 正常
	{0.1, 0.4, 0.2, 0.3, 0},//0.1},	// 晴
	{0.1, 0,   0.4, 0.5, 0},//0.2}	// 雨
};



/***************** 操作函数 *****************/
static const object_action* action_choice(object_AI* object, uint8_t WEATHER, uint8_t hour){
	/**
	* @brief	事件选择器
	* @param	object	- 主体
	* @param	WEATHER - 天气
	* @param	hour	- 时间（小时）
	* @return	随机决策后的事件
	*/
	uint8_t i, counter;
	uint8_t randInt = (rand() % 10) + 1;	// 产生1~10的随机数
	// 睡眠时间
	if (hour >= 23 || hour <= 7) {	
		object->action_state = (object_action*)&action_sleep;
		return &action_sleep;
	}

	// 随机器选择事件
	if(randInt <= 6){		// 60%概率保持当前状态
		return object->action_state;
	}
	counter = 0;
	randInt = (rand() % 10) + 1;
	for (i = 0; i < 5; i++) {
		counter = counter + 10 * action_probability[WEATHER][i];
		if (counter >= randInt) {
			i++;
			break;
		}
	}
	i--;

	// 执行结果
	object->action_state = (object_action*)action_list[i];
	return (object_action*)action_list[i];
}


void action_execute(object_AI* object) {
	/**
	* @brief	事件执行
	* @param	object	- 主体
	* @return	None
	*/
	uint8_t randInt;

	switch (object->action_state->state)
	{
	case ACTION_WALK:	// 行走
		// 在边缘时，往反方向走。其他情况按随机来
		if (object->locate[0] <= 10) {
			object->direct = 1;	// →
		}
		else if ((object->locate[0] >= 18)) {
			object->direct = 0;	// ←
		}
		else {
			randInt = rand() % 4;	// 随机方向
			if(randInt == 0){			// 1/4概率转头
				object->direct = randInt;
			}
		}

		// 坐标变换
		if (object->direct) {	// >=0， 右
			object->locate[0] += 1;
		}
		else {			// ==0， 左
			object->locate[0] -= 1;
		}
		break;
	default:
		break;
	}
}


static void AI_dev_refresh(void){
	/**
	* @brief	数据更新
	* @param	None
	* @return	None
	*/
	DHT.read(DHT11_PIN);
	devData.temp = DHT.temperature;
	devData.humi = DHT.humidity;
	
	devData.hour = calendar.hour;
	devData.min = calendar.min;
	devData.sec = calendar.sec;
	
	
	log("SensorData:\t temp:%d, humi:%d, time:%d:%d\r\n", devData.temp, devData.humi,\
																												devData.hour, devData.min);
}


void AI_background_refresh(void){
	/**
	* @brief	环境对象更新
	* @param	None
	* @return	None
	*/
	// 温湿度
	if(devData.temp < TEMP_THRE_COLD){				// cold
		background_temp.pic = &pic_temp_cold;
		log("Env:\t cold\r\n");
	}else if(devData.temp > TEMP_THRE_HOT){	// hot
		background_temp.pic = &pic_temp_hot;
		log("Env:\t hot\r\n");
	}else{												// normal
		background_temp.pic = NULL;
	}
	
	if(devData.humi < HUMI_THRE_DRY){				// dry    
		background_humi.pic = &pic_humi_dry;
		log("Env:\t dry\r\n");
	}else if(devData.humi > HUMI_THRE_WET){	// wet
		background_humi.pic = &pic_humi_wet;
		log("Env:\t wet\r\n");
	}else{												// normal
		background_humi.pic = NULL;
	}	

  if((devData.temp > 25) && (devData.humi < 80) && (adc_re_light() >= ADC_LIGHT_MID)){  // sunny
    devData.weather = WEATHER_SUN;
  }else if((devData.temp < 20) && (devData.humi > 80) && (adc_re_light() <= ADC_LIGHT_MID)){ // rainy
    devData.weather = WEATHER_RAIN;
  }else{  // unknow
    devData.weather = WEATHER_NOR;
  }
}


void AI_refresh(void) {
	/**
	* @brief	行为迭代器
	* @param	None
	* @return	None
	*/
	#if SWITCH_DEV
	AI_dev_refresh();	// 外设数据更新
	#endif
	
	// 宠物行为迭代
	action_choice(&cat_default, devData.weather, devData.hour);
	action_execute(&cat_default);
	layer_1.objectList[0] = &cat_default;

	// 背景对象迭代
	AI_background_refresh();
	layer_background.objectList[0] = &background_temp;
	layer_background.objectList[1] = &background_humi;
}
