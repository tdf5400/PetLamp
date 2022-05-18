/**
* @brief	按键驱动
* @date		2022.04.16
* @ps			使用引脚：B6-Mode, B7-Select, B8-Back
*/

#include "keyBoard.h"
#include "delay.h"


#define DEBUG_KEY 0
#if DEBUG_KEY
#include <stdio.h>
#define log(...) printf("[DEBUG][KEY]\t"__VA_ARGS__)
#else
#define log(...) ;
#endif

#define val_mode  	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
#define val_select  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define val_back   	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)



void key_init(void){
	/**
	* @brief 按键初始化
	*/
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	
} 


uint8_t key_scan(uint8_t mode){
	/**
	* @brief 按键检测
	* @param mode-模式选择，0-不支持连续按，其他-支持连续按
	* @return 0-无按键，1-Mode，2-Select，3-Back
	*/
	static uint8_t key_up=1;//按键按松开标志
	
	if(mode)  //支持连按		  
		{key_up=1;}
				
	if(key_up&&(val_mode==0||val_select==0||val_back==0))	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(val_mode==0)					return key_mode;
		else if(val_select==0)	return key_select;
		else if(val_back==0)		return key_back; 
	}else if(val_mode==1&&val_select==1&&val_back==1)
		{key_up=1;}
		
	return 0;// 无按键按下
}



