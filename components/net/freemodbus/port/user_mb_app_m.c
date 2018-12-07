/*
 * FreeModbus Libary: user callback functions and buffer define in master mode
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: user_mb_app_m.c,v 1.60 2013/11/23 11:49:05 Armink $
 */
#include "user_mb_app.h"
#include "sys_conf.h"
#include "bit_op.h"

//Master mode:InputRegister variables
//USHORT   usMRegInStart                              = M_REG_INPUT_START;
//USHORT   usMRegInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_INPUT_NREGS];
//Master mode:HoldingRegister variables
//USHORT   usMRegHoldStart                            = M_REG_HOLDING_START;
//USHORT   usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];
mbm_dev_st mbm_dev_inst;
extern sys_reg_st  g_sys;
/**
  * @brief  modbus master poll thread
  * @param  none
  * @retval none
**/
void mbm_thread_entry(void* parameter)
{
		eMBErrorCode    eStatus = MB_ENOERR;
		rt_thread_delay(MBM_THREAD_DELAY);    
    
    uint32_t mbm_baudrate;
    
    if((g_sys.conf.mbm.baudrate == 4800)||(g_sys.conf.mbm.baudrate == 9600)||(g_sys.conf.mbm.baudrate == 19200)||(g_sys.conf.mbm.baudrate == 38400))
        mbm_baudrate = g_sys.conf.mbm.baudrate;        
    else
    {
        rt_kprintf("MBM baudrate illegal,set to 9600\n");
        mbm_baudrate = 9600;
    }

    rt_kprintf("MBM baudrate: %d\n",mbm_baudrate);
    
    eStatus = eMBMasterInit(MB_RTU, 5, mbm_baudrate, MB_PAR_NONE);
		if(eStatus != MB_ENOERR)
		{
				rt_kprintf("MBM init fail\n");
		}
		eStatus = eMBMasterEnable();
		if(eStatus != MB_ENOERR)
		{
				rt_kprintf("MBM enable fail\n");
		}
		while(1)
		{ 
//        if(g_sys.conf.mbm.enable == 1)
//        {
            eStatus = eMBMasterPoll();	
            if(eStatus != MB_ENOERR)
            {
                rt_kprintf("MBM poll err!\n");
            }
//        }
				rt_thread_delay(10);
		}
}

static int16_t mbm_sts_update(void)
{
    if((g_sys.conf.mbm.dev_bitmap_holding==mbm_dev_inst.sts_bitmap_hold)&&(g_sys.conf.mbm.dev_bitmap_input == mbm_dev_inst.sts_bitmap_input))
    {
        bit_op_set(&g_sys.stat.gen.status_bm,GBM_MBM,1);
        return 0;
    }
    else
    {
        bit_op_set(&g_sys.stat.gen.status_bm,GBM_MBM,0);
        return -1;
    }
}

void mbm_fsm_thread_entry(void* parameter)
{
		rt_thread_delay(MBM_FSM_THREAD_DELAY+200);
    mbm_fsm_init(&mbm_dev_inst);
    
		while(1)
		{		
//        if(g_sys.conf.mbm.enable == 1)
//        {
            mbm_reg_holding_update(&mbm_dev_inst);
            mbm_reg_input_update(&mbm_dev_inst);
            mbm_sts_update();
//        }
				rt_thread_delay(g_sys.conf.mbm.sample_period);
		}
}


/**
 * Modbus master holding register callback function.
 *
 * @param pucRegBuffer holding register buffer
 * @param usAddress holding register address
 * @param usNRegs holding register number
 * @param eMode read or write
 *
 * @return result
 */
