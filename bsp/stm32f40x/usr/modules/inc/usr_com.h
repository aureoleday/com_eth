#ifndef __USR_COM_H
#define	__USR_COM_H
#include "sys_def.h"

#define COM_RTX_BUF_DEPTH 16

uint32_t com_tx(uint8_t* tx_buf, uint16_t tx_len);
int16_t com_recv(uint8_t *rx_ptr);
#endif //__USR_COM_H
