/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    iw_sys_time.c
**  Author:  fangfuhan
**  Date:    04/11/2016
**
**  Purpose:
**    维护当地时间.
**************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "iw_dev_timer.h"
#include "iw_sys_time.h"


/* Macro constant definitions. */
#define TIMEZONE_MIN		(-12)
#define TIMEZONE_MAX		(12)

/* Type definitions. */
typedef struct local_time_ctrl_s {
	u_int8_t type;
	u_int32_t time_diff; //同步时系统运行时间的秒数
	bool timezone_setted;
	u_int8_t timezone;
	u_int32_t time;
}local_time_ctrl_t;

/* Local function declarations. */


/* Macro API definitions. */
#define LEAPS_THRU_END_OF(y) ((y)/4 - (y)/100 + (y)/400)

//#define LOCALTIME_DEBUG_TEST
#ifdef LOCALTIME_DEBUG_TEST
#define LOCALTIME_DEBUG(fmt...)   DEBUG(fmt)
#define LOCALTIME_INFO(fmt...)	   INFO(fmt)
#define LOCALTIME_ERROR(fmt...)   ERROR(fmt)
#else
#define LOCALTIME_DEBUG(fmt...)  
#define LOCALTIME_INFO(fmt...)	  
#define LOCALTIME_ERROR(fmt...)   ERROR(fmt)   
#endif

/* Global variable declarations. */
static local_time_ctrl_t local_time;
static const unsigned char rtc_days_in_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

u_int8_t ICACHE_FLASH_ATTR get_local_timezone();

//时区转换
int ICACHE_FLASH_ATTR hour_utc_2_local(int hour,int zone)
{
	hour = hour + 24 + zone;
	hour = hour%24;

	return hour;
}

int ICACHE_FLASH_ATTR hour_local_2_utc(int hour,int zone)
{
	hour = hour + 24 - zone;
	hour = hour%24;

	return hour;
}

u_int32_t ICACHE_FLASH_ATTR get_local_time_sec(u_int8_t hour, u_int8_t min, u_int8_t sec)
{
	return (hour_utc_2_local(hour, get_local_timezone())*ONE_HOUR_SEC + min*ONE_MINI_SEC + sec);
}

u_int32_t ICACHE_FLASH_ATTR get_utc_time_sec(u_int8_t hour, u_int8_t min, u_int8_t sec)
{
	return (hour*ONE_HOUR_SEC + min*ONE_MINI_SEC + sec);
}


static inline bool ICACHE_FLASH_ATTR is_leap_year(unsigned int year)
{
	return (!(year % 4) && (year % 100)) || !(year % 400);
}

static int ICACHE_FLASH_ATTR rtc_month_days(unsigned int month, unsigned int year)
{
	return rtc_days_in_month[month] + (is_leap_year(year) && month == 1);
}


void ICACHE_FLASH_ATTR localtimes(
    time_t time,
    struct tm *tm)
{
	unsigned int month, year;
	unsigned long secs;
	int days;

	/* time must be positive */
	days = time / 86400;
	secs = time - (unsigned int) days * 86400;

	/* day of the week, 1970-01-01 was a Thursday */
	tm->tm_wday = (days + 4) % 7;

	year = 1970 + days / 365;
	days -= (year - 1970) * 365
	        + LEAPS_THRU_END_OF(year - 1)
	        - LEAPS_THRU_END_OF(1970 - 1);
	if (days < 0) {
		year -= 1;
		days += 365 + is_leap_year(year);
	}
	//tm->tm_year = year - 1900;

	tm->tm_year = year;
	tm->tm_yday = days + 1;

	for (month = 0; month < 11; month++) {
		int newdays;

		newdays = days - rtc_month_days(month, year);
		if (newdays < 0)
			break;
		days = newdays;
	}
	tm->tm_mon = month + 1;
	tm->tm_mday = days + 1;

	tm->tm_hour = secs / 3600;
	secs -= tm->tm_hour * 3600;
	tm->tm_min = secs / 60;
	tm->tm_sec = secs - tm->tm_min * 60;

	tm->tm_isdst = 0;

}

void ICACHE_FLASH_ATTR localtimes_withtone(
		   time_t time,   
		   struct tm *tm)
{
	unsigned int month, year;
	unsigned long secs;
	int days;

	if(local_time.timezone_setted){
		time += local_time.timezone * ONE_HOUR_SEC;
	}
	
	/* time must be positive */
	days = time/86400;
	secs = time - (unsigned int) days * 86400;

	/* day of the week, 1970-01-01 was a Thursday */
	tm->tm_wday = (days + 4) % 7;

