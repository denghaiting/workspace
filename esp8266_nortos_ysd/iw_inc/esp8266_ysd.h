/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: lede宏控制文件
**  File:    esp8266_lede.h
**  Author:  zhaogang
**  Date:    06/02/2015
**
**  Purpose:
**  lede宏控制文件
**************************************************************************/


#ifndef ESP8266_LEDE_H
#define ESP8266_LEDE_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */
//设备名称
#define DEV_HOST_NAME			""

//支持LEDE LED
#define HAS_LEDE_LED
#define HAS_TIMER_EXT
#define TIMER_EXT_DATA_LEN	8

#define HAS_STM_UPGRADE
//定时器功能
//#define HAS_TIMER


//支持多镜像选择
//#define HAS_BOOTLOAD

//硬件支持版本信息
//#define HAS_HARD_VERSION

// 支持AP监控
#define HAS_UAM

//使用串口产测命令
//#define HAS_UART_PT


/* Type definitions. */


/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif
