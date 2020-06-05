/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    iiw_smartconfig.c
**  Author:  fangfuhan
**  Date:    04/05/2016
**
**  Purpose:
**    一键配置（扫描 手机热点  sniffer  product）.
**************************************************************************/

/* Include files. */
#include "iw_priv.h"
#include "iw_smartconfig.h"


/* Macro constant definitions. */
#define WIFI_PHONE_AP_CONNECTED_TIMEOUT			(15)//秒
#define SMART_SSID_LEN			(33)

char *product_ap_ssid = "GALAXYWIND_TEST";
char *product_ap_passwd = "12345678";
//char *product_ap_ssid = "1-fangfh";
//char *product_ap_passwd = "88888888";

char *phone_ap_ssid = "phone-discovery";
char *phone_ap_passwd = "smartconfig";
//char *phone_ap_ssid = "1-fangfh";
//char *phone_ap_passwd = "88888888";

//设备监听端口
#define 	SNIFFER_PORT				9919

#define MY_SN	ucd.sn
//广播sn
#define	SN_BROCAST	0
//命令先定义一个
#define 	SNIFFER_CMD_SMARTCONFIG			1

//定时发送ok ms
#define 	SNIFFER_SEND_OK_TIMER		500

#define SMART_SSID_LEN			(33)
#define SMART_PASSWD_LEN		(65)

#define YHMLEN 4

#define MAC_ADDR_LEN (6)
#define RECSNIFFP_LEN (128)

#define MAC_MULTIL_MAGIC	0x55
#define RECV_DATA_LEN_MAX_LIMIT	500
#define RECV_MSG_MAX_LEN	300
#define RECV_MSG_TAG_HEAD	12
#define RECV_MSG_TAG_TAIL	4
#define RECV_MSG_MIX_NUMS	1

#define LASTPACET_REC_TIMEOUT 5 //锁定ap后的能获取到ssid的超时时间，单位秒

#define BASE64_STR_MAXLEN		(256)
#define	SCT_WIFI_SSID	0x10
#define	SCT_WIFI_PASSWD	0x11

#define STOP_SMCF 1
/* Type definitions. */
//type定义
enum {
	SNIFFER_TLV_TYPE_OK = 1,
	SNIFFER_TLV_TYPE_ACK = 2,
	SNIFFER_TLV_TYPE_SSID = 3,
	SNIFFER_TLV_TYPE_PASSWD = 4,
	SNIFFER_TLV_TYPE_AUTHMODE = 5,
	SNIFFER_TLV_TYPE_MAX
};

typedef struct wfsmcf_s {
	int state;	//状态机状态，报文驱动
	bool is_stopsmartconfig;//停止扫描
	bool is_product_ssid;
	bool is_phone_ssid;

	unsigned char mac_addr_phone[MAC_ADDR_LEN];//锁定的mac addr1 手机
	unsigned char mac_addr_ap[MAC_ADDR_LEN];//锁定的mac addr2 AP
	unsigned char mac_addr_multv[MAC_ADDR_LEN];//载体值
	bool is_smcf_ok;//一键配置是否成功
	char cf_ssid[IW_MAX_WIFI_SSID];
	char cf_passwd[IW_MAX_WIFI_PASSWD];
	u_int32_t cfg_time;

	int buff_len;
	int authmode;
	void *dev_conn;
	remot_info premot;
	u_int8_t buff[MAX_UC_PKT_SIZE];
	u_int8_t status[SNIFFER_TLV_TYPE_MAX];

	u_int32_t connect_phone_stare_time;
	bool is_init;
} wfsmcf_t;

//事件
typedef struct {
	int event;
	void *param;
} iws_event_t;

typedef int (* smartconfig_func_t)(wfsmcf_t *smcf);
typedef int (*smartconfig_proc_t)(wfsmcf_t *smcf, iws_event_t *e);

//state machine
typedef struct {
	char *name;
	smartconfig_func_t into;
	smartconfig_proc_t proc;
	smartconfig_func_t out;
	smartconfig_func_t tout;
	int timeout;
} smartconfig_stmc_t;

//状态机状态
enum {
	IW_SC_SCAN_AP 			= 0,  //扫描特定AP进行自动连接
	IW_SC_LISTEN_CHANNEL 	= 1,  //在信道上监听配置信道
	IW_SC_LOCK_CHANNEL		= 2,  //锁定信道
	IW_SC_LISTEN_CFG 		= 3,  //在配置信道上监听配置
	IW_SC_DECODE_CFG        = 4,
	IW_SC_OK 				= 5,  //接收完配置信息进行连接
	IW_SC_PRODUCT_CONNECT	= 6,  //产测ssid
	IW_SC_PHONE_HOT_CONNECT = 7,  //连接手机wifi
	IW_SC_PHONE_HOT_PRO		= 8,  //连接手机wifi成功
	IW_SC_STOP				= 9,  //停止状态机
	IW_SC_MAX
};

//消息类型
enum {
	IW_E_STOP,
	IW_E_SCAN_DONE,
	IW_E_RCV_PACKET,
	IW_E_PHONE_RCV_PACKET,
	IW_E_MAX
};

typedef struct {
	STATUS status;
	void *arg;
} iw_scan_param_t;

typedef struct {
	uint8 *buf;
	uint16 len;
} iw_rx_param_t;

typedef struct sniffer_hdr_s {
	u_int64_t src_sn;
	u_int64_t dst_sn;
	//命令=>SNIFFER_CMD_SMARTCONFIG
	u_int16_t cmd;
	// 参数长度
	u_int16_t param_len;
	//id,重复发送什么的，现在可以不用
	u_int16_t req_id;
	u_int16_t flags;
	//后面紧跟数据
	//u_int8_t data[0];
} sniffer_hdr_t;

typedef struct {
	int buff_len;
	int authmode;
	void *dev_conn;
	remot_info premot;
	char ssid[SMART_SSID_LEN];
	char passwd[SMART_PASSWD_LEN];
	u_int8_t buff[MAX_UC_PKT_SIZE];
	u_int8_t status[SNIFFER_TLV_TYPE_MAX];
} sniffer_t;

#pragma pack(push,1)
typedef	struct	 {
	UINT16		Ver: 2;				// Protocol version
	UINT16		Type: 2;				// MSDU type
	UINT16		SubType: 4;			// MSDU subtype
	UINT16		ToDs: 1;				// To DS indication
	UINT16		FrDs: 1;				// From DS indication
	UINT16		MoreFrag: 1;			// More fragment bit
	UINT16		Retry: 1;			// Retry status bit
	UINT16		PwrMgmt: 1;			// Power management bit
	UINT16		MoreData: 1;			// More data bit
	UINT16		Wep: 1;				// Wep data
	UINT16		Order: 1;			// Strict order expected
} FRAME_CONTROL, *PFRAME_CONTROL;

typedef	struct  _HEADER_802_11	{
	FRAME_CONTROL   FC;//2byte
	UINT16         	Duration;//2byte
	UINT8 	   	Addr1[MAC_ADDR_LEN];//6byte
	UINT8           	Addr2[MAC_ADDR_LEN];//6byte
	UINT8		Addr3[MAC_ADDR_LEN];//6byte
	UINT16		Frag: 4;
	UINT16		Sequence: 12;
}	HEADER_802_11, *PHEADER_802_11;

