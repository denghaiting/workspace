#ifndef __IWUOS_EVENT_HEADER__
#define __IWUOS_EVENT_HEADER__

#include "stlc_list.h"
#include "iw_api.h"

#define IW_OBJ_BUCKET 16
#define IW_EVT_BUCKET 16

typedef struct{
	struct stlc_hlist_head obj_head[IW_OBJ_BUCKET];
}iw_obj_hash_t;

typedef struct{
	struct stlc_hlist_head evt_head[IW_OBJ_BUCKET];
}iw_event_hash_t;


#include "iw_priv.h"

typedef struct{
	iw_obj_t obj;
	struct stlc_hlist_node link;
}iw_obj_node_t;

typedef struct{
	u_int16_t event;
	iw_event_callback_t cb;
	struct stlc_hlist_node link;
}iw_event_node_t;

/*
	功能:
		从事件hash表中找到对应事件的节点
	输入参数:
		@event: 事件
	输出参数:
	返回值:
		iw_event_node_t *: 找到时间节点
		NULL: 失败
	说明:
*/
iw_event_node_t * _iw_event_find(u_int16_t event);

/*
	功能:
		初始化事件hash表
	输入参数:
		@priv: 设备维护结构
	输出参数:
	返回值:
	说明:
*/
void iw_event_init(iw_priv_t *priv);

/*
	功能:
		根据事件回调函数
	输入参数:
		@event: 事件
		@info: 回调需要使用的信息
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
rs_t  iw_event_call(u_int16_t event, iw_event_info_t *info);

void iw_event_foruart(uint8 IN_value);
#endif
