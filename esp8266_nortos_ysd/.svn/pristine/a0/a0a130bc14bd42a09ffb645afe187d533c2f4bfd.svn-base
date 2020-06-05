#ifndef __DS_PROTO_HEADER__
#define __DS_PROTO_HEADER__

/*role type of DS007 system*/
enum {
	TP_CENTER = 0,		/*生产中心*/
	TP_DISPATCHER = 1,	/*分配服务器*/
	TP_DEV_SRV = 2,		/*设备服务器*/
	TP_WEB_SRV = 3,		/*web服务器*/
	TP_USER = 4,		/*手机或流量器用户*/
	TP_DS007 = 5,		/* DS007设备*/
	TP_CHARGE_SRV = 6,		/* 充值服务器 */
	TP_CHARGE_CLI_RO = 7,	/* 充值客户端，只读 */
	TP_CHARGE_CLI_WR = 8,	/* 充值客户端，读写  */
	TP_MAX
};

/*subtype of 007 device*/
#define IJ_TYPE_NUM 0x04

#define IJ_007 			0x0  	/*i+007*/
#define IJ_001 			0x01  	/*i+001 目前主要是i+001E无线红外转发器*/
#define IJ_002 			0x02  	/*i+002 iTV-C*/
#define IJ_003 			0x03  	/*i+003 wireless camera*/
#define IJ_006 			0x6  	/*i+006*/
#define IJ_008 			0x08  	/* i+008 iTV-C */
#define IJ_807 			0x09  	/*i+807E*/
#define IJ_FANSBOX 		0x0A 	/* fansbox*/

#define IJ_808			0X10	/*i+808插座*/
#define IJ_812 			0x11  /*udp暖风机*/


#define IJ_UDP_DEVICE_BASE IJ_808
#define IJ_UDP_DEVICE_END 0x1f


#define IJ_OPT 			0x20 	/*openwrt刷机设备*/
#define IJ_ANDROID 		0x21  	/*Andriod刷机设备*/
#define IJ_COMMUNITY 	0x22   /* 小区服务器 */

#define IJ_AIRCONDITION 0x30	/* 智慧家电单品空调 */
#define IJ_WATERHEATER 0X31		/* 智慧家电单品热水器 */
#define IJ_AIRHEATER 0x32		/* 智慧家电单品快热炉 */
#define IJ_AIRCLEANER 0x33		/* 智慧家电单品空气净化器 */
#define IJ_ELECTRICFAN 0x34		/* 智慧家电单品电风扇 */
#define IJ_TEST_PRODUCT 0x54    /*预研以及测试阶段产品*/

#define	IJ_UNKNOW	0xFF /* 未知的类型 */

/*extented type of IJ_008*/
#define EX_008_SL 1
#define EX_008_SH 2
#define EX_008_EL 3
#define EX_008_EH 4

/*extented type of IJ_003*/
/*下面定义从mu定义中拷贝过来*/
//增加硬件平台版本参数
enum {
	/*  版本1:
		i+003Cam-H
		i+003Cam-M
		以及其他平台目前只有一个硬件版本,例如i+007E ...
	*/
	EX_003_CAMH = 1,
	
	/*  版本2:
		i+003Cam-Y
	*/
	EX_003_CAMY = 2,
	
	/*  版本3:
		 i+003Cam-G(toplink厂家的硬件oem版本) 
	*/
	EX_003_CAMG = 3, 
	
	/*  版本4:
		i+003Cam-H(S)(在i+003Cam-H硬件的基础上，裁剪了usb接口和tf卡接口)
	*/
	EX_003_CAMHS = 4,
	
	/*  版本5:
		i+003Cam-OG(室外枪型，兴华安科技厂家的硬件oem版本)
	*/
	EX_003_CAMOG = 5,
	
	/*  版本6:
		i+003Cam-OB(室外圆球型，兴华安科技厂家的硬件oem版本)
	*/
	EX_003_CAMOB = 6,

	/*  版本7:
		i+003Cam-YH(Cam-YS的增强版本，增加红外控制单片机)
	*/
	EX_003_CAMYH = 7,
	
	/*  版本8:
		i+003Cam-G galaxywind
	*/
	EX_003_CAMYG = 8,
	EX_003_MAX
};
#define STP_MAX 256 /*max sub type*/

