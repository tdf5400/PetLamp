#ifndef __PIC_H
#define __PIC_H

extern const unsigned char pic_background[3];

// Pic Struct
typedef struct{
	unsigned char w;
	unsigned char h;
	char* picData; // From Left to Down and From Up to Down
}object_pic;

/***************** Function declare *****************/
extern void GRB2HSL(float *output, unsigned char *GRB_color);
extern void HSL2GRB(unsigned char *output, float *HSL_color);


/***************** Color system *****************/
#define code_back	 -1
#define code_black  0
#define code_white  1
#define code_pink   2
#define code_pinkL  3
#define code_blue   4
#define code_blueL  5
#define code_green  6
#define code_red    7
#define code_gold	  8
#define code_purple 9

// GRB color map
#define color_black {0, 		0, 			0}
#define color_white {0x07, 0xff, 0x07}
#define color_pink	{0x03, 0x01, 0x01}
#define color_pinkL	{0x03, 0x03, 0x03}
#define color_blue	{0x00, 0x00, 0xff}
#define color_blueL	{0x03, 0x00, 0xff}
#define color_green	{0x01, 0x00, 0xfa}
#define color_red		{204, 153, 204}
#define color_gold		{0x01, 0x01, 0xaa}
#define color_purple	{204, 153, 255}

extern const unsigned char colorList[][3];	// 颜色表
extern const object_pic *num_b[10];					// 数字（大）
extern const object_pic *num_s[10];					// 数字（小）

/***************** Pic list *****************/
extern const object_pic pic_test;
// pet
extern const object_pic pic_default_idle;
extern const object_pic pic_default_walk_0;	
extern const object_pic pic_default_walk_1;
extern const object_pic pic_default_sit_0;	
extern const object_pic pic_default_sit_1;
extern const object_pic pic_default_lay_0;	
extern const object_pic pic_default_lay_1;
extern const object_pic pic_default_sleep;
// background
extern const object_pic pic_drink_cold;
extern const object_pic pic_drink_hot;
extern const object_pic pic_leaf;
extern const object_pic pic_hotGas;
extern const object_pic pic_city;
extern const object_pic pic_sun;
extern const object_pic pic_moon;
// num big
extern const object_pic pic_num_b0;
extern const object_pic pic_num_b1;
extern const object_pic pic_num_b2;
extern const object_pic pic_num_b3;
extern const object_pic pic_num_b4;
extern const object_pic pic_num_b5;
extern const object_pic pic_num_b6;
extern const object_pic pic_num_b7;
extern const object_pic pic_num_b8;
extern const object_pic pic_num_b9;
// num small
extern const object_pic pic_num_s0;
extern const object_pic pic_num_s1;
extern const object_pic pic_num_s2;
extern const object_pic pic_num_s3;
extern const object_pic pic_num_s4;
extern const object_pic pic_num_s5;
extern const object_pic pic_num_s6;
extern const object_pic pic_num_s7;
extern const object_pic pic_num_s8;
extern const object_pic pic_num_s9;

#endif


