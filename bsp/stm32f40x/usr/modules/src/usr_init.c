#include <rtthread.h> 
#include <dfs_posix.h>
#include "sys_conf.h"
#include "usr_init.h"
#include "global_var.h"

void get_man_date(uint8_t *year, uint8_t *month, uint8_t *day)
{
    const char *pmonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    const char Date[12] = __DATE__;
    uint8_t i;

    for(i = 0; i < 12; i++)if(memcmp(Date, pmonth[i], 3) == 0)*month = i + 1, i = 12;
    *year = (uint8_t)atoi(Date + 9); 
    *day = (uint8_t)atoi(Date + 4);
}

uint32_t get_comp_date(void)
{
    uint8_t year, month, day;
    uint32_t ret;
    ret = 0;
    get_man_date(&year, &month, &day);
    ret = year<<24|month<<16|day<<8;
    return ret;
}

void get_comp_time(void)
{
    rt_kprintf("%s\n",__TIME__);
}


static int16_t dir_init(void)
{
    DIR *dirp;
    int16_t ret;
  
    ret = 0;
    dirp = opendir("/conf");
    if (dirp == RT_NULL)
    {
        rt_kprintf("dir /conf not exist, creat one\n");
        ret = mkdir("/conf", 0x777);
        if(ret < 0)
            rt_kprintf("mkdir /conf fail\n");
        else
            rt_kprintf("mkdir /conf ok\n");
        ret |= 1;
    }
    else
        closedir(dirp);
    
    dirp = opendir("/data");
    if (dirp == RT_NULL)
    {
        rt_kprintf("dir /data not exist, creat one\n");
        ret = mkdir("/data", 0x777);
        if(ret < 0)
            rt_kprintf("mkdir /data fail\n");
        else
            rt_kprintf("mkdir /data ok\n");
        ret |= 2;
    }
    else
        closedir(dirp);
    
    dirp = opendir("/log");
    if (dirp == RT_NULL)
    {
        rt_kprintf("dir /log not exist, creat one\n");
        ret = mkdir("/log", 0x777);
        if(ret < 0)
            rt_kprintf("mkdir /log fail\n");
        else
            rt_kprintf("mkdir /log ok\n");
        ret |= 4;
    }
    else
        closedir(dirp);
    
    return ret;
}

int16_t set_startup_flag(uint8_t conf_opt)
{
    
    int fd, fsize, ret;
  
    ret = 0;
    
    fsize = sizeof(conf_opt);
  
    fd = open("/conf/startup_flag.ini", O_RDWR|O_CREAT);
    
    if(fd>=0)
    {
        write(fd,(void*)&conf_opt,fsize);
        close(fd);
    }
    else
    {
        rt_kprintf("Error open load_flag file\n");
    }
    return ret;
}

static int16_t load_startup_flag(void)
{
    int fd, fsize, ret, start_flag;
  
    ret = 0;  
  
    fd = open("/conf/startup_flag.ini", O_RDWR|O_CREAT);
    
    if(fd>=0)
    {
        read(fd,(void*)&start_flag,fsize);
        close(fd);
        ret = start_flag;
    }
    else
    {
        rt_kprintf("Error open load_flag file\n");
    }
    
    return ret;
}

void save_conf(uint8_t conf_opt)
{
    extern sys_reg_st  g_sys;
    int fd, fsize;
  
    fsize = sizeof(g_sys.conf);
  
    switch(conf_opt)
    {
        case(CONF_OPT_USR):
        {
            fd = open("/conf/usr.ini", O_WRONLY|O_CREAT);
            if(fd>=0)
            {
                write(fd,(void*)&g_sys.conf,fsize);
                close(fd);
            }
            else
            {
                rt_kprintf("Error open usr conf file\n");
            }
            break;
        }
        case(CONF_OPT_FACT):
        {
            fd = open("/conf/fact.ini", O_WRONLY|O_CREAT);
            if(fd>=0)
            {
                write(fd,(void*)&g_sys.conf,fsize);
                close(fd);
            }
            else
            {
                rt_kprintf("Error open fact conf file\n");
            }
            break;
        }
        default:
        {
            break;
        }
    }    
}
 
