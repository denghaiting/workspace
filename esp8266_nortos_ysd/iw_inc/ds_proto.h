#ifndef __DS_PROTO_HEADER__
#define __DS_PROTO_HEADER__

/*role type of DS007 system*/
enum {
	TP_CENTER = 0,		/*��������*/
	TP_DISPATCHER = 1,	/*���������*/
	TP_DEV_SRV = 2,		/*�豸������*/
	TP_WEB_SRV = 3,		/*web������*/
	TP_USER = 4,		/*�ֻ����������û�*/
	TP_DS007 = 5,		/* DS007�豸*/
	TP_CHARGE_SRV = 6,		/* ��ֵ������ */
	TP_CHARGE_CLI_RO = 7,	/* ��ֵ�ͻ��ˣ�ֻ�� */
	TP_CHARGE_CLI_WR = 8,	/* ��ֵ�ͻ��ˣ���д  */
	TP_MAX
};

/*subtype of 007 device*/
#define IJ_TYPE_NUM 0x04

#define IJ_007 			0x0  	/*i+007*/
#define IJ_001 			0x01  	/*i+001 Ŀǰ��Ҫ��i+001E���ߺ���ת����*/
#define IJ_002 			0x02  	/*i+002 iTV-C*/
#define IJ_003 			0x03  	/*i+003 wireless camera*/
#define IJ_006 			0x6  	/*i+006*/
#define IJ_008 			0x08  	/* i+008 iTV-C */
#define IJ_807 			0x09  	/*i+807E*/
#define IJ_FANSBOX 		0x0A 	/* fansbox*/

#define IJ_808			0X10	/*i+808����*/
#define IJ_812 			0x11  /*udpů���*/


#define IJ_UDP_DEVICE_BASE IJ_808
#define IJ_UDP_DEVICE_END 0x1f


#define IJ_OPT 			0x20 	/*openwrtˢ���豸*/
#define IJ_ANDROID 		0x21  	/*Andriodˢ���豸*/
#define IJ_COMMUNITY 	0x22   /* С�������� */

#define IJ_AIRCONDITION 0x30	/* �ǻۼҵ絥Ʒ�յ� */
#define IJ_WATERHEATER 0X31		/* �ǻۼҵ絥Ʒ��ˮ�� */
#define IJ_AIRHEATER 0x32		/* �ǻۼҵ絥Ʒ����¯ */
#define IJ_AIRCLEANER 0x33		/* �ǻۼҵ絥Ʒ���������� */
#define IJ_ELECTRICFAN 0x34		/* �ǻۼҵ絥Ʒ����� */
#define IJ_TEST_PRODUCT 0x54    /*Ԥ���Լ����Խ׶β�Ʒ*/

#define	IJ_UNKNOW	0xFF /* δ֪������ */

/*extented type of IJ_008*/
#define EX_008_SL 1
#define EX_008_SH 2
#define EX_008_EL 3
#define EX_008_EH 4

/*extented type of IJ_003*/
/*���涨���mu�����п�������*/
//����Ӳ��ƽ̨�汾����
enum {
	/*  �汾1:
		i+003Cam-H
		i+003Cam-M
		�Լ�����ƽ̨Ŀǰֻ��һ��Ӳ���汾,����i+007E ...
	*/
	EX_003_CAMH = 1,
	
	/*  �汾2:
		i+003Cam-Y
	*/
	EX_003_CAMY = 2,
	
	/*  �汾3:
		 i+003Cam-G(toplink���ҵ�Ӳ��oem�汾) 
	*/
	EX_003_CAMG = 3, 
	
	/*  �汾4:
		i+003Cam-H(S)(��i+003Cam-HӲ���Ļ����ϣ��ü���usb�ӿں�tf���ӿ�)
	*/
	EX_003_CAMHS = 4,
	
	/*  �汾5:
		i+003Cam-OG(����ǹ�ͣ��˻����Ƽ����ҵ�Ӳ��oem�汾)
	*/
	EX_003_CAMOG = 5,
	
	/*  �汾6:
		i+003Cam-OB(����Բ���ͣ��˻����Ƽ����ҵ�Ӳ��oem�汾)
	*/
	EX_003_CAMOB = 6,

	/*  �汾7:
		i+003Cam-YH(Cam-YS����ǿ�汾�����Ӻ�����Ƶ�Ƭ��)
	*/
	EX_003_CAMYH = 7,
	
	/*  �汾8:
		i+003Cam-G galaxywind
	*/
	EX_003_CAMYG = 8,
	EX_003_MAX
};
#define STP_MAX 256 /*max sub type*/

enum{/*��¼����*/
	LT_NORMAL = 1, /*��ͨ��¼��δ���ð��ֻ�*/
	LT_BIND = 2, /*���ֻ���¼*/
	LT_UNBIND = 3 /*δ���ֻ���¼*/
};

enum{/*������������*/
	NT_SERVER = 1, /*���ӷ�����*/
	NT_DEVICE = 2, /*������ֱ���豸���豸���ӵ�������*/
	NT_DEVICE_OFFLINE = 3, /*������ֱ���豸���豸δ���ӵ�������*/
};