typedef struct RxControl_s {
	signed rssi: 8; // signal intensity of packet
	unsigned rate: 4;
	unsigned is_group: 1;
	unsigned : 1;
	unsigned sig_mode: 2; // 0:is 11n packet; 1:is not 11n packet;
	unsigned legacy_length: 12; // if not 11n packet, shows length of packet.
	unsigned damatch0: 1;
	unsigned damatch1: 1;
	unsigned bssidmatch0: 1;
	unsigned bssidmatch1: 1;
	unsigned MCS: 7; // if is 11n packet, shows the modulation
	// and code used (range from 0 to 76)
	unsigned CWB: 1; // if is 11n packet, shows if is HT40 packet or not
	unsigned HT_length: 16; // if is 11n packet, shows length of packet.
	unsigned Smoothing: 1;
	unsigned Not_Sounding: 1;
	unsigned: 1;
	unsigned Aggregation: 1;
	unsigned STBC: 2;
	unsigned FEC_CODING: 1; // if is 11n packet, shows if is LDPC packet or not.
	unsigned SGI: 1;
	unsigned rxend_state: 8;
	unsigned ampdu_cnt: 8;
	unsigned channel: 4; //which channel this packet in.
	unsigned: 12;
} RxControl_t;

typedef struct LenSeq_s {
	u16 len; // length of packet
	u16 seq; // serial number of packet, the high 12bits are serial number,
	// low 14 bits are Fragment number (usually be 0)
	u8 addr3[6]; // the third address in packet
} LenSeq_t;

typedef struct sniffer_buf_s {
	RxControl_t rx_ctrl;
	u8 buf[36]; // head of ieee80211 packet
	u16 cnt; // number count of packet
	LenSeq_t lenseq[1]; //length of packet
} sniffer_buf_t, *Psniffer_buf_t;

typedef struct smart_tlv_s {
	unsigned char type;
	unsigned char length;
	//unsigned char value[1];
} smart_tlv_t;

#pragma pack(pop)

typedef struct phoneap_info_s {
	char ssid[SMART_SSID_LEN];
	char passwd[SMART_PASSWD_LEN];
	int authmode;
} phoneap_info_t;
/* Local function declarations. */


/* Macro API definitions. */
#define SMARTCONFIG_DEBUG_CTRL
#ifdef SMARTCONFIG_DEBUG_CTRL
#define SMARTCONFIG_DEBUG(fmt...) DEBUG(fmt)
#define SMARTCONFIG_INFO(fmt...)  INFO(fmt)
#define SMARTCONFIG_ERROR(fmt...) ERROR(fmt)
#else
#define SMARTCONFIG_DEBUG(fmt...)
#define SMARTCONFIG_INFO(fmt...)
#define SMARTCONFIG_ERROR(fmt...) ERROR(fmt)
#endif

#define	IWS_MAX(a, b)	(((a) > (b)) ? (a) : (b))

#define IW_TIME_N_SECOND(x)	( x * 1000)
#define _IP_SHOW(IP) (IP)[0], (IP)[1], (IP)[2], (IP)[3]

/* Global variable declarations. */
wfsmcf_t smartconfig;

static unsigned char recv_head[RECV_MSG_TAG_HEAD] = {0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x5, 0x6, 0x7, 0x8};
static unsigned char recv_tail[RECV_MSG_TAG_TAIL] = {0x9, 0xa, 0xb, 0xc};

static unsigned char recv_msg[RECV_MSG_MAX_LEN + 1];
static unsigned char multi_map[RECV_MSG_MAX_LEN + 1];

static int last_max_num = -1;
static int multi_map_num = 0;

const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

static int ICACHE_FLASH_ATTR iws_proc(wfsmcf_t *smcf, iws_event_t *e);
static int ICACHE_FLASH_ATTR iws_set_state(wfsmcf_t * smcf, int state);

#define LEN_CHECK()  		do {\
	if ( l > len) {				\
		os_printf("error len\n");	\
		return (-1);				\
	}							\
}while(0)

//返回０表示不过，１表示过

const uint8_t __crc8_tbl[256] = {
	0x00, 0x91, 0xe3, 0x72, 0x07, 0x96, 0xe4, 0x75,
	0x0e, 0x9f, 0xed, 0x7c, 0x09, 0x98, 0xea, 0x7b,
	0x1c, 0x8d, 0xff, 0x6e, 0x1b, 0x8a, 0xf8, 0x69,
	0x12, 0x83, 0xf1, 0x60, 0x15, 0x84, 0xf6, 0x67,
	0x38, 0xa9, 0xdb, 0x4a, 0x3f, 0xae, 0xdc, 0x4d,
	0x36, 0xa7, 0xd5, 0x44, 0x31, 0xa0, 0xd2, 0x43,
	0x24, 0xb5, 0xc7, 0x56, 0x23, 0xb2, 0xc0, 0x51,
	0x2a, 0xbb, 0xc9, 0x58, 0x2d, 0xbc, 0xce, 0x5f,
	0x70, 0xe1, 0x93, 0x02, 0x77, 0xe6, 0x94, 0x05,
	0x7e, 0xef, 0x9d, 0x0c, 0x79, 0xe8, 0x9a, 0x0b,
	0x6c, 0xfd, 0x8f, 0x1e, 0x6b, 0xfa, 0x88, 0x19,
	0x62, 0xf3, 0x81, 0x10, 0x65, 0xf4, 0x86, 0x17,
	0x48, 0xd9, 0xab, 0x3a, 0x4f, 0xde, 0xac, 0x3d,
	0x46, 0xd7, 0xa5, 0x34, 0x41, 0xd0, 0xa2, 0x33,
	0x54, 0xc5, 0xb7, 0x26, 0x53, 0xc2, 0xb0, 0x21,
	0x5a, 0xcb, 0xb9, 0x28, 0x5d, 0xcc, 0xbe, 0x2f,
	0xe0, 0x71, 0x03, 0x92, 0xe7, 0x76, 0x04, 0x95,
	0xee, 0x7f, 0x0d, 0x9c, 0xe9, 0x78, 0x0a, 0x9b,
	0xfc, 0x6d, 0x1f, 0x8e, 0xfb, 0x6a, 0x18, 0x89,
	0xf2, 0x63, 0x11, 0x80, 0xf5, 0x64, 0x16, 0x87,
	0xd8, 0x49, 0x3b, 0xaa, 0xdf, 0x4e, 0x3c, 0xad,
	0xd6, 0x47, 0x35, 0xa4, 0xd1, 0x40, 0x32, 0xa3,
	0xc4, 0x55, 0x27, 0xb6, 0xc3, 0x52, 0x20, 0xb1,
	0xca, 0x5b, 0x29, 0xb8, 0xcd, 0x5c, 0x2e, 0xbf,
	0x90, 0x01, 0x73, 0xe2, 0x97, 0x06, 0x74, 0xe5,
	0x9e, 0x0f, 0x7d, 0xec, 0x99, 0x08, 0x7a, 0xeb,
	0x8c, 0x1d, 0x6f, 0xfe, 0x8b, 0x1a, 0x68, 0xf9,
	0x82, 0x13, 0x61, 0xf0, 0x85, 0x14, 0x66, 0xf7,
	0xa8, 0x39, 0x4b, 0xda, 0xaf, 0x3e, 0x4c, 0xdd,
	0xa6, 0x37, 0x45, 0xd4, 0xa1, 0x30, 0x42, 0xd3,
	0xb4, 0x25, 0x57, 0xc6, 0xb3, 0x22, 0x50, 0xc1,
	0xba, 0x2b, 0x59, 0xc8, 0xbd, 0x2c, 0x5e, 0xcf
};

void ICACHE_FLASH_ATTR start_wifi(void)
{
	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;
	struct station_config stcfg;

	wifi_station_get_config(&stcfg);
	os_memcpy((uint8_t *)stcfg.ssid, (uint8_t *)info->ssid, IW_MAX_WIFI_SSID);
	os_memcpy((uint8_t *)stcfg.password, (uint8_t *)info->passwd, IW_MAX_WIFI_PASSWD);
	wifi_station_set_config(&stcfg);
	wifi_station_connect();
	if(!wifi_station_get_auto_connect()) {
		int ret = wifi_station_set_auto_connect(1);
		ret = ret; //解决编译警告
		SMARTCONFIG_ERROR("set wifi auto connect return %d\n", ret);
	}
}

