#include "iw_priv.h"


static rs_t ICACHE_FLASH_ATTR iw_do_get_cb(iw_event_info_t *info)
{
	iw_obj_t *uobj = &info->cu.obj;

	if(info == NULL || info->event != IW_EV_OBJECT_GET) {
		return RS_ERROR;
	}


	iw_do_obj_ctrl(info, uobj);
	return RS_OK;
}


static rs_t ICACHE_FLASH_ATTR iw_do_set_cb(iw_event_info_t *info)
{
	iw_obj_t *uobj = &info->cu.obj;


	if(info == NULL || info->event != IW_EV_OBJECT_SET) {
		return RS_ERROR;
	}


	iw_do_obj_ctrl(info, uobj);
	return RS_OK;
}

static rs_t ICACHE_FLASH_ATTR iw_do_delete_cb(iw_event_info_t *info)
{
	iw_obj_t *uobj = &info->cu.obj;


	if(info == NULL || info->event != IW_EV_OBJECT_DELETE) {
		return RS_ERROR;
	}


	iw_do_obj_ctrl(info, uobj);
	return RS_OK;
}

static void ICACHE_FLASH_ATTR iw_obj_fill(iw_obj_t *tmp, u_int16_t objct, u_int16_t sub_objct, u_int16_t attr)
{
	tmp->attr = attr;
	tmp->objct = objct;
	tmp->sub_objct = sub_objct;
}

#define IW_OBJ_MAX 1
void ICACHE_FLASH_ATTR iw_obj_event_init()
{
	char *obj =  iw_calloc(sizeof(iw_obj_t) * IW_OBJ_MAX, 1);
	iw_obj_t *tmp = (iw_obj_t *)obj;
	iw_obj_fill(tmp, UCOT_IA, UCSOT_IA_AC, UCAT_AC_WORK);
	//注册对象
	iw_obj_register((iw_obj_t *)obj, IW_OBJ_MAX);

	//注册动作
	iw_event_register(IW_EV_OBJECT_SET, iw_do_set_cb);
	iw_event_register(IW_EV_OBJECT_GET, iw_do_get_cb);
	iw_event_register(IW_EV_OBJECT_DELETE, iw_do_delete_cb);

	return;
}

