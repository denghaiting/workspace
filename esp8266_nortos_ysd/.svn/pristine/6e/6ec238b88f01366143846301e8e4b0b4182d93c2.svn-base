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
#include "user_plug.h"
#include "user_light.h"
#include "iw_dev_timer.h"
#include "iw_obj_system.h"
#include "iw_ntp_time_syn.h"
#include "packet_clone.h"
#include "iw_func_support.h"

//#define SER_DEBUG_
#ifdef SER_DEBUG_
#define	SER_DEBUG(fmt...) DEBUG(fmt)
#define	SER_INFO(fmt...)  INFO(fmt)
#define	SER_ERROR(fmt...) ERROR(fmt)
#else
#define	SER_DEBUG(fmt...)
#define	SER_INFO(fmt...)
#define	SER_ERROR(fmt...) ERROR(fmt)
#endif

typedef struct wifi_tlv_s {
	u_int16_t type;
	u_int16_t length;
} wifi_tlv_t;
#define SET_WIFI_SSID  1
#define SET_WIFI_PW    2
#define SET_WIFI_TLV_COUNT      2



int	uci_send_index = 0;
u_int8_t uci_send_buf[MAX_UC_PKT_SIZE];
u_int8_t uci_proc_num = 0;

static void ucds_set_status(ucds_session_t *s, int status);
static int ucds_die(void  *cp);
static void ucds_estab_do_request(ucds_session_t *s, ucph_t *hdr);
static void ucds_question_proc(ucds_session_t *s, ucph_t *hdr);
extern int rand(void);
extern void srand(unsigned int seed);
extern void pkt_free(void *pkt);

static void ICACHE_FLASH_ATTR order_uc_time(uc_time_t *t)
{
	t->year = ntohs(t->year);
}


static void ICACHE_FLASH_ATTR ucds_try_update_ip(ucds_session_t *s, u_int32_t ip, u_int16_t port)
{
	if (s->ip == ip && s->port == port)
		return;
#if 1
	SER_INFO("session %s update ip:port from %u.%u.%u.%u:%u to %u.%u.%u.%u:%u\n",
	         s->name, IP_SHOW(s->ip), s->port, IP_SHOW(ip), port);
#endif
	s->ip = ip;
	s->port = port;
}

// 发送复位报文
static int ICACHE_FLASH_ATTR ucds_send_reset(struct espconn *conn, u_int32_t ip, u_int16_t port, u_int32_t sid)
{
	pkt_t *pkt;
	uc_keeplive_reset_t *kr;

	// send keeplive.reset, request=0, rid=0
	pkt = uc_pkt_new(CMD_UDP_KEEPLIVE, sizeof(uc_keeplive_reset_t),
	                 false, false, 0, sid, ucd.client.device_sid, 0);
	if (pkt == NULL) {
		SER_ERROR("no buffer for packets.\n");
		return RS_ERROR;
	}
	kr = get_ucp_payload(pkt, uc_keeplive_reset_t);
	kr->action = UCA_RESET;

	iw_udp_send(ip, port, (char *)pkt->data, pkt->total, conn);

	pkt_free(pkt);

	SER_INFO("send reset pkt to %u.%u.%u.%u:%u, sid=%u\n", IP_SHOW(ip), port, sid);

	return RS_OK;
}

/*********得到客户端个数*************/
int ICACHE_FLASH_ATTR ucds_get_clients(void)
{
	int i;
	int num = 0;
	ucds_session_t *s;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];
		if (s->is_used) {
			num++;
		}
	}

	return num;
}

//lpc ****last_pkt_cach
LOCAL void ICACHE_FLASH_ATTR ucds_lpc_send(ucds_session_t *s)
{
	int n;
	pkt_t *pkt = s->last_pkt;

	if (!pkt) {
		return;
	}

	n = iw_udp_send(s->ip, s->port, (char *)pkt->data, pkt->total, s->conn);
	if (n <= 0) {
		SER_ERROR("ucds_lpc_send %s send pkt failed: ip=%u.%u.%u.%u, port=%d, want=%d, send=%d\n",
		          s->name, IP_SHOW(s->ip), s->port, pkt->total, n);
	} else {
#ifdef SER_DEBUG_
		//默认打开报文收发打印
		ucph_t *hdr;
		hdr = (ucph_t *)pkt->data;
		SER_DEBUG("ucds_lpc_send %s send ip=%u.%u.%u.%u, port=%d, len=%d: cmd=%u, action=%u, req=%d, enc=%d, csid=%08x, dsid=%08x, rid=%u, my_request_id=%u\n",
		          s->name, IP_SHOW(s->ip), s->port, n,
		          pkt->cmd, *((u_int8_t *)(hdr + 1)), hdr->request, hdr->encrypt,
		          ntohl(hdr->client_sid), ntohl(hdr->device_sid), hdr->request_id, s->my_request_id);
#endif
	}
}


LOCAL void ICACHE_FLASH_ATTR ucds_lpc_clean(ucds_session_t *s)
{
	if (s->last_pkt) {
		pkt_free(s->last_pkt);
		s->last_pkt = NULL;
	}
}

LOCAL void ICACHE_FLASH_ATTR ucds_lpc_set(ucds_session_t *s, pkt_t *pkt)
{
	if (s->last_pkt) {
		ucds_lpc_clean(s);
	}

	s->last_pkt = pkt;
}

LOCAL bool ICACHE_FLASH_ATTR ucds_lpc_is_need_send(ucds_session_t *s)
{
	return (s->last_pkt != NULL && s->last_pkt_enable);
}

LOCAL void ICACHE_FLASH_ATTR ucds_lpc_enable(ucds_session_t *s, bool enable)
{
	s->last_pkt_enable = enable;
}


/*****************************************
            发送报文相关函数
 *****************************************/

// 应答报文不需要重传，这里要注意一下，因为加了最后包缓存，用该函数发送数据后不用释放包
LOCAL int ICACHE_FLASH_ATTR ucds_send_pkt(ucds_session_t *s, pkt_t *pkt)
{
	int n;

	//释放上次包缓存，覆盖新包。
	ucds_lpc_clean(s);
	ucds_lpc_set(s, pkt);



	n = iw_udp_send(s->ip, s->port, (char *)pkt->data, pkt->total, s->conn);
	if (n <= 0) {
		SER_ERROR("%s send pkt failed: ip=%u.%u.%u.%u, port=%d, want=%d, send=%d\n",
		          s->name, IP_SHOW(s->ip), s->port, pkt->total, n);
	} else {
#ifdef SER_DEBUG_
		//默认打开报文收发打印
		ucph_t *hdr;
		hdr = (ucph_t *)pkt->data;
		SER_DEBUG("%s send ip=%u.%u.%u.%u, port=%d, len=%d: cmd=%u, action=%u, req=%d, enc=%d, csid=%08x, dsid=%08x, rid=%u, my_request_id=%u\n",
		          s->name, IP_SHOW(s->ip), s->port, n,
		          pkt->cmd, *((u_int8_t *)(hdr + 1)), hdr->request, hdr->encrypt,
		          ntohl(hdr->client_sid), ntohl(hdr->device_sid), hdr->request_id, s->my_request_id);
#endif
	}

	return 0;
}


/*****************************************
           加解密相关函数
 *****************************************/

static int ICACHE_FLASH_ATTR ucds_none_decrypt(void *s, u_int8_t *data, int len)
{
	return len;
}

static int ICACHE_FLASH_ATTR ucds_none_encrypt(void *s, u_int8_t *data, int len)
{
	return len;
}

