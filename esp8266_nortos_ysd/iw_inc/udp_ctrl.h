#ifndef		__UDP_CTRL_H__
#define	__UDP_CTRL_H__
#include "stlc_list.h"
//#include "lib.h"
#include "iw_func_support.h"
#include "c_types.h"
//#define	EMPTY_FUNC	log_err(false, "\n\n\nEEEEEEEE please write function %s \n\n\n", __FUNCTION__)

/*****************************************
	UDP����Э�鱨��ͷ(CMD_UDP_XXX)
 *****************************************/

// ���غ�
#define	MAX_UC_PKT_SIZE	1500
// UDP CTRL ͨ�ñ��ĵ�TTL���ݴ�С:object + sub object + attri + param_len
#define UDP_CTRL_TL_SIZE 8
// UDP CTRL ͨ�ñ��ĵ�TTL���ݴ�С:object + sub object + attri 
#define UDP_CTRL_TL_SIZE_NO_LEN 6

#pragma pack(push,1)
/*
	udp ctrl packet header
*/
typedef struct{
#ifdef LITTLE_ENDIAN
	u_int8_t ver:3,
		hlen:3,
		request:1,
		encrypt:1;
#else
	u_int8_t 	encrypt:1,
		request:1,
		hlen:3,
		ver:3;
#endif
	u_int8_t flags;
	u_int8_t request_id;
	u_int8_t resv;
	// session id
	u_int32_t client_sid;
	u_int32_t device_sid;

	// ���濪ʼ���ܼ���
	u_int16_t command;
	u_int16_t param_len;
} ucph_t;

#define BOFP(buf, ofs) (&((char *)(buf))[ofs])

#define	ucph_hdr_len(ucph)	(((ucph_t *)ucph)->hlen<<2)
#define ucph_hdr_size (sizeof(ucph_t))
#define	get_ucp_payload(pkt, type) (type *)BOFP((pkt->data), ucph_hdr_len((pkt->data)))
#define	get_net_ucp_payload(hdr, type) (type *)BOFP(hdr, ucph_hdr_len(hdr))

// ͷ���������ĵĳ���
#define	ucph_hdr_plain_size	12

// udp control action
#define	UCA_NONE		0
#define	UCA_REQUEST		1
#define	UCA_REPLY		2
#define	UCA_QUESTION	3
#define	UCA_ANSWER		4
#define	UCA_RESULT		5
#define	UCA_REDIRECT	6
#define	UCA_RESET		7
#define	UCA_GET			8
#define	UCA_SET			9
#define	UCA_DELETE		10
#define UCA_PUSH		11


/*
	�ỰID�ķ���
*/
#define	UC_SID_INVALID	0
// 0 - 0x10000000 �������豸�����������������ֱ�����ֻ�
#define	UC_SID_DEV_RESV_BEGIN	0x00000001
#define	UC_SID_DEV_RESV_END	0x0FFFFFFF

// ������������������豸
#define	UC_SID_SERV_DEV_BEGIN_1	0x80000000
#define	UC_SID_SERV_DEV_END_1	0x8FFFFFFF

#define	UC_SID_SERV_DEV_BEGIN_2	0x90000000
#define	UC_SID_SERV_DEV_END_2	0x9FFFFFFF


// ��������������������������ӵ��ֻ�
#define	UC_SID_SERV_CLIENT_BEGIN	0xA0000000
#define	UC_SID_SERV_CLIENT_END	0xAFFFFFFF


// δ��֤�ɹ���������״̬������ʱ��
#define	DFL_TIME_UCS_HALF_DIE	5
// ɾ�����ӵ���ʱ
#define	DFL_TIME_UCS_DEL_DELAY	3

// �ͻ�����IDLE״̬�ȴ����ٺ���󣬽�����һ�γ���
#define	DFL_UC_IDLE_TIME	3000
//���Ϊ��¡��ʱ��ʱ���ӳ�
#define	CLONE_UC_IDLE_TIME	6000

#define	log_err(print_err, fmt, ...)

#define	UCDS_CHECK_PLEN(s, hdr, min_len) \
	do { \
		if (hdr->param_len < min_len) { \
			log_err(false, "%s recv bad pkt: cmd=%u, param_len=%u, min_len=%u\n", s->name, hdr->command, hdr->param_len, min_len); \
			return RS_ERROR; \
		} \
	} while (0)

