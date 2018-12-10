#include "stdlib.h"
#include <rtthread.h>
#include "sys_conf.h"
#include "stm32f4xx.h"
#include "drv_wtd.h"
#include "plc.h"
#include "fifo.h"
#include "ethernetif.h"
#include "drv_led.h"
#include "touch.h"
#include "lcd.h"
#include "display.h"

static void update_sys_status(void);
//static uint16_t	touch_timer_init(void);

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
    drv_led_init();
    lcd_bl(1);  
    Touch_Init();
    
//    touch_timer_init();
 
    watchdog_init();
		
		while(1)
		{
        disp_fresh();
        usr_net_status();
        update_sys_status();
        dog();
        led_toggle(0);
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

/**
  * @brief  digital input sample interval timeout callback function, calls di_read() each time to update di buffer queue
  * @param  none
  * @retval none
  */
//static void touch_timeout(void* parameter)
//{
//    extern touch_st touch_inst;
//    touch_scan();
//    if(touch_inst.x_down<lcd_width&&touch_inst.y_down<lcd_height)
//		{	
//			if(touch_inst.x_down>(lcd_width-40)&&touch_inst.y_down>lcd_height-18)
//          Clear_Screen();  //Çå¿ÕÆÁÄ»
//			else 
//          Draw_Point(touch_inst.x_down,touch_inst.y_down,RED);		//»­Í¼
//		}
//    if(touch_inst.x_up<lcd_width&&touch_inst.y_up<lcd_height)
//        Draw_Point(touch_inst.x_up,touch_inst.y_up,GREEN);
//}

/**
  * @brief   sample interval timer initialization, expires in 6 miliseconds pieriod
  * @param  none
  * @retval none
  */
//rt_timer_t tm_touch;
//static uint16_t	touch_timer_init(void)
//{
//    extern sys_reg_st  g_sys;
//		tm_touch = rt_timer_create("tm_touch",
//									touch_timeout,
//									RT_NULL,
//									5,
//									RT_TIMER_FLAG_PERIODIC);
//		rt_timer_start(tm_touch);
//		return 1;
//}