eMBErrorCode eMBMasterRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
        USHORT usNRegs, eMBRegisterMode eMode)
{     
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegHoldingBuf;
    USHORT          REG_HOLDING_START;
    USHORT          REG_HOLDING_NREGS;
    USHORT          usRegHoldStart;
      
    uint32_t        reg_temp;
  
    reg_temp = g_sys.conf.mbm.arr_slave_hbase_regn[ucMBMasterGetDestAddress() - 1];    
  
    pusRegHoldingBuf = mbm_dev_inst.holdbuf_reg[ucMBMasterGetDestAddress() - 1];
    REG_HOLDING_START = (uint16_t)((reg_temp>>16)&0x0000ffff);
    REG_HOLDING_NREGS = (uint16_t)(reg_temp&0x0000ffff);
    usRegHoldStart = REG_HOLDING_START;  
    /* if mode is read, the master will write the received date to buffer. */
    eMode = MB_REG_WRITE;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= REG_HOLDING_START)
            && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = usAddress - usRegHoldStart;
        switch (eMode)
        {
        /* read current register values from the protocol stack. */
        case MB_REG_READ:
            while (usNRegs > 0)
            {
                *pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] >> 8);
                *pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] & 0xFF);
                iRegIndex++;
                usNRegs--;
            }
            break;
        /* write current register values with new values from the protocol stack. */
        case MB_REG_WRITE:
            while (usNRegs > 0)
            {
                pusRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                pusRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
 * Modbus master input register callback function.
 *
 * @param pucRegBuffer input register buffer
 * @param usAddress input register address
 * @param usNRegs input register number
 *
 * @return result
 */
eMBErrorCode eMBMasterRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegInputBuf;
    USHORT          REG_INPUT_START;
    USHORT          REG_INPUT_NREGS;
    USHORT          usRegInStart;

    uint32_t        reg_temp;
  
    reg_temp = g_sys.conf.mbm.arr_slave_ibase_regn[ucMBMasterGetDestAddress() - 1];     
  
    pusRegInputBuf = mbm_dev_inst.inputbuf_reg[ucMBMasterGetDestAddress() - 1];
    REG_INPUT_START = (uint16_t)((reg_temp>>16)&0x0000ffff);
    REG_INPUT_NREGS = (uint16_t)(reg_temp&0x0000ffff);  
    usRegInStart = REG_INPUT_START;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= REG_INPUT_START)
            && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = usAddress - usRegInStart;
        while (usNRegs > 0)
        {
            pusRegInputBuf[iRegIndex] = *pucRegBuffer++ << 8;
            pusRegInputBuf[iRegIndex] |= *pucRegBuffer++;
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}


/**
  * @brief  modbus local data structure initialization
  * @param  mbm_dev_inst: modbus master device data struct.
  * @retval none
  */
static void mbm_fsm_init(mbm_dev_st* mbm_dev_inst)
{
    uint16_t i,j;
		mbm_dev_inst->sts_bitmap_hold = 0;
    mbm_dev_inst->sts_bitmap_input = 0;
    for(i=0;i<MB_MASTER_TOTAL_SLAVE_NUM;i++)
    {
        mbm_dev_inst->err_code_ih[i] = 0;
        for(j=0;j<M_REG_HOLDING_NREGS;j++)
          mbm_dev_inst->holdbuf_reg[i][j] = 0;
        for(j=0;j<M_REG_INPUT_NREGS;j++)
          mbm_dev_inst->inputbuf_reg[i][j] = 0;
    }
}

/**
  * @brief  update local modbus master register map with only variable device reg values
  * @param  
			@mbm_dev_inst: modbus master device data struct.
  * @retval 
			@arg 1: all device online
			@arg 0: not all device online
  */
static uint16_t mbm_reg_holding_update(mbm_dev_st* mbm_dev_inst)
{
		eMBMasterReqErrCode    errorCode = MB_MRE_NO_ERR;
		uint32_t dev_conf_bitmap_reg;
		uint32_t dev_stat_bitmap_reg;
    uint32_t reg_temp;
    
		uint16_t i;
		
		dev_conf_bitmap_reg = g_sys.conf.mbm.dev_bitmap_holding;
		dev_stat_bitmap_reg = 0;	    

		
		for(i=0;i<MB_MASTER_TOTAL_SLAVE_NUM;i++)
		{
        reg_temp = g_sys.conf.mbm.arr_slave_hbase_regn[i];
				if(((dev_conf_bitmap_reg>>i)&0x00000001) == 1)
				{
						errorCode = eMBMasterReqReadHoldingRegister((i+1), ((reg_temp>>16)&0x0000ffff), (reg_temp&0x0000ffff),MBM_RESPONSE_DELAY);
						if(errorCode == MB_MRE_NO_ERR)
						{
								dev_stat_bitmap_reg |= (0x0001<<i);
						}
            mbm_dev_inst->err_code_ih[i] = (mbm_dev_inst->err_code_ih[i]&0xffff0000)|errorCode;
            rt_thread_delay(MBM_QUEST_DELAY);
				}        
		}		
		mbm_dev_inst->sts_bitmap_hold = dev_stat_bitmap_reg;
		if(dev_stat_bitmap_reg == dev_conf_bitmap_reg)
		{
				return 1;
		}
		else
		{
				return 0;
		}		
}


/**
  * @brief  update local modbus master register map with only variable device reg values
  * @param  
			@mbm_dev_inst: modbus master device data struct.
  * @retval 
			@arg 1: all device online
			@arg 0: not all device online
  */
