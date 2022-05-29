#ifndef __AI_H
#define __AI_H

#include "renderer.h"

#define DHT11_PIN PA0

/***************** �������� *****************/
extern object_AI cat_default;	// è_Ĭ��
extern object_AI background_temp;	// ����_�¶�
extern object_AI background_humi;	// ����_ʪ��

/***************** �������� *****************/
extern void AI_refresh(void);

#endif
