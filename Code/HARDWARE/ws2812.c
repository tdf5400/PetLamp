	/**
  * @brief 	ws2812驱动
	* @ps   	使用引脚: A7-MOSI
  */

#include "stm32f10x.h"
#include "ws2812.h"

#if SPI_DMA_SWITCH
	#define SIG_0 0xc0
	#define SIG_1 0xf8
	static void SPI1_DMA_init(void);
#else
	#define SIG_0 0xf8
	#define SIG_1 0xe0
#endif

static void SPI1_init(void);


void ws2812_init(void){
	/**
  * @brief 	ws2812 初始化
  * @param 	None
	* @ps   	使用引脚: A7-MOSI
  */
  SPI1_init();

	#if SPI_DMA_SWITCH
		SPI1_DMA_init();		
	#endif
}

static void ws2812_sendByte(uint8_t data){
	uint8_t send_buff[8];
	uint8_t i;

	for(i=0; i<8; i++){
		if(data&0x01){
			send_buff[i] = SIG_0;
		}else{
			send_buff[i] = SIG_1;
		}
		data >>= 1;
	}
		
	for(i=0; i<8; i++){
		SPI_I2S_SendData(SPI1, send_buff[i]);
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	}
}


void ws2812_sendData(uint8_t* data, uint16_t len){
	uint8_t i;
	
#ifdef SYSTEM_SUPPORT_OS
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
#endif
	
	for(i=0; i<len; i++){
			ws2812_sendByte(data[i]);
	}
	
#ifdef SYSTEM_SUPPORT_OS
	OS_CRITICAL_EXIT();
#endif
}


static void SPI1_init(void){
	/**
  * @brief 	SPI1初始化
  * @param 	None
	* @ps   	使用引脚: A7-MOSI
  */
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
}


#if SPI_DMA_SWITCH
uint8_t DMA_send_buff[128*3*8];

static void SPI1_DMA_init(void){
	/**
  * @brief 	SPI1的DMA初始化
	* @ps DMA1 Channel_3
  */
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	DMA_DeInit(DMA1_Channel3); 
	
	  DMA_InitStructure.DMA_PeripheralBaseAddr = 	(uint32_t)&(SPI1->DR);                          //设置  接收外设(0x4001300C) 地址(源地址)
  DMA_InitStructure.DMA_MemoryBaseAddr = 			(uint32_t)DMA_send_buff;                    //设置 SRAM 存储地址(源地址)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                //传输方向 内存-外设
  DMA_InitStructure.DMA_BufferSize = 128*3*8;                           //设置 SPI1 接收长度
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  //外设地址增量(不变)
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                           //内存地址增量(变化)
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;           //外设传输宽度(字节)
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                   //内存传输宽度(字节)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                     //传输方式,一次传输完停止,不重新加载
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                           //中断方式-高(三级)
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                      //内存到内存方式禁止
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}


void ws2812_show128(uint8_t *dataList){
	/**
	* @brief 显示128格
	* @param dataList 长度为128*3的数组
	*/
	uint16_t i;
	uint8_t j, cache;
	uint8_t *p = &DMA_send_buff[0];
	
	// 数据编译
	for(i=0; i<(128*3); i++){
		for(j=0; j<8; j++){
			cache = dataList[i];
			if(cache & 0x01){
				*p = SIG_1;
			}else{
				*p = SIG_0;
			}
			p++;
			cache >>= 1;
		}
	}

	// 数据写入
	DMA_Cmd(DMA1_Channel3, DISABLE);   
	DMA_ClearFlag(DMA1_FLAG_TC3);								// 清除DMA置位

	DMA1_Channel3->CCR |= DMA_MemoryInc_Enable;	// 自增
	DMA1_Channel3->CMAR = (uint32_t)DMA_send_buff; 
	
	DMA_SetCurrDataCounter(DMA1_Channel3, 128*3*8);
	DMA_Cmd(DMA1_Channel3, ENABLE);
}


void ws2812_closeAll(void){
	/**
	* @brief 关闭所有灯珠
	*/
	uint8_t closeBuf = SIG_0;
	
	DMA_Cmd(DMA1_Channel3, DISABLE); 
	DMA_ClearFlag(DMA1_FLAG_TC3);								// 清除DMA置位

	DMA1_Channel3->CCR &= ~(DMA_MemoryInc_Enable);	// 取消自增
	DMA1_Channel3->CMAR = (uint32_t)&closeBuf; 
	
 	DMA_SetCurrDataCounter(DMA1_Channel3, 128*3*8);
	DMA_Cmd(DMA1_Channel3, ENABLE);
}


#endif



