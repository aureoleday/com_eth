#ifndef __GLOBAL_VAR
#define	__GLOBAL_VAR
#include "sys_conf.h"
#include <rtthread.h>

uint16_t reg_map_write(uint16_t reg_addr, uint32_t *wr_data, uint8_t wr_cnt);
uint16_t reg_map_read(uint16_t reg_addr, uint32_t* reg_data, uint8_t read_cnt);
uint16_t sys_local_var_init(void);
uint16_t init_load_default(void);
void init_load_status(void);
uint8_t reset_runtime(uint16_t param);
uint8_t load_factory_pram(void);

#endif //__GLOBAL_VAR