/*
返回值为0，设备上电之后，没有进行一键配置
返回值为1-120，设备刚刚进行了一键配置
返回大于120，设备已经配置设备超过120s
*/
uint32_t ICACHE_FLASH_ATTR get_smcf_time(void)
{
	if (0 == smartconfig.cfg_time) {
		return 0;
	}

	return (get_up_sec() - smartconfig.cfg_time);
}

void ICACHE_FLASH_ATTR start_wifi_config_save(void)
{

	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;

	conf_wifi_set(WIFI_SSID_INDEX, (uint8_t *)info->ssid, IW_MAX_WIFI_SSID);
	conf_wifi_set(WIFI_PASSWD_INDEX, (uint8_t *)info->passwd, IW_MAX_WIFI_PASSWD);

	iw_request_event(EVENT_SYS_WIFI_CONFIG);
}

bool ICACHE_FLASH_ATTR is_lock_ap(void)
{
	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;
	return (SMCF_STA_ISLOCKAP == info->smcf_stat);
}

static uint8_t ICACHE_FLASH_ATTR NST_GetCrc8(uint8_t *ptr, uint32_t len)
{
	uint8_t crc8, data;

	crc8 = 0;
	while(len-- != 0) {
		data = *ptr++;
		crc8 = __crc8_tbl[crc8 ^ data];
	}
	return crc8;
}

//是否是产测模式:1 是产测模式0:不是产测模式
//这个函数用GALAXYWIND_TSET的标志
u_int8_t ICACHE_FLASH_ATTR is_product_mode_f()
{
	return smartconfig.is_product_ssid;
}

u_int8_t ICACHE_FLASH_ATTR is_phonehotpoint_mode_f()
{
	return smartconfig.is_phone_ssid;
}

u_int8_t ICACHE_FLASH_ATTR set_phonehotpoint_mode_f(u_int8_t mode)
{
	smartconfig.is_phone_ssid = mode;
	return smartconfig.is_phone_ssid;
}

static void ICACHE_FLASH_ATTR iws_promiscuous_rx_cb(uint8 *buf, uint16 len)
{
	iws_event_t iws_event;
	iw_rx_param_t iw_rx_param;

	iw_rx_param.buf = buf;
	iw_rx_param.len = len;

	iws_event.event = IW_E_RCV_PACKET;
	iws_event.param = &iw_rx_param;

	iws_proc(&smartconfig, &iws_event);
}


static void ICACHE_FLASH_ATTR iws_scan_callback(void *arg, STATUS status)
{
	iws_event_t iws_event;
	iw_scan_param_t iw_scan_param;

	iw_scan_param.status = status;
	iw_scan_param.arg = arg;

	iws_event.event = IW_E_SCAN_DONE;
	iws_event.param = &iw_scan_param;
	
	SMARTCONFIG_DEBUG("iws_scan_callback\r\n");
	iws_proc(&smartconfig, &iws_event);
}


//接收到的字节数等于编码最大数表示数据接收完成
bool ICACHE_FLASH_ATTR iws_check_msg_complete(u_int8_t rev_count, u_int8_t max_num)
{
	return (rev_count == max_num);
}

static int ICACHE_FLASH_ATTR msg_check(unsigned char *msg, int len)
{
	unsigned char *ptr = NULL;
	int data_len = 0;
	unsigned char crc8 = 0;
	unsigned char tmp_crc8 = 0;
	int l = 0;
	int i;

	ptr = msg;

	//数据长度
	l++;
	LEN_CHECK();
	data_len = *ptr++;
	if (0 == data_len) {
		return -1;
	}
	l += data_len;
	LEN_CHECK();
	ptr += data_len;
	//解析校验值
	l++;
	LEN_CHECK();
	crc8 |= (*ptr++) << 4;
	l++;
	LEN_CHECK();
	crc8 |= *ptr++;
	//crc检测
	tmp_crc8 = (unsigned char)NST_GetCrc8(msg, (unsigned int)(data_len + 1));
	if (crc8 != tmp_crc8) {
		return (-1);
	}
	l += sizeof(recv_tail);
	LEN_CHECK();
	if (os_memcmp(ptr, recv_tail, sizeof(recv_tail))) {
		SMARTCONFIG_ERROR("error recv tail\n");
		for(i = 0; i < sizeof(recv_tail); i++) {
			SMARTCONFIG_ERROR("ok_tail[%d] = %d error_tail[%d]=%d\n",
			                  i, recv_tail[i], i, ptr[i]);
		}
		return -1;
	}
	*ptr = 0;

	return 0;
}



static void ICACHE_FLASH_ATTR  mcast_proc(u_int8_t *mult_addr)
{

	u_int8_t num = 0;
	u_int8_t ch;

	num = mult_addr[4]; //编号
	ch  = mult_addr[5]; //信息值

	SMARTCONFIG_DEBUG("num=%d, ch=%02x\r\n", num, ch);

	//收到最后一个数据包
	if (0 == num) {
		if (iws_check_msg_complete(multi_map_num, last_max_num))
			goto check;
		else
			return;
	}

	//重复的数据
	if (multi_map[num - 1] == 1) {
		return;
	}

	recv_msg[num - 1] = ch;
	multi_map[num - 1] = 1;

	multi_map_num++;
	last_max_num = IWS_MAX(last_max_num, num);

	if (!iws_check_msg_complete(multi_map_num, last_max_num)) {
		SMARTCONFIG_DEBUG("data recv not finished  to %d\n", last_max_num);
		return;
	}

check:

	//查找尾部并校验
	if(0 == msg_check(recv_msg + sizeof(recv_head), RECV_MSG_MAX_LEN - sizeof(recv_head))) {
		last_max_num = -1;
		multi_map_num = 0;
		iws_set_state(&smartconfig, IW_SC_DECODE_CFG);
	}

}


#if 1//状态机所有的状态
static int ICACHE_FLASH_ATTR iws_scan_ap_into(wfsmcf_t *smcf)
{
	struct scan_config config;
	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;

	info->smcf_stat = SMCF_STA_ISSCAN_AP;

	wifi_set_opmode(STATION_MODE);
	wifi_station_disconnect();

	os_memset(&config, '\0', sizeof(struct scan_config));

	wifi_station_scan(&config, iws_scan_callback);

	SMARTCONFIG_DEBUG("iws_scan_ap_into\r\n");
	return RS_OK;
}

static int ICACHE_FLASH_ATTR iws_scan_ap_proc(wfsmcf_t *smcf, iws_event_t *e)
{
	iw_scan_param_t *scan_param = NULL;
	struct bss_info *info = NULL;
	int is_product = 0, is_phone = 0;
	int count = 0;
	SMARTCONFIG_DEBUG("iws_scan_ap_proc\r\n");
	if (e->event != IW_E_SCAN_DONE) {
		SMARTCONFIG_ERROR("Is not scan done event in scan done proc\r\n");
		return RS_ERROR;
	}
	
	scan_param = (iw_scan_param_t *)e->param;

	if (scan_param->status != OK || scan_param->arg == NULL) {
		iws_set_state(smcf, IW_SC_LISTEN_CHANNEL);
		return RS_OK;
	}

	info = (struct bss_info *)scan_param->arg;
	while(info) {
		SMARTCONFIG_DEBUG("[%d]bssid:%02x-%02x-%02x-%02x-%02x-%02x ssid:%s authmode:%u channel:%u rssi:%d\n",
		                  count, MAC_SHOW(info->bssid), info->ssid, info->authmode, info->channel, info->rssi);
		//产测ssid
		if (strlen(product_ap_ssid) == strlen((char *)info->ssid) &&
		    0 == os_memcmp(info->ssid, (uint8*)product_ap_ssid, os_strlen(product_ap_ssid))) {
			is_product = 1;
			break;
		}
		//热点ssid
		if (strlen(phone_ap_ssid) == strlen((char *)info->ssid) &&
		    0 == os_memcmp(info->ssid, (uint8*)phone_ap_ssid, os_strlen(phone_ap_ssid))) {
			is_phone = 1;
		}
		info = info->next.stqe_next;
		count++;
	}

	if (is_product) {
		os_strncpy(smcf->cf_ssid, product_ap_ssid, IW_MAX_WIFI_SSID);
		os_strncpy(smcf->cf_passwd, product_ap_passwd, IW_MAX_WIFI_PASSWD);
		iws_set_state(smcf, IW_SC_PRODUCT_CONNECT);
		smcf->is_product_ssid = 1;
	} else if(is_phone) {
		os_strncpy(smcf->cf_ssid, phone_ap_ssid, IW_MAX_WIFI_SSID);
		os_strncpy(smcf->cf_passwd, phone_ap_passwd, IW_MAX_WIFI_PASSWD);
		iws_set_state(smcf, IW_SC_PHONE_HOT_CONNECT);
		smcf->is_phone_ssid = 1;
	} else {
		smcf->is_phone_ssid = 0;
		smcf->is_product_ssid = 0;

		iws_set_state(smcf, IW_SC_LISTEN_CHANNEL);
	}

	return RS_OK;
}


