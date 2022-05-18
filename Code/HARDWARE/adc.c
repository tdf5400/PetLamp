/**
* @brief adc����
* @date  22.04.12
* @ps    ʹ�����ţ�A1
*/

#include "adc.h"
#include "delay.h"


void adc_init(void){
	/**
	* @brief adc��ʼ��
	*/
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_DeInit(ADC1);	// ��λADC1
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	// ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				// ��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	// ����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// �������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	// �����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;							// ˳����й���ת����ADCͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);	// ��λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);	// ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));
}


uint16_t adc_re(void){
	/**
	* @brief ��ȡADC
	*/
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//ʹ��ָ����ADC1�����ת����������	
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));	// �ȴ�ת������
	
	return ADC_GetConversionValue(ADC1);	// �������һ��ADC1�������ת�����
}


uint16_t adc_re_ave(uint8_t times){
	/**
	* @brief ��ȡADC����ֵ�˲���
	* @param times - ����
	*/
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0; t<times; t++){
		temp_val = temp_val + adc_re();
		delay_ms(5);
	}
	return temp_val/times;
}