enum{
	DIT_TYPE,
	DIT_NAME,
	DIT_IPHONE_NEWEST_VERSION,
	DIT_ANDRIOD_NEWEST_VERSION,
	DIT_IPHONE_EN,     /*iphone english newest version description*/
	DIT_IPHONE_CH,     /*iphone chinese newest version description*/
	DIT_ANDRIOD_EN,   /*android english newest version description*/
	DIT_ANDRIOD_CH,   /*android chinese newest version description*/
	DIT_LOGINTYPE,      /*LT_NORMAL/LT_BIND/LT_UNBIND*/
	DIT_NETTYPE,          /*NT_SERVER/NT_DEVICE*/
	DIT_MAX
};

#ifndef CLIB_HDR
enum{
	CID_IPHONE = 0,
	CID_ANDROID = 1,
	CID_SYMBIAN = 2,
	CID_PC = 3,
	CID_MAX
};
#endif


#define DEF_WEB_DOMAIN "www.jiazhang007.com"

#ifdef CHINA
//���ڷ�����
#define DEF_DISPATCHER_DOMAIN "www.jiazhang008.com"	
#else
//���������
#define DEF_DISPATCHER_DOMAIN "www.iiwifi.com"
#endif

#define DEF_APPLE_DOMAIN "www.apple.com"

/*���������������̶˿�*/
#define DEF_DISPATCHER_PORT 1180
/*�豸������������̶˿�*/
#define DEF_DEVSERVER_PORT 1181
/*Web������������̶˿�*/
#define DEF_WEBSERVER_PORT 1182
/* ���������UDP�����˿�*/
#define DEF_AGENT_UDP_PORT	1183
/*��ֵ������ȱʡ�˿�*/
#define DEF_CHARGE_SRV_PORT     1184
/*�豸������http�˿�*/
#define DEF_DEV_HTTP_PORT 880
/*Web������http�˿�*/
#define DEF_WEB_HTTP_PORT 80
/*DISPATCHER������http�˿�*/
#define DEF_DISPATCHER_HTTP_PORT 88
/* ����udp��echo_q�Ķ˿� */
#define DEF_DISPATCHER_UDP_PORT	1190
/* �豸�˷���udp echo_q�Ķ˿� */
#define DEF_DEV_UDP_PORT	1191
/* ͨ��UDP�����豸�Ķ˿ںţ��ṩ���ͻ��˵� */
#define	DFL_UDP_CTRL_CLIENT_WAN_PORT	1192
/* ���������豸�ṩ�����UDP�˿� */
#define	DFL_UDP_CTRL_DEV_PORT	1193
/* ���������豸�ṩ�����UDP�˿� */
#define	DFL_UDP_CTRL_DEV_PORT1	42613
/* ���������豸�ṩ�����UDP�˿� */
#define	DFL_UDP_CTRL_DEV_PORT2	51193
/* �豸��������UDP���ƵĶ˿� */
#define	DFL_UDP_CTRL_CLIENT_LAN_PORT	1194
/* ����udp�Ķ˿�1 ̽�Ȿ��ICE */
#define DEV_REGSERVER_PORT1    42190
/* ����udp�Ķ˿�2 ̽�Ȿ��ICE */
#define DEV_REGSERVER_PORT2    52190



#ifndef CLIB_HDR
/*С��������udp�˿ڣ��豸ֱ��С��������*/
#define CMT_PORT 5362
#endif

#define DSFTP_STDIN "/dsserver/dsftp-in"
#define DSFTP_PORT 2345

#undef	BIT
#define	BIT(n)	(1 << (n))
#define DEV_LIST_TMP_FILE "/var/tmp-device-%u"

