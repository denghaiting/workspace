/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 定时器功能
**  File:    uc_dev_timer.c
**  Author:  liubenlong
**  Date:    07/29/2014
**
**  Purpose:
**    定时器功能.
**************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "iw_api.h"
#include "stlc_list.h"
#include "time.h"
#include "iw_lan_srv.h"
#include "iw_dev_timer.h"
#include "iw_user_config.h"
#include "iw_sys_time.h"
#include "iw_ntp_time_syn.h"

/* Macro constant definitions. */

/* Type definitions. */

/* Local function declarations. */


/* Macro API definitions. */
#define DEVTIMER_DEBUG_TEST
#ifdef DEVTIMER_DEBUG_TEST
#define DEVTIMER_DEBUG(fmt...)   DEBUG(fmt)
#define DEVTIMER_INFO(fmt...)	 INFO(fmt)
#define DEVTIMER_ERROR(fmt...)   ERROR(fmt)
#else
#define DEVTIMER_DEBUG(fmt...)  
#define DEVTIMER_INFO(fmt...)	
#define DEVTIMER_ERROR(fmt...)    ERROR(fmt)
#endif

#ifndef HAS_TIMER_EXT
#define HAS_TIMER_EXT
#endif

/* Global variable declarations. */
#define TIMEZONE_MIN		(-12)
#define TIMEZONE_MAX		(12)


//定时器链表
#define ID_TIMER_MAX	21

//本地时间控制
static uc_timer_func_t m_start_proc, m_finish_proc;

extern RS ucds_request_add(ucds_session_t *s, pkt_t *pkt);
static bool try_timer_push(void);

static int ICACHE_FLASH_ATTR ucd_timer_get_id()
{
	int i;
	u_int8_t array[ID_TIMER_MAX];	
	ucd_period_timer_node_t *node;

	//先定５０个吧
	memset(array, 0, sizeof(array));

	stlc_list_for_each_entry(ucd_period_timer_node_t, node, &PERIOD_TIMER, link) {
		if (node->timer.id < sizeof(array)) {
			array[node->timer.id] = 1;
		}
	}

	for(i = 1; i< sizeof(array); i++) {
		if (array[i] == 0) {
			return i;
		}
	}

	return -1;
}

static u_int32_t ICACHE_FLASH_ATTR uc_time_get_next_time_once(struct tm * pone_tm, ucd_period_timer_node_t *node)
{
	int time1 = 0;
	int time2 = 0;	
	int ret_time = 0;

	time1 = get_utc_time_sec(pone_tm->tm_hour, pone_tm->tm_min, pone_tm->tm_sec);
	time2 = get_utc_time_sec(node->timer.hours, node->timer.minute, 0);

	DEVTIMER_INFO( " onece uc_time_get_next_time_once time1=%d time2 = %d\n", time1, time2);
	//这里修改一下，如果当前时间比设置的时间晚，则定时到下一天去
	ret_time = time2 - time1;

	if (ret_time < 0) {
		ret_time += ONE_DAY_SEC;
	}

	return ret_time;
}

static u_int32_t ICACHE_FLASH_ATTR uc_time_get_next_time_repeat(struct tm * pone_tm, ucd_period_timer_node_t *node)
{
	int i;
	int time1 = 0;
	int time2 = 0;	 

	if (!node->repeat || node->timer.week == 0) {
		return 0;
	}

	time1 = get_utc_time_sec(pone_tm->tm_hour, pone_tm->tm_min, pone_tm->tm_sec);	

	for(i = pone_tm->tm_wday; i < 7*2; i++) {
		if (!(node->timer.week & (1 << i%7))) {
			continue;
		}

		time2 = (i - pone_tm->tm_wday)*ONE_DAY_SEC + get_utc_time_sec(node->timer.hours, node->timer.minute, 0);

		DEVTIMER_INFO( "i=%d id=%u on=%u  node->week=%u repeat uc_time_get_next_time_repeat time1=%d time2 = %d\n", 
			i, node->timer.id, node->timer.on, node->timer.week, time1, time2);			
		if (time2 > time1) {
			return time2 - time1;
		}
	}

	return 0;
}

u_int32_t ICACHE_FLASH_ATTR uc_time_get_next_time(struct tm * pone_tm, ucd_period_timer_node_t *node)
{
	if (!node->repeat) {
		return uc_time_get_next_time_once(pone_tm, node);
	}

	return uc_time_get_next_time_repeat(pone_tm, node);
}

