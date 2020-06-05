/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: mu
**  File:    uuid.h
**  Author:  cailiyang
**  Date:    08/08/2014
**
**  Purpose:
**    Uuid.
**************************************************************************/


#ifndef UUID_H
#define UUID_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "types_def.h"


/* Macro constant definitions. */
#define UUID_LEN	16

/* Type definitions. */
#pragma pack(push, 8)

/* 保存在flash中uuid数据结构*/
typedef struct{
	u_int32_t magic;
	u_int32_t valid;
	u_int32_t uuid[UUID_LEN];
}nvm_uuid_t;

#pragma pack(pop)

/* External function declarations. */
/*
	功能:
		写UUID到flash
	输入参数:
		@uuid: 数据
		@uuid_len: 数据长度
	输出参数:
		@info: 保存系统信息
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS uuid_write(u_int8_t *uuid, u_int32_t uuid_len);

/* External function declarations. */
/*
	功能:
		读UUID从flash，数据放在g_uuid全局变量中
	输入参数:
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS uuid_read(void);

/* Macro API definitions. */


/* Global variable declarations. */
extern nvm_uuid_t g_uuid;


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* UUID_H */

