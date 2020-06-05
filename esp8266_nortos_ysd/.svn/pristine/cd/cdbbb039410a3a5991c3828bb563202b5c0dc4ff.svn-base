/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: object
**  File:    obj_system.c
**  Author:  cailiyang
**  Date:    08/16/2014
**
**  Purpose:
**    System.
**************************************************************************/
#include "iw_priv.h"
#include "iw_obj_system.h"
#include "iw_upgrade.h"
#include "iw_linkage_conf.h"
#include "iw_func_support.h"
#include "iw_lan_srv.h"
#include "iw_dev_timer.h"
#include "iw_uart_active.h"
#include "iw_ntp_time_syn.h"
#include "c_types.h"
#include "iw_func_support.h"
#include "domain.h"
#include "ysd_version.h"
#include "stm_upgrade.h"

/* Type definitions. */
typedef struct sub_obj_support_s {
	u_int16_t obj;
	u_int16_t obj_sub;
	u_int16_t addr;
} sub_obj_support_t;

/* Local function declarations. */
static int uc_put_elemet2(u_int8_t *buf, int size, int *pos, u_int16_t object, u_int16_t sub_object, u_int16_t attri);

static void do_ucot_system_support(iw_event_info_t *info, iw_obj_t *uobj);
static void do_ucot_system_hostname(iw_event_info_t *info, iw_obj_t *uobj);
static void do_ucot_system_software(iw_event_info_t *info, iw_obj_t *uobj);
static  void do_ucot_system_svn(iw_event_info_t *info, iw_obj_t *uobj);
static void do_ucot_system_upgrade(iw_event_info_t *info, iw_obj_t *uobj);
static void do_ucot_system_hardware(iw_event_info_t *info, iw_obj_t *uobj);
static void do_ucot_system_vendor(iw_event_info_t *info, iw_obj_t *uobj);
static void do_ucot_system_server(iw_event_info_t *info, iw_obj_t *uobj);

static void do_ucot_system_user(iw_event_info_t *info, iw_obj_t *uobj);
static void do_ucot_system_pwd(iw_event_info_t *info, iw_obj_t *uobj);

static void do_ucot_system_version(iw_event_info_t *info, iw_obj_t *uobj);
static void do_ucot_system_reboot(iw_event_info_t *info, iw_obj_t *uobj);
static void do_ucot_system_uptime(iw_event_info_t *info, iw_obj_t *uobj);
static void do_ucot_system_ip(iw_event_info_t *info, iw_obj_t *uobj);
static int sys_get_debuginfo(u_int8_t **buff);
extern u_int32_t get_cpu_percentage(void);

static void do_ucot_system_licence(iw_event_info_t *info, iw_obj_t *uobj);


const char FwCreatedDate[] = __DATE__;
const char FwCreatedTime[] = __TIME__;
char *disp_server = DEF_DISPATCHER_DOMAIN;


/* Macro API definitions. */
#define OBJ_ARRAY_NUM			sizeof(obj_array)/sizeof(obj_array[0])
//#define SYS_DEBUG_
#ifdef SYS_DEBUG_
#define SYS_INFO(x...)  INFO(x)
#define SYS_DEBUG(x...) DEBUG(x)
#define SYS_ERROR(x...)  ERROR(x)
#else
#define SYS_DEBUG(x...)
#define SYS_INFO(x...)
#define SYS_ERROR(x...) ERROR(x)
#endif

/* Global variable declarations. */

