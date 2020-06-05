/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 用户配置文件
**  File:    user_config.h
**  Author:  liubenlong
**  Date:    07/26/2014
**
**  Purpose:
**    用户配置文件.
**************************************************************************/


#ifndef IW_USER_CONFIG_H
#define IW_USER_CONFIG_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "iw_flash.h"
#include "iw_func_support.h"

/* Macro constant definitions. */


/* wifi分区的保存值 */
enum {
	WIFI_SSID_INDEX 		= 1,		//wifi ssid 
	WIFI_PASSWD_INDEX 		= 2,		//wifi passwd
	WIFI_AUTHMODE_INDEX 	= 3,		//wifi authmode
	WIFI_ENC_INDEX 			= 4,		//wifi encype
	WIFI_MAX_ID
};

/* 用户配置信息和设备一次性保存信息 */
enum {
	USER_VENDOR_INDEX 		= 1,		//厂商id
	USER_CLONE_INDEX 		= 2,		//是否是克隆机
	USER_SYS_HOSTNAME 		= 3, 		//设备昵称．
	USER_PASSWD_MD5 		= 4,		//密码md5值保存．
	USER_TIMEZONE 			= 5, 		//时区配置
	USER_AC_TIMER			= 6,			//空调定时器
	USER_IA_FLAG 			= 7,		//ia红外编码下载成功标志．０表示未配，１表示成功
	USER_HARD_LED_INDEX 	= 8, 		//led配置
	USER_AC_SMART_ON_INDEX 	= 9,  		//智能开
	USER_AC_SMART_OFF_INDEX = 10, 		//智能关
	USER_AC_SMART_SLEEP_INDEX = 11,		//智能睡眠
	USER_HARD_LEDCOLOR_INDEX 	= 12, 	//led color配置
	USER_IA_DELTA			= 13,		// 匹配相似度的DELTA值
	USER_IA_LEVELT_COUNT2 = 14,			// 可能的短编码电平个数
	#ifdef LINKAGE_SUPPORT
	SERVER_DOMAIN_NAME		=15,			//服务器域名
	HOME_ID					=16,			//家庭ID
	#endif
	SET_ICE_DOMAIN = 17,					//设置的服务器域名(联动功能)
	USER_MAX_ID
};

/* 设备的状态信息保存 */
enum {
	DEV_STA_ELE_TIME_INDEX 		= 1, 	//三种电类型的时间段－峰谷平
	DEV_STA_ELE_PRICE_INDEX 	= 2,	//三种电类型的价钱
	DEV_STA_ELE_MONTH_INDEX 	= 3,	//三种电的电量 ???
	DEV_AC_WORK_INDEX 			= 4,	//空调工作
	DEV_STAT_LAST_YEAR 			= 5,	//电量保存记录的当前年份
	DEV_STAT_LAST_MONTH 		= 6,	//电量保存记录的当前月份，用来跨月份或跨年处理的
	DEV_HLOCATION 				= 7,	//家的坐标配置
	DEV_LIGHT_SAMPLE 			= 8,     //睡眠模式光照强度采样
	DEV_TOTAL_ELE				= 9,	// 总电量
	DEV_PHASE_ELE				= 10,	//阶段电量
	DEV_MONTH_TYPE				=11,	//月电量
	DEV_DETECT_CUR_PARAM		=12,    //空调电流检测的阀值
	DEV_LEFT_HEAT_AREA_CONFIG	= 13,	// 彩虹电热毯左侧加热区域配置
	DEV_RIGHT_HEAT_AREA_CONFIG	= 14,	// 彩虹电热毯右侧加热区域配置
	DEV_EB_WORK					=15,	//e宝开关
	DEV_ONEDAY_ELE				=16,	//当天的电量，分阶保存
	DEV_PUSH2SEV_HISTORY_TIME	=17,	//上报服务器间隔时间
	DEV_AUTO_TEMPAJUST			=18,	//自动温度调节参数
	DEV_OPENDEV_CTCHECK			=19,	//开机电流检测
	DEV_AC_CURRENT_DEF			=20,	//出厂额定电流值
	DEV_AC_TEMP_RANGE				=21,	//温控阀值
	DEV_PERSONIR_CTRL				=22,	//人体红外探测
	DEV_POWER_EACHDAY           = 23,		// 每日用电量	
	DEV_CONFIG_YL				=24,	//亿林温控器配置	
	DEV_EXCEPTION				= 25,		// 最近30条异常数据	DEV_MAX_ID
	DEV_TOTAL_ELE_2				= 26, //为了兼容电量64位保存的问题 = 总电量
	DEV_PHASE_ELE_2				= 27,	//阶段电量	
	DEV_MONTH_TYPE_2			= 28,	//月电量	
	DEV_POWER_EACHDAY1		= 29,	//
	DEV_POWER_EACHDAY2		= 30,	// 
	DEV_POWER_EACHDAY3		= 31,	// 
#ifdef LINKAGE_SUPPORT
	DEV_TRAP_REGISTER			= 32,//设备告警事件注册
#endif	
	DEV_LEDE_COLOR_CONFIG = 33,
	DEV_SERVER_DONAME_INDEX = 34,	
	DEV_POWERONOFF_TIMES = 35,
	DEV_SERVER_PORT_INDEX  = 36, //设备连接服务器的最优端口下标,这个废弃掉
	DEV_SERVER_YSD_STATE	 = 37,
	DEV_MAX_ID,
};


