#ifndef __IWUOS_TIMER_HEADER__
#define __IWUOS_TIMER_HEADER__

#include "os_type.h"

//定时器 1秒
#define TIME_PER_SECOND (1000)
//定时器 1毫秒
#define TIME_PER_MSECOND (1)

#define TIME_N_SECOND(n) ((n)*(TIME_PER_SECOND))

#define TIME_N_MSECOND(n) (n)

#define TIME_N_MINUTE(n) (60*TIME_N_SECOND((n)))

#define	TIME_PU_MSG TIME_N_MINUTE(5)
#define	TIME_LOGIN_TIMEOUT	TIME_N_SECOND(30)
#define TIME_DNS_TIMEOUT	(3) // DNS解析超时
#define	TIME_DNS_FORCE	(60*30)	// 强制重新解析

#define DNS_MAX 6

#define NTP_PCK_LEN 48
#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6

/*
	功能:
		得到设备运行时间
	输入参数:
	输出参数:
	返回值:
		u_int32_t: UTC时间 单位秒
	说明:
*/
u_int32_t get_up_sec();

/*
	功能:
		得到设备运行时间
	输入参数:
	输出参数:
	返回值:
		u_int32_t: UTC时间 单位毫秒
	说明:
*/
u_int64_t get_up_msec();

/*
	功能:
		得到设备运行时间初始化
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void iw_uptime_init(void);
#endif
