#ifndef	__THREADS
#define	__THREADS

enum
{
		INIT_THREAD_THREAD_PRIO=21,
    LCD_THREAD_PRIO,
    CMD_THREAD_PRIO,
    TCP_TX_THREAD_PRIO,
    TCP_RX_THREAD_PRIO,
    MBM_THREAD_PRIO,
    MBM_FSM_THREAD_PRIO,  
    TCPCLIENT_THREAD_PRIO,
    BKG_THREAD_PRIO,		
    TEST_THREAD_PRIO,
		USR_MAX_PRIO
};


void bkg_thread_entry(void* parameter);
void test_thread_entry(void* parameter);
void tcpclient_thread_entry(void* parameter);
void mbm_thread_entry(void* parameter);
void mbm_fsm_thread_entry(void* parameter);
void cmd_thread_entry(void* parameter);
void lcd_thread_entry(void* parameter);

#endif	//__THREADS
