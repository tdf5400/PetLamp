/**
* @brief	ͼ��
* @date		2022.03.17
* @ps			�ù̶�ɫ���Խ�Լ�ڴ�
*/

#include "pic.h"
#include <Math.h>

#define uint8_t unsigned char
#define max(a,b) 	(a>b?a:b)
#define min(a,b) 	(a<b?a:b)


const uint8_t colorList[][3] = {
	color_black, 
	color_white, 	color_pink, 	color_pinkL,
	color_blue,		color_blueL,	color_green,
	color_red,		color_gold,		color_purple,
};

const object_pic *num_b[10] = {&pic_num_b0, &pic_num_b1, &pic_num_b2, &pic_num_b3, &pic_num_b4, &pic_num_b5, &pic_num_b6, &pic_num_b7, &pic_num_b8, &pic_num_b9};
const object_pic *num_s[10] = {&pic_num_s0, &pic_num_s1, &pic_num_s2, &pic_num_s3, &pic_num_s4, &pic_num_s5, &pic_num_s6, &pic_num_s7, &pic_num_s8, &pic_num_s9};

/***************** Picture lib *****************/
static const signed char test_pic[] = {
	code_black, code_back,	code_back,
	code_white, code_pink, 	code_pinkL,
	code_blue,	code_blueL,	code_green,
	code_red,		code_back,	code_back
};
// pet
static const signed char default_idle[] = {
	code_back,	code_pink,	code_back,	code_back,	code_pink,	code_back,	code_back,	code_back,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	code_back,	code_pink,
	code_pink,	code_blue,	code_pink,	code_blue,	code_pink,	code_back,	code_back,	code_back,	code_pink,
	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,
	code_back,	code_back,	code_pinkL,	code_back,	code_pink,	code_pinkL,	code_back,	code_pink,	code_back,
	code_back,	code_back,	code_pinkL,	code_back,	code_pink,	code_pinkL,	code_back,	code_pink,	code_back,
};
static const signed char default_walk_0[] = {
	code_back,	code_pink,	code_back,	code_back,	code_pink,	code_back,	code_back,	code_back,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	code_back,	code_pink,
	code_pink,	code_blue,	code_pink,	code_blue,	code_pink,	code_back,	code_back,	code_back,	code_pink,
	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,
	code_back,	code_back,	code_pinkL,	code_back,	code_pink,	code_pinkL,	code_back,	code_pink,	code_back,
	code_back,	code_pinkL,	code_back,	code_back,	code_pink,	code_pinkL,	code_pink,	code_back,	code_back,
};
static const signed char default_walk_1[] = {
	code_back,	code_pink,	code_back,	code_back,	code_pink,	code_back,	code_back,	code_back,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	code_back,	code_pink,
	code_pink,	code_blue,	code_pink,	code_blue,	code_pink,	code_back,	code_back,	code_back,	code_pink,
	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,
	code_back,	code_back,	code_pinkL,	code_back,	code_pink,	code_pinkL,	code_back,	code_pink,	code_back,
	code_back,	code_back,	code_pinkL,	code_pink,	code_back,	code_back,	code_back,	code_pink,	code_back,
};
static const signed char default_sit_0[] = {
	code_back, code_back, 	code_pink, code_back, code_back, code_pink, code_back, code_back, code_back, code_back, code_back,
	code_back, code_back, 	code_pink, code_pink, code_pink, code_pink, code_back, code_back, code_back, code_back, code_back,
	code_back, code_pink,  	code_blue, code_pink, code_blue, code_pink, code_back, code_back, code_back, code_back, code_back,
	code_back, code_pink,  	code_pink, code_pink, code_pink, code_pink, code_pink, code_back, code_back, code_back,	code_back,
	code_back, code_back, 	code_pink, code_pink, code_pink, code_pink, code_pink, code_back, code_back, code_pink, code_back,
	code_back, code_pink,  	code_back, code_pink, code_pink, code_pink, code_pink, code_pink, code_back, code_pink, code_back,
	code_pink, code_back,  	code_pink, code_back, code_pink, code_pink, code_pink, code_pink, code_pink, code_back, code_back,
};
static const signed char default_sit_1[] = {
	code_back, code_back, code_pink, code_back, code_back, 	code_pink, code_back,  code_back, code_back, code_back, code_back,
	code_back, code_back, code_pink, code_pink, code_pink,  code_pink, code_back,  code_back, code_back, code_back, code_back,
	code_back, code_pink, code_blue, code_pink, code_blue,  code_pink, code_back,  code_back, code_back, code_back, code_back,
	code_back, code_pink, code_pink, code_pink, code_pink,  code_pink, code_pink,  code_back, code_back, code_back,	code_back,
	code_back, code_back, code_pink, code_pink, code_pink,  code_pink, code_pink,  code_back, code_back, code_back, code_pink,
	code_back, code_pink, code_back, code_pink, code_pink,  code_pink, code_pink,  code_pink, code_back, code_pink, code_back,
	code_pink, code_back, code_pink, code_back, code_pink,  code_pink, code_pink,  code_pink, code_pink, code_back, code_back,
};
static const signed char default_lay_0[] = {
	code_back,	code_pink,	code_back,	code_back,	code_pink,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,
	code_pink,	code_blue,	code_pink,	code_blue,	code_pink,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,
	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	code_back,	code_back,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	code_back,
	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,
};
static const signed char default_lay_1[] = {
	code_back,	code_pink,	code_back,	code_back,	code_pink,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,
	code_pink,	code_blue,	code_pink,	code_blue,	code_pink,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,
	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	code_back,	code_back,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_pink,	code_back,
	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,
};
static const signed char default_sleep[] = {
	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_gold,	code_gold,	code_back,
	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_gold,	code_gold,
	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_gold,	code_gold,
	code_back,	code_pink,	code_back,	code_back,	code_pink,	code_back,	code_back,	code_back,	code_back,	code_gold,	code_gold,	code_back,
	code_back,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	
	code_pink,	code_pinkL,	code_pink,	code_pinkL,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	code_back,	code_back,	
	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_pink,	code_back,	code_back,	
};




