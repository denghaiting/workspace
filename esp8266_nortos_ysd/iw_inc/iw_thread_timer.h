/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    iw_thread_timer.h
**  Author:  fangfuhan
**  Date:    04/11/2016
**
**  Purpose:
**    定时器伪线程.
**************************************************************************/
#ifndef IW_THREAD_TIMER_H
#define IW_THREAD_TIMER_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "iw_api.h"
#include "c_types.h"

/* Macro constant definitions. */


/* Type definitions. */
typedef struct{
	struct stlc_hlist_node link;
	u_int32_t ms;
	void *cp;
	iw_timer_callback_t cb;
	u_int16_t tid;
	u_int8_t periodical;
	u_int8_t pad;
#if (IW_PLATFORM==PLT_8266)
	os_timer_t timer;
#endif
}iw_timer_t;

enum{
	TID_UCDC_IDLE  = 1,
	TID_UCDC_DIE  = 2,
	TID_UCDC_KEEPALIVE  = 3,
	TID_UCDC_SEND  = 4,
	TID_WIFI_SCAN  = 5,
	TID_WIFI_SMCF  = 6,
	TID_UCDC_AP_MONITOR1 = 7,
	TID_UCDC_AP_MONITOR2 = 8,
	TID_UCDC_DELAY_RST_WIFI = 9,
	TID_UCDC_CHECK_STATE = 10,
	TID_SYS_EVENT_TIMER = 11,
	TID_WIFI_SCAN_TIMEOUT = 12,
	TID_UCDC_PUSH = 13,
	TID_UART_ACTIVE = 14,
	TID_UART_PARSE = 15,
	TID_PERIOD_TIMER = 16,
	TID_UPGRADE_TIMER = 17,
    TID_CRC_TIMER = 18,
    TID_CRC_RESULT_TIMER = 19,
    TID_SMCF_CHECK_WIFI_UP = 20,
    TID_PHP_SEND_OK = 21,		
    TID_PHP_ACK_OK = 22,
    TID_PRO_REV_SEND_TIME = 23,
    TID_DNS_TIMER  = 24,
    TID_TRAP_RESET = 25,
    TID_CHANGE_CHN_TIMER = 26,
    TID_CHECK_SMARTCONFIG = 27,
    TID_DEBUG_THREAD = 28,
    TID_DN_TIME_EVENT = 29,
    TID_DN_SIGN_RESO_TIME = 30,
    TID_DN_TIMER = 31, 
    TID_UART_CMD_PARAM = 32,
    TID_UART_SEND = 33,
    TID_UART_DATE_LOOK_UP = 34,
    TID_UER_QUE = 35,
    TID_WIFI_PARAM_CHECK = 36,
    TID_STM_UPGRADE_TIMER = 37,
    TID_STM_TIMER = 38,
	TID_UCDC_PHONE0 = 100,          /* 100~199 is used by phone0 */
	TID_UCDC_PHONE1 = 200,
	TID_UCDC_PHONE2 = 300,
	TID_UCDC_PHONE3 = 400,
	TID_UCDC_PHONE4 = 500,
	TID_UCDC_PHONE5 = 600,
	TID_UCDC_PHONE6 = 700,
	TID_UCDC_PHONE7 = 800,
	TID_UCDC_PHONE8 = 900,
	TID_UCDC_PHONE9 = 1000,
};

enum{
	TID_UCDS_IDLE  = 1,
	TID_UCDS_DIE  = 2,
	TID_UCDS_KEEPALIVE  = 3,
	TID_UCDS_SEND  = 4,
};

#define TID_UCDS_DIE(index) ((index)*100 + TID_UCDC_PHONE0 + TID_UCDS_DIE)
#define TID_UCDS_SEND(index) ((index)*100 + TID_UCDC_PHONE0 + TID_UCDS_SEND)


/* External function declarations. */
/*
	功能:
		设置定时器
	输入参数:
		@id: 定时器唯一标识
		@ms: 多少秒后触发该定时器。触发后会调用相应的回调函数
		@periodical: 是否是周期性定时器
		@cb: call back routine
		@cp: call back param
	输出参数:
		无
	返回值:
		RS_OK: 设置定时器成功
		其他: 错误
	说明:
		同一个定时器可以设置多次，以最后一次为准。
		周期性定时器，在触发后，会自动继续计时等待下次触发
		一次性定时器，在触发后，不会再触发
*/
extern rs_t iw_timer_set(u_int16_t id, u_int32_t ms, bool periodical, iw_timer_callback_t cb, void *cp);

/*
	功能:
		删除定时器
	输入参数:
		@id: 定时器唯一标识
	返回值:
		RS_OK: 设置定时器成功
		其他: 错误
	说明:
*/
extern rs_t iw_timer_delete(u_int16_t id);

/*
	功能:
		初始化定时器链表
	输入参数:
		无
	返回值:
		无
	说明:
*/
void iw_thread_timer_init();

/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* IW_THREAD_TIMER_H */