#define	UCDS_CHECK_PLEN_VOID(s, hdr, min_len) \
	do { \
		if (hdr->param_len < min_len) { \
			log_err(false, "%s recv bad pkt: cmd=%u, param_len=%u, min_len=%u\n", s->name, hdr->command, hdr->param_len, min_len); \
			return; \
		} \
	} while (0)
	
/*****************************************
	״̬
 *****************************************/

// udp ctrl server status
enum {
	UCSS_QUESTION	= 0,
	UCSS_ESTABLISH	 = 1,
	UCSS_DIE = 2,
	UCSS_MAX = 3
};

typedef void (* ucs_func_t)();

typedef struct {
	char *name;
	// ���뱾״̬���õĺ���
	ucs_func_t on_into;
	// ��������
	ucs_func_t proc_pkt;
} ucs_proc_t;


// udp ctrl client status
enum {
	UCCS_IDLE = 0,
	UCCS_AUTH_REQ = 1,
	UCCS_AUTH_ANSWER = 2,
	UCCS_ESTABLISH = 3,
	UCCS_ERROR = 4,
	UCCS_MAX = 5
};

typedef void (* ucc_func_t)();

typedef struct {
	char *name;
	// ���뱾״̬���õĺ���
	ucc_func_t on_into;
	// �뿪��״̬���õĺ���
	ucc_func_t on_out;
	// ��������
	ucc_func_t proc_pkt;
} ucc_proc_t;


/*
	�������ݵķ��������ؼ��ܺ�ĳ���
*/
typedef int (*ucs_enc_func_t)(void *session, u_int8_t *data, int len);
/*
	�������ݵķ��������ؽ��ܺ�ĳ���
*/
typedef int (*ucs_dec_func_t)(void *session, u_int8_t *data, int len);


/*****************************************
	��֤���ĸ�ʽ (CMD_UDP_AUTH)
 *****************************************/


#define MAX_UUID_BIN_LEN 16

// type define for TLV
#define	UCT_VENDOR			1
#define	UCT_LOCAL_TIME		2
#define	UCT_TIME_PARAM		3
#define	UCT_E_TM_PRICE		4
#define UCT_CHIP_ATTISTR_PHONE 	6 //͸��ģʽ�£���ȡ�豸�����ַ�����ͨ����ֻ���������ͳһ��7��Ϊ�˼����ֻ��˴��룬����ʹ��6
#define UCT_DEV_INFO			8
#define UCT_SMARTCFG_TIME	  14  //һ�����óɹ���ʱ��
#define UCT_CLONE_PKT_SEND_ENABLE 15 //�������·����Ƿ��ϱ���¡����

#define UCT_LINK_SUPPORT			24  //�ڸ���������֤��answer���ĵ�tlv��type����


#define	tlv_next(tlv) (uc_tlv_t *)BOFP((uc_tlv_t *)tlv + 1, tlv->len)
#define	tlv_n_next(tlv) (uc_tlv_t *)BOFP((uc_tlv_t *)tlv + 1, ntohs(tlv->len))
#define	tlv_val(tlv) ((u_int8_t *)((uc_tlv_t *)tlv + 1))

typedef struct {
	// UCT_XX
	u_int16_t type;
	// ���ݳ��ȣ�������ͷ��
	u_int16_t len;
} uc_tlv_t;


/******************debug info********************************/
#define DBG_TLV_HD_LEN		sizeof(uc_tlv_t)
//SVN��
#define DBG_TYPE_SVN		1
//����
#define DBG_TYPE_CUR		2
//������ӦADֵ
#define DBG_TYPE_CUR_AD		3
//����kֵ
#define DBG_TYPE_CUR_K		4
//����bֵ
#define DBG_TYPE_CUR_B		5
//��ѹ
#define DBG_TYPE_VOL		6
//��ѹ��ӦADֵ
#define DBG_TYPE_VOL_AD		7
//��ѹkֵ
#define DBG_TYPE_VOl_K		8
//��ѹbֵ
#define DBG_TYPE_VOL_B		9
//����ADֵ 
#define DBG_TYPE_LIGHT_AD	10
//���ӷ���������
#define DBG_TYPE_SERVER_DONAME	11
//���ӷ�����ʱ��
#define DBG_TYPE_SERVER_CONNTIME 12
//��ǰ�������ֻ���������
#define DBG_TYPE_CLIENTS		13
//�豸uptime
#define DBG_TYPE_UPTIME			14
//���ѧϰ
#define DBG_TYPE_LIGHT_STUDY		15
//cpuʹ�ðٷֱ�
#define DGB_TYPE_CPU_PERCENTAGE		16
//memʹ�ðٷֱ�
#define DBG_TYPE_MEM_PERCENTAGE		17

