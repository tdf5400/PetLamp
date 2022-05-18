#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

/****************** temp *******************/
#include "AI.h"
#include "renderer.h"
#include "menu.h"

#include "led.h"


void clock_init(){
//	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
	ErrorStatus HSEStatus;
	
	RCC_DeInit();
	
	RCC_LSICmd(ENABLE);
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStatus  = RCC_WaitForHSEStartUp();	// wait for startup
	if(HSEStatus == SUCCESS){
		// 使能预取指，FLASH.H里的函数
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2);
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
		
		//第2步，配置3总线
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		
		// 第3步，配置锁相环PLLCLK = HSE * RCC_PLLMul_x
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);
		
    // 使能PLL
		RCC_PLLCmd(ENABLE);
		
		// 等待PLL稳定
		while( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET );
		
    // 第4步，选择系统时钟
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
    while( RCC_GetSYSCLKSource() != 0x08 );
	}
}

 
int main(void)
{		
	clock_init();
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	menu_init();
	
	printf("Init finished!\r\n");
	
	menu_task(NULL);
	
  while(1)
	{
		LED_on();
		delay_ms(1000);
		LED_off();
		delay_ms(1000);
	}
 }

