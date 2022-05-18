/**
* @brief	��������
* @date		2022.04.16
* @ps			ʹ�����ţ�B6-Mode, B7-Select, B8-Back
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
	* @brief ������ʼ��
	*/
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	
} 


uint8_t key_scan(uint8_t mode){
	/**
	* @brief �������
	* @param mode-ģʽѡ��0-��֧��������������-֧��������
	* @return 0-�ް�����1-Mode��2-Select��3-Back
	*/
	static uint8_t key_up=1;//�������ɿ���־
	
	if(mode)  //֧������		  
		{key_up=1;}
				
	if(key_up&&(val_mode==0||val_select==0||val_back==0))	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(val_mode==0)					return key_mode;
		else if(val_select==0)	return key_select;
		else if(val_back==0)		return key_back; 
	}else if(val_mode==1&&val_select==1&&val_back==1)
		{key_up=1;}
		
	return 0;// �ް�������
}



