#ifndef __IWUOS_NET_HEADER__
#define __IWUOS_NET_HEADER__

//#include "espconn.h"
//#include "stlc_list.h"
//#include "iw_api.h"

#if (IW_PLATFORM==PLT_8266)
u_int64_t t_ntoh_ll(u_int64_t n);
#define ntohl(_n)  \
        ((((u_int32_t)(_n) & 0xff000000) >> 24) | (((u_int32_t)(_n) & 0x00ff0000) >> 8) \
                | (((u_int32_t)(_n) & 0x0000ff00) << 8) | (((u_int32_t)(_n) & 0x000000ff) << 24))
                
#define htonl(_n) ntohl(_n)

#define ntohs(_n) \
        ((((u_int16_t)(_n) & 0xff00) >> 8) | (((u_int16_t)(_n) & 0x00ff) << 8))
        
#define htons(_n) ntohs(_n)
#define ntoh_ll(_n) t_ntoh_ll(_n)
#define hton_ll(_n) ntoh_ll(_n) 
#endif

typedef void (* iw_dns_cb)(void *arg, char *host, u_int32_t ip);
typedef void (* iw_udp_recv_cb)(void *arg, char *pdata, unsigned short len);

/*
	功能:
		cs模型初始化，
	输入参数:
		@ip: ip地址
		@port: 端口号
		@cb: 接收回调
		@sb: 发送回调
	输出参数:
	返回值:
		struct espconn * :成功
		NULL :失败
	说明:
*/
void *iw_udp_server_create(u_int32_t ip, u_int16_t port, iw_udp_recv_cb cb, espconn_sent_callback sb);
/*
	功能:
		通过网络发送数据
	输入参数:
		@ip: 对端IP地址
		@port: 对端端口
		@data: 数据
		@len: 数据长度
		@udp: 由iw_udp_server_create得到的不断通信结构
	输出参数:
		@info: 保存系统信息
	返回值:
		int :  内部没有处理，直接返回的len
	说明:
		必须是主机字节序
*/
int iw_udp_send(u_int32_t ip, u_int16_t port, char *data, int len, void *udp);
/*
	功能:
		发送回调
	输入参数:
		@arg: 参数
	输出参数:
	返回值:
	说明:
		该sdk 如果没有发送回调函数，程序跑飞
*/
void iw_dev_sendback(void *arg);
/*
	功能:
		接收回调
	输入参数:
		@arg: 参数
	输出参数:
	返回值:
	说明:
*/
void iw_lan_srv_sendback(void *arg);

typedef struct{
#if __BYTE_ORDER == __BIG_ENDIAN
	u_int8_t ver:3,
		hlen:3,
		request:1,
		encrypt:1;
#elif __BYTE_ORDER == __LITTLE_ENDIAN
	u_int8_t 	encrypt:1,
		request:1,
		hlen:3,
		ver:3;
#else
# error "Please fix <bits/endian.h>"
#endif
	u_int8_t flags;
	u_int8_t request_id;
	u_int8_t resv;
} xxucph_t;


#endif
