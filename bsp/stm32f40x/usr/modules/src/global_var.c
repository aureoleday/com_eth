#include "sys_conf.h"
#include "global_var.h"
#include "user_mb_app.h"
#include "reg_map_check.h"
#include "usr_init.h"
#include "bit_op.h"

//configuration parameters
sys_reg_st  g_sys; 																	//global parameter declairation
extern mbm_dev_st mbm_dev_inst;

//configuration register map declairation
const conf_reg_map_st conf_reg_map_inst[CONF_REG_MAP_NUM]=  
{	//id		mapped registers		                         min	    max				    default			  type    chk_prt
	{	0,		&g_sys.conf.eth.tcp_client_en,               0,		    1,				    1,				    0,      NULL   	          },
	{	1,		&g_sys.conf.eth.tcp_period,                  1,		    0xffffffff,		30,			      0,      tcp_timer_opt     },
	{	2,		&g_sys.conf.eth.remote_ip,                   0,		    0xffffffff,		0x60b44e78,		0,      NULL   	          },
	{	3,		&g_sys.conf.eth.remote_port,	               0,		    65535,		    19999,       	0,      NULL   	          },
	{	4,		&g_sys.conf.eth.local_ip,			               0,		    0xffffffff,		0x6403a8c0,		0,      NULL   	          },
	{	5,		&g_sys.conf.eth.local_gateway,               0,		    0xffffffff,		0x0103a8c0,		0,      NULL   	          },
	{	6,		&g_sys.conf.eth.local_mask,                  0,		    0xffffffff,		0x00ffffff,		0,      NULL   	          },
	{	7,		&g_sys.conf.eth.local_port,	                 0,		    65535,    		80,				    0,      NULL   	          },
	{	8,  	&g_sys.conf.eth.dns_server,		               0,		    0xffffffff,	  0x0103a8c0,	  0,      NULL   	          },
	{	9,		&g_sys.conf.eth.dhcp_en,		                 0,		    1,	       		1,				    0,      dhcp_trigger      },
	{	10,		&g_sys.conf.eth.reconnect_period,            100,     1000000,	    5000,			    0,      NULL   	          },
	{	11,		&g_sys.conf.eth.dns_en,                      0,		    1,				    0,				    0,      NULL   	          },
	{	12,   NULL,                                        0,		    0,				    0,				    0,      NULL   	          },
	{	13,		&g_sys.conf.mbm.dev_bitmap_holding,          0,		    0xffffffff,   0,			      0,      NULL   	          },
	{	14,		&g_sys.conf.mbm.dev_bitmap_input,            0,		    0xffffffff,   1,			      0,      NULL   	          },
	{	15,		&g_sys.conf.mbm.baudrate,   	               1200,    115200,		    9600,			    0,      NULL   	          },
	{	16,		&g_sys.conf.mbm.sample_period,               0,		    1000000,      1000,			    0,      NULL   	          },
	{	17,		&g_sys.conf.mbm.enable,                      0,		    1,				    1,				    0,      NULL   	          },
	{	18,		&g_sys.conf.gen.disp_dev,                    1,		    8,				    1,				    0,      NULL   	          },  
	{	19,		&g_sys.conf.dbg.usr_com,                     0,		    1,				    0,				    0,      NULL   	          },
	{	20,		&g_sys.conf.geo.enable,                      0,		    1,            0,			      0,      geo_pwr_opt       },
	{	21,		&g_sys.conf.geo.pkg_period,                  0,		    1000000,      10,			      0,      geo_timer_opt     },
	{	22,		&g_sys.conf.geo.sample_period,               0,		    1000,         10,			      0,      NULL              },
	{	23,		&g_sys.conf.geo.filter,                      0,		    255,          10,			      0,      geo_filter_opt    },
	{	24,		&g_sys.conf.gen.temp_offset,                 0,		    999,          10,			      0,      NULL              },
	{	25,		&g_sys.conf.gen.temp_win,                    1,		    63,				    8,				    0,      NULL   	          },
	{	26,		NULL,                                        0,		    0,				    0,				    0,      NULL   	          },  
	{	27,		NULL,                                        0,		    1,				    0,				    1,      update_data_opt   },
	{	28,		NULL,                                        0,		    0xffffffff,   0,				    1,      set_timestamp     },
	{	29,		NULL,                                        1,		    2,				    0,				    1,      save_conf_opt     },
	{	30,		NULL,                                        1,		    4,				    0,				    1,      set_boot_opt   	  },
	{	31,		NULL,                                        0,		    1,				    0,				    1,      sys_reset_opt     },
	{	32,		&g_sys.conf.mbm.arr_slave_hbase_regn[0],     0,		    0xffffffff,		0x00000004, 	0,      NULL   	          },
	{	33,		&g_sys.conf.mbm.arr_slave_hbase_regn[1],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	34,		&g_sys.conf.mbm.arr_slave_hbase_regn[2],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	35,		&g_sys.conf.mbm.arr_slave_hbase_regn[3],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	36,		&g_sys.conf.mbm.arr_slave_hbase_regn[4],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	37,		&g_sys.conf.mbm.arr_slave_hbase_regn[5],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	38,		&g_sys.conf.mbm.arr_slave_hbase_regn[6],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	39,		&g_sys.conf.mbm.arr_slave_hbase_regn[7],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	40,		&g_sys.conf.mbm.arr_slave_hbase_regn[8],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	41,		&g_sys.conf.mbm.arr_slave_hbase_regn[9],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	42,		&g_sys.conf.mbm.arr_slave_hbase_regn[10],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	43,		&g_sys.conf.mbm.arr_slave_hbase_regn[11],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	44,		&g_sys.conf.mbm.arr_slave_hbase_regn[12],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	45,		&g_sys.conf.mbm.arr_slave_hbase_regn[13],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	46,		&g_sys.conf.mbm.arr_slave_hbase_regn[14],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	47,		&g_sys.conf.mbm.arr_slave_hbase_regn[15],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	48,		&g_sys.conf.mbm.arr_slave_hbase_regn[16],  	 0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	49,		&g_sys.conf.mbm.arr_slave_hbase_regn[17],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	50,		&g_sys.conf.mbm.arr_slave_hbase_regn[18],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	51,		&g_sys.conf.mbm.arr_slave_hbase_regn[19],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	52,		&g_sys.conf.mbm.arr_slave_hbase_regn[20],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	53,		&g_sys.conf.mbm.arr_slave_hbase_regn[21],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	54,		&g_sys.conf.mbm.arr_slave_hbase_regn[22],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	55,		&g_sys.conf.mbm.arr_slave_hbase_regn[23],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	56,		&g_sys.conf.mbm.arr_slave_hbase_regn[24],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	57,		&g_sys.conf.mbm.arr_slave_hbase_regn[25],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	58,		&g_sys.conf.mbm.arr_slave_hbase_regn[26],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	59,		&g_sys.conf.mbm.arr_slave_hbase_regn[27],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	60,		&g_sys.conf.mbm.arr_slave_hbase_regn[28],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	61,		&g_sys.conf.mbm.arr_slave_hbase_regn[29],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	62,		&g_sys.conf.mbm.arr_slave_hbase_regn[30],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	63,		&g_sys.conf.mbm.arr_slave_hbase_regn[31],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	64,		&g_sys.conf.mbm.arr_slave_ibase_regn[0],     0,		    0xffffffff,		0x00000018, 	0,      NULL   	          },
	{	65,		&g_sys.conf.mbm.arr_slave_ibase_regn[1],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	66,		&g_sys.conf.mbm.arr_slave_ibase_regn[2],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	67,		&g_sys.conf.mbm.arr_slave_ibase_regn[3],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	68,		&g_sys.conf.mbm.arr_slave_ibase_regn[4],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	69,		&g_sys.conf.mbm.arr_slave_ibase_regn[5],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	70,		&g_sys.conf.mbm.arr_slave_ibase_regn[6],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	71,		&g_sys.conf.mbm.arr_slave_ibase_regn[7],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	72,		&g_sys.conf.mbm.arr_slave_ibase_regn[8],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	73,		&g_sys.conf.mbm.arr_slave_ibase_regn[9],     0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	74,		&g_sys.conf.mbm.arr_slave_ibase_regn[10],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	75,		&g_sys.conf.mbm.arr_slave_ibase_regn[11],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	76,		&g_sys.conf.mbm.arr_slave_ibase_regn[12],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	77,		&g_sys.conf.mbm.arr_slave_ibase_regn[13],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	78,		&g_sys.conf.mbm.arr_slave_ibase_regn[14],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	79,		&g_sys.conf.mbm.arr_slave_ibase_regn[15],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	80,		&g_sys.conf.mbm.arr_slave_ibase_regn[16],  	 0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	81,		&g_sys.conf.mbm.arr_slave_ibase_regn[17],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	82,		&g_sys.conf.mbm.arr_slave_ibase_regn[18],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	83,		&g_sys.conf.mbm.arr_slave_ibase_regn[19],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	84,		&g_sys.conf.mbm.arr_slave_ibase_regn[20],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	85,		&g_sys.conf.mbm.arr_slave_ibase_regn[21],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	86,		&g_sys.conf.mbm.arr_slave_ibase_regn[22],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	87,		&g_sys.conf.mbm.arr_slave_ibase_regn[23],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	88,		&g_sys.conf.mbm.arr_slave_ibase_regn[24],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	89,		&g_sys.conf.mbm.arr_slave_ibase_regn[25],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	90,		&g_sys.conf.mbm.arr_slave_ibase_regn[26],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	91,		&g_sys.conf.mbm.arr_slave_ibase_regn[27],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	92,		&g_sys.conf.mbm.arr_slave_ibase_regn[28],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	93,		&g_sys.conf.mbm.arr_slave_ibase_regn[29],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	94,		&g_sys.conf.mbm.arr_slave_ibase_regn[30],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          },
	{	95,		&g_sys.conf.mbm.arr_slave_ibase_regn[31],    0,		    0xffffffff,		0x00000004,		0,      NULL   	          }
};      
 

