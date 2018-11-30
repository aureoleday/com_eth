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
    
//    rt_pin_mode(74,0); 
//    rt_pin_mode(28,0); 
//    rt_pin_mode(29,0);
  
		while(1)
		{        
        rt_thread_delay(1000);
		}
}

//void test_spi(uint8_t pin_id,uint8_t data)
//{    
//    rt_pin_write(pin_id,data);
//}

//#ifdef RT_USING_FINSH
//#include <finsh.h>
//FINSH_FUNCTION_EXPORT(test_spi, test spi pin.);
//#endif
