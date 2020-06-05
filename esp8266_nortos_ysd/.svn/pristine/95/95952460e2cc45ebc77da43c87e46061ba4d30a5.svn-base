/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: mu
**  File:    packet_clone.c
**  Author:  cailiyang
**  Date:    08/16/2014
**
**  Purpose:
**    Packet clone.
**************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "mu_type.h"
#include "packet_clone.h"
#include "iw_dev.h"
/* Macro constant definitions. */
//#define PACKET_CLONE_DEBUG

/* Type definitions. */


/* Local function declarations. */
static int pc_put_element(u_int8_t *buf, int size, u_int16_t attri, u_int16_t len, u_int8_t *value);

/* Macro API definitions. */


/* Global variable declarations. */

extern RS ucdc_enc_pkt(ucdc_session_t *s, pkt_t *pkt);
void pkt_free(void *pkt);

void ICACHE_FLASH_ATTR ucds_clone_send(ucph_t *hdr, int len, bool recv)
{
	ucdc_session_t *s;
	pkt_t * pkt = NULL;
	ucp_ctrl_t *uc_reply;

	s = &sys_mu_config.ucd->client;

	if (s->status != UCCS_ESTABLISH) {
		return ;
	}

	pkt = uc_pkt_new(CMD_UDP_CTRL, len + sizeof(ucp_ctrl_t) + UDP_CTRL_TL_SIZE,
	                 true, s->is_enc, 0, s->client_sid, s->device_sid, s->my_request_id);

	if (pkt == NULL) {
		ERROR("no buffer for packets.\n");
		return ;
	}

	uc_reply = get_ucp_payload(pkt, ucp_ctrl_t);
	uc_reply->action = UCA_SET;
	if (recv) {
		hdr->client_sid = htonl(hdr->client_sid);
		hdr->device_sid = htonl(hdr->device_sid);
		hdr->command = htons(hdr->command);
		hdr->param_len = htons(hdr->param_len);
	}
	//填充数据
	pc_put_element((u_int8_t *)(uc_reply + 1), len + UDP_CTRL_TL_SIZE,
	               (recv ? UCAT_PACKET_RECEIVE : UCAT_PACKET_SEND),
	               (u_int16_t)len, (u_int8_t *)hdr);
	if (recv) {
		hdr->client_sid = htonl(hdr->client_sid);
		hdr->device_sid = htonl(hdr->device_sid);
		hdr->command = htons(hdr->command);
		hdr->param_len = htons(hdr->param_len);
	}

#ifdef	PACKET_CLONE_DEBUG
	type = recv ? "recv" : "send";
	DEBUG("pkt clone %s len=%d appandlen=%d pkt->total=%d\n",
	      type, len, sizeof(ucph_t) + sizeof(ucp_ctrl_t) + UDP_CTRL_TL_SIZE, pkt->total);
	//mem_dump(type, pkt->data, pkt->total);
#endif

	//加下密
	if (ucdc_enc_pkt(s, pkt) != RS_OK) {
		pkt_free(pkt);
		return;
	}

	ucdc_send_pkt_no_cach(s, pkt);
	pkt_free(pkt);
}

static int ICACHE_FLASH_ATTR pc_put_element(u_int8_t *buf, int size, u_int16_t attri, u_int16_t len, u_int8_t *value)
{
	u_int8_t *ptr;
	u_int16_t tmp[4];

	if (len + UDP_CTRL_TL_SIZE > size) {
		return -1;
	}

	ptr = buf;

	tmp[0] = htons(UCOT_STORAGE);
	tmp[1] = htons(UCSOT_STORAGE_PACKET);
	tmp[2] = htons(attri);
	tmp[3] = htons(len);
	os_memcpy(ptr, &tmp, sizeof(tmp));
	ptr += sizeof(tmp);

	if (len > 0) {
		memcpy(ptr, value, len);
	}

	return 0;
}