static bool ICACHE_FLASH_ATTR ucd_timer_id_is_valid(u_int8_t id)
{
	ucd_period_timer_node_t *node;

	stlc_list_for_each_entry(ucd_period_timer_node_t, node, &PERIOD_TIMER, link) {
		if (node->timer.id == id) {
			return true;
		}
	}

	return false;
}

int ICACHE_FLASH_ATTR ucd_timer_add(uc_timer_func_t start_proc, uc_timer_func_t finish_proc, u_int8_t id,  net_period_timer_t *expir)
{
	int tmp_id = (int)id;	
	ucd_period_timer_node_t *node;

	if(expir == NULL){
		DEVTIMER_ERROR( "ucd_timer_add expir == NULL\n", tmp_id);	
		return -1;		
	}

	if (ucd_timer_id_is_valid(tmp_id)) {
		ucd_timer_mod(id, expir);
		return tmp_id;
	}

	//包括两种，一种指定timer id添加，一种自己生成。
	if (tmp_id == 0) {
		tmp_id = ucd_timer_get_id();
	}
	
	if (tmp_id < 0 || tmp_id >= ID_TIMER_MAX) {
		DEVTIMER_ERROR( "timer id=%u is invalid\n", tmp_id);	
		return -1;
	}
	
	node = iw_calloc(sizeof(ucd_period_timer_node_t), 1);
	if (node == NULL) {
		DEVTIMER_ERROR( "calloc failed\n");	
		return -1;
	}
	node->timer.id = tmp_id;
	node->timer.hours = expir->hours;	
	node->timer.minute = expir->minute;
	node->timer.week = expir->week;
	node->timer.enable = expir->enable;
	node->timer.on = expir->on;
	node->timer.duration = expir->duration;
	node->timer.ext_data = expir->ext_data;
	node->start_proc = start_proc;
	node->finish_proc = finish_proc;
	node->pdata = node;
	node->repeat = (expir->week != 0);
	node->is_processing = false;
	node->time_sec = 0;

	stlc_list_add_tail(&node->link, &PERIOD_TIMER);

	iw_request_event(EVENT_SETUP_TIMER);

	return tmp_id;
}

#ifdef HAS_TIMER_EXT
int ICACHE_FLASH_ATTR 
ucd_timer_add_ext(uc_timer_func_t start_proc, uc_timer_func_t finish_proc, u_int8_t id,  net_period_timer_t *expir, void * ext_data, int data_len)
{
	UINT8 * data = NULL;

	if (ucd_timer_id_is_valid(id)) {
		ucd_timer_mod_ext(id, expir, ext_data, data_len);
		return id;
	}

	if(ext_data && data_len)
		data = iw_calloc(data_len, 1);

	if (data == NULL) {
		DEVTIMER_ERROR( "calloc failed\n");	
		return -1;
	}	

	os_memcpy(data, ext_data, data_len);
	expir->ext_data = data;
	return ucd_timer_add(start_proc, finish_proc, id, expir);
}
#endif

RS ICACHE_FLASH_ATTR ucd_timer_del(u_int8_t id)
{
	ucd_period_timer_node_t *node, *next;

	if(id >= 100) {
		id = (id - 100 + 1) / 2;
	}

	stlc_list_for_each_entry_safe(ucd_period_timer_node_t, node, next, &PERIOD_TIMER, link) {
		if (node->timer.id == id) {
			stlc_list_del_only(&node->link);
			iw_free(node);	
			break;
		}
	}
	
	return RS_OK;
}
	
#ifdef HAS_TIMER_EXT
RS ICACHE_FLASH_ATTR ucd_timer_del_ext(u_int8_t id)
{
	ucd_period_timer_node_t *node, *next;

	if(id >= 100) {
		id = (id - 100 + 1) / 2;
	}

	stlc_list_for_each_entry_safe(ucd_period_timer_node_t, node, next, &PERIOD_TIMER, link) {
		if (node->timer.id == id) {
			stlc_list_del_only(&node->link);
			iw_free(node);	
			if(node->timer.ext_data)
				iw_free(node->timer.ext_data);
			break;
		}
	}
	
	return RS_OK;
}
#endif