static int ICACHE_FLASH_ATTR iws_change_channel_timer(void *cp)
{
	u_int8_t chn;

	chn = wifi_get_channel();
	++chn;
	chn = chn % 15;
	if(chn == 0) {
		chn = 1;
	}
	wifi_set_channel(chn);
	SMARTCONFIG_INFO("Chage channel to chn=%d\r\n", chn);

	iw_timer_set(TID_CHANGE_CHN_TIMER, 100,
	             0, iws_change_channel_timer, (void *)&smartconfig);

	return 1;
}


static int ICACHE_FLASH_ATTR iws_listen_channel_into(wfsmcf_t *smcf)
{
	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;

	info->smcf_stat = SMCF_STA_SNIFF;

	wifi_set_opmode(STATION_MODE);
	wifi_station_disconnect();

	iw_timer_set(TID_CHANGE_CHN_TIMER, 100,
	             0, iws_change_channel_timer, (void *)&smartconfig);

	SMARTCONFIG_DEBUG("Listen channel %d\r\n", wifi_get_channel());


	wifi_promiscuous_enable(1);
	wifi_set_promiscuous_rx_cb(iws_promiscuous_rx_cb);

	return 1;
}

static int ICACHE_FLASH_ATTR iws_listen_channel_proc(wfsmcf_t *smcf, iws_event_t *e)
{
	iw_rx_param_t *rx_param = NULL;
	PHEADER_802_11 pHeader = NULL;
	Psniffer_buf_t psniffer_buf = NULL;
	char YHM[YHMLEN] = {0x01, 0x00, 0x5e, 0x55};

	if (e->event != IW_E_RCV_PACKET) {
		SMARTCONFIG_ERROR("Is not rcv Packet \r\n");
		return RS_ERROR;
	}

	rx_param = (iw_rx_param_t *)e->param;

	psniffer_buf = (Psniffer_buf_t)rx_param->buf;
	pHeader = (PHEADER_802_11)psniffer_buf->buf;

	if(pHeader->Addr3[0] == YHM[0] && pHeader->Addr3[1] == YHM[1]
	   && pHeader->Addr3[2] == YHM[2] && pHeader->Addr3[3] == YHM[3]) {

		os_memcpy(smcf->mac_addr_ap, pHeader->Addr1, MAC_ADDR_LEN);
		os_memcpy(smcf->mac_addr_phone, pHeader->Addr2, MAC_ADDR_LEN);
		iws_set_state(smcf, IW_SC_LOCK_CHANNEL);
	} else if(pHeader->Addr1[0] == YHM[0] && pHeader->Addr1[1] == YHM[1]
	          && pHeader->Addr1[2] == YHM[2] && pHeader->Addr1[3] == YHM[3]) {

		os_memcpy(smcf->mac_addr_ap, pHeader->Addr2, MAC_ADDR_LEN);
		os_memcpy(smcf->mac_addr_phone, pHeader->Addr3, MAC_ADDR_LEN);
		iws_set_state(smcf, IW_SC_LOCK_CHANNEL);
	}

	return RS_OK;
}

static int ICACHE_FLASH_ATTR iws_listen_channel_out(wfsmcf_t *smcf)
{
	wifi_promiscuous_enable(0);

	iw_timer_delete(TID_CHANGE_CHN_TIMER);

	return 1;
}


static int ICACHE_FLASH_ATTR iws_listen_channel_timeout(wfsmcf_t * smcf)
{
	SMARTCONFIG_DEBUG("iws_listen_channel_timeout\r\n");
	iws_set_state(smcf, IW_SC_SCAN_AP);
	return RS_OK;
}


static int ICACHE_FLASH_ATTR iws_lock_channel_into(wfsmcf_t *smcf)
{
	struct scan_config config;
	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;

	SMARTCONFIG_ERROR("iws_lock_channel_into -> Lock AP bssid %02x:%02x:%02x:%02x:%02x:%02x\r\n", MAC_SHOW(smcf->mac_addr_ap));
	info->smcf_stat = SMCF_STA_ISLOCKAP;

	wifi_set_opmode(STATION_MODE);

	os_memset(&config, '\0', sizeof(struct scan_config));
	config.bssid = smcf->mac_addr_ap;
	wifi_station_scan(&config, iws_scan_callback);

	return RS_OK;
}

static int ICACHE_FLASH_ATTR iws_lock_channel_proc(wfsmcf_t *smcf, iws_event_t *e)
{
	iw_scan_param_t *scan_param = NULL;
	struct bss_info *info = NULL;

	if (e->event != IW_E_SCAN_DONE) {
		SMARTCONFIG_ERROR("Is not scan done event\r\n");
		return RS_ERROR;
	}

	scan_param = (iw_scan_param_t *)e->param;
	info = (struct bss_info *)scan_param->arg;

	if (scan_param->status != OK || NULL == info) {
		SMARTCONFIG_ERROR("status:%d, info=%p\r\n", scan_param->status, info);
		iws_set_state(smcf, IW_SC_LOCK_CHANNEL);//?
		return RS_OK;
	}

	SMARTCONFIG_DEBUG("info->bssid:%02x:%02x:%02x:%02x:%02x:%02x:%02x\r\n", MAC_SHOW(info->bssid));
	if (os_memcmp(smcf->mac_addr_ap, info->bssid, 6) == 0) {
		SMARTCONFIG_DEBUG("change channel %d --> %d\r\n", wifi_get_channel(), info->channel);
		wifi_set_channel(info->channel);
		iws_set_state(smcf, IW_SC_LISTEN_CFG);
	} else {
		iws_set_state(smcf, IW_SC_LOCK_CHANNEL);//?
	}

	return RS_OK;
}


int ICACHE_FLASH_ATTR iws_lock_channel_timeout(wfsmcf_t *smcf)
{
	iws_set_state(smcf, IW_SC_SCAN_AP);
	SMARTCONFIG_ERROR("iws_lock_channel_die 8\r\n");
	return RS_OK;
}


int ICACHE_FLASH_ATTR iws_listen_cfg_into(wfsmcf_t *smcf)
{
	//初始化接收变量
	os_memset(recv_msg, '\0', sizeof(recv_msg));
	os_memset(multi_map, '\0', sizeof(recv_msg));
	last_max_num = -1;
	multi_map_num = 0;

	wifi_set_opmode(STATION_MODE);
	wifi_station_disconnect();

	wifi_promiscuous_enable(1);
	wifi_set_promiscuous_rx_cb(iws_promiscuous_rx_cb);

	SMARTCONFIG_DEBUG("iws_listen_cfg_into ->Listen channel %d\r\n", wifi_get_channel());

	return RS_OK;
}

