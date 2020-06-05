/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: ijcfg
**  File:    ijcfg.c
**  Author:  liubenlong
**  Date:    07/28/2014
**
**  Purpose:
**    Ijcfg.
**************************************************************************/
/* Include files. */
#include "c_types.h"
#include "iw_priv.h"
#include "iw_ijcfg.h"
#include "iw_dev.h"
#include "iw_smartconfig.h"
#include "iw_func_support.h"
#include "iw_linkage_conf.h"

//#define IJ_DEBUG_
#ifdef IJ_DEBUG_
#define IJ_DEBUG(x...) DEBUG(x)
#define IJ_INFO(x...) INFO(x)
#define IJ_ERROR(x...) ERROR(x)
#else
#define IJ_INFO(x...)  
#define IJ_DEBUG(x...) 
#define IJ_ERROR(x...)  ERROR(x)
#endif


/* Macro constant definitions. */
#define IJCFG_PUSH_TIMER			(3)
#define IJCFG_PUSH_FAST_TIMER		(1)
#define	SN_BROCAST	0

#define IJCFG_PUSH_FAST_MAX_NUM		(60)
#define HAS_SMARTCFG 0

extern ucd_t ucd;
#define MY_SN	ucd.sn
#define MY_DEV_TYPE	ucd.dev_type
#define MY_DEV_EXT_TYPE ucd.ext_type

ijcfg_t ijcfg;

static void ICACHE_FLASH_ATTR ijcfg_order_header(cfg_pkt_hdr_t *hdr)
{
	if (NULL == hdr)
		return;
	
	hdr->src_sn = ntoh_ll(hdr->src_sn);
	hdr->dst_sn = ntoh_ll(hdr->dst_sn);
	hdr->cmd = ntohs(hdr->cmd);
	hdr->req_id = ntohs(hdr->req_id);
	hdr->param_len = ntohs(hdr->param_len);
}

static u_int32_t ICACHE_FLASH_ATTR clis_get_id()
{
	static u_int32_t req_id = 0;
	IJ_DEBUG("enter func:%s\n",__FUNCTION__);

	if (++req_id == 0) {
		req_id = 1;
	}

	return req_id;
}

static void ICACHE_FLASH_ATTR clis_send_ihere(void *arg, u_int64_t dst_sn, u_int32_t req_id)
{
	cfg_pkt_hdr_t *hdr;
	cfgp_i_here_t *i_here;
	u_int16_t param_len = 0;
	uc_tlv_t *tlv;
	u_int32_t *smart_cfg_time = NULL, cfg_time = get_smcf_time();
	struct espconn *conn = NULL;
#ifdef LINKAGE_SUPPORT
	u_int32_t home_id;
#endif	
	IJ_DEBUG("enter func:%s\n",__FUNCTION__);

	conn = (struct espconn *)arg;

	param_len = sizeof(cfgp_i_here_t);
	if(ucd.chip_devinfo[0] != 0){
		param_len += sizeof(uc_tlv_t) + (os_strlen((char *)ucd.chip_devinfo) + 1);
	}	

#ifdef LINKAGE_SUPPORT
	//带home_id	
	param_len += sizeof(uc_tlv_t) + sizeof(u_int32_t);
#endif

	// 带一键配置成功的时间
	param_len += sizeof(uc_tlv_t) + sizeof(u_int32_t);
	
	hdr = (cfg_pkt_hdr_t *)ijcfg.buff;
	i_here = (cfgp_i_here_t *)(hdr + 1);
	os_memset(hdr, 0, ijcfg.buff_len);

	hdr->src_sn = ntoh_ll(MY_SN);
	hdr->dst_sn = ntoh_ll(dst_sn);
	hdr->cmd = htons(CFGPT_I_HERE);
	hdr->param_len = htons(param_len);
	hdr->req_id = htons(req_id);
	i_here->config_count = ntohs(10);	
	hdr->flags |= CFGPF_MASTER_SUPPORT;
	hdr->flags |= CFGPF_UDP_CTRL;
	hdr->flags = htons(hdr->flags);


	i_here->type = TP_DS007; 
	i_here->sub_type = MY_DEV_TYPE;
	i_here->ext_type = MY_DEV_EXT_TYPE;
	i_here->sys_mode = ntohs(SYS_MODE_MASTER);

	
	tlv = (uc_tlv_t *)(i_here + 1);

	tlv->type = htons(UCT_SMARTCFG_TIME);
	tlv->len = htons(4);
	smart_cfg_time = (u_int32_t *)tlv_val(tlv);

	if (0 == cfg_time || cfg_time > 120){
		*smart_cfg_time = htonl(0xFFFFFFFF);
	} else {
		*smart_cfg_time = htonl(cfg_time);
	}

#ifdef LINKAGE_SUPPORT
	home_id = get_home_id();
	home_id = htonl(home_id);
	tlv = tlv_n_next(tlv);
	tlv->type = htons(UCT_DEV_NEW_LA_SUPPORT);
	tlv->len = htons(sizeof(u_int32_t));
	os_memcpy(tlv_val(tlv), &home_id, sizeof(home_id));	
#endif
	
	espconn_sendto(conn, ijcfg.buff,  sizeof(cfg_pkt_hdr_t) + param_len);
}

