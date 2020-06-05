
/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: mu中的类型定义
**  File:    mu_type.h
**  Author:  liubenlong
**  Date:    08/10/2014
**
**  Purpose:
**    Mu中的类型定义.
**************************************************************************/


#ifndef MU_TYPE_H
#define MU_TYPE_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */

#include "os_type.h"

/* Macro constant definitions. */
#define HOSTNAME_LEN		64


#define BIN_FMT8 "%02X%02X%02X%02X%02X%02X%02X%02X"
#define BIN_FMT16 BIN_FMT8""BIN_FMT8

#define BIN_SHOW8(addr) (addr)[0], (addr)[1], (addr)[2], (addr)[3], (addr)[4], (addr)[5], (addr)[6], (addr)[7]
#define BIN_SHOW16(addr) BIN_SHOW8(addr), BIN_SHOW8((addr+8))

enum {
	LED_SET_CLOSE = 0,
	LED_SET_OPEN = 1,
	LED_SET_SMT = 2
};

#pragma pack(push,1)

//led 控制
typedef struct led_s {
	u_int8_t enable;// 是否开启设备的LED灯显示模式，0表示一直灭,1表示开启,2表示智能led灯。默认是2
	u_int8_t on;// led的当前状态，0表示熄灭,1表示亮
}led_t;

//led 颜色
typedef struct led_color_s {
	u_int8_t color_of_acon;// 空调开机时候的颜色
	u_int8_t color_of_acoff;// 空调关机时候的颜色
	u_int16_t pad;	
}led_color_t;

#define 	chip_LED_OK   		0x01 //led控制 
#define 	chip_CURRENT_OK 	0x02//电流检测
#define 	chip_VOLTAGE_OK 	0x04//电压检测
#define 	chip_TEMP_OK 		0x08//温度检测
#define 	chip_ALLFUN_OK  (chip_LED_OK|chip_CURRENT_OK|chip_VOLTAGE_OK|chip_TEMP_OK)

typedef struct  stm32_chip_s{
	u_int16_t iswork;//能正常工作的模块
	u_int8_t ver;
	u_int8_t resev;
}stm32_chip_t;

typedef struct ac_ad_s{
	u_int32_t ad;
	u_int32_t k;
	u_int32_t b;
	//空载电流ad值
	u_int32_t noload_ad;
}ac_ad_t;

typedef struct sys_s {
	led_t led;	//保留，和前端交互复用这个结构
	led_color_t led_color;	
	u_int32_t time_connect_server;
	u_int32_t soft_ver;
	u_int32_t hard_ver;
	u_int32_t upgrade_ver;	
	char hostname[HOSTNAME_LEN];
	stm32_chip_t  stm32_chip;
	u_int32_t soft_svn;	/* 编译镜像的svn号*/

	//电流
	u_int32_t current;
	//电压
	u_int32_t voltage;
	//光度采样
	u_int32_t ac_light;
	u_int32_t firstday_light;
	u_int32_t nextday_light;
	bool ac_light_modify;	// 手动修改
	//电压ad值
	ac_ad_t ac_voltage;
	//电流ad值
	ac_ad_t ac_current;

	//单片机软件版本
	u_int32_t smt_soft_ver;
	//单片机硬件版本
	u_int32_t smt_hard_ver;
	
	//人体红外感应
	u_int32_t personir_ischange;//人体红外热能有改变，从单片机送上来的
}sys_t;

typedef struct location_s{
	u_int8_t is_east;
	u_int8_t is_north;
	u_int8_t latitude[16];
	u_int8_t longitude[16];
}location_t;

typedef struct mu_type_s {
	bool init_done;
	// WIFI是否连接成功
	u_int8_t is_apclient_connectok;
	// dhcp曾经获取到地址没。不能在DHCP模块里面调用，否则系统起不来，可能是烧录到静态分区的原因
	u_int8_t has_dhcp_ok;
#ifdef HAS_AC	
	ac_t ac;
	int smt_home;
	u_int8_t is_do_smtslp;
	location_t home_location; //家的坐标
#endif
	ucd_t *ucd;
	sys_t sys;
	//定时器链表
	struct stlc_list_head dev_timer;
}mu_type_t;


typedef struct version_s {
	u_int32_t soft_ver;
	u_int32_t upgrade_ver;
	u_int32_t soft_svn;	/* 编译镜像的svn号*/
} mu_version_t;

#pragma pack(pop)

/* Type definitions. */


/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */
/*
	功能:
		加载一下默认配置
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void init_mu_sys_config(void);

/*
	功能:
		保存USER_SYS_HOSTNAME， MD5
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void mu_sys_config_save(void);

/*
	功能:
		获取设备类型，扩展设备类型，svn，版本号， 硬件版本号，厂商
	输入参数:
		@pdev_info: 设备结构
	输出参数:
	返回值:
	说明:
*/
void  get_dev_info(dev_info_t *pdev_info);

extern mu_type_t sys_mu_config;

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* MU_TYPE_H */

