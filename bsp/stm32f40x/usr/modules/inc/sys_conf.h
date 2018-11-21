#ifndef __SYS_CONF
#define	__SYS_CONF
//#include <rtthread.h>
#include "sys_def.h"

//application delay
#define		CMD_THREAD_DELAY            400
#define		BKG_THREAD_DELAY            700
#define		LCD_THREAD_DELAY            300
#define		TCPCLIENT_THREAD_DELAY      600
#define		TEST_THREAD_DELAY		        800
#define		MBM_THREAD_DELAY		        500
#define		MBM_FSM_THREAD_DELAY        500

typedef struct
{	
    uint32_t 		power_mode;					
}conf_gen_st;

typedef struct
{
    uint32_t 		baudrate;
    uint32_t    sample_period;
    uint32_t 		dev_bitmap_holding;         //0: disable; 1:enable
    uint32_t 		dev_bitmap_input;           //0: disable; 1:enable
    uint32_t    plc_bitmap;
    uint32_t    arr_slave_hbase_regn[MBM_TOTAL_SLAVE_NUM];
    uint32_t    arr_slave_ibase_regn[MBM_TOTAL_SLAVE_NUM];    
}conf_mbm_st;

typedef struct
{
    uint32_t    tcp_client_en;
    uint32_t    tcp_period;
    uint32_t    reconnect_period;
    uint32_t    dhcp_en;
    uint32_t    remote_ip;
    uint32_t    remote_port;
    uint32_t    local_ip;
    uint32_t    local_gateway;
    uint32_t    local_mask;
    uint32_t    local_port;
    uint32_t    dns_server;
    uint32_t    dns_en;
}conf_eth_st;

typedef struct
{
    conf_gen_st       gen;
    conf_mbm_st       mbm;
    conf_eth_st       eth;
}config_st;


typedef struct
{	    
    uint32_t    status_reg_num;
    uint32_t    config_reg_num;
    uint32_t    software_ver;
    uint32_t    hardware_ver;
    uint32_t    status_bm;
    uint32_t    reset_counts;
    uint32_t 		time;
    uint32_t    lcd_type;
}stat_gen_st;

typedef struct
{	
	uint32_t 		plc_bitmap;					
}stat_mbm_st;

typedef struct
{	
    uint32_t    server_ip;
    uint32_t    dns_ip;
}stat_eth_st;

typedef struct
{	
    uint32_t    serial_no;
    uint32_t    man_date;
    uint32_t    dev_type;
}stat_man_st;

typedef struct
{
		stat_gen_st gen;
    stat_mbm_st mbm;
    stat_eth_st eth;
    stat_man_st man;
}status_st;

typedef struct
{
    config_st     conf;
    status_st     stat;	
}sys_reg_st;

typedef struct 
{
		uint16_t 	  id;
		uint32_t*	  reg_ptr;
		int32_t	    min;
		uint32_t	  max;
		uint32_t	  dft;
    uint8_t	    type;                     //0:RW, 1:WO
		uint16_t    (*chk_ptr)(uint32_t pram);
}conf_reg_map_st;

typedef struct 
{
		uint16_t 	id;
		uint32_t*	reg_ptr;
    uint32_t  dft;
}sts_reg_map_st;

uint16_t sys_global_var_init(void);

#endif //	__SYS_CONF