//status register map declairation
const sts_reg_map_st status_reg_map_inst[STAT_REG_MAP_NUM]=
{	// 	id			mapped registers																	
  {	0,    &g_sys.stat.gen.status_reg_num,			STAT_REG_MAP_NUM},
  {	1,    &g_sys.stat.gen.config_reg_num,			CONF_REG_MAP_NUM},
  {	2,    &g_sys.stat.gen.software_ver,				SOFTWARE_VER},
  {	3,    &g_sys.stat.gen.hardware_ver,				HARDWARE_VER},
  {	4,    &g_sys.stat.man.serial_no,				  SERIAL_NO},
  {	5,    &g_sys.stat.man.man_date,					  MAN_DATE},
  {	6,    &g_sys.stat.man.dev_type,					  DEVICE_TYPE},
  {	7,    NULL,						                    0},
  {	8,    NULL,						                    0},
  {	9,    &g_sys.stat.gen.reset_counts,				0},
	{	10,		&g_sys.stat.gen.time,   						0},
	{	11,		&g_sys.stat.gen.status_bm,          0},
	{	12,		&g_sys.stat.eth.dns_ip,             0},
	{	13,		&g_sys.stat.eth.server_ip,					0},
	{	14,		NULL,						                    0},
	{	15,		NULL,						                    0},
	{	16,		NULL,						                    0},
	{	17,		NULL,						                    0},
	{	18,		NULL,						                    0},
	{	19,		NULL,						                    0},
	{	20,		&mbm_dev_inst.sts_bitmap_hold,      0},
	{	21,		&mbm_dev_inst.sts_bitmap_input,     0},
	{	22,		NULL,						                    0},
	{	23,		NULL,						                    0},
	{	24,		NULL,						                    0},
	{	25,		NULL,						                    0},
	{	26,		NULL,						                    0},
	{	27,		NULL,						                    0},
	{	28,		NULL,						                    0},
	{	29,		NULL,						                    0},
	{	30,		NULL,						                    0},
	{	31,		NULL,						                    0},
	{	32,		&mbm_dev_inst.err_code_ih[0], 	    0},
	{	33,		&mbm_dev_inst.err_code_ih[1], 	    0},
	{	34,		&mbm_dev_inst.err_code_ih[2], 	    0},
	{	35,		&mbm_dev_inst.err_code_ih[3], 	    0},
	{	36,		&mbm_dev_inst.err_code_ih[4], 	    0},
	{	37,		&mbm_dev_inst.err_code_ih[5], 	    0},
	{	38,		&mbm_dev_inst.err_code_ih[6], 	    0},
	{	39,		&mbm_dev_inst.err_code_ih[7], 	    0},
	{	40,		&mbm_dev_inst.err_code_ih[8], 	    0},
	{	41,		&mbm_dev_inst.err_code_ih[9], 	    0},
	{	42,		&mbm_dev_inst.err_code_ih[10],	    0},
	{	43,		&mbm_dev_inst.err_code_ih[11],	    0},
	{	44,		&mbm_dev_inst.err_code_ih[12],	    0},
	{	45,		&mbm_dev_inst.err_code_ih[13],	    0},
	{	46,		&mbm_dev_inst.err_code_ih[14],	    0},
	{	47,		&mbm_dev_inst.err_code_ih[15],	    0},
	{	48,		&mbm_dev_inst.err_code_ih[16],	    0},
	{	49,		&mbm_dev_inst.err_code_ih[17],	    0},
	{	50,		&mbm_dev_inst.err_code_ih[18],	    0},
	{	51,		&mbm_dev_inst.err_code_ih[19],	    0},
	{	52,		&mbm_dev_inst.err_code_ih[20],	    0},
	{	53,		&mbm_dev_inst.err_code_ih[21],	    0},
	{	54,		&mbm_dev_inst.err_code_ih[22],	    0},
	{	55,		&mbm_dev_inst.err_code_ih[23],	    0},
	{	56,		&mbm_dev_inst.err_code_ih[24],	    0},
	{	57,		&mbm_dev_inst.err_code_ih[25],	    0},
	{	58,		&mbm_dev_inst.err_code_ih[26],	    0},
	{	59,		&mbm_dev_inst.err_code_ih[27],	    0},
	{	60,		&mbm_dev_inst.err_code_ih[28],	    0},
	{	61,		&mbm_dev_inst.err_code_ih[29],	    0},
	{	62,		&mbm_dev_inst.err_code_ih[30],	    0},
	{	63,		&mbm_dev_inst.err_code_ih[31],	    0}
};