//����ƽ������
#define DBG_TYPE_AVG_AD		18
//����������
#define DBG_TYPE_MAX_AD		19
//��������ͻ���ӳ�ʱ��
#define DBG_TYPE_PN_TIME   20
//�ػ�����ͻ��ʱ��		
#define DBG_TYPE_PF_TIME   21
//���ص���adֵ
#define DBG_TYPE_NO_LOAD_AD 22
//��Ƭ�������汾
#define DBG_TYPE_SMT_SOFT_VER  23
//��Ƭ��Ӳ���汾
#define DBG_TYPE_SMT_HARD_VER  24
//���������id
#define DBG_TYPE_IR_LIB_ID	   25

#define DBG_TYPE_COLD_DELAY_PN_TIME 26
#define DBG_TYPE_COLD_DELAY_PF_TIME 27
#define DBG_TYPE_HOT_DELAY_PN_TIME  28
#define DBG_TYPE_HOT_DELAY_PF_TIME  29

//��Ƭ��������Ϣ
#define DGB_TYPE_STM32_INFO		30

//��Ƭ���ȴ�У��
#define DBG_TYPE_STM_WAIT_ADJUST	31
//dev server ip
#define DBG_TYPE_DEVSERVERIP		32

//����WiFi(·����)��ʱ��
#define DBG_TYPE_TIME_CONN_WIFI		34
//��ǰ����WiFi���ź�ǿ��
#define DBG_TYPE_WIFI_RSSI   		35
//��ǰ����WiFi��ģʽ(phy_mode)
#define DBG_TYPE_WIFI_PHY_MODE		36
//����SDK�汾
#define DBG_TYPE_WIFI_SDK_VERSION	37


typedef struct {
	u_int16_t year;
	u_int8_t month;
	u_int8_t day;
	u_int8_t hour;
	u_int8_t minute;
	u_int8_t second;
	u_int8_t timezone;
} uc_time_t;

#if 1
//������ݽṹ
enum {
	EP_TYPE_HIGH = 1,
	EP_TYPE_LOW = 2,
	EP_TYPE_LEVEL = 3,
};

//���ݵ��:��ֹʱ��-���
typedef struct {
	u_int8_t begin_hour;
	u_int8_t begin_minute;
	u_int8_t end_hour;
	u_int8_t end_minute;
	u_int8_t tm_type;//��1 ��2 ƽ3 EP_TYPE_XXX
	u_int8_t pad;
	u_int16_t price;//��ۣ���λΪ�֣���168��ʾ1Ԫ6ë8
} uc_e_tm_t;

typedef struct {
	u_int8_t type;	//����(ȱʡ):0  ��ҵ:1 
	u_int8_t count;	//uc_e_tm_t����
	u_int8_t pad[2];
//	uc_e_tm_t item[0];//uc_e_tm_t
} uc_e_price_t;

#endif

/*
	�ش�ʱ�䡢����ʱ�䡢����ʱ��
*/
typedef struct {
	// �������ش�ʱ�����������κ�����ش�ʱ�䶼�������һ���ˡ���λ100ms
	u_int8_t retry_100ms[3];
	// ���ͱ��������ʱ��������λ��
	u_int8_t keeplive;
	// ������ʱ��������λ��
	u_int16_t die;
} uc_time_param_item_t;

typedef struct {
	// �豸���������
	uc_time_param_item_t dev;
	// �ֻ����豸������ֱ��
	uc_time_param_item_t lan;
	// �ֻ����豸���������ӣ�APP��ǰ������
	uc_time_param_item_t wan_front;
	// �ֻ����豸���������ӣ�APP�ں������
	uc_time_param_item_t wan_background;
} uc_time_param_all_t;


typedef struct {
	// UAA_XXX
	u_int8_t action;
             u_int8_t flag_bits;
	u_int8_t reserved[2];
	// �����
	u_int8_t rand1[4];
	u_int64_t sn;
	// �ͻ���Ψһ��ʶ
	u_int8_t my_uuid[MAX_UUID_BIN_LEN];
	
	// ��ǰ����ʱ�ӣ����ڿͻ��˸��豸ͬ��ʱ�䡣���ɿ���Դ
	uc_time_t time;
} uc_auth_request_t;

