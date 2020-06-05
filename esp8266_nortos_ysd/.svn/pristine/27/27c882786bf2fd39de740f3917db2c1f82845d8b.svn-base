/**************************************************************************
**  Copyright (c) 2010 GalaxyWind, Ltd.
**
**  Project: iw_uart_active
**  File:    iw_uart_active.c
**  Author:  
**  Date:    06/09/2015
**
**  Purpose:
**    Uart active device.
**************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "licence.h"

/* Macro constant definitions. */


/* Type definitions. */

//状态
enum {
	//空闲，可接受+++
	AC_FSM_IDLE	= 0,
	//收到+++后回激活状态
	AC_FSM_WAITT_REPLY = 1,
	//接收证书数据
	AC_FSM_WAIT_DATA = 2,
	//等待证书校验及保存
	AC_FSM_WAIT_PROC = 3,
	//激活成功后，等待重启
	AC_FSM_WAIT_RESET = 4,
	//命令模式，主要用来处理一些串口通用命令
	AC_FSM_CMD_MODE = 5,
	AC_FSM_MAX
};

//data接收超时
#define 	ACTIVE_DATA_RECV_TIMEOUT	1
//表示返回
#define 	SERIAL_RET				1

#define 	UART_UART_BUFF_LEN	254
#define 	UART_LICENCE_LEN		sizeof(serial_active_t)
#define 	UART_LICENCE_KEY_LEN		sizeof(lcc_key_t)

#define 	DEV_CMD_NO_ACTIVE	0	//表示为激活过
#define 	DEV_CMD_IS_ACTIVE	1	//表示激活过
#define 	DEV_CMD_ACTIVE_OK	2	//表示激活成功
#define 	DEV_CMD_ACTIVE_TIMEOUT	3	//表示超时，没有返回任何结果
#define 	DEV_CMD_IS_ACTIVE_SN	4	//表示激活过并上报已经激活SN
#define 	DEV_CMD_IS_IMPC	5	//测试证书激活，告诉前端我是陪测

#define 	ACITVE_TIMER_MSEC		100

//#define ACTIVE_DEBUG_
#ifdef ACTIVE_DEBUG_
#define ACTIVE_INFO(x...)  INFO(x)
#define ACTIVE_DEBUG(x...) DEBUG(x)
#define ACTIVE_ERROR(x...)  ERROR(x)
#else
#define ACTIVE_DEBUG(x...)
#define ACTIVE_INFO(x...)
#define ACTIVE_ERROR(x...) ERROR(x)
#endif


#pragma  pack(push,1)
typedef struct serial_active_s{
	lcc_key_t key;
	u_int8_t pad[3];
	u_int8_t checksum;
}serial_active_t;

//串口模式协议头部
typedef struct cmd_mode_hd_s{
	u_int8_t cmd;
	u_int8_t param_len;
	u_int8_t ret;
	u_int8_t check_sum;
}cmd_mode_hd_t;
typedef struct cmd_mode_ctrl_s{
	u_int32_t last_input_time;
	u_int8_t status;
	u_int8_t index;
	cmd_mode_hd_t hd;
}cmd_mode_ctrl_t;

typedef struct {
	u_int32_t active_status;
	u_int64_t active_sn;
}net_active_t;

#pragma pack(pop)

typedef struct serial_ctrl_s{
	bool activing;
	u_int8_t status;	
	u_int8_t uart_value_index;
	u_int32_t time_out;
	u_int32_t last_time;
	bool get_cmd;
	cmd_mode_ctrl_t cmd_mode_ctrl;
	u_int8_t uart_buff[UART_UART_BUFF_LEN];
}serial_ctrl_t;


typedef void (* uartact_func_t)();
typedef void (* uartact_func_t_read)(uint8);

typedef struct serial_fsm_s{
	char *name;
	uartact_func_t on_do;
	uartact_func_t_read on_read;
	uartact_func_t out_do;
	uartact_func_t timer;
}serial_fsm_t;


/* Local function declarations. */
serial_ctrl_t sa_ctrl;
volatile bool activing = false;

static void idle_ondo(void);
static void idle_read(u_int8_t ch);
static void reply_ondo(void);
static void reply_timer(void);
static void data_ondo(void);
static void data_read(u_int8_t ch);
static void data_timer(void);
static void wait_timer(void);
static void reset_ondo(void);
extern void  BSP_ExitActiveMode(VOID);
extern void  BSP_EnterActiveMode(VOID);
extern void  BSP_ActiveUartPutcPolled(uint8 Data);
extern void  BSP_ActiveUartPutstr(const char *str);
/* Macro API definitions. */


