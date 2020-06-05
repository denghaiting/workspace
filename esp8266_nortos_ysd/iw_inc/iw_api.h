#ifndef __IWUOS_API_HEADER__
#define __IWUOS_API_HEADER__

#include "c_types.h"

/*
	函数调用返回值
*/
typedef int rs_t;

typedef int RS;
#define	RS_OK				0 		/* 操作成功完成 */
#define	RS_ERROR			-1		/* 出错 */
#define	RS_NOT_INIT		-2
#define	RS_NOT_SUPPORT		-3
#define	RS_NOT_LOGIN		-4		/* 尚未登录 */
#define	RS_INVALID_PARAM	-5		/* 无效的参数 */
#define	RS_EMPTY			-6		/* 没有可用数据 */
#define RS_MEMORY_MALLOC_FAIL  -7	/* 内存分配失败 */
#define	RS_TOO_MANY	-8
#define RS_NTP_NONE -9

#define	RS_EXIST			1		/* 添加时已存在 */
#define	RS_NOT_FOUND		2		/* 未找到 */
#define RS_INVALID_LICENSE 	3		/*无效授权文件*/
#define RS_OFFLINE 			4		/* 设备离线 */
#define RS_DOMAIN_SAME      5       /* 相同的域名 */


#define IW_MAX_DESC 64
#define IW_MAX_WIFI_SSID 32
#define IW_MAX_WIFI_PASSWD 64

#define MAX_VENDOR_LEN 16
#define MAX_DESC_LEN 32


typedef enum{
	SMCF_STA_NONEED_SMCF = 1,//不需要一键配置
	SMCF_STA_ISSCAN_AP,//扫描ap，看是否有产测,手机热点的ssid存在
	SMCF_STA_SNIFF,//接收sniff报文
	SMCF_STA_ISLOCKAP,//锁定到ap信道,持续收包中
	SMCF_STA_CONFOK,//一键配置成功
	SMCF_STA_CONNECTAP,//连接上AP
	SMCF_STA_GETIP,//连接上AP,而且获取到ip
	SMCF_STA_DISCONNECTAP//连接断开
}smcf_sta_t;


//事件处理
typedef struct event_s{
	bool have_event; 
}event_t;

enum {
	EVENT_SYS_SAVE_CONFIG,	// 请求保存配置
	EVENT_SYS_REBOOT,		// 请求重启系统
	EVENT_SYS_RESET, 	//reset
	EVENT_SETUP_TIMER,
	EVENT_SYS_WIFI_CONFIG,
	EVENT_SYS_MAX
};

typedef struct sys_shm_s{
	event_t event[EVENT_SYS_MAX];
}sys_shm_t;


/*
	系统相关信息
*/
typedef struct {
	// 系统描述信息。多个字段用空格隔开
	char descript[IW_MAX_DESC];
	// 序列号
	u_int64_t sn;
	u_int32_t version;
	
	// 系统有多少个GPIO口。从0开始顺序编号。
	u_int16_t num_gpio;
	// 系统有多少个串口。从0开始顺序编号。
	u_int16_t num_uart;
	// 系统允许最多多少个客户端同时登录
	u_int16_t max_client;
	// 系统允许最多创建多少个定时器
	u_int16_t	max_timer;
	
	bool is_smart_link_config;
	char ssid[IW_MAX_WIFI_SSID];
	char passwd[IW_MAX_WIFI_PASSWD];
	u_int8_t smcf_stat;//一键配置的状态反馈
	
	// Wi-Fi是否连接成功
	bool is_linkup;
	// 从Wi-Fi连接成功到现在过了多少秒
	u_int32_t linkup_time;
	// 从AP分配到的IP地址
	u_int32_t ip;
	// 从AP分配到的网络掩码
	u_int32_t mask;
	// 网关地址
	u_int32_t gateway;
	// 域名服务器地址
	u_int32_t dns;

	// 与云服务器是否正常连接
	bool is_server_connect;
	// 与云服务器正常连接到现在过了多少秒
	u_int32_t server_connect_time;
	u_int32_t up_time;
	
	// 与路由器连接了多少秒
	u_int32_t wifi_connect_time;
} iw_sys_info_t;

/*
	初始化需要的信息
*/
typedef struct {
	// 厂商ID
	char vendor[MAX_VENDOR_LEN];
	// 产品描述
	char desc[MAX_DESC_LEN];
} iw_init_t;

/**************************************************
 	系统操作
 **************************************************/

/*
	功能:
		系统初始化
	输入参数:
		@ii: 一些系统信息
	输出参数:
		无
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:		
*/
extern rs_t iw_sys_init(iw_init_t *ii);

/*
	功能:
		获取系统信息
	输入参数:
		@info: 存放系统信息的内存块
	输出参数:
		@info: 保存系统信息
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
extern rs_t iw_sys_info_get(iw_sys_info_t *info);

/*
	功能:
		重启系统
	输入参数:
		无
	输出参数:
		无
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
		立即重启系统
*/
extern rs_t iw_sys_reboot();