// background
static const signed char drink_cold[] = {
	code_back,	code_back,	code_back,	code_back,	code_green,	code_back,
	code_back,	code_back,	code_back,	code_back,	code_green,	code_back,
	code_blue,	code_back,	code_back,	code_green,	code_back,	code_blue,
	code_blue,	code_blueL,	code_blueL,	code_blueL,	code_blueL,	code_blue,
	code_back,	code_blue,	code_blueL,	code_blueL,	code_blue,	code_back,
	code_back,	code_blue,	code_blueL,	code_blueL,	code_blue,	code_back,
	code_back,	code_blue,	code_blue,	code_blue,	code_blue,	code_back,
};
static const signed char drink_hot[] = {
	code_back,	code_red,		code_back,	code_back,	code_red,		code_back,
	code_back,	code_back,	code_red,		code_back,	code_back,	code_red,
	code_back,	code_red,		code_back,	code_back,	code_red,		code_back,
	code_blue,	code_back,	code_back,	code_back,	code_back,	code_blue,
	code_blue,	code_red,		code_red,		code_red,		code_red,		code_blue,
	code_back,	code_blue,	code_red,		code_red,		code_blue,	code_back,
	code_back,	code_blue,	code_red,		code_red,		code_blue,	code_back,
	code_back,	code_blue,	code_blue,	code_blue,	code_blue,	code_back,
};
static const signed char leaf[] = {
	code_back,	code_back,	code_back,	code_back,	code_green,	code_green,	code_green,	code_back,	code_back,	code_back,
	code_back,	code_back,	code_back,	code_green,	code_green,	code_green,	code_blueL,	code_blueL,	code_green,	code_green,	
	code_back,	code_back,	code_green,	code_back,	code_back,	code_green,	code_green,	code_green,	code_green, code_green,
	code_back,	code_green,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	
	code_back,	code_green,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	
	code_green,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	
	code_green,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	
	code_green,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	
};
static const signed char hotGas[] = {
	code_back,	code_red,		code_back,	code_back,	code_red,		code_back,	code_back, 	code_red,
	code_red,		code_back,	code_back,	code_red,		code_back,	code_back,	code_red,		code_back,
	code_red,		code_back,	code_back,	code_red,		code_back,	code_back,	code_red,		code_back,
	code_back,	code_red,		code_back,	code_back,	code_red,		code_back,	code_back, 	code_red,
	code_red,		code_back,	code_back,	code_red,		code_back,	code_back,	code_red,		code_back,
};
static const signed char city[] = {
	code_back,	code_back,	code_back,	code_back,	code_back,	code_white,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_back,	code_white,
	code_back,	code_back,	code_back,	code_back,	code_back,	code_white,	code_white,	code_back,	code_back,	code_back,	code_white,	code_white,	code_white,	code_back,	code_back,	code_white,
	code_white,	code_back,	code_white,	code_back,	code_back,	code_white,	code_white,	code_back,	code_back,	code_back,	code_white,	code_white,	code_white,	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,	code_back,	code_back,	code_white,	code_white,	code_back,	code_back,	code_back,	code_white,	code_white,	code_white,	code_back,	code_white,	code_white,
	code_white,	code_white,	code_white,	code_back,	code_back,	code_white,	code_white,	code_white,	code_back,	code_back,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,
	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,	code_white,
};
static const signed char sun[] = {
	code_red,	code_red,
	code_red,	code_red,
};
static const signed char moon[] = {
	code_gold, 	code_back,
	code_back,	code_gold,
	code_gold,	code_back,
};

