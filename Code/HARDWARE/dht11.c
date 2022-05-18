/**
* @brief DHT11温湿度芯片驱动
* @date  2022.03.27
* @ps    使用引脚：IO - A0
*/

#include "delay.h"
#include "dht11.h"

/********* 内部函数 *********/
static void dht11_reset(void);
static uint8_t dht11_tryAck(void);
static uint8_t dht11_readBit(void);
static uint8_t dht11_readByte(void);



void dht11_init(void){
	/**
	* @brief DHT11初始化
	*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = DHT_DAT_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT_PORT, &GPIO_InitStructure);
	
	DHT_IO_O = 1;
	dht11_reset();	// 复位
}


uint8_t dht11_re(DHT11_DataDef* output){
	/**
	* @brief dht11读取数据
	* @param output - 输出结果
	* @return 状态码
	*		 @arg 0 - 成功
	*	   @arg 1 - 失败
	* @ps 温度范围：0~50°，湿度范围：20%~90%
	*/
	uint8_t buf[5];
	uint8_t i;
	
	dht11_reset();
	
	if(dht11_tryAck() == 0){
		for(i=0; i<5; i++){
			buf[i] = dht11_readByte();
		}
		
		if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4]){
			output->humi = buf[0];// + buf[1] / 10.0;
			output->temp = buf[2];// + buf[3] / 10.0;
		}
	}else{
		return 1;
	}
	
	return 0;
}


static void dht11_reset(void){
	/**
	* @brief 复位
	*/
	DHT_Dire_O();
	DHT_IO_O = 0;
	delay_ms(20);
	DHT_IO_O = 1;
	delay_us(30);
}


static uint8_t dht11_tryAck(void){
	/**
	* @brief 检测模块是否回应
	* @return 回应状态
	*    @arg 1 - 不存在
	*		 @arg 0 - 存在
	*/
	uint8_t retry = 0;
	
	DHT_Dire_I();
	
	while(DHT_IO_I && retry < 10){	// DHT11会拉低40~80us
		retry++;
		delay_us(10);
	}
	
	if(retry >= 10){return 1;}
	else					{retry = 0;}
	
	while(!DHT_IO_I && retry < 10){	// DHT11拉低后会拉高40~80us
		retry++;
		delay_us(10);
	}
	
	if(retry >= 10){return 1;}
	
	return 0;
}


uint8_t dht11_readBit(void){
	/**
	* @brief 读一位数据
	*/
	uint8_t retry = 0;
	
	while(DHT_IO_I && retry < 10){	// DHT11会拉低40~80us
		retry++;
		delay_us(10);
	}
	
	retry = 0;
	while(!DHT_IO_I && retry < 10){	// DHT11拉低后会拉高40~80us
		retry++;
		delay_us(10);
	}
	
	delay_us(40);

	if(DHT_IO_I){return 1;}
	else				{return 0;}
}


static uint8_t dht11_readByte(void){
	/**
	* @brief 读一字节数据
	*/
	uint8_t i, dat;
	
	dat = 0;
	for(i=0; i<8; i++){
		dat = dat << 1;
		dat |= dht11_readBit();
	}
	return dat;
}