RS ICACHE_FLASH_ATTR ucd_timer_mod(u_int8_t id, net_period_timer_t *expir)
{
	ucd_period_timer_node_t *node;

	if(id >= 100) {
		id = (id - 100 + 1) / 2;
	}
	
	stlc_list_for_each_entry(ucd_period_timer_node_t, node, &PERIOD_TIMER, link) {
		if (node->timer.id== id) {
			node->timer.enable = expir->enable;
			node->timer.week = expir->week;
			node->repeat = (expir->week != 0);
			node->timer.hours = expir->hours;	
			node->timer.minute = expir->minute;				
			node->timer.on = expir->on;
			node->timer.duration = expir->duration;
			iw_request_event(EVENT_SETUP_TIMER);			
			break;
		}
	}

	return RS_OK;
}

#ifdef HAS_TIMER_EXT
RS ICACHE_FLASH_ATTR ucd_timer_mod_ext(u_int8_t id, net_period_timer_t *expir, void* ext_data, int data_len)
{
	ucd_period_timer_node_t *node;

	if(id >= 100) {
		id = (id - 100 + 1) / 2;
	}
	
	stlc_list_for_each_entry(ucd_period_timer_node_t, node, &PERIOD_TIMER, link) {
		if (node->timer.id== id) {
			node->timer.enable = expir->enable;
			node->timer.week = expir->week;
			node->repeat = (expir->week != 0);
			node->timer.hours = expir->hours;	
			node->timer.minute = expir->minute;				
			node->timer.on = expir->on;
			node->timer.duration = expir->duration;
			if(node->timer.ext_data)
				os_memcpy(node->timer.ext_data, ext_data, data_len);
			iw_request_event(EVENT_SETUP_TIMER);			
			break;
		}
	}

	return RS_OK;
}
#endif

static int ICACHE_FLASH_ATTR ucd_period_timer_proc(void *t)
{
	time_t now = 0;
	struct tm tm;	
	ucd_period_timer_node_t *node, *next;
	ucd_period_timer_node_t *processing_timer = NULL;

	//now = time(NULL);//local_time.time  + get_sec() - local_time.time_diff;
	now = get_sec();
	localtimes(now, &tm);

	//看下ntp是否同步过了
	if (!time_syn_ok()) {
		iw_timer_set(TID_PERIOD_TIMER, TIME_N_SECOND(60 - tm.tm_sec), 0, ucd_period_timer_proc, NULL);
		return 0;
	}

	DEVTIMER_DEBUG("now time:%lu\n",now);

	DEVTIMER_DEBUG("ucd_period_timer_proc,now year:%lu mouth:%u day:%u hour = %d, min = %d, week = %d\n", 
	tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_wday);

	stlc_list_for_each_entry_safe(ucd_period_timer_node_t, node, next, &PERIOD_TIMER, link) {
		DEVTIMER_INFO("timer, id = %u, is_processing = %u, enable = %u\n", node->timer.id, node->is_processing, node->timer.enable);
		DEVTIMER_INFO("timer, hours = %u, minute = %u, duration= 0x%X, week = %u\n", node->timer.hours, node->timer.minute, node->timer.duration, node->timer.week);
		if(node->is_processing) {
			DEVTIMER_INFO("is_processing == 1\r\n");
			//正在执行的定时器，检查持续时间有没有到
			if(processing_timer) {
				processing_timer->is_processing = false;
				if(!processing_timer->repeat) {
					processing_timer->timer.enable = false;
				}
			}
			processing_timer = node;
			if(!node->timer.enable || node->timer.duration == 0xFFFF) {
				//定时器不生效或持续时间为永久的定时器
				node->is_processing = false;
				continue;
			}
 			if(node->time_sec / 60 + node->timer.duration == now / 60) {
				//定时器持续时间已到
				if(!node->repeat) {
					node->timer.enable = false;
				}
				node->is_processing = false;
				//这里定时器持续时间已到点，不属于正在执行的定时器
				processing_timer = NULL;
				if(node->finish_proc) {
					node->finish_proc(node->pdata);
				}
				try_timer_push();
 			}
		} else {
			DEVTIMER_INFO("is_processing == 0\r\n");
			//不是正在执行的定时器，检查是否到执行时间
			if(!node->timer.enable || (node->repeat && !(node->timer.week & (1 << tm.tm_wday)) )) {
				continue;
			}
			if(tm.tm_hour * ONE_HOUR_MIN + tm.tm_min  == node->timer.hours * ONE_HOUR_MIN + node->timer.minute) {
				DEVTIMER_INFO("time is arrive\r\n");
				//到点需要执行的定时器
				if(processing_timer) {
					//processing_timer正在执行，此时有新定时器到点需要执行,废弃processing_timer的规则
					processing_timer->is_processing = false;
					processing_timer = NULL;
				}
				node->time_sec = now;
 				if(node->timer.duration == 0xFFFF) {
 					node->is_processing = false;
					if(!node->repeat) {
						node->timer.enable = false;
					}
 				} else {
					node->is_processing = true;
					processing_timer = node;
				}
				if(node->start_proc) {
					node->start_proc(node->pdata);
				}
				try_timer_push();
			}
		}
	}
	
	iw_timer_set(TID_PERIOD_TIMER, TIME_N_SECOND(60 - tm.tm_sec), 0, ucd_period_timer_proc, NULL);
	return 0;
}

