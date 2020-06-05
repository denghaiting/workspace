/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    ice_domain.h
**  Author:  fangfuhan
**  Date:    06/12/2016
**
**  Purpose:
**    Dns解析.
**************************************************************************/


#ifndef ICE_DOMAIN_H
#define ICE_DOMAIN_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "domain.h"
#include "ip_addr.h"
/* Macro constant definitions. */
#define GDR_MAX_IP  		(3)

#define NDS_CLI_MAX_IP				 (3)   //一个域名对应的最多ip
/* Type definitions. */

typedef struct {
	char *doname;
	u_int32_t ip_num;           //域名对应的IP数
	u_int32_t ip[GDR_MAX_IP];   //具体的IP
} gdr_hostent_t;


typedef struct {	
	u_int8_t state;	
	struct espconn *dns_conn;	//SOCKET sock;	
	pkt_t *pkt;	
	u_int8_t *rcv_buf;	
	u_int8_t send_retry;	
	u_int32_t domain_idx;		
	u_int32_t ip_idx;	
	u_int32_t port_idx;			//产生时间事件，推动状态机
	u_int32_t sys_err_num;

	gdr_hostent_t *reso_doname; //指向解析需要解析的域名数组
	u_int8_t domain_num;		//域名的个数
	u_int8_t reso_index;		//解析到哪一个域名了
	u_int8_t reso_ok;			//域名解析ok
	struct espconn *reso_conn;	//域名解析的结构
	u_int8_t enable_send_doname;	//接收到一个域名的ip
	u_int8_t afreso_event;		//接收到一个域名的ip
	u_int8_t is_reso_doname;	//是否解析数组里面的域名成功
	u_int8_t ip_index;			//在数组中那个位置
	u_int8_t dns_send_index;	//如果第一个发送不成功， 就发送第二个， 这个是计数位
} dn_t;

/** For compatibility with BSD code */
struct in_addr {
  u_int32_t s_addr;
};

/* External function declarations. */


/* Macro API definitions. */
#define inet_aton(cp, addr)   ipaddr_aton(cp, (ip_addr_t*)addr)


/* Global variable declarations. */
int dns_cli_parse_packet(char *name, u_int8_t* data, int data_len, u_int32_t addr[NDS_CLI_MAX_IP], int addr_count);
dm_type_e domain_get_type();
int dn_start(void);
void dn_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ICE_DOMAIN_H */