enum{/*登录类型*/
	LT_NORMAL = 1, /*普通登录，未启用绑定手机*/
	LT_BIND = 2, /*绑定手机登录*/
	LT_UNBIND = 3 /*未绑定手机登录*/
};

enum{/*网络连接类型*/
	NT_SERVER = 1, /*连接服务器*/
	NT_DEVICE = 2, /*局域网直连设备，设备连接到服务器*/
	NT_DEVICE_OFFLINE = 3, /*局域网直连设备，设备未连接到服务器*/
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
//国内服务器
#define DEF_DISPATCHER_DOMAIN "www.jiazhang008.com"	
#else
//国外服务器
#define DEF_DISPATCHER_DOMAIN "www.iiwifi.com"
#endif

#define DEF_APPLE_DOMAIN "www.apple.com"

/*分配服务器服务进程端口*/
#define DEF_DISPATCHER_PORT 1180
/*设备服务器服务进程端口*/
#define DEF_DEVSERVER_PORT 1181
/*Web服务器服务进程端口*/
#define DEF_WEBSERVER_PORT 1182
/* 代理服务器UDP监听端口*/
#define DEF_AGENT_UDP_PORT	1183
/*充值服务器缺省端口*/
#define DEF_CHARGE_SRV_PORT     1184
/*设备服务器http端口*/
#define DEF_DEV_HTTP_PORT 880
/*Web服务器http端口*/
#define DEF_WEB_HTTP_PORT 80
/*DISPATCHER服务器http端口*/
#define DEF_DISPATCHER_HTTP_PORT 88
/* 接收udp的echo_q的端口 */
#define DEF_DISPATCHER_UDP_PORT	1190
/* 设备端发送udp echo_q的端口 */
#define DEF_DEV_UDP_PORT	1191
/* 通过UDP控制设备的端口号，提供给客户端的 */
#define	DFL_UDP_CTRL_CLIENT_WAN_PORT	1192
/* 服务器给设备提供服务的UDP端口 */
#define	DFL_UDP_CTRL_DEV_PORT	1193
/* 服务器给设备提供服务的UDP端口 */
#define	DFL_UDP_CTRL_DEV_PORT1	42613
/* 服务器给设备提供服务的UDP端口 */
#define	DFL_UDP_CTRL_DEV_PORT2	51193
/* 设备局域网内UDP控制的端口 */
#define	DFL_UDP_CTRL_CLIENT_LAN_PORT	1194
/* 接收udp的端口1 探测本国ICE */
#define DEV_REGSERVER_PORT1    42190
/* 接收udp的端口2 探测本国ICE */
#define DEV_REGSERVER_PORT2    52190



#ifndef CLIB_HDR
/*小区服务器udp端口，设备直连小区服务器*/
#define CMT_PORT 5362
#endif

#define DSFTP_STDIN "/dsserver/dsftp-in"
#define DSFTP_PORT 2345

#undef	BIT
#define	BIT(n)	(1 << (n))
#define DEV_LIST_TMP_FILE "/var/tmp-device-%u"

/*通信协议命令
禁止在已有命令中间插入新命令
只能顺序添加到后面*/
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
	CMD_YW_SERVER_Q = 38, /*运维查询服务器请求*/
	CMD_KEEP_DATA = 39,
	CMD_NICKNAME_CONFIRM = 40, 	/*修改昵称成功确认命令，服务器内部使用*/
	CMD_NICKNAME_FAIL = 41,   	/*修改昵称失败命令，服务器内部使用*/
	CMD_LOAD_USER_INFO = 42,  	/*加载用户基本信息*/
	CMD_UDP_KEEP_DATA = 43,
	CMD_UDP_DEV_STAT = 44,
	CMD_SERVICE_DATE_Q = 45,  	/*服务套餐查询请求*/
	CMD_SERVICE_DATE_A = 46,  	/*服务套餐查询响应*/
#if 0	
	CMD_SERVICE_CHARGE = 47,  	/* 服务套餐更新*/
#endif	
	CMD_URL_HIT_Q = 48, 		/*查询URL 命中请求*/
	CMD_URL_HIT_A = 49, 		/*查询URL 命中响应*/
	CMD_IPLOCATION_Q	= 50,	/*查询客户端IP地址范围*/
	CMD_IPLOCATION_A = 51,	/*dispatcher 查询客户端IP地址范围*/
	CMD_SELLER_LOGIN_Q	= 52,	/*销售人员登录认证请求*/
	CMD_SELLER_LOGIN_A	= 53,	/*销售人员登录认证应答*/
	CMD_SELLER_USER_Q	= 54,	/*用户账号查询请求*/
	CMD_SELLER_USER_A	= 55,	/*用户账号查询应答*/
	CMD_SELLER_USER_ADD	= 56,	/*添加账号*/
	CMD_SELLER_USER_MODIFY = 57,/*修改账号*/
	CMD_SELLER_USER_DEL	= 58,	/*删除账号*/
	CMD_VEDIO_Q = 59, /* 请求查看摄像头视频 */
	CMD_VEDIO_A = 60, /* 摄像头视频信息应答 */
	CMD_SELLER_PWD = 61,	/*修改账号密码*/
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
	CMD_UDP_YW_DISPATCH_Q = 78,//运维--在分配服务器下-向设备服务器发送-取得设备服务器下详细设备列表
	CMD_LOAD_PACKAGE_LOG_Q = 79,//查询充值日志
	CMD_LOAD_PACKAGE_LOG_A = 80,//查询充值日志
	CMD_UDP_YW_DISPATCH_A = 81,////运维--在分配服务器下-向设备服务器发送
	CMD_ADD_RC_CARD_Q = 83,  /*添加充值卡请求*/
	CMD_ADD_RC_CARD_A = 84,  /*添加充值卡响应*/
	#if 0
	CMD_MOD_RC_CARD_Q = 85 , /*更新充值卡状态请求*/
	CMD_MOD_RC_CARD_A = 86,  /*更新充值卡状态响应*/
	CMD_DEL_RC_CARD_Q = 87,  /*删除充值卡请求*/
	CMD_DEL_RC_CARD_A = 88, /*删除充值卡响应*/
	#endif
	CMD_TRANS_RC_CARD_Q = 89, /*转移充值卡请求*/
	CMD_TRANS_RC_CARD_A = 90, /*转移充值卡响应*/
	CMD_QUERY_RC_CARD_Q = 91, /*查询充值卡状态请求*/
	CMD_QUERY_RC_CARD_A = 92, /*查询充值卡状态响应*/
	CMD_CARD_CHARGE_Q = 93, /*充值请求*/
	CMD_CARD_CHARGE_A = 94, /*充值响应*/
	CMD_DEV_CHARGE = 95,  /*充值成功同步通知命令*/
	CMD_YW_SERVER_A = 96, /*运维查询服务器响应*/
	CMD_VEDIO_AGENT_OK = 97,/*代理成功*/
	CMD_ADD_CARD_TYPE_Q = 98,/*添加卡类型请求*/
	CMD_ADD_CARD_TYPE_A = 99,/*添加卡类型响应*/
	CMD_QUERY_CARD_TYPE_Q = 100,/*查询卡类型请求*/
	CMD_QUERY_CARD_TYPE_A = 101,/*查询卡类型响应*/
	CMD_QUERY_USER_PWD_Q = 102,/*取得当前登录DEV密码*/
	#if 0
	CMD_CARD_CHARGE_SYN_Q = 103,/*充值信息同步请求*/
	CMD_CARD_CHARGE_SYN_A = 104,/*充值信息同步应答*/
	CMD_CARD_TYPE_SYN_Q = 105,/*充值卡类型同步请求*/
	CMD_CARD_TYPE_SYN_A = 106,/*充值卡类型同步应答*/
	CMD_CARD_STATUS_SYN_Q = 107,/*充值卡状态同步请求*/
	CMD_CARD_STATUS_SYN_A = 108,/*充值卡状态同步应答*/
	CMD_DEV_CHARGE_CONFIRM = 109,/*充值成功同步通知确认命令*/
	#endif
	CMD_YW_DEVSERVER_Q = 110,/*运维管理--设备服务器查询*/
	CMD_YW_DEVSERVER_A = 111,/*运维管理--设备服务器查询*/
	CMD_BIND = 112, /* 主从设备命令 */
	CMD_SET_NAME = 113, /* 主从设备命令 */
	CMD_MS_CTRL = 114,  /* 主从设备命令，服务器不需要处理 */
	CMD_VIDEO_ROLL = 115, /* 控制云台转动 */
	CMD_FM_CONFIG_Q = 116,  /*配置家庭测量人员请求*/
	CMD_FM_CONFIG_A = 117,  /*配置家庭测量人员响应*/
	CMD_FM_Q = 118,  /*查询家庭测量人员请求*/
	CMD_FM_A = 119,  /*查询家庭测量人员响应*/
	CMD_MESURE_TRANS = 120, /*传输测量结果*/
	CMD_MESURE_Q = 121,      /*查询测量结果请求*/
	CMD_MESURE_A = 122,      /*查询测量结果响应*/
	CMD_MESURE_DEL = 123,   /*删除测量结果*/
	CMD_MESURE_TRANS_CONFIRM = 124, /*设备服务器间同步测量数据确认*/
	CMD_PLUG_TIMER_Q = 125,	/* 定时开关的请求 */
	CMD_PLUG_TIMER_A = 126,	/* 定时开关的回应 */
	CMD_USER_DEBUG = 127,     /*手机上传调试信息*/
	CMD_IR_LIST = 128,		/*手机请求支持列表*/
	CMD_IR_CONFIG = 129,	/*手机配置和获取已配置家电*/
	CMD_IR_DB = 130,		/*设备从服务器获取编码库*/
	CMD_IR_URL = 131,		/*设备获取电器遥控列表的URL */
	CMD_IR_CTRL= 132,		/* 手机测试红外编码库是有有效 */
	CMD_REMOTE_CONFIG = 133, /* CMD_REMOTE_CONFIG */
	CMD_REMOTE_KEY_CONFIG = 134, /* 手机为家庭电器配置一个按键 */
	CMD_REMOTE_CODE = 135, /* 手机为按键配置一个编码 */
	CMD_REMOTE_CTRL= 136, /* 手机向家庭电器发送控制命令 */
	CMD_REMOTE_STATE = 137, /* 手机对单个电器状态的操作 */
	CMD_VIDEO_TEMP_QUALITY = 138, /* 用于云台控制时，临时调试画质，不保存配置 */
	CMD_VIDEO_RECOR_CTRL = 139,/*设备端本地录像命令------已经取消此报文*/
	CMD_ALARM_BIND_PHONE = 140,	/*报警短信绑定手机*/
	CMD_ALARM_MSG = 141,	/*报警信息发送请求*/
	CMD_FAIL_EX = 142,		/* cmd_fail扩展命令 */
	CMD_ALARM_CONFIG_PHONE = 143,
	CMD_SLAVE_HIS = 144, /*从设备通告视屏信息给主设备*/
	CMD_GET_CMDOK = 145,	/* 设备端向服务器查询手机认证成功后需要的相关信息 */
	CMD_ALARM_LOG = 146,/*报警日志相关操作*/
	CMD_ALARM_LOG_SYNC = 147,/*报警日志的同步*/
	CMD_TIME_SYNC = 148,/*设备向手机查询时间同步*/
	CMD_VTAP_LIST_Q = 149,/*查询设备录像列表*/
	CMD_VTAP_LIST_A = 150,/*响应设备录像列表的查询*/
	CMD_VTAP_Q = 151,/*请求设备录像文件*/
	CMD_VTAP_KEEP = 152,/*录像观看保活*/
	CMD_VTAP_A = 153,/*录像请求结果*/
	CMD_VTAP_TIMESTAP = 154, /*请求播放指定时戳*/

