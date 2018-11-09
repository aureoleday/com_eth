#include <rtthread.h>
#include "sys_conf.h"
#include "cmd_resolve.h"

/**
  * @brief 	output control module components cooldown 
	* @param  none
	* @retval none
  */
void cmd_thread_entry(void* parameter)
{  
    rt_thread_delay(CMD_THREAD_DELAY);
		cmd_dev_init();
		while(1)
		{
        recv_frame_fsm();
				cmd_frame_resolve();
//        report_data();
				rt_thread_delay(10);
		}
}
