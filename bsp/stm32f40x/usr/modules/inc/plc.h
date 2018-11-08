#ifndef __PLC_H
#define	__PLC_H
#include "sys_def.h"

#define PLC_RTX_BUF_DEPTH 16

uint32_t plc_tx(uint8_t* tx_buf, uint16_t tx_len);
int32_t get_plc_status(void);
#endif //__PLC_H
