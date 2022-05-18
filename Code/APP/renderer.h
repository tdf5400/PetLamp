/*
* @brief: ������Ⱦ����ͷ�ļ�
* @date: 2022.03.13
*/
#ifndef __RENDERER_H
#define __RENDERER_H

#include "pic.h"


#define RENDER_PWM_EN  0 	// PWM���ȿ��ƿ��أ���Ҫ��screen_pwm_task��������ѭ������


/***************** �ṹ�� *****************/
// ��Ϊ
typedef struct {
	unsigned char state;		// ״̬����
	const object_pic *pic_data[2];	// �ж�ͼ����ʵ�ֶ�����
}object_action;		
// �������
typedef struct {
	int locate[2];					// λ��
	unsigned char direct;			// ����=0:��, >0:��
	const object_action *action_state;	// ��Ϊ״̬
	const object_pic *pic;			// ��ʾͼ��
}object_AI;				
// ��Ⱦ����
typedef struct{
	object_AI* objectList[2];	// ��Ⱦ����
}layer_object;



/************* ��Ⱦ�� *************/
extern layer_object layer_1;					// ͼ��1
extern layer_object layer_background;	// ����ͼ��

extern void renderer_layerRendering(void);



/************* ��Ļ�� *************/
extern void screen_init(void);
extern void screen_clear(void);
extern int  screen_setPoint(int x, int y, unsigned char *color);
extern void screen_drawPic(int x, int y, const object_pic* pic);
extern void screen_drawPic_hFlip(int x, int y, const object_pic* pic);
extern void screen_show(void);

#if RENDER_PWM_EN
extern void screen_pwm_task(unsigned int round);	
#endif

#endif
