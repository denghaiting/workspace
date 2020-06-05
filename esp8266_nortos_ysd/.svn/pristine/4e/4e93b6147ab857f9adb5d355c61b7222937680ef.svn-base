/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: mu
**  File:    licence.h
**  Author:  cailiyang
**  Date:    08/07/2014
**
**  Purpose:
**    Licence.
**************************************************************************/


#ifndef LICENCE_H
#define LICENCE_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "c_types.h"
#include "iw_api.h"

/* Macro constant definitions. */
#define LCC_MAC_LEN			(6)
#define LCC_SN_LEN			(6)
#define LCC_VENDOR_LEN		(16)
#define LCC_PWD_LEN			(8)
#define LCC_PRESHARE_KEY_LEN (8)

#define LCC_MAGIC	0x38303000 //808

#pragma pack(push,8)

/* Type definitions. */
/* 证书数据结构 */
typedef struct{
	u_int8_t ver;
	u_int8_t len;
	u_int8_t dev_type;
	u_int8_t ext_type;
	u_int8_t mac[LCC_MAC_LEN];
	u_int8_t sn[LCC_SN_LEN];
	u_int8_t vendor[LCC_VENDOR_LEN];
	u_int8_t df_pwd[LCC_PWD_LEN];
	u_int8_t preshare_key[LCC_PRESHARE_KEY_LEN];
}lcc_key_t;

/* 保存在flash中证书 */
typedef struct{
	u_int32_t magic;	/* 表示这4K是证书的标识*/
	u_int32_t valid;	/* 证书是否有效 */
	lcc_key_t key;		/* 证书内容*/
}lcc_nvm_key_t;

/* 设备信息*/
typedef struct devlic_s{
	u_int64_t sn;
	u_int8_t dev_type;
	u_int8_t ext_type;
	u_int8_t mac[LCC_MAC_LEN];
	u_int8_t vendor[LCC_VENDOR_LEN];
	u_int8_t df_pwd[LCC_PWD_LEN];
	u_int8_t preshare_key[LCC_PRESHARE_KEY_LEN];
	u_int8_t active;
}devlic_t;

#pragma pack(pop)


/* External function declarations. */

/* Macro API definitions. */


/* Global variable declarations. */

extern devlic_t g_dev;

/*
	功能:
		得到设备的激活状态
	输入参数:
	输出参数:
	返回值:
		TRUE: 已激活
		其他: 没有激活
	说明:
*/
bool dev_is_active(void);

/*
	功能:
		保存证书到flash中
	输入参数:
		@nvm_key: 证书数据
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS lcc_write(lcc_nvm_key_t * nvm_key);

/*
	功能:
		从flash中读取证书，保存数据到g_dev
	输入参数:
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS lcc_read(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* LICENCE_H */