static int ICACHE_FLASH_ATTR ucds_aes128_decrypt(void *ses, u_int8_t *data, int len)
{
	ucds_session_t *s = (ucds_session_t *)ses;
	u_int8_t pad;


	if ((len % AES128_EKY_LEN) != 0) {
		SER_ERROR("%s pkt encrypt but encrypt data len=%d\n", s->name, len);
		return -1;
	}
	aes_dec_with_key(data, len, s->key);
	pad = data[len - 1];
	if (pad <= 0 || pad > AES128_EKY_LEN) {
		SER_ERROR("%s pkt encrypt but pad len=%d\n", s->name, pad);
		return -2;
	}
	len -= pad;

	return len;
}

static int ICACHE_FLASH_ATTR ucds_aes128_encrypt(void *ses, u_int8_t *data, int len)
{
	u_int8_t pad;
	ucds_session_t *s = (ucds_session_t *)ses;

	pad = AES128_EKY_LEN - (len & (AES128_EKY_LEN - 1));
	memset(data + len, pad, pad);
	len += pad;

	aes_enc_with_key(data, len, s->key);

	return len;
}


int ICACHE_FLASH_ATTR ucds_enc_pkt(ucds_session_t *s, pkt_t *pkt)
{
	int n;
	ucph_t *hdr;

	n = s->enc(s, pkt->data + ucph_hdr_plain_size, pkt->total - ucph_hdr_plain_size);
	if (n < 0) {
		SER_ERROR("%s encypt pkt failed, cmd=%u, total=%u\n", s->name, pkt->cmd, pkt->total);
		return RS_ERROR;
	}

	pkt->total = n + ucph_hdr_plain_size;
	hdr = (ucph_t *)pkt->data;
	hdr->encrypt = (s->enc != ucds_none_encrypt);

	return RS_OK;
}



/***************************
	DIE
 ***************************/

/*
	释放整个队列的所有报文
*/
static void ICACHE_FLASH_ATTR ucds_reset_send(ucds_session_t *s)
{
	pkt_t *pkt, *next;

	stlc_list_for_each_entry_safe(pkt_t, pkt, next, &s->send_list, link) {
		stlc_list_del(&pkt->link);
		pkt_free(pkt);
	}


	if (s->last_pkt) {
		pkt_free(s->last_pkt);
		s->last_pkt = NULL;
	}

	s->send_retry = 0;
}


static void ICACHE_FLASH_ATTR ucds_session_free(ucds_session_t *s)
{
	iw_timer_delete(s->t_die);
	iw_timer_delete(s->t_send);
	ucds_reset_send(s);

	if (s->link.next != NULL) {
		stlc_list_del(&s->link);
	}

	s->is_used = false;
	s->debug = false;
	s->ir_sound_push = 0;

	s->my_request_id = 0;
	s->peer_request_id = 0;
	s->my_last_request_id = 0;
	s->peer_last_request_id = 0;
	s->need_push = false;
	s->is_recv_udp = false;
}



/*****************************************
            会话管理相关函数
 *****************************************/

static int ICACHE_FLASH_ATTR ucds_die(void  *cp)
{
	ucds_session_t *s = (ucds_session_t *)cp;

	if (s->status == UCSS_DIE) {
		SER_DEBUG("%s delete now.\n", s->name);
		ucds_session_free(s);
	} else {
		ucds_set_status(s, UCSS_DIE);
	}

	return 0;
}

static u_int32_t ICACHE_FLASH_ATTR ucds_sid_alloc()
{
	int i;
	u_int32_t sid;
	ucds_session_t *s;

	do {
		sid = ucd.next_cleint_sid;
		ucd.next_cleint_sid++;
		if (ucd.next_cleint_sid > UC_SID_DEV_RESV_END)
			ucd.next_cleint_sid = UC_SID_DEV_RESV_BEGIN;

		for (i = 0; i < MAX_UCDS_SESSION; i++) {
			s = &ucd.sessions[i];
			if (s->is_used && s->client_sid == sid) {
				sid = UC_SID_INVALID;
				break;
			}
		}
	} while (sid == UC_SID_INVALID);

	return sid;
}



// 分配一个会话。需要检查本地的会话数满了没
static ucds_session_t *ICACHE_FLASH_ATTR ucds_session_alloc(struct espconn *conn, u_int32_t csid, u_int32_t dsid, uc_auth_request_t *ar)
{
	ucds_session_t * s;
	u_int32_t rand2;
	int i;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];
		if ( ! s->is_used )
			break;
	}
	if (i == MAX_UCDS_SESSION) {
		SER_INFO("alloc session failed: exceed max=%d\n", MAX_UCDS_SESSION);
		return NULL;
	}

	memset(s, 0, sizeof(ucds_session_t));
	s->is_used = true;
	s->t_die = TID_UCDS_DIE(i);
	s->t_send = TID_UCDS_SEND(i);

	if (csid == 0) {
		s->client_sid = ucds_sid_alloc();
	} else {
		s->client_sid = csid;
	}


	// 回话来自客户端
	s->ucd_session_type = UCD_ST_CLIENT;


	s->status = UCSS_QUESTION;
	s->conn = conn;
	os_sprintf(s->name, "'UCDS[session-%08x]'", s->client_sid);

	iw_timer_set(s->t_die, TIME_N_SECOND(DFL_TIME_UCS_HALF_DIE), 0, ucds_die, (void *)s);

	os_memcpy(s->uuid, ar->my_uuid, sizeof(s->uuid));
	os_memcpy(s->rand1, ar->rand1, sizeof(s->rand1));

	if (dsid == UC_SID_INVALID) {
		s->is_lan = true;
	}
	s->is_front = true;
	if (s->is_lan) {
		s->time_param = &ucd.client.time_param.lan;
	} else {
		s->time_param = &ucd.client.time_param.wan_front;
	}

	srand(get_sec());
	rand2 = rand();
	os_memcpy(&s->rand2, &rand2, sizeof(s->rand2));

	s->enc = ucds_none_encrypt;
	s->dec = ucds_none_decrypt;

	STLC_INIT_LIST_HEAD(&s->send_list);

	SER_DEBUG("create new session %s, client_sid=0x%08x\n", s->name, s->client_sid);

	return s;
}


// 根据客户端的UUID查找会话
static ucds_session_t * ICACHE_FLASH_ATTR ucds_session_lookup_by_uuid(u_int8_t *uuid)
{
	int i;
	ucds_session_t *s;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];
		if (s->is_used && memcmp(s->uuid, uuid, sizeof(s->uuid)) == 0)
			return s;
	}

	return NULL;
}


// 根据会话ID查找会话
static ucds_session_t *ICACHE_FLASH_ATTR ucds_session_lookup(u_int32_t client_sid)
{
	int i;
	ucds_session_t *s;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];
		if (s->client_sid == client_sid)
			return s;
	}

	return NULL;
}


void ICACHE_FLASH_ATTR mem_dump(char *msg, unsigned char *ptr, int len)
{
#if (DEBUG_LEVEL > 2)
	int i;

	if(NULL == ptr){
		SER_ERROR("mem_dump SER_ERROR\n");
		return;
	}

	SER_DEBUG("%s:", msg);
	for(i = 0; i < len; i++) {
		if (i % 16 == 0) {
			SER_DEBUG("\n");
		}
		SER_DEBUG("%02x ", ptr[i]);
	}
	SER_DEBUG("\n");
#endif
}

/***************server*******************************/
int ICACHE_FLASH_ATTR _uc_put_elemet(u_int8_t *buf, int size, int *pos, u_int16_t object, u_int16_t sub_object, u_int16_t attri, u_int16_t len, u_int8_t *value)
{
	u_int8_t *ptr;
	u_int16_t tmp[4];

	if (*pos + UDP_CTRL_TL_SIZE + len > size) {
		SER_ERROR("1error len\n");
		return -1;
	}
	ptr = (u_int8_t *)(buf + (*pos));

	tmp[0] = htons(object);
	tmp[1] = htons(sub_object);
	tmp[2] = htons(attri);
	tmp[3] = htons(len);
	os_memcpy(ptr, &tmp, sizeof(tmp));
	ptr += sizeof(tmp);

	if (len > 0) {
		os_memcpy(ptr, value, len);
	}
	*pos = (*pos) + UDP_CTRL_TL_SIZE + len;
	return 0;
}

