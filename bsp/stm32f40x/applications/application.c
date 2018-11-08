/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>
#include <dfs_fs.h>

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f4xx_eth.h"
#endif

#ifdef RT_USING_FINSH
#include <shell.h>
#include <finsh.h>
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif

#include "threads.h"
#include "stm32f4_rtc.h"
#include "sys_conf.h"
#include "w25qxx.h"
#include "usr_init.h"

static rt_uint8_t tcpclient_stack[ 2048 ];
static rt_uint8_t cmd_stack[ 2048 ];
static rt_uint8_t mbm_stack[ 1024 ];
static rt_uint8_t mbm_fsm_stack[ 512 ];
static rt_uint8_t lcd_stack[ 1024 ];
static rt_uint8_t bkg_stack[ 512 ];
static rt_uint8_t test_stack[ 512 ];

static struct rt_thread tcpclient_thread;
static struct rt_thread cmd_thread;
static struct rt_thread mbm_thread;
static struct rt_thread mbm_fsm_thread;
static struct rt_thread lcd_thread;
static struct rt_thread bkg_thread;
static struct rt_thread test_thread;

void rt_init_thread_entry(void* parameter)
{
    int ret;
    /* initialization RT-Thread Components */
    rt_components_init();    
    /* GDB STUB */
#ifdef RT_USING_GDB
    gdb_set_device("uart6");
    gdb_start();
#endif   
    ret = dfs_mount("flash0", "/", "elm", 0, 0);
    if(ret == 0)
        rt_kprintf("spi flash0 mount to /. OK!\n");
    else
        rt_kprintf("file system init fail!\n");
    usr_init();
}

int rt_application_init()
{
    rt_thread_t tid;
    rt_err_t result;

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, INIT_THREAD_THREAD_PRIO, 2000);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    result = rt_thread_init(&tcpclient_thread,
                            "th_tcp",
                            tcpclient_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&tcpclient_stack[0],
                            sizeof(tcpclient_stack),
                            TCPCLIENT_THREAD_PRIO,
                            7);
    if (result == RT_EOK)
    {
        rt_thread_startup(&tcpclient_thread);
    }
    
    result = rt_thread_init(&cmd_thread,
                            "th_cmd",
                            cmd_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&cmd_stack[0],
                            sizeof(cmd_stack),
                            CMD_THREAD_PRIO,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&cmd_thread);
    }    

    result = rt_thread_init(&mbm_thread,
                            "th_mbm",
                            mbm_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&mbm_stack[0],
                            sizeof(mbm_stack),
                            MBM_THREAD_PRIO,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&mbm_thread);
    }    
    
    result = rt_thread_init(&mbm_fsm_thread,
                            "th_mbm_fsm",
                            mbm_fsm_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&mbm_fsm_stack[0],
                            sizeof(mbm_fsm_stack),
                            MBM_FSM_THREAD_PRIO,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&mbm_fsm_thread);
    }

    result = rt_thread_init(&lcd_thread,
                            "th_lcd",
                            lcd_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&lcd_stack[0],
                            sizeof(lcd_stack),
                            LCD_THREAD_PRIO,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&lcd_thread);
    }	   
    
    result = rt_thread_init(&bkg_thread,
                            "th_bkg",
                            bkg_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&bkg_stack[0],
                            sizeof(bkg_stack),
                            BKG_THREAD_PRIO,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&bkg_thread);
    }	    
    
    result = rt_thread_init(&test_thread,
                            "th_test",
                            test_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&test_stack[0],
                            sizeof(test_stack),
                            TEST_THREAD_PRIO,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&test_thread);
    }	        
    
    return 0;
}