static int ICACHE_FLASH_ATTR smartconfig_muli_check(unsigned char *ap_addr2, unsigned char *phone_addr3)
{
	if (os_memcmp(smartconfig.mac_addr_ap, ap_addr2, MAC_ADDR_LEN) ||
	    os_memcmp(smartconfig.mac_addr_phone, phone_addr3, MAC_ADDR_LEN)) {
		return 0;
	}

	return 1;
}

int ICACHE_FLASH_ATTR iws_listen_cfg_proc(wfsmcf_t *smcf, iws_event_t *e)
{
	iw_rx_param_t *rx_param = NULL;
	PHEADER_802_11 pHeader = NULL;
	Psniffer_buf_t psniffer_buf = NULL;
	char YHM[YHMLEN] = {0x01, 0x00, 0x5e, 0x55};

	if (e->event != IW_E_RCV_PACKET) {
		SMARTCONFIG_ERROR("Is not rcv done\r\n");
		return RS_ERROR;
	}

	rx_param = (iw_rx_param_t *)e->param;
	psniffer_buf = (Psniffer_buf_t)rx_param->buf;
	pHeader = (PHEADER_802_11)psniffer_buf->buf;


	if(pHeader->Addr3[0] == YHM[0] && pHeader->Addr3[1] == YHM[1]
	   && pHeader->Addr3[2] == YHM[2] && pHeader->Addr3[3] == YHM[3]) {

		if(smartconfig_muli_check(pHeader->Addr1, pHeader->Addr2)) {
			os_memcpy(smcf->mac_addr_multv, pHeader->Addr3, MAC_ADDR_LEN);
			mcast_proc(smcf->mac_addr_multv);
		}
	} else if(pHeader->Addr1[0] == YHM[0] && pHeader->Addr1[1] == YHM[1]
	          && pHeader->Addr1[2] == YHM[2] && pHeader->Addr1[3] == YHM[3]) {

		if(smartconfig_muli_check(pHeader->Addr2, pHeader->Addr3)) {
			os_memcpy(smcf->mac_addr_multv, pHeader->Addr1, MAC_ADDR_LEN);
			mcast_proc(smcf->mac_addr_multv);
		}
	}

	return RS_OK;
}

int ICACHE_FLASH_ATTR iws_listen_cfg_out(wfsmcf_t *smcf)
{
	wifi_promiscuous_enable(0);

	SMARTCONFIG_DEBUG("iws_listen_cfg_out\r\n");

	return RS_OK;
}

int ICACHE_FLASH_ATTR iws_listen_cfg_timeout(wfsmcf_t *smcf)
{
	iws_set_state(smcf, IW_SC_SCAN_AP);

	SMARTCONFIG_DEBUG("iws_listen_cfg_timeout\r\n");
	wifi_promiscuous_enable(0);
	return RS_OK;
}

static char ICACHE_FLASH_ATTR find_pos(char ch)
{
	char *ptr = (char*)strrchr(base, ch);//the last position (the only) in base[]
	return (ptr - base);
}

char * ICACHE_FLASH_ATTR base64_decode(const char *data, char *dst_data)
{
	int ret_len = 0;
	int equal_count = 0;
	char *ret = NULL;
	char *f = NULL;
	int tmp = 0;
	int temp = 0;
	char need[3];
	int prepare = 0;
	int data_len = 0;
	int i = 0;

	if (!dst_data) {
		return NULL;
	}
	data_len = os_strlen(data);
	ret_len = (data_len / 4) * 3;

	if (*(data + data_len - 1) == '=') {
		equal_count += 1;
	}
	if (*(data + data_len - 2) == '=') {
		equal_count += 1;
	}
	if (*(data + data_len - 3) == '=') {
		//seems impossible
		equal_count += 1;
	}
	switch (equal_count) {
		case 0:
			ret_len += 4;//3 //+ 1 [1 for NULL]
			break;
		case 1:
			ret_len += 4;//Ceil((6*3)/8)+1
			break;
		case 2:
			ret_len += 3;//Ceil((6*2)/8)+1
			break;
		case 3:
			ret_len += 2;//Ceil((6*1)/8)+1
			break;
	}
	ret = dst_data;
	f = ret;
	while (tmp < (data_len - equal_count)) {
		temp = 0;
		prepare = 0;
		os_memset(need, 0, 4);
		while (temp < 4) {
			if (tmp >= (data_len - equal_count)) {
				break;
			}
			prepare = (prepare << 6) | (find_pos(data[tmp]));
			temp++;
			tmp++;
		}
		prepare = prepare << ((4 - temp) * 6);
		for (i = 0; i < 3 ; i++ ) {
			if (i == temp) {
				break;
			}
			*f = (char)((prepare >> ((2 - i) * 8)) & 0xFF);
			f++;
		}
	}
	*f = '\0';
	return ret;
}


int ICACHE_FLASH_ATTR iws_decode_cfg_into(wfsmcf_t *smcf)
{
	int n = 0;
	int base_len = 0;
	int len_limit = 0;
	static char buf_tmp[BASE64_STR_MAXLEN] = {0};
	static char buf_base64[BASE64_STR_MAXLEN] = {0};
	smart_tlv_t *ptlv = NULL;

	char *ptr_recv_data = (char *)recv_msg + sizeof(recv_head) + 1 ;
	base_len = RECV_MSG_MAX_LEN - sizeof(recv_head) - 1;
	ptlv = (smart_tlv_t *)ptr_recv_data;
	len_limit = 0;

	while(1) {
		len_limit += (sizeof(smart_tlv_t) );
		if (len_limit > base_len) {
			goto need_redo;
		}

		switch(ptlv->type) {
			case SCT_WIFI_SSID:
				len_limit += ptlv->length;
				if (len_limit > base_len) {
					goto need_redo;
				}

				os_memcpy((uint8_t *)buf_tmp, (uint8_t  *) & (ptlv[1]), ptlv->length);
				base64_decode(buf_tmp, buf_base64);
				os_memcpy((uint8_t *)smcf->cf_ssid, (uint8_t *)buf_base64, IW_MAX_WIFI_SSID);
				ptlv = (smart_tlv_t *)(ptr_recv_data + len_limit);
				n++;
				break;
			case SCT_WIFI_PASSWD:
				len_limit += ptlv->length;
				if (len_limit > base_len) {
					goto need_redo;
				}

				if (0 != ptlv->length) {
					os_memset(buf_tmp, 0, sizeof(buf_tmp));
					os_memcpy((uint8_t  *)buf_tmp, (uint8_t  *) & (ptlv[1]), ptlv->length);
					base64_decode(buf_tmp, buf_base64);
					os_memcpy((uint8_t  *)smcf->cf_passwd, (uint8_t  *)buf_base64, IW_MAX_WIFI_PASSWD);
				}
				ptlv = (smart_tlv_t *)(ptr_recv_data + len_limit);
				n++;
				break;
			default:
				SMARTCONFIG_ERROR("error type=%d\n", ptlv->type);
				goto need_redo;
		}

		if (n >= 2) {
			break;
		}
	}

	smcf->cfg_time = get_up_sec();
	SMARTCONFIG_INFO("get ssid=%s\n", smcf->cf_ssid);
	SMARTCONFIG_INFO("get passwd=%s\n", smcf->cf_passwd);

	iws_set_state(smcf, IW_SC_OK);
	return 1;

need_redo:
	iws_set_state(smcf, IW_SC_SCAN_AP);
	return 0;
}

