/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    iiw_smartconfig.h
**  Author:  fangfuhan
**  Date:    04/05/2016
**
**  Purpose:
**    一键配置（扫描 手机热点  sniffer  product）..
**************************************************************************/


#ifndef IIW_SMARTCONFIG_H
#define IIW_SMARTCONFIG_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "c_types.h"
#include "iw_api.h"

/* Macro constant definitions. */

/* Type definitions. */

/* External function declarations. */

/* Macro API definitions. */


/* Global variable declarations. */
/*
	功能:
		判断是不是在产测模式
	输入参数:
	输出参数:
	返回值:
		1: 是
		0: 不是
	说明:
*/
u_int8_t is_product_mode_f();
/*
	功能:
		判断是不是在手机热点模式
	输入参数:
	输出参数:
	返回值:
		1: 是
		0: 不是
	说明:
*/
u_int8_t is_phonehotpoint_mode_f();

/*
	功能:
		启动一键配置
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void  iw_smartcfg_start(void);
/*
	功能:
		判断判断是不是已经锁定信道
	输入参数:
	输出参数:
	返回值:
		1: 是
		0: 不是
	说明:
*/
bool is_lock_ap(void);
/*
	功能:
		wifi配置保存到flash中
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void start_wifi_config_save(void);
/*
	功能:
		得到一键配置所使用的时间
	输入参数:
	输出参数:
	返回值:
		uint32_t: 一键配置使用的时间
	说明:
		返回值为0，设备上电之后，没有进行一键配置
		返回值为1-120，设备刚刚进行了一键配置
		返回大于120，设备已经配置设备超过120s
*/
uint32_t get_smcf_time(void);
/*
	功能:
		退出一键配置
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void exit_smartconfig();
/*
	功能:
		判断设备是否已经连接上路由器
	输入参数:
	输出参数:
	返回值:
		true: 是
		其他: 不是	
	说明:
*/
bool is_wifi_up(void); 
/*
	功能:
		开始连接wifi
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void start_wifi(void);


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* IIW_SMARTCONFIG_H */

