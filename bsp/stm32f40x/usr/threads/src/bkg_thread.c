#include "stdlib.h"
#include <rtthread.h>
#include "sys_conf.h"
#include "stm32f4xx.h"
#include "drv_wtd.h"
#include "plc.h"
#include "fifo.h"
#include "ethernetif.h"

static void update_sys_status(void);

/**
  * @brief 	output control module components cooldown 
	* @param  none
	* @retval none
  */
  
  
void bkg_thread_entry(void* parameter)
{
    RCC_ClocksTypeDef clock_st;
		rt_thread_delay(BKG_THREAD_DELAY);
    RCC_GetClocksFreq(&clock_st);	
		rt_kprintf("sys_freq:%d\n",clock_st.SYSCLK_Frequency);
    watchdog_init();
		
		while(1)
		{
        usr_net_status();
        update_sys_status();
        dog();
        rt_thread_delay(500);
		}
}



static void update_sys_status(void)
{
    extern sys_reg_st  g_sys;
    int32_t temp;

    g_sys.stat.gen.time = time(RT_NULL);
    temp = get_plc_status();
    if(temp>=0)
        g_sys.stat.mbm.plc_bitmap = temp;
}