/* Global variable declarations. */
serial_fsm_t serial_fsm[AC_FSM_MAX] = {
	{"idle", idle_ondo, idle_read, NULL, NULL},
	{"wait reply", reply_ondo, NULL, NULL, reply_timer},
	{"wait data", data_ondo, data_read, NULL, data_timer},
	{"wait proc", NULL, NULL, NULL, wait_timer},
	{"wait reset", reset_ondo, NULL, NULL, NULL},
};

static void ICACHE_FLASH_ATTR serial_status_set(u_int8_t status)
{
	u_int8_t last_status;
	
	if (status >= AC_FSM_MAX) {
		return;
	}

	last_status = sa_ctrl.status;
	
	if (serial_fsm[last_status].out_do) {
		serial_fsm[last_status].out_do();
	}
	sa_ctrl.status = status;	
	if (serial_fsm[status].on_do) {
		serial_fsm[status].on_do();
	}
}

static void ICACHE_FLASH_ATTR idle_ondo(void)
{
	BSP_ExitActiveMode();
	activing = false;
}

static bool ICACHE_FLASH_ATTR uart_active_enter_mode(u_int8_t ch)
{
	static int plus_num = 0;
	
	if (ch == '+'){
		plus_num++;
	}else{
		plus_num = 0;
	}

	if (plus_num == 3){
		plus_num = 0;
		return TRUE;
	}

	return FALSE;
}

static void ICACHE_FLASH_ATTR idle_read(u_int8_t ch)
{
	if (uart_active_enter_mode(ch)) {
		serial_status_set(AC_FSM_WAITT_REPLY);
		return;
	}

}

static void ICACHE_FLASH_ATTR reply_ondo(void)
{
	system_set_os_print(0);
	BSP_EnterActiveMode();
	activing = true;
}

static void ICACHE_FLASH_ATTR active_reply_flag(void)
{
	BSP_ActiveUartPutcPolled('+');
	BSP_ActiveUartPutcPolled('+');
	BSP_ActiveUartPutcPolled('+');	
}


static void ICACHE_FLASH_ATTR uart_active_reply_notactive(void)
{
	active_reply_flag();
	BSP_ActiveUartPutcPolled(DEV_CMD_NO_ACTIVE);
}


static void ICACHE_FLASH_ATTR active_status_reply(void)
{
	uart_active_reply_notactive();
}


static void  ICACHE_FLASH_ATTR reply_timer(void)
{
	active_status_reply();
	sa_ctrl.uart_value_index = 0;
	serial_status_set(AC_FSM_WAIT_DATA);
}


static void ICACHE_FLASH_ATTR data_ondo(void)
{
	sa_ctrl.uart_value_index = 0;

	sa_ctrl.last_time = get_sec();
	sa_ctrl.time_out = ACTIVE_DATA_RECV_TIMEOUT;	
}

static void ICACHE_FLASH_ATTR data_read(u_int8_t ch)
{
	u_int8_t index = sa_ctrl.uart_value_index++%UART_UART_BUFF_LEN;

	sa_ctrl.uart_buff[index] = ch;
	
	if ((index + 1) == UART_LICENCE_LEN) {
		serial_status_set(AC_FSM_WAIT_PROC);
	}	
}

static void ICACHE_FLASH_ATTR data_timer(void)
{
	if (get_sec() - sa_ctrl.last_time > sa_ctrl.time_out) {
		serial_status_set(AC_FSM_IDLE);
		system_set_os_print(1);
	}
}


static bool ICACHE_FLASH_ATTR acitve_data_check(u_int8_t *data, int data_len)
{
	int i;
	u_int8_t checksum = 0;

	for(i = 0; i < data_len - 1; i++) {
		checksum ^= data[i];
	}

	return (checksum == data[i]?true:false);
} 

static bool ICACHE_FLASH_ATTR  licence_active(void *data, int data_len)
{
	lcc_nvm_key_t nvm_key;
	lcc_key_t *pkey = NULL;
	u_int8_t uuid[MU_DEV_UUID_LEN];
	
	nvm_key.magic = LCC_MAGIC;
	nvm_key.valid = TRUE;

	if (data_len != sizeof(*pkey)) {
		return false;
	}

	os_memcpy((void *)&nvm_key.key, data, data_len);

	if (lcc_write(&nvm_key) == RS_ERROR) {
		return false;
	}
	//清除uuid
	os_memset(uuid, 0, sizeof(uuid));
	uuid_write(uuid, sizeof(uuid));
	
	return true;
}


static bool ICACHE_FLASH_ATTR active_check(void)
{
	//checksum
	if (acitve_data_check(sa_ctrl.uart_buff, UART_LICENCE_LEN) == false) {
		return false;
	}

	if (licence_active(sa_ctrl.uart_buff, UART_LICENCE_KEY_LEN) == false) {
		BSP_ActiveUartPutstr("licence_active failed\n");
		return false;	
	}		

	return true;
}