void connect_wifi(wfsmcf_t *smcf)
{
	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;
	static struct station_config stcfg;

	info->smcf_stat = SMCF_STA_CONFOK;

	if(!smcf->is_phone_ssid && !smcf->is_product_ssid) {
		conf_wifi_set(WIFI_SSID_INDEX, (UINT8 *)smcf->cf_ssid, IW_MAX_WIFI_SSID);
		conf_wifi_set(WIFI_PASSWD_INDEX, (UINT8 *)smcf->cf_passwd, IW_MAX_WIFI_PASSWD);
		conf_wifi_commit();
	}

	wifi_station_get_config(&stcfg);
	os_memcpy((uint8_t *)stcfg.ssid, (uint8_t *)smcf->cf_ssid, IW_MAX_WIFI_SSID);
	os_memcpy((uint8_t *)stcfg.password, (uint8_t *)smcf->cf_passwd, IW_MAX_WIFI_PASSWD);
	wifi_station_set_config(&stcfg);

	wifi_station_connect();

	if(!wifi_station_get_auto_connect()) {
		int ret = wifi_station_set_auto_connect(1);
		ret = ret;   //解决编译警告
		SMARTCONFIG_ERROR("set wifi auto connect return %d\n", ret);
	}

	iws_set_state(smcf, IW_SC_STOP);
}

int ICACHE_FLASH_ATTR iws_ok_into(wfsmcf_t *smcf)
{
	connect_wifi(smcf);
	return RS_OK;
}

int ICACHE_FLASH_ATTR iws_product_into(wfsmcf_t *smcf)
{
	SMARTCONFIG_INFO("iws_product_into\r\n");
	connect_wifi(smcf);
	return RS_OK;
}

int ICACHE_FLASH_ATTR phonerd_wifi_linkup_check_timer(void *t)
{
	wfsmcf_t *smcf = (wfsmcf_t *)t;
	uint8 station_st = 0;
	station_st = wifi_station_get_connect_status();
	if(station_st != STATION_GOT_IP) {
		SMARTCONFIG_INFO("phonerd_wifi_linkup_check_timer check not linkup!!!\r\n");
		return 0;
	}
	SMARTCONFIG_INFO("phonerd_wifi_linkup_check_timer check linkup!!! \r\n");

	iw_timer_delete(TID_SMCF_CHECK_WIFI_UP);

	iws_set_state(smcf, IW_SC_PHONE_HOT_PRO);

	return 0;
}

int ICACHE_FLASH_ATTR iws_phone_hot_connect_into(wfsmcf_t *smcf)
{
	connect_wifi(smcf);

	iw_timer_set(TID_SMCF_CHECK_WIFI_UP, TIME_N_SECOND(1),
	             1, phonerd_wifi_linkup_check_timer, (void *)smcf);
	return RS_OK;
}

void  ICACHE_FLASH_ATTR phone_hotpoint_sendback(void *arg)
{
	
}


bool is_mem_zero(u_int8_t *value, int len)
{
	int i;

	for(i = 0; i < len; i++) {
		if (0 != value[i]) {
			return false;
		}
	}

	return true;
}

bool phoneap_is_proc_ok(wfsmcf_t *smcf)
{
	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;
	struct station_config stcfg;

	info->smcf_stat = SMCF_STA_CONFOK;


	SMARTCONFIG_INFO("will run phoneap_is_proc_ok into \n");

	wifi_station_disconnect();
	wifi_station_get_config(&stcfg);
	SMARTCONFIG_INFO("old ssid:%s, pwd:%s\n", stcfg.ssid, stcfg.password);
	os_memcpy((uint8_t *)stcfg.ssid, (uint8_t *)smcf->cf_ssid, IW_MAX_WIFI_SSID);
	os_memcpy((uint8_t *)stcfg.password, (uint8_t *)smcf->cf_passwd, IW_MAX_WIFI_PASSWD);
	SMARTCONFIG_INFO("new ssid:%s, pwd:%s\n", stcfg.ssid, stcfg.password);
	wifi_station_set_config(&stcfg);

	os_memcpy((uint8_t *)info->ssid, (uint8_t *)smcf->cf_ssid, IW_MAX_WIFI_SSID);
	os_memcpy((uint8_t *)info->passwd, (uint8_t *)smcf->cf_passwd, IW_MAX_WIFI_PASSWD);

	//每秒定时器
	//一键配置之后，初始化密码
	iw_ucd_license();
	iw_request_event(EVENT_SYS_WIFI_CONFIG);

	wifi_station_connect();
	if(!wifi_station_get_auto_connect()) {
		SMARTCONFIG_ERROR("set wifi auto connect return %d\n", wifi_station_set_auto_connect(1));
	}

	set_phonehotpoint_mode_f(0);
	return 1;
}

static void sniffer_order_header(sniffer_hdr_t *hdr)
{
	if (NULL == hdr)
		return;

	hdr->src_sn = ntoh_ll(hdr->src_sn);
	hdr->dst_sn = ntoh_ll(hdr->dst_sn);
	hdr->cmd = ntohs(hdr->cmd);
	hdr->req_id = ntohs(hdr->req_id);
	hdr->param_len = ntohs(hdr->param_len);
	hdr->flags = ntohs(hdr->flags);
}

static void sniffer_status_set(u_int16_t type, wfsmcf_t *smcf)
{
	if (type < SNIFFER_TLV_TYPE_MAX) {
		smcf->status[type] = 1;
	}
}

static void sniffer_tlv_proc(sniffer_hdr_t *pkt, wfsmcf_t *smcf)
{
	int remain = 0;
	uc_tlv_t *ptlv = NULL;

	remain = pkt->param_len;
	if (remain < sizeof(*ptlv)) {
		SMARTCONFIG_ERROR("remain =%u sizeof(tlv)=%u\n", remain, sizeof(*ptlv));
		return;
	}

	ptlv = (uc_tlv_t *)(pkt + 1);
	ptlv->type = ntohs(ptlv->type);
	ptlv->len = ntohs(ptlv->len);

	while (remain >= (int)(sizeof(uc_tlv_t)) && remain >= (int)(sizeof(uc_tlv_t) + ptlv->len)) {
		switch (ptlv->type) {
			case SNIFFER_TLV_TYPE_OK:
				SMARTCONFIG_DEBUG(" recv SNIFFER_TLV_TYPE_OK\n");
				break;
			case SNIFFER_TLV_TYPE_ACK:
				SMARTCONFIG_DEBUG(" recv SNIFFER_TLV_TYPE_ACK\n");
				break;
			case SNIFFER_TLV_TYPE_SSID:
				SMARTCONFIG_DEBUG(" recv SNIFFER_TLV_TYPE_SSID\n");
				if (ptlv->len > sizeof(smcf->cf_ssid)) {
					SMARTCONFIG_ERROR("error ssid len=%u\n", ptlv->len);
					return ;
				}
				os_memset(smcf->cf_ssid, 0, sizeof(smcf->cf_ssid));
				os_memcpy((u_int8_t *)smcf->cf_ssid, (u_int8_t *)(ptlv + 1), ptlv->len);
				smcf->cf_ssid[sizeof(smcf->cf_ssid) - 1] = 0;
				SMARTCONFIG_DEBUG("ssid=%s\n", smcf->cf_ssid);
				break;
			case SNIFFER_TLV_TYPE_PASSWD:
				SMARTCONFIG_DEBUG(" recv SNIFFER_TLV_TYPE_PASSWD\n");
				if (ptlv->len > sizeof(smcf->cf_passwd)) {
					SMARTCONFIG_ERROR("error passwd len=%u\n", ptlv->len);
					return ;
				}
				os_memset(smcf->cf_passwd, 0, sizeof(smcf->cf_passwd));
				os_memcpy((u_int8_t *)smcf->cf_passwd, (u_int8_t *)(ptlv + 1), ptlv->len);
				smcf->cf_passwd[sizeof(smcf->cf_passwd) - 1] = 0;
				SMARTCONFIG_DEBUG("cf_passwd=%s\n", smcf->cf_passwd);
				break;
			case SNIFFER_TLV_TYPE_AUTHMODE:
				SMARTCONFIG_DEBUG("recv SNIFFER_TLV_TYPE_AUTHMODE\n");
				if (ptlv->len == sizeof(smcf->authmode)) {
					SMARTCONFIG_DEBUG("buf[0]=[%u], buf[1]=[%u], buf[2]=[%u], buf[3]=[%u]\r\n", *((char *)(ptlv + 1)), *((char *)(ptlv + 1) + 1), *((char *)(ptlv + 1) + 2), *((char *)(ptlv + 1) + 3));
					//psniffer_ctrl->authmode = *((u_int32_t *)(ptlv+1));
					os_memcpy(&smcf->authmode, ptlv + 1, 4);
					smcf->authmode = (int)htonl(smcf->authmode);
					SMARTCONFIG_DEBUG("authmode=%d, int=%d\n", smcf->authmode, sizeof(int));
				}
				break;
			default:
				SMARTCONFIG_ERROR("not support type=%u\n", ptlv->type);
				break;
		}
		SMARTCONFIG_DEBUG("tlv type = %d, tlv len = %d\r\n", ptlv->type, ptlv->len);
		sniffer_status_set(ptlv->type, smcf);
		remain -= sizeof(uc_tlv_t) + ptlv->len;

		if (remain >= sizeof(uc_tlv_t)) {
			ptlv = tlv_next(ptlv);
			ptlv->type = ntohs(ptlv->type);
			ptlv->len = ntohs(ptlv->len);
		} else {
			SMARTCONFIG_DEBUG("tlv end\r\n");
		}
	}

}

