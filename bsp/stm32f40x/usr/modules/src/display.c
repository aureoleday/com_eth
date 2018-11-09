#include <rtthread.h>
#include "sys_conf.h"
#include "bit_op.h"
#include "user_mb_app.h"
#include "lcd.h"
#include "display.h"
#include "hanzi.h"
#include "stdio.h"
/*
0:    barometric pressure (mbar)
1:    temprature(C) x 100
2:    PH x 100
3:    oxidation/reduction potential(mv x 10)
4:    turbidity (NTU x 10)

9:    Electrical conductivity (uS /cm)(upper 16bit)
10:   Electrical conductivity (uS /cm)(lower 16bit)
11:   Electrical resistivity (O.cm) upper 16 bits
12:   Electrical resistivity (O.cm) lower 16 bits
13:   Sanility(PSU x 100)
14:   Total dissolved solids (mg/L) upper 16 bits
15:   Total dissolved solids (mg/L) lower 16 bits
16:   Specific seawater gravity (dt x 10)
17:   Desolved Oxigen (mg/L x 100)
18:   Desolved Oxigen (% air saturation x 10)
19:   probe depth (cm)

32:   Ammonia concentration(mg/L) upper 16 bits
33:   Ammonia concentration(mg/L) lower 16 bits
*/
sensor_namesting_st sname_inst = 
{
    "Barometric:",
    "Temprature:",
    "Ph:",
    "Oxidation:",    
    "Turbidity:",
    "Condutivity:",
    "Resistivity:",
    "Sanility:",
    "Tds:",
    "Ssg:",
    "Des_oxigen:",
    "Des_oxigen_air:",
    "Probe_depth:",
    "Amm_concentra:"   
};

sensor_namesting_st csname_inst = 
{
    "大气压",
    "温度",
    "酸碱度",
    "氧化还原电位",    
    "浊度",
    "电导率",
    "电阻率",
    "盐度",
    "总溶解固体",
    "海水比重",
    "溶解氧",
    "氧溶量",
    "深度",
    "氨浓度"   
};

sensor_namesting_st sunit_inst = 
{
    "mbar",
    "~C",
    "N/A",
    "mv",
    "NTU",
    "uS/cm",
    "=.cm",
    "PSU",
    "mg/L",
    "^t",
    "mg/L",
    "%",
    "cm",
    "mg/L"   
};

uint16_t color_bkg = BLACK;
uint16_t color_title = 0x463f;
uint16_t color_brush = WHITE;
uint16_t color_sensor = WHITE;
uint16_t color_value = YELLOW;
uint16_t color_time = GRAY;

disp_st disp_inst;
static void lcd_print_time(uint16_t x,uint16_t y,uint16_t size);
//static void disp_name(void)
//{
//    LCD_DisplayString(10,10,16, (u8*)&sname_inst.barometric);
//    LCD_DisplayString(10,30,16, (u8*)&sname_inst.temprature);
//    LCD_DisplayString(10,50,16, (u8*)&sname_inst.ph);
//    LCD_DisplayString(10,70,16, (u8*)&sname_inst.oxidation);
//    LCD_DisplayString(10,90,16, (u8*)&sname_inst.turbidity);
//    LCD_DisplayString(10,110,16,(u8*)&sname_inst.condutivity);
//    LCD_DisplayString(10,130,16,(u8*)&sname_inst.resistivity);
//    LCD_DisplayString(10,150,16,(u8*)&sname_inst.sanility);
//    LCD_DisplayString(10,170,16,(u8*)&sname_inst.tds);
//    LCD_DisplayString(10,190,16,(u8*)&sname_inst.ssg);
//    LCD_DisplayString(10,210,16,(u8*)&sname_inst.desolved_oxigen);
//    LCD_DisplayString(10,230,16,(u8*)&sname_inst.desolved_oxigen_air);
//    LCD_DisplayString(10,250,16,(u8*)&sname_inst.probe_depth);
//    LCD_DisplayString(10,270,16,(u8*)&sname_inst.ammonia_concentration);
//}

