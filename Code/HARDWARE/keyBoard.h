#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "sys.h"


#define key_mode  	1
#define key_select  2
#define key_back   	3


void key_init(void);							//������ʼ��
uint8_t key_scan(uint8_t mode);  	//����ɨ�躯��				


#endif
