#ifndef    USER_APP
#define USER_APP
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"

/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START        0
#define S_DISCRETE_INPUT_NDISCRETES   16
#define S_COIL_START                  0
#define S_COIL_NCOILS                 64
#define S_REG_INPUT_START             0
#define S_REG_INPUT_NREGS             32
#define S_REG_HOLDING_START           0
#define S_REG_HOLDING_NREGS           100
/* salve mode: holding register's all address */
#define          S_HD_RESERVE                     0
/* salve mode: input register's all address */
#define          S_IN_RESERVE                     0
/* salve mode: coil's all address */
#define          S_CO_RESERVE                     0
/* salve mode: discrete's all address */
#define          S_DI_RESERVE                     0

/* -----------------------Master Defines -------------------------------------*/
#define M_DISCRETE_INPUT_START        0
#define M_DISCRETE_INPUT_NDISCRETES   16
#define M_COIL_START                  0
#define M_COIL_NCOILS                 64
#define M_REG_INPUT_START             0
#define M_REG_INPUT_NREGS             24
#define M_REG_HOLDING_START           0
#define M_REG_HOLDING_NREGS           16
/* master mode: holding register's all address */
#define          M_HD_RESERVE                     0
/* master mode: input register's all address */
#define          M_IN_RESERVE                     0
/* master mode: coil's all address */
#define          M_CO_RESERVE                     0
/* master mode: discrete's all address */
#define          M_DI_RESERVE                     0


#define MBM_RESPONSE_DELAY	30
#define	MBM_QUEST_DELAY			30

//modbus master FSM definition
#define					MBM_FSM_IDLE			0x01
#define					MBM_FSM_SYNC			0x02
#define					MBM_FSM_UPDATE		0x04

//
#define					MBM_POLL_TIMEOUT_THRESHOLD 				5
#define					MBM_INIT_TIMEOUT_THRESHOLD 				5
#define					MBM_UPDATE_TIMEOUT_THRESHOLD 			5


typedef struct
{
		uint32_t      sts_bitmap_hold;
    uint32_t      sts_bitmap_input;
    uint32_t      err_code_ih[MB_MASTER_TOTAL_SLAVE_NUM];
    uint16_t      holdbuf_reg[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];
    uint16_t      inputbuf_reg[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_INPUT_NREGS];
}mbm_dev_st;

static void mbm_fsm_init(mbm_dev_st* mbm_dev_inst);
static uint16_t mbm_reg_holding_update(mbm_dev_st* mbm_dev_inst);
static uint16_t mbm_reg_input_update(mbm_dev_st* mbm_dev_inst);
static void mbm_local_update(void);

#endif

