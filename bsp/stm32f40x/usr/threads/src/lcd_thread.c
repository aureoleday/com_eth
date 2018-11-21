#include <rtthread.h>
#include "sys_conf.h"
#include "display.h"
#include "lcd.h"
void lcd_thread_entry(void* parameter)
{  
    rt_thread_delay(LCD_THREAD_DELAY);
    disp_init();
//    pic_test();
	while(1)
	{   
		disp_fresh();
		rt_thread_delay(1000);
	}
}