/*
	功能:
		恢复出厂设置并重启系统
	输入参数:
		无
	输出参数:
		无
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
		清空所有保存的配置后，立即重启系统
*/
extern rs_t iw_sys_reset();

/*
	客户端信息。
*/
typedef struct {
	// 该客户端在本地的唯一标识
	u_int16_t client_id;
	// 客户端的公网UDP端口
	u_int16_t port;
	// 客户端的公网IP地址
	u_int32_t ip;
	// 连接上来到现在过了多少秒
	u_int32_t connect_time;
	// 客户端最后一次活跃时间
	u_int32_t last_active;
	// 是否是局域网直连
	bool is_direct;
	// 是否是登录成功了的
	bool is_login;
	// 填充
	char pad[2];
} iw_client_t;

/**************************************************
 	事件注册、回调
 **************************************************/

/*
	事件类型定义
*/

/*
	一键配置成功
*/
#define	IW_EV_SMART_CONFIG_OK	0x0101
// 网络连接成功
#define	IW_EV_LINK_UP	0x0102
// 网络断开
#define	IW_EV_LINK_DOWN	0x0103
// ip地址、掩码、网关、DNS等有变化
#define	IW_EV_IP_MODIFY	0x0104
// 与服务器连接成功
#define	IW_EV_SERVER_UP	0x0105
// 与服务器连接失败
#define	IW_EV_SERVER_DOWN	0x0106
// 时钟NTP同步成功
#define	IW_EV_TIME_SYNC	0x0107
// 客户端登录成功
#define	IW_EV_USER_LOGIN	0x0110
// 处理客户端的GET请求
#define	IW_EV_OBJECT_GET	0x0201
// 处理客户端的SET请求
#define	IW_EV_OBJECT_SET	0x0202
// 处理客户端的delete请求
#define	IW_EV_OBJECT_DELETE	0x0203


//串口相关
//读取到一个字符到统一回调函数
#define	IW_EV_UARTR_2COMMON			0x0301 //统一接收回调
#define IW_EV_UARTR_2ACTIVE			0x0302 //回调到激活模块
#define IW_EV_UARTR_2PRETEST		0x0303 //回调到产测模块
#define IW_EV_UARTR_2IJPROTO		0x0304 //i+ 串口协议状态机处理


// 发送一个字符
#define	IW_EV_UARTW_2COMMON			0x0331 //统一发送回调

// GPIO中断
#define	IW_EV_GPIO_INTR	0x0401
// 定时器到点
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
		定时器到点事件: 定时器ID
		客户端登录登出事件: 客户端ID
		串口读写事件: 串口ID
		报文事件: 客户端ID
	*/
	u_int16_t id;
	// 收到网络报文中的动作，UCA_XXX
	u_int8_t action;
	u_int8_t pad;
	// GPIO_IN_COUNT模式时，1秒有多少个方波输入
	u_int32_t gpio_count;
	void *s;
	// 回调联合
	union{
	// 其它回调参数，入定时器
	void *cp;
	// 用于收到报文回调
	iw_obj_t obj;
	}cu;
} iw_event_info_t;

typedef rs_t (* iw_event_callback_t)(iw_event_info_t *info);
typedef rs_t (* iw_timer_callback_t)(void *cp);

/*
	功能:
		向系统注册事件回调
	输入参数:
		@event: 事件，IW_EV_XXX
		@callback: 回调函数
	输出参数:
		无
	返回值:
		RS_OK: 设置成功
		其他: 错误
	说明:
*/
extern rs_t iw_event_register(u_int16_t event, iw_event_callback_t callback);

/*
	功能:
		告知系统自己支持哪些对象操作
	输入参数:
		@obj: 支持的对象数组
		@num: 对象个数
	输出参数:
		无
	返回值:
		RS_OK: 设置成功
		其他: 错误
	说明:
		告知系统自己支持哪些对象。不支持的对象，系统收到
		客户端请求时候，系统会应答错误，不会交给上层回调
		处理
*/
extern rs_t iw_obj_register(iw_obj_t *obj, int num);

/*
在串口里面接收到字符的入口回调函数
*/
extern void  iw_event_foruart(uint8 IN_value);


/*
	功能:
		时间请求
	输入参数:
		@event: 注册的事件的id
	输出参数:
		无
	返回值:
		无
	说明:
*/
void  iw_request_event(unsigned char event);


/*
	功能:
		打印16进制数据
	输入参数:
		@msg: 数据的说明
		@ptr: 数据
		@len: 数据长度
	输出参数:
		无
	返回值:
		无
	说明:
*/
extern void mem_dump(char *msg, unsigned char *ptr, int len);
/*
	功能:
		判断需不需一键配置
	输入参数:
	输出参数:
	返回值:
		true: 是
		flase: 不是
	说明:
*/
bool  needsmartconfig(void);
#endif
