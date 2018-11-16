#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "common.h"

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t x_up;
    uint16_t y_up;
    uint16_t x_down;
    uint16_t y_down;
    uint16_t touch_time;
}touch_st;

void Touch_Init(void);
void Clear_Screen(void);
void Draw_Point(u16 x,u16 y,u16 color);	//在按下处画一个2*2点		
void R_Touch_test(void);
void C_Touch_test(void);
void touch_scan(void);
#endif