/**
	UDP CTRL 通用报文追加对象，用于对端请求回报文

*/

int ICACHE_FLASH_ATTR uc_put_elemet(u_int8_t *buf, int size, int *pos, u_int16_t object, u_int16_t sub_object, u_int16_t attri, u_int16_t len, u_int8_t *value)
{
	_uc_put_elemet(buf, size, pos, object, sub_object, attri, len , value);

	uci_proc_num++;

	return 0;
}

void ICACHE_FLASH_ATTR uc_do_set_reply(ucp_obj_t *uobj, u_int16_t err)
{
	err = htons(err);
	uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
	              uobj->sub_objct, uobj->attr, sizeof(err), (u_int8_t *)&err);
}


// 应答报文不需要重传,可靠传输链表里的定时发送函数，不需要记录最后一次发送缓存
int ICACHE_FLASH_ATTR ucds_send_pkt_no_cach(ucds_session_t *s, pkt_t *pkt)
{
	int n;

	n = iw_udp_send(s->ip, s->port, (char *)pkt->data, pkt->total, s->conn);
	if (n <= 0) {
		SER_ERROR("%s send pkt failed: ip=%u.%u.%u.%u, port=%d, want=%d, send=%d\n",
		          s->name, IP_SHOW(s->ip), s->port, pkt->total, n);
	} else {
		//默认打开报文收发打印
#ifdef SER_DEBUG_
		ucph_t *hdr;
		hdr = (ucph_t *)pkt->data;
		SER_DEBUG("%s send ip=%u.%u.%u.%u, port=%d, len=%d: cmd=%u, action=%u, req=%d, enc=%d, csid=%08x, dsid=%08x, rid=%u, my_request_id=%u\n",
		          s->name, IP_SHOW(s->ip), s->port, n,
		          pkt->cmd, *((u_int8_t *)(hdr + 1)), hdr->request, hdr->encrypt,
		          ntohl(hdr->client_sid), ntohl(hdr->device_sid), hdr->request_id, s->my_request_id);
#endif
	}

	return 0;
}

// 发送定时器
static int ICACHE_FLASH_ATTR ucds_send_timer(void *cp)
{
	int n;
	pkt_t *pkt;
	ucds_session_t *s = (ucds_session_t *)cp;


	if (s->send_retry >= 3)
		n = 2;
	else
		n = s->send_retry;
	n = s->time_param->retry_100ms[n] * 100;

	iw_timer_set(s->t_send, n, 0, ucds_send_timer, (void*)s);
	s->send_retry++;

	pkt = (pkt_t *)stlc_list_entry(s->send_list.next, pkt_t, link);
	ucds_send_pkt_no_cach(s, pkt);

	return 0;
}


// 请求报文需要重传，一直等到回复为止
int ICACHE_FLASH_ATTR ucds_request_add(ucds_session_t *s, pkt_t *pkt)
{
	ucph_t *hdr;
	if (stlc_list_empty(&s->send_list)) {
		s->send_retry = 0;
		iw_timer_set(s->t_send, 0, 0, ucds_send_timer, (void*)s);
	}

	stlc_list_add_tail(&pkt->link, &s->send_list);

	hdr = (ucph_t *)pkt->data;
	if (s->status == UCSS_ESTABLISH) {
		if(hdr->encrypt) {
			if (ucds_enc_pkt(s, pkt) != RS_OK) {
				stlc_list_del(&pkt->link);
				pkt_free(pkt);
				return RS_ERROR;
			}
		}
		s->my_request_id++;
	}

	return RS_OK;
}


/*
	删除头节点，触发下一节点的发送
*/
static void ICACHE_FLASH_ATTR ucds_request_del(ucds_session_t *s)
{
	pkt_t *pkt;

	if (stlc_list_empty(&s->send_list))
		return;

	pkt = (pkt_t *)stlc_list_entry(s->send_list.next, pkt_t, link);
	stlc_list_del(&pkt->link);
	pkt_free(pkt);

	iw_timer_delete(s->t_send);

	if (stlc_list_empty(&s->send_list)) {
		return;
	}

	s->send_retry = 0;
	iw_timer_set(s->t_send, 0, 0, ucds_send_timer, (void*)s);
}

LOCAL int ICACHE_FLASH_ATTR do_event_action(u_int8_t action)
{
	u_int16_t ev = 0;

	switch(action) {
		case UCA_SET:
			ev = IW_EV_OBJECT_SET;
			break;
		case UCA_GET:
			ev = IW_EV_OBJECT_GET;
			break;
		case UCA_DELETE:
			ev = IW_EV_OBJECT_DELETE;
		default:
			break;
	}

	return ev;
}

LOCAL void ICACHE_FLASH_ATTR do_udp_ctrl_obj(void *s, bool is_from_server, u_int8_t action, ucp_obj_t *uobj, u_int8_t *param)
{
	iw_event_node_t *event;
	u_int16_t ev;
	char *tmp;
	char buf[MAX_UC_PKT_SIZE] = {0};
	iw_event_info_t *upevent = NULL;
	ucds_session_t *ucds_s = (ucds_session_t *)s;

	ev = do_event_action(action);

	if(ev <= 0) {
		SER_ERROR("enter func:%s line:%d \n", __FUNCTION__, __LINE__);
		return;
	}

	event = _iw_event_find(ev);
	if(event) {
		memset(buf, 0, MAX_UC_PKT_SIZE);
		upevent = (iw_event_info_t *)buf;
		upevent->event = ev;
		upevent->id = ucds_s->client_sid;
		upevent->action = action;
		upevent->s = s;
		os_memcpy(&upevent->cu.obj, uobj, sizeof(ucp_obj_t));
		tmp = (char *)(upevent + 1);
		os_memcpy(tmp, param, uobj->param_len);

		event->cb(upevent);
	} else {
		SER_ERROR("error event is NULL\n");
		uc_do_set_reply(uobj, ERR_CMD_INVALID);
		return;

	}
	return;

}

/**
	is_from_server: 是否来自服务器
*/
void ICACHE_FLASH_ATTR do_udp_ctrl_core(void *s, bool is_from_server, u_int8_t action, ucp_obj_t *uobj, u_int8_t *param)
{
	//ERROR("obj=%u, sub=%u, attr=%u, len=%u\r\n", uobj->objct, uobj->sub_objct, uobj->attr, uobj->param_len);
	do_udp_ctrl_obj(s, is_from_server, action, uobj, param);
}

LOCAL int ICACHE_FLASH_ATTR ucds_check_answer(ucds_session_t *s, u_int8_t *answer)
{
	MD5_CTX ctx;
	u_int8_t real[16];

	// MD5(token1 + token2 + password_md5)
	Md5_Init(&ctx);
	Md5_Update(&ctx, s->rand1, sizeof(s->rand1));
	Md5_Update(&ctx, s->rand2, sizeof(s->rand2));
	Md5_Update(&ctx, ucd.passwd_md5, sizeof(ucd.passwd_md5));
	Md5_Final(real, &ctx);

	mem_dump("rand1", s->rand1, sizeof(s->rand1));
	mem_dump("rand2", s->rand2, sizeof(s->rand2));
	mem_dump("passwd_md5", ucd.passwd_md5, sizeof(ucd.passwd_md5));
	mem_dump("real", real, sizeof(real));
	mem_dump("answer", answer, sizeof(real));

	return (memcmp(real, answer, sizeof(real)) == 0 ? RS_OK : RS_ERROR);
}