/**
  * @brief  initialize system status reg data
  * @param  None
  * @retval None
  */
void init_load_status(void)
{
		uint16_t i;	
		for(i=0;i<STAT_REG_MAP_NUM;i++)
		{
				if(status_reg_map_inst[i].reg_ptr != NULL)
				{
						*(status_reg_map_inst[i].reg_ptr) = status_reg_map_inst[i].dft;
				}
		}    
//    g_sys.stat.gen.reset_counts = get_reset_counts();
    g_sys.stat.man.serial_no = get_serial_id();
    g_sys.stat.man.man_date = get_comp_date();
    g_sys.stat.gen.reset_counts = update_reset_cnt(0);
    bit_op_set(&g_sys.stat.gen.status_bm,GBM_FLASH,1);
}


uint16_t init_load_default(void)
{
    uint16_t i, ret;
    ret = USR_EOK;
    for(i=0;i<CONF_REG_MAP_NUM;i++)		//initialize global variable with default values
    {
        if(conf_reg_map_inst[i].reg_ptr != NULL)
        {
            *(conf_reg_map_inst[i].reg_ptr) = conf_reg_map_inst[i].dft;
        }
    }    
    return ret;
}


/**
  * @brief  write register map with constraints.
  * @param  reg_addr: reg map address.
  * @param  wr_data: write data. 
	* @param  permission_flag:  
  *   This parameter can be one of the following values:
  *     @arg PERM_PRIVILEGED: write opertion can be performed dispite permission level
  *     @arg PERM_INSPECTION: write operation could only be performed when pass permission check
  * @retval 
  *   This parameter can be one of the following values:
  *     @arg 1: write operation success
  *     @arg 0: write operation fail
  */
