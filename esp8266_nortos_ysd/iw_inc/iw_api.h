#ifndef __IWUOS_API_HEADER__
#define __IWUOS_API_HEADER__

#include "c_types.h"

/*
	�������÷���ֵ
*/
typedef int rs_t;

typedef int RS;
#define	RS_OK				0 		/* �����ɹ���� */
#define	RS_ERROR			-1		/* ���� */
#define	RS_NOT_INIT		-2
#define	RS_NOT_SUPPORT		-3
#define	RS_NOT_LOGIN		-4		/* ��δ��¼ */
#define	RS_INVALID_PARAM	-5		/* ��Ч�Ĳ��� */
#define	RS_EMPTY			-6		/* û�п������� */
#define RS_MEMORY_MALLOC_FAIL  -7	/* �ڴ����ʧ�� */
#define	RS_TOO_MANY	-8
#define RS_NTP_NONE -9

#define	RS_EXIST			1		/* ����ʱ�Ѵ��� */
#define	RS_NOT_FOUND		2		/* δ�ҵ� */
#define RS_INVALID_LICENSE 	3		/*��Ч��Ȩ�ļ�*/
#define RS_OFFLINE 			4		/* �豸���� */
#define RS_DOMAIN_SAME      5       /* ��ͬ������ */


#define IW_MAX_DESC 64
#define IW_MAX_WIFI_SSID 32
#define IW_MAX_WIFI_PASSWD 64

#define MAX_VENDOR_LEN 16
#define MAX_DESC_LEN 32


typedef enum{
	SMCF_STA_NONEED_SMCF = 1,//����Ҫһ������
	SMCF_STA_ISSCAN_AP,//ɨ��ap�����Ƿ��в���,�ֻ��ȵ��ssid����
	SMCF_STA_SNIFF,//����sniff����
	SMCF_STA_ISLOCKAP,//������ap�ŵ�,�����հ���
	SMCF_STA_CONFOK,//һ�����óɹ�
	SMCF_STA_CONNECTAP,//������AP
	SMCF_STA_GETIP,//������AP,���һ�ȡ��ip
	SMCF_STA_DISCONNECTAP//���ӶϿ�
}smcf_sta_t;


//�¼�����
typedef struct event_s{
	bool have_event; 
}event_t;

enum {
	EVENT_SYS_SAVE_CONFIG,	// ���󱣴�����
	EVENT_SYS_REBOOT,		// ��������ϵͳ
	EVENT_SYS_RESET, 	//reset
	EVENT_SETUP_TIMER,
	EVENT_SYS_WIFI_CONFIG,
	EVENT_SYS_MAX
};

typedef struct sys_shm_s{
	event_t event[EVENT_SYS_MAX];
}sys_shm_t;


/*
	ϵͳ�����Ϣ
*/
typedef struct {
	// ϵͳ������Ϣ������ֶ��ÿո����
	char descript[IW_MAX_DESC];
	// ���к�
	u_int64_t sn;
	u_int32_t version;
	
	// ϵͳ�ж��ٸ�GPIO�ڡ���0��ʼ˳���š�
	u_int16_t num_gpio;
	// ϵͳ�ж��ٸ����ڡ���0��ʼ˳���š�
	u_int16_t num_uart;
	// ϵͳ���������ٸ��ͻ���ͬʱ��¼
	u_int16_t max_client;
	// ϵͳ������ഴ�����ٸ���ʱ��
	u_int16_t	max_timer;
	
	bool is_smart_link_config;
	char ssid[IW_MAX_WIFI_SSID];
	char passwd[IW_MAX_WIFI_PASSWD];
	u_int8_t smcf_stat;//һ�����õ�״̬����
	
	// Wi-Fi�Ƿ����ӳɹ�
	bool is_linkup;
	// ��Wi-Fi���ӳɹ������ڹ��˶�����
	u_int32_t linkup_time;
	// ��AP���䵽��IP��ַ
	u_int32_t ip;
	// ��AP���䵽����������
	u_int32_t mask;
	// ���ص�ַ
	u_int32_t gateway;
	// ������������ַ
	u_int32_t dns;

	// ���Ʒ������Ƿ���������
	bool is_server_connect;
	// ���Ʒ������������ӵ����ڹ��˶�����
	u_int32_t server_connect_time;
	u_int32_t up_time;
	
	// ��·���������˶�����
	u_int32_t wifi_connect_time;
} iw_sys_info_t;

/*
	��ʼ����Ҫ����Ϣ
*/
typedef struct {
	// ����ID
	char vendor[MAX_VENDOR_LEN];
	// ��Ʒ����
	char desc[MAX_DESC_LEN];
} iw_init_t;

/**************************************************
 	ϵͳ����
 **************************************************/

/*
	����:
		ϵͳ��ʼ��
	�������:
		@ii: һЩϵͳ��Ϣ
	�������:
		��
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:		
*/
extern rs_t iw_sys_init(iw_init_t *ii);

/*
	����:
		��ȡϵͳ��Ϣ
	�������:
		@info: ���ϵͳ��Ϣ���ڴ��
	�������:
		@info: ����ϵͳ��Ϣ
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
extern rs_t iw_sys_info_get(iw_sys_info_t *info);

/*
	����:
		����ϵͳ
	�������:
		��
	�������:
		��
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
		��������ϵͳ
*/
extern rs_t iw_sys_reboot();

/*
	����:
		�ָ��������ò�����ϵͳ
	�������:
		��
	�������:
		��
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
		������б�������ú���������ϵͳ
*/
extern rs_t iw_sys_reset();

