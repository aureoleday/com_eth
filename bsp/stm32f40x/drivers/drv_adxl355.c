#include <rtthread.h>
#include <rtdevice.h>
#include "drv_spi.h"
#include "drv_adxl355.h"
#include "fifo.h"

#define DEV_GEO_RTX_SIZE    2048
#define DEV_GEO_FIFO_SIZE   256
//const static float t_sens = -9.05;    
//const static float t_bias = 1852;  

fifo32_cb_td geo_rx_fifo;
static uint8_t rxd_temp[DEV_GEO_RTX_SIZE];

typedef struct
{
    uint32_t  x;
    uint32_t  y;
    uint32_t  z;
}geo_loc_st;

struct spi_geo_device
{
    struct rt_spi_device *          rt_spi_device;
    uint8_t                         txd[DEV_GEO_RTX_SIZE];
    uint8_t                         rxd[DEV_GEO_RTX_SIZE];
  
    ADXL355_calibdata_t calib_data; 
  
    float axis355_sens;
    float axis357_sens;       
};

struct spi_geo_device spi_geo_device;

void geo_ds_init(void)
{
    fifo32_init(&geo_rx_fifo,1,DEV_GEO_FIFO_SIZE);
}

rt_err_t adxl_wr_reg(uint8_t addr, uint8_t* data, uint8_t cnt)
{  
    rt_err_t ret;
    uint8_t i;
    spi_geo_device.txd[0] = (addr<<1);
    for(i=0;i<cnt;i++)
    {
        spi_geo_device.txd[1+i] = *(data+i);
    }
    ret = rt_spi_transfer(spi_geo_device.rt_spi_device,spi_geo_device.txd,RT_NULL,(cnt+1));
    return ret;
}

uint8_t adxl_rd_reg(uint8_t addr, uint8_t * rx_buf, uint8_t cnt)
{  
    uint8_t i;
    spi_geo_device.txd[0] = (addr<<1)|0x01;  
    for(i=0;i<cnt;i++)
    {
        spi_geo_device.txd[1+i] = 0;
    }
    rt_spi_transfer(spi_geo_device.rt_spi_device,spi_geo_device.txd,rx_buf,(cnt+1));
    return *(rx_buf+1);
}

void adxl355_set_power_ctl_reg(uint8_t data)
{
     adxl_wr_reg(POWER_CTL, &data,1);
}
void adxl355_set_filter_ctl_reg(ADXL355_filter_ctl_t hpf, ADXL355_filter_ctl_t odr)
{
    uint8_t temp;
    temp = hpf|odr;
    adxl_wr_reg(FILTER, &temp,1);
}
void adxl355_set_clk(ADXL355_sync_ctl_t data) 
{
    uint8_t temp;
    temp = data;  
    adxl_wr_reg(SYNC, &temp,1);
}
void adxl355_set_device(ADXL355_range_ctl_t range) 
{
    uint8_t temp;
    temp = range;  
    adxl_wr_reg(RANGE, &temp,1);
    switch(range){
        case 0x01:
            spi_geo_device.axis355_sens = 3.9e-6;
            spi_geo_device.axis357_sens = 19.5e-6;
            break;
        case 0x02:
            spi_geo_device.axis355_sens = 7.8e-6;
            spi_geo_device.axis357_sens = 39e-6;
            break;
        case 0x03:
            spi_geo_device.axis355_sens = 15.6e-6;
            spi_geo_device.axis357_sens = 78e-6;
            break;
        }
}
/** ----------------------------------- */
/** Read the STATUS registers           */
/** ----------------------------------- */
uint8_t adxl355_read_status()
{
    uint8_t temp[3];
    adxl_rd_reg(STATUS,temp,1);
    return temp[1];
}
/** ----------------------------------- */
/** ADXL must be set in measurement     */
/** mode to read the data registers     */
/** ----------------------------------- */
uint32_t adxl355_scanx(){
    uint8_t temp[4];
    uint32_t ret;
    adxl_rd_reg(XDATA3,temp,3);
    ret = (temp[1]<<16)|(temp[2]<<8)|temp[3];
    return ret;
}
uint32_t adxl355_scany()
{
    uint8_t temp[4];
    uint32_t ret;
    adxl_rd_reg(YDATA3,temp,3);
    ret = (temp[1]<<16)|(temp[2]<<8)|temp[3];
    return ret;
}
uint32_t adxl355_scanz()
{
    uint8_t temp[4];
    uint32_t ret;
    adxl_rd_reg(ZDATA3,temp,3);
    ret = (temp[1]<<16)|(temp[2]<<8)|temp[3];
    return ret;
}
uint16_t adxl355_scant(){
    uint8_t temp[3];
    uint16_t ret;
    adxl_rd_reg(TEMP2,temp,2);
    ret = (temp[1]<<8)|temp[2];
    return ret;
}

