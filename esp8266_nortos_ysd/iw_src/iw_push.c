/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: push
**  File:    uc_push.c
**  Author:  liubenlong
**  Date:    09/30/2014
**
**  Purpose:
**    Push.
**************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "iw_push.h"
#include "iw_dev.h"

/* Macro constant definitions. */
#define FHF_PUSH_MAX_TIME		20
#define FHF_PUSH_WAN_MIN_TIME		20
#define FHF_PUSH_LAN_MIN_TIME		1

#define FHF_PUSH_PKT_TYPE			1

/* Type definitions. */
typedef struct fhf_push_s{
	bool need_push;
	push_proc func;
	u_int8_t timer_index;
	u_int32_t last_uptime;	
}fhf_push_t;

/* Local function declarations. */


/* Macro API definitions. */
//#define PUSH_DEBUG_
#ifdef PUSH_DEBUG_
#define PUSH_INFO(x...)  INFO(x)
#define PUSH_DEBUG(x...) DEBUG(x)
#define PUSH_ERROR(x...)  ERROR(x)
#else
#define PUSH_DEBUG(x...)
#define PUSH_INFO(x...)
#define PUSH_ERROR(x...) ERROR(x)
#endif


/* Global variable declarations. */
extern RS ucds_request_add(ucds_session_t *s, pkt_t *pkt);

fhf_push_t fhf_push;

static bool ICACHE_FLASH_ATTR is_need_push(ucds_session_t *s)
{
	pkt_t *node, *next;
	int num = 0;
	int all_num = 0;
	
	if (stlc_list_empty(&s->send_list)) {
		PUSH_DEBUG("is_need_push send_list is null\n");
		return true;
	}

	stlc_list_for_each_entry_safe(pkt_t, node, next, &s->send_list, link) {
		all_num++;
		if (node->type == FHF_PUSH_PKT_TYPE) {
			num++;
		}
	}

	//for test
	PUSH_DEBUG("sessionid=%08x is_need_push num=%d all_num=%d\n", s->client_sid, num, all_num);
	if (num > 0) {
		return false;
	}
	
	return true;
}

static void ICACHE_FLASH_ATTR try_push_proc(ucds_session_t *s)
{
	pkt_t *pkt = NULL;
	
	if (fhf_push.func) {
		pkt = fhf_push.func(s);
		if (pkt) {
			pkt->type = FHF_PUSH_PKT_TYPE;
			ucds_request_add(s, pkt);
		}
	}
}

static bool ICACHE_FLASH_ATTR try_fhf_push(void)
{
	int i;
	ucds_session_t *s;
	PUSH_DEBUG("enter try_fhf_push !!!!!!!!!!!!!\n");

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];
		if (!s->is_used || 
			s->status != UCSS_ESTABLISH ||
			!s->need_push || 
			!s->is_recv_udp)	{
			continue;
		}
		
		if (!is_need_push(s)) {
			continue;
		}
		
		if (s->is_lan) {
			if (s->need_push) {
				try_push_proc(s);
				s->need_push = false;
			}
		} else {
			if (s->need_push && s->wan_push_enable) {
				try_push_proc(s);
				s->wan_push_enable = false;
				s->need_push = false;					
			}
		}
	}	

	return true;
}

static void ICACHE_FLASH_ATTR fhf_set_session_wan_push_enable(void)
{
	int i;
	ucds_session_t *s;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];

		if (s->is_used && s->status == UCSS_ESTABLISH)	{
			s->wan_push_enable = true;
		} else {
			s->wan_push_enable = false;
		}
	}	
}

static void ICACHE_FLASH_ATTR fhf_set_session_need_push(void)
{
	int i;
	ucds_session_t *s;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];

		if (s->is_used && s->status == UCSS_ESTABLISH)	{
			s->need_push = true;
		} else {
			s->need_push = false;
		}
	}	
}

static int ICACHE_FLASH_ATTR fhf_push_info(void *t)
{
	fhf_push.timer_index++;

	if (fhf_push.timer_index%FHF_PUSH_WAN_MIN_TIME == 0) {
		fhf_set_session_wan_push_enable();
	}

	if (fhf_push.need_push) {
		fhf_set_session_need_push();
		fhf_push.need_push = false;
	}

	//´¦Àí¡£
	try_fhf_push();

	return 0;
}

void ICACHE_FLASH_ATTR uc_push_set(void)
{
	fhf_push.need_push = true;
}
void ICACHE_FLASH_ATTR uc_push_clear(void)
{
	fhf_push.need_push = false;
}


void ICACHE_FLASH_ATTR uc_push_init(push_proc func)
{
	static bool push_init = false;
	
	fhf_push.func = func;

	if (push_init) {
		return;
	}
	
	fhf_push.timer_index = 0;
	fhf_push.need_push = false;
	fhf_push.last_uptime = get_up_sec();	

	iw_timer_set(TID_UCDC_PUSH, TIME_N_MSECOND(100), 1, fhf_push_info, NULL);
	push_init = true;
}

void ICACHE_FLASH_ATTR uc_push2serv(push2serv_proc func)
{
	ucdc_session_t *sc = &ucd.client;
	pkt_t *pkt = NULL;

	if(sc->status != UCCS_ESTABLISH){
		return;
	}

	if (func) {
		pkt = func();
		if (pkt) {
			pkt->type = FHF_PUSH_PKT_TYPE;
			ucdc_request_add(sc, pkt);
		}
	}
	
}


