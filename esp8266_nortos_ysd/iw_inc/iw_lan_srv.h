#ifndef __IWUOS_LAN_SRV_HEADER__
#define __IWUOS_LAN_SRV_HEADER__

#include "os_type.h"
#include "stlc_list.h"
#include "udp_ctrl.h"
#include "iw_api.h"


#define MAX_VENDER 16

typedef struct {
	// 回话类型，必须放在结构体第一个地方 ,UCD_ST_XXX
	u_int8_t ucd_session_type;	

	u_int8_t is_used;
	// 是否是局域网直连
	u_int8_t is_lan;
	// 是否是前台
	u_int8_t is_front;
        
	//是否是红外产测声音push
	u_int8_t ir_sound_push;
	//是否需要调试信息
	u_int8_t debug;
	
	struct stlc_list_head link;

	char name[32];
	// 当前状态，UCCS_xxx
	int status;

	// 可能是ucd.sock_server，也可能是ucd.sock_client
	//SOCKET sock;
	// 这里只是保存一下是属于代理还是局域网，不可释放
             struct espconn *conn;
        
	// 会话ID
	u_int32_t client_sid;

	// 请求ID
	u_int8_t my_request_id;
	u_int8_t peer_request_id;
	u_int8_t my_last_request_id;
	u_int8_t peer_last_request_id;

	//对应客户端时区，避免手机时区不一致问题。
	u_int8_t time_zone_for_client;
	//对每个session做最后一次发包缓存
	pkt_t *last_pkt;
	//是否可以发送
	bool last_pkt_enable;

        
	//是否可以wan口push
	bool wan_push_enable;
	//是否可以lan口push
	bool need_push;
	//是否接受到过udp ctrl 报文,避免手机与设备建立连接不同步,避免设备建立连接成功手机还没有，这时发包手机不回。
	bool is_recv_udp;
	// 地址端口号
	u_int16_t port;
	u_int16_t pad;
	u_int32_t ip;

	// 加解密秘钥. sizeof(symmetric_key) = 516
	u_int8_t key[520];
	// 随机数
	u_int8_t rand1[4];
	u_int8_t rand2[4];

	// 加密、解密函数
	ucs_enc_func_t enc;
	ucs_dec_func_t dec;
	
	// 时间参数，指向 ucd->time_param中的对应项
	uc_time_param_item_t *time_param;

        
	bool is_enc;
	// 选择的加密方式
	u_int16_t select_enc;
	// 死亡的原因
	u_int16_t die_err;     
	bool issupport_tt;//支持透明传输  Transparent transmission

	// 客户端的UUID
	u_int8_t uuid[MAX_UUID_BIN_LEN];

	// 发送请求的队列
	struct stlc_list_head send_list;
	// 发送的重试次数
	int send_retry;
	// 重试发送的定时器
	u_int16_t t_send;
	u_int16_t t_die;
}ucds_session_t;

extern int uci_send_index;
extern u_int8_t uci_send_buf[MAX_UC_PKT_SIZE];
extern u_int8_t uci_proc_num;

/*
	功能:
		复位所有的会话
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void other_client_session_reset(void);
/*
	功能:
		网络数据控制，里面分为system和ia
	输入参数:
		@info: 信息
		@uobj: 属性值
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
rs_t  iw_do_obj_ctrl(iw_event_info_t *info, iw_obj_t *uobj);
/*
	功能:
		网络数据回复包设置
	输入参数:
		@err: 通信协议命令 参见ds_proto.h
		@uobj: 属性值
	输出参数:
	返回值:
	说明:
*/
void uc_do_set_reply(ucp_obj_t *uobj, u_int16_t err);
/*
	功能:
		UDP CTRL 通用报文追加对象，用于对端请求回报文
	输入参数:
		@buf: 缓存地址
		@size: 缓存大小
		@pos: 已储存的缓存位置标记
		@object: 对象
		@sub_object: 子对象
		@attri: 属性
		@len: 参数长度
		@value: 参数值
	输出参数:
	返回值:
		0: 成功
	说明:
*/
int uc_put_elemet(u_int8_t *buf, int size, int *pos, u_int16_t object, u_int16_t sub_object, u_int16_t attri, u_int16_t len, u_int8_t *value);
/*
	功能:
		网络报文过来，通过get，set，delete事件回调注册的事件
	输入参数:
		@s: 会话
		@is_from_server: 是服务器还是手机端
		@action: 动作get，set，delete
		@uobj: 三级属性， 参数长度
		@param: 参数值
	输出参数:
	返回值:
	说明:
*/
void  do_udp_ctrl_core(void *s, bool is_from_server, u_int8_t action, ucp_obj_t *uobj, u_int8_t *param);
/*
	功能:
		得到在线的客服端
	输入参数:
	输出参数:
	返回值:
		int: 在线的客服端个数
	说明:
*/
int ucds_get_clients(void);
/*
	功能:
		复位除ds其他的所有的会话
	输入参数:
		@ds: 指定的会话
	输出参数:
	返回值:
	说明:
*/
void other_session_reset(ucds_session_t *ds);
/*
	功能:
		服务器转发的数据，和局域网手机端直接发送的数据处理
	输入参数:
		@conn: 设备端的网络结构
		@data: 数据值
		@len: 数据长度
	输出参数:
	返回值:
	说明:
*/
void ucds_proc_pkt(struct espconn *conn, u_int8_t *data, int len);
/*
	功能:
		UDP CTRL 通用报文追加对象，用于对端请求回报文
	输入参数:
		@buf: 缓存地址
		@size: 缓存大小
		@pos: 已储存的缓存位置标记
		@object: 对象
		@sub_object: 子对象
		@attri: 属性
		@len: 参数长度
		@value: 参数值
	输出参数:
	返回值:
		0: 成功
		<0:失败
	说明:
*/
int _uc_put_elemet(u_int8_t *buf, int size, int *pos, u_int16_t object, u_int16_t sub_object, u_int16_t attri, u_int16_t len, u_int8_t *value);
/*
	功能:
		智能设备控制
	输入参数:
		@info: 信息
		@uobj: 属性值
	输出参数:
	返回值:
	说明:
*/
void ia_do_public(iw_event_info_t *info, iw_obj_t *uobj);

int ucds_request_add(ucds_session_t *s, pkt_t *pkt);
#endif