LOCAL int ICACHE_FLASH_ATTR ucds_select_enc(ucds_session_t *s, u_int16_t support_enc)
{
	MD5_CTX ctx;
	u_int8_t key[16];

	if (ucd.dev_enc == UC_ENC_NONE) {
		s->select_enc = UC_ENC_NONE;
		s->enc = ucds_none_encrypt;
		s->dec = ucds_none_decrypt;
		s->is_enc = false;

		return RS_OK;
	}

	if (support_enc & UC_ENC_AES128) {
		s->select_enc = UC_ENC_AES128;
		s->enc = ucds_aes128_encrypt;
		s->dec = ucds_aes128_decrypt;
		s->is_enc = true;
		// 初始化秘钥, MD5(token1 + token2 + password_md5 + password_md5)
		Md5_Init(&ctx);
		Md5_Update(&ctx, s->rand1, sizeof(s->rand1));
		Md5_Update(&ctx, s->rand2, sizeof(s->rand2));
		Md5_Update(&ctx, ucd.passwd_md5, sizeof(ucd.passwd_md5));
		Md5_Update(&ctx, ucd.passwd_md5, sizeof(ucd.passwd_md5));
		Md5_Final(key, &ctx);

		aes_init_key_buf(key, s->key, sizeof(s->key));

		return RS_OK;
	}

	//  测试阶段都用AES128
	SER_DEBUG("%s bad enc=0x%04x\n", s->name, support_enc);

	return RS_ERROR;
}

static int ICACHE_FLASH_ATTR ucds_check_request_id(ucds_session_t *s, ucph_t *request)
{
	if (request->request_id == s->peer_request_id) {
		if (!ucds_lpc_is_need_send(s)) {
			return RS_OK;
		}
		SER_DEBUG("recv the same pkt request_id=%u\n", request->request_id);
		ucds_lpc_send(s);
		return RS_ERROR;
	} else if (request->request_id == (u_int8_t)(s->peer_request_id + 1)) {
		// good, update to next
		SER_DEBUG("%s update rquest id to %u\n", s->name, request->request_id);
		s->peer_request_id++;
		return RS_OK;
	}

	SER_DEBUG( "%s ignore request pkt: reply id=%u, but local is %u.\n",
	           s->name, request->request_id, s->peer_request_id);

	//这里处理一下，如果客户端直连重启了，服务器这边可能没断，如果收到客户端来的认证命令，则直接切换到认证状态
	if (request->command == CMD_UDP_AUTH) {
		ucds_set_status(s, UCSS_QUESTION);
	}

	return RS_ERROR;
}


static int ICACHE_FLASH_ATTR ucds_do_auth_request(ucds_session_t *s, ucph_t *hdr, uc_auth_request_t *ar)
{
	pkt_t *pkt;
	uc_auth_question_t *aq;

	if(ar->flag_bits & TT_ABLILITY) {
		s->issupport_tt = true;
		SER_INFO("session[%08x] is support tt\n", s->client_sid);
	} else {
		s->issupport_tt = false;
		SER_INFO("session[%08x] is not support tt\n", s->client_sid);
	}
	// byte order
	ar->sn = ntoh_ll(ar->sn);
	//order_uc_time(&ar->time);

	//客户端不可靠时间同步
	ntp_from_phone(&ar->time);
	
	//时区设置
	s->time_zone_for_client = ar->time.timezone;

	// update info
	os_memcpy(s->rand1, ar->rand1, sizeof(s->rand1));

	// send question
	pkt = uc_pkt_new(CMD_UDP_AUTH, sizeof(uc_auth_question_t),
	                 false, false, 0, s->client_sid, ucd.client.device_sid, 0);
	if (pkt == NULL) {
		SER_ERROR("no buffer for packets.\n");
		return RS_ERROR;
	}
	aq = get_ucp_payload(pkt, uc_auth_question_t);
	aq->action = UCA_QUESTION;
	os_memcpy(aq->rand2, s->rand2, sizeof(aq->rand2));

	ucds_send_pkt(s, pkt);
	return RS_OK;
}


static int ICACHE_FLASH_ATTR ucds_do_auth_answer(ucds_session_t *s, ucph_t *hdr, uc_auth_answer_t *aa)
{
	int n;
	pkt_t *pkt;
	uc_auth_result_t *ar;
	uc_tlv_t *tlv;

	if (s->status == UCSS_DIE)
		goto err;

	// 检查是否是克隆机
	if (ucd.is_clone) {
		s->die_err = ERR_CLONE;
		goto err;
	}

	// byte order
	aa->support_enc = ntohs(aa->support_enc);
	order_uc_time(&aa->time);

	// 检查密码。密码必检查，即使之前已经认证通过，避免被欺骗
	if (ucds_check_answer(s, aa->answer) != RS_OK) {
		s->die_err = ERR_PASSWD_INVALID;
		goto err;
	}

	// 检查加密方式
	if (ucds_select_enc(s, aa->support_enc) != RS_OK) {
		s->die_err = ERR_NEED_ENCRPYT;
		goto err;
	}

	// reply good result
	ucds_set_status(s, UCSS_ESTABLISH);
	n = sizeof(uc_auth_result_t)
	    + sizeof(uc_tlv_t) + MAX_VENDER
	    + sizeof(uc_tlv_t) + sizeof(uc_time_param_all_t);

	if(ucd.chip_devinfo[0] != 0) {
		n += sizeof(uc_tlv_t) + (strlen((char *)ucd.chip_devinfo) + 1);
	}

	pkt = uc_pkt_new(CMD_UDP_AUTH, n,
	                 false, false, 0, s->client_sid, ucd.client.device_sid, 0);
	if (pkt == NULL) {
		SER_ERROR("no buffer for packets.\n");
		return RS_ERROR;
	}
	ar = get_ucp_payload(pkt, uc_auth_result_t);

	ar->action = UCA_RESULT;
	ar->select_enc = htons(s->select_enc);
	ar->err_num = ERR_NONE;
	ar->dev_type = ucd.dev_type;
	ar->ext_type = ucd.ext_type;
	ar->login_type = LT_NORMAL;
	ar->net_type = s->is_lan ? NT_DEVICE : NT_SERVER;

	tlv = (uc_tlv_t *)(ar + 1);
	tlv->type = htons(UCT_VENDOR);
	tlv->len = htons(MAX_VENDER);
	os_memcpy(tlv_val(tlv), ucd.vendor, MAX_VENDER);

	tlv = tlv_n_next(tlv);
	tlv->type = htons(UCT_TIME_PARAM);
	tlv->len = htons(sizeof(uc_time_param_all_t));
	os_memcpy(tlv_val(tlv), &ucd.client.time_param_net, sizeof(uc_time_param_all_t));

	if(ucd.chip_devinfo[0] != 0) {
		tlv = tlv_n_next(tlv);
		tlv->type = htons(UCT_CHIP_ATTISTR_PHONE);
		tlv->len = htons((strlen((char *)ucd.chip_devinfo) + 1));
		os_memcpy(tlv_val(tlv), ucd.chip_devinfo, (strlen((char *)ucd.chip_devinfo) + 1));
		SER_INFO("!!!! in result send devinfo[%s] to session[%08x]\n", (char *)tlv_val(tlv), s->client_sid);
	}

	ucds_send_pkt(s, pkt);
	return RS_OK;

err:
	// reply bad result
	ucds_set_status(s, UCSS_DIE);
	pkt = uc_pkt_new(CMD_UDP_AUTH, sizeof(uc_auth_result_t),
	                 false, s->is_enc, 0, s->client_sid, ucd.client.device_sid, 0);
	if (pkt == NULL) {
		SER_ERROR( "no buffer for packets.\n");
		return RS_ERROR;
	}
	ar = get_ucp_payload(pkt, uc_auth_result_t);
	ar->action = UCA_RESULT;
	ar->err_num = htons(s->die_err);
	ar->select_enc = htons(s->select_enc);
	ar->dev_type = ucd.dev_type;
	ar->ext_type = ucd.ext_type;
	ar->login_type = LT_NORMAL;
	ar->net_type = s->is_lan ? NT_DEVICE : NT_SERVER;

	ucds_send_pkt(s, pkt);
	return RS_ERROR;
}

