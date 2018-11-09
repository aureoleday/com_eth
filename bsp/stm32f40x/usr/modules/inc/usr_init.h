#ifndef __USR_INIT
#define	__USR_INIT
#include "sys_conf.h"

enum
{
		CONF_OPT_USR = 1,
		CONF_OPT_FACT,
    CONF_OPT_DEBUT,
    CONF_OPT_DEFAULT
};

void usr_init(void);
int16_t set_startup_flag(uint8_t conf_opt);
void save_conf(uint8_t conf_opt);
void load_conf(uint8_t conf_opt);
uint16_t plc_conf_opt(uint32_t pram);
uint32_t get_serial_id(void);
uint32_t get_comp_date(void);
int32_t update_reset_cnt(uint8_t flag);

#endif //	__USR_INIT