typedef struct {
	// UAA_XXX
	u_int8_t action;
	u_int8_t reserved[3];

	u_int32_t ip;
	u_int16_t port;
	u_int16_t pad;
} uc_auth_redirect_t;

typedef struct {
	// UAA_XXX
	u_int8_t action;
	u_int8_t reserved[3];
	// �����
	u_int8_t rand2[4];
	u_int8_t your_uuid[MAX_UUID_BIN_LEN];
} uc_auth_question_t;

#define	UC_ENC_NONE	0x0000
#define	UC_ENC_AES128	0x0001

typedef struct {
	// UAA_XXX
	u_int8_t action;
	u_int8_t reserved;
	// UC_ENC_xxx
	u_int16_t support_enc;

	u_int8_t answer[16];
	// ��ǰ����ʱ�ӣ����ڿͻ��˸��豸ͬ��ʱ�䡣�ɿ���Դ
	uc_time_t time;
} uc_auth_answer_t;

typedef struct {
	// UAA_XXX
	u_int8_t action;
	u_int8_t reserved1;
	u_int16_t select_enc;

	u_int16_t err_num;
	u_int16_t reserved2;

	// Ӧ��Ϊ�豸��ʱ��Ч��Ϊ������ʱ����
	u_int8_t dev_type;
	u_int8_t ext_type;
	u_int8_t login_type; /* LT_NORMAL, LT_BIND, LT_UNBIND */
	u_int8_t net_type; /* NT_SERVER, NT_DEVICE, NT_DEVICE_OFFLINE */

	// UCT_VENDOR, UCT_LOCAL_TIME, UCT_TIME_PARAM
	//�������tlv
	//uc_tlv_t *tlv;
} uc_auth_result_t;

/*****************************************
	����ĸ�ʽ (CMD_UDP_KEEPLIVE)
 *****************************************/


typedef struct {
	u_int8_t action;
	u_int8_t flags;
	u_int16_t reserved;
	uc_tlv_t *tlv;
} uc_keeplive_request_t;

typedef struct {
	u_int8_t action;
	u_int8_t reserved[3];
	//�������tlv
	//uc_tlv_t *tlv;
} uc_keeplive_reply_t;

typedef struct {
	u_int8_t action;
	u_int8_t reserved[3];
} uc_keeplive_reset_t;

/* ͸�����Transparent transmission */

/* Transparent transmission ability */
#define TT_ABLILITY BIT(0)

/*͸������ʹ��TLV��ʽ������չ*/

#define UCT_TT_RAW_CHIPDATA 1 //���͸�����оƬ��ԭʼ����



/*****************************************
	���Ʊ��ĸ�ʽ (CMD_UDP_CTRL)
 *****************************************/


/*****************************************
	UDP Control Object Type 
	UDP Control Sub Object Type
	UDP Control Attribute Type
 ****************************************/
 
/*
	system. (ϵͳ)
*/
#define	UCOT_SYSTEM	1

// system.support.
#define	UCSOT_SYS_SUPPORT	1
// system.support.support
#define	UCAT_SUPPORT_SUPPORT	1

// system.support.
#define	UCSOT_SYS_HOSTNAME	2
// system.hostname.hostname
#define	UCAT_HOSTNAME_HOSTNAME	1

// system.software.
#define	UCSOT_SYS_SOFTWARE	3
// system.software.version
#define	UCAT_SYS_VERSION	100 /* ���ܱ�����Ӷ����õģ��Ӵ�һ��Ķ��� */
//system.software.upgrade
#define UCAT_SYS_UPGRADE	101	/*��������*/
#define UCAT_SYS_STM_UPGRADE 102 /*stm��Ƭ������*/
//system.software.reboot
#define UCAT_SYS_REBOOT		1
//system.software.dev_uptime
#define UCAT_SYS_UPTIME		2
//system.software.ip
#define UCAT_SYS_IP			3
//system.software.svn		     
#define UCAT_SYS_SVN		4
//system.software.ssid
#define UCAT_SYS_SSID		5
//system.software.passwd			
#define UCAT_SYS_PASSWD		6
//system.software.devstatus
#define UCAT_SYS_DEVSTATUS	7
//system.software.compile_date
#define UCAT_SYS_COM_DATE	8
//system.software.compile_time
#define UCAT_SYS_COM_TIME	9
//system.software.systime
#define UCAT_SYS_SYSTIME	10
//system.software.setsystime
#define UCAT_SYS_SET_SYSTIME	11
//system.software.debuginfo
#define UCAT_SYS_DEBUGINFO	12
//system.software.reset
#define UCAT_SYS_RESET	    13