	year = 1970 + days / 365;
	days -= (year - 1970) * 365
		+ LEAPS_THRU_END_OF(year - 1)
		- LEAPS_THRU_END_OF(1970 - 1);
	if (days < 0) {
		year -= 1;
		days += 365 + is_leap_year(year);
	}
	//tm->tm_year = year - 1900;

	tm->tm_year = year;
	tm->tm_yday = days + 1;

	for (month = 0; month < 11; month++) {
		int newdays;

		newdays = days - rtc_month_days(month, year);
		if (newdays < 0)
			break;
		days = newdays;
	}
	tm->tm_mon = month+1;
	tm->tm_mday = days + 1;

	tm->tm_hour = secs / 3600;
	secs -= tm->tm_hour * 3600;
	tm->tm_min = secs / 60;
	tm->tm_sec = secs - tm->tm_min * 60;

	tm->tm_isdst = 0;

}

u_int8_t ICACHE_FLASH_ATTR get_local_timezone()
{
	return local_time.timezone;
}

void ICACHE_FLASH_ATTR set_local_timezone(u_int8_t timezone)
{
	//参数合法性检查
	if (timezone < TIMEZONE_MIN || timezone > TIMEZONE_MAX)
		return;

	//只设置一次
	if (local_time.timezone_setted)
		return;
	
	LOCALTIME_INFO("timezone=%d\n", timezone);
	local_time.timezone = (int)timezone;
	local_time.timezone_setted = true;
	
	conf_user_set(USER_TIMEZONE, (u_int8_t *)&timezone, sizeof(timezone));
}

u_int8_t ICACHE_FLASH_ATTR get_syn_type()
{
	return local_time.type;
}

/*
*	1.ntp同步之后，ntp还可以重新同不
*	2.时间类型优先级 NTP_TIME > PHONE_TIME > NONE_TIME
*/
static bool ICACHE_FLASH_ATTR need_syn_time(u_int8_t type_old, u_int8_t type_new)
{
	if(type_old == NTP_TIME && type_new == NTP_TIME)
		return true;
	
	return   (type_new > type_old) ;
}

u_int8_t ICACHE_FLASH_ATTR set_sec(u_int32_t time, u_int8_t type)
{
	if(!need_syn_time(local_time.type, type)){
		return RS_NTP_NONE;
	}

	local_time.type = type;
	local_time.time_diff = get_up_sec();
	local_time.time = time;
	
	LOCALTIME_INFO("time2 = %u, type = %d\r\n", time, type);
	return RS_OK;
}

/*
*	带时区调用此接口设置时间
*/
rs_t ICACHE_FLASH_ATTR set_sec_with_timetone(u_int32_t time, u_int8_t type, int timetome)
{
	u_int32_t time_temp;
	
	if(timetome > TIMETONE_MAX && timetome < TIMETONE_MAX){
		return RS_INVALID_PARAM;
	}

	LOCALTIME_INFO("time1 = %u, type = %d, time_tone = %d\r\n", time, type, timetome);
	time_temp = time - ONE_HOUR_SEC * timetome;
	
	return set_sec(time_temp, type);
}

/*
* 	功能:得到utc时间
* 	返回值:utc时间 单位秒
*	不能通过统一的debug打印，会陷入是循环
*/
u_int32_t ICACHE_FLASH_ATTR get_sec(void)
{
	if (local_time.type != NONE_TIME) {
		return local_time.time +  (get_up_sec() - local_time.time_diff);
	}

	return system_get_time()/1000/1000;
}

/**************************************************************************
**  Function Name: get_local_sec
**  Purpose:
**    得到本地时间的秒数.
**  Parameters:
**    None.
**  Return:
**    本地时间秒
**  Author:  fangfuhan
**  Date:    03/25/2016
**  Notes:
**    Utc+timetone*3600.
**************************************************************************/
u_int32_t ICACHE_FLASH_ATTR get_local_sec()
{
	return (get_sec() + (local_time.timezone * ONE_HOUR_SEC));
} /* End of get_local_sec() */


void ICACHE_FLASH_ATTR iw_local_time_init(void)
{	
	config_t *pconfig = NULL;
	
	memset(&local_time, 0 , sizeof(local_time));	
	
	LOCALTIME_INFO("iw_local_time_init\r\n");
	pconfig = conf_user_get(USER_TIMEZONE);
	if (NULL != pconfig && (pconfig->len >= sizeof(local_time.timezone))) {
		local_time.timezone = *(pconfig->value);
		local_time.timezone_setted = true;
	}
}

