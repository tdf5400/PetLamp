/**
* @brief adc驱动
* @date  22.04.12
* @ps    使用引脚：A1
*/

#include "./Inc/adc.h"
#include <Arduino.h>

#define uint8_t unsigned char
#define uint16_t unsigned short

#define ADC_PIN PA1


void adc_init(void){
  /**
  * @brief adc初始化
  */
  pinMode(ADC_PIN, INPUT_ANALOG);
}



uint16_t adc_re_light(void){
  /**
  * @brief 读取ADC亮度
  */
  uint16_t dat = analogRead(ADC_PIN);

  if(dat > 350){
    return ADC_LIGHT_HIGH;
  }else if(dat > 70){
    return ADC_LIGHT_MID;
  }

  return ADC_LIGHT_LOW;
}


uint16_t adc_re_vol(void){
  /**
  * @brief 读取ADC电压
  */
  return analogRead(ADC_PIN);
}
