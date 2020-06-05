/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    reset_already_finish.h
**  Author:  fangfuhan
**  Date:    05/21/2016
**
**  Purpose:
**    保证恢复出厂设置彻底完成.
**************************************************************************/


#ifndef RESET_ALREADY_FINISH_H
#define RESET_ALREADY_FINISH_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "c_types.h"

/* Macro constant definitions. */


/* Type definitions. */
typedef struct{
	u_int8_t reset_finish;
	u_int8_t pad[3];
}reset_finish_t;

/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */

/*
	功能:
		设置是否需需要继续恢复出厂的标志
	输入参数:
	输出参数:0 不需要， 1 需要
	返回值:
		state: 标志
	说明:
*/
void set_reset_state(u_int8_t state);

/*
	功能:
		检查是否需需要继续恢复出厂
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void check_reset_state(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* RESET_ALREADY_FINISH_H */