static int ICACHE_FLASH_ATTR ucds_do_auth(ucds_session_t *s, ucph_t *hdr)
{
	uc_auth_request_t *ar;
	int ret = RS_OK;

	if (hdr->param_len < 4) {
		SER_ERROR( "%s auth pkt too short: param len=%u\n", s->name, hdr->param_len);
		return RS_ERROR;
	}
	ar = get_net_ucp_payload(hdr, uc_auth_request_t);
	switch (ar->action) {
		case UCA_REQUEST:
			if (s->status == UCSS_ESTABLISH)
				break;
			UCDS_CHECK_PLEN(s, hdr, sizeof(uc_auth_request_t));
			ret = ucds_do_auth_request(s, hdr, ar);
			break;
		case UCA_ANSWER:
			UCDS_CHECK_PLEN(s, hdr, sizeof(uc_auth_answer_t));
			ret = ucds_do_auth_answer(s, hdr, get_net_ucp_payload(hdr, uc_auth_answer_t));
			break;
		default:
			SER_DEBUG("%s ignore pkt type=auth.%u, status=%d\n", s->name, ar->action, s->status);
			break;
	}
	return ret;
}


/*****************************************************/
static void ICACHE_FLASH_ATTR ucds_do_udp_ctrl(ucds_session_t *s, ucph_t *hdr)
{
	ucp_ctrl_t *uc;
	ucp_ctrl_t *uc_reply;
	ucp_obj_t *uobj;
	pkt_t * pkt = NULL;
	u_int8_t *ptr, *end;


	uc = get_net_ucp_payload(hdr, ucp_ctrl_t);
	ptr = (u_int8_t*)&uc[1];
	end = ptr + hdr->param_len - sizeof(ucp_ctrl_t);

	uci_send_index = 0;
	uci_proc_num = 0;

	s->is_recv_udp = true;
#ifdef HAS_PKT_CLONE
    if (ucd.is_clone_pkt_enable)
    {
    	//接收到的报文克隆份给服务器，明文
    	ucds_clone_send(hdr, hdr->param_len + sizeof(ucph_t), true);
    }
#endif

	for (; ptr < end; ptr += sizeof(ucp_obj_t) + uobj->param_len) {
		uobj = (ucp_obj_t *)ptr;
		ucp_obj_order(uobj);
		do_udp_ctrl_core(s, false, uc->action, uobj, (u_int8_t *)&uobj[1]);
	}

	if(hdr->request == 0) {
		SER_DEBUG("request = 0, no need send a request to client\n");
		return;
	}

	//准备发送
	pkt = uc_pkt_new(CMD_UDP_CTRL, uci_send_index + sizeof(ucp_ctrl_t),
	                 false, s->is_enc, 0, s->client_sid, ucd.client.device_sid, s->peer_request_id);

	if (pkt == NULL) {
		SER_ERROR("no buffer for packets.\n");
		return ;
	}

	uc_reply = get_ucp_payload(pkt, ucp_ctrl_t);
	uc_reply->action = uc->action;
	uc_reply->count = uci_proc_num;
	os_memcpy((u_int8_t *)(uc_reply + 1), uci_send_buf, uci_send_index);

#ifdef HAS_PKT_CLONE
    if (ucd.is_clone_pkt_enable)
    {
    	//将发送给客户端的数据克隆一份发给服务器，明文
    	ucds_clone_send(pkt->data, pkt->total, false);
    }
#endif
	//这里服务器与客户端加密时加密方法来源判断不一样
	if (ucds_enc_pkt(s, pkt) != RS_OK) {
		pkt_free(pkt);
		return;
	}

	ucds_send_pkt(s, pkt);
}


static int ICACHE_FLASH_ATTR ucds_do_ctrl(ucds_session_t *s, ucph_t *hdr)
{
	ucds_do_udp_ctrl(s, hdr);

	return RS_OK;
}



/*****************************************
            接收报文相关函数
 *****************************************/

static void ICACHE_FLASH_ATTR do_keeplive_request(ucds_session_t *s, ucph_t *hdr, uc_keeplive_request_t *request)
{
	int n;
	pkt_t *pkt;
	uc_keeplive_reply_t *reply;
	uc_tlv_t *tlv;

	n = sizeof(uc_keeplive_reply_t)
	    + sizeof(uc_tlv_t) + sizeof(uc_time_param_all_t);

	pkt = uc_pkt_new(CMD_UDP_KEEPLIVE, n,
	                 false, s->is_enc, 0, s->client_sid, ucd.client.device_sid, hdr->request_id);
	if (pkt == NULL) {
		SER_ERROR("no buffer for packets.\n");
		return;
	}
	reply = get_ucp_payload(pkt, uc_keeplive_reply_t);
	reply->action = UCA_REPLY;
	tlv = (uc_tlv_t *)(reply + 1);
	tlv->type = htons(UCT_TIME_PARAM);
	tlv->len = htons(sizeof(uc_time_param_all_t));
	os_memcpy(tlv_val(tlv), &ucd.client.time_param_net, sizeof(uc_time_param_all_t));

	if (ucds_enc_pkt(s, pkt) != RS_OK) {
		pkt_free(pkt);
		return;
	}

	ucds_send_pkt(s, pkt);
}


static int ICACHE_FLASH_ATTR ucds_do_keeplive(ucds_session_t *s, ucph_t *hdr)
{
	uc_keeplive_request_t *request;

	request = get_net_ucp_payload(hdr, uc_keeplive_request_t);
	switch (request->action) {
		case UCA_REQUEST:
			do_keeplive_request(s, hdr, request);
			break;
		case UCA_RESET:
			SER_INFO("%s reset by peer\n", s->name);
			ucds_set_status(s, UCSS_DIE);
			break;
		default:
			SER_DEBUG("%s ignore keeplve.%u\n", s->name, request->action);
			break;
	}

	return RS_OK;
}



LOCAL bool ICACHE_FLASH_ATTR is_auth_request(ucph_t *hdr)
{
	uc_auth_question_t *aq;

	aq = get_net_ucp_payload(hdr, uc_auth_question_t);

	return (ntohs(hdr->command) == CMD_UDP_AUTH && aq->action == UCA_REQUEST);
}

LOCAL void ICACHE_FLASH_ATTR ucds_session_reset(ucds_session_t *s)
{
	iw_timer_set(s->t_die, TIME_N_SECOND(DFL_TIME_UCS_HALF_DIE), 0, ucds_die, (void *)s);
	iw_timer_delete(s->t_send);

	ucds_reset_send(s);

	s->my_request_id = 0;
	s->peer_request_id = 0;
	s->my_last_request_id = 0;
	s->peer_last_request_id = 0;

	s->need_push = false;
	s->is_recv_udp = false;
	s->debug = 0;
	s->ir_sound_push = 0;

	s->status = UCSS_QUESTION;
}