void load_conf(uint8_t conf_opt)
{
    extern sys_reg_st  g_sys;
    config_st   * config_st_ptr;
    int fd, fsize;
  
    fsize = sizeof(g_sys.conf);
    
    config_st_ptr = rt_malloc(fsize);    
  
    switch(conf_opt)
    {
        case(CONF_OPT_USR):
        {
            fd = open("/conf/usr.ini", O_RDONLY);
            if(fd>=0)
            {
                read(fd,(void*)config_st_ptr,fsize);
                close(fd);
                g_sys.conf = *config_st_ptr;
                rt_kprintf("CONF_USR loaded successfully.\n");
            }
            else
            {
                rt_kprintf("Error open usr conf file\n");
                rt_kprintf("CONF_USR loaded fail.\n");
            }
            break;
        }
        case(CONF_OPT_FACT):
        {
            fd = open("/conf/fact.ini", O_RDONLY);
            if(fd>=0)
            {
                read(fd,(void*)config_st_ptr,fsize);
                close(fd);
                g_sys.conf = *config_st_ptr;
                rt_kprintf("CONF_FACT loaded successfully.\n");
                set_startup_flag(CONF_OPT_USR);
            }
            else
            {
                rt_kprintf("Error open fact conf file\n");
                rt_kprintf("CONF_FACT loaded fail.\n");
            }
            break;
        }
        case(CONF_OPT_DEBUT):
        {
            init_load_default();
            save_conf(CONF_OPT_USR);
            save_conf(CONF_OPT_FACT);
            set_startup_flag(CONF_OPT_USR);            
            rt_kprintf("CONF_DEBUT loaded successfully.\n");
            break;
        }        
        default:
        {
            init_load_default();
            save_conf(CONF_OPT_USR);
            save_conf(CONF_OPT_FACT);
            set_startup_flag(CONF_OPT_USR);
            init_load_default();
            rt_kprintf("CONF_DEFAULT loaded successfully.\n");
            break;
        }
    }
    rt_free(config_st_ptr);
}

void usr_init(void)
{
    int16_t ret;
    ret = dir_init();  
  
    init_load_status();
    
    if(ret != 0)
    {
        load_conf(CONF_OPT_DEBUT);
    }
    else
    {
        load_conf(load_startup_flag());
    }
}

static int16_t set_serial_id(uint32_t serial_id)
{
    DIR *dirp;
    int16_t ret;
    int fd, fsize;
  
    ret = 0;
    fsize = sizeof(uint32_t);
    
    dirp = opendir("/conf");
    if (dirp == RT_NULL)
    {
        rt_kprintf("dir /conf not exist, creat one\n");
        ret = mkdir("/conf", 0x777);
        if(ret < 0)
        {
            rt_kprintf("mkdir /conf fail\n");
            ret = -1;
            return ret;
        }
        else
        {
            rt_kprintf("mkdir /conf ok\n");
            dirp = opendir("/conf");
        }
    }    
    
    fd = open("/conf/serial_id.dat", O_WRONLY|O_CREAT);
    if(fd>=0)
    {
        write(fd,(void*)&serial_id,fsize);
        close(fd);
        
        ret = 0;
    }
    closedir(dirp);
    return ret;
}

uint32_t get_serial_id(void)
{
    int32_t ret;
    int fd, fsize;
    uint32_t serial_id;
  
    ret = 0;
    fsize = sizeof(uint32_t);    

    fd = open("/conf/serial_id.dat", O_RDONLY);
    if(fd>=0)
    {
        read(fd,(void*)&serial_id,fsize);
        close(fd);
        ret = serial_id;
    }
    return ret;
}

  
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(set_serial_id, uint32 type ID.);
FINSH_FUNCTION_EXPORT(set_startup_flag,1:usr 2:fact 3:debut 4:default.);
FINSH_FUNCTION_EXPORT(save_conf, 1:usr 2:fact else:default.);
FINSH_FUNCTION_EXPORT(load_conf, load conf regs from file.);
#endif
  

