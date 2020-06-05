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

pkt_t * ICACHE_FLASH_ATTR
uc_pkt_new(int cmd, int param_len, 
			bool is_request, bool is_enc, u_int8_t flags,
			u_int32_t client_sid, u_int32_t device_sid, u_int8_t request_id)
{
	pkt_t *pkt;
	ucph_t *hdr;
	int total_len = sizeof(pkt_t) + ucph_hdr_size + param_len;

	// 多申请一块内存，用于加密时候用sizeof(pkt_t) + ucph_hdr_size + param_len;
	if ((pkt = (pkt_t *)iw_calloc(total_len + AES128_EKY_LEN, 1)) == NULL) {
		ERROR( "%s alloc %d failed\n", __FUNCTION__, total_len);
		return NULL;
	} 	
	pkt->data = (u_int8_t *)(pkt + 1);
	pkt->total = ucph_hdr_size + param_len;
	pkt->cmd = cmd; 
	hdr = (ucph_t *)(pkt + 1); 
	hdr->ver = PROTO_VER1;
	hdr->hlen = ucph_hdr_size/4;
	hdr->request = is_request;
	hdr->encrypt = is_enc;
	hdr->flags = flags;
	hdr->client_sid = ntohl(client_sid);
	hdr->device_sid = ntohl(device_sid);
	hdr->request_id = request_id; 
	hdr->command = ntohs(cmd);
	hdr->param_len = ntohs(param_len);
 
	return pkt;
}

void ICACHE_FLASH_ATTR
pkt_free(void *pkt)
{
	if (pkt)
		iw_free(pkt);
}

/*
	retyr0、1、2的单位为100毫秒
	keeplive和die的单位为秒
*/
void ICACHE_FLASH_ATTR
uc_set_time_param(uc_time_param_item_t *it,
		u_int8_t retry0, u_int8_t retry1, u_int8_t retry2,
		u_int8_t keeplive, u_int16_t die)
{
	it->retry_100ms[0] = retry0;
	it->retry_100ms[1] = retry1;
	it->retry_100ms[2] = retry2;

	it->keeplive = keeplive;
	it->die = die;
}


void ICACHE_FLASH_ATTR
uc_set_default_time_param(uc_time_param_all_t *time_param, uc_time_param_all_t *time_param_net)
{
	// 设备与服务器通信	1秒	3秒	3秒，20秒	30秒
	uc_set_time_param(&time_param->dev, 10, 30, 30, 20, 30);
	// 手机与设备局域网连接	0.1秒	0.3秒	1秒，3秒	10秒
	uc_set_time_param(&time_param->lan, 1, 3, 10, 3, 10);
	// 手机与设备广域网前台	1秒	2秒	3秒，20秒	5分钟
	uc_set_time_param(&time_param->wan_front, 10, 20, 30, 20, 300);
	// 手机与设备广域网后台	1秒	5秒	20秒，2分钟	5分钟
	uc_set_time_param(&time_param->wan_background, 10, 50, 100, 120, 300);

	memcpy(time_param_net, time_param, sizeof(uc_time_param_all_t));
	time_param_net->dev.die = htons(time_param_net->dev.die);
	time_param_net->lan.die = htons(time_param_net->lan.die);
	time_param_net->wan_front.die = htons(time_param_net->wan_front.die);
	time_param_net->wan_background.die = htons(time_param_net->wan_background.die);
}


void ICACHE_FLASH_ATTR ucp_obj_order(ucp_obj_t *uobj)
{
	uobj->objct = ntohs(uobj->objct);
	uobj->sub_objct = ntohs(uobj->sub_objct);
	uobj->attr = ntohs(uobj->attr);
	uobj->param_len = ntohs(uobj->param_len);	
}