// system.upgrade.
#define	UCSOT_SYS_UPGRADE	4

// system.hardware.
#define	UCSOT_SYS_HARDWARE	5
// system.hardware.LED
#define	UCATT_HARDWARE_LED	1

// system.vendor.
#define	UCSOT_SYS_VENDOR	6
// system.vendor.oem_id
#define	UCAT_VENDOR_OEM_ID	1

// system.server.
#define	UCSOT_SYS_SERVER	7
// system.server.connect_time
#define	UCAT_SERVER_CONNECT_TIME	1
//system.server.domainname
#define UCAT_SERVER_DONAME	2
//system.server.ip
#define UCAT_SERVER_IP		3
#ifdef LINKAGE_SUPPORT
//system.server.monitor
#define UCAT_SERVER_MONITOR	4
//system.server.joinhome
#define UCAT_SERVER_HOME_ID	5
#endif


// system.user
#define UCSOT_SYS_USER		8 /*һЩ�û�������Ϣ*/
// system.user.passwd
#define UCAT_USER_PASSWD	1

//system.licence
#define UCSOT_SYS_LICENCE	9
//system.licence.active
#define UCAT_SYS_LICENCE_ACTIVE 1


/*
	IA(���ܵ���)
*/
// IA.
#define	UCOT_IA	2

// IA.stat.
#define	UCSOT_IA_STAT	1
// IA.stat.total_peak
#define	UCAT_STAT_TOTAL_PEAK	1
// IA.stat.total_valley
#define	UCAT_STAT_TOTAL_VALLEY	2
// IA.stat.period_time
#define	UCAT_STAT_PERIOD_TIME	3
// IA.stat.period_peak
#define	UCAT_STAT_PERIOD_PEAK	4
// IA.stat.period_valley
#define	UCAT_STAT_PERIOD_VALLEY	5
// IA.stat.month_peak
#define	UCAT_STAT_MONTH_PEAK	6
// IA.stat.month_valley
#define	UCAT_STAT_MONTH_VALLEY	7
// IA.stat.cur_power
#define	UCAT_STAT_CUR_POWER	8
// IA.stat.peak_time
#define	UCAT_STAT_PEAK_TIME	9
// IA.stat.peak_price
#define	UCAT_STAT_PEAK_PRICE	10
// IA.stat.valley_price
#define	UCAT_STAT_VALLEY_PRICE	11
// IA.stat.month_flat
#define	UCAT_STAT_MONTH_FLAT	12
// IA.stat.month_flat
#define UCAT_STAT_FLAT_TIME		13
// IA.stat.flast_price
#define	UCAT_STAT_FLAT_PRICE	14
// IA.stat.valley_time
#define	UCAT_STAT_VALLEY_TIME	15


// IA.AIRCONDITION.
#define	UCSOT_IA_AC	2
// IA.AIRCONDITION.WORK
#define	UCAT_AC_WORK	1
// IA.AIRCONDITION.CUR_TEMP
#define	UCAT_AC_CUR_TEMP	2
// IA.AIRCONDITION.TIMER
#define	UCAT_AC_TIMER	3
//IA.AIRCONDITION.SMART_ON
#define UCAT_AC_SMART_ON		4
//IA.AIRCONDITION.SMART_OFF
#define UCAT_AC_SMART_OFF		5
//IA.AIRCONDITION.SMART_SLEEP
#define UCAT_AC_SMART_SLEEP		6
//IA.AIRCONDITION.SOUND_MATCH_REQUEST
#define UCAT_SOUND_MATCH_REQUEST 7
//IA.AIRCONDITION.SOUND_DATA			 
#define UCAT_SOUND_DATA			 8
//IA.AIRCONDITION.SOUND_MATCH_RESULT
#define UCAT_SOUND_MATCH_RESULT	 9
// IA.AIRCONDITION.ATTRI
#define UCAT_AC_ATTRI		10


// IA.CODE
#define	UCSOT_IA_CODE		3
#define UCAT_CODE_PROCESS		1
#define UCAT_CODE_MATCH		2
#define UCAT_CODE_LIST		3
#define UCAT_CODE_INFO		4
#define UCAT_CODE_DATA		5