/*ͨ��Э������
��ֹ�����������м����������
ֻ��˳����ӵ�����*/
enum {
	CMD_OK = 0,
	CMD_FAIL = 1,
	CMD_ECHO_Q = 2,
	CMD_ECHO_A = 3,
	CMD_AUTH_Q = 4,
	CMD_AUTH_A = 5,
	CMD_AUTH_K = 6,
	CMD_EXCHANG_Q = 7,
	CMD_EXCHANG_A = 8,
	CMD_USER_BIND_Q = 9,
	CMD_USER_BIND_A = 10,
	CMD_USER_AUTH_Q = 11,
	CMD_USER_AUTH_A = 12,
	CMD_USER_AUTH_K = 13,
	CMD_SERVER_Q = 14,
	CMD_SERVER_A = 15,
	CMD_DEV_Q = 16,
	CMD_DEV_A = 17,
	CMD_URL_Q = 18,
	CMD_URL_A = 19,
	CMD_DEV_CONFIG_Q = 20,
	CMD_DEV_CONFIG_A = 21,
	CMD_DEV_STAT_Q = 22,
	CMD_DEV_STAT_A = 23,
	CMD_DEV_STAT_CYCLE = 24,
	CMD_ELECT_MASTER_Q = 25,
	CMD_ELECT_MASTER_A = 26,
	CMD_ELECT_MASTER_C = 27,
	CMD_NICKNAME = 28,
	CMD_PASSWD = 29,
	CMD_LINK_OPTION_Q = 30,
	CMD_LINK_OPTION_A = 31,
	CMD_WEB_AUTH_Q = 32,
	CMD_WEB_AUTH_A = 33,
	CMD_NICKNAME_Q = 34,
	CMD_NICKNAME_A = 35,
	CMD_FTP_Q = 36,
	CMD_FTP_A = 37,
	CMD_YW_SERVER_Q = 38, /*��ά��ѯ����������*/
	CMD_KEEP_DATA = 39,
	CMD_NICKNAME_CONFIRM = 40, 	/*�޸��ǳƳɹ�ȷ������������ڲ�ʹ��*/
	CMD_NICKNAME_FAIL = 41,   	/*�޸��ǳ�ʧ������������ڲ�ʹ��*/
	CMD_LOAD_USER_INFO = 42,  	/*�����û�������Ϣ*/
	CMD_UDP_KEEP_DATA = 43,
	CMD_UDP_DEV_STAT = 44,
	CMD_SERVICE_DATE_Q = 45,  	/*�����ײͲ�ѯ����*/
	CMD_SERVICE_DATE_A = 46,  	/*�����ײͲ�ѯ��Ӧ*/
#if 0	
	CMD_SERVICE_CHARGE = 47,  	/* �����ײ͸���*/
#endif	
	CMD_URL_HIT_Q = 48, 		/*��ѯURL ��������*/
	CMD_URL_HIT_A = 49, 		/*��ѯURL ������Ӧ*/
	CMD_IPLOCATION_Q	= 50,	/*��ѯ�ͻ���IP��ַ��Χ*/
	CMD_IPLOCATION_A = 51,	/*dispatcher ��ѯ�ͻ���IP��ַ��Χ*/
	CMD_SELLER_LOGIN_Q	= 52,	/*������Ա��¼��֤����*/
	CMD_SELLER_LOGIN_A	= 53,	/*������Ա��¼��֤Ӧ��*/
	CMD_SELLER_USER_Q	= 54,	/*�û��˺Ų�ѯ����*/
	CMD_SELLER_USER_A	= 55,	/*�û��˺Ų�ѯӦ��*/
	CMD_SELLER_USER_ADD	= 56,	/*����˺�*/
	CMD_SELLER_USER_MODIFY = 57,/*�޸��˺�*/
	CMD_SELLER_USER_DEL	= 58,	/*ɾ���˺�*/
	CMD_VEDIO_Q = 59, /* ����鿴����ͷ��Ƶ */
	CMD_VEDIO_A = 60, /* ����ͷ��Ƶ��ϢӦ�� */
	CMD_SELLER_PWD = 61,	/*�޸��˺�����*/
	CMD_VIDEO_SYN_Q = 62,
	CMD_VIDEO_SYN_A = 63,
	CMD_VIDEO_HELLO_REQ = 64,
	CMD_VIDEO_HELLO =65,
	CMD_VIDEO_JPG = 66,
	CMD_VIDEO_ACK = 67,
	CMD_VIDEO_QUALITY = 68,
	CMD_VIDEO_STOP = 69,
	CMD_VEDIO_AGENT_A = 70,
	CMD_VEDIO_AGENT_Q = 71,
	CMD_VEDIO_AGENT_SETUP = 72,
	CMD_MISC_Q = 73,
	CMD_MISC_A = 74,
	CMD_UDP_YW_DISPATCH_Q = 78,//��ά--�ڷ����������-���豸����������-ȡ���豸����������ϸ�豸�б�
	CMD_LOAD_PACKAGE_LOG_Q = 79,//��ѯ��ֵ��־
	CMD_LOAD_PACKAGE_LOG_A = 80,//��ѯ��ֵ��־
	CMD_UDP_YW_DISPATCH_A = 81,////��ά--�ڷ����������-���豸����������
	CMD_ADD_RC_CARD_Q = 83,  /*��ӳ�ֵ������*/
	CMD_ADD_RC_CARD_A = 84,  /*��ӳ�ֵ����Ӧ*/
	#if 0
	CMD_MOD_RC_CARD_Q = 85 , /*���³�ֵ��״̬����*/
	CMD_MOD_RC_CARD_A = 86,  /*���³�ֵ��״̬��Ӧ*/
	CMD_DEL_RC_CARD_Q = 87,  /*ɾ����ֵ������*/
	CMD_DEL_RC_CARD_A = 88, /*ɾ����ֵ����Ӧ*/
	#endif
	CMD_TRANS_RC_CARD_Q = 89, /*ת�Ƴ�ֵ������*/
	CMD_TRANS_RC_CARD_A = 90, /*ת�Ƴ�ֵ����Ӧ*/
	CMD_QUERY_RC_CARD_Q = 91, /*��ѯ��ֵ��״̬����*/
	CMD_QUERY_RC_CARD_A = 92, /*��ѯ��ֵ��״̬��Ӧ*/
	CMD_CARD_CHARGE_Q = 93, /*��ֵ����*/
	CMD_CARD_CHARGE_A = 94, /*��ֵ��Ӧ*/
	CMD_DEV_CHARGE = 95,  /*��ֵ�ɹ�ͬ��֪ͨ����*/
	CMD_YW_SERVER_A = 96, /*��ά��ѯ��������Ӧ*/
	CMD_VEDIO_AGENT_OK = 97,/*����ɹ�*/
	CMD_ADD_CARD_TYPE_Q = 98,/*��ӿ���������*/
	CMD_ADD_CARD_TYPE_A = 99,/*��ӿ�������Ӧ*/
	CMD_QUERY_CARD_TYPE_Q = 100,/*��ѯ����������*/
	CMD_QUERY_CARD_TYPE_A = 101,/*��ѯ��������Ӧ*/
	CMD_QUERY_USER_PWD_Q = 102,/*ȡ�õ�ǰ��¼DEV����*/
	#if 0
	CMD_CARD_CHARGE_SYN_Q = 103,/*��ֵ��Ϣͬ������*/
	CMD_CARD_CHARGE_SYN_A = 104,/*��ֵ��Ϣͬ��Ӧ��*/
	CMD_CARD_TYPE_SYN_Q = 105,/*��ֵ������ͬ������*/
	CMD_CARD_TYPE_SYN_A = 106,/*��ֵ������ͬ��Ӧ��*/
	CMD_CARD_STATUS_SYN_Q = 107,/*��ֵ��״̬ͬ������*/
	CMD_CARD_STATUS_SYN_A = 108,/*��ֵ��״̬ͬ��Ӧ��*/
	CMD_DEV_CHARGE_CONFIRM = 109,/*��ֵ�ɹ�ͬ��֪ͨȷ������*/
	#endif
	CMD_YW_DEVSERVER_Q = 110,/*��ά����--�豸��������ѯ*/
	CMD_YW_DEVSERVER_A = 111,/*��ά����--�豸��������ѯ*/
	CMD_BIND = 112, /* �����豸���� */
	CMD_SET_NAME = 113, /* �����豸���� */
	CMD_MS_CTRL = 114,  /* �����豸�������������Ҫ���� */
	CMD_VIDEO_ROLL = 115, /* ������̨ת�� */
	CMD_FM_CONFIG_Q = 116,  /*���ü�ͥ������Ա����*/
	CMD_FM_CONFIG_A = 117,  /*���ü�ͥ������Ա��Ӧ*/
	CMD_FM_Q = 118,  /*��ѯ��ͥ������Ա����*/
	CMD_FM_A = 119,  /*��ѯ��ͥ������Ա��Ӧ*/
	CMD_MESURE_TRANS = 120, /*����������*/
	CMD_MESURE_Q = 121,      /*��ѯ�����������*/
	CMD_MESURE_A = 122,      /*��ѯ���������Ӧ*/
	CMD_MESURE_DEL = 123,   /*ɾ���������*/
	CMD_MESURE_TRANS_CONFIRM = 124, /*�豸��������ͬ����������ȷ��*/
	CMD_PLUG_TIMER_Q = 125,	/* ��ʱ���ص����� */
	CMD_PLUG_TIMER_A = 126,	/* ��ʱ���صĻ�Ӧ */
	CMD_USER_DEBUG = 127,     /*�ֻ��ϴ�������Ϣ*/
	CMD_IR_LIST = 128,		/*�ֻ�����֧���б�*/
	CMD_IR_CONFIG = 129,	/*�ֻ����úͻ�ȡ�����üҵ�*/
	CMD_IR_DB = 130,		/*�豸�ӷ�������ȡ�����*/
	CMD_IR_URL = 131,		/*�豸��ȡ����ң���б��URL */
	CMD_IR_CTRL= 132,		/* �ֻ����Ժ�������������Ч */
	CMD_REMOTE_CONFIG = 133, /* CMD_REMOTE_CONFIG */
	CMD_REMOTE_KEY_CONFIG = 134, /* �ֻ�Ϊ��ͥ��������һ������ */
	CMD_REMOTE_CODE = 135, /* �ֻ�Ϊ��������һ������ */
	CMD_REMOTE_CTRL= 136, /* �ֻ����ͥ�������Ϳ������� */
	CMD_REMOTE_STATE = 137, /* �ֻ��Ե�������״̬�Ĳ��� */
	CMD_VIDEO_TEMP_QUALITY = 138, /* ������̨����ʱ����ʱ���Ի��ʣ����������� */
	CMD_VIDEO_RECOR_CTRL = 139,/*�豸�˱���¼������------�Ѿ�ȡ���˱���*/
	CMD_ALARM_BIND_PHONE = 140,	/*�������Ű��ֻ�*/
	CMD_ALARM_MSG = 141,	/*������Ϣ��������*/
	CMD_FAIL_EX = 142,		/* cmd_fail��չ���� */
	CMD_ALARM_CONFIG_PHONE = 143,
	CMD_SLAVE_HIS = 144, /*���豸ͨ��������Ϣ�����豸*/
	CMD_GET_CMDOK = 145,	/* �豸�����������ѯ�ֻ���֤�ɹ�����Ҫ�������Ϣ */
	CMD_ALARM_LOG = 146,/*������־��ز���*/
	CMD_ALARM_LOG_SYNC = 147,/*������־��ͬ��*/
	CMD_TIME_SYNC = 148,/*�豸���ֻ���ѯʱ��ͬ��*/
	CMD_VTAP_LIST_Q = 149,/*��ѯ�豸¼���б�*/
	CMD_VTAP_LIST_A = 150,/*��Ӧ�豸¼���б�Ĳ�ѯ*/
	CMD_VTAP_Q = 151,/*�����豸¼���ļ�*/
	CMD_VTAP_KEEP = 152,/*¼��ۿ�����*/
	CMD_VTAP_A = 153,/*¼��������*/
	CMD_VTAP_TIMESTAP = 154, /*���󲥷�ָ��ʱ��*/