void ICACHE_FLASH_ATTR ucd_timer_rebuild(void)
{
	time_t now = 0;
	struct tm tm;	

	//now = time(NULL);//local_time.time  + get_sec() - local_time.time_diff;
	now = get_sec();
	localtimes(now, &tm);
	
	iw_timer_set(TID_PERIOD_TIMER, TIME_N_SECOND(60 - tm.tm_sec), 0, ucd_period_timer_proc, NULL);
}

void ICACHE_FLASH_ATTR ia_do_period_time_head_fill(period_timer_head_t *timer_head)
{
	time_t one_s = 0;
	time_t tmp_on_s = 0;
	time_t tmp_off_s = 0;
	u_int32_t now_s = 0;
	struct tm one_tm;
	ucd_period_timer_node_t *node, *next;
	ucd_period_timer_node_t *on_next = NULL, *off_next = NULL;
	u_int32_t local_time_on, local_time_off;

	one_s = get_sec();
	localtimes(one_s, &one_tm);

	DEVTIMER_DEBUG("ia_do_period_time_head_fill,now year:%lu mouth:%u day:%u hour = %d, min = %d, week = %d\n", 
	one_tm.tm_year, one_tm.tm_mon, one_tm.tm_mday, one_tm.tm_hour, one_tm.tm_min, one_tm.tm_wday);

	DEVTIMER_DEBUG("sec = %d\r\n", one_s);
	
	now_s = get_local_time_sec(one_tm.tm_hour, one_tm.tm_min, one_tm.tm_sec);
	stlc_list_for_each_entry_safe(ucd_period_timer_node_t, node, next, &PERIOD_TIMER, link) {
		if (!node->timer.enable || node->is_processing) {
			continue;
		}
		//下次最小开机,0表示开机
		if (node->timer.on == TIMER_ON) {
			DEVTIMER_DEBUG("node->timer.on == TIMER_ON\r\n");
			one_s = uc_time_get_next_time(&one_tm, node);
			if (tmp_on_s > one_s || tmp_on_s == 0) {
				tmp_on_s = one_s;
				on_next = node;
			}
		} else {
			//下次最小关机
			one_s = uc_time_get_next_time(&one_tm, node);
			if (tmp_off_s > one_s || tmp_off_s == 0) {
				tmp_off_s = one_s;
				off_next = node;
			}
			DEVTIMER_DEBUG("node->timer.on != TIMER_ON\r\n\r\n");
		}
	}

	DEVTIMER_INFO("on tmp_on_s=%u\n", tmp_on_s);
	DEVTIMER_INFO("off tmp_off_s=%u\n", tmp_off_s);
	//先填充下次最小开机时间
	if (tmp_on_s > 0 && on_next) {
		timer_head->on_valid = 1;
		timer_head->on_day = tmp_on_s/ONE_DAY_SEC;
		local_time_on = get_local_time_sec(on_next->timer.hours, on_next->timer.minute, 0);
		DEVTIMER_INFO("on now_s=%u node_time=%u\n", now_s, local_time_on);
		//这里有点晕，应该用本地时间来比较大小，因为是用的本地时间习惯。
		if (now_s > local_time_on) {
			timer_head->on_day++;
		}
		DEVTIMER_ERROR("now_m=%u next=%u\n", one_s, on_next->timer.hours * ONE_HOUR_MIN + on_next->timer.minute);
		timer_head->on_hour = on_next->timer.hours;
		timer_head->on_min = on_next->timer.minute;
		timer_head->on_duration = htons(on_next->timer.duration);
	}
	//填充下次最小关机时间
	if (tmp_off_s > 0 && off_next) {
		timer_head->off_valid = 1;
		timer_head->off_day = tmp_off_s/ONE_DAY_SEC;
		local_time_off = get_local_time_sec(off_next->timer.hours, off_next->timer.minute, 0);
		DEVTIMER_INFO("off now_m=%u node_time=%u\n", now_s, local_time_off);	
		if (now_s > local_time_off) {
			timer_head->off_day++;
		}
		timer_head->off_hour = off_next->timer.hours;
		timer_head->off_min = off_next->timer.minute;
		timer_head->off_duration = htons(off_next->timer.duration);
	}
}