static sub_obj_support_t obj_array[] = {
	{UCOT_SYSTEM, UCSOT_SYS_SUPPORT, UCAT_SUPPORT_SUPPORT},
	{UCOT_SYSTEM, UCSOT_SYS_HOSTNAME, UCAT_HOSTNAME_HOSTNAME},
	{UCOT_SYSTEM, UCSOT_SYS_SOFTWARE, UCAT_SYS_VERSION},
	{UCOT_SYSTEM, UCSOT_SYS_SOFTWARE, UCAT_SYS_REBOOT},
	{UCOT_SYSTEM, UCSOT_SYS_SOFTWARE, UCAT_SYS_UPTIME},
	{UCOT_SYSTEM, UCSOT_SYS_SOFTWARE, UCAT_SYS_IP},
	{UCOT_SYSTEM, UCSOT_SYS_SOFTWARE, UCAT_SYS_SVN},
	{UCOT_SYSTEM, UCSOT_SYS_SOFTWARE, UCAT_SYS_SSID},
	{UCOT_SYSTEM, UCSOT_SYS_SOFTWARE, UCAT_SYS_PASSWD},
	{UCOT_SYSTEM, UCSOT_SYS_SOFTWARE, UCAT_SYS_UPGRADE},
	{UCOT_SYSTEM, UCSOT_SYS_SOFTWARE, UCAT_SYS_UPGRADE},
	{UCOT_SYSTEM, UCSOT_SYS_SOFTWARE, UCAT_SYS_RESET},
	{UCOT_IA, UCSOT_IA_PUBLIC, UCAT_IA_PUBLIC_WIFI_SET},
#ifdef HAS_LED
	{UCSOT_SYS_HARDWARE, UCATT_HARDWARE_LED},
	{UCSOT_SYS_HARDWARE, UCATT_HARDWARE_LEDCOLOR},
#endif

#ifdef HAS_TIMER
	{UCOT_IA, UCSOT_IA_PUBLIC, UCAT_IA_PUBLIC_PERIOD_TIMER},
#endif
	{UCOT_SYSTEM, UCSOT_SYS_VENDOR, UCAT_VENDOR_OEM_ID},
	{UCOT_SYSTEM, UCSOT_SYS_SERVER, UCAT_SERVER_CONNECT_TIME},
	{UCOT_SYSTEM, UCSOT_SYS_SERVER, UCAT_SERVER_DONAME},
	{UCOT_SYSTEM, UCSOT_SYS_SERVER, UCAT_SERVER_IP},
#ifdef LINKAGE_SUPPORT
	{UCOT_SYSTEM, UCSOT_SYS_SERVER, UCAT_SERVER_HOME_ID},
#endif

	{UCOT_SYSTEM, UCSOT_SYS_USER, UCAT_USER_PASSWD}
};


void ICACHE_FLASH_ATTR do_ucot_system(iw_event_info_t *info, iw_obj_t *uobj)
{
	//返回一些测试用，还有get/set判断与否．
	switch(uobj->sub_objct) {
		case UCSOT_SYS_SUPPORT:
			do_ucot_system_support(info, uobj);
			break;
		case UCSOT_SYS_HOSTNAME:
			do_ucot_system_hostname(info, uobj);
			break;
		case UCSOT_SYS_SOFTWARE:
			do_ucot_system_software(info, uobj);
			break;
		case UCSOT_SYS_UPGRADE:
			do_ucot_system_upgrade(info, uobj);
			break;
		case UCSOT_SYS_HARDWARE:
			do_ucot_system_hardware(info, uobj);
			break;
		case UCSOT_SYS_VENDOR:
			do_ucot_system_vendor(info, uobj);
			break;
		case UCSOT_SYS_SERVER:
			do_ucot_system_server(info, uobj);
			break;
		case UCSOT_SYS_USER:
			do_ucot_system_user(info, uobj);
			break;
		case UCSOT_SYS_LICENCE:
			do_ucot_system_licence(info, uobj);
			break;
		default:
			uc_do_set_reply((ucp_obj_t*)uobj, ERR_CMD_INVALID);
			break;
	}
}

static void ICACHE_FLASH_ATTR do_ucot_system_support(iw_event_info_t *info, iw_obj_t *uobj)
{
	int i;
	int len = 0;
	int index = 0;
	u_int8_t *buf_tmp = NULL;
	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	if (action != UCA_GET || uobj->attr != UCAT_SUPPORT_SUPPORT) {
		DEBUG_LINE();
		return;
	}
	len = OBJ_ARRAY_NUM * UDP_CTRL_TL_SIZE_NO_LEN;
	buf_tmp = iw_calloc(len, 1);

	if (buf_tmp) {
		for(i = 0; i < OBJ_ARRAY_NUM; i++) {
			uc_put_elemet2(buf_tmp, len, &index, obj_array[i].obj, obj_array[i].obj_sub, obj_array[i].addr);
		}

		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, UCOT_SYSTEM,
		              UCSOT_SYS_SUPPORT, UCAT_SUPPORT_SUPPORT, len, buf_tmp);
	}

	if (buf_tmp) {
		iw_free(buf_tmp);
	}
}

