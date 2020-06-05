/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_plug.c
 *
 * Description: plug demo's function realization
 *
 * Modification history:
 *     2014/5/1, v1.0 create this file.
*******************************************************************************/

#include "iw_priv.h"

/* 该sdk 如果没有发送回调函数，程序跑飞*/
/* the sdk must be have socket sendback function, or the 8266 will Run to fly */
void  ICACHE_FLASH_ATTR 
iw_dev_sendback(void *arg)
{
        //os_printf("send ok\n");
}

void  ICACHE_FLASH_ATTR 
iw_lan_srv_sendback(void *arg)
{
        //os_printf("send ok\n");
}

void * ICACHE_FLASH_ATTR 
iw_udp_server_create(u_int32_t ip, u_int16_t port, iw_udp_recv_cb cb, espconn_sent_callback sb)
{
	struct espconn *conn;

	conn = iw_malloc(sizeof(*conn));
	if(conn == NULL)
		return NULL;
	
	conn->type = ESPCONN_UDP;
    conn->state = ESPCONN_NONE;
	conn->proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
	if(port)
		conn->proto.udp->local_port = port;
	else
		conn->proto.udp->local_port = espconn_port();
	
	if(espconn_create(conn) != ESPCONN_OK) {
                        os_free(conn);
                        return NULL;
     }

     if(espconn_regist_recvcb(conn, cb) != ESPCONN_OK) {
                espconn_delete(conn);
                os_free(conn);
                return NULL;
     }

 #if 1            
     if(espconn_regist_sentcb(conn, sb) != ESPCONN_OK) {
                espconn_delete(conn);
                os_free(conn);
                return NULL;
     }
#endif
             
	return (void *)conn;
}

//必须是主机字节序
int ICACHE_FLASH_ATTR iw_udp_send(u_int32_t ip, u_int16_t port, char *data, int len, void *udp)
{
	struct espconn *conn = (struct espconn *)udp;

	ip = htonl(ip);
	os_memcpy(conn->proto.udp->remote_ip, &ip, 4);
	conn->proto.udp->remote_port = port;
	espconn_sent(conn, (uint8 *)data, len);
#if 0	
	os_printf("send pkt, ip: %u.%u.%u.%u, remote_port:%d, port:%d, len:%d\n", 
	conn->proto.udp->remote_ip[0], conn->proto.udp->remote_ip[1],
	conn->proto.udp->remote_ip[2], conn->proto.udp->remote_ip[3],
	conn->proto.udp->remote_port, conn->proto.udp->local_port, len);
#endif
	return len;
}

u_int64_t ICACHE_FLASH_ATTR t_ntoh_ll(u_int64_t n)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	return (((n>>56)&0xFFULL)
			|((n>>40)&0xFF00ULL)
			|((n>>24)&0xFF0000ULL)
			|((n>>8)&0xFF000000ULL)
			|((n<<8)&0xFF00000000ULL)
			|((n<<24)&0xFF0000000000ULL)
			|((n<<40)&0xFF000000000000ULL)
			|((n<<56)&0xFF00000000000000ULL));
#elif __BYTE_ORDER == __BIG_ENDIAN
	return n;
#else
# error "Please fix <bits/endian.h>"
#endif
}


