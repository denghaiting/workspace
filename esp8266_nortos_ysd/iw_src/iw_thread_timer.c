/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    iw_thread_timer.c
**  Author:  fangfuhan
**  Date:    04/11/2016
**
**  Purpose:
**    定时器伪线程.
**************************************************************************/

/* Include files. */
#include "iw_priv.h"
#include "iw_thread_timer.h"

/* Macro constant definitions. */
#define IW_TIMER_BUCKET 16


/* Type definitions. */


/* Local function declarations. */


/* Macro API definitions. */
//#define THTIMER_DEBUG_
#ifdef THTIMER_DEBUG_
#define THTIMER_DEBUG(x...)  ERROR(x) 
#define THTIMER_INFO(x...)   ERROR(x) 
#define THTIMER_ERROR(x...)  ERROR(x)
#else
#define THTIMER_DEBUG(x...) DEBUG(x)
#define THTIMER_INFO(x...)  INFO(x)
#define THTIMER_ERROR(x...) ERROR(x)
#endif

/* Global variable declarations. */
static struct stlc_hlist_head timer_hd[IW_TIMER_BUCKET];


#define _iw_timer_del(t) \
do{\
	os_timer_disarm(&t->timer);\
	stlc_hlist_del(&t->link);\
	iw_free(t);\
}while(0)

static void ICACHE_FLASH_ATTR _iw_timer_cb(void *arg)
{
	iw_timer_t *t = (iw_timer_t *)arg;

	if(t ) {
		if(!t->periodical) {

			os_timer_disarm(&t->timer);
			stlc_hlist_del(&t->link);
			if(t->cb) {
				t->cb(t->cp);
			}

			iw_free(t);
			return;
		}

		if(t->cb) {
			t->cb(t->cp);
		}
	}
}

rs_t ICACHE_FLASH_ATTR
_iw_time_modify(u_int16_t id, u_int32_t ms, bool periodical)
{
	struct stlc_hlist_node *pos, *n;
	iw_timer_t *t;
	rs_t ret = RS_EMPTY;

	stlc_hlist_for_each_entry_safe(t, pos, n, &timer_hd[id % IW_TIMER_BUCKET], link) {
		if(t->tid != id) {
			continue;
		}
		ret = RS_EXIST;
		
		t->ms = ms;
		t->periodical = periodical;
		os_timer_disarm(&t->timer);
		os_timer_setfn(&t->timer, _iw_timer_cb, (void*)t);
		os_timer_arm(&t->timer, ms, periodical);
	}

	return ret;
}

/* the sdk set ms == 0, is not effectived, 1 ms is effectived(may be) */
rs_t ICACHE_FLASH_ATTR iw_timer_set(u_int16_t id, u_int32_t ms, bool periodical, iw_timer_callback_t cb, void *cp)
{
	iw_timer_t *t;

	if(ms <= 0) {
		ms = 1;
	}

	if(_iw_time_modify(id, ms, periodical) == RS_EXIST) {
		return RS_OK;
	}


	t = (iw_timer_t *)iw_malloc(sizeof(*t));
	if(t == NULL) {
		return RS_MEMORY_MALLOC_FAIL;
	}
	
	t->tid = id;
	t->ms = ms;
	t->cb = cb;
	t->cp = cp;
	t->periodical = periodical;
	
	os_timer_disarm(&t->timer);
	os_timer_setfn(&t->timer, _iw_timer_cb, (void*)t);
	os_timer_arm(&t->timer, ms, periodical);
	stlc_hlist_add_head(&t->link, &timer_hd[id % IW_TIMER_BUCKET]);
	return RS_OK;
}

rs_t ICACHE_FLASH_ATTR iw_timer_delete(u_int16_t id)
{
	struct stlc_hlist_node *pos, *n;
	iw_timer_t *t;

	stlc_hlist_for_each_entry_safe(t, pos, n, &timer_hd[id % IW_TIMER_BUCKET], link) {
		if(t->tid == id) {
			_iw_timer_del(t);
		}
	}
	return RS_OK;
}

void ICACHE_FLASH_ATTR
iw_thread_timer_init()
{
	int i;
	THTIMER_DEBUG("iw_thread_timer_init\r\n");
	for( i = 0; i < IW_TIMER_BUCKET; i++) {
		STLC_INIT_HLIST_HEAD(&timer_hd[i]);
	}	
}