static void disp_ver(uint16_t x,uint16_t y,uint16_t size);

static void disp_cname(void)
{
    disp_ver(200,0,16);
    LCD_DisplayCString_color(5,0,  "易科捷生态科技有限公司",color_title,color_bkg);    
    LCD_DisplayCString(10,20, (char*)&csname_inst.barometric);
    LCD_DisplayCString(10,40, (char*)&csname_inst.temprature);
    LCD_DisplayCString(10,60, (char*)&csname_inst.ph);
    LCD_DisplayCString(10,80, (char*)&csname_inst.oxidation);
    LCD_DisplayCString(10,100,(char*)&csname_inst.turbidity);
    LCD_DisplayCString(10,120,(char*)&csname_inst.condutivity);
    LCD_DisplayCString(10,140,(char*)&csname_inst.resistivity);
    LCD_DisplayCString(10,160,(char*)&csname_inst.sanility);
    LCD_DisplayCString(10,180,(char*)&csname_inst.tds);
//    LCD_DisplayCString(10,200,(char*)&csname_inst.ssg);
    LCD_DisplayCString(10,200,(char*)&csname_inst.desolved_oxigen);
//    LCD_DisplayCString(10,240,(char*)&csname_inst.desolved_oxigen_air);
    LCD_DisplayCString(10,220,(char*)&csname_inst.probe_depth);
    LCD_DisplayCString(10,240,(char*)&csname_inst.ammonia_concentration);    
}

static void disp_unit(void)
{
    LCD_DisplayString(200,20, 16, (u8*)&sunit_inst.barometric);
    LCD_DisplayString(200,40, 16, (u8*)&sunit_inst.temprature);
    LCD_DisplayString(200,60, 16, (u8*)&sunit_inst.ph);
    LCD_DisplayString(200,80, 16, (u8*)&sunit_inst.oxidation);
    LCD_DisplayString(200,100,16, (u8*)&sunit_inst.turbidity);
    LCD_DisplayString(200,120,16,(u8*)&sunit_inst.condutivity);
    LCD_DisplayString(200,140,16,(u8*)&sunit_inst.resistivity);
    LCD_DisplayString(200,160,16,(u8*)&sunit_inst.sanility);
    LCD_DisplayString(200,180,16,(u8*)&sunit_inst.tds);
//    LCD_DisplayString(200,200,16,(u8*)&sunit_inst.ssg);
    LCD_DisplayString(200,200,16,(u8*)&sunit_inst.desolved_oxigen);
//    LCD_DisplayString(200,240,16,(u8*)&sunit_inst.desolved_oxigen_air);
    LCD_DisplayString(200,220,16,(u8*)&sunit_inst.probe_depth);
    LCD_DisplayString(200,240,16,(u8*)&sunit_inst.ammonia_concentration);    
}


void disp_init(void)
{
    extern sys_reg_st  g_sys;
    if(LCD_Init() == 0)
        bit_op_set(&g_sys.stat.gen.status_bm,GBM_LCD,1);
//        g_sys.stat.gen.status_bm |= (0x00000001<<GBM_LCD);
    else
        bit_op_set(&g_sys.stat.gen.status_bm,GBM_LCD,0);
//        g_sys.stat.gen.status_bm &= ~(0x00000001<<GBM_LCD);
    LCD_Clear(color_bkg);
    BACK_COLOR=color_bkg;
    BRUSH_COLOR=color_brush; 
    disp_cname();
    disp_unit();
}

