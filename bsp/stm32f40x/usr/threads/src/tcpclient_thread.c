#include <rtthread.h>
#include <string.h>
#include <sys/socket.h> /* 使用BSD socket，需要包含socket.h头文件 */
#include "sys_conf.h"
#include "threads.h"
#include "fifo.h"
#include "net_utils.h"
#include "bit_op.h"

#define BUFSZ 1024

static uint32_t tx_buf[BUFSZ];
static uint8_t  rx_buf[BUFSZ];
static uint8_t  tcp_flag;
//uint32_t rx_buf[BUFSZ];

extern  sys_reg_st  g_sys;		
extern  fifo32_cb_td cmd_rx_fifo;
extern  fifo32_cb_td cmd_tx_fifo;

static rt_uint8_t tcp_tx_stack[ 1024 ];
static rt_uint8_t tcp_rx_stack[ 1024 ];
static struct rt_thread tcp_tx_thread;
static struct rt_thread tcp_rx_thread;
static void tcp_tx_thread_entry(void* parameter);
static void tcp_rx_thread_entry(void* parameter);

/**
  * @brief 	tcp client 
	* @param  none
	* @retval none
  */
void tcpclient_thread_entry(void* parameter)
{
    rt_err_t result;
    uint16_t keepalive;  
    int sock;
    struct sockaddr_in server_addr;
  
    rt_thread_delay(TCPCLIENT_THREAD_DELAY);

    keepalive = 1;
    tcp_flag = 0;

  
    while(1)
    {
        if(g_sys.conf.eth.tcp_client_en == 1)
        {
            /* 创建一个socket，类型是SOCKET_STREAM，TCP类型 */
            if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            {
                /* 创建socket失败 */
                rt_kprintf("Socket error\n");

                return;
            }

            while(get_link_sts() != 0)
            {
                rt_thread_delay(1000);
                rt_kprintf("waiting for linkup or dhcp......\n");
            }
            
            if(get_remote_ip() != 0)
            {
                rt_thread_delay(1000);
                continue;
            }
            
            /* 初始化预连接的服务端地址 */
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(g_sys.conf.eth.remote_port);
            server_addr.sin_addr = *(struct in_addr *)(&g_sys.stat.eth.server_ip);
            rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));            
            
            setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(uint16_t));            
            
            /* 连接到服务端 */
            if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
            {
                /* 连接失败 */
                rt_kprintf("Connect fail! Remote ip:port: (%s:%d)\n",
                           inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
                closesocket(sock);
        
                rt_thread_delay(g_sys.conf.eth.reconnect_period);
                bit_op_set(&g_sys.stat.gen.status_bm,GBM_TCP,0);
//                g_sys.stat.gen.status_bm &= ~(0x00000001<<GBM_TCP);
                continue;
            }
            else
            {
                bit_op_set(&g_sys.stat.gen.status_bm,GBM_TCP,1);
//                g_sys.stat.gen.status_bm |= (0x00000001<<GBM_TCP);
                rt_kprintf("Connect scuccess! Remote ip:port: (%s:%d)\n",
                           inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
              
                result = rt_thread_init(&tcp_tx_thread,
                                        "tcp_tx",
                                        tcp_tx_thread_entry,
                                        &sock,
                                        (rt_uint8_t*)&tcp_tx_stack[0],
                                        sizeof(tcp_tx_stack),
                                        TCP_TX_THREAD_PRIO,
                                        5);
                if (result == RT_EOK)
                {
                    rt_thread_startup(&tcp_tx_thread);
                }                  
                
                result = rt_thread_init(&tcp_rx_thread,
                                        "tcp_rx",
                                        tcp_rx_thread_entry,
                                        &sock,
                                        (rt_uint8_t*)&tcp_rx_stack[0],
                                        sizeof(tcp_rx_stack),
                                        TCP_RX_THREAD_PRIO,
                                        5);
                if (result == RT_EOK)
                {
                    rt_thread_startup(&tcp_rx_thread);
                }    
                
                while (1)
                {
                    if((tcp_flag != 0)||(g_sys.conf.eth.tcp_client_en == 0)||(!bit_op_get(g_sys.stat.gen.status_bm,GBM_LINK))||(!bit_op_get(g_sys.stat.gen.status_bm,GBM_LINK)))
                    {
                        rt_thread_detach(&tcp_rx_thread);
                        rt_thread_detach(&tcp_tx_thread);
                        rt_kprintf("detach tcp_rtx threads\n");
                        tcp_flag = 0;
                        break;
                    }                    
                    rt_thread_delay(1000);
                }
                closesocket(sock);
                bit_op_set(&g_sys.stat.gen.status_bm,GBM_TCP,0);
//                g_sys.stat.gen.status_bm &= ~(0x00000001<<GBM_TCP);
            }
        }
        rt_thread_delay(100);
    }
}

static void tcp_tx_thread_entry(void* parameter)
{    
    int t_sock,i,ret;
		rt_thread_delay(100);
    uint16_t buf_len;
    t_sock = *(int*)parameter;
		
    fifo32_reset(&cmd_tx_fifo);
  
    ret = 0;
		while(1)
		{
        /* 发送数据到sock连接 */
        if(is_fifo32_empty(&cmd_tx_fifo) == 0)
        {
            buf_len = get_fifo32_length(&cmd_tx_fifo);
            for(i=0;i<buf_len;i++)
            {
                fifo32_pop(&cmd_tx_fifo, &tx_buf[i]);
            }
            ret = send(t_sock, tx_buf, (i<<2), 0);
            if (ret < 0)
            {
                rt_kprintf("\nsend error, close socket %x.\r\n",ret);
                tcp_flag |= 2;
                rt_thread_delay(1000000);
            }
            else if (ret == 0)
            {
                /* 打印send函数返回值为0的警告信息 */
                rt_kprintf("\nSend warning,send function return 0.\r\n");
            }
        }
        rt_thread_delay(50);
		}    
}

static void tcp_rx_thread_entry(void* parameter)
{    
    int r_sock,i;
    int bytes_received;
    uint32_t host_data;
		rt_thread_delay(100);
    r_sock = *(int*)parameter;
		
		while(1)
		{
        bytes_received = recv(r_sock, (char*)rx_buf, BUFSZ - 1, 0);
        if (bytes_received < 0)
        {
            rt_kprintf("\nsock err %d.\r\n",bytes_received);
              tcp_flag |= 1;
              rt_thread_delay(1000000);
        }
        else if (bytes_received == 0)
        {
          
            rt_kprintf("\nTcp connection close 0.\r\n");
            tcp_flag |= 1;
            rt_thread_delay(1000000);
            
        }
        for(i=0;i<(bytes_received>>2);i++)
        {
            host_data = ntohl(*(uint32_t *)(rx_buf+4*i));
            fifo32_push(&cmd_rx_fifo, &host_data);
        }
        rt_thread_delay(10);
		}
}



