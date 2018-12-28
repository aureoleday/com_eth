#include <rtthread.h>
#include "sys_conf.h"
#include "bit_op.h"
#include "user_mb_app.h"
#include "lcd.h"
#include "display.h"
#include "hanzi.h"
#include "stdio.h"
#include "drv_timer.h"
#include "GUI.h"
#include "GUIDemo.h"
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

const uint16_t opt_list[] =
{ 1,      //barometer
  100,    //tempreture
  100,    //PH
  10,     //oxidation
  10,     //conductivity
  1,      //resistivity
  100,    //salinity
  1,      //tds
  100,    //disolved oxigen
  1,      //depth
  1,      //nitrite
  1,     //ammonia
  1,
  1,
  1,
};


//disp_st disp_inst;
disp_sen_st disp_sen_inst;
int32_t ma_buf[64];

static void disp_sen_inst_init(void)
{
    int i = 0;
    for(i=0;i<DPOS_SENSOR_MAX;i++)
    {
        disp_sen_inst.sid[i].val = 0;
        disp_sen_inst.sid[i].opt = opt_list[i];
    }
    for(i=0;i<64;i++)
    {
        ma_buf[i] = 0;
    }
}

#include "sram.h"
#include "malloc.h"

void disp_init(void)
{
    extern sys_reg_st  g_sys;
    if(LCD_Init() == 0)
        bit_op_set(&g_sys.stat.gen.status_bm,GBM_LCD,1);
    else
        bit_op_set(&g_sys.stat.gen.status_bm,GBM_LCD,0);
    disp_sen_inst_init();
    drv_timer_init();
    FSMC_SRAM_Init();
    Memory_Init(EXSRAM);
    MainTask();
}

static int32_t moving_avg(int32_t data, int16_t ma_cnt)
{
    int i;
    int32_t sum=0;
  
    for(i=ma_cnt;i>0;i--)
    {
        ma_buf[i] = ma_buf[i-1];
    }
    ma_buf[0] = data;
    
    for(i=0;i<ma_cnt;i++)
    {
        sum += ma_buf[i];
    }
    sum = sum/ma_cnt;
    
    return sum;
}