static void update_sensor(uint8_t addr_ex,uint8_t type)
{
    extern  mbm_dev_st mbm_dev_inst;
    uint8_t addr;
    addr = addr_ex - 1;
    
    switch(type)
    {
        case(SENSOR_MULTI):
        {
            if((mbm_dev_inst.sts_bitmap_input & (1<<addr)) != 0 )
            {
                disp_inst.multi.barometric = mbm_dev_inst.inputbuf_reg[addr][0];
                disp_inst.multi.temprature = mbm_dev_inst.inputbuf_reg[addr][1];
                disp_inst.multi.ph = mbm_dev_inst.inputbuf_reg[addr][2];
                disp_inst.multi.oxidation = mbm_dev_inst.inputbuf_reg[addr][3];
                disp_inst.multi.turbidity = mbm_dev_inst.inputbuf_reg[addr][4];              
                disp_inst.multi.condutivity = (mbm_dev_inst.inputbuf_reg[addr][5]<<16)| mbm_dev_inst.inputbuf_reg[addr][6];
                disp_inst.multi.resistivity = (mbm_dev_inst.inputbuf_reg[addr][11]<<16)| mbm_dev_inst.inputbuf_reg[addr][12];
                disp_inst.multi.sanility = mbm_dev_inst.inputbuf_reg[addr][13];
                disp_inst.multi.tds = (mbm_dev_inst.inputbuf_reg[addr][14]<<16)| mbm_dev_inst.inputbuf_reg[addr][15];
                disp_inst.multi.ssg = mbm_dev_inst.inputbuf_reg[addr][16];
                disp_inst.multi.desolved_oxigen = mbm_dev_inst.inputbuf_reg[addr][17];
                disp_inst.multi.desolved_oxigen_air = mbm_dev_inst.inputbuf_reg[addr][18];
                disp_inst.multi.probe_depth = mbm_dev_inst.inputbuf_reg[addr][19];
                disp_inst.multi.ammonia_concentration = (mbm_dev_inst.inputbuf_reg[addr][32]<<16)| mbm_dev_inst.inputbuf_reg[addr][33];
            }
            break;
        }
        case(SENSOR_PH):
        {
            if((mbm_dev_inst.sts_bitmap_hold & (1<<addr)) == 1 )
            {
                disp_inst.ph.ph_val = mbm_dev_inst.holdbuf_reg[addr][0];
                disp_inst.ph.temprature = mbm_dev_inst.holdbuf_reg[addr][1];
            }
            break;
        }
        case(SENSOR_DO2):
        {
            if((mbm_dev_inst.sts_bitmap_hold & (1<<addr)) == 1 )
            {
                disp_inst.do2.desolved_o2 = mbm_dev_inst.holdbuf_reg[addr][0];
                disp_inst.do2.temprature = mbm_dev_inst.holdbuf_reg[addr][1];
            }
            break;
        }        
        default:
        {
            break;
        }
    }
}

static void disp_data(uint16_t x,uint16_t y,uint16_t size, uint32_t data,uint16_t dn)
{
    float   temp;
    char    dbuf[32];
    
    if(dn>1)
    {
        temp = (float)data/dn;
        sprintf(dbuf,"%.2f",temp);
    }
    else
    {
        temp = data;
        sprintf(dbuf,"%d",(uint32_t)temp);
    }
        
    LCD_DisplayString_color(x,y,size, (u8 *)dbuf,color_value,color_bkg);    
}

static void disp_xdata(uint16_t x,uint16_t y,uint16_t size, uint32_t data,uint16_t dn)
{
    float   temp;
    char    dbuf[32];
    
    if(dn>1)
    {
        temp = (float)data/dn;
        sprintf(dbuf,"%.2f",temp);
    }
    else
    {
        temp = data;
        sprintf(dbuf,"%x",(uint32_t)temp);
    }
        
    LCD_DisplayString_color(x,y,size, (u8 *)dbuf,color_value,color_bkg);    
}

static void disp_ver(uint16_t x,uint16_t y,uint16_t size)
{
    uint32_t main_ver,sub_ver;
    char    dbuf[32];   

    main_ver = SOFTWARE_VER>>16;
    sub_ver = SOFTWARE_VER&0x0000ffff;
  
    sprintf(dbuf,"V%d.%d",main_ver,sub_ver);
        
    LCD_DisplayString_color(x,y,size, (u8 *)dbuf,color_brush,color_bkg);    
}

