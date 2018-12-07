#include <rtthread.h> 
#include "touch.h"
#include "ft5426.h"
#include "xpt2046.h"
#include "lcd.h"
#include "drv_led.h"
#include "sys_conf.h"

/*********************************************************************************
*********************启明欣欣 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: touch.c                                                              *
* 文件简述：触摸屏测试程序                                                       *
* 创建日期：2018.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
**********************************************************************************
*********************************************************************************/

touch_st touch_inst;

//触摸屏初始化
void Touch_Init(void)
{
  extern sys_reg_st					g_sys;
	if(g_sys.stat.gen.lcd_type==0x9341)
	{
	  XPT2046_Init();
    rt_kprintf("TOUCH:XPT2046.\n");
	}
	else if(g_sys.stat.gen.lcd_type==0x1963)
	{
	  FT5426_Init();
    rt_kprintf("TOUCH:FT5426.\n");
	}
  else
  {
    rt_kprintf("TOUCH not initialized! err: %x\n",g_sys.stat.gen.lcd_type);
  }
}

//清空屏幕
void Clear_Screen(void)
{
	LCD_Clear(WHITE);//清屏   
 	BRUSH_COLOR=BLUE;//设置字体为蓝色 
	LCD_DisplayString(lcd_width-40,lcd_height-18,16,"Clear");//显示清屏区域
  BRUSH_COLOR=RED;//设置画笔蓝色 
}

/****************************************************************************
* 名    称: u8 Draw_Point(u16 x,u16 y,u16 color)
* 功    能：画一个点(4*4的点)	
* 入口参数：x,y:坐标
            color:颜色
* 返回参数: 无  
* 说    明：        
****************************************************************************/
void Draw_Point(u16 x,u16 y,u16 color)
{	    	
	u8 i=0;
//  u16 color_temp;
  
//  color_temp = BRUSH_COLOR;
	BRUSH_COLOR=color;
	for(i=0;i<4;i++)
	{
	 LCD_DrawPoint(x,y+i);
	 LCD_DrawPoint(x+1,y+i);
	 LCD_DrawPoint(x+2,y+i);
	 LCD_DrawPoint(x+3,y+i);
	} 	  	
//  BRUSH_COLOR = color_temp;
}	

//电阻触摸屏测试函数
void R_Touch_test(void)
{
	u8 i=0;	  
	while(1)
	{
		XPT2046_Scan(0); 		 
	
		 	if(touch_inst.x_down<lcd_width&&touch_inst.y_down<lcd_height)
			{	
				if(touch_inst.x_down>(lcd_width-40)&&touch_inst.y_down>lcd_height-18)
            Clear_Screen();  //清空屏幕
				else 
            Draw_Point(touch_inst.x_down,touch_inst.y_down,RED);		//画图	  			   
			} 
    			
		i++;
		if(i%20==0)
      led_toggle(1);
    rt_thread_delay(5);
	}
}

//电容触摸屏测试函数
void C_Touch_test(void)
{
	u8 i=0;	  	    

	while(1)
	{
		FT5426_Scan();
	 
//		if(x[0]>(lcd_width-40)&&y[0]>lcd_height-24)
//			{
//				Clear_Screen();//清屏
//			}

		i++;
		if(i%20==0)
        led_toggle(1);
    rt_thread_delay(5);
	}	
}

void touch_scan(void)
{
    extern sys_reg_st g_sys;
    if(g_sys.stat.gen.lcd_type==0x9341)
        XPT2046_Scan(0);
    else if(g_sys.stat.gen.lcd_type==0x1963)
        FT5426_Scan();
    else
        ;      
}

