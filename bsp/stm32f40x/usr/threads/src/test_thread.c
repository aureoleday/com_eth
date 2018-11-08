#include <rtthread.h> 
#include "sys_conf.h"
#include <rtdevice.h>
/**
  * @brief 	
	* @param  none
	* @retval none
  */
  
void test_thread_entry(void* parameter)
{	
		extern sys_reg_st					g_sys;
    rt_thread_delay(TEST_THREAD_DELAY);    
    rt_pin_mode(2,0);  
		rt_pin_mode(22,0);
    
    rt_thread_delay(TEST_THREAD_DELAY);
  
		while(1)
		{
        rt_pin_write(22,1);  
        rt_pin_write(2,1);
        rt_thread_delay(500);
        rt_pin_write(2,0);
        rt_thread_delay(500);
		}
}


//#ifdef RT_USING_FINSH
//#include <finsh.h>
//FINSH_FUNCTION_EXPORT(get_comp_date, print compile date.);
//#endif