	CMD_REMOTE_BROADCAST = 155, /* 广播控制命令给从设备 */
	CMD_VIDEO_SONIX = 156, /*对sonix 设置*/
	CMD_REC_TIMER_Q = 157,/* 视频录制定时开关的请求 */
	CMD_REC_TIMER_A  = 158,/* 视频录制定时开关的回应 */
	CMD_VIDEO_CONTROL_ALARM_CFG = 159, /*侦测报警配置信息*/
	CMD_MS_DEV_INFO = 160, /*主从设备通信告知相互的版本，时区，支持的功能 */

	CMD_DEV_SAHRD_INFO = 161, /*设备共享命令，请求与响应相同*/
	CMD_DEV_SAHRD_COUNT = 162, /*设备分享次数增加命令*/
	CMD_OPEN_TELNET = 163, /*开启从设备telnet服务*/
	CMD_ALARM_SWITCH_CTL = 164, /*安防报警总开关控制*/
	CMD_VTAP_END = 165,/*设备通知客户端，录像播放完毕*/
	CMD_VIDEO_QUALITY_V2 = 166,/* 自定义分辨率设置 */
	CMD_PLUG_ELECTRIC_STAT = 167,/*电量统计功能支持查询命令*/
	CMD_SCENE_CONFIG = 168,/*情景模式操作命令*/
	CMD_OPT_SCAN_Q = 169, /*扫描刷机设备请求*/
	CMD_OPT_SCAN_A = 170, /*扫描刷机设备响应*/
	CMD_DEV_REG_Q = 171,  /*刷机设备注册请求*/
	CMD_DEV_REG_A = 172, /*刷机设备注册响应*/
	CMD_ARIA2C_Q = 173, /*aria2c控制请求*/
	CMD_ARIA2C_A = 174, /*aria2c控制响应*/
	CMD_NETWORK_DETECT_Q = 175, /* 视频网络探测请求包 */
	CMD_NETWORK_DETECT_A = 176, /* 视频网络探测响应包 */
	CMD_SSIDPW = 177, /*手机修改设备wifi SSID和密码*/
	CMD_DEVSERVER_LIST_Q = 178, /*获取设备服务器列表请求*/
	CMD_DEVSERVER_LIST_A = 179, /*获取设备服务器列表响应*/
	CMD_NET_PROBE_Q = 180,  /*设备到服务器网络探测请求*/
	CMD_NET_PROBE_A = 181,  /*设备到服务器网络探测响应*/
	CMD_LOCATION_Q = 182,  /*设备位置请求*/
	CMD_LOCATION_A = 183,  /*设备位置响应*/
	CMD_MACDENY = 184, /*添加wifi mac禁止列表*/
	CMD_MACALLOW = 185, /*删除wifi mac禁止列表*/
	CMD_BIND_SLAVE_INFO = 186,/*获取从设备绑定信息*/
	CMD_MASTER_SLAVE_CTRL = 187,/*主从之间的控制命令，不通过服务器*/
	CMD_REBOOT = 188,/*远程重启命令*/
	CMD_AREA_CONFIG = 189,/*区域操作命令*/	
	CMD_STATIC_PIC_Q = 190,/*最近静态图片请求*/
	CMD_STATIC_PIC_A = 191,/*最近静态图片响应*/
	CMD_POSITION_Q = 192, /*定位信息请求*/
	CMD_POSITION_A = 193, /*定位信息响应*/
	CMD_SPEED_MAX_Q = 194,/*限速阀值请求*/
	CMD_SPEED_MAX_A = 195,/*限速阀值响应*/
	CMD_SCHLBUS_BIND = 196,/*校车绑定*/
	CMD_VOICE = 197,
	CMD_VOICE_ACK = 198,
	CMD_SPEEK_Q = 199,
	CMD_SPEEK_A = 200,
	CMD_VOICE_REG = 201,
	CMD_REMOTE_CODE_UPLOAD = 202,/*上传本地设备学习到的控制编码到服务器*/
	CMD_RECORD_QUALITY_V2 = 203,/*录像质量配置*/
	CMD_NOTIFY_HELLO = 204, /* 设备与小区服务器握手请求 */
	CMD_NOTIFY_HELLO_ACK = 205, /* 设备与小区服务器握手响应 */
	CMD_NOTIFY = 206, /* 消息推送命令，如报警、健康、公告 */
	CMD_NOTIFY_RESULT = 207, /* 消息推送应答 */
	CMD_NOTIFY_EXPECT = 208, /* 消息ID 同步命令*/
	CMD_NOTIFY_CENTER_LIST = 209, /* 小区服务器把自己ip/port上报给服务器，服务器推送给设备*/
	CMD_CMT_OP_DEVICE = 210, /* 对小区所管理的设备进行操作，如添加。参数net_cmt_op_device_t*/
	CMD_VOICE_PROMPT  = 211, /* 播放本地音乐文件 */
	CMD_REMOTE_BD_BIND = 212,	/* 双向RF协议绑定解绑定 */
	CMD_REMOTE_CONFIG_SOUNDLIGHT = 213,	/* 配置安防报警器关联的声光报警器 */
	CMD_REMOTE_TD_CODE = 214, /* 电器二维码添加 */
	CMD_PHONE_BIND_Q = 215, /*绑定手机申请提交，参数net_phone_bind_t*/
	CMD_PHONE_REQUESTLIST_Q =216, /*绑定手机申请列表查询*/
	CMD_PHONE_REQUESTLIST_A =217, /*绑定手机申请列表响应，参数net_phone_bind_list_t*/
	CMD_PHONE_BIND_OPERATION = 218, /*已绑定手机对绑定申请操作，参数net_phone_bind_operation_t*/
	CMD_PHONE_BIND_RESULT = 219, /*已绑定手机对绑定申请操作结果，参数net_phone_bind_result_t*/
	CMD_PHONE_BIND_DEL = 220, /*删除绑定手机，net_phone_bind_uuid_t*/
	CMD_PHONE_UNBINDLOGIN_ALLOW = 221, /*允许未绑定手机登录*/
	CMD_PHONE_UNBINDLOGIN_DENY = 222, /*禁止未绑定手机登录*/
	CMD_PHONE_BINDLIST_Q = 223, /*绑定手机列表查询请求*/
	CMD_PHONE_BINDLIST_A = 224, /*绑定手机列表查询响应，参数net_phone_bind_list_t*/
	CMD_SCENE_TIMER_Q  =  225,	/*配置情景模式的定时器请求命令*/
	CMD_SCENE_TIMER_A  =  226,	/*配置情景模式的定时器的应答命令*/
	CMD_SCENE_LINKAGE = 227,	/* 联动情景配置命令 */
	CMD_PHONE_APN_OPERATION = 228, /*配置手机推送命令，参数net_phone_push_t*/
	CMD_STATIC_PIC_Q_V2 = 229,/*第二版本最近静态图片请求*/
	CMD_STATIC_PIC_A_V2 = 230,/*第二版本最近静态图片响应*/
	CMD_REMOTE_VTY = 231,/* 启动远程telnetd服务 */
	CMD_IA = 232,		/* 智能家居单品控制和查询 */
	CMD_NEWUPVER_Q = 233,	/*手机获取设备最新升级版本请求*/
	CMD_NEWUPVER_A = 234,	/*手机获取设备最新升级版本应答*/
	CMD_SET_NEWUPVER = 235,	/*手机设置设备升级版本*/
	CMD_NOTICE_DEVUP = 236,	/*手机通知设备立即升级*/
	CMD_V4L2_COLOR = 237, /* v4l2 color参数查询和控制 */
	CMD_NOTIFY_QUERY = 238, /* 查询推送消息请求，参数net_notify_query_t，响应CMD_NOTIFY */
	CMD_MOTO_ATTRIBUTE = 239, /* IPC云台属性相关命令 */
	CMD_MOTO_PRE_POSITION = 240, /* IPC云台预置位命令 */
	CMD_MOTO_CRUISE = 241, /* IPC云台巡航命令 */
	CMD_UDP_AUTH = 242,
	CMD_UDP_KEEPLIVE = 243,
	CMD_UDP_CTRL = 244,
	CMD_UDP_NOTIFY = 245,
	CMD_UDP_BIND_PHONE = 246,