static void ICACHE_FLASH_ATTR do_ucot_system_hostname(iw_event_info_t *info, iw_obj_t *uobj)
{
	sys_t * sys = &sys_mu_config.sys;
	u_int16_t err = ERR_NONE;

	u_int8_t action = 0;
	u_int8_t *param = NULL;

	if(!info) {
		return;
	}
	action = info->action;
	param = (u_int8_t *)(info + 1);

	if (action == UCA_GET) {
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		              uobj->sub_objct, uobj->attr, os_strlen(sys->hostname) + 1, (u_int8_t *)sys->hostname);
	} else if (action == UCA_SET) {
		if (uobj->param_len <= sizeof(sys->hostname)) {
			os_memcpy((u_int8_t *)sys->hostname, param , uobj->param_len);
			sys->hostname[sizeof(sys->hostname) - 1] = 0;
			iw_request_event(EVENT_SYS_SAVE_CONFIG);
		} else {
			err = ERR_PARAM_INVALID;
		}
		uc_do_set_reply	((ucp_obj_t *)uobj, err);
	}
}

static void ICACHE_FLASH_ATTR do_ucot_system_version(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int32_t vertion;
	sys_t * sys = &sys_mu_config.sys;
	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	vertion = htonl(sys->soft_ver);

	if (action == UCA_GET) {
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		              uobj->sub_objct, uobj->attr, sizeof(vertion), (u_int8_t *)&vertion);
	}
}

static  void ICACHE_FLASH_ATTR do_ucot_system_svn(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int32_t svn = 0;

	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	svn = htonl(sys_mu_config.sys.soft_svn);

	if (action != UCA_GET)
		return;

	uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index,
	              uobj->objct, uobj->sub_objct, uobj->attr,
	              sizeof(svn), (u_int8_t*)&svn);
}

static  void ICACHE_FLASH_ATTR do_ucot_system_ssid(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int8_t action = 0;
	char ssid[32] = {0};

	os_memcpy(ssid, piw_priv->iw_sys.ssid, 32);

	if(!info) {
		return;
	}
	action = info->action;

	if (action != UCA_GET) {
		return;
	}

	uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index,
	              uobj->objct, uobj->sub_objct, uobj->attr,
	              32, (u_int8_t *)ssid);

}

static  void ICACHE_FLASH_ATTR do_ucot_system_passwd(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int8_t action = 0;
	char pw[64] = {0};
	
	os_memcpy(pw, piw_priv->iw_sys.passwd, 64);

	if(!info) {
		return;
	}
	action = info->action;

	if (action != UCA_GET) {
		return;
	}
	uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index,
	              uobj->objct, uobj->sub_objct, uobj->attr,
	              64, (u_int8_t *)pw);

}

static  void ICACHE_FLASH_ATTR do_ucot_system_devstatus(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int8_t status = 0;

	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	if (action != UCA_GET) {
		return;
	}

	status = (u_int8_t)ucd.client.status;

	uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index,
	              uobj->objct, uobj->sub_objct, uobj->attr,
	              sizeof(status), &status);
}

static  void ICACHE_FLASH_ATTR do_ucot_system_com_time(iw_event_info_t *info, iw_obj_t *uobj, bool is_date)
{
	const char *str = NULL;

	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	if (action != UCA_GET) {
		return;
	}

	if (is_date) {
		str = FwCreatedDate;
	} else {
		str = FwCreatedTime;
	}

	uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index,
	              uobj->objct, uobj->sub_objct, uobj->attr,
	              strlen(str), (u_int8_t *)str);
}

