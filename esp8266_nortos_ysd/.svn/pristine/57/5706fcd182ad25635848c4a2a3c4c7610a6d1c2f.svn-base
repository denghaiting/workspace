/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: esp8266 plat
**  File:    iw_ntp_time_syn.c
**  Author:  fangfuhan
**  Date:    03/23/2016
**
**  Purpose:
**    Ntp时间同步.
**************************************************************************/


/* Include files. */
#include "iw_ntp_time_syn.h"
#include "iw_sys_time.h"
#include "c_types.h"
#include "udp_ctrl.h"
#include "iw_uptime.h"
#include "iw_debug.h"
#include "stlc_list.h"
#include "iw_mem.h"

/* Macro constant definitions. */
#define SYN_TIME_BUCKET 16

/* Type definitions. */
typedef struct{
	struct stlc_list_head syn_head;	
}syn_time_list_t;

typedef struct{
	u_int8_t *name;
	syn_time_ok_callback_t cb;
	struct stlc_list_head link;
}syn_time_node_t;

/* Local function declarations. */


/* Macro API definitions. */
//#define NTP_DEBUG_TEST
#ifdef NTP_DEBUG_TEST
#define NTP_DEBUG(fmt...)   DEBUG(fmt)
#define NTP_INFO(fmt...)	   INFO(fmt)
#define NTP_ERROR(fmt...)   ERROR(fmt)
#else
#define NTP_DEBUG(fmt...)  
#define NTP_INFO(fmt...)	INFO(fmt)   
#define NTP_ERROR(fmt...)   ERROR(fmt)   
#endif

/* Global variable declarations. */
static syn_time_list_t syn_time_list;
syn_time_list_t *syn_time_list_p = &syn_time_list;
static bool syn_time_init = false;



void ICACHE_FLASH_ATTR syn_time_list_init()
{
	STLC_INIT_LIST_HEAD(&syn_time_list_p->syn_head);
}

rs_t ICACHE_FLASH_ATTR syn_time_event_call()
{
	syn_time_node_t *node = NULL;
	syn_time_node_t *next = NULL;
	u_int8_t type = get_syn_type();

	stlc_list_for_each_entry_safe(syn_time_node_t, node, next, &syn_time_list_p->syn_head, link){
		if(NULL != node->cb){
			NTP_INFO("syn time ok, %s is called!\r\n", node->name);
			node->cb(type);
		}	
	}
	return RS_OK;
}

rs_t ICACHE_FLASH_ATTR syn_time_event_register(syn_time_ok_callback_t callback, u_int8_t *name)
{
	syn_time_node_t *node;
	
	node = (syn_time_node_t *)iw_malloc(sizeof(*node));
	if(node == NULL)
		return RS_MEMORY_MALLOC_FAIL;
	node->name = name;
	node->cb = callback;
	stlc_list_add_tail(&node->link, &syn_time_list_p->syn_head);
	return RS_OK;
}

/*
*	功能:判断时间是否同步完成
*	返回值:true 同步完成
*		 false 没有同步完成
*/
bool ICACHE_FLASH_ATTR time_syn_ok(void)
{
	return (get_syn_type() != NONE_TIME);
}


static unsigned long ICACHE_FLASH_ATTR mktime2(const unsigned int year0, const unsigned int mon0,
       const unsigned int day, const unsigned int hour,
       const unsigned int min, const unsigned int sec)
{
	unsigned int mon = mon0, year = year0;

	/* 1..12 -> 11,12,1..10 */
	if (0 >= (int) (mon -= 2)) {
		mon += 12;	/* Puts Feb last since it has leap day */
		year -= 1;
	}

	return ((((unsigned long)
		  (year/4 - year/100 + year/400 + 367*mon/12 + day) +
		  year*365 - 719499
	    )*24 + hour /* now have hours */
	  )*60 + min /* now have minutes */
	)*60 + sec; /* finally seconds */
}

/* 
	t->year 必须是网络字节
*/
RS ICACHE_FLASH_ATTR ntp_from_ice(uc_time_t *t)
{
	struct tm tm1;
	u_int32_t time = 0;

	if(NULL == t){
		return RS_INVALID_PARAM;
	}

	t->year = ntohs(t->year);
	//换算成秒
	time = (u_int32_t)mktime2(t->year, t->month, t->day, t->hour, 
								t->minute, t->second);
	set_sec_with_timetone(time, NTP_TIME, t->timezone);
	
	NTP_INFO("time=%u\r\n", time);
	localtimes_withtone(time, &tm1);
	NTP_INFO("ice tm.tm_year=%d, tm_mon=%d, tm_mday=%d, tm_hour=%d, tm_min=%d, tm_sec=%d,tm_wday=%d, tm_yday=%d, tm_isdst=%d, zone=%d\r\n",
		tm1.tm_year, tm1.tm_mon, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec,\
		tm1.tm_wday, tm1.tm_yday, tm1.tm_isdst, t->timezone);

	syn_time_event_call();
	return RS_OK;
}

/* 
	t->year 必须是网络字节
*/
RS ICACHE_FLASH_ATTR ntp_from_phone(uc_time_t *t)
{
	u_int32_t time = 0;
	struct tm tm1;
	RS ret;
		
	if(NULL == t){
		return RS_INVALID_PARAM;
	}

	t->year = ntohs(t->year);

	//只设置一次
	set_local_timezone(t->timezone);

	//换算成秒
	time = (u_int32_t)mktime2(t->year, t->month, t->day, t->hour, 
								t->minute, t->second);
	ret = set_sec_with_timetone(time, PHONE_TIME, t->timezone);
	if(ret == RS_OK){
		syn_time_event_call();		
	}
	
	NTP_INFO("time=%u\r\n", time);
	localtimes_withtone(time, &tm1);	
	NTP_INFO("phone tm.tm_year=%d, tm_mon=%d, tm_mday=%d, tm_hour=%d, tm_min=%d, tm_sec=%d,tm_wday=%d, tm_yday=%d, tm_isdst=%d, zone=%d\r\n",
		tm1.tm_year, tm1.tm_mon, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec,\
		tm1.tm_wday, tm1.tm_yday, tm1.tm_isdst, t->timezone);
		
	return RS_OK;
}

bool iw_ntp_get_init_state()
{
	return syn_time_init;
}

void ICACHE_FLASH_ATTR iw_ntp_time_syn_init()
{
	NTP_INFO("iw_ntp_time_syn_init\r\n");
	syn_time_list_init();

	syn_time_init = true;
}