	CMD_UDP_DNS_PROB = 301, /* 探测本地域名 */
	CMD_MAX
};
/*通信协议命令
禁止在已有命令中间插入新命令
只能顺序添加到后面*/


enum{
	ERR_NONE = 0,
	ERR_SN_INVALID = 1,    		/*DS007序列号无效*/
	ERR_NICKNAME_INVALID = 2,	/*用户昵称无效*/
	ERR_PASSWD_INVALID = 3,   	/*用户口令错误*/
	ERR_CMD_INVALID = 4,     	/* 无效命令*/
	ERR_PARAM_INVALID = 5, 		/*无效参数*/
	ERR_MEMORY = 6,       		/*服务器内部分配内存失败*/
	ERR_SYSTEM = 7,       		/*服务器内部系统还是调用失败*/
	ERR_NICKNAME_CONFLICT = 8,  /*昵称冲突*/
	ERR_NICKNAME_TOO_LONG =9, 	/*昵称过长*/
	ERR_EMAIL_TOO_LONG = 10,    /*email地址过长*/
	ERR_DATABASE = 11,			/*数据库操作失败*/
	ERR_CLIENT_VER = 12, 		/*手机客户端版本过低*/
	ERR_DEV_OFFLINE = 13,		/* 设备离线 */
	ERR_VEDIO_OFF = 14,		/* 未插入摄像头 */
	ERR_DEV_SYS_ERR = 15,		/* 设备系统错误 */
	ERR_SELLER_NAME_CONFLICT = 16,/*销售人员用户名冲突*/
	ERR_TOO_MANY = 17, /* 太多人在观看视频了 */
	ERR_PACKAGE_NAME_CONFLICT = 18, /* 太多人在观看视频了 */
	ERR_OUT_SERVICE = 19, /* 服务到期 */
	ERR_CARD_SN_INVALID = 20, /*充值卡序列号无效*/
	ERR_CARD_PWD_INVALID = 21, /*充值卡密码无效*/
	ERR_CARD_STATE_INVALID = 22, /*充值卡状态无效*/
	ERR_CARD_NOTIME_TRANS = 23, /*设备无服务期限可转移*/
	ERR_TIMEOUT1 = 24, /*超时失败*/
	ERR_AGENT = 25,		/* 代理失败*/
	ERR_EMAIL_INVALID =26, /*email地址无效*/
	ERR_FM_ID = 27,/* 家庭成员ID无效 */
	ERR_FM_LIMIT = 28, /* 家庭成员配置过多 */
	ERR_DEV_SYS_BUSY = 29, /* 系统忙，可能正在升级系统 */
	ERR_PLUG_TIMER_LIMIT = 30, /* 定时开关策略配置个数已达到最大 */
	ERR_PLUG_TIMER_ID = 31, /* 定时开关策略ID无效 */
	ERR_REMOTE_LIMIT = 32, /* 可控制电器配置已达最大数量 */
	ERR_IR_DB_INVALID = 33, /* 红外编码库错误 */
	ERR_REMOTE_BUTTON_LIMIT = 34, /* 可控电器按键达到上限 */
	ERR_REMOTE_ID_INVALID = 35, /* 可控电器ID无效 */
	ERR_REMOTE_KEY_ID_INVALID = 36, /* 可控电器KEY ID无效 */
	ERR_REMOTE_BUSY = 37, /* 电器正忙，比如处于学习状态 */
	ERR_REMOTE_KEY_VALID = 38, /* 电器按钮无效 */
	ERR_REMOTE_CODE_LEARN_FAILED = 39, /*学习失败*/
	ERR_PHONE_NUM_EXCESS = 40,/*超出支持的最大电话绑定数*/
	ERR_NO_BIND_PHONE = 41,/*该智能网关尚未绑定手机*/
	ERR_DEV_UNLINK = 42,/*设备未连接设备服务器*/
	ERR_ALARM_PHONE_NOT_FOUNT = 43, /*绑定的报警手机号不存在*/
	ERR_ALARM_VIDEO_NOT_FOUNT = 44,/*没有指定的报警录像*/
	ERR_ALARM_LOG = 45,/*报警日志操作出错*/
	ERR_ALARM_LOG_SYNC = 46,/*报警日志同步出错*/
	ERR_REC_TIMER_LIMIT = 47,/*视频录制定时器:已经达到策略最大可配置数*/
	ERR_REC_TIMER_OPT = 48,/*视频录制定时器:操作失败*/
	ERR_REC_TIMER_ID   = 49,/*视频录制定时器:定时器id无效*/
	ERR_REC_TIMER_NTP = 50,/*ntp 未同步 添加失败*/
	ERR_REC_TIMER_DURATION = 51,/*时长太短*/
	ERR_NO_VTAP = 52,/*没有视频录像文件*/
	ERR_SLAVE_OFFLINE = 53, /* 从设备离线 */
	ERR_DPI_FOR_PHONE = 54, /* 手机在线不支持大分辨率、帧率的配置 */
	ERR_CODE_ADJUST = 55, /* 对应编码不支持微调 */
	ERR_VTAP_CLIENT_EXCEED = 56, /*观看录像的人太多了，同时只能有1个人看*/
	ERR_VTAP_DAMAGE = 57,/*录像文件损坏*/
	ERR_SCENE_VERSION = 58,/*版本儿不匹配*/
	ERR_SCENE_ID	=59,/*非法场景id*/
	ERR_SCENE_FAIL	=60,/*执行失败*/
	ERR_SCENE_ACTION	= 61,/*操作非法*/
	ERR_SCENE_ID_MAX	=62,/*场景id已达上限*/
	ERR_SCENE_BUSY		=63,/*执行忙*/
	ERR_AREA_VERSION = 64,/*区域版本儿不匹配*/
	ERR_AREA_ID	=65,/*非法区域id*/
	ERR_AREA_FAIL	=66,/*操作失败*/
	ERR_AREA_ACTION	= 67,/*操作非法*/
	ERR_AREA_ID_MAX	=68,/*区域id已达上限*/
	ERR_AREA_ERR_OBJTYPE =69,/*错误的对象类型*/	
	ERR_NO_SD_DETECTED = 70, /* 没有存储设备插入 */
	ERR_NOT_SUPPORT = 71,/*设备不支持*/
	ERR_BUSY = 72,/*正与其他人通话*/
	ERR_REMOTE_NOT_SUPPORT = 73,/*本设备不支持电器控制,也许是从设备支持*/
	ERR_TF_NOT_INSERT = 74,/*TF卡未插入*/
	ERR_REMOTE_INVALID_TD = 75,/* 添加电器未知的二维码信息 */
	ERR_UNBIND =  76, /*非绑定用户禁止登录*/
	ERR_BIND_FULL =77, /*达到绑定数量限制，不能申请绑定了*/
	ERR_BINDLATE = 78, /*已经有人对绑定申请进行处理了*/
	ERR_SCENE_TIMER_LIMIT = 79, /*情景模式定时器操作最大数*/
	ERR_SCENE_TIMER_ID = 80,	/*情景模式定时器的ID号不对*/
	ERR_SCENE_INVALID_ID = 81, /* 场景联动时无效的场景ID */
	ERR_SCENE_INVALID_REMOTE_ID = 82, /* 场景联动时无效的电器ID */
	ERR_UNBIND_WITH_DEV_OFFLINE = 83, /*未绑定登录失败，且设备离线，不要提示用户申请绑定*/
	ERR_DUPLICATE_REMOTE_CODE = 84,	/* 重复添加按键编码 */
	ERR_IA_NOT_READY = 85, /*智能单品没初始化*/
	ERR_IA_OPERATE_INVALID = 86, /*智能单品操作无效*/
	ERR_UPGRADE_VER_EMPTY = 87, /*手机查询设备最新升级信息为空*/

