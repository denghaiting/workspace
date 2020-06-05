/**************************************************************************
**  Copyright (c) 2010 GalaxyWind, Ltd.
**
**  Project: uc_tt_common.c
**  File:    uc_tt_common.c
**  Author:  
**  Date:    12/10/2014
**
**  Purpose:
**    透明传输通用部分代码.
**************************************************************************/


/* Include files. */
#include "iw_uc_tt_common.h"
#include "iw_flash.h"
#include "iw_user_config.h"
#include "iw_push.h"

//#include "mu_active.h"
//#include "uc_uart_common.h"


//#include "change_evm_no.h"
#include "iw_uart_ijuartpro.h"


/* Macro constant definitions. */
#define MAX_TT_UART_DATA_LEN	256
#define	KXM_UART_PREMBLE_CODE1	0x5A
#define	KXM_UART_PREMBLE_CODE2	0xA5


/* Type definitions. */

/* Local function declarations. */


/* Macro API definitions. */

/* Global variable declarations. */
void dump_mem(char *msg, u_int8_t *data, int data_len)
{
	char buf[256] = {0};
	int i, len;

	if (data_len > 100) {
		return;
	}


	for (i = len = 0; i < data_len; i++) {
		len += os_sprintf(&buf[len], "%02X ", data[i]);
	}
	
	INFO("%s: len %u \n %s\n", msg, data_len, buf);
}

int tt_uart_send(u_int8_t *data, u_int16_t data_len)
{
	dump_mem("net_to_uart_date----------->", data, data_len);
	
	emptying_uart_send_list();
	base_uart_send(data, data_len);
	return 0;
}

static void tt_app2chip_cb(iw_event_info_t *info, iw_obj_t *uobj)
{
	uc_tlv_t *tlv = NULL;
	
	if(uobj->param_len == 0 || uobj->param_len < sizeof(uc_tlv_t)){
		ERROR("param_len = 0\r\n");
		return;
	}

	tlv = (uc_tlv_t *)&uobj[1];
	tlv->type = ntohs(tlv->type);
	tlv->len = ntohs(tlv->len);
	INFO("type=%u, len=%u!\n", tlv->type, tlv->len);
	
	tt_uart_send(tlv_val(tlv),tlv->len);
}

void ia_do_tt_cmd(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int8_t action = 0;
	u_int8_t *param = NULL;
	if(!info) {
		ERROR("info is NULL\r\n");
		return;
	}

	action = info->action;
	param = (u_int8_t *)(info + 1);
	if(action != UCA_SET){
		ERROR("action isn't set\r\n");
		return;		
	}
	
	INFO("len = %u\n", uobj->param_len);
	dump_mem("ia_do_tt_cmd", param, uobj->param_len);

	uc_push_clear();

	tt_app2chip_cb(info, uobj);	

	uc_do_set_reply((ucp_obj_t *)uobj, ERR_NONE);
}

void is_do_tt_allstate(iw_event_info_t *info, iw_obj_t *uobj)
{
	config_t * config;
	u_int8_t action = 0;
	u_int8_t *uobj_param;

	if(!info) {
		ERROR("allstate is null\r\n");
		return;
	}
	
	action = info->action;
	if(UCA_GET != action){
		ERROR("action isn't get\r\n");
		return;	
	}
	
	config = conf_dev_get(DEV_SERVER_YSD_STATE);	
	if(NULL != config){
		if ((uobj_param = os_malloc(config->len+4)) == NULL) {
			ERROR("calloc len %u failed\n", config->len + 4);
			return;
		}
		*(u_int16_t *)&uobj_param[0] =  htons(UCT_VENDOR);
		*(u_int16_t *)&uobj_param[2] = htons(config->len);

		os_memcpy(uobj_param + 4, config->value, config->len);

		dump_mem("is_do_tt_allstate", config->value, config->len);

		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		              uobj->sub_objct, uobj->attr, config->len + 4, uobj_param);	
		os_free(uobj_param);
	}else{
		uc_do_set_reply((ucp_obj_t *)uobj, ERR_PARAM_INVALID);
	}
}

void ia_do_tt(iw_event_info_t *info, iw_obj_t *uobj)
{
	DEBUG("sub_objct = %u\n", uobj->sub_objct);
	switch(uobj->attr) {
		case UCAT_IA_TT_ALLSTATE:
			INFO("UCAT_IA_TT_ALLSTATE\r\n");
			is_do_tt_allstate(info, uobj);
			break;
		case UCAT_IA_TT_CMD:
			INFO("UCAT_IA_TT_CMD\r\n");
			ia_do_tt_cmd(info, uobj);
			break;
		default:
			//ERROR("ia_do_public error sub_objct=%u\n", uobj->sub_objct);
			uc_do_set_reply((ucp_obj_t *)uobj, ERR_CMD_INVALID);
			break;
	}	
}

