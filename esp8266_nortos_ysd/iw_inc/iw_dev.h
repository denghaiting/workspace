#ifndef __IWUOS_DEV_HEADER__
#define __IWUOS_DEV_HEADER__

//#include "espconn.h"
//#include "stlc_list.h"
#include "iw_func_support.h"

#define	MAX_UCDS_SESSION	10

#include "iw_lan_srv.h"
#include "iw_api.h"
typedef enum {
	UCD_ST_CLIENT = 1,	// 回话结构体是 ucdc_session_t
	UCD_ST_SERVER = 2, // 回话结构体是 ucds_session_t
} UCD_ST;

typedef struct {
	// 回话类型，必须放在结构体第一个地方 ,UCD_ST_XXX
	u_int8_t ucd_session_type;

	ucd_t *dev;
	u_int8_t uuid[MAX_UUID_BIN_LEN];

	char name[32];

	int status;
	// 会话ID，请求认证时候为0，服务器/设备应答answer时填写
	u_int32_t client_sid;
	u_int32_t device_sid;

	u_int8_t my_request_id;
	u_int8_t peer_request_id;
	u_int8_t peer_last_request_id; /* 记录服务器上次请求的request_id,用于跟 peer_request_id比较判断是否是重传包  */


	u_int32_t link_server_time;		/* 连接服务器时的时间，用于其他模块判断这个会话是否重连*/


	//对每个session做最后一次发包缓存
	pkt_t *last_pkt;
	//是否可以发送
	bool last_pkt_enable;

	//是否uuid flash写失败
	bool uuid_write_failed;

	// 选择的加密方式
	u_int16_t select_enc;
	//是否加密
	bool is_enc;

	u_int8_t rand1[4];
	u_int8_t rand2[4];

	// 刚创建是0，后面变成正常的
	int idle_time;

	// 频繁解析也没必要
	u_int32_t next_resolve_time;
	// 对端的IP地址、端口号。主机序
	u_int32_t ip;
	int port;

	// 发送的重试次数
	int send_retry;
	// 指向time_param中的一个，当前使用的
	uc_time_param_item_t *time_param_cur;
	// 对端传过来的时间参数，修改成本地字节序了
	uc_time_param_all_t time_param;
	// 对端传过来的时间参数，原始的网络字节序的
	uc_time_param_all_t time_param_net;


	// 加解密秘钥
	u_int8_t key[16];

	u_int8_t *rcv_buf;
	u_int16_t rcv_len;
	struct stlc_list_head send_list;
	struct espconn *conn; /* 保存piw_priv->devconn，在devconn失效时候更新，不可释放 */
	int exception_num;
} ucdc_session_t;

#define CHIP_DEVINFO_STRLEN_MAX (64)
typedef struct _ucd_s {
	u_int64_t sn;

	// IJ_XXX
	u_int8_t dev_type;
	u_int8_t ext_type;
	u_int8_t chip_devinfo[CHIP_DEVINFO_STRLEN_MAX];

	u_int8_t mac[6];
	char passwd[32];
	u_int8_t passwd_md5[16];
	u_int8_t vendor[MAX_VENDER];

	// 与服务器的预共享秘钥
	u_int8_t preshare_key[8];

	// 是否是克隆机
	u_int8_t is_clone;
	//设备客户端加密类型选择
	u_int16_t dev_enc;
	u_int8_t pad;

#ifdef HAS_PKT_CLONE
	/* 是否上报克隆报文 */
	bool is_clone_pkt_enable;
#endif

	/************** 与服务器连接的一些信息 **************/
	ucdc_session_t client;

	/************** 客户端相关的信息，包括局域网直连和广域网的 **************/
	//SOCKET sock_server;
	//cl_thread_t *t_server_read;
	// 所有客户端来的会话
	ucds_session_t sessions[MAX_UCDS_SESSION];
	// 下一个sid
	u_int32_t next_cleint_sid;

} ucd_t;

/*
	功能:
		改变设备连接服务器的状态
	输入参数:
		@s: 服务器与设备之间建立的回话
		@status: 设备的状态
	输出参数:
	返回值:
	说明:
*/
rs_t  iw_dev_init(iw_priv_t *piw);

/*
	功能:
		添加一个包到会话的发送队列中去
	输入参数:
		@s: 服务器与设备之间建立的回话
		@pkt: 数据包
	输出参数:
		RS_OK: 正确
		其他: 失败
	返回值:
	说明:
*/
rs_t ucdc_request_add(ucdc_session_t *s, pkt_t *pkt);

/*
	功能:
		读取证书等
	输入参数:
	输出参数:
		RS_OK: 正确
		其他: 失败
	返回值:
	说明:
*/
int iw_ucd_license(void);

/*
	功能:
		发送数据包
	输入参数:
		@s: 服务器与设备之间建立的回话
		@pkt: 数据包
	输出参数:
		RS_OK: 正确
		其他: 失败
	返回值:
	说明:
		应答报文不需要重传，这里要注意一下，因为加了最后包缓存，用该函数发送数据后不用释放包
*/
int ucdc_send_pkt(ucdc_session_t *s, pkt_t *pkt);

/*
	功能:
		发送数据包
	输入参数:
		@s: 服务器与设备之间建立的回话
		@pkt: 数据包
	输出参数:
		RS_OK: 正确
		其他: 失败
	返回值:
	说明:
		 应答报文不需要重传,可靠传输链表里的定时发送函数，不需要记录最后一次发送缓存
*/
RS ucdc_send_pkt_no_cach(ucdc_session_t *s, pkt_t *pkt);

/*
	功能:
		复位设备端连接服务器的状态，重新连接服务器
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void reset_server_session(void);


extern char ucd_rcv_buf[MAX_UC_PKT_SIZE];
extern ucd_t ucd;

#endif
