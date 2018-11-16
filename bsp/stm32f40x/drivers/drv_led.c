#include <rtdevice.h>
void drv_led_init(void)
{
    rt_pin_mode(2,0); 
    rt_pin_mode(3,0);
    rt_pin_mode(22,0);
}

void led_toggle(uint8_t led_id)
{
    static uint8_t led_status[2]={0,0};
  
    rt_pin_write((led_id+2),led_status[led_id]);
    led_status[led_id] = !led_status[led_id];
//    rt_kprintf("led: %d,%d\n",led_status[0],led_id);
//    if(led_id == 0)
//        rt_pin_write(led_id,1);
//    else
//        rt_pin_write(led_id,0);
}

void lcd_bl(uint8_t flag)
{
    if(flag)
        rt_pin_write(22,1);
    else
        rt_pin_write(22,0);
}

