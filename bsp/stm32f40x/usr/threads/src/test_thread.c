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
 
		while(1)
		{        
        rt_thread_delay(1000);
		}
}


//#ifdef RT_USING_FINSH
//#include <finsh.h>
//FINSH_FUNCTION_EXPORT(ini_test, print compile date.);
//#endif
