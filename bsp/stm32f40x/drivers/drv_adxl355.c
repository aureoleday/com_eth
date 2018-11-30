#include <rtthread.h>
#include <rtdevice.h>
#include "drv_spi.h"
#include "drv_adxl355.h"

#define DEV_GEO_RTX_SIZE    8

const static float t_sens = -9.05;    
const static float t_bias = 1852;  

struct spi_geo_device
{
    struct rt_spi_device *          rt_spi_device;
    uint8_t                         cmd;
    uint8_t                         txd[DEV_GEO_RTX_SIZE];
    uint8_t                         rxd[DEV_GEO_RTX_SIZE];
  
    ADXL355_calibdata_t calib_data; 
  
    float axis355_sens;
    float axis357_sens;       
};

struct spi_geo_device spi_geo_device;

rt_err_t adxl_wr_reg(uint8_t addr, uint8_t* data, uint8_t cnt)
{  
    rt_err_t ret;
    uint8_t tx_buf[DEV_GEO_RTX_SIZE];
    spi_geo_device.cmd = (addr<<1);
    ret = rt_spi_send_then_send(spi_geo_device.rt_spi_device, &spi_geo_device.cmd, 1, &data, cnt);
    return ret;
}

rt_err_t adxl_rd_reg(uint8_t addr, uint8_t * rx_buf, uint8_t cnt)
{  
    rt_err_t ret;
    spi_geo_device.cmd = (addr<<1)|0x0001;
    ret = rt_spi_send_then_recv(spi_geo_device.rt_spi_device, &spi_geo_device.cmd, 1, &ret, cnt);
    return ret;
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
    return temp[0];
}
/** ----------------------------------- */
/** ADXL must be set in measurement     */
/** mode to read the data registers     */
/** ----------------------------------- */
uint32_t adxl355_scanx(){
    uint8_t temp[3];
    uint32_t ret;
    adxl_rd_reg(XDATA3,temp,3);
    ret = (temp[0]<<16)|(temp[1]<<8)|temp[2];
    return ret;
}
uint32_t adxl355_scany()
{
    uint8_t temp[3];
    uint32_t ret;
    adxl_rd_reg(YDATA3,temp,3);
    ret = (temp[0]<<16)|(temp[1]<<8)|temp[2];
    return ret;
}
uint32_t adxl355_scanz()
{
    uint8_t temp[3];
    uint32_t ret;
    adxl_rd_reg(ZDATA3,temp,3);
    ret = (temp[0]<<16)|(temp[1]<<8)|temp[2];
    return ret;
}
uint16_t adxl355_scant(){
    uint8_t temp[3];
    uint16_t ret;
    adxl_rd_reg(TEMP2,temp,2);
    ret = (temp[0]<<8)|temp[1];
    return ret;
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

int adxl355_init(void)
{
    struct rt_spi_device * rt_spi_device;
    char spi_device_name[] = "geospi";

    stm32_spi_bus_attach_device(RT_GEO_CS_PIN, RT_GEO_SPI_BUS_NAME, "geospi");
  
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
        cfg.max_hz = 10 * 1000 * 1000; /* 10M */
        rt_spi_configure(spi_geo_device.rt_spi_device, &cfg);
    }
    return RT_EOK;
}


INIT_DEVICE_EXPORT(adxl355_init);
