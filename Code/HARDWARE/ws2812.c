	/**
  * @brief 	ws2812����
	* @ps   	ʹ������: A7-MOSI
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
  * @brief 	ws2812 ��ʼ��
  * @param 	None
	* @ps   	ʹ������: A7-MOSI
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
  * @brief 	SPI1��ʼ��
  * @param 	None
	* @ps   	ʹ������: A7-MOSI
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
  * @brief 	SPI1��DMA��ʼ��
	* @ps DMA1 Channel_3
  */
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	DMA_DeInit(DMA1_Channel3); 
	
	  DMA_InitStructure.DMA_PeripheralBaseAddr = 	(uint32_t)&(SPI1->DR);                          //����  ��������(0x4001300C) ��ַ(Դ��ַ)
  DMA_InitStructure.DMA_MemoryBaseAddr = 			(uint32_t)DMA_send_buff;                    //���� SRAM �洢��ַ(Դ��ַ)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                //���䷽�� �ڴ�-����
  DMA_InitStructure.DMA_BufferSize = 128*3*8;                           //���� SPI1 ���ճ���
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  //�����ַ����(����)
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                           //�ڴ��ַ����(�仯)
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;           //���贫����(�ֽ�)
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                   //�ڴ洫����(�ֽ�)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                     //���䷽ʽ,һ�δ�����ֹͣ,�����¼���
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                           //�жϷ�ʽ-��(����)
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                      //�ڴ浽�ڴ淽ʽ��ֹ
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}


void ws2812_show128(uint8_t *dataList){
	/**
	* @brief ��ʾ128��
	* @param dataList ����Ϊ128*3������
	*/
	uint16_t i;
	uint8_t j, cache;
	uint8_t *p = &DMA_send_buff[0];
	
	// ���ݱ���
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

	// ����д��
	DMA_Cmd(DMA1_Channel3, DISABLE);   
	DMA_ClearFlag(DMA1_FLAG_TC3);								// ���DMA��λ

	DMA1_Channel3->CCR |= DMA_MemoryInc_Enable;	// ����
	DMA1_Channel3->CMAR = (uint32_t)DMA_send_buff; 
	
	DMA_SetCurrDataCounter(DMA1_Channel3, 128*3*8);
	DMA_Cmd(DMA1_Channel3, ENABLE);
}


void ws2812_closeAll(void){
	/**
	* @brief �ر����е���
	*/
	uint8_t closeBuf = SIG_0;
	
	DMA_Cmd(DMA1_Channel3, DISABLE); 
	DMA_ClearFlag(DMA1_FLAG_TC3);								// ���DMA��λ

	DMA1_Channel3->CCR &= ~(DMA_MemoryInc_Enable);	// ȡ������
	DMA1_Channel3->CMAR = (uint32_t)&closeBuf; 
	
 	DMA_SetCurrDataCounter(DMA1_Channel3, 128*3*8);
	DMA_Cmd(DMA1_Channel3, ENABLE);
}


#endif



