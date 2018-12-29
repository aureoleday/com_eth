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
    static uint8_t timeout = 0;
    static uint8_t fsm = 0;
    static uint16_t flen = 0;
    uint16_t i;
    uint16_t c_fifo_len;
    ret = 0;
    c_fifo_len = get_fifo8_length(&com_rx_fifo);
    switch (fsm)
		{
        case 0:
        {
            if(c_fifo_len > 0)
            {
                fsm = 1;
                flen = c_fifo_len;
                timeout = 5;
            }
            else
            {
                fsm = 0;
                flen = 0;
                timeout = 0;
            }
            break;
        }
        case 1:
        {
            if(timeout <= 0)
            {
                fsm = 0;
                flen = 0;
                timeout=0;
                if(g_sys.conf.dbg.usr_com)
                    rt_kprintf("\nusr_com timeout!\n");
            }
            else if(c_fifo_len > flen)
            {
                fsm = 1;
                flen = c_fifo_len;
                timeout--;
                if(g_sys.conf.dbg.usr_com)
                    rt_kprintf("\nusr_com wait %d...\n",timeout);
            }
            else
            {
                for(i=0;i<flen;i++)
                {
                    fifo8_pop(&com_rx_fifo,(rx_ptr+i));   
                }
                if(g_sys.conf.dbg.usr_com)
                {
                    rt_kprintf("\nusr_com rxd:\n");
                    for(i=0;i<flen;i++)
                    {
                        rt_kprintf("%x ",*(rx_ptr+i));     
                    }
                    rt_kprintf("\n");
                }
                ret = flen;
                fsm = 0;
                flen = 0;
                timeout=0;                
            }
            break;
        }
        default:
        {
            fsm = 0;
            flen = 0;
            timeout=0;
            break;
        }
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