	CMD_REMOTE_BROADCAST = 155, /* �㲥������������豸 */
	CMD_VIDEO_SONIX = 156, /*��sonix ����*/
	CMD_REC_TIMER_Q = 157,/* ��Ƶ¼�ƶ�ʱ���ص����� */
	CMD_REC_TIMER_A  = 158,/* ��Ƶ¼�ƶ�ʱ���صĻ�Ӧ */
	CMD_VIDEO_CONTROL_ALARM_CFG = 159, /*��ⱨ��������Ϣ*/
	CMD_MS_DEV_INFO = 160, /*�����豸ͨ�Ÿ�֪�໥�İ汾��ʱ����֧�ֵĹ��� */

	CMD_DEV_SAHRD_INFO = 161, /*�豸���������������Ӧ��ͬ*/
	CMD_DEV_SAHRD_COUNT = 162, /*�豸���������������*/
	CMD_OPEN_TELNET = 163, /*�������豸telnet����*/
	CMD_ALARM_SWITCH_CTL = 164, /*���������ܿ��ؿ���*/
	CMD_VTAP_END = 165,/*�豸֪ͨ�ͻ��ˣ�¼�񲥷����*/
	CMD_VIDEO_QUALITY_V2 = 166,/* �Զ���ֱ������� */
	CMD_PLUG_ELECTRIC_STAT = 167,/*����ͳ�ƹ���֧�ֲ�ѯ����*/
	CMD_SCENE_CONFIG = 168,/*�龰ģʽ��������*/
	CMD_OPT_SCAN_Q = 169, /*ɨ��ˢ���豸����*/
	CMD_OPT_SCAN_A = 170, /*ɨ��ˢ���豸��Ӧ*/
	CMD_DEV_REG_Q = 171,  /*ˢ���豸ע������*/
	CMD_DEV_REG_A = 172, /*ˢ���豸ע����Ӧ*/
	CMD_ARIA2C_Q = 173, /*aria2c��������*/
	CMD_ARIA2C_A = 174, /*aria2c������Ӧ*/
	CMD_NETWORK_DETECT_Q = 175, /* ��Ƶ����̽������� */
	CMD_NETWORK_DETECT_A = 176, /* ��Ƶ����̽����Ӧ�� */
	CMD_SSIDPW = 177, /*�ֻ��޸��豸wifi SSID������*/
	CMD_DEVSERVER_LIST_Q = 178, /*��ȡ�豸�������б�����*/
	CMD_DEVSERVER_LIST_A = 179, /*��ȡ�豸�������б���Ӧ*/
	CMD_NET_PROBE_Q = 180,  /*�豸������������̽������*/
	CMD_NET_PROBE_A = 181,  /*�豸������������̽����Ӧ*/
	CMD_LOCATION_Q = 182,  /*�豸λ������*/
	CMD_LOCATION_A = 183,  /*�豸λ����Ӧ*/
	CMD_MACDENY = 184, /*���wifi mac��ֹ�б�*/
	CMD_MACALLOW = 185, /*ɾ��wifi mac��ֹ�б�*/
	CMD_BIND_SLAVE_INFO = 186,/*��ȡ���豸����Ϣ*/
	CMD_MASTER_SLAVE_CTRL = 187,/*����֮��Ŀ��������ͨ��������*/
	CMD_REBOOT = 188,/*Զ����������*/
	CMD_AREA_CONFIG = 189,/*�����������*/	
	CMD_STATIC_PIC_Q = 190,/*�����̬ͼƬ����*/
	CMD_STATIC_PIC_A = 191,/*�����̬ͼƬ��Ӧ*/
	CMD_POSITION_Q = 192, /*��λ��Ϣ����*/
	CMD_POSITION_A = 193, /*��λ��Ϣ��Ӧ*/
	CMD_SPEED_MAX_Q = 194,/*���ٷ�ֵ����*/
	CMD_SPEED_MAX_A = 195,/*���ٷ�ֵ��Ӧ*/
	CMD_SCHLBUS_BIND = 196,/*У����*/
	CMD_VOICE = 197,
	CMD_VOICE_ACK = 198,
	CMD_SPEEK_Q = 199,
	CMD_SPEEK_A = 200,
	CMD_VOICE_REG = 201,
	CMD_REMOTE_CODE_UPLOAD = 202,/*�ϴ������豸ѧϰ���Ŀ��Ʊ��뵽������*/
	CMD_RECORD_QUALITY_V2 = 203,/*¼����������*/
	CMD_NOTIFY_HELLO = 204, /* �豸��С���������������� */
	CMD_NOTIFY_HELLO_ACK = 205, /* �豸��С��������������Ӧ */
	CMD_NOTIFY = 206, /* ��Ϣ��������籨�������������� */
	CMD_NOTIFY_RESULT = 207, /* ��Ϣ����Ӧ�� */
	CMD_NOTIFY_EXPECT = 208, /* ��ϢID ͬ������*/
	CMD_NOTIFY_CENTER_LIST = 209, /* С�����������Լ�ip/port�ϱ��������������������͸��豸*/
	CMD_CMT_OP_DEVICE = 210, /* ��С����������豸���в���������ӡ�����net_cmt_op_device_t*/
	CMD_VOICE_PROMPT  = 211, /* ���ű��������ļ� */
	CMD_REMOTE_BD_BIND = 212,	/* ˫��RFЭ��󶨽�� */
	CMD_REMOTE_CONFIG_SOUNDLIGHT = 213,	/* ���ð������������������ⱨ���� */
	CMD_REMOTE_TD_CODE = 214, /* ������ά����� */
	CMD_PHONE_BIND_Q = 215, /*���ֻ������ύ������net_phone_bind_t*/
	CMD_PHONE_REQUESTLIST_Q =216, /*���ֻ������б��ѯ*/
	CMD_PHONE_REQUESTLIST_A =217, /*���ֻ������б���Ӧ������net_phone_bind_list_t*/
	CMD_PHONE_BIND_OPERATION = 218, /*�Ѱ��ֻ��԰��������������net_phone_bind_operation_t*/
	CMD_PHONE_BIND_RESULT = 219, /*�Ѱ��ֻ��԰�����������������net_phone_bind_result_t*/
	CMD_PHONE_BIND_DEL = 220, /*ɾ�����ֻ���net_phone_bind_uuid_t*/
	CMD_PHONE_UNBINDLOGIN_ALLOW = 221, /*����δ���ֻ���¼*/
	CMD_PHONE_UNBINDLOGIN_DENY = 222, /*��ֹδ���ֻ���¼*/
	CMD_PHONE_BINDLIST_Q = 223, /*���ֻ��б��ѯ����*/
	CMD_PHONE_BINDLIST_A = 224, /*���ֻ��б��ѯ��Ӧ������net_phone_bind_list_t*/
	CMD_SCENE_TIMER_Q  =  225,	/*�����龰ģʽ�Ķ�ʱ����������*/
	CMD_SCENE_TIMER_A  =  226,	/*�����龰ģʽ�Ķ�ʱ����Ӧ������*/
	CMD_SCENE_LINKAGE = 227,	/* �����龰�������� */
	CMD_PHONE_APN_OPERATION = 228, /*�����ֻ������������net_phone_push_t*/
	CMD_STATIC_PIC_Q_V2 = 229,/*�ڶ��汾�����̬ͼƬ����*/
	CMD_STATIC_PIC_A_V2 = 230,/*�ڶ��汾�����̬ͼƬ��Ӧ*/
	CMD_REMOTE_VTY = 231,/* ����Զ��telnetd���� */
	CMD_IA = 232,		/* ���ܼҾӵ�Ʒ���ƺͲ�ѯ */
	CMD_NEWUPVER_Q = 233,	/*�ֻ���ȡ�豸���������汾����*/
	CMD_NEWUPVER_A = 234,	/*�ֻ���ȡ�豸���������汾Ӧ��*/
	CMD_SET_NEWUPVER = 235,	/*�ֻ������豸�����汾*/
	CMD_NOTICE_DEVUP = 236,	/*�ֻ�֪ͨ�豸��������*/
	CMD_V4L2_COLOR = 237, /* v4l2 color������ѯ�Ϳ��� */
	CMD_NOTIFY_QUERY = 238, /* ��ѯ������Ϣ���󣬲���net_notify_query_t����ӦCMD_NOTIFY */
	CMD_MOTO_ATTRIBUTE = 239, /* IPC��̨����������� */
	CMD_MOTO_PRE_POSITION = 240, /* IPC��̨Ԥ��λ���� */
	CMD_MOTO_CRUISE = 241, /* IPC��̨Ѳ������ */
	CMD_UDP_AUTH = 242,
	CMD_UDP_KEEPLIVE = 243,
	CMD_UDP_CTRL = 244,
	CMD_UDP_NOTIFY = 245,
	CMD_UDP_BIND_PHONE = 246,

