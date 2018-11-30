/*
 * File      : gpio.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2015, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author            Notes
 * 2017-11-08     ZYH            the first version
 */
#include "board.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#ifdef RT_USING_SPI
#define SPIRXEVENT 0x01
#define SPITXEVENT 0x02
#define SPITIMEOUT 2
#define SPICRCEN 0
struct stm32_hw_spi_cs
{
    rt_uint32_t pin;
};

struct stm32_spi
{
    SPI_TypeDef *Instance;
    struct rt_spi_configuration *cfg;
    
};

static rt_err_t stm32_spi_init(SPI_TypeDef *spix, struct rt_spi_configuration *cfg)
{
//    SPI_HandleTypeDef hspi;
//    hspi.Instance = spix;
    return RT_EOK;
}

#define SPISTEP(datalen) (((datalen) == 8) ? 1 : 2)
#define SPISEND_1(reg, ptr, datalen)       \
    do                                     \
    {                                      \
        if (datalen == 8)                  \
        {                                  \
            (reg) = *(rt_uint8_t *)(ptr);  \
        }                                  \
        else                               \
        {                                  \
            (reg) = *(rt_uint16_t *)(ptr); \
        }                                  \
    } while (0)
#define SPIRECV_1(reg, ptr, datalen)      \
    do                                    \
    {                                     \
        if (datalen == 8)                 \
        {                                 \
            *(rt_uint8_t *)(ptr) = (reg); \
        }                                 \
        else                              \
        {                                 \
            *(rt_uint16_t *)(ptr) = reg;  \
        }                                 \
    } while (0)

static rt_err_t spitxrx1b(struct stm32_spi *hspi, void *rcvb, const void *sndb)
{
    rt_uint32_t padrcv = 0;
    rt_uint32_t padsnd = 0xFF;
    if (!rcvb && !sndb)
    {
        return RT_ERROR;
    }
    if (!rcvb)
    {
        rcvb = &padrcv;
    }
    if (!sndb)
    {
        sndb = &padsnd;
    }
    while (SPI_I2S_GetFlagStatus(hspi->Instance, SPI_I2S_FLAG_TXE) == RESET)
        ;
    SPISEND_1(hspi->Instance->DR, sndb, hspi->cfg->data_width);
    while (SPI_I2S_GetFlagStatus(hspi->Instance, SPI_I2S_FLAG_TXE) == RESET)
        ;
    SPIRECV_1(hspi->Instance->DR, rcvb, hspi->cfg->data_width);
    return RT_EOK;
}

static rt_uint32_t spixfer(struct rt_spi_device *device, struct rt_spi_message *message)
{
    rt_err_t res;
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);
    RT_ASSERT(device->bus->parent.user_data != RT_NULL);
    struct stm32_spi *hspi = (struct stm32_spi *)device->bus->parent.user_data;
    struct stm32_hw_spi_cs *cs = device->parent.user_data;
    if (message->cs_take)
    {
        rt_pin_write(cs->pin, 0);
    }
    const rt_uint8_t *sndb = message->send_buf;
    rt_uint8_t *rcvb = message->recv_buf;
    rt_int32_t length = message->length;
    while (length)
    {
        res = spitxrx1b(hspi, rcvb, sndb);
        if (rcvb)
        {
            rcvb += SPISTEP(hspi->cfg->data_width);
        }
        if (sndb)
        {
            sndb += SPISTEP(hspi->cfg->data_width);
        }
        if (res != RT_EOK)
        {
            break;
        }
        length--;
    }
    /* Wait until Busy flag is reset before disabling SPI */
    while (SPI_I2S_GetFlagStatus(hspi->Instance, SPI_I2S_FLAG_BSY) == SET)
        ;
    if (message->cs_release)
    {
        rt_pin_write(cs->pin, 1);
    }
    return message->length - length;
}

rt_err_t spi_configure(struct rt_spi_device *device,
                       struct rt_spi_configuration *configuration)
{
    struct stm32_spi *hspi = (struct stm32_spi *)device->bus->parent.user_data;
    hspi->cfg = configuration;
    return stm32_spi_init(hspi->Instance, configuration);
}
const struct rt_spi_ops stm_spi_ops =
{
    .configure = spi_configure,
    .xfer = spixfer,
};

