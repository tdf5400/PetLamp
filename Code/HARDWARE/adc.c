/**
* @brief adc驱动
* @date  22.04.12
* @ps    使用引脚：A1
*/

#include "adc.h"
#include "delay.h"


void adc_init(void){
	/**
	* @brief adc初始化
	*/
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_DeInit(ADC1);	// 复位ADC1
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	// 独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				// 单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	// 单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// 软件启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	// 数据右对其
	ADC_InitStructure.ADC_NbrOfChannel = 1;							// 顺序进行规则转换的ADC通道数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);	// 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);	// AD校准
	while(ADC_GetCalibrationStatus(ADC1));
}


uint16_t adc_re(void){
	/**
	* @brief 读取ADC
	*/
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//使能指定的ADC1的软件转换启动功能	
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));	// 等待转换结束
	
	return ADC_GetConversionValue(ADC1);	// 返回最近一次ADC1规则组的转换结果
}


uint16_t adc_re_ave(uint8_t times){
	/**
	* @brief 读取ADC（均值滤波）
	* @param times - 次数
	*/
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0; t<times; t++){
		temp_val = temp_val + adc_re();
		delay_ms(5);
	}
	return temp_val/times;
}

