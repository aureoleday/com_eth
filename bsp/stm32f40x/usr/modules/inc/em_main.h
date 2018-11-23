#ifndef __EM_MAIN
#define	__EM_MAIN
//#include <rtthread.h>
#include "sys_def.h"

typedef struct
{
    char     name[16];
    char     value[16];
    char     uinit[8];
    uint16_t  text_id;
}sensor_label_st;

typedef struct
{
    sensor_label_st  usr_label[32];
}usr_label_st;


#endif //__EM_MAIN