static uint16_t mbm_reg_input_update(mbm_dev_st* mbm_dev_inst)
{
		eMBMasterReqErrCode    errorCode = MB_MRE_NO_ERR;
		uint32_t dev_conf_bitmap_reg;
		uint32_t dev_stat_bitmap_reg;
    uint32_t reg_temp;
    
		uint16_t i;
		
		dev_conf_bitmap_reg = g_sys.conf.mbm.dev_bitmap_input;
		dev_stat_bitmap_reg = 0;
		
		for(i=0;i<MB_MASTER_TOTAL_SLAVE_NUM;i++)
		{
        reg_temp = g_sys.conf.mbm.arr_slave_ibase_regn[i];
				if(((dev_conf_bitmap_reg>>i)&0x00000001) == 1)
				{
						errorCode = eMBMasterReqReadInputRegister((i+1), ((reg_temp>>16)&0x0000ffff), (reg_temp&0x0000ffff),MBM_RESPONSE_DELAY);
						if(errorCode == MB_MRE_NO_ERR)
						{
								dev_stat_bitmap_reg |= (0x0001<<i);
						}
            mbm_dev_inst->err_code_ih[i] = (mbm_dev_inst->err_code_ih[i]&0x0000ffff)|(errorCode<<16);
            rt_thread_delay(MBM_QUEST_DELAY);
				}        
		}		
		mbm_dev_inst->sts_bitmap_input = dev_stat_bitmap_reg;
		if(dev_stat_bitmap_reg == dev_conf_bitmap_reg)
		{
				return 1;
		}
		else
		{
				return 0;
		}		
}


static void mbm_show_reg(uint8_t slave_addr, uint16_t reg_addr,uint16_t reg_num, uint8_t mode)
{
    uint16_t i;
    uint16_t buf;
    rt_kprintf("addr | value\n");
    if(mode ==0)  //holding buf
    {
        for(i=0;i<reg_num;i++)
        {
            buf = mbm_dev_inst.holdbuf_reg[slave_addr-1][reg_addr+i];
            rt_kprintf(" %d   |   %d\n",(reg_addr+i),buf);
        }
    }
    else if(mode == 1)  //input buf
    {
        for(i=0;i<reg_num;i++)
        {
            buf = mbm_dev_inst.inputbuf_reg[slave_addr-1][reg_addr+i];
            rt_kprintf(" %d   |   %d\n",(reg_addr+i),buf);
        } 
    }
}

static void mbm_info(void)
{
    uint16_t i;
    uint16_t reg_base,reg_cnt;
    uint32_t reg_temp;
    rt_kprintf("\n=================Bitmap info:===============\n");
    rt_kprintf("hold conf bitmap: %x, stats bitmap: %x\n",g_sys.conf.mbm.dev_bitmap_holding, mbm_dev_inst.sts_bitmap_hold);
    rt_kprintf("input conf bitmap: %x, stats bitmap: %x\n",g_sys.conf.mbm.dev_bitmap_input, mbm_dev_inst.sts_bitmap_input);
  
    rt_kprintf("\n=================holding_reg info:===============\n");
    rt_kprintf("  id  |  offset | reg_cnt | stats | err_no\n");
    for(i=0;i<MB_MASTER_TOTAL_SLAVE_NUM;i++)
    {
        reg_temp = g_sys.conf.mbm.arr_slave_hbase_regn[i];
        reg_base = ((reg_temp>>16)&0x0000ffff);
        reg_cnt = (reg_temp&0x0000ffff);
        if(((g_sys.conf.mbm.dev_bitmap_holding>>i)&0x01)>0)
            rt_kprintf(" %2d   |  %4d   |  %4d   |  %2d   |  %4d\n",(i+1),reg_base, reg_cnt, ((mbm_dev_inst.sts_bitmap_hold&(((uint16)1<<i)))>>i), mbm_dev_inst.err_code_ih[i]&0x0000ffff);
    }    

    rt_kprintf("\n=================input_reg info:===============\n");
    rt_kprintf("  id  |  offset | reg_cnt | stats | err_no\n");
    for(i=0;i<MB_MASTER_TOTAL_SLAVE_NUM;i++)
    {
        reg_temp = g_sys.conf.mbm.arr_slave_ibase_regn[i];
        reg_base = ((reg_temp>>16)&0x0000ffff);
        reg_cnt = (reg_temp&0x0000ffff);
        if(((g_sys.conf.mbm.dev_bitmap_input>>i)&0x01)>0)
            rt_kprintf(" %2d   |  %4d   |  %4d   |  %2d   |  %4d\n",(i+1),reg_base, reg_cnt, ((mbm_dev_inst.sts_bitmap_input&(((uint16)1<<i)))>>i), mbm_dev_inst.err_code_ih[i]>>16);
    }    
}

MSH_CMD_EXPORT(mbm_info, show mbm reg info);

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(mbm_show_reg, list mbm regs);
FINSH_FUNCTION_EXPORT(mbm_info, show mbm reg info);
#endif