	CMD_UDP_DNS_PROB = 301, /* ̽�Ȿ������ */
	CMD_MAX
};
/*ͨ��Э������
��ֹ�����������м����������
ֻ��˳����ӵ�����*/


enum{
	ERR_NONE = 0,
	ERR_SN_INVALID = 1,    		/*DS007���к���Ч*/
	ERR_NICKNAME_INVALID = 2,	/*�û��ǳ���Ч*/
	ERR_PASSWD_INVALID = 3,   	/*�û��������*/
	ERR_CMD_INVALID = 4,     	/* ��Ч����*/
	ERR_PARAM_INVALID = 5, 		/*��Ч����*/
	ERR_MEMORY = 6,       		/*�������ڲ������ڴ�ʧ��*/
	ERR_SYSTEM = 7,       		/*�������ڲ�ϵͳ���ǵ���ʧ��*/
	ERR_NICKNAME_CONFLICT = 8,  /*�ǳƳ�ͻ*/
	ERR_NICKNAME_TOO_LONG =9, 	/*�ǳƹ���*/
	ERR_EMAIL_TOO_LONG = 10,    /*email��ַ����*/
	ERR_DATABASE = 11,			/*���ݿ����ʧ��*/
	ERR_CLIENT_VER = 12, 		/*�ֻ��ͻ��˰汾����*/
	ERR_DEV_OFFLINE = 13,		/* �豸���� */
	ERR_VEDIO_OFF = 14,		/* δ��������ͷ */
	ERR_DEV_SYS_ERR = 15,		/* �豸ϵͳ���� */
	ERR_SELLER_NAME_CONFLICT = 16,/*������Ա�û�����ͻ*/
	ERR_TOO_MANY = 17, /* ̫�����ڹۿ���Ƶ�� */
	ERR_PACKAGE_NAME_CONFLICT = 18, /* ̫�����ڹۿ���Ƶ�� */
	ERR_OUT_SERVICE = 19, /* ������ */
	ERR_CARD_SN_INVALID = 20, /*��ֵ�����к���Ч*/
	ERR_CARD_PWD_INVALID = 21, /*��ֵ��������Ч*/
	ERR_CARD_STATE_INVALID = 22, /*��ֵ��״̬��Ч*/
	ERR_CARD_NOTIME_TRANS = 23, /*�豸�޷������޿�ת��*/
	ERR_TIMEOUT1 = 24, /*��ʱʧ��*/
	ERR_AGENT = 25,		/* ����ʧ��*/
	ERR_EMAIL_INVALID =26, /*email��ַ��Ч*/
	ERR_FM_ID = 27,/* ��ͥ��ԱID��Ч */
	ERR_FM_LIMIT = 28, /* ��ͥ��Ա���ù��� */
	ERR_DEV_SYS_BUSY = 29, /* ϵͳæ��������������ϵͳ */
	ERR_PLUG_TIMER_LIMIT = 30, /* ��ʱ���ز������ø����Ѵﵽ��� */
	ERR_PLUG_TIMER_ID = 31, /* ��ʱ���ز���ID��Ч */
	ERR_REMOTE_LIMIT = 32, /* �ɿ��Ƶ��������Ѵ�������� */
	ERR_IR_DB_INVALID = 33, /* ����������� */
	ERR_REMOTE_BUTTON_LIMIT = 34, /* �ɿص��������ﵽ���� */
	ERR_REMOTE_ID_INVALID = 35, /* �ɿص���ID��Ч */
	ERR_REMOTE_KEY_ID_INVALID = 36, /* �ɿص���KEY ID��Ч */
	ERR_REMOTE_BUSY = 37, /* ������æ�����紦��ѧϰ״̬ */
	ERR_REMOTE_KEY_VALID = 38, /* ������ť��Ч */
	ERR_REMOTE_CODE_LEARN_FAILED = 39, /*ѧϰʧ��*/
	ERR_PHONE_NUM_EXCESS = 40,/*����֧�ֵ����绰����*/
	ERR_NO_BIND_PHONE = 41,/*������������δ���ֻ�*/
	ERR_DEV_UNLINK = 42,/*�豸δ�����豸������*/
	ERR_ALARM_PHONE_NOT_FOUNT = 43, /*�󶨵ı����ֻ��Ų�����*/
	ERR_ALARM_VIDEO_NOT_FOUNT = 44,/*û��ָ���ı���¼��*/
	ERR_ALARM_LOG = 45,/*������־��������*/
	ERR_ALARM_LOG_SYNC = 46,/*������־ͬ������*/
	ERR_REC_TIMER_LIMIT = 47,/*��Ƶ¼�ƶ�ʱ��:�Ѿ��ﵽ��������������*/
	ERR_REC_TIMER_OPT = 48,/*��Ƶ¼�ƶ�ʱ��:����ʧ��*/
	ERR_REC_TIMER_ID   = 49,/*��Ƶ¼�ƶ�ʱ��:��ʱ��id��Ч*/
	ERR_REC_TIMER_NTP = 50,/*ntp δͬ�� ���ʧ��*/
	ERR_REC_TIMER_DURATION = 51,/*ʱ��̫��*/
	ERR_NO_VTAP = 52,/*û����Ƶ¼���ļ�*/
	ERR_SLAVE_OFFLINE = 53, /* ���豸���� */
	ERR_DPI_FOR_PHONE = 54, /* �ֻ����߲�֧�ִ�ֱ��ʡ�֡�ʵ����� */
	ERR_CODE_ADJUST = 55, /* ��Ӧ���벻֧��΢�� */
	ERR_VTAP_CLIENT_EXCEED = 56, /*�ۿ�¼�����̫���ˣ�ͬʱֻ����1���˿�*/
	ERR_VTAP_DAMAGE = 57,/*¼���ļ���*/
	ERR_SCENE_VERSION = 58,/*�汾����ƥ��*/
	ERR_SCENE_ID	=59,/*�Ƿ�����id*/
	ERR_SCENE_FAIL	=60,/*ִ��ʧ��*/
	ERR_SCENE_ACTION	= 61,/*�����Ƿ�*/
	ERR_SCENE_ID_MAX	=62,/*����id�Ѵ�����*/
	ERR_SCENE_BUSY		=63,/*ִ��æ*/
	ERR_AREA_VERSION = 64,/*����汾����ƥ��*/
	ERR_AREA_ID	=65,/*�Ƿ�����id*/
	ERR_AREA_FAIL	=66,/*����ʧ��*/
	ERR_AREA_ACTION	= 67,/*�����Ƿ�*/
	ERR_AREA_ID_MAX	=68,/*����id�Ѵ�����*/
	ERR_AREA_ERR_OBJTYPE =69,/*����Ķ�������*/	
	ERR_NO_SD_DETECTED = 70, /* û�д洢�豸���� */
	ERR_NOT_SUPPORT = 71,/*�豸��֧��*/
	ERR_BUSY = 72,/*����������ͨ��*/
	ERR_REMOTE_NOT_SUPPORT = 73,/*���豸��֧�ֵ�������,Ҳ���Ǵ��豸֧��*/
	ERR_TF_NOT_INSERT = 74,/*TF��δ����*/
	ERR_REMOTE_INVALID_TD = 75,/* ��ӵ���δ֪�Ķ�ά����Ϣ */
	ERR_UNBIND =  76, /*�ǰ��û���ֹ��¼*/
	ERR_BIND_FULL =77, /*�ﵽ���������ƣ������������*/
	ERR_BINDLATE = 78, /*�Ѿ����˶԰�������д�����*/
	ERR_SCENE_TIMER_LIMIT = 79, /*�龰ģʽ��ʱ�����������*/
	ERR_SCENE_TIMER_ID = 80,	/*�龰ģʽ��ʱ����ID�Ų���*/
	ERR_SCENE_INVALID_ID = 81, /* ��������ʱ��Ч�ĳ���ID */
	ERR_SCENE_INVALID_REMOTE_ID = 82, /* ��������ʱ��Ч�ĵ���ID */
	ERR_UNBIND_WITH_DEV_OFFLINE = 83, /*δ�󶨵�¼ʧ�ܣ����豸���ߣ���Ҫ��ʾ�û������*/
	ERR_DUPLICATE_REMOTE_CODE = 84,	/* �ظ���Ӱ������� */
	ERR_IA_NOT_READY = 85, /*���ܵ�Ʒû��ʼ��*/
	ERR_IA_OPERATE_INVALID = 86, /*���ܵ�Ʒ������Ч*/
	ERR_UPGRADE_VER_EMPTY = 87, /*�ֻ���ѯ�豸����������ϢΪ��*/