static  void ICACHE_FLASH_ATTR do_ucot_set_system_time(iw_event_info_t *info, iw_obj_t *uobj, bool is_date)
{
	uc_time_t *ptime = NULL;
	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	if (action == UCA_SET &&
	    uobj->param_len >= sizeof(*ptime)) {
		ptime = (uc_time_t *)(uobj + 1);
		ntp_from_ice(ptime);
	}
}

static  void ICACHE_FLASH_ATTR do_ucot_system_debuginfo(iw_event_info_t *info, iw_obj_t *uobj)
{
	int len = 0;
	u_int8_t *buff = NULL;
	u_int8_t action = 0;

	action = info->action;
	if ((action == UCA_GET) &&
	    (len = sys_get_debuginfo(&buff)) > 0) {
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index,
		              uobj->objct, uobj->sub_objct, uobj->attr, len, buff);
		iw_free(buff);
		return;
	}

}

static void ICACHE_FLASH_ATTR do_ucot_system_reboot(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int8_t on = 0;
	u_int16_t err = ERR_NONE;
	u_int8_t action = 0;
	u_int8_t *param = NULL;

	if(!info) {
		return;
	}
	action = info->action;
	param = (u_int8_t *)(info + 1);

	if (action == UCA_SET) {
		if (uobj->param_len >= sizeof(on)) {
			on = *param;
		} else {
			err = ERR_PARAM_INVALID;
		}

		if (on == 1) {
			iw_request_event(EVENT_SYS_REBOOT);
		}
		uc_do_set_reply	((ucp_obj_t*)uobj, err);
	}
}

static void ICACHE_FLASH_ATTR do_ucot_system_uptime(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int32_t uptime = get_up_sec();

	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;
	uptime = htonl(uptime - piw_priv->iw_sys.up_time);
	if (action == UCA_GET) {
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		              uobj->sub_objct, uobj->attr, sizeof(uptime), (u_int8_t *)&uptime);
	}
}
static void ICACHE_FLASH_ATTR do_ucot_system_time(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int32_t sys_time = get_sec();
	u_int8_t action = 0;

	if (!time_syn_ok()) {
		return;
	}

	if(!info) {
		return;
	}
	action = info->action;

	sys_time = htonl(sys_time);
	if (action == UCA_GET) {
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		              uobj->sub_objct, uobj->attr, sizeof(sys_time), (u_int8_t *)&sys_time);
	}
}

static void ICACHE_FLASH_ATTR do_ucot_system_ip(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int32_t ip = piw_priv->iw_sys.ip;
	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	ip = htonl(ip);
	if (action == UCA_GET) {
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		              uobj->sub_objct, uobj->attr, sizeof(ip), (u_int8_t *)&ip);
	}
	SYS_DEBUG("my ip is %u.%u.%u.%u\n", ((u_int8_t*)&piw_priv->iw_sys.ip)[0],
	          ((u_int8_t*)&piw_priv->iw_sys.ip)[1],
	          ((u_int8_t*)&piw_priv->iw_sys.ip)[2],
	          ((u_int8_t*)&piw_priv->iw_sys.ip)[3]);

}

static void do_ucot_system_reset(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int16_t err = ERR_NONE;
	u_int8_t action = 0;
	u_int8_t *param = NULL;

	if(!info) {
		return;
	}
	action = info->action;
	param = (u_int8_t *)(info + 1);


	if (action == UCA_SET) {
		if (*param == 1) {
			iw_sys_reset();
		} else {
			err = ERR_PARAM_INVALID;
		}
	}
	uc_do_set_reply	((ucp_obj_t *)uobj, err);
}