static int ICACHE_FLASH_ATTR set_timer_push_buff(u_int8_t **buff)
{
	int i = 0;
	int len = 0;
	net_period_timer_t * net_timer;	
	u_int8_t *timer_buff = NULL;	
	period_timer_head_t *timer_head;
	ucd_period_timer_node_t *node, *next;	
	
	if (!buff) {
		return 0;
	}
	
	//先计算还有几个timer
	stlc_list_for_each_entry_safe(ucd_period_timer_node_t, node, next, &PERIOD_TIMER, link) {
		i++;
	}	
	DEVTIMER_INFO("have %d timer\r\n", i);
	len = i*sizeof(net_period_timer_t) + sizeof(period_timer_head_t);
	timer_buff = iw_calloc(len, 1);
	if (!timer_buff) {
		DEVTIMER_ERROR("calloc failed\n");
		return 0;
	}
	timer_head = (period_timer_head_t *)timer_buff;
	ia_do_period_time_head_fill(timer_head);
	net_timer = (net_period_timer_t *)(timer_head + 1);
	stlc_list_for_each_entry_safe(ucd_period_timer_node_t, node, next, &PERIOD_TIMER, link) {
		net_timer->id = node->timer.id;
		net_timer->enable = node->timer.enable;
		net_timer->week = node->timer.week;
		net_timer->hours = node->timer.hours;
		net_timer->minute = node->timer.minute;
		net_timer->on = node->timer.on;
		net_timer->duration = htons(node->timer.duration);
		net_timer->ext_data = 0;
		net_timer++;
	}

	*buff = timer_buff;

	return len;
}

static pkt_t * ICACHE_FLASH_ATTR get_timer_push_pkt(ucds_session_t *s)
{
	u_int8_t *timer_buff = NULL;
	u_int8_t *buf;
	u_int32_t timer_len, total_len;
	pkt_t *pkt;
	ucp_ctrl_t *uc;
	int uci_send_index = 0;

	timer_len = set_timer_push_buff(&timer_buff);
	if(timer_len <= 0) {
		return NULL;
	}
	
	total_len =  timer_len + sizeof(ucp_obj_t);

	pkt = uc_pkt_new(CMD_UDP_CTRL, total_len, 
		true, s->is_enc, 0, s->client_sid , ucd.client.device_sid, s->my_request_id);
	
	if (pkt == NULL) {
		return NULL;
	}	
	uc = get_ucp_payload(pkt, ucp_ctrl_t);
	uc->action = UCA_PUSH;
	uc->count = total_len;

	buf = (u_int8_t*)&(uc[1]);

	_uc_put_elemet(buf,  total_len, &uci_send_index, 
		UCOT_IA, UCSOT_IA_PUBLIC, UCAT_IA_PUBLIC_PERIOD_TIMER, timer_len, timer_buff);
	iw_free(timer_buff);

	DEVTIMER_DEBUG("secid=%u uci_send_index=%u\n",s->client_sid, uci_send_index);

	return pkt;	
	
}

static bool ICACHE_FLASH_ATTR try_timer_push(void)
{
	int i;
	ucds_session_t *s;
	pkt_t *pkt;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];
		if (!s->is_used || 
			s->status != UCSS_ESTABLISH ||
			!s->is_recv_udp)	{
			continue;
		}

		DEVTIMER_INFO("push id == [%d]\r\n", i);
		pkt = get_timer_push_pkt(s);
		ucds_request_add(s, pkt);
	}	
	
	return true;
}