	ERR_NEED_ENCRPYT = 88, /* ��Ҫ���ܣ�ȴ�޷�Э�̳�һ���ļ����㷨 */
	ERR_CLONE = 89,  /* �豸�ǿ�¡�� */
	ERR_NOT_LEARN = 90,	/* �豸��û��ѧϰ���� */
	ERR_MAX /* ���ڴ�֮ǰ��Ӵ����� */
};

/* proto version control */
enum {
	PROTO_VER1 = 1,
	PROTO_VER2 = 2,
	/* further version add here */
};

#define PROTO_SUPPORT PROTO_VER2
#define PROTO_VERSION_INDEX(v)  ((v)-1)
#define PROTO_VERSION_VALID(v)	(((v) >= PROTO_VER1 && \
								(v) <= PROTO_SUPPORT) ? 1 : 0)

#define PROTO_MAX PROTO_VER2
#define MAX_PARAM_LEN (1024*1024*64)
#define SN_LEN 12	/*DS007�豸���к��ַ�������*/
#define MAX_NICKNAME 16  /*�û��ǳ���󳤶�*/
#define MAX_PLUG_TIMER_NAME	64	/* ��ʱ����������󳤶� */
#define MAX_EMAIL 32
#define MAX_PHONE_SINGLE 16
#define MAX_PHONE_NUM 10
#define MAX_SL_NUM (MAX_REMOTE_NUM - 1)
#define MAX_PHONE (MAX_PHONE_SINGLE * MAX_PHONE_NUM)
#define MAX_WEB_ROOT 64
#define MAX_HANDLE 0xFFFFFFFF
#define MAX_SERVER_HANDLE 0xEE6B2800 /*��֤�豸���������������handle�����ظ�*/
#define MAX_SSIDPW_SSID_LEN 33
#define MAX_SSIDPW_PW_LEN 64

