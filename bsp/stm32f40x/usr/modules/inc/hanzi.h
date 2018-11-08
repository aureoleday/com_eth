#ifndef __HANZI_H
#define __HANZI_H
#include "sys_def.h"

void LCD_DisplayCString(uint16_t x, uint16_t y, char *pstr);
void LCD_DisplayCString_color(uint16_t x, uint16_t y, char *pstr, uint16_t brush_color, uint16_t back_color);
#endif //__HANZI_H
