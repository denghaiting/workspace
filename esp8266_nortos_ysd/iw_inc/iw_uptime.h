#ifndef __IWUOS_TIMER_HEADER__
#define __IWUOS_TIMER_HEADER__

#include "os_type.h"

//��ʱ�� 1��
#define TIME_PER_SECOND (1000)
//��ʱ�� 1����
#define TIME_PER_MSECOND (1)

#define TIME_N_SECOND(n) ((n)*(TIME_PER_SECOND))

#define TIME_N_MSECOND(n) (n)

#define TIME_N_MINUTE(n) (60*TIME_N_SECOND((n)))

#define	TIME_PU_MSG TIME_N_MINUTE(5)
#define	TIME_LOGIN_TIMEOUT	TIME_N_SECOND(30)
#define TIME_DNS_TIMEOUT	(3) // DNS������ʱ
#define	TIME_DNS_FORCE	(60*30)	// ǿ�����½���

#define DNS_MAX 6

#define NTP_PCK_LEN 48
#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6

/*
	����:
		�õ��豸����ʱ��
	�������:
	�������:
	����ֵ:
		u_int32_t: UTCʱ�� ��λ��
	˵��:
*/
u_int32_t get_up_sec();

/*
	����:
		�õ��豸����ʱ��
	�������:
	�������:
	����ֵ:
		u_int32_t: UTCʱ�� ��λ����
	˵��:
*/
u_int64_t get_up_msec();

/*
	����:
		�õ��豸����ʱ���ʼ��
	�������:
	�������:
	����ֵ:
	˵��:
*/
void iw_uptime_init(void);
#endif
