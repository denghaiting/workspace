/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: push info
**  File:    uc_push.h
**  Author:  liubenlong
**  Date:    09/30/2014
**
**  Purpose:
**    Push.
**************************************************************************/


#ifndef UC_PUSH_H
#define UC_PUSH_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */


/* Type definitions. */
typedef pkt_t *(*push_proc)(ucds_session_t *s);
typedef pkt_t *(*push2serv_proc)(void);


/* External function declarations. */

/* Macro API definitions. */

/* Global variable declarations. */
/*
	功能:
		注册请求push是的回调，并启动push请求检查
	输入参数:
		@func: 回调
	输出参数:
	返回值:
	说明:
*/
void uc_push_init(push_proc func);
/*
	功能:
		请求push
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void uc_push_set(void);
void uc_push_clear();
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* UC_PUSH_H */

