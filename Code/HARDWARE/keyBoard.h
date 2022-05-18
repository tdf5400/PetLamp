#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "sys.h"


#define key_mode  	1
#define key_select  2
#define key_back   	3


void key_init(void);							//按键初始化
uint8_t key_scan(uint8_t mode);  	//按键扫描函数				


#endif