void lc_SPI1_Setclock(u8 SPI_Prescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_Prescaler));
	SPI1->CR1&=0XFFC7;                           //先将bit3-5清零 
	SPI1->CR1|=SPI_Prescaler;	                   //设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE);                        //使能SPI1
} 


static void lc_SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);           //使能GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);            //使能SPI1时钟
 
  //GPIOFB3,4,5IO口初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; //PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;              //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                    //上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);                          //初始化IO口
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);           //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);           //PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);           //PB5复用为 SPI1
 
	//SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);             //复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);            //停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                      //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                          //CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE);    //使能SPI外设
	 
} 

void lc_SPI2_Setclock(u8 SPI_Prescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_Prescaler));
	SPI2->CR1&=0XFFC7;                           //先将bit3-5清零 
	SPI2->CR1|=SPI_Prescaler;	                   //设置SPI1速度 
	SPI_Cmd(SPI2,ENABLE);                        //使能SPI1
} 


static void lc_SPI2_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);           //使能GPIOB时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);           //使能GPIOC时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);            //使能SPI2时钟
 
  //GPIOFB3,4,5IO口初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; //PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;              //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                    //上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);                          //初始化IO口
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;              //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                    //上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);                          //初始化IO口  
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource2,GPIO_AF_SPI2);           //PB3复用为 SPI2
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource3,GPIO_AF_SPI2);           //PB4复用为 SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);           //PB5复用为 SPI2
 
	//SPI口初始化
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);             //复位SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);            //停止复位SPI2

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                      //串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	                      //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                          //CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE);    //使能SPI外设
	 
} 


void HAL_SPI_MspInit(void)
{
    lc_SPI1_Init();
    lc_SPI1_Setclock(SPI_BaudRatePrescaler_2);
    lc_SPI2_Init();
    lc_SPI2_Setclock(SPI_BaudRatePrescaler_2);  
}

void HAL_SPI_MspDeInit(void)
{
}

struct rt_spi_bus _spi_bus1, _spi_bus2, _spi_bus3;
struct stm32_spi _spi1, _spi2, _spi3;
int stm32_spi_register_bus(SPI_TypeDef *SPIx, const char *name)
{
    struct rt_spi_bus *spi_bus;
    struct stm32_spi *spi;
    if (SPIx == SPI1)
    {
        spi_bus = &_spi_bus1;
        spi = &_spi1;
    }
    else if (SPIx == SPI2)
    {
        spi_bus = &_spi_bus2;
        spi = &_spi2;
    }
    else if (SPIx == SPI3)
    {
        spi_bus = &_spi_bus3;
        spi = &_spi3;
    }
    else
    {
        return -1;
    }
    spi->Instance = SPIx;
    spi_bus->parent.user_data = spi;
    HAL_SPI_MspInit();
    return rt_spi_bus_register(spi_bus, name, &stm_spi_ops);
}

//cannot be used before completion init
rt_err_t stm32_spi_bus_attach_device(rt_uint32_t pin, const char *bus_name, const char *device_name)
{
    struct rt_spi_device *spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device));
    RT_ASSERT(spi_device != RT_NULL);
    struct stm32_hw_spi_cs *cs_pin = (struct stm32_hw_spi_cs *)rt_malloc(sizeof(struct stm32_hw_spi_cs));
    RT_ASSERT(cs_pin != RT_NULL);
    cs_pin->pin = pin;
    rt_pin_mode(pin, PIN_MODE_OUTPUT);
    rt_pin_write(pin, 1);
    return rt_spi_bus_attach_device(spi_device, device_name, bus_name, (void *)cs_pin);
}

int stm32_hw_spi_init(void)
{
    int result = 0;
#ifdef RT_USING_SPI1
    result = stm32_spi_register_bus(SPI1, "spi1");
#endif
#ifdef RT_USING_SPI2
    result = stm32_spi_register_bus(SPI2, "spi2");
#endif
#ifdef RT_USING_SPI3
    result = stm32_spi_register_bus(SPI3, "spi3");
#endif
    return result;
}
INIT_BOARD_EXPORT(stm32_hw_spi_init);

#endif /*RT_USING_SPI*/