void ICACHE_FLASH_ATTR phone_hotpoint_recv(void *arg, char *pusrdata, unsigned short length)
{
	struct espconn *conn;
	remot_info *premot = NULL;
	sint8 ret = ESPCONN_OK;
	sniffer_hdr_t *pkt;
	u_int32_t ip;
	wfsmcf_t *smcf;
	iws_event_t iws_event;
	iw_rx_param_t iw_rx_param;

	SMARTCONFIG_DEBUG("********************************************phone_hotpoint_recv is called\r\n");
	conn = (struct espconn *)arg;
	smcf = (wfsmcf_t *)conn->reverse;

	os_memcpy(smcf->buff, pusrdata, length);
	smcf->buff_len = length;
	if (length < sizeof(sniffer_hdr_t)) {
		SMARTCONFIG_ERROR("Recv bad pkt: len=%d\n", length);
		return ;
	}

	pkt = (sniffer_hdr_t *)smcf->buff;

	iw_rx_param.buf = (void *)pkt;
	iw_rx_param.len = length;

	iws_event.event = IW_E_PHONE_RCV_PACKET;
	iws_event.param = &iw_rx_param;

	sniffer_order_header(pkt);

	if (length < (pkt->param_len + sizeof(sniffer_hdr_t))) {
		SMARTCONFIG_ERROR("Recv bad pkt: param_len=%d, but n=%d\n", pkt->param_len, length);
		return ;
	}

	SMARTCONFIG_DEBUG("src_sn=%12llu dst_sn=%12llu cmd=%u param_len=%u, flags=%u, req_id=%u\n",
	                  pkt->src_sn, pkt->dst_sn, pkt->cmd, pkt->param_len, pkt->flags, pkt->req_id);

	if ((ret = espconn_get_connection_info(conn, &premot, 0)) == ESPCONN_OK) {
		os_memcpy(&ip, premot->remote_ip, 4);
		ip = ntohl(ip);
		os_memcpy(premot->remote_ip, &ip, 4);
		os_memcpy(&smcf->premot, premot, sizeof(remot_info));
		SMARTCONFIG_DEBUG("%s recv from ip %u.%u.%u.%u port:%d\n", __FUNCTION__,
		                  smcf->premot.remote_ip[0],
		                  smcf->premot.remote_ip[1],
		                  smcf->premot.remote_ip[2],
		                  smcf->premot.remote_ip[3],
		                  smcf->premot.remote_port);

		iws_proc(smcf, &iws_event);
	} else {
		SMARTCONFIG_ERROR("iw_lan_srv_recv get premot ip and port failed ret(%d)\n", ret);
	}
}


int ICACHE_FLASH_ATTR iws_phone_hot_connect_timeout(wfsmcf_t *smcf)
{
	iw_timer_delete(TID_SMCF_CHECK_WIFI_UP);
	iws_set_state(smcf, IW_SC_SCAN_AP);
	return RS_OK;
}

bool is_get_phoneap_info(wfsmcf_t *smcf)
{
	return (smcf->status[SNIFFER_TLV_TYPE_SSID] == 1) &&
	       (smcf->status[SNIFFER_TLV_TYPE_PASSWD] == 1) &&
	       (smcf->status[SNIFFER_TLV_TYPE_AUTHMODE] == 1);
}

static void send_ok(wfsmcf_t *smcf)
{
	int ret;
	sniffer_hdr_t *hdr;
	uc_tlv_t *ptlv = NULL;
	u_int32_t ip;

	hdr = (sniffer_hdr_t *)smcf->buff;
	os_memset(hdr, 0, MAX_UC_PKT_SIZE);

	hdr->src_sn = ntoh_ll(MY_SN);
	hdr->dst_sn = ntoh_ll(SN_BROCAST);
	hdr->cmd = htons(SNIFFER_CMD_SMARTCONFIG);
	hdr->param_len = htons(sizeof(uc_tlv_t));
	ptlv = (uc_tlv_t *)(hdr + 1);
	ptlv->type = htons(SNIFFER_TLV_TYPE_OK);
	ptlv->len = 0;

	os_memcpy(&ip, smcf->premot.remote_ip, 4);

	ret = iw_udp_send(ip, smcf->premot.remote_port, (char *)smcf->buff, sizeof(sniffer_hdr_t) + sizeof(uc_tlv_t), smcf->dev_conn);
	if (ret <= 0) {
		SMARTCONFIG_DEBUG("send_ok %s send pkt failed: ip=%u.%u.%u.%u, port=%d, len=%d\n",
		                  "sendok", _IP_SHOW(smcf->premot.remote_ip), smcf->premot.remote_port, sizeof(sniffer_hdr_t) + sizeof(uc_tlv_t));
	} else {
		//默认打开报文收发打印
		SMARTCONFIG_DEBUG("send_ok %s send ip=%u.%u.%u.%u, port=%d, len=%d: \n",
		                  "PHONE HOT POINT", _IP_SHOW(smcf->premot.remote_ip), smcf->premot.remote_port, sizeof(sniffer_hdr_t) + sizeof(uc_tlv_t));
	}
}

int ICACHE_FLASH_ATTR send_ok_timer(void *t)
{
	wfsmcf_t *smcf = (wfsmcf_t *)t;

	if (is_get_phoneap_info(smcf)) {
		iw_timer_delete(TID_PHP_SEND_OK);
		send_ok(smcf);
	}

	return 0;
}

bool is_get_phoneap_ack(wfsmcf_t *smcf)
{
	return smcf->status[SNIFFER_TLV_TYPE_ACK] == 1;
}

bool ICACHE_FLASH_ATTR is_wifi_up(void)
{
	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;
	return (SMCF_STA_CONNECTAP ==  info->smcf_stat ||
	        SMCF_STA_GETIP == info->smcf_stat );
}

int ICACHE_FLASH_ATTR phonehotpoint_connected_work(void *t)
{
	u_int32_t now = get_up_sec();
	wfsmcf_t *smcf = (wfsmcf_t *)t;

	//超时或者无线断了或者得到ack确认
	if ((now - smcf->connect_phone_stare_time > WIFI_PHONE_AP_CONNECTED_TIMEOUT) ||
	    !is_wifi_up() ||
	    is_get_phoneap_ack(smcf)) {
		//超时调到sniffer状态
		SMARTCONFIG_DEBUG("smcf_connected_work proc  n-l=[%d], wifiup=[%d], ack=[%d]\n", now - smcf->connect_phone_stare_time,  !is_wifi_up(), is_get_phoneap_ack(smcf));
		if (!is_get_phoneap_info(smcf)) {
			//去smart config不用删除保存信道的内存
			SMARTCONFIG_ERROR("phoneap_is_proc_ok error!!\r\n");
			iws_set_state(smcf, IW_SC_SCAN_AP);

			iw_timer_delete(TID_PHP_ACK_OK);
			return 0;
		}
		smcf->cfg_time = get_up_sec();
		phoneap_is_proc_ok(smcf);

		iws_set_state(smcf, IW_SC_STOP);//?
	}
	return 1;
}