// num big
static const signed char num_b0[] = {
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
};
static const signed char num_b1[] = {
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
};
static const signed char num_b2[] = {
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_back,
	code_white,	code_back,	code_back,
	code_white,	code_white,	code_white,
};
static const signed char num_b3[] = {
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,
};
static const signed char num_b4[] = {
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
};
static const signed char num_b5[] = {
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_back,
	code_white,	code_back,	code_back,
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,
};
static const signed char num_b6[] = {
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_back,
	code_white,	code_back,	code_back,
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
};
static const signed char num_b7[] = {
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
};
static const signed char num_b8[] = {
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
};
static const signed char num_b9[] = {
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,
};



// num small
static const signed char num_s0[] = {
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
};
static const signed char num_s1[] = {
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
};
static const signed char num_s2[] = {
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_back,
	code_white,	code_white,	code_white,
};
static const signed char num_s3[] = {
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,
};
static const signed char num_s4[] = {
	code_white,	code_back,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
};
static const signed char num_s5[] = {
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_back,
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,
};
static const signed char num_s6[] = {
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_back,
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
};
static const signed char num_s7[] = {
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
	code_back,	code_back,	code_white,
};
static const signed char num_s8[] = {
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
};
static const signed char num_s9[] = {
	code_white,	code_white,	code_white,
	code_white,	code_back,	code_white,
	code_white,	code_white,	code_white,
	code_back,	code_back,	code_white,
	code_white,	code_white,	code_white,
};


const object_pic pic_test = {3, 4, (char*) test_pic};		// test pic
// pet
const object_pic pic_default_idle = { 9, 7, (char*)default_idle };				// idle
const object_pic pic_default_walk_0 = { 9, 7, (char*)default_walk_0 };		// walk
const object_pic pic_default_walk_1 = { 9, 7, (char*)default_walk_1 };	
const object_pic pic_default_sit_0 = { 11, 7, (char*)default_sit_0 };			// sit
const object_pic pic_default_sit_1 = { 11, 7, (char*)default_sit_1 };
const object_pic pic_default_lay_0 = { 11, 6, (char*)default_lay_0 };			// lay
const object_pic pic_default_lay_1 = { 11, 6, (char*)default_lay_1 };
const object_pic pic_default_sleep = { 12, 7, (char*)default_sleep };	// sleep
// background
const object_pic pic_drink_cold = { 6, 7, (char*)drink_cold};
const object_pic pic_drink_hot  = { 6, 8, (char*)drink_hot};
const object_pic pic_leaf   = { 10, 8, (char*)leaf};
const object_pic pic_hotGas = { 8, 5, (char*)hotGas};
const object_pic pic_city 	= { 16, 6, (char*)city};
const object_pic pic_sun 	= { 2, 2, (char*)sun};
const object_pic pic_moon 	= { 2, 3, (char*)moon};
// num big
const object_pic pic_num_b0 = { 3, 7, (char*)num_b0};
const object_pic pic_num_b1 = { 3, 7, (char*)num_b1};
const object_pic pic_num_b2 = { 3, 7, (char*)num_b2};
const object_pic pic_num_b3 = { 3, 7, (char*)num_b3};
const object_pic pic_num_b4 = { 3, 7, (char*)num_b4};
const object_pic pic_num_b5 = { 3, 7, (char*)num_b5};
const object_pic pic_num_b6 = { 3, 7, (char*)num_b6};
const object_pic pic_num_b7 = { 3, 7, (char*)num_b7};
const object_pic pic_num_b8 = { 3, 7, (char*)num_b8};
const object_pic pic_num_b9 = { 3, 7, (char*)num_b9};
// num small
const object_pic pic_num_s0 = { 3, 5, (char*)num_s0};
const object_pic pic_num_s1 = { 3, 5, (char*)num_s1};
const object_pic pic_num_s2 = { 3, 5, (char*)num_s2};
const object_pic pic_num_s3 = { 3, 5, (char*)num_s3};
const object_pic pic_num_s4 = { 3, 5, (char*)num_s4};
const object_pic pic_num_s5 = { 3, 5, (char*)num_s5};
const object_pic pic_num_s6 = { 3, 5, (char*)num_s6};
const object_pic pic_num_s7 = { 3, 5, (char*)num_s7};
const object_pic pic_num_s8 = { 3, 5, (char*)num_s8};
const object_pic pic_num_s9 = { 3, 5, (char*)num_s9};