uint16 reg_map_write(uint16 reg_addr, uint32_t *wr_data, uint8_t wr_cnt)
{
    uint16_t i;
    uint16_t err_code;
    err_code = REGMAP_ERR_NOERR;		
    
    if((reg_addr+wr_cnt) > CONF_REG_MAP_NUM)	//address range check
    {
        err_code = REGMAP_ERR_ADDR_OR;
        rt_kprintf("MB_SLAVE REGMAP_ERR_ADDR_OR1 failed\n");
        return err_code;
    }
    
    for(i=0;i<wr_cnt;i++)										//min_max limit check
    {
        if((*(wr_data+i)>conf_reg_map_inst[reg_addr+i].max)||(*(wr_data+i)<conf_reg_map_inst[reg_addr+i].min))		//min_max limit check
        {
            err_code = REGMAP_ERR_DATA_OR;
            rt_kprintf("REGMAP_ERR_WR_OR03 failed\n");
            return err_code;	
        }

        if(conf_reg_map_inst[reg_addr+i].chk_ptr != NULL)
        {
            if(conf_reg_map_inst[reg_addr+i].chk_ptr(*(wr_data+i))==0)
            {
                err_code = REGMAP_ERR_CONFLICT_OR;
                rt_kprintf("CHK_PTR:REGMAP_ERR_WR_OR failed\n");
                return err_code;	
            }
        }
    }
    
    for(i=0;i<wr_cnt;i++)										//data write
    {
        if(conf_reg_map_inst[reg_addr+i].reg_ptr != NULL)
            *(conf_reg_map_inst[reg_addr+i].reg_ptr) = *(wr_data+i);//write data to designated register
    }	
    return err_code;		
}

