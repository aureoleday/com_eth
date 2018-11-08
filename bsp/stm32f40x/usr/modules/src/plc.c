#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "plc.h"
#include "fifo.h"

static rt_device_t uart_device = RT_NULL;

fifo8_cb_td plc_rx_fifo;
fifo8_cb_td plc_tx_fifo;

static rt_err_t plc_rx(rt_device_t dev, rt_size_t size)
{
    /* ·¢ËÍÊÂ¼þ */
    //rt_event_send(&event, UART_RX_EVENT);
    rt_uint8_t ch;
               
    if(size == 1)
    {        
        rt_device_read(uart_device, 0, &ch, 1);
        fifo8_push(&plc_rx_fifo,&ch);
//        rt_kprintf("s1: %x\n",ch);
    }
    return RT_EOK;
}


uint32_t plc_tx(uint8_t* tx_buf, uint16_t tx_len)
{
    rt_err_t ret;
    ret = rt_device_write(uart_device, 0, tx_buf, tx_len);
    return ret;
}


int32_t get_plc_status(void)
{
    uint8_t rx_buf[32];
    int32_t ret;
    uint16_t fifo_len = 0;
    uint16_t i;
  
    fifo_len = get_fifo8_length(&plc_rx_fifo);
    if(fifo_len == 0)
    {
        ret = -1;
    }
    else if(fifo_len == 1)
    {
        rt_thread_delay(100);
        fifo_len = get_fifo8_length(&plc_rx_fifo);
        if(fifo_len == 1)
        {
            fifo8_reset(&plc_rx_fifo);
            ret = -2;
        }                
    }
    else
    {
        for(i=0;i<fifo_len;i++)
        {
            fifo8_pop(&plc_rx_fifo,&rx_buf[i]);
        }
        ret = (rx_buf[i-2]<<8)|rx_buf[i-1];
//        rt_kprintf("ret: %x\n",ret);
    }      
    return ret;
}

int32_t plc_init(void)
{  
    struct serial_configure sconfig = RT_SERIAL_CONFIG_DEFAULT;
    sconfig.baud_rate = 9600;  
    uart_device = rt_device_find("uart1");   

    rt_device_control(uart_device, RT_DEVICE_CTRL_CONFIG, &sconfig);
  
    rt_device_set_rx_indicate(uart_device, plc_rx);
    rt_device_open(uart_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX );
  
    fifo8_init(&plc_rx_fifo,1,PLC_RTX_BUF_DEPTH);
		fifo8_init(&plc_tx_fifo,1,PLC_RTX_BUF_DEPTH);
    return 0;
}

INIT_APP_EXPORT(plc_init);

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(plc_tx, plc rtx test.);
#endif
