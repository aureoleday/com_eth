#ifndef _DRV_LED_
#define _DRV_LED_
#include "sys_def.h"

void drv_led_init(void);
void led_toggle(uint8_t led_id);
void lcd_bl(uint8_t flag);

#endif //_DRV_LED_
