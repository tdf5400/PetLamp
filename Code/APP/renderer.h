/*
* @brief: 画面渲染程序头文件
* @date: 2022.03.13
*/
#ifndef __RENDERER_H
#define __RENDERER_H

#include "pic.h"


#define RENDER_PWM_EN  0 	// PWM亮度控制开关，需要用screen_pwm_task函数定期循环启动


/***************** 结构体 *****************/
// 行为
typedef struct {
	unsigned char state;		// 状态代码
	const object_pic *pic_data[2];	// 行动图像（以实现动作）
}object_action;		
// 物体对象
typedef struct {
	int locate[2];					// 位置
	unsigned char direct;			// 朝向，=0:←, >0:→
	const object_action *action_state;	// 行为状态
	const object_pic *pic;			// 显示图像
}object_AI;				
// 渲染对象
typedef struct{
	object_AI* objectList[2];	// 渲染对象
}layer_object;



/************* 渲染层 *************/
extern layer_object layer_1;					// 图层1
extern layer_object layer_background;	// 背景图层

extern void renderer_layerRendering(void);



/************* 屏幕层 *************/
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