void ICACHE_FLASH_ATTR config_timer_save(void)
{
	int i;
	int len = 0;
	void *pmem = NULL;	
	ucd_period_timer_node_t *node, *next;
	net_period_timer_t *net_timer = NULL;
	u_int32_t version = 0xFFAA0002; // TODO: 新旧版本定义有问题

	//空调定时器读回		
	i = 0;
	stlc_list_for_each_entry_safe(ucd_period_timer_node_t, node, next, &PERIOD_TIMER, link) {
		i++;
	}

	
#ifdef HAS_TIMER_EXT
	len = i* (sizeof(net_period_timer_t) + TIMER_EXT_DATA_LEN) + sizeof(u_int32_t);
#else
	len = i*sizeof(net_period_timer_t) + sizeof(u_int32_t);
#endif
	pmem = iw_calloc(len, 1); 
	if (pmem) {
		//增加一个版本号标识来处理配置文件的兼容性
		os_memcpy((u_int8_t*)pmem, (u_int8_t*)&version, sizeof(version));
		
		net_timer = (net_period_timer_t *)((u_int8_t*)pmem + sizeof(u_int32_t));
		stlc_list_for_each_entry_safe(ucd_period_timer_node_t, node, next, &PERIOD_TIMER, link) {
			net_timer->id = node->timer.id;
			net_timer->enable = node->timer.enable;
			net_timer->hours = node->timer.hours;
			net_timer->minute = node->timer.minute;
			net_timer->on = node->timer.on;
			net_timer->week = node->timer.week;
			net_timer->duration = node->timer.duration;
#ifdef HAS_TIMER_EXT
			if(node->timer.ext_data)
				os_memcpy(net_timer + 1, node->timer.ext_data, TIMER_EXT_DATA_LEN);
			net_timer = (net_period_timer_t *)((u_int8_t *)net_timer + TIMER_EXT_DATA_LEN);
#endif
			net_timer++;
		}
		conf_user_set(USER_AC_TIMER, pmem, len);
		iw_free(pmem);
		pmem = NULL;
	}	
}

void ICACHE_FLASH_ATTR config_timer_init(uc_timer_func_t start_proc, uc_timer_func_t finish_proc)
{
	int len = 0;
	config_t *config;
	net_timer_t *net_timer = NULL;	
	u_int32_t version = 0xFFAA0002; // TODO: 新旧版本定义有问题

	net_period_timer_t period_timer;
	net_period_timer_t *p_period_timer;

	m_start_proc = start_proc;
	m_finish_proc = finish_proc;
	
	//空调定时器读回
	config = conf_user_get(USER_AC_TIMER);
	if(config) {
		if(*((u_int32_t *)config->value) != version) {
			//旧版本，需要处理
			for(len = 0; len + sizeof(net_timer_t) <= config->len; len += sizeof(net_timer_t)) {
				net_timer = (net_timer_t *)(config->value + len);
				period_timer.id = net_timer->id;
				period_timer.hours = net_timer->hours;
				period_timer.minute = net_timer->minute;
				period_timer.week = net_timer->week;
				period_timer.enable = net_timer->enable;
				period_timer.on = net_timer->on;
				period_timer.duration = 0xFFFF;
				period_timer.ext_data = 0;
				ucd_timer_add(start_proc, finish_proc, period_timer.id, &period_timer);
				DEVTIMER_ERROR("now add a old timer, id=%u\n" ,period_timer.id);
			}
		} else {
			for(len = sizeof(u_int32_t); len + sizeof(net_period_timer_t) <= config->len; len += sizeof(net_period_timer_t)) {
				p_period_timer = (net_period_timer_t *)(config->value + len);
#ifdef HAS_TIMER_EXT
				ucd_timer_add_ext(start_proc, finish_proc, p_period_timer->id, p_period_timer, p_period_timer + 1, TIMER_EXT_DATA_LEN);
				len += TIMER_EXT_DATA_LEN;
#else
				ucd_timer_add(start_proc, finish_proc, p_period_timer->id, p_period_timer);
#endif
				DEVTIMER_ERROR("now add a timer, id=%u\n" ,p_period_timer->id);
			}
		}
	}
}

rs_t ICACHE_FLASH_ATTR dev_timer_start(u_int8_t type)
{
	iw_request_event(EVENT_SETUP_TIMER);
	return RS_OK;
}

void ICACHE_FLASH_ATTR iw_ucd_timer_init(void)
{
	DEVTIMER_ERROR("iw_ucd_timer_init\r\n");
	STLC_INIT_LIST_HEAD(&PERIOD_TIMER);

	if(iw_ntp_get_init_state()){
		syn_time_event_register(dev_timer_start, (u_int8_t *)"dev_timer_start");		
	}
}