static void ICACHE_FLASH_ATTR do_ucot_system_software(iw_event_info_t *info, iw_obj_t *uobj)
{
	switch(uobj->attr) {
		case UCAT_SYS_VERSION:
			do_ucot_system_version(info, uobj);
			break;
		case UCAT_SYS_SVN:
			do_ucot_system_svn(info, uobj);
			break;
		case UCAT_SYS_SSID:
			do_ucot_system_ssid(info, uobj);
			break;
		case UCAT_SYS_PASSWD:
			do_ucot_system_passwd(info, uobj);
			break;
		case UCAT_SYS_DEVSTATUS:
			do_ucot_system_devstatus(info, uobj);
			break;
		case UCAT_SYS_COM_DATE:
			do_ucot_system_com_time(info, uobj, true);
			break;
		case UCAT_SYS_COM_TIME:
			do_ucot_system_com_time(info, uobj, false);
			break;
		case UCAT_SYS_SET_SYSTIME:
			do_ucot_set_system_time(info, uobj, false);
			break;
		case UCAT_SYS_DEBUGINFO:
			do_ucot_system_debuginfo(info, uobj);
			break;
		case UCAT_SYS_REBOOT:
			do_ucot_system_reboot(info, uobj);
			break;
		case UCAT_SYS_UPTIME:
			do_ucot_system_uptime(info, uobj);
			break;
		case UCAT_SYS_SYSTIME:
			do_ucot_system_time(info, uobj);
			break;
		case UCAT_SYS_IP:
			do_ucot_system_ip(info, uobj);
			break;
		case UCAT_SYS_UPGRADE:
			do_ucot_system_upgrade_image(info, uobj);
			break;
#ifdef HAS_STM_UPGRADE				
		case UCAT_SYS_STM_UPGRADE:
			do_ucot_system_stm_upgrade_image(info, uobj);
			break;
#endif		
		case UCAT_SYS_RESET:
			do_ucot_system_reset(info, uobj);
			break;
		default:
			uc_do_set_reply((ucp_obj_t*)uobj, ERR_CMD_INVALID);
			break;
	}

}

static void ICACHE_FLASH_ATTR do_ucot_system_upgrade(iw_event_info_t *info, iw_obj_t *uobj)
{
#if 1
	u_int32_t vertion;
	sys_t * sys = &sys_mu_config.sys;

	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	vertion = htonl(sys->upgrade_ver);

	if (action == UCA_GET) {
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		              uobj->sub_objct, uobj->attr, sizeof(vertion), (u_int8_t *)&vertion);
	}
#endif
}

static  void ICACHE_FLASH_ATTR do_ucot_system_hardware_version(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int32_t version;
	sys_t * sys = &sys_mu_config.sys;
	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	if (action == UCA_GET) {
		version = htonl(sys->hard_ver);
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		              uobj->sub_objct, uobj->attr, sizeof(version), (u_int8_t *)&version);
	}

}
static  void ICACHE_FLASH_ATTR do_ucot_system_hardware_all(iw_event_info_t *info, iw_obj_t *uobj)
{

	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	if (action == UCA_GET) {
		uobj->attr = UCAT_SYS_VERSION;
		do_ucot_system_hardware_version(info, uobj);
	}

}
static void ICACHE_FLASH_ATTR do_ucot_system_hardware(iw_event_info_t *info, iw_obj_t *uobj)
{
	switch(uobj->attr) {
		case ATTR_ALL:
			do_ucot_system_hardware_all(info, uobj);
			break;
		case UCAT_SYS_VERSION:
			do_ucot_system_hardware_version(info, uobj);
			break;
		default:
			break;
	}
}

static void ICACHE_FLASH_ATTR do_ucot_system_vendor(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	DEBUG_LINE();
	if (action == UCA_GET) {
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		              uobj->sub_objct, uobj->attr, strlen((char*)ucd.vendor) + 1, ucd.vendor);
	}
}

static void ICACHE_FLASH_ATTR do_ucot_system_server_conn_time(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int32_t time;

	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;
	if (action != UCA_GET) {
		return;
	}

	time = htonl(piw_priv->iw_sys.server_connect_time != 0 ? (get_sec() - piw_priv->iw_sys.server_connect_time) : 0);

	SYS_DEBUG("my time_connect timer %lu,now %lu\n",
	          piw_priv->iw_sys.server_connect_time,
	          get_sec());

	uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
	              uobj->sub_objct, uobj->attr, sizeof(time), (u_int8_t *)&time);
}

