#ifndef __KEYBOARD_H
#define __KEYBOARD_H


#define key_mode  	1
#define key_select  2
#define key_back   	3


void key_init(void);							//按键初始化
unsigned char key_scan(unsigned char mode);  	//按键扫描函数				


#endif
