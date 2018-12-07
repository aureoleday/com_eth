#include <rtthread.h> 
#include "touch.h"
#include "ft5426.h"
#include "xpt2046.h"
#include "lcd.h"
#include "drv_led.h"
#include "sys_conf.h"

/*********************************************************************************
*********************�������� STM32F407Ӧ�ÿ�����(�����)*************************
**********************************************************************************
* �ļ�����: touch.c                                                              *
* �ļ����������������Գ���                                                       *
* �������ڣ�2018.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����                                                                     * 
**********************************************************************************
*********************************************************************************/

touch_st touch_inst;

//��������ʼ��
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

//�����Ļ
void Clear_Screen(void)
{
	LCD_Clear(WHITE);//����   
 	BRUSH_COLOR=BLUE;//��������Ϊ��ɫ 
	LCD_DisplayString(lcd_width-40,lcd_height-18,16,"Clear");//��ʾ��������
  BRUSH_COLOR=RED;//���û�����ɫ 
}

/****************************************************************************
* ��    ��: u8 Draw_Point(u16 x,u16 y,u16 color)
* ��    �ܣ���һ����(4*4�ĵ�)	
* ��ڲ�����x,y:����
            color:��ɫ
* ���ز���: ��  
* ˵    ����        
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

//���败�������Ժ���
void R_Touch_test(void)
{
	u8 i=0;	  
	while(1)
	{
		XPT2046_Scan(0); 		 
	
		 	if(touch_inst.x_down<lcd_width&&touch_inst.y_down<lcd_height)
			{	
				if(touch_inst.x_down>(lcd_width-40)&&touch_inst.y_down>lcd_height-18)
            Clear_Screen();  //�����Ļ
				else 
            Draw_Point(touch_inst.x_down,touch_inst.y_down,RED);		//��ͼ	  			   
			} 
    			
		i++;
		if(i%20==0)
      led_toggle(1);
    rt_thread_delay(5);
	}
}

//���ݴ��������Ժ���
void C_Touch_test(void)
{
	u8 i=0;	  	    

	while(1)
	{
		FT5426_Scan();
	 
//		if(x[0]>(lcd_width-40)&&y[0]>lcd_height-24)
//			{
//				Clear_Screen();//����
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