static void ICACHE_FLASH_ATTR cli_proc_pkt(cfg_pkt_hdr_t * pkt, void *arg)
{
	u_int32_t req_id;
	IJ_DEBUG("enter func:%s\n",__FUNCTION__);

	if (pkt->dst_sn != MY_SN && pkt->dst_sn != SN_BROCAST) {
		IJ_DEBUG("pkt->dst_sn %lu %lu\n",
		((u_int32_t*)&pkt->dst_sn)[0],((u_int32_t*)&pkt->dst_sn)[1]);
		IJ_DEBUG("my_sn %lu %lu\n",
		((u_int32_t*)&ucd.sn)[0],((u_int32_t*)&ucd.sn)[1]);
		return ;
	}

	req_id = clis_get_id();
	if (pkt->cmd == CFGPT_DISCOVERY) {
		clis_send_ihere(arg, pkt->src_sn, req_id);
	}
}

static void ICACHE_FLASH_ATTR ijcfg_read(void *arg, char *pdata, unsigned short length)
{
	cfg_pkt_hdr_t *pkt;
	struct espconn *conn = NULL;
	remot_info *premot = NULL;
	
	IJ_DEBUG("into ijcfg_read lenth:%u\n", length);
	
	if(pdata == NULL || !arg)
		return;

	os_memcpy(ijcfg.buff, pdata, length);
	pkt = (cfg_pkt_hdr_t *)ijcfg.buff;
	conn = arg;
	
	ijcfg_order_header(pkt);

	if (length < (pkt->param_len + sizeof(cfg_pkt_hdr_t))){
		IJ_DEBUG("Recv bad pkt: param_len=%d, but n=%d\n", pkt->param_len, length);
		return ;
	}
	
	if (espconn_get_connection_info(conn,&premot,0) == ESPCONN_OK){
		 conn->proto.udp->remote_port = premot->remote_port;
		 conn->proto.udp->remote_ip[0] = premot->remote_ip[0];
		 conn->proto.udp->remote_ip[1] = premot->remote_ip[1];
		 conn->proto.udp->remote_ip[2] = premot->remote_ip[2];
		 conn->proto.udp->remote_ip[3] = premot->remote_ip[3];
		 IJ_DEBUG("%s recv from ip %u.%u.%u.%u port:%d\n", __FUNCTION__,
		   	conn->proto.udp->remote_ip[0],
		   	conn->proto.udp->remote_ip[1],
		   	conn->proto.udp->remote_ip[2],
		   	conn->proto.udp->remote_ip[3],
		   	conn->proto.udp->remote_port);
	} else {
		IJ_ERROR("%s get romote ip and port fialed\n",__FUNCTION__);
		return;
	}

	cli_proc_pkt(pkt, arg);
	
	return ;
}
void  ICACHE_FLASH_ATTR iw_ijcfg_sendback(void *arg)
{
        //os_printf("send ok\n");
}

int  ICACHE_FLASH_ATTR iw_init_ijcfg(iw_priv_t *piw)
{	
	struct espconn *conn = NULL;

	if(NULL == piw){
		IJ_ERROR("iw_init_ijcfg iw_init_ijcfg failed \r\n");
		return RS_ERROR;
	}

	conn = iw_udp_server_create(0, IJCFG_PORT_DEV, ijcfg_read, iw_ijcfg_sendback);
	if(conn == NULL) {
		IJ_ERROR("iw_init_ijcfg udp_server_create failed \r\n");
		return RS_ERROR;
	}
    conn->reverse = (void *)&ijcfg;
	piw->ijcfgconn = (void*)conn;
             
	ijcfg.buff = (u_int8_t *)ucd_rcv_buf;
	ijcfg.buff_len = MAX_UC_PKT_SIZE;
	
	ijcfg.init = true;

	IJ_DEBUG("iw_init_ijcfg is init\n");

	return RS_OK;
}


