#ifndef __CLOCK_H
#define __CLOCK_H



extern void clock_set(unsigned char hour, unsigned char min);
extern void clock_num_disp(int num0, int num1, int num2, int num3, char cleanFlag);
extern void clock_time_show(char cleanFlag);
extern unsigned char clock_time_optionTask(void);


#endif