	ERR_NEED_ENCRPYT = 88, /* 需要加密，却无法协商出一样的加密算法 */
	ERR_CLONE = 89,  /* 设备是克隆机 */
	ERR_NOT_LEARN = 90,	/* 设备还没有学习编码 */
	ERR_MAX /* 请在此之前添加错误码 */
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
#define SN_LEN 12	/*DS007设备序列号字符串长度*/
#define MAX_NICKNAME 16  /*用户昵称最大长度*/
#define MAX_PLUG_TIMER_NAME	64	/* 定时开关名称最大长度 */
#define MAX_EMAIL 32
#define MAX_PHONE_SINGLE 16
#define MAX_PHONE_NUM 10
#define MAX_SL_NUM (MAX_REMOTE_NUM - 1)
#define MAX_PHONE (MAX_PHONE_SINGLE * MAX_PHONE_NUM)
#define MAX_WEB_ROOT 64
#define MAX_HANDLE 0xFFFFFFFF
#define MAX_SERVER_HANDLE 0xEE6B2800 /*保证设备端与服务器产生的handle不能重复*/
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

/* 用原始接收到的协议头部填充新构造协议头部V2部分(注意SRC中V2的SN在接收时候被转换了字节序),并且填充v1的handle */
#define net_header_v2_wraper(dst, src) do {\
	memcpy((char*)(dst)+net_header_size, (char*)(src)+net_header_size, net_header_v2_size - net_header_size);\
	((net_header_v2_t*)(dst))->sn = ntoh_ll(((net_header_v2_t*)(src))->sn);\
	((net_header_t*)(dst))->handle = ((net_header_t*)(src))->handle;\
} while (0);

#pragma pack(pop)
#endif