static void update_sensor(uint8_t addr_ex,uint8_t type)
{
    extern sys_reg_st  g_sys;
    extern  mbm_dev_st mbm_dev_inst;
    uint8_t addr;
    addr = addr_ex - 1;
  
    switch(type)
    {
        case(SENSOR_MULTI):
        {
            if((mbm_dev_inst.sts_bitmap_input & (1<<addr)) != 0 )
            {              
                disp_sen_inst.sid[DPOS_BARO].val = mbm_dev_inst.inputbuf_reg[addr][0];
                disp_sen_inst.sid[DPOS_TEMP].val = mbm_dev_inst.inputbuf_reg[addr][1];
                disp_sen_inst.sid[DPOS_PH].val= mbm_dev_inst.inputbuf_reg[addr][2];
                disp_sen_inst.sid[DPOS_OXIDATION].val = mbm_dev_inst.inputbuf_reg[addr][3];
                disp_sen_inst.sid[DPOS_TURB].val = mbm_dev_inst.inputbuf_reg[addr][4];              
                disp_sen_inst.sid[DPOS_CONDUCTIVITY].val = (mbm_dev_inst.inputbuf_reg[addr][5]<<16)| mbm_dev_inst.inputbuf_reg[addr][6];
                disp_sen_inst.sid[DPOS_RESISTIVITY].val = (mbm_dev_inst.inputbuf_reg[addr][11]<<16)| mbm_dev_inst.inputbuf_reg[addr][12];
                disp_sen_inst.sid[DPOS_SANILITY].val = mbm_dev_inst.inputbuf_reg[addr][13];
                disp_sen_inst.sid[DPOS_TDS].val = (mbm_dev_inst.inputbuf_reg[addr][14]<<16)| mbm_dev_inst.inputbuf_reg[addr][15];
                disp_sen_inst.sid[DPOS_SSG].val = mbm_dev_inst.inputbuf_reg[addr][16];
                disp_sen_inst.sid[DPOS_DESOVLED_OXIGEN].val = mbm_dev_inst.inputbuf_reg[addr][17];
                disp_sen_inst.sid[DPOS_DESOVLED_OXIGEN_AIR].val = mbm_dev_inst.inputbuf_reg[addr][18];
                disp_sen_inst.sid[DPOS_PROBE_DEPTH].val = mbm_dev_inst.inputbuf_reg[addr][19];
                disp_sen_inst.sid[DPOS_AMMOIA_CONCTRA].val = (mbm_dev_inst.inputbuf_reg[addr][20]<<16)| mbm_dev_inst.inputbuf_reg[addr][21];
                disp_sen_inst.sid[DPOS_NITRAT_CONCTRA].val = (mbm_dev_inst.inputbuf_reg[addr][22]<<16)| mbm_dev_inst.inputbuf_reg[addr][23];              
            }
            break;
        }
        case(SENSOR_BT):
        {
            disp_sen_inst.sid[0].opt = 10;
            disp_sen_inst.sid[1].opt = 100;
            if((mbm_dev_inst.sts_bitmap_hold & (1<<addr)) != 0 )
            {
                disp_sen_inst.sid[DPOS_BARO].val = (mbm_dev_inst.holdbuf_reg[addr][0]<<16)|mbm_dev_inst.holdbuf_reg[addr][1];
//                disp_sen_inst.sid[DPOS_TEMP].val = (mbm_dev_inst.holdbuf_reg[addr][2]<<16)|mbm_dev_inst.holdbuf_reg[addr][3] + (int32_t)(g_sys.conf.gen.temp_offset);
                disp_sen_inst.sid[DPOS_TEMP].val = moving_avg((mbm_dev_inst.holdbuf_reg[addr][2]<<16)|mbm_dev_inst.holdbuf_reg[addr][3],g_sys.conf.gen.temp_win) + (int32_t)(g_sys.conf.gen.temp_offset);
            }
            break;
        }        
        case(SENSOR_PH):
        {
            if((mbm_dev_inst.sts_bitmap_hold & (1<<addr)) != 0 )
            {
                disp_sen_inst.sid[DPOS_PH].val = (mbm_dev_inst.holdbuf_reg[addr][0]<<16)|mbm_dev_inst.holdbuf_reg[addr][1];
                disp_sen_inst.sid[DPOS_TEMP].val = (mbm_dev_inst.holdbuf_reg[addr][2]<<16)|mbm_dev_inst.holdbuf_reg[addr][3];
            }
            break;
        }
        case(SENSOR_DO2):
        {
            if((mbm_dev_inst.sts_bitmap_hold & (1<<addr)) != 0 )
            {
                disp_sen_inst.sid[DPOS_DESOVLED_OXIGEN].val = (mbm_dev_inst.holdbuf_reg[addr][0]<<16)|mbm_dev_inst.holdbuf_reg[addr][1];
                disp_sen_inst.sid[DPOS_TEMP].val = (mbm_dev_inst.holdbuf_reg[addr][2]<<16)|mbm_dev_inst.holdbuf_reg[addr][3];
            }
            break;
        }        
        default:
        {
            break;
        }
    }
}

void disp_fresh(void)
{
    extern  sys_reg_st  g_sys;
//    update_sensor(1,SENSOR_MULTI);
    update_sensor(g_sys.conf.gen.disp_dev,g_sys.conf.gen.disp_dev);
}

static void set_temp_off(int32_t offset)
{
    extern  sys_reg_st  g_sys;
    g_sys.conf.gen.temp_offset = offset;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(moving_avg, calt mav.);
FINSH_FUNCTION_EXPORT(set_temp_off, set temp offset);
#endif

