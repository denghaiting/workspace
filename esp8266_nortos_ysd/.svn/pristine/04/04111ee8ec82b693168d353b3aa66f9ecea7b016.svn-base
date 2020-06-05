/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    iw_sys_time.h
**  Author:  fangfuhan
**  Date:    04/11/2016
**
**  Purpose:
**    维护当地时间.
**************************************************************************/


#ifndef IW_SYS_TIME_H
#define IW_SYS_TIME_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "c_types.h"
#include "udp_ctrl.h"
#include "iw_api.h"
#include "iw_priv.h"

/* Macro constant definitions. */
#define TIMETONE_MAX (12)
#define TIMETONE_MIN (-12)

//同步时间类型 是有优先级的
enum {
	NONE_TIME,
	PHONE_TIME,
	NTP_TIME
};

enum {
	ACTION_NONE,
	ACTION_INSERT,
	ACTION_MODE,
	ACTION_DEL
};

/* Type definitions. */

/* External function declarations. */
/*
	功能:
		得到当地的时间错
	输入参数:
	输出参数:
	返回值:
		u_int8_t: 时间搓
	说明:
*/
u_int8_t get_local_timezone();
/*
	功能:
		设置当地的时间搓，保存到flash中
	输入参数:
		@timezone: 时间搓
	输出参数:
	返回值:
	说明:
*/
void set_local_timezone(u_int8_t timezone);

/*
	功能:
  		得到时间的同步状态 NONE_TIME,PHONE_TIME,NTP_TIME
	输入参数:
	输出参数:
	返回值:
		u_int8_t: 成功
		其他: 失败
	说明:
*/
u_int8_t get_syn_type();

/*
	功能:
		设备设备的同步时间，并且设备是从哪里同步过来的
	输入参数:
		@time: 时间
		@type: PHONE_TIME,NTP_TIME
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
u_int8_t set_sec(u_int32_t time, u_int8_t type);
/*
	功能:
		设备带时区同步时间，并且设备是从哪里同步过来的
	输入参数:
		@time: 时间
		@type: PHONE_TIME,NTP_TIME
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
rs_t set_sec_with_timetone(u_int32_t time, u_int8_t type, int timetome);

/*
	功能:
		得到UTC时间
	输入参数:
	输出参数:
	返回值:
		u_int32_t: UTC时间 单位秒
	说明:
*/

u_int32_t get_sec(void);

/*
	功能:
		本地时间结构初始化
	输入参数:
	输出参数:
	返回值:
	说明:
*/

void iw_local_time_init(void);

/*
	功能:
		内部不带时间戳计算出年月日时分秒
	输入参数:
		@time: 秒数
		@tm: 转换的年月日时分秒
	输出参数:
	返回值:
	说明:
*/
void localtimes(time_t time, struct tm *tm);

/*
	功能:
		内部带时间戳计算出年月日时分秒
	输入参数:
		@time: 秒数
		@tm: 转换的年月日时分秒
	输出参数:
	返回值:
	说明:
*/
void localtimes_withtone(time_t time, struct tm *tm);


//时区转换
/*
	功能:
		utc小时转换为当地时间小时
	输入参数:
		@hour: utc小时
		@zone: 时间戳
	输出参数:
	返回值:
		int : 当地时间 单位小时
	说明:
*/
int hour_utc_2_local(int hour,int zone);

/*
	功能:
		当地时间小时转换为utc小时
	输入参数:
		@hour: 当地时间小时
		@zone: 时间戳
	输出参数:
	返回值:
		int : utc小时 单位小时
	说明:
*/
int hour_local_2_utc(int hour,int zone);
/*
	功能:
		utc时间转换成当地时间的秒数
	输入参数:
		@hour: utc小时
		@min: utc分钟
		@sec: utc秒
	输出参数:
	返回值:
		u_int32_t :当地时间的秒数
	说明:
*/
u_int32_t get_local_time_sec(u_int8_t hour, u_int8_t min, u_int8_t sec);
/*
	功能:
		utc时间转换成utc时间的秒数
	输入参数:
		@hour: utc小时
		@min: utc分钟
		@sec: utc秒
	输出参数:
	返回值:
		u_int32_t :utc时间的秒数
	说明:
*/
u_int32_t get_utc_time_sec(u_int8_t hour, u_int8_t min, u_int8_t sec);

/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* IW_SYS_TIME_H */