static void ICACHE_FLASH_ATTR do_ucot_system_server_domainname(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int16_t err = ERR_NONE;
	char *pconfig = NULL;
	u_int8_t action = 0;
	u_int8_t *param;
	
	if(!info) {
		return;
	}
	
	param = (u_int8_t *)(info + 1);
	param = param;

	action = info->action;

	if (action == UCA_GET) {
		SYS_ERROR("LINKAGE_SUPPORT get\n");
		pconfig = domain_get_name();
		if(NULL != pconfig){
			uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
									uobj->sub_objct, uobj->attr, os_strlen(pconfig), (u_int8_t *)pconfig);
		}else{
			uc_do_set_reply((ucp_obj_t *)uobj, ERR_SYSTEM);
		}
	}else if(action == UCA_SET){
#ifdef LINKAGE_SUPPORT
		SYS_ERROR("do_ucot_system_server_domainname set\n");
		err = domain_set_name((char *)param, uobj->param_len, DM_SET);
		// 重新解析连接服务器
		if (err == RS_OK){
			reset_server_session();
		}
	
		uc_do_set_reply((ucp_obj_t *)uobj, err);
#else
		err = ERR_PARAM_INVALID;
		uc_do_set_reply((ucp_obj_t *)uobj, err);
#endif
	}
	
}

static void ICACHE_FLASH_ATTR do_ucot_system_server_ip(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int32_t server_ip;
	u_int8_t action = 0;

	if(!info) {
		return;
	}
	action = info->action;

	if (action != UCA_GET) {
		return;
	}

	server_ip = htonl(piw_priv->iw_sys.dns);

	uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
	              uobj->sub_objct, uobj->attr, sizeof(server_ip), (u_int8_t *)&server_ip);

}

#ifdef LINKAGE_SUPPORT
static void ICACHE_FLASH_ATTR do_ucot_system_server_joinhome(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int32_t home_id = 0;
	config_t *config = NULL;
	u_int8_t action = info->action;
	u_int8_t *param = (u_int8_t *)(info + 1);

	switch( action ) {
		case UCA_GET:
			config = conf_user_get(HOME_ID);
			if(config && (config->len == sizeof(home_id))) {
				home_id = *(u_int32_t *)config->value;
			} else{
				SYS_ERROR("%s Get error!\n", __FUNCTION__);
			}
			
			SYS_ERROR("%s line%d home_id get= %d\n", __FUNCTION__, __LINE__, home_id);
			home_id = htonl(home_id);
			uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
			              uobj->sub_objct, uobj->attr, sizeof(home_id), (u_int8_t*)&home_id);
			break;
		case UCA_SET:
			home_id = ntohl(*(u_int32_t *)param);
			SYS_ERROR("%s line%d home_id = %d\n", __FUNCTION__, __LINE__, home_id);
			if( uobj->param_len != sizeof(home_id))
				uc_do_set_reply((ucp_obj_t *)uobj, ERR_PARAM_INVALID);
			else {
				conf_user_set(HOME_ID, (u_int8_t *)&home_id, uobj->param_len );

				uc_do_set_reply((ucp_obj_t *)uobj, ERR_NONE);
				SYS_ERROR("%s line%d home_id set= %d\n", __FUNCTION__, __LINE__, home_id);
			}

			break;
		default:
			break;
	}
}
#endif

static void ICACHE_FLASH_ATTR do_ucot_system_server(iw_event_info_t *info, iw_obj_t *uobj)
{
	switch(uobj->attr) {
		case UCAT_SERVER_CONNECT_TIME:
			do_ucot_system_server_conn_time(info, uobj);
			break;
		case UCAT_SERVER_DONAME:
			do_ucot_system_server_domainname(info, uobj);
			break;
		case UCAT_SERVER_IP:
			do_ucot_system_server_ip(info, uobj);
			break;
#ifdef LINKAGE_SUPPORT
		case UCAT_SERVER_HOME_ID:
			SYS_ERROR("-------------------------->>>UCAT_SERVER_HOME_ID");
			do_ucot_system_server_joinhome(info, uobj);
			break;
#endif
		default:
			uc_do_set_reply((ucp_obj_t*)uobj, ERR_CMD_INVALID);
			break;
	}
}