#pragma pack(push,1)

typedef struct{
#ifndef LITTLE_ENDIAN
	u_int8_t 	encrypt:1,
		compress:1,
		hlen:3,
		ver:3;
#else
	u_int8_t ver:3,
		hlen:3,
		compress:1,
		encrypt:1;
#endif
	u_int8_t ds_type;
	u_int16_t command;
	u_int32_t param_len;
	u_int32_t handle;
}net_header_t;

//efine  NHF_TRANSPARENT	0x01
//efine  NHF_WAIT_REPLY	0x02

typedef struct {
	// hdr.ver = 2, hdr.hlen = sizeof(net_header_v2_t)/4
	net_header_t hdr;
	// NHF_XXX
	u_int8_t flags;
	// must be 0
	u_int8_t resv[3];
	u_int64_t sn;
} net_header_v2_t;


#define	net_hdr_len(net_hdr)	(((net_header_t *)net_hdr)->hlen<<2)
#define net_header_v2_size (sizeof(net_header_v2_t))

#define net_header_size (sizeof(net_header_t))
#ifdef CLIB_HDR
#define net_header_real_size(hdr) (((net_header_t *)hdr)->ver == PROTO_VER1 ? net_header_size : net_header_v2_size)
#else
#define net_header_real_size(v) ((v) == PROTO_VER1 ? net_header_size : net_header_v2_size)
#endif
#define	net_param_len(hdr) (((net_header_t *)hdr)->param_len)

/* ��ԭʼ���յ���Э��ͷ������¹���Э��ͷ��V2����(ע��SRC��V2��SN�ڽ���ʱ��ת�����ֽ���),�������v1��handle */
#define net_header_v2_wraper(dst, src) do {\
	memcpy((char*)(dst)+net_header_size, (char*)(src)+net_header_size, net_header_v2_size - net_header_size);\
	((net_header_v2_t*)(dst))->sn = ntoh_ll(((net_header_v2_t*)(src))->sn);\
	((net_header_t*)(dst))->handle = ((net_header_t*)(src))->handle;\
} while (0);

#pragma pack(pop)
#endif