// 获取一个会话。该查找的查找，该申请的申请
static ucds_session_t * ICACHE_FLASH_ATTR ucds_session_get(struct espconn *conn, u_int32_t ip, u_int16_t port, ucph_t *hdr, int len)
{
	ucds_session_t *s;
	u_int32_t csid = hdr->client_sid;

	if (ucph_hdr_len(hdr) < ucph_hdr_size) {
		SER_ERROR("bad hdr len=%d\n", ucph_hdr_len(hdr));
		return NULL;
	}

	if (csid != 0 && (s = ucds_session_lookup(csid)) != NULL) {
		if (s->status == UCSS_ESTABLISH && is_auth_request(hdr)) {
			ucds_session_reset(s);
		}
		return s;
	}

	// 本地新来的连接或者服务器新来的连接
	if (csid == 0
	    || (conn == (struct espconn *)piw_priv->devconn
	        && csid >= UC_SID_SERV_CLIENT_BEGIN
	        && csid <= UC_SID_SERV_CLIENT_END)) {
		uc_auth_request_t *ar;

		if (hdr->encrypt || ntohs(hdr->command) != CMD_UDP_AUTH) {
			SER_ERROR("no cleint session id, but cmd=%u, enc=%d\n", ntohs(hdr->command), hdr->encrypt);
			return NULL;
		}
		if (hdr->param_len < sizeof(uc_auth_request_t)
		    || (unsigned)len < ucph_hdr_len(hdr) + sizeof(uc_auth_request_t)) {
			SER_ERROR("bad pkt len, len=%u, param_len=%u\n", len, hdr->param_len);
			return NULL;
		}

		ar = get_net_ucp_payload(hdr, uc_auth_request_t);
		if (ar->action != UCA_REQUEST) {
			SER_ERROR("no cleint session id, but action=%u\n", ar->action);
			return NULL;
		}

		/*这里处理一种情况，当从服务器透传过来的设备时可能会出现csid与uuid不匹配的怦，
		*原因是，手机到服务器掉线重认证时，会由服务器重新分配csid，透传过来的csid会找不到，
		*设备会回由uuid找到的上个sestion，这里这样处理下，以uuid为准，直接覆盖csid
		*/
		if ((s = ucds_session_lookup_by_uuid(ar->my_uuid)) != NULL) {
			if (s->status == UCSS_QUESTION) {
				SER_ERROR("status=%u, need not del, reuse\n "
				         "client %s reuse by uuid, csid old=%08x, now=%08x, del old session\n",
				         s->status, s->name, s->client_sid, csid);
				/*
					产生这个BUG的过程是:
					1.当手机连接服务器进行认证时，ICE服务器会给手机分配一个client_id.
					2.当手机发送了request请求报文后，设备一直没有回应。再次发送请求，ICE对这个回话判定超时，然后由重新分配client_id.
					3.设备收到了第一个request_id, 然后后面收到的request都用这个id应答，即使新的client_id到来，也会用就的id应答。导致ICE会把应答报文丢到。

					出现这个问题，在ICE抓包现象就是会给手机app的回话ID不对。
				*/
				if(csid != 0){
					s->client_sid = csid;		//修改bug
				}
				
				return s;
			}

			SER_INFO("client %s reuse by uuid, csid old=%08x, now=%08x, del old session\n", s->name, s->client_sid, csid);
			ucds_session_free(s);
			s = NULL;
		}

		if (ntoh_ll(ar->sn) != ucd.sn) {
			SER_ERROR("ignore auth.request: bad sn=%llu\n", ntoh_ll(ar->sn));
			return NULL;
		}

		return ucds_session_alloc(conn, csid, hdr->device_sid, ar);
	}

	ucds_send_reset(conn, ip, port, csid);

	return NULL;
}

static int ICACHE_FLASH_ATTR ucds_do_notify(ucds_session_t *s, ucph_t *hdr)
{
	// TODO:
	return RS_OK;
}


static int ICACHE_FLASH_ATTR ucds_check_reply_id(ucds_session_t *s, ucph_t *reply)
{
	pkt_t *pkt;
	ucph_t *request;

	if (stlc_list_empty(&s->send_list)) {
		SER_DEBUG( "%s ignore reply pkt: no request in send list.\n", s->name);
		return RS_ERROR;
	}

	pkt = (pkt_t *)stlc_list_entry(s->send_list.next, pkt_t, link);
	request = (ucph_t *)(pkt + 1);
	if (request->request_id != reply->request_id) {
		SER_DEBUG("%s ignore reply pkt: my request id=%u, but %u in pkt.\n",
		          s->name, request->request_id, reply->request_id);
		return RS_ERROR;
	}
	//这里处理一下，如果客户端直连重启了，服务器这边可能没断，如果收到客户端来的认证命令，则直接切换到认证状态
	if (request->command == CMD_UDP_AUTH) {
		ucds_set_status(s, UCSS_QUESTION);
	}

	return RS_OK;
}



/*
	解密，并检查长度是否合法
*/
static int ICACHE_FLASH_ATTR ucds_decrypt_and_check(ucds_session_t *s, ucph_t *hdr, int len)
{
	int n;

	if ( ! hdr->encrypt ) {
		hdr->command = ntohs(hdr->command);
		hdr->param_len = ntohs(hdr->param_len);
		goto check;
	}

	n = s->dec(s, (u_int8_t*)BOFP(hdr, ucph_hdr_plain_size), len - ucph_hdr_plain_size);
	if (n < 0)
		return RS_ERROR;

	len = n + ucph_hdr_plain_size;
	hdr->command = ntohs(hdr->command);
	hdr->param_len = ntohs(hdr->param_len);

check:
	if (ucph_hdr_len(hdr) < ucph_hdr_size || len != ucph_hdr_len(hdr) + hdr->param_len) {
		SER_INFO("%s Drop bad packet: len=%d, hdr_len=%d, param_len=%d\n",
		         s->name, len, ucph_hdr_len(hdr), hdr->param_len);
		return RS_ERROR;
	}

	return RS_OK;
}

static void ICACHE_FLASH_ATTR ucds_update_die(ucds_session_t *s)
{
	iw_timer_set(s->t_die, TIME_N_SECOND(s->time_param->die), 0, ucds_die, (void *)s);
}


static void ICACHE_FLASH_ATTR ucds_question_into(ucds_session_t *s)
{
	// do nothing
}

static void ICACHE_FLASH_ATTR ucds_estab_into(ucds_session_t *s)
{
	ucds_update_die(s);
	ucds_lpc_clean(s);
	ucds_lpc_enable(s, false);
}

static void ICACHE_FLASH_ATTR ucds_estab_proc(ucds_session_t *s, ucph_t *hdr)
{
	ucds_update_die(s);


	// process request packet
	if (hdr->request) {
		ucds_estab_do_request(s, hdr);
		return;
	}

	/*
		bellow process reply packet
	*/
	if (ucds_check_reply_id(s, hdr) != RS_OK) {
		return;
	}

	switch (hdr->command) {
		case CMD_UDP_NOTIFY:
			ucds_do_notify(s, hdr);
			break;
		case CMD_UDP_CTRL:
			break;
		default:
			break;
	}

	// 删除掉已经应答的报文
	ucds_request_del(s);

}


static void ICACHE_FLASH_ATTR ucds_die_into(ucds_session_t *s)
{
	iw_timer_delete(s->t_send);
	ucds_reset_send(s);
	s->debug = 0;

	// 延迟几秒删除
	iw_timer_set(s->t_die, TIME_N_SECOND(DFL_TIME_UCS_DEL_DELAY), 0, ucds_die, (void *)s);
}

static void ICACHE_FLASH_ATTR ucds_die_proc(ucds_session_t *s, ucph_t *hdr)
{
	switch (hdr->command) {
		case CMD_UDP_AUTH:
			//这里处理一下，如果客户端直连重启了，服务器这边可能没断，如果收到客户端来的认证命令，则直接切换到认证状态
			ucds_session_free(s);
			ucds_set_status(s, UCSS_QUESTION);
			break;
		default:
			ucds_send_reset(s->conn, s->ip, s->port, s->client_sid);
			break;
	}
}


