/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: FOTA
**  File:    upgrade.h
**  Author:  xushengjian
**  Date:    07/20/2015
**
**  Purpose:
**    Firmware upgrade over the air.
**************************************************************************/


#ifndef UPGRADE_H
#define UPGRADE_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */


/* Type definitions. */


/* External function declarations. */
/*
	功能:
		镜像网络升级借口
	输入参数:
		@info: 存放信息结构
		@uobj: 三级属性， 数据长度
	输出参数:
	返回值:
	说明:
*/
void do_ucot_system_upgrade_image(iw_event_info_t *info, iw_obj_t *uobj);
/*
	功能:
		系统升级模块初始化
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void sys_upgrade_init(void);


/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* UPGRADE_H */

