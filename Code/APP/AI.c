/**
* @brief	AI��Ϊ����
* @date		2022.03.18
*/

#include "AI.h"
#include "pic.h"
#include <stdlib.h>

// devices
#include "dht11.h"
#include "adc.h"
#include "rtc.h"

// function declaration
static const object_action* action_choice(object_AI* object, unsigned char WEATHER, unsigned char hour);
static void action_execute(object_AI * object);
static void AI_dev_refresh(void);
static void AI_background_refresh(void);


/***************** ���� *****************/
#define SWITCH_DEV 1	// ���裬����������״̬ 
#define DEBUG_AI 0	// �������DEBUG��Ϣ
#if DEBUG_AI
#include <stdio.h>
#define log(...) printf("[DEBUG][AI]\t"__VA_ARGS__)
#else
#define log(...) ;
#endif


/***************** ״ֵ̬ *****************/
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
	* @brief ����������
	*/
	uint8_t temp;	// �¶�
	uint8_t humi;	// ʪ��
	
	uint8_t hour;	// Сʱ
	uint8_t min;	// ����
	uint8_t sec;	// ��
	
	uint16_t lightVol;	// ����ǿ��(��ѹ)
}devData;



/*************** ���� ***************/
// ����
#define pic_temp_cold pic_drink_hot	// �¶���
#define pic_temp_hot  pic_drink_cold	// �¶���
#define pic_humi_wet	pic_leaf;				// ʪ��ʪ

object_AI background_temp = {{14, 0}, 0, NULL, NULL};	// �¶�
object_AI background_humi = {{9, 0}, 0, NULL, NULL};	// ʪ��

// ����
static const object_action action_idle  = { ACTION_IDLE, {&pic_default_idle, &pic_default_idle} };
static const object_action action_walk  = { ACTION_WALK, {&pic_default_walk_0, &pic_default_walk_1} };
static const object_action action_sit   = { ACTION_SIT,  {&pic_default_sit_0, &pic_default_sit_1} };
static const object_action action_lay   = { ACTION_LAY,  {&pic_default_lay_0, &pic_default_lay_1} };
static const object_action action_sleep = { ACTION_SLEEP, {&pic_default_sleep, &pic_default_sleep} };
static const object_action* action_list[5] = {&action_idle, &action_walk, &action_sit, &action_lay, &action_sleep};

object_AI cat_default = { {12,0}, 0, &action_idle,  &pic_default_idle};


/*************** �������� ***************/
static const float action_probability[3][5] = {
	/**
	* @brief	�¼�����
	* @ps		��һά(��)-����,�ƶ�,����,ſ��,˯��;
	*				�ڶ�ά(��)-��,��
	*/
	{0,   0.3, 0.5, 0.2, 0},//0.1},	// ����
	{0.1, 0.4, 0.2, 0.3, 0},//0.1},	// ��
	{0.1, 0,   0.4, 0.5, 0},//0.2}	// ��
};



/***************** �������� *****************/
static const object_action* action_choice(object_AI* object, uint8_t WEATHER, uint8_t hour){
	/**
	* @brief	�¼�ѡ����
	* @param	object	- ����
	* @param	WEATHER - ����
	* @param	hour	- ʱ�䣨Сʱ��
	* @return	������ߺ���¼�
	*/
	uint8_t i, counter;
	uint8_t randInt = (rand() % 10) + 1;	// ����1~10�������
	// ˯��ʱ��
	if (hour >= 21 || hour <= 7) {	
		object->action_state = (object_action*)&action_sleep;
		return &action_sleep;
	}

	// �����ѡ���¼�
	if(randInt <= 6){		// 60%���ʱ��ֵ�ǰ״̬
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

	// ִ�н��
	object->action_state = (object_action*)action_list[i];
	return (object_action*)action_list[i];
}


void action_execute(object_AI* object) {
	/**
	* @brief	�¼�ִ��
	* @param	object	- ����
	* @return	None
	*/
	uint8_t randInt;

	switch (object->action_state->state)
	{
	case ACTION_WALK:	// ����
		// �ڱ�Եʱ�����������ߡ���������������
		if (object->locate[0] <= 10) {
			object->direct = 1;	// ��
		}
		else if ((object->locate[0] >= 18)) {
			object->direct = 0;	// ��
		}
		else {
			randInt = rand() % 4;	// �������
			if(randInt == 0){			// 1/4����תͷ
				object->direct = randInt;
			}
		}

		// ����任
		if (object->direct) {	// >=0�� ��
			object->locate[0] += 1;
		}
		else {			// ==0�� ��
			object->locate[0] -= 1;
		}
		break;
	default:
		break;
	}
}


static void AI_dev_refresh(void){
	/**
	* @brief	���ݸ���
	* @param	None
	* @return	None
	*/
	static DHT11_DataDef dht11;
	dht11_re(&dht11);
	devData.temp = dht11.temp;
	devData.humi = dht11.humi;
	
	devData.hour = calendar.hour;
	devData.min = calendar.min;
	devData.sec = calendar.sec;
	
	devData.lightVol = adc_re();
	
	log("SensorData:\t temp:%d, humi:%d, adc:%d, time:%d:%d\r\n", devData.temp, devData.humi, devData.lightVol, \
																														devData.hour, devData.min);
}


static void AI_background_refresh(void){
	/**
	* @brief	�����������
	* @param	None
	* @return	None
	*/
	// ��ʪ��
	if(0 < devData.temp < 18){				// cold
		background_temp.pic = &pic_temp_cold;
		log("Env:\t cold\r\n");
	}else if(devData.temp > 23){	// hot
		background_temp.pic = &pic_temp_hot;
		log("Env:\t hot\r\n");
	}else{												// normal
		background_temp.pic = NULL;
	}
	
	if(0 < devData.humi < 45){				// dry
		background_humi.pic = NULL;
		log("Env:\t dry\r\n");
	}else if(devData.humi > 65){	// wet
		background_humi.pic = &pic_humi_wet;
		log("Env:\t wet\r\n");
	}else{												// normal
		background_humi.pic = NULL;
	}	
	
}


void AI_refresh(void) {
	/**
	* @brief	��Ϊ������
	* @param	None
	* @return	None
	*/
	#if SWITCH_DEV
	AI_dev_refresh();	// �������ݸ���
	#endif
	
	// ������Ϊ����
	action_choice(&cat_default, WEATHER_NOR, devData.hour);
	action_execute(&cat_default);
	layer_1.objectList[0] = &cat_default;

	// �����������
	AI_background_refresh();
//	layer_background.objectList[0] = &background_temp;
	layer_background.objectList[1] = &background_humi;
}
