#include <rtthread.h>
#include <dfs_posix.h>
#include "sys_conf.h"
#include "ethernetif.h"
#include "bit_op.h"

static int fget_sdn(uint8_t *svr_dns)
{
    int ret;
    int fd;
    const char default_sdn[] = DEFAULT_SDN;
    struct stat buf;
  
    ret = stat("/conf/dns.dat", &buf);
    if(ret==0)
    {
        fd = open("/conf/dns.dat", O_RDONLY);
        if(fd>=0)
        {
            ret = read(fd,svr_dns,buf.st_size);
            close(fd);
            *(svr_dns+ret) = '\0';
        }
    }
    else
    {
        strcpy((char *)svr_dns,default_sdn);
    }        
    return ret;
}

int fset_sdn(const char *svr_dns)
{
    int ret;
    int fd;
  
    fd = open("/conf/dns.dat", O_WRONLY|O_CREAT|O_TRUNC);
    if(fd>=0)
    {
        ret = write(fd,svr_dns,strlen(svr_dns));
        close(fd);
    }
    else
        ret = fd;
    return ret;
}

int32_t get_link_sts(void)
{
    extern  sys_reg_st  g_sys;
    int32_t ret;
    if(bit_op_get(g_sys.stat.gen.status_bm,GBM_LINK)&&bit_op_get(g_sys.stat.gen.status_bm,GBM_DHCP))
        ret = 0;
    else
        ret = -1;
    return ret;
}

int32_t get_remote_ip(void)
{
    extern  sys_reg_st  g_sys;
    int32_t ret;
    int dns_len;
    uint8_t dns_buf[32];          
  
    ret = 0;
  
    if(g_sys.conf.eth.dns_en)
    {
        if(bit_op_get(g_sys.stat.gen.status_bm,GBM_DNS)==0)
        {
            dns_len = fget_sdn(dns_buf);
            ret = get_dns((const char*)dns_buf);
            g_sys.stat.eth.server_ip = g_sys.stat.eth.dns_ip;
            if(dns_len>0)
                rt_kprintf("Set remote server using new DNS.\n");
            else
                rt_kprintf("Server domain name not exist in fs,using default!\n");
        }
        else
        {
            g_sys.stat.eth.server_ip = g_sys.stat.eth.dns_ip;
            rt_kprintf("Set remote server using previous DNS.\n");
        }
    }
    else
    {
        if(bit_op_get(g_sys.stat.gen.status_bm,GBM_DNS)==1)
            bit_op_set(&g_sys.stat.gen.status_bm,GBM_DNS,0);
        g_sys.stat.eth.server_ip = g_sys.conf.eth.remote_ip;
        rt_kprintf("Set remote server using local config.\n");
    }
    return ret;
}


#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(fset_sdn, server_dns write .);
#endif
