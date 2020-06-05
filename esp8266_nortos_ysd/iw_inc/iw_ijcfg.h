/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: ijcfg
**  File:    ijcfg.h
**  Author:  liubenlong
**  Date:    07/28/2014
**
**  Purpose:
**    Ijcfg.
**************************************************************************/


#ifndef IJCFG_H
#define IJCFG_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */
#define	IJCFG_PORT_DEV	8818
#define	IJCFG_PORT_PC	8819


#define	CFGPT_RESULT			0
#define	CFGPT_DISCOVERY			1
#define	CFGPT_I_HERE			2
#define	CFGPT_SET_CONFIG		5
#define	CFGPT_GET_CONFIG		6

#define	CFG_ENC_NONE	0
// 使用设备密码加密
#define	CFG_ENC_PASSWD	1
// 使用缺省密码加密
#define	CFG_ENC_DEFAULT	2

#define	CFGPF_MASTER_SUPPORT	0x02
#define	CFGPF_UDP_CTRL	0x20

#define	CFG_DEFAULT_PASSWD	"galaxywind-enc"

#pragma pack(push,1)

#define	CFGP_CFG_HDR_LEN	(sizeof(cfgp_cfg_t) - 16)

enum {
	SYS_MODE_SLAVE,
	SYS_MODE_MASTER,
	// independence, 独立
	SYS_MODE_INDPD,
	SYS_MODE_MAX
};


/* Type definitions. */
typedef struct {
	bool init;
	u_int8_t *buff;
	int buff_len;
	int fast_push_num;
	//是否已经配置上网的ssid和密码
	int wifi_configed;
} ijcfg_t;

typedef struct {
	u_int64_t src_sn;
	u_int64_t dst_sn;
	// CFGPT_XXX
	u_int16_t cmd;
	// 参数长度
	u_int16_t param_len;
	// 事务ID
	u_int16_t req_id;
	// CFGPF_xxx
	u_int16_t flags;
	//后面紧跟数据
	//u_int8_t data[0];
} cfg_pkt_hdr_t;


// 配置信息，加密发送。从pad开始加密
typedef struct {
	u_int16_t config_count;
	// 加密方式, CFG_ENC_xxx
	u_int8_t enc;
	u_int8_t resv;
	// 如果是全0表示没加密
	u_int8_t checksum[16];
	// 配置信息
	//...
	u_int8_t pad[16];
} cfgp_cfg_t;


typedef struct {
	u_int8_t type;
	u_int8_t sub_type;
	// 配置计数，每改变一次配置就加1
	u_int16_t config_count;	
	u_int8_t auth[16];
	u_int16_t sys_mode;/*设备运行模式：主、从、独立*/
	u_int8_t ext_type;
	u_int8_t pad;
	/*设备需要上报设备属性字符串，引入这个TLV,方便后面扩展*/
	//uc_tlv_t *tlv;

} cfgp_i_here_t;


#pragma pack(pop)
/* External function declarations. */
/*
	功能:
		设备维护结构
	输入参数:
		@piw: 存放系统信息的结构
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
int iw_init_ijcfg(iw_priv_t *piw);


/* Macro API definitions. */


/* Global variable declarations. */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* IJCFG_H */