uint16_t adxl355_scanfifo(void)
{
    uint16_t ret;
    uint16_t i;
    uint16_t total_cnt;  
    uint8_t  sample_cnt;
    uint32_t buf_temp;
    uint8_t status;
  
    status = adxl_rd_reg(STATUS,rxd_temp,1);
    if((status&0x6) != 0)
        rt_kprintf("adxl_fifo fuov!\n");
    
    sample_cnt = adxl_rd_reg(FIFO_ENTRIES,rxd_temp,1);
    
    total_cnt = sample_cnt*3*3;
  
    if(rxd_temp[1] > 0)
    {
        adxl_rd_reg(FIFO_DATA, rxd_temp, total_cnt);
    }
    
    for(i=0;i<sample_cnt;i++)
    {
        buf_temp = (rxd_temp[1+i*3]<<16)|(rxd_temp[2+i*3]<<8)|(rxd_temp[3+i*3]);
        if(fifo32_push(&geo_rx_fifo,&buf_temp) == 0)
            rt_kprintf("geo fifo full\n");
    }
    return ret;
}

static void fifo_peak(uint16_t cnt)
{
    uint16_t i;
    uint32_t temp;
    for(i=0;i<cnt;i++)
    {
        fifo32_pop(&geo_rx_fifo,&temp);
        rt_kprintf("d: %x\n",temp);
    }
}

/** ----------------------------------- */
/** Activity SetUp - the measured       */
/** acceleration on any axis is above   */ 
/** the ACT_THRESH bits for ACT_COUNT   */
/** consecutive measurements.           */
/** ----------------------------------- */

void adxl355_set_activity_axis(ADXL355_act_ctl_t axis) 
{
    uint8_t temp;
    temp = axis;  
    adxl_wr_reg(ACT_EN, &temp,1);
}

void adxl355_set_activity_cnt(uint8_t count) 
{
    uint8_t temp;
    temp = count;  
    adxl_wr_reg(ACT_COUNT, &temp,1);
}

void adxl355_set_activity_threshold(uint8_t data_h, uint8_t data_l) 
{
    uint8_t temp[2];
    temp[0] = data_h;
    temp[1] = data_l;
    adxl_wr_reg(ACT_THRESH_H, temp,2);
}
void adxl355_set_inactivity(void) 
{
    uint8_t temp;
    temp = 0;  
    adxl_wr_reg(ACT_EN, &temp,1); 
}


uint8_t adxl355_reset(void)
{
    uint8_t buf;
    buf = 0x52;
    adxl_wr_reg(ARESET,&buf,1);
    return 0;
}

uint8_t adxl355_activate(uint8_t enable)
{
    uint8_t buf;
    if(enable)
    {
//        buf = 0x52;
//        adxl_wr_reg(ARESET,&buf,1);
        fifo32_reset(&geo_rx_fifo);
        buf = 0;
    }
    else
    {
        buf =  1;        
    }
    adxl_wr_reg(POWER_CTL,&buf,1);
    
    return 0;
}

int adxl355_init(void)
{
    struct rt_spi_device * rt_spi_device;
    char spi_device_name[] = "geo_spi";    
  
    stm32_spi_bus_attach_device(RT_GEO_CS_PIN, RT_GEO_SPI_BUS_NAME, spi_device_name);
  
    rt_spi_device = (struct rt_spi_device *)rt_device_find(spi_device_name);
    if(rt_spi_device == RT_NULL)
    {
        return -RT_ENOSYS;
    }
    
    spi_geo_device.rt_spi_device = rt_spi_device;
    
    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = RT_SPI_MODE_0 | RT_SPI_MSB; /* SPI Compatible: Mode 0 and Mode 3 */
        cfg.max_hz = 40 * 1000 * 1000; /* 50M */
        rt_spi_configure(spi_geo_device.rt_spi_device, &cfg);
    }
    
    geo_ds_init();
    return RT_EOK;
}


static void ad_wr_reg(uint8_t addr, uint8_t data)
{
    adxl_wr_reg(addr,&data,1);
}

static void ad_rd_reg(uint8_t addr,uint8_t cnt)
{
    uint8_t data[16];
    uint8_t i;
    adxl_rd_reg(addr,data,cnt);
    rt_kprintf("reg : value\n");
    for(i=0;i<cnt;i++)
    {
        rt_kprintf(" %x  : %x\n",(addr+i),data[i+1]);
    }  
}

INIT_DEVICE_EXPORT(adxl355_init);
FINSH_FUNCTION_EXPORT(adxl355_scanfifo, scan fifo.);
FINSH_FUNCTION_EXPORT(fifo_peak, peak fifo content.);
FINSH_FUNCTION_EXPORT(ad_wr_reg, geo spi reg wr.);
FINSH_FUNCTION_EXPORT(ad_rd_reg, geo spi reg rd.);
