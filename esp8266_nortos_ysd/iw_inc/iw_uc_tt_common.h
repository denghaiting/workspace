/**************************************************************************
**  Copyright (c) 2010 GalaxyWind, Ltd.
**
**  Project: uc_tt_common.h
**  File:    iw_uc_tt_common.h
**  Author:  
**  Date:    12/10/2014
**
**  Purpose:
**    透明传输通用头文件.
**************************************************************************/


#ifndef UC_TT_COMMON_H
#define UC_TT_COMMON_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "iw_priv.h"
#include "iw_lib.h"
//#include "uc_dev.h"
//#include "uc_dev_event.h"
//#include "mu_type.h"
//#include "uc_push.h"
//#include "cl_sys.h"



/* Macro constant definitions. */
extern u_int8_t lc_uartraw_getchipinfo[];

/* Type definitions. */
#pragma pack(push,1)

/*不同厂家回调函数使用的参数定义*/
typedef struct uc_tt_common_app2chip_s{
	void *session;
	u_int32_t session_clid;
	bool is_from_serv;
	u_int8_t action;
	ucp_obj_t *uobj;
	u_int8_t *param;	
}uc_tt_common_app2chip_t;

typedef struct uc_tt_common_pushfun_parm_s{
	ucds_session_t *ucds_session;
	pkt_t *OUTpkt;
}uc_tt_common_pushfun_parm_t;

/*透传的数据内容格式*/
typedef struct uc_tt_uartdata_s{
	u_int16_t data_len;//串口命令原始长度，兼容其他厂家的长度，2字节表示，网络字节序
	u_int16_t pad;
//	u_int8_t data[0];//串口命令原始数据，原封不动放在这里
}uc_tt_uartdata_t;

#pragma pack(pop)

/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */
void ia_do_tt(iw_event_info_t *info, iw_obj_t *uobj);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* UC_TT_COMMON_H */

