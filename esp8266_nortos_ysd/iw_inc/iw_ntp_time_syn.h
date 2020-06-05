/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: esp8266.h
**  File:    iw_ntp_time_syn.h
**  Author:  fangfuhan
**  Date:    03/23/2016
**
**  Purpose:
**    Ntp时间同步.
**************************************************************************/


#ifndef IW_NTP_TIME_SYN_H
#define IW_NTP_TIME_SYN_H	


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "iw_api.h"
#include "udp_ctrl.h"
/* Macro constant definitions. */


/* Type definitions. */
typedef rs_t (* syn_time_ok_callback_t)(u_int8_t type);

bool iw_ntp_get_init_state();
/* External function declarations. */
/*
	功能:
		时间同步初始化
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void iw_ntp_time_syn_init();
/*
	功能:
		从手机端同步时间， 服务器端同步了，手机就不在同步了
	输入参数:
		@t: 时间信息
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
		t->year 必须是网络字节
*/
RS ntp_from_phone(uc_time_t *t);
/*
	功能:
		从服务器端同步时间， 服务器端同步了，手机就不在同步了
	输入参数:
	输入参数:
		@t: 时间信息
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
		t->year 必须是网络字节
*/
RS ntp_from_ice(uc_time_t *t);
/*
*	功能:判断时间是否同步完成
*	返回值:true 同步完成
*		 false 没有同步完成
*/
bool time_syn_ok(void);
/*
	功能:
		时间同步完成时需要回调的函数注册
	输入参数:
		@callback: 回调
		@name: 名字
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
rs_t syn_time_event_register(syn_time_ok_callback_t callback, u_int8_t *name);
/* Macro API definitions. */


/* Global variable declarations. */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* IW_NTP_TIME_SYN_H */