/*
*HEATER_LCYT �͵�֧��
*/
//IA.HEATER_LCYT
#define UCOT_IA_HEATER_LCYT 			4
//IA.HEART_LCYT.WORK
#define UCAT_HEATER_LCYT_WORK			1
//IA.HEART_LCYT.MODE
#define UCAT_HEATER_LCYT_MODE			2
//IA.HEART_LCYT.GEAR
#define UCAT_HEATER_LCYT_GEAR			3	
//IA.HEART_LCYT.CUR_TEMP
#define UCAT_HEATER_LCYT_CUR_TEMP		4
//IA.HEART_LCYT.SET_TEMP				
#define UCAT_HEATER_LCYT_SET_TEMP		5
//IA.HEART_LCYT.SET_TIME
#define UCAT_HEATER_LCYT_SET_TIME		6
//IA.HEART_LCYT.REMAIN_TIME	
#define UCAT_HEATER_LCYT_REMAIN_TIME	7

/*
*��������¯����
*/
//IA.FAST_HOT_FURNACE
#define UCOT_IA_FHF					5
//IA.FAST_HOT_FURNCE.WORK
#define UCAT_FHF_WORK				1
//IA.FAST_HOT_FURNCE.SET_TIMER
#define UCAT_FHF_SET_TIMER			2
//IA.FAST_HOT_FURNCE.GET_TIME
#define UCAT_FHF_GET_TIMER			3
//IA.FAST_HOT_FURNCE.POWER
#define UCAT_FHF_GET_POWER			4
//IA.FAST_HOT_FURNCE.SET_TEMP	
#define UCAT_FHF_SET_TEMP			5
//IA.FAST_HOT_FURNCE.GET_TEMP
#define UCAT_FHF_GET_TEMP			6
//IA.FAST_HOT_FURNCE.GET_SN
#define UCAT_FHF_GET_SN				7

/*
���ƿ���������
*/
//IA.AIR_CLEANER
#define UCOT_IA_HKAC					6
//IA.HK_AIR_CLEANER.WORK
#define UCAT_HKAC_WORK				1
//IA.HK_AIR_CLEANER.MODE
#define UCAT_HKAC_MODE				2
//IA.HK_AIR_CLEANER.WIND
#define UCAT_HKAC_WIND				3
//IA.HK_AIR_CLEANER.TEMP
#define UCAT_HKAC_TEMP				4
//IA.HK_AIR_CLEANER.PM25
#define UCAT_HKAC_PM25				5
//IA.HK_AIR_CLEANER.HUMIDITY
#define UCAT_HKAC_HUMIDITY				6
//IA.HK_AIR_CLEANER.ANION_UVL
#define UCAT_HKAC_ANION_UVL			7
//IA.HK_AIR_CLEANER.TIMER
#define UCAT_HKAC_TIMER				8

// ��Ѷ�����
#define UCOT_IA_GX_LED		7
#define UCAT_GX_LED_STATUS	1


//���ܵ���֮͸������: Transparent transmission
//IA.TT
#define UCSOT_IA_TT  11

//���ܵ�����������
//IA.TT. ALLSTATE ��ѯ��push���е�״̬�������Է����ֻ�ͳһ����������WIFIģ�鶨ʱ��ѯά����ֵ
#define UCAT_IA_TT_ALLSTATE 1
//IA.TT.command ctrl �������ѯ����
#define UCAT_IA_TT_CMD 2
/*
 * E��
 */
//IA.EB
#define UCOT_IA_EB			8
//IA.EB.WORK
#define UCAT_EB_WORK		1
//IA.EB.TIMER
#define UCAT_EB_TIMER		2


//���ܵ�����������
//IA.PUBLIC
#define UCSOT_IA_PUBLIC 12
//IA.PUBLIC. PERIOD_TIMER ʱ��ζ�ʱ��
#define UCAT_IA_PUBLIC_PERIOD_TIMER  1

//ͨ���ֻ���������·������ssid��password
#define UCAT_IA_PUBLIC_WIFI_SET            3


/*
*LEDE led
*/
//IA.LEDE_LED
#define	UCOT_IA_LEDE_LED				19
//IA.LEDE_LED.STATE
#define UCAT_LEDE_LED_STATE				1
//IA.LEDE_LED.TIMER
#define UCAT_LEDE_LED_TIMER				2


/*
	STORAGE(�洢)
*/
// storage.
#define	UCOT_STORAGE	3

// storage.string.
#define	UCSOT_STORAGE_STRING	1
// storage.binary.
#define	UCSOT_STORAGE_BINARY	2
// storage.packet.
#define	UCSOT_STORAGE_PACKET	3