ucs_proc_t ucds_proc[UCSS_MAX] = {
	{"QUESTION", ucds_question_into, ucds_question_proc},
	{"ESTABLISH", ucds_estab_into, ucds_estab_proc},
	{"DIE", ucds_die_into, ucds_die_proc},
};

static void ICACHE_FLASH_ATTR ucds_set_status(ucds_session_t *s, int status)
{
	int prev_satus = s->status;

	if (status >= UCSS_MAX) {
		SER_INFO("ucds_set_status unknow new status = %d\n", status);
		return;
	}
	prev_satus = prev_satus;  //解决编译警告
	s->status = status;
	SER_ERROR("'%s' modify status from %s to %s\n",
	          s->name, ucds_proc[prev_satus].name, ucds_proc[status].name);

	ucds_proc[status].on_into(s);
}


static void ICACHE_FLASH_ATTR ucds_question_proc(ucds_session_t *s, ucph_t *hdr)
{
	switch (hdr->command) {
		case CMD_UDP_AUTH:
			ucds_do_auth(s, hdr);
			break;
		default:
			SER_DEBUG("%s status %s ignore pkt cmd=%u\n", s->name, ucds_proc[s->status].name, hdr->command);
			break;
	}
}

static RS ICACHE_FLASH_ATTR ucds_do_debug(ucds_session_t *s, ucph_t *hdr)
{
	pkt_t *pkt;
	u_int8_t *on = NULL;

	if (!s || !hdr) {
		return RS_ERROR;
	}

	on = get_net_ucp_payload(hdr, u_int8_t);

	if (*on == 0) {
		SER_DEBUG("net debug is close\n");
	} else {
		SER_DEBUG("net debug is open\n");
	}
	s->debug = *on;

	//用hdr->flag来表示到底是回的调试开关命令响应包还是普通push包。flag为0表示响应包，为1表示push包。
	pkt = uc_pkt_new(CMD_USER_DEBUG, 1,
	                 false, s->is_enc, 0, s->client_sid, ucd.client.device_sid, hdr->request_id);
	if (!pkt) {
		SER_ERROR("malloc failed\n");
		return RS_ERROR;
	}
	if (ucds_enc_pkt(s, pkt) != RS_OK) {
		pkt_free(pkt);
		return RS_ERROR;
	}

	ucds_send_pkt(s, pkt);

	return RS_OK;
}


static void ICACHE_FLASH_ATTR ucds_estab_do_request(ucds_session_t *s, ucph_t *hdr)
{
	if (ucds_check_request_id(s, hdr) != RS_OK)
		return;
	//如果走到这里，表示是另一个包了
	ucds_lpc_clean(s);
	ucds_lpc_enable(s, true);

	switch (hdr->command) {
		case CMD_UDP_CTRL:
			ucds_do_ctrl(s, hdr);
			break;
		case CMD_UDP_KEEPLIVE:
			ucds_do_keeplive(s, hdr);
			break;
		case CMD_UDP_NOTIFY:
			break;
		case CMD_UDP_BIND_PHONE:
			break;
		case CMD_UDP_AUTH:
			/* 判断下peer_request_id，适当避免下伪造身份 */
			if (s->peer_request_id == 0) {
				ucds_do_auth(s, hdr);
			}
			break;
		case CMD_USER_DEBUG:
			// 开关调试
			ucds_do_debug(s, hdr);
			SER_INFO("set debug, client_sid = %u, debug = %u", s->client_sid, s->debug);
			break;
		default:
			SER_INFO("%s at %s ignore reqeust pkt cmd=%u\n", s->name, ucds_proc[s->status].name, hdr->command);
			break;
	}

	s->peer_last_request_id = s->peer_request_id;

}


void ICACHE_FLASH_ATTR ucds_proc_pkt(struct espconn *conn, u_int8_t *data, int len)
{
	ucph_t *hdr;
	ucds_session_t *s;
	u_int32_t ip;
	u_int16_t port;
	u_int32_t *tip = (u_int32_t *)conn->proto.udp->remote_ip;

	hdr = (ucph_t *)data;
	hdr->client_sid = ntohl(hdr->client_sid);
	hdr->device_sid = ntohl(hdr->device_sid);

	/* Host sequence */
	port = conn->proto.udp->remote_port;
	ip = *tip;
	ip = ntohl(ip);

	if ((s = ucds_session_get(conn, ip, port, hdr, len)) == NULL) {
		SER_INFO("UCDS drop pkt: get session failed.client_sid:%d\n",
		         hdr->client_sid);
		if (hdr->client_sid != 0) {
			SER_ERROR("UCDS reset packet\r\n");
			ucds_send_reset(conn, ip, port, hdr->client_sid);
		}
		return;
	}

	if (!s->is_used) {
		s->is_used = true;
	}

	if (ucds_decrypt_and_check(s, hdr, len) != RS_OK) {
		SER_INFO("UCDS drop pkt: ucds_decrypt_and_check failed.\n");
		return;
	}

	// 及时更新，避免客户端网络环境变化
	ucds_try_update_ip(s, ip, port);

	//默认打开报文收发打印
	SER_DEBUG("%s recv from %u.%u.%u.%u:%u: cmd=%u, action=%u, param_len=%u, csid=%08x, dsid=%08x, request id=%u(is %s)s->status=%d\n",
	          s->name, IP_SHOW(ip), port, hdr->command, *((u_int8_t *)(hdr + 1)), hdr->param_len,
	          hdr->client_sid, hdr->device_sid, hdr->request_id, hdr->request ? "request" : "reply", s->status);

	// 处理报文
	ucds_proc[s->status].proc_pkt(s, hdr);
}

LOCAL void ICACHE_FLASH_ATTR
iw_lan_srv_recv(void *arg, char *pusrdata, unsigned short length)
{
	//ucph_t *hdr;
	struct espconn *conn;
	remot_info *premot = NULL;
	sint8 ret = ESPCONN_OK;

	if(length < (u_int16_t)ucph_hdr_size || pusrdata == NULL || !arg) {
		SER_ERROR("iw_lan_srv_recv err lenth:%d pusrdata:%p\r\n", length, pusrdata);
		return;
	}

	conn = (struct espconn *)arg;
	os_memcpy(ucd_rcv_buf, pusrdata, length);

	if ((ret = espconn_get_connection_info(conn, &premot, 0)) == ESPCONN_OK) {
		conn->proto.udp->remote_port = premot->remote_port;
		conn->proto.udp->remote_ip[0] = premot->remote_ip[0];
		conn->proto.udp->remote_ip[1] = premot->remote_ip[1];
		conn->proto.udp->remote_ip[2] = premot->remote_ip[2];
		conn->proto.udp->remote_ip[3] = premot->remote_ip[3];
		SER_DEBUG("%s recv from ip %u.%u.%u.%u port:%d\n", __FUNCTION__,
		          conn->proto.udp->remote_ip[0],
		          conn->proto.udp->remote_ip[1],
		          conn->proto.udp->remote_ip[2],
		          conn->proto.udp->remote_ip[3],
		          conn->proto.udp->remote_port);
		ucds_proc_pkt(conn, (u_int8_t *)ucd_rcv_buf, length);
	} else {
		SER_ERROR("iw_lan_srv_recv get premot ip and port failed ret(%d)\n", ret);
	}

}

rs_t ICACHE_FLASH_ATTR iw_init_lan_udp(iw_priv_t *piw)
{
	struct espconn *conn;

	conn  = iw_udp_server_create(0, DFL_UDP_CTRL_CLIENT_LAN_PORT, iw_lan_srv_recv, iw_lan_srv_sendback);
	piw->srvconn = (void *)conn;
	return RS_OK;
}

