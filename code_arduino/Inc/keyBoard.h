#ifndef __KEYBOARD_H
#define __KEYBOARD_H


#define key_mode  	1
#define key_select  2
#define key_back   	3


void key_init(void);							//������ʼ��
unsigned char key_scan(unsigned char mode);  	//����ɨ�躯��				


#endif