// storage.packet.receive
#define	UCAT_PACKET_RECEIVE	1
// storage.packet.send
#define	UCAT_PACKET_SEND	2

//ͨ���0xFFFF
#define ATTR_ALL	(0xFFFF)
#define MAX_VENDER 16



typedef struct {
	u_int16_t objct;
	u_int16_t sub_objct;
	u_int16_t attr;
	u_int16_t param_len;
	// u_int8_t param[0];
} ucp_obj_t;

typedef struct {
	// UCA_XXX
	u_int8_t action;
	u_int8_t count;
	u_int16_t reserved;
	// ucp_obj_t objs[0];
} ucp_ctrl_t;

typedef struct {
	// ���汾��
	u_int8_t major;
	// �ΰ汾��
	u_int8_t minor;
	// �޶��汾��
	u_int8_t revise;
	u_int8_t reserved;
} ucp_version_t;

typedef struct {
	u_int8_t user_enable;
	u_int8_t device_enable;
} ucp_led_t;

typedef struct {
	ucp_version_t soft_ver;
    ucp_version_t stm_ver;//��Ƭ���汾
	u_int8_t dev_type;
	u_int8_t ext_type;
	u_int8_t vendor[MAX_VENDER];
	u_int8_t pad[2];
	ucp_version_t hard_ver;
    ucp_version_t stm_hard_ver; //��Ƭ��Ӳ���汾
}dev_info_t;


// ����
#define	AC_POWER_ON	0
#define	AC_POWER_OFF	1

// ģʽ
#define	AC_MODE_AUTO	0
#define	AC_MODE_COLD	1
#define	AC_MODE_AREFACTION 2
#define	AC_MODE_WIND	3
#define	AC_MODE_HOT	4

#define	AC_TEMP_BASE	16

// ����
#define	AC_WIND_AUTO	0
#define	AC_WIND_1	1
#define	AC_WIND_2	2
#define	AC_WIND_3	3

// ����
#define	AC_DIR_AUTO	0
#define	AC_DIR_1	1
#define	AC_DIR_2	2
#define	AC_DIR_3	3

// ��ֵ
#define	AC_KEY_POWER	0
#define	AC_KEY_MODE	1
#define	AC_KEY_TEMP	2
#define	AC_KEY_WIND	3
#define	AC_KEY_DIR		4

// ��ʽ�յ���ʵ�ʰ���
#define AC_CKEY_POWER 0
#define AC_CKEY_MODE_AUTO 1
#define AC_CKEY_MODE_COLD 2
#define AC_CKEY_MODE_AREFACTION 3
#define AC_CKEY_MODE_WIND 4
#define AC_CKEY_MODE_HOT 5
#define AC_CKEY_TEMP_UP 6
#define AC_CKEY_TEMP_DOWN 7
#define AC_CKEY_WIND_AUTO 8
#define AC_CKEY_WIND_LOW 9
#define AC_CKEY_WIND_MID 10
#define AC_CKEY_WIND_HIGH 11
#define AC_CKEY_DIR_MANUAL 12
#define AC_CKEY_DIR_AUTO 13

typedef struct {
	// AC_POWER_xxx
	u_int8_t onoff;
	// AC_MODE_XXX
	u_int8_t mode;
	// ʵ���¶� �� temp - AC_TEMP_BASE
	u_int8_t temp;
	// ���٣�AC_WIND_xxx
	u_int8_t wind;
	// ����AC_DIR_xxx
	u_int8_t direct;
	// ��ֵ��AC_KEY_xxx
	u_int8_t key;
	// ��ʽ�յ���ǰ����, AC_CKEY_xxx
	//u_int8_t ckey;
} ucp_ac_work_t;

typedef struct {
	u_int8_t id;
	u_int8_t enable;
	u_int8_t week;
	u_int8_t hour;
	u_int8_t minute;
	// AC_POWER_ON / AC_POWER_OFF
	u_int8_t onoff;
	u_int16_t reserved;
} ucp_ac_timer_item_t;

typedef struct {
	u_int8_t on_effect;
	u_int8_t reserved1;
	// ��һ�ζ�ʱ����ʱ�������ڻ��м�����
	u_int16_t on_minute;
	
	u_int8_t off_effect;
	u_int8_t reserved2;
	// ��һ�ζ�ʱ�ػ�ʱ�������ڻ��м�����
	u_int16_t off_minute;

	// ucp_ac_timer_item_t item[0];
} ucp_ac_timer_t;