/**
	传入 ucdc_session_t 或者ucds_session_t 指针，返回类型
*/
static u_int8_t ICACHE_FLASH_ATTR ucd_session_type(void *s)
{
	if (s) {
		return *((u_int8_t*)s);
	} else {
		return 0;
	}
}

static void ICACHE_FLASH_ATTR do_ucot_system_pwd(iw_event_info_t *info, iw_obj_t *uobj)
{
	u_int16_t err = ERR_NONE;
	ucd_t *ucd = sys_mu_config.ucd;
	ucds_session_t *s = NULL;

	u_int8_t action = 0;
	u_int8_t *param = NULL;

	if(!info) {
		return;
	}
	action = info->action;
	param = (u_int8_t *)(info + 1);
	s = (ucds_session_t *)info->s;

	if (action == UCA_GET) {
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, uobj->objct,
		              uobj->sub_objct, uobj->attr, sizeof(ucd->passwd_md5), ucd->passwd_md5);
	} else if (action == UCA_SET) {
		if (uobj->param_len >= sizeof(ucd->passwd_md5)) {
			if(os_strncmp((char *)ucd->passwd_md5, (char *)param, sizeof(ucd->passwd_md5))){
				os_memcpy((u_int8_t*)ucd->passwd_md5, param, sizeof(ucd->passwd_md5));
				// TODO:这里要处理下，reset其它session
				if (ucd_session_type(s) == UCD_ST_CLIENT) {
					other_session_reset(s);
				} else {
					other_client_session_reset();
				}
				iw_request_event(EVENT_SYS_SAVE_CONFIG);				
				mem_dump("+++++++++++++++++set new paswdmd5", ucd->passwd_md5, sizeof(ucd->passwd_md5));
			}else{
				mem_dump("set new paswdmd1", ucd->passwd_md5, sizeof(ucd->passwd_md5));
				mem_dump("set new paswdmd2", param, sizeof(ucd->passwd_md5));
			}
		} else {
			err = ERR_PARAM_INVALID;
		}
		uc_do_set_reply	((ucp_obj_t *)uobj, err);
	}
}

static void ICACHE_FLASH_ATTR do_ucot_system_user(iw_event_info_t *info, iw_obj_t *uobj)
{
	switch(uobj->attr) {
		case UCAT_USER_PASSWD:
			do_ucot_system_pwd(info, uobj);
			break;
		default:
			uc_do_set_reply((ucp_obj_t*)uobj, ERR_CMD_INVALID);
			break;
	}
}

static void ICACHE_FLASH_ATTR do_ucot_system_licence(iw_event_info_t *info, iw_obj_t *uobj)
{
	switch(uobj->attr) {
		case UCAT_SYS_LICENCE_ACTIVE:
			do_ucot_system_active(info, uobj);
			break;
		default:
			uc_do_set_reply((ucp_obj_t*)uobj, ERR_CMD_INVALID);
			break;
	}
}


static int ICACHE_FLASH_ATTR uc_put_elemet2(u_int8_t *buf, int size, int *pos, u_int16_t object, u_int16_t sub_object, u_int16_t attri)
{
	u_int16_t *ptr;

	if (*pos + UDP_CTRL_TL_SIZE_NO_LEN > size) {
		DEBUG_LINE();
		SYS_ERROR("error len size=%d pos=%d\n", size, *pos);
		return -1;
	}

	ptr = (u_int16_t *)(buf + (*pos));
	*ptr = htons(object);
	ptr++;

	*ptr = htons(sub_object);
	ptr++;

	*ptr = htons(attri);
	ptr++;

	*pos = (*pos) + UDP_CTRL_TL_SIZE_NO_LEN;

	return 0;
}