int ICACHE_FLASH_ATTR iws_phone_hot_pro_into(wfsmcf_t *smcf)
{
	struct espconn *conn;
	iw_sys_info_t *info = (iw_sys_info_t *)&piw_priv->iw_sys;

	SMARTCONFIG_DEBUG("iws_phone_hot_pro_into\r\n");
	info->smcf_stat = SMCF_STA_ISLOCKAP;


	conn  = iw_udp_server_create(0, SNIFFER_PORT, phone_hotpoint_recv, phone_hotpoint_sendback);
	smcf->dev_conn = (void *)conn;
	conn->reverse = (void *)smcf;

	if (smcf->dev_conn == NULL) {
		iws_set_state(smcf, IW_SC_SCAN_AP);
		return RS_ERROR;
	}

	smcf->connect_phone_stare_time = get_up_sec();

	iw_timer_set(TID_PHP_ACK_OK, 2, 1, phonehotpoint_connected_work, (void *)smcf);
	iw_timer_set(TID_PHP_SEND_OK, SNIFFER_SEND_OK_TIMER, 1, send_ok_timer, (void *)smcf);

	return RS_OK;
}

int ICACHE_FLASH_ATTR iws_phone_hot_pro_pro(wfsmcf_t *smcf, iws_event_t *e)
{
	iw_rx_param_t *iw_rx_param = e->param;
	sniffer_hdr_t * pkt = (sniffer_hdr_t *)iw_rx_param->buf;

	if(e->event != IW_E_PHONE_RCV_PACKET) {
		SMARTCONFIG_DEBUG("PHONE HOT POINT RECV EVENT ERROR\r\n");
		return RS_ERROR;
	}

	switch(pkt->cmd) {
		case SNIFFER_CMD_SMARTCONFIG:
			sniffer_tlv_proc(pkt, smcf);
			break;
		default:
			SMARTCONFIG_ERROR("not support cmd=%u\n", pkt->cmd);
			break;
	}
	return RS_OK;
}

int ICACHE_FLASH_ATTR iws_phone_hot_pro_out(wfsmcf_t *smcf)
{
	iw_timer_delete(TID_PHP_ACK_OK);
	iw_timer_delete(TID_PHP_SEND_OK);

	espconn_delete(smcf->dev_conn);
	os_free(smcf->dev_conn);

	SMARTCONFIG_ERROR("iws_phone_hot_pro_out\n");
	return RS_OK;
}

int ICACHE_FLASH_ATTR iws_phone_hot_pro_timeout(wfsmcf_t *smcf)
{
	iws_set_state(smcf, IW_SC_SCAN_AP);
	SMARTCONFIG_ERROR("iws_phone_hot_pro_die\n");
	return RS_OK;
}


int ICACHE_FLASH_ATTR iws_stop_into(wfsmcf_t *smcf)
{
	SMARTCONFIG_ERROR("iws_stop_into\n");
	return RS_OK;
}
#endif

void ICACHE_FLASH_ATTR exit_smartconfig()
{
	SMARTCONFIG_ERROR("exit_smartconfig\n");
	iws_set_state(&smartconfig, IW_SC_STOP);//?
	smartconfig.is_stopsmartconfig = true;
}

smartconfig_stmc_t smcf_proc[IW_SC_MAX] = {
	{"SCAN_AP", iws_scan_ap_into, iws_scan_ap_proc, NULL, NULL, 0},
	{"LISTEN_CHANNEL", iws_listen_channel_into, iws_listen_channel_proc, iws_listen_channel_out, iws_listen_channel_timeout, IW_TIME_N_SECOND(5)},
	{"LOCK_CHANNEL", iws_lock_channel_into, iws_lock_channel_proc, NULL, iws_lock_channel_timeout, IW_TIME_N_SECOND(6)},
	{"LISTEN_CFG", iws_listen_cfg_into, iws_listen_cfg_proc, iws_listen_cfg_out, iws_listen_cfg_timeout, IW_TIME_N_SECOND(10)},
	{"DECODE_CFG", iws_decode_cfg_into, NULL, NULL, NULL, 0},
	{"SMARTCONFIG_OK", iws_ok_into, NULL, NULL, NULL, 0},
	{"PRODUCT_CONNECT", iws_product_into, NULL, NULL, NULL, 0},
	{"PHONE_HOT_CONNECT", iws_phone_hot_connect_into, NULL, NULL, iws_phone_hot_connect_timeout, IW_TIME_N_SECOND(20)},
	{"PHONE_HOT_PRO", iws_phone_hot_pro_into, iws_phone_hot_pro_pro, iws_phone_hot_pro_out, iws_phone_hot_pro_timeout, IW_TIME_N_SECOND(20)},
	{"STOP", iws_stop_into, NULL, NULL, NULL, 0},
};

static int ICACHE_FLASH_ATTR ICACHE_FLASH_ATTR smartconfig_timeout(void *cp)
{
	wfsmcf_t *smcf = (wfsmcf_t *)cp;
	SMARTCONFIG_DEBUG(" smartconfig_timeout  status=%d timeout=%d\n", smcf->state, smcf_proc[smcf->state].timeout);

	if(smcf_proc[smcf->state].tout) {
		smcf_proc[smcf->state].tout(smcf);
	}

	return 0;
}

static int ICACHE_FLASH_ATTR iws_set_state(wfsmcf_t * smcf, int state)
{
	int pre_state;

	pre_state = smcf->state;

	SMARTCONFIG_DEBUG("%s --> %s\r\n", smcf_proc[pre_state].name, smcf_proc[state].name);

	if(STOP_SMCF == smcf->is_stopsmartconfig) {
		SMARTCONFIG_ERROR("is_stopsmartconfig = true\r\n");
		return RS_OK;
	}

	smcf->state = state;

	if (smcf_proc[pre_state].out != NULL) {
		smcf_proc[pre_state].out(smcf);
	}

	iw_timer_delete(TID_WIFI_SCAN_TIMEOUT);

	if (smcf_proc[state].into != NULL) {
		smcf_proc[state].into(smcf);
	}

	if (smcf_proc[state].tout != NULL) {
		iw_timer_set(TID_WIFI_SCAN_TIMEOUT, smcf_proc[state].timeout,
		             0, smartconfig_timeout, (void *)&smartconfig);
	}

	return RS_OK;
}

static int ICACHE_FLASH_ATTR iws_proc(wfsmcf_t *smcf, iws_event_t *e)
{
	int state = smcf->state;

	if (e->event == IW_E_STOP) {
		iws_set_state(smcf, IW_SC_STOP);//?
		return RS_OK;
	}

	if (smcf_proc[state].proc != NULL) {
		smcf_proc[state].proc(smcf, e);
	}

	return RS_OK;
}

static int ICACHE_FLASH_ATTR smartconfig_start_(void *cp)
{
	SMARTCONFIG_DEBUG("smartconfig_start_\r\n");

	iws_set_state(&smartconfig, IW_SC_SCAN_AP);
	return 0;
}

void ICACHE_FLASH_ATTR iw_smartcfg_start(void)
{
	os_memset(&smartconfig, 0, sizeof(smartconfig));

	wifi_station_set_auto_connect(0);

	smartconfig.state = IW_SC_SCAN_AP;
	smartconfig.is_stopsmartconfig = false;

	smartconfig.is_init = true;
	
	iw_timer_set(TID_WIFI_SCAN, TIME_N_SECOND(1), 0,
	             smartconfig_start_, (void *)&smartconfig);
}

