#include <rtthread.h> 
#include "sys_conf.h"
#include <rtdevice.h>
#include "drv_led.h"
#include "touch.h"
/**
  * @brief 	
	* @param  none
	* @retval none
  */
  
void test_thread_entry(void* parameter)
{	
		extern sys_reg_st					g_sys;
    rt_thread_delay(TEST_THREAD_DELAY);
    drv_led_init();
    lcd_bl(1);
//    Touch_Init();
//    rt_pin_mode(2,0);  
//		rt_pin_mode(22,0);
    
    rt_thread_delay(TEST_THREAD_DELAY);
//    R_Touch_test();
		while(1)
		{
//        rt_pin_write(22,1);  
//        rt_pin_write(2,1);
//        rt_thread_delay(500);
//        rt_pin_write(2,0);
        led_toggle(0);
        rt_thread_delay(500);
		}
}


//#ifdef RT_USING_FINSH
//#include <finsh.h>
//FINSH_FUNCTION_EXPORT(ini_test, print compile date.);
//#endif