int ICACHE_FLASH_ATTR delay_wifi_reset_timer(void * t)
{
	struct station_config stcfg;
	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;

	wifi_station_disconnect();
	wifi_station_get_config(&stcfg);
	SER_ERROR("old ssid:%s, pwd:%s\n", stcfg.ssid, stcfg.password);
	os_memcpy((uint8_t *)stcfg.ssid, (uint8_t *)info->ssid, IW_MAX_WIFI_SSID);
	os_memcpy((uint8_t *)stcfg.password, (uint8_t *)info->passwd, IW_MAX_WIFI_PASSWD);
	SER_ERROR("new ssid:%s, pwd:%s\n", stcfg.ssid, stcfg.password);
	wifi_station_set_config(&stcfg);
	wifi_station_connect();
	return 0;
}

void ia_do_wifi_set(iw_event_info_t *info, iw_obj_t *uobj)
{
	wifi_tlv_t *ptlv = NULL;
	int n = 0;
	char ssid[IW_MAX_WIFI_SSID] = {0};
	char passwd[IW_MAX_WIFI_PASSWD] = {0};
	iw_sys_info_t *wificfg = (iw_sys_info_t *)&piw_priv->iw_sys;
	u_int16_t err = ERR_NONE;
	int limit_len = 0;
	cf_wifi_t cf_wifi;
	u_int16_t para_len = 0, total_len = 0;
	u_int8_t action = 0;
	u_int8_t *param = NULL;

	if(!info) {
		return;
	}
	action = info->action;
	param = (u_int8_t *)(info + 1);


	SER_INFO("enter functiong:%s ACCTION:%s\n", __FUNCTION__,
	         (action == UCA_GET) ? "get" : "set");

	if (!uobj || !param) {
		SER_ERROR("Param is NULL");
		err = ERR_PARAM_INVALID;
		goto sendreply;
	}

	ptlv = (wifi_tlv_t *)param;
	total_len = uobj->param_len;

	if (action == UCA_GET) {
		//not support
		err = ERR_PARAM_INVALID;
		goto sendreply;
	} else if (action == UCA_SET) {
		if(total_len < SET_WIFI_TLV_COUNT * sizeof(wifi_tlv_t)) {
			err = ERR_PARAM_INVALID;
			goto sendreply;
		}

		while (total_len >= sizeof(wifi_tlv_t)) {
			para_len = ntohs(ptlv->length);

			switch(ntohs(ptlv->type)) {
				case SET_WIFI_SSID:
					//ssid最大保存长度是33字节
					if (para_len) {
						limit_len = para_len < sizeof(ssid) ? para_len : sizeof(ssid);
						os_memcpy((u_int8_t *)ssid, (u_int8_t  *) & (ptlv[1]),	limit_len);
					} else {
						SER_ERROR("Ssid length is 0");
						err = ERR_PARAM_INVALID;
						goto sendreply;
					}
					ptlv = (wifi_tlv_t *)(param + para_len + sizeof(wifi_tlv_t));
					total_len -= total_len - para_len - sizeof(wifi_tlv_t);
					n++;
					break;
				case SET_WIFI_PW:
					if (para_len) {
						limit_len = para_len < sizeof(passwd) ? para_len : sizeof(passwd);
						os_memcpy((u_int8_t  *)passwd, (u_int8_t  *) & (ptlv[1]), limit_len);
					} else {
						passwd[0] = '\0';
					}
					ptlv = (wifi_tlv_t *)(param + para_len + sizeof(wifi_tlv_t));
					total_len -= total_len - para_len - sizeof(wifi_tlv_t);
					n++;
					break;
				default:
					SER_ERROR("Worng type %u", ntohs(ptlv->type));
					err = ERR_PARAM_INVALID;
					goto sendreply;
			}
			//因为这个参数是长度可变的，用n的值来判断获取属性的个数，而不只是长度来判断
			if (n >= SET_WIFI_TLV_COUNT) {
				break;
			}
		}
	} else {
		SER_ERROR("Worng acction %d", action);
		err = ERR_PARAM_INVALID;
		goto sendreply;
	}

	SER_INFO("set ssid:%s pw:%s\n", ssid, passwd);

	if (ssid[0] != '\0' ) {
		os_memcpy((uint8_t *)cf_wifi.ssid, (uint8_t *)ssid, IW_MAX_WIFI_SSID);
		os_memcpy((uint8_t *)wificfg->ssid, (uint8_t *)ssid, IW_MAX_WIFI_SSID);

		os_memcpy((uint8_t *)cf_wifi.passwd, (uint8_t *)passwd, IW_MAX_WIFI_PASSWD);
		os_memcpy((uint8_t *)wificfg->passwd, (uint8_t *)passwd, IW_MAX_WIFI_PASSWD);

		iw_request_event(EVENT_SYS_WIFI_CONFIG);
		//主要是让设备先回复手机，然后在切换wifi
		iw_timer_set(TID_UCDC_DELAY_RST_WIFI, TIME_N_SECOND(1), 0,
		             delay_wifi_reset_timer, NULL);
		SER_INFO("User set succed and exit now");
	}

sendreply:
	uc_do_set_reply((ucp_obj_t *)uobj, err);
}


void ia_do_public(iw_event_info_t *info, iw_obj_t *uobj)
{
	SER_DEBUG("%u\n", uobj->sub_objct);
	switch(uobj->attr) {
		case UCAT_IA_PUBLIC_WIFI_SET:
			ia_do_wifi_set(info, uobj);
			break;
		default:
			SER_DEBUG("ia_do_public error sub_objct=%u\n", uobj->sub_objct);
			uc_do_set_reply((ucp_obj_t *)uobj, ERR_CMD_INVALID);
			break;
	}
}


rs_t ICACHE_FLASH_ATTR iw_do_obj_ctrl(iw_event_info_t *info, iw_obj_t *uobj)
{
	switch (uobj->objct) {
		case UCOT_SYSTEM:
			SER_DEBUG("--------do_ucot_system-------------\n");
			do_ucot_system(info, uobj);
			break;
		case UCOT_IA:
			SER_DEBUG("--------do_ucot_ia-------------\n");
			do_ucot_ia_obj(info, uobj);
			break;
		default:
			SER_ERROR("error uobj->objct=%u\n", uobj->objct);
			uc_do_set_reply((ucp_obj_t *)uobj, ERR_CMD_INVALID);
			break;
	}
	return RS_OK;
}

rs_t ICACHE_FLASH_ATTR iw_lan_srv_init(iw_priv_t *piw)
{
	iw_init_lan_udp(piw);
	return RS_OK;
}


static void ICACHE_FLASH_ATTR session_reset_send(ucds_session_t *s)
{
	ucds_send_reset(s->conn, s->ip, s->port, s->client_sid);
	ucds_set_status(s, UCSS_DIE);
}

void ICACHE_FLASH_ATTR other_session_reset(ucds_session_t *ds)
{
	int i;
	ucds_session_t *s;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];
		if (s != ds &&
		    s->is_used &&
		    s->status == UCSS_ESTABLISH) {
			SER_INFO("reset send\n");
			session_reset_send(s);
		}
	}

	return;
}

void ICACHE_FLASH_ATTR other_client_session_reset(void)
{
	int i;
	ucds_session_t *s;

	for (i = 0; i < MAX_UCDS_SESSION; i++) {
		s = &ucd.sessions[i];

		if (s->is_used &&
		    s->status == UCSS_ESTABLISH) {
			SER_INFO("reset send\n");
			session_reset_send(s);
		}
	}

	return;
}