/****************************调试信息获取*************************************/
static int ICACHE_FLASH_ATTR uc_put_tlv(u_int8_t *buf, int size, int *pos, u_int16_t type, u_int16_t len, u_int8_t *data)
{
	u_int8_t *ptr8;
	u_int16_t	temp;

	if (*pos + DBG_TLV_HD_LEN + len > size) {
		DEBUG_LINE();
		SYS_ERROR("error len size=%d pos=%d\n", size, *pos);
		return -1;
	}

	ptr8 = (u_int8_t *)(buf + (*pos));
	temp = htons(type);
	os_memcpy(ptr8, &temp, sizeof(type));
	ptr8 += sizeof(type);

	temp = htons(len);
	os_memcpy(ptr8, &temp, sizeof(len));
	ptr8 += sizeof(len);

	if (len > 0) {
		os_memcpy(ptr8, data, len);
	}

	*pos = (*pos) + DBG_TLV_HD_LEN + len;

	return 0;
}

static int ICACHE_FLASH_ATTR sys_get_debuginfo(u_int8_t **buff)
{
	int len = 0;
	u_int32_t temp = 0;
	u_int8_t *pbuf = NULL;
	sys_t *sys = &sys_mu_config.sys;
	char *ptemp = NULL;
	sint8_t rssi;

	pbuf = (u_int8_t *)iw_malloc(MAX_UC_PKT_SIZE);
	if (pbuf == NULL) {
		return 0;
	}

	//SVN号
	temp = htonl(sys->soft_svn);
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_SVN, sizeof(temp), (u_int8_t *)&temp);
	//连接服务器时间
	temp = htonl(piw_priv->iw_sys.server_connect_time != 0 ? (get_sec() - piw_priv->iw_sys.server_connect_time) : 0);
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_SERVER_CONNTIME, sizeof(temp), (u_int8_t *)&temp);

	SYS_DEBUG("my debug time_connect timer %lu,now %lu\n",
	          piw_priv->iw_sys.server_connect_time,
	          get_sec());

	// TODO: 连接服务器域名   一定要字节序对其，不然会死掉,
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_SERVER_DONAME, os_strlen(disp_server), (u_int8_t *)disp_server);
	//当前局域网手机接入数量
	temp = htonl(ucds_get_clients());
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_CLIENTS, sizeof(temp), (u_int8_t *)&temp);

	//设备连接
	temp = htonl(ucd.client.ip);
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_DEVSERVERIP, sizeof(temp), (u_int8_t *)&temp);


	//设备ip
	temp = htonl(ucd.client.ip);
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_DEVSERVERIP, sizeof(temp), (u_int8_t *)&temp);

	//设备uptime
	temp = htonl(get_up_sec());
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_UPTIME, sizeof(temp), (u_int8_t *)&temp);	

	*buff = pbuf;

	//连接服务器域名
    ptemp = domain_get_name();
	if (NULL != ptemp) {
		uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_SERVER_DONAME, os_strlen(ptemp), (u_int8_t *)ptemp);	
	}

   //单片机软件版本
	temp = htonl(sys->smt_soft_ver);
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_SMT_SOFT_VER, sizeof(temp), (u_int8_t *)&temp);

   //单片机硬件版本
	temp = htonl(sys->smt_hard_ver);
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_SMT_HARD_VER, sizeof(temp), (u_int8_t *)&temp);

	//连接WiFi(路由器)的时间 
	temp = get_sec() - piw_priv->iw_sys.wifi_connect_time;
	temp = htonl(temp);
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_TIME_CONN_WIFI, sizeof(temp), (u_int8_t *)&temp);


	//当前连接WiFi的信号强度 
	rssi = htonl(wifi_station_get_rssi());
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_WIFI_RSSI, sizeof(rssi), (u_int8_t *)&rssi);


	//当前连接WiFi的模式(phy_mode) 
	temp = htonl(wifi_get_phy_mode() - 1);
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_WIFI_PHY_MODE, sizeof(temp), (u_int8_t *)&temp);

	//无线SDK版本 
	temp = htonl(WIFI_SDK_VER);
	uc_put_tlv(pbuf, MAX_UC_PKT_SIZE, &len, DBG_TYPE_WIFI_SDK_VERSION, sizeof(temp), (u_int8_t *)&temp);

	*buff = pbuf;

	return len;
}

