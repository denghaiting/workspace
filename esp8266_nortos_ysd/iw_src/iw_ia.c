#include "iw_priv.h"
#include "iw_ia.h"
#include "user_light.h"
#include "iw_uc_tt_common.h"

rs_t ICACHE_FLASH_ATTR do_ucot_ia_obj(iw_event_info_t *info, iw_obj_t *uobj)
{
	if(NULL == info || NULL == info){
		return RS_ERROR;
	}
	
	switch(uobj->sub_objct) {
		case UCSOT_IA_TT:
			ia_do_tt(info, uobj);
			break;
		case UCSOT_IA_PUBLIC:
			ia_do_public(info, uobj);
			break;
		default:
			DEBUG("ia error sub_objct=%u\n", uobj->sub_objct);
			uc_do_set_reply((ucp_obj_t *)uobj, ERR_CMD_INVALID);
			break;
	}
	return RS_OK;
}
