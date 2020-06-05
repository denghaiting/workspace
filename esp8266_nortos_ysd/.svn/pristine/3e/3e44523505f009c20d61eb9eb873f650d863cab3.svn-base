/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_plug.c
 *
 * Description: plug demo's function realization
 *
 * Modification history:
 *     2014/5/1, v1.0 create this file.
*******************************************************************************/

#include "iw_priv.h"
#include "iw_obj_register.h"

#define _evt_hash(eid) ((eid)%IW_EVT_BUCKET)

u_int32_t ICACHE_FLASH_ATTR _obj_hash(iw_obj_t *obj)
{
	u_int32_t x;

	x = ((((u_int32_t)obj->objct)<<9) ^  (((u_int32_t)obj->sub_objct)<<13) ^ obj->sub_objct) + obj->attr;
	x = x ^ 0x9e3779b9;

	return (x%IW_OBJ_BUCKET);
}

#define IW_OBJ_SAME(obj1 , obj2) \
(((obj1)->objct == (obj2)->objct) \
&& ((obj1)->sub_objct == (obj2)->sub_objct) \
&& ((obj1)->attr == (obj2)->attr))


void ICACHE_FLASH_ATTR iw_event_hash_init(iw_event_hash_t *hash)
{
	int i;
	for( i = 0; i < IW_EVT_BUCKET; i++){
		STLC_INIT_HLIST_HEAD(&hash->evt_head[i]);
	}
}

void ICACHE_FLASH_ATTR iw_obj_hash_init(iw_obj_hash_t *hash)
{
	int i;
	for( i = 0; i < IW_OBJ_BUCKET; i++){
		STLC_INIT_HLIST_HEAD(&hash->obj_head[i]);
	}
}

iw_event_node_t * ICACHE_FLASH_ATTR _iw_event_find(u_int16_t event)
{
	struct stlc_hlist_node *pos;
	iw_event_node_t *node;
	iw_event_hash_t *hash = &piw_priv->event_hash;
	
	stlc_hlist_for_each_entry(node, pos, &hash->evt_head[_evt_hash(event)], link){
		if(node->event == event){
			return node;
		}
	}

	return NULL;
}

rs_t ICACHE_FLASH_ATTR iw_event_call(u_int16_t event, iw_event_info_t *info)
{
    iw_event_node_t *ev = NULL;
	iw_event_info_t *evp = NULL;

	if (event < 0 ){
		return RS_ERROR;
	}

	if((ev = _iw_event_find(event)) == NULL){
		return RS_ERROR;	
	}
	
	evp = info;
	ev->cb(evp);

	return RS_OK;
}

void ICACHE_FLASH_ATTR iw_event_foruart(uint8 IN_value)
{
	iw_event_info_t *info = NULL;
	uint8 *pvalue = NULL;
	if(!piw_priv->event_isinited){
		return;
	}

	info = (iw_event_info_t *)os_malloc(sizeof(iw_event_info_t) + sizeof(uint8));
	if (info == NULL) {
		return;
	}

	pvalue = (uint8 *)(info + 1);
	*pvalue = IN_value;
	
	info->event = IW_EV_UARTR_2COMMON;
	info->err_no = RS_OK;
	info->cu.cp = (void *)pvalue;
	iw_event_call(IW_EV_UARTR_2COMMON, info);
	
	os_free(info);
	
	return;
}


rs_t ICACHE_FLASH_ATTR iw_event_register(u_int16_t event, iw_event_callback_t callback)
{
	struct stlc_hlist_node *pos;
	iw_event_node_t *node;
	iw_event_hash_t *hash = &piw_priv->event_hash;
	
	stlc_hlist_for_each_entry(node, pos, &hash->evt_head[_evt_hash(event)], link){
		if(node->event == event && node->cb == callback){
			return RS_EXIST;
		}
	}
	node = (iw_event_node_t *)iw_malloc(sizeof(*node));
	if(node == NULL)
		return RS_MEMORY_MALLOC_FAIL;
	node->event = event;
	node->cb = callback;
	stlc_hlist_add_head(&node->link, &hash->evt_head[_evt_hash(event)]);
	return RS_OK;
}

iw_obj_t * ICACHE_FLASH_ATTR _iw_obj_find(iw_obj_t *obj, iw_obj_hash_t *hash)
{
	struct stlc_hlist_node *pos;
	iw_obj_node_t *node;
	u_int32_t index;

	if(NULL == obj || NULL == hash){
		return NULL;
	}

	index = _obj_hash(obj);	
	stlc_hlist_for_each_entry(node, pos, &hash->obj_head[index], link){
		if(IW_OBJ_SAME(&node->obj, obj)){
			return &node->obj;
		}
	}
	
	return NULL;
}


rs_t ICACHE_FLASH_ATTR _iw_obj_register(iw_obj_t * obj, iw_obj_hash_t *hash)
{
	struct stlc_hlist_node *pos;
	iw_obj_node_t *node;
	u_int32_t index;

	index = _obj_hash(obj);	
	stlc_hlist_for_each_entry(node, pos, &hash->obj_head[index], link){
		if(IW_OBJ_SAME(&node->obj, obj)){
			return RS_EXIST;
		}
	}
	node = (iw_obj_node_t *)iw_malloc(sizeof(*node));
	if(node == NULL)
		return RS_MEMORY_MALLOC_FAIL;
	node->obj= *obj;
	stlc_hlist_add_head(&node->link, &hash->obj_head[index]);
	return RS_OK;
}

rs_t ICACHE_FLASH_ATTR iw_obj_register(iw_obj_t * obj, int num)
{
	int i;
	iw_obj_hash_t *hash = &piw_priv->obj_hash;

	for(i = 0; i < num; i++){
		_iw_obj_register(obj, hash);
		obj = (iw_obj_t *)(((char*)(obj+1)) + obj->param_len);
	}
	
	return RS_OK;
}

void ICACHE_FLASH_ATTR iw_event_init(iw_priv_t *priv)
{
	if(NULL == priv){
		return;
	}
	
	iw_event_hash_init(&priv->event_hash);
	iw_obj_hash_init(&priv->obj_hash);
	
	iw_obj_event_init();
	priv->event_isinited = 1;
}