/*
	�ͻ�����Ϣ��
*/
typedef struct {
	// �ÿͻ����ڱ��ص�Ψһ��ʶ
	u_int16_t client_id;
	// �ͻ��˵Ĺ���UDP�˿�
	u_int16_t port;
	// �ͻ��˵Ĺ���IP��ַ
	u_int32_t ip;
	// �������������ڹ��˶�����
	u_int32_t connect_time;
	// �ͻ������һ�λ�Ծʱ��
	u_int32_t last_active;
	// �Ƿ��Ǿ�����ֱ��
	bool is_direct;
	// �Ƿ��ǵ�¼�ɹ��˵�
	bool is_login;
	// ���
	char pad[2];
} iw_client_t;

/**************************************************
 	�¼�ע�ᡢ�ص�
 **************************************************/

/*
	�¼����Ͷ���
*/

/*
	һ�����óɹ�
*/
#define	IW_EV_SMART_CONFIG_OK	0x0101
// �������ӳɹ�
#define	IW_EV_LINK_UP	0x0102
// ����Ͽ�
#define	IW_EV_LINK_DOWN	0x0103
// ip��ַ�����롢���ء�DNS���б仯
#define	IW_EV_IP_MODIFY	0x0104
// ����������ӳɹ�
#define	IW_EV_SERVER_UP	0x0105
// �����������ʧ��
#define	IW_EV_SERVER_DOWN	0x0106
// ʱ��NTPͬ���ɹ�
#define	IW_EV_TIME_SYNC	0x0107
// �ͻ��˵�¼�ɹ�
#define	IW_EV_USER_LOGIN	0x0110
// �����ͻ��˵�GET����
#define	IW_EV_OBJECT_GET	0x0201
// �����ͻ��˵�SET����
#define	IW_EV_OBJECT_SET	0x0202
// �����ͻ��˵�delete����
#define	IW_EV_OBJECT_DELETE	0x0203


//�������
//��ȡ��һ���ַ���ͳһ�ص�����
#define	IW_EV_UARTR_2COMMON			0x0301 //ͳһ���ջص�
#define IW_EV_UARTR_2ACTIVE			0x0302 //�ص�������ģ��
#define IW_EV_UARTR_2PRETEST		0x0303 //�ص�������ģ��
#define IW_EV_UARTR_2IJPROTO		0x0304 //i+ ����Э��״̬������


// ����һ���ַ�
#define	IW_EV_UARTW_2COMMON			0x0331 //ͳһ���ͻص�

// GPIO�ж�
#define	IW_EV_GPIO_INTR	0x0401
// ��ʱ������
#define	IW_EV_TIMER	0x0501
 
 
/*
	iWuOS object
*/
typedef struct {
	u_int16_t objct;
	u_int16_t sub_objct;
	u_int16_t attr;
	u_int16_t param_len;
	// u_int8_t param[0];
} iw_obj_t;

#define	UCA_NONE		0
#define	UCA_REQUEST	1
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


typedef struct {
	u_int16_t event;
	rs_t err_no;
	/*
		��ʱ�������¼�: ��ʱ��ID
		�ͻ��˵�¼�ǳ��¼�: �ͻ���ID
		���ڶ�д�¼�: ����ID
		�����¼�: �ͻ���ID
	*/
	u_int16_t id;
	// �յ����籨���еĶ�����UCA_XXX
	u_int8_t action;
	u_int8_t pad;
	// GPIO_IN_COUNTģʽʱ��1���ж��ٸ���������
	u_int32_t gpio_count;
	void *s;
	// �ص�����
	union{
	// �����ص��������붨ʱ��
	void *cp;
	// �����յ����Ļص�
	iw_obj_t obj;
	}cu;
} iw_event_info_t;

typedef rs_t (* iw_event_callback_t)(iw_event_info_t *info);
typedef rs_t (* iw_timer_callback_t)(void *cp);

/*
	����:
		��ϵͳע���¼��ص�
	�������:
		@event: �¼���IW_EV_XXX
		@callback: �ص�����
	�������:
		��
	����ֵ:
		RS_OK: ���óɹ�
		����: ����
	˵��:
*/
extern rs_t iw_event_register(u_int16_t event, iw_event_callback_t callback);

/*
	����:
		��֪ϵͳ�Լ�֧����Щ�������
	�������:
		@obj: ֧�ֵĶ�������
		@num: �������
	�������:
		��
	����ֵ:
		RS_OK: ���óɹ�
		����: ����
	˵��:
		��֪ϵͳ�Լ�֧����Щ���󡣲�֧�ֵĶ���ϵͳ�յ�
		�ͻ�������ʱ��ϵͳ��Ӧ����󣬲��ύ���ϲ�ص�
		����
*/
extern rs_t iw_obj_register(iw_obj_t *obj, int num);

/*
�ڴ���������յ��ַ�����ڻص�����
*/
extern void  iw_event_foruart(uint8 IN_value);


/*
	����:
		ʱ������
	�������:
		@event: ע����¼���id
	�������:
		��
	����ֵ:
		��
	˵��:
*/
void  iw_request_event(unsigned char event);


/*
	����:
		��ӡ16��������
	�������:
		@msg: ���ݵ�˵��
		@ptr: ����
		@len: ���ݳ���
	�������:
		��
	����ֵ:
		��
	˵��:
*/
extern void mem_dump(char *msg, unsigned char *ptr, int len);
/*
	����:
		�ж��費��һ������
	�������:
	�������:
	����ֵ:
		true: ��
		flase: ����
	˵��:
*/
bool  needsmartconfig(void);
#endif