/**
  * @brief  read register map.
  * @param  reg_addr: reg map address.
	* @param  *rd_data: read data buffer ptr.
  * @retval 
  *   This parameter can be one of the following values:
  *     @arg 1: write operation success
  *     @arg 0: read operation fail
  */
uint16 reg_map_read(uint16 reg_addr,uint32_t* reg_data,uint8_t read_cnt)
{
    uint16_t i;
    uint16_t err_code;
    err_code = REGMAP_ERR_NOERR;
    if((reg_addr&0x8000) != 0)
    {
        reg_addr &= 0x7fff;
        if(reg_addr > STAT_REG_MAP_NUM)	//address out of range
        {
            err_code = REGMAP_ERR_ADDR_OR;
        }
        else
        {
            for(i=0;i<read_cnt;i++)
            {
                *(reg_data+i) = *(status_reg_map_inst[reg_addr+i].reg_ptr);//read data from designated register
            }
        }
    }
    else
    {
        reg_addr = reg_addr;
        if(reg_addr > CONF_REG_MAP_NUM)	//address out of range
        {
            err_code = REGMAP_ERR_ADDR_OR;
        }
        else
        {
            for(i=0;i<read_cnt;i++)
            {
                *(reg_data+i) = *(conf_reg_map_inst[reg_addr+i].reg_ptr);//read data from designated register						
            }
        }		
    }	
    return err_code;
}

/**
  * @brief  show register map content.
  * @param  reg_addr: reg map address.
	* @param  *rd_data: register read count.
  * @retval none
  */
static void rd_reg_map(uint16_t reg_addr, uint16_t reg_cnt)
{
    uint32_t reg_buf[32];
    uint16_t i;
    reg_map_read(reg_addr, reg_buf, reg_cnt);
    rt_kprintf("Reg map info:\n");
    for(i=0;i<reg_cnt;i++)
    {
        rt_kprintf("addr:%d;data:%x\n", (reg_addr+i)&0x3fff,reg_buf[i]);
    }
}

static uint16_t wr_reg_map(uint16_t reg_addr,uint32_t data)
{
    uint16_t ret;	
    ret = reg_map_write(reg_addr,&data,1);
    return ret;
}

FINSH_FUNCTION_EXPORT(rd_reg_map, show registers map.);
FINSH_FUNCTION_EXPORT(wr_reg_map, write data into conf registers.);
