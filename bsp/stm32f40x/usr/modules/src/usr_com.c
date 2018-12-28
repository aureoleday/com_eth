#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "usr_com.h"
#include "fifo.h"
#include "sys_conf.h"

static rt_device_t uart_device = RT_NULL;

fifo8_cb_td com_rx_fifo;
fifo8_cb_td com_tx_fifo;

static rt_err_t com_rx(rt_device_t dev, rt_size_t size)
{
    /* ·¢ËÍÊÂ¼þ */
    //rt_event_send(&event, UART_RX_EVENT);
    rt_uint8_t ch;
    extern sys_reg_st  g_sys; 	
               
    if(size == 1)
    {        
        rt_device_read(uart_device, 0, &ch, 1);
        fifo8_push(&com_rx_fifo,&ch);
//        if(g_sys.conf.dbg.usr_com)
//            rt_kprintf("s1: %x\n",ch);
    }
    return RT_EOK;
}


uint32_t com_tx(uint8_t* tx_buf, uint16_t tx_len)
{
    extern sys_reg_st  g_sys; 
    rt_err_t ret;
    int16_t i;
    ret = rt_device_write(uart_device, 0, tx_buf, tx_len);
    if(g_sys.conf.dbg.usr_com)
    {
        rt_kprintf("\nusr_com txd:\n");
        for(i=0;i<tx_len;i++)
        {
            rt_kprintf("%x ",*(tx_buf+i));    
        }
        rt_kprintf("\n");
    }
    return ret;
}

int16_t com_recv(uint8_t *rx_ptr)
{
    extern sys_reg_st  g_sys; 
    int16_t ret;
    uint16_t flen[2];
    uint16_t timeout;
    uint16_t i;
  
    ret = 0;
    flen[0] = 0;
    flen[1] = 0;
    flen[0] = get_fifo8_length(&com_rx_fifo);
    timeout = 5;
  
    if(flen[0] > 0)
    {
        while(timeout > 0)
        {
            flen[1] = get_fifo8_length(&com_rx_fifo);
            if(flen[1] > flen[0])
            {
                flen[0] = flen[1];
                timeout--;
            }
            else
            {
                for(i=0;i<flen[1];i++)
                {
                    fifo8_pop(&com_rx_fifo,(rx_ptr+i));   
                }
                if(g_sys.conf.dbg.usr_com)
                {
                    rt_kprintf("\nusr_com rxd:\n");
                    for(i=0;i<flen[1];i++)
                    {
                        rt_kprintf("%x ",*(rx_ptr+i));     
                    }
                    rt_kprintf("\n");
                }
                ret = flen[1];
                break;
            }
            rt_thread_delay(10);
        }
        if(timeout == 0)
            ret = -1;
    }
    return ret;
}

int32_t com_init(void)
{  
    struct serial_configure sconfig = RT_SERIAL_CONFIG_DEFAULT;
    sconfig.baud_rate = 9600;  
    uart_device = rt_device_find("uart1");   

    rt_device_control(uart_device, RT_DEVICE_CTRL_CONFIG, &sconfig);
  
    rt_device_set_rx_indicate(uart_device, com_rx);
    rt_device_open(uart_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX );
  
    fifo8_init(&com_rx_fifo,1,COM_RTX_BUF_DEPTH);
		fifo8_init(&com_tx_fifo,1,COM_RTX_BUF_DEPTH);
    return 0;
}

INIT_APP_EXPORT(com_init);

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(com_tx, plc rtx test.);
#endif