/***************** Picture function *****************/
void GRB2HSL(float *output, uint8_t *GRB_color){
	/**
	* @brief Convert GRB color to HSL color
	* @param output - write the convert result to the output
	* @param color 	- GRB color pointer
	* @ps The value scale of H is 0~360, S and L is 0~1
	*/
	float R_, G_, B_;
	float Cmax, Cmin;
	float delta;
	float H, S, L;
	
	R_ = GRB_color[1] / 255.0;
	G_ = GRB_color[0] / 255.0;
	B_ = GRB_color[2] / 255.0;
	
	Cmax = max(max(R_,G_), B_);
	Cmin = min(min(R_,G_), B_);
	delta = Cmax - Cmin;
	
	// L
	L = (Cmax + Cmin) / 2.0;
	
	// H
	if(delta == 0.0){
		H = 0;
	}else if(Cmax == R_){
		H = 60 * fmod(((G_ - B_) / delta), 6.0);
	}else if(Cmax == G_){
		H = 60 * (((B_ - R_) / delta) + 2);
	}else{
		H = 60 * (((R_ - G_) / delta) + 4);
	}
	
	// S
	if(delta == 0.0)	S = 0;
	else							S = delta / (1 - fabs(2 * L - 1));
	
	// write data
	output[0] = H;
	output[1] = S;
	output[2] = L;
}
void HSL2GRB(uint8_t *output, float *HSL_color){
	/**
	* @brief Convert HSL color to GRB color
	* @param output - write the convert result to the output
	* @param color 	- HSL color pointer
	* @ps The value scale of GRB is 0~255
	*/
	float H, S, L;
	float vR, vG, vB;
	uint8_t R, G, B;
	float C, X, m;
	
	H = HSL_color[0];
	S = HSL_color[1];
	L = HSL_color[2];
	
	if(H<0)	H = 0;
	if(S<0) S = 0;
	if(L<0) L = 0;
	if(H>=360)	H = 359;
	if(S>1) 		S = 1;
	if(L>1) 		L = 1;
	
	
	C = (1 - fabs(2 * L - 1)) * S;
	X = C * (1 - fabs(fmod((H / 60), 2) - 1));
	
	if (H >= 0 && H < 60) {
		vR = C;
		vG = X;
		vB = 0;
  } else if (H >= 60 && H < 120) {
    vR = X;
		vG = C;
		vB = 0;
  } else if (H >= 120 && H < 180) {
    vR = 0;
		vG = C;
		vB = X;
  } else if (H >= 180 && H < 240) {
    vR = 0;
		vG = X;
		vB = C;
  } else if (H >= 240 && H < 300) {
    vR = X;
		vG = 0;
		vB = C;
  } else{
    vR = C;
		vG = 0;
		vB = X;
  }
	m = L - C / 2;

	R = 255.0 * (vR + m) + 0.5;
	G = 255.0 * (vG + m) + 0.5;
	B = 255.0 * (vB + m) + 0.5;
	
	output[0] = G;
	output[1] = R;
	output[2] = B;
}