typedef struct {
	// ��ʼʱ�䣬��λ�����ӣ�0:0����
	u_int16_t begin_minute;
	// �����೤����λ����
	u_int16_t last_minute;
} ucp_stat_peak_time_t;

// IA.CODE.PROCESS
enum {
	CODE_PROCESS_ACTION_CANCEL = 0,
	CODE_PROCESS_ACTION_MATCH = 1,
	CODE_PROCESS_ACTION_ALL_MATCH = 2,
	CODE_PROCESS_ACTION_COMPLETE = 3,
	CODE_PROCESS_ACTION_ERR = 4,
};

enum {
	CODE_PROCESS_TYPE_WAIT_IR = 0,
	CODE_PROCESS_TYPE_MATCHING = 1,
};

enum {
	//��ƥ��ȴ������źų�ʱ
	ERR_CODE_PROCESS_WAIT_IR_TIMEOUT = 1,
	//����ƥ��
	ERR_CODE_PROCESS_MATCHING = 2,
	//���ƶ˻�ȡcode��ʱ
	ERR_CODE_PROCESS_GET_CODE_TIMEOUT = 3,
	//û��ƥ�䵽����
	ERR_CODE_PROCESS_NO_CODE_MATCH = 4,
	// ������û�������ϣ�������ͨ
	ERR_CODE_PROCESS_SERVER_NOT_READY = 5,
};

typedef struct {
	// CODE_PROCESS_ACTION_XXX
	u_int8_t action;
	u_int8_t time_out;
	u_int16_t pad;
} ucp_code_process_request_t;

typedef struct {
	// CODE_PROCESS_ACTION_XXX
	u_int8_t action;
	u_int8_t step_type;
	u_int8_t step_count;
	u_int8_t step_index;
	u_int8_t err;
	u_int8_t pad[3];
} ucp_code_process_reply_t;

typedef struct {
	u_int8_t err;
	u_int8_t pad;
	u_int16_t lib_id;
	u_int8_t lib_name_len;
} ucp_code_info_reply_head_t;

typedef struct {
	u_int16_t lib_id;
	u_int16_t idx_begin;
	u_int16_t idx_end;
} ucp_code_data_reqeust_head_t;

/* ����ƥ������ݽṹ */
typedef struct {
	/*sample rate, 8000*/
	u_int16_t sample;
	/*sample bit, 8*/
	u_int8_t bit;
	/*sample time*/
	u_int8_t time;
} ucp_ac_sound_match_request_t;

typedef struct {
	/*match result. 1:match OK, 0:match failed*/
	u_int8_t result;
	/*next sample time*/
	u_int8_t next_time;
	u_int8_t pad[2];
} ucp_ac_sound_match_result_t;

/*ͨ��Э�����ݽṹ1�ֽڶ��룬�����ݽṹ�����ڱ���֮ǰ*/
#pragma pack(pop)


/*****************************************
	���ú���
 *****************************************/
 typedef struct _pkt_t {
	struct stlc_list_head link;
	// �ܳ���
	int total;
	// ���ܺ󲻺�֪����������һ�����ڵ���
	int cmd;
	int type;
	// net_header_t *
	u_int8_t *data;
} pkt_t;

/* External function declarations. */
/*
	����:
		��̬����һ���ڴ棬 ��������ز���
	�������:
		@cmd: ����
		@param_len: ��������
		@is_request: ������1��Ӧ����0��
		@is_enc: �Ƿ����
		@flags: 
		@client_sid: �ͻ��˻ỰID���ɷ��������䣬0������
		@device_sid: �豸�˻ỰID���ɷ��������䣬0������
		@request_id: ����id
	�������:
	����ֵ:
		pkt_t *: �ɹ�
		NULL: ʧ��
	˵��:
		����Ĵ�С: ������һ���ڴ棬���ڼ���ʱ���� + sizeof(pkt_t) + ucph_hdr_size + param_len;
*/
extern pkt_t *uc_pkt_new(int cmd, int param_len, 
			bool is_request, bool is_enc, u_int8_t flags,
			u_int32_t client_sid, u_int32_t device_sid, u_int8_t request_id);

/*
	����:
		��ucp_obj_t����ṹ��������ݴ������ֽ���->�����ֽ���
	�������:
		@uobj: ��������
	�������:
	����ֵ:
	˵��:
*/
extern void ucp_obj_order(ucp_obj_t *uobj);

#endif
