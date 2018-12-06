#include <rtthread.h>
#include "drv_adxl355.h"
#include "sys_conf.h"
#include "bit_op.h"

void geo_thread_entry(void* parameter)
{		
    extern sys_reg_st  g_sys; 
    rt_thread_delay(GEO_THREAD_DELAY);
  
    adxl355_reset();
  
		while(1)
		{            
        if((bit_op_get(g_sys.stat.gen.status_bm,GBM_TCP) == 1)&&(g_sys.conf.geo.geo_en == 1))
        {
            adxl355_scanfifo();
            rt_thread_delay(1);
        }
        else
            rt_thread_delay(100);
		}
}