//恢复出厂不清楚的数据块
enum {
	FACTORY_RESET_COUNT = 1,	//恢复出厂的次数
	FACTORY_ERASE_SIGN = 2,	
	FIX_MAX_ID
};

//证书
#define MU_LICENCE_LEN			48
//厂商id
#define MU_VENDOR_LEN			16
//是否是克隆机
#define MU_CLONE_LEN			1
//是否激活
#define MU_ACTIVE_LEN			1

//wifi ssid 
#define MU_WIFI_SSID_LEN		32
//wifi passwd
#define MU_WIFI_PASSWD_LEN		64
//wifi authmode
#define MU_WIFI_AUTHMODE_LEN	1
//wifi encype
#define MU_WIFI_ENC_LEN			1
//user passwd
#define MU_USER_PASSWD_LEN		16
//dev name
#define MU_DEV_NAME_LEN			64
//bind phone uuid
#define MU_BIND_PHONE_UUID_LEN		90
//dev uuid
#define MU_DEV_UUID_LEN			16
//led配置
#define MU_DEV_LED_LEN			2


/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */
/* Wi-Fi配置块接口函数 */
/*
	功能:
		WIFI数据初始化
	输入参数:
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_wifi_init(void);
/*
	功能:
		复位wifi数据
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void conf_wifi_reset(void);
/*
	功能:
		通过id得到wifi配置
	输入参数:
		@id: 数据id
	输出参数:
		@info: 保存系统信息
	返回值:
		config_t*: 成功
		NULL: 失败
	说明:
*/
config_t* conf_wifi_get( u_int16_t id);

/*
	功能:
		通过id把数据存放到wifi数据块
	输入参数:
		@id: 数据id
		@buff: 数据
		@len: 数据长度
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_wifi_set(u_int16_t id, UINT8 *buff, u_int16_t len);
/*
	功能:
		把wifi数据快中的数据写进flash中
	输入参数:
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_wifi_commit(void);


/*
	功能:
		user数据初始化
	输入参数:
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_user_init(void);
/*
	功能:
		复位user数据
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void conf_user_reset(void);
/*
	功能:
		通过id得到user配置
	输入参数:
		@id: 数据id
	输出参数:
		@info: 保存系统信息
	返回值:
		config_t*: 成功
		NULL: 失败
	说明:
*/
config_t *conf_user_get(u_int16_t id);
/*
	功能:
		通过id把数据存放到user数据块
	输入参数:
		@id: 数据id
		@buff: 数据
		@len: 数据长度
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_user_set(u_int16_t id, UINT8*buff, u_int16_t len);
/*
	功能:
		把用户数据快中的数据写进flash中
	输入参数:
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_user_commit(void);


/*
	功能:
		dev数据初始化
	输入参数:
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_dev_init(void);
/*
	功能:
		复位dev数据
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void conf_dev_reset(void);
/*
	功能:
		通过id得到dev配置
	输入参数:
		@id: 数据id
	输出参数:
		@info: 保存系统信息
	返回值:
		config_t*: 成功
		NULL: 失败
	说明:
*/
config_t *conf_dev_get(u_int16_t id);
/*
	功能:
		通过id把数据存放到dev数据块
	输入参数:
		@id: 数据id
		@buff: 数据
		@len: 数据长度
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_dev_set(u_int16_t id, UINT8*buff, u_int16_t len);
/*
	功能:
		把设备数据快中的数据写进flash中
	输入参数:
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_dev_commit(void);

/*
	功能:
		固定数据初始化
	输入参数:
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_fix_init(void);
/*
	功能:
		复位fix数据
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void conf_fix_reset(void);
/*
	功能:
		通过id得到fix配置
	输入参数:
		@id: 数据id
	输出参数:
		@info: 保存系统信息
	返回值:
		config_t*: 成功
		NULL: 失败
	说明:
*/
config_t* conf_fix_get( u_int16_t id);
/*
	功能:
		通过id把数据存放到fix数据块
	输入参数:
		@id: 数据id
		@buff: 数据
		@len: 数据长度
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_fix_set(u_int16_t id, UINT8 *buff, u_int16_t len);
/*
	功能:
		把永久保存数据快中的数据写进flash中
	输入参数:
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS conf_fix_commit(void);


/*回复出厂的接口*/
/*
	功能:
		复位dev，user， wifi数据块中的数据
	输入参数:
		@info: 存放系统信息的内存块
	输出参数:
		@info: 保存系统信息
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
void config_reset(void);

/*
	功能:
		保存设备用户数据块中的数据到flash中
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void config_save(void);

/*
	功能:
		复位dev，user， wifi数据块中的数据, 但是錼eset_count不加一
	输入参数:
		@info: 存放系统信息的内存块
	输出参数:
		@info: 保存系统信息
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
void config_reset_two(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* USER_CONFIG_H */

