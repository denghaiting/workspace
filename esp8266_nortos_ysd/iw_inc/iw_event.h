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
	����:
		���¼�hash�����ҵ���Ӧ�¼��Ľڵ�
	�������:
		@event: �¼�
	�������:
	����ֵ:
		iw_event_node_t *: �ҵ�ʱ��ڵ�
		NULL: ʧ��
	˵��:
*/
iw_event_node_t * _iw_event_find(u_int16_t event);

/*
	����:
		��ʼ���¼�hash��
	�������:
		@priv: �豸ά���ṹ
	�������:
	����ֵ:
	˵��:
*/
void iw_event_init(iw_priv_t *priv);

/*
	����:
		�����¼��ص�����
	�������:
		@event: �¼�
		@info: �ص���Ҫʹ�õ���Ϣ
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
rs_t  iw_event_call(u_int16_t event, iw_event_info_t *info);

void iw_event_foruart(uint8 IN_value);
#endif