void disp_fresh(void)
{
    extern  sys_reg_st  g_sys;
    update_sensor(1,SENSOR_MULTI);
    disp_xdata(220, 300, 16,g_sys.stat.gen.status_bm,1);
    disp_data(135,20, 16,disp_inst.multi.barometric,1);
    disp_data(135,40, 16,disp_inst.multi.temprature,100);
    disp_data(135,60, 16,disp_inst.multi.ph,100);
    disp_data(135,80, 16,disp_inst.multi.oxidation,10);
    disp_data(135,100,16,disp_inst.multi.turbidity,10);
    disp_data(135,120,16,disp_inst.multi.condutivity,1);
    disp_data(135,140,16,disp_inst.multi.resistivity,1);
    disp_data(135,160,16,disp_inst.multi.sanility,100);
    disp_data(135,180,16,disp_inst.multi.tds,1);
//    disp_data(135,200,16,disp_inst.multi.ssg,10);
    disp_data(135,200,16,disp_inst.multi.desolved_oxigen,100);
//    disp_data(135,240,16,disp_inst.multi.desolved_oxigen_air,10);
    disp_data(135,220,16,disp_inst.multi.probe_depth,1);
    disp_data(135,240,16,disp_inst.multi.ammonia_concentration,1);
    lcd_print_time(0,300,16);
}

static void disp_info(void)
{   
    rt_kprintf("bp: %d\n",disp_inst.multi.barometric);
    rt_kprintf("temprature: %d\n",disp_inst.multi.temprature);
    rt_kprintf("oxidation: %d\n",disp_inst.multi.oxidation);
    rt_kprintf("ph: %d\n",disp_inst.multi.ph);
    rt_kprintf("turbidity: %d\n",disp_inst.multi.turbidity);
    rt_kprintf("condutivity: %d\n",disp_inst.multi.condutivity);
    rt_kprintf("resistivity: %d\n",disp_inst.multi.resistivity);
    rt_kprintf("Sanility: %d\n",disp_inst.multi.sanility);
    rt_kprintf("tds: %d\n",disp_inst.multi.tds);
    rt_kprintf("ssg: %d\n",disp_inst.multi.ssg);
    rt_kprintf("desolved_oxigen: %d\n",disp_inst.multi.desolved_oxigen);
    rt_kprintf("desolved_oxigen_air: %d\n",disp_inst.multi.desolved_oxigen_air);
    rt_kprintf("probe_depth: %d\n",disp_inst.multi.probe_depth);
    rt_kprintf("ammonia_concentration: %d\n",disp_inst.multi.ammonia_concentration);
    
}


uint8_t cp[] = 
{
    0x08,0x00,0x08,0x00,0x08,0x00,0x13,0xFC,
    0x10,0x00,0x30,0x00,0x30,0x00,0x50,0x00,
    0x90,0x00,0x10,0x00,0x10,0x00,0x10,0x00,
    0x17,0xFE,0x10,0x00,0x10,0x00,0x10,0x00
};

void LCD_ShowChinese_fill(u16 x,u16 y,u8 fontSize,u16 pointColor,u16 backColor,u8 *pChineseData)
{
    u32 i;  
    u8 j =0;
    u8 k =0;     
    u8 d =0;     
    for(i=0;i<(fontSize*fontSize);i++)
    {
        if((*pChineseData)&(1<<(7-j)))
        {
            LCD_Color_DrawPoint(x+j+k*8, y+d, pointColor);
        }
        else
        {
            LCD_Color_DrawPoint(x+j+k*8, y+d, backColor);
        }
        j = j +1;
        if(j ==8)
        {
            j =0;
            pChineseData++;
            k++;       
            if(k>=(fontSize/8))
            {
                k=0;
                d++;
            }
        }
    }
}


static void lcd_print_time(uint16_t x,uint16_t y,uint16_t size)
{
    time_t  now;
    char    dbuf[32];
    
    now = time(RT_NULL);
    sprintf(dbuf,"%s",ctime(&now));

    LCD_DisplayString_color(x,y,size, (u8 *)dbuf,color_time,color_bkg);   
        
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(disp_info, dispaly general info.);
#endif

