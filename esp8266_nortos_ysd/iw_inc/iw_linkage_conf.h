/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 联动模块
**  File:    iw_linkage_conf.h
**  Author:  cailiyang
**  Date:    12/24/2015
**
**  Purpose:
**    重启配置保存.
**************************************************************************/


#ifndef LINKAGE_CONF_H
#define LINKAGE_CONF_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "iw_api.h"
#include "iw_flash.h"


/* Macro constant definitions. */
//域名最大长度
#define MAX_DOMAIN_LENGTH	64
#define UCT_DEV_NEW_LA_SUPPORT 23	// 设备支持联动标志，发送的homeid上来

/* Type definitions. */
/* Wi-Fi设备在认证时告知服务支持联动，并带上home_id和reset_count */
typedef struct {
   u_int32_t home_id; 
   u_int16_t reset_count;
   u_int8_t pad[2];
}linkage_info_t;


/* External function declarations. */
/*
	功能:
		恢复出厂设置初始化
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void lc_init(void);
/*
	功能:
		得到恢复出厂设置次数
	输入参数:
	输出参数:
	返回值:
		u_int16_t: 恢复出厂设置次数
	说明:
*/
u_int16_t lc_get_reset_count(void);
/*
	功能:
		恢复出厂设置次数自加一
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void lc_reset_count_increased(void);
/*
	功能:
		得到服务器域名
	输入参数:
	输出参数:
	返回值:
		config_t *: 服务器域名数据
		NULL: 失败
	说明:
*/
config_t *lc_get_linkage_domain(void);
/*
	功能:
		保存域名，重新连接服务器
	输入参数:
		@domain: 域名
		@len: 长度
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS lc_update_linkage_server(u_int8_t *domain, u_int8_t len);

/*
	得到支持联动的home_id
*/
u_int32_t get_home_id(void);

/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* LINKAGE_CONF_H */

