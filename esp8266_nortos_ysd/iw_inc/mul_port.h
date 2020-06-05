/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: mul_port.h
**  File:    mul_port.h
**  Author:  xianxiaojun
**  Date:    09/21/2015
**
**  Purpose:
**    服务器多端口选择.
**************************************************************************/


#ifndef	__MUL_PORT_H__
#define	__MUL_PORT_H__



#include "iw_dev.h"


/* 会话获取设备连接服务器的端口 */
extern void get_serverport(ucdc_session_t *s);
/* 连接设备成功，保存端口 */
extern void save_serverport(ucdc_session_t *s);



#endif