static void ICACHE_FLASH_ATTR uart_active_reply_timeout(void)
{
	active_reply_flag();
	BSP_ActiveUartPutcPolled(DEV_CMD_ACTIVE_TIMEOUT);
}

static void ICACHE_FLASH_ATTR uart_active_reply(void)
{
	active_reply_flag();
	BSP_ActiveUartPutcPolled(DEV_CMD_ACTIVE_OK);
}


static void  ICACHE_FLASH_ATTR wait_timer(void)
{
	if (active_check() == false) {
		uart_active_reply_timeout();
		serial_status_set(AC_FSM_IDLE);
		system_set_os_print(1);
	} else {
		//返回原值当ok
		uart_active_reply();
		serial_status_set(AC_FSM_WAIT_RESET);
	}
}

static void ICACHE_FLASH_ATTR reset_ondo(void)
{	
	iw_request_event(EVENT_SYS_REBOOT);
	serial_status_set(AC_FSM_IDLE);
}


static void  ICACHE_FLASH_ATTR serial_active(u_int8_t ch)
{
	if (serial_fsm[sa_ctrl.status].on_read) {
		serial_fsm[sa_ctrl.status].on_read(ch);
	}
}


static rs_t ICACHE_FLASH_ATTR iw_uart_2active(iw_event_info_t *INOUT_info)
{
	u_int8_t *pvalue = NULL;
	iw_event_info_t *info = INOUT_info;
	u_int8_t ch;
	if(INOUT_info == NULL || INOUT_info->event != IW_EV_UARTR_2COMMON) {
		BSP_ActiveUartPutstr("not find event IW_EV_UARTR_2COMMON child IW_EV_UARTR_2ACTIVE \n");
		return RS_ERROR;
	}

	pvalue = (u_int8_t *)(info->cu.cp);
	ch = *(pvalue);
	serial_active(ch);

	return RS_OK;
}


static rs_t ICACHE_FLASH_ATTR serial_timer(void *p)
{
    iw_timer_set(TID_UART_ACTIVE, ACITVE_TIMER_MSEC, 0, serial_timer, NULL);
	if (serial_fsm[sa_ctrl.status].timer) {
		serial_fsm[sa_ctrl.status].timer();
	}
	return 0;
}

void ICACHE_FLASH_ATTR iw_uart_active_init(void)
{
	os_memset(&sa_ctrl, 0, sizeof(sa_ctrl));

    iw_timer_set(TID_UART_ACTIVE, ACITVE_TIMER_MSEC, 0, serial_timer, NULL);
	serial_status_set(AC_FSM_IDLE);
	
	if(iw_event_register(IW_EV_UARTR_2ACTIVE, iw_uart_2active) != RS_OK){
		BSP_ActiveUartPutstr("iw_uart_active_init error \n");	
	}
}

/***********************************wifi active
********************************************************************/
void ICACHE_FLASH_ATTR do_ucot_system_active(iw_event_info_t *info, iw_obj_t *uobj)
{
	net_active_t net_active;
             
	u_int8_t action = 0;
	u_int8_t *param = NULL; 

	if(!info) {
	        return;
	}
	action = info->action;
	param = (u_int8_t *)(info + 1);


	if (action == UCA_GET) {
		if (g_dev.active){
			net_active.active_status = htonl(DEV_CMD_IS_ACTIVE);
			net_active.active_sn = ntoh_ll(g_dev.sn);
		} else {
			net_active.active_status = htonl(DEV_CMD_NO_ACTIVE);
			net_active.active_sn = 0;
		}
		ACTIVE_ERROR("***********************net_active.active_status=%u\n", net_active.active_status);
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		uobj->sub_objct, uobj->attr, sizeof(net_active), (u_int8_t *)&net_active);
		return;
	} else if (action == UCA_SET) {
		if (uobj->param_len < UART_LICENCE_LEN) {
			ACTIVE_ERROR("error len uobj->param_len=%u\n", uobj->param_len);
			uc_do_set_reply	((ucp_obj_t*)uobj, ERR_PARAM_INVALID);
			return;
		}
		if (acitve_data_check(param, UART_LICENCE_LEN) == false) {
			ACTIVE_ERROR("check sum failed\n");
			uc_do_set_reply	((ucp_obj_t*)uobj, ERR_PARAM_INVALID);
			return;
		}

		if (licence_active(param, UART_LICENCE_KEY_LEN) == false) {
			ACTIVE_ERROR("licence_active failed\n");
			uc_do_set_reply	((ucp_obj_t*)uobj, ERR_PARAM_INVALID);
			return;
		}

		ACTIVE_ERROR("************************active successd\n");
		iw_request_event(EVENT_SYS_REBOOT);
		uc_do_set_reply	((ucp_obj_t*)uobj, ERR_NONE);
		return;
	}

	uc_do_set_reply	((ucp_obj_t*)uobj, ERR_PARAM_INVALID);
}


