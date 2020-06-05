/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: iWulink
**  File:    iw_debug_net.c
**  Author:  cailiyang
**  Date:    03/11/2016
**
**  Purpose:
**    网络调试.
**************************************************************************/


/* Include files. */

#include "iw_api.h"
#include "iw_priv.h"
#include "udp_ctrl.h"
#include "iw_lan_srv.h"
#include "stlc_list.h"
#include "iw_dev.h"
#include "iw_debug_net.h"
#include <stdarg.h>

/* Macro constant definitions. */
#define UART_MESG_LEN_MAX (200)
#define UART_MESG_COUNT_MAX (20)


/* Type definitions. */

typedef struct uart_netmesg_s{
	struct stlc_list_head link;
	ucds_session_t *msg_session;
	u_int8_t mesg[UART_MESG_LEN_MAX];
	int mesg_len;
} uart_netmesg_t;

typedef struct uart_netmesg_ctrl_s{
	struct stlc_list_head uart_netmsg_ctrl_link;
	//cl_thread_t *t_work_timer;
} uart_netmesg_ctrl_t;

/* Local function declarations. */
int ets_snprintf(char *str, size_t size, const char *format, ...);
int ucds_enc_pkt(ucds_session_t *s, pkt_t *pkt);
void pkt_free(void *pkt);

static void ucds_net_debug_notify(u_int8_t *buff, int len);
static void ucds_net_debug_notify_later(u_int8_t *buff, int len);

static void _ucds_net_debug_notify(ucds_session_t *s, u_int8_t *buff, int len);
static int net_debug_notify_work(void *t);
static bool iw_in_irq(void);

int ets_vsprintf(char *str, const char *format, va_list argptr);
int ets_vsnprintf(char *buffer, size_t sizeOfBuffer,  const char *format, va_list argptr);


/* Macro API definitions. */


/* Global variable declarations. */

static u_int8_t intr_debug = 0;

static uart_netmesg_ctrl_t uart_netmesg_ctrl;



void ICACHE_FLASH_ATTR ucds_net_debug_notify_init(void)
{
	os_memset(&uart_netmesg_ctrl, 0, sizeof(uart_netmesg_ctrl));
	
	STLC_INIT_LIST_HEAD(&uart_netmesg_ctrl.uart_netmsg_ctrl_link);
	
	iw_timer_set(TID_DEBUG_THREAD, 500, 1, net_debug_notify_work, NULL);

	
	//uart_netmesg_ctrl.t_work_timer = NULL;
	//CL_THREAD_TIMER_ON(MASTER, uart_netmesg_ctrl.t_work_timer, net_debug_notify_work, NULL, 500);
}

void ICACHE_FLASH_ATTR iw_debug_net(u_int8_t level, char *fmt, va_list ap)
{
    static char string[256];
	int len = 0;

	len = ets_snprintf(string, sizeof(string), "%u %u(s) |", level, get_up_sec());
	len += ets_vsnprintf(&string[len], sizeof(string) - len, fmt, ap);
	
	/*通过网络打印调试*/
	if (iw_in_irq()){
		ucds_net_debug_notify_later((u_int8_t *)string, len);
	}else{
		ucds_net_debug_notify((u_int8_t *)string, len);
	}
	
}


static void ICACHE_FLASH_ATTR ucds_net_debug_notify(u_int8_t *buff, int len)
{
	int i;
	ucds_session_t *s;

	if (NULL == buff)
		return;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];
		if (s->is_used && s->debug)	{
			_ucds_net_debug_notify(s, buff, len);
		}
	}
}

//在串口里面的打印，由于受到中断的影响，不能直接网络dump出来，故增加这个延迟dump的函数
static void ICACHE_FLASH_ATTR ucds_net_debug_notify_later(u_int8_t *buff, int len)
{
	int i;
	ucds_session_t *s;
	uart_netmesg_t *uart_netmesgp = NULL;
	int count = 0;

	if (NULL == buff)
		return;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];
		
		if (s->is_used && s->debug)	{

			stlc_list_count(count, &uart_netmesg_ctrl.uart_netmsg_ctrl_link);
			if(count > UART_MESG_COUNT_MAX){
				break;
			}

			uart_netmesgp = iw_calloc(sizeof(uart_netmesg_t), 1);
			if(uart_netmesgp == NULL){
				break;
			}
			
			uart_netmesgp->msg_session = s;
			uart_netmesgp->mesg_len = len;
			os_memcpy(uart_netmesgp->mesg, buff, UART_MESG_LEN_MAX);
			
			stlc_list_add_tail(&uart_netmesgp->link, &uart_netmesg_ctrl.uart_netmsg_ctrl_link);
		}
	}
}

static void ICACHE_FLASH_ATTR _ucds_net_debug_notify(ucds_session_t *s, u_int8_t *buff, int len)
{
	pkt_t *pkt;
	u_int8_t *buf;
	int n;
	
	//用hdr->flag来表示到底是回的调试开关命令响应包还是普通push包。flag为0表示响应包，为1表示push包。
	pkt = uc_pkt_new(CMD_USER_DEBUG, len,
		false, s->is_enc, 0x1, s->client_sid , ucd.client.device_sid, s->my_request_id);
	
	if (pkt == NULL) {
		return ;
	}

	buf = get_ucp_payload(pkt, u_int8_t );
	os_memcpy(buf, buff, len);

	ucds_enc_pkt(s, pkt);
	//uc_send_pkt_raw(s->sock, s->ip, s->port, pkt);

	n = iw_udp_send(s->ip, s->port, (char *)pkt->data, pkt->total, s->conn);
	if (n <= 0) {
		os_printf("ucds_lpc_send %s send pkt failed: ip=%u.%u.%u.%u, port=%d, want=%d, send=%d\n",
		          s->name, IP_SHOW(s->ip), s->port, pkt->total, n);
	}

	pkt_free(pkt);
}


static int ICACHE_FLASH_ATTR net_debug_notify_work(void *t)
{
	uart_netmesg_t *node, *next;
	
	stlc_list_for_each_entry_safe(uart_netmesg_t, node, next, &uart_netmesg_ctrl.uart_netmsg_ctrl_link, link){
		
		_ucds_net_debug_notify(node->msg_session, node->mesg, node->mesg_len);
		stlc_list_del(&node->link);
		iw_free(node);
	}

	return 0;
}


//判断当前是否处于中断上下文中
static bool ICACHE_FLASH_ATTR iw_in_irq(void)
{
	// TODO:  功能未实现
	return (intr_debug != 0);
}


