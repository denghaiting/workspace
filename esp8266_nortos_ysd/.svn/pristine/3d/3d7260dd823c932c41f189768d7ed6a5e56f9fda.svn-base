/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    iw_uptime.c
**  Author:  fangfuhan
**  Date:    04/11/2016
**
**  Purpose:
**    系统运行时间.
**************************************************************************/


/* Include files. */

#include "iw_priv.h"


/* Macro constant definitions. */
#define IW_TIME_AJUST_INTERVAL (1 * 1000 * 1000)

/* Type definitions. */
typedef struct{
	u_int64_t up_rtc; // 启动后rtc时钟周期

	u_int64_t up_msec; // 启动后毫秒
	u_int32_t up_sec; // 启动后秒

	u_int32_t prev_rtc; //上次rtc时钟
	u_int32_t rtc_us; //rtc周期，单位微妙
}uptime_t;


/* Local function declarations. */


/* Macro API definitions. */
//#define RUNTIME_DEBUG_TEST
#ifdef RUNTIME_DEBUG_TEST
#define RUNTIME_DEBUG(fmt...)   DEBUG(fmt)
#define RUNTIME_INFO(fmt...)	   INFO(fmt)
#define RUNTIME_ERROR(fmt...)   ERROR(fmt)
#else
#define RUNTIME_DEBUG(fmt...)  
#define RUNTIME_INFO(fmt...)	  
#define RUNTIME_ERROR(fmt...)   ERROR(fmt)   
#endif


/* Global variable declarations. */
uptime_t g_uptime;
LOCAL ETSTimer g_uptime_timer;

#define LEAPS_THRU_END_OF(y) ((y)/4 - (y)/100 + (y)/400)

static const unsigned char rtc_days_in_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

bool ICACHE_FLASH_ATTR
u32_is_bigger(u_int32_t a, u_int32_t b)
{
	u_int32_t sub = a - b;

	return (a != b && sub < 0x7FFFFFFF);
}

void ICACHE_FLASH_ATTR
upate_uptime()
{
	u_int32_t cur = system_get_time();

	if (cur < g_uptime.prev_rtc) {
		if (g_uptime.prev_rtc - cur <= IW_TIME_AJUST_INTERVAL) {//1s
			return;
		}
	}

	if(u32_is_bigger(cur, g_uptime.prev_rtc)) {
		g_uptime.up_rtc += (cur - g_uptime.prev_rtc);
	} else {
		g_uptime.up_rtc += (0xFFFFFFFF - g_uptime.prev_rtc + cur + 1);
	}
	g_uptime.up_msec = g_uptime.up_rtc / 1000;
	g_uptime.up_sec = g_uptime.up_msec / 1000;

	g_uptime.prev_rtc = cur;
}

u_int32_t ICACHE_FLASH_ATTR
get_up_sec()
{
	upate_uptime();
	return g_uptime.up_sec;
}

u_int64_t ICACHE_FLASH_ATTR
get_up_msec()
{
	upate_uptime();
	return g_uptime.up_msec;
}

void ICACHE_FLASH_ATTR
iw_uptime_init()
{
	RUNTIME_ERROR("iw_uptime_init\r\n");
	os_memset(&g_uptime, 0, sizeof(g_uptime));

	g_uptime.prev_rtc = system_get_rtc_time();
	g_uptime.rtc_us = system_rtc_clock_cali_proc();
	g_uptime.rtc_us = g_uptime.rtc_us >> 12;
	if(g_uptime.rtc_us == 0)
		g_uptime.rtc_us = 5;


	os_timer_disarm(&g_uptime_timer);
	os_timer_setfn(&g_uptime_timer, upate_uptime, NULL);
	os_timer_arm(&g_uptime_timer, 1000 * 60 * 1, true);
	
	piw_priv->iw_sys.up_time = get_up_sec();
	return;
}

