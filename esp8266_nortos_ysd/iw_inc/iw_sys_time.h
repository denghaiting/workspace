/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: ħ��
**  File:    iw_sys_time.h
**  Author:  fangfuhan
**  Date:    04/11/2016
**
**  Purpose:
**    ά������ʱ��.
**************************************************************************/


#ifndef IW_SYS_TIME_H
#define IW_SYS_TIME_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "c_types.h"
#include "udp_ctrl.h"
#include "iw_api.h"
#include "iw_priv.h"

/* Macro constant definitions. */
#define TIMETONE_MAX (12)
#define TIMETONE_MIN (-12)

//ͬ��ʱ������ �������ȼ���
enum {
	NONE_TIME,
	PHONE_TIME,
	NTP_TIME
};

enum {
	ACTION_NONE,
	ACTION_INSERT,
	ACTION_MODE,
	ACTION_DEL
};

/* Type definitions. */

/* External function declarations. */
/*
	����:
		�õ����ص�ʱ���
	�������:
	�������:
	����ֵ:
		u_int8_t: ʱ���
	˵��:
*/
u_int8_t get_local_timezone();
/*
	����:
		���õ��ص�ʱ��꣬���浽flash��
	�������:
		@timezone: ʱ���
	�������:
	����ֵ:
	˵��:
*/
void set_local_timezone(u_int8_t timezone);

/*
	����:
  		�õ�ʱ���ͬ��״̬ NONE_TIME,PHONE_TIME,NTP_TIME
	�������:
	�������:
	����ֵ:
		u_int8_t: �ɹ�
		����: ʧ��
	˵��:
*/
u_int8_t get_syn_type();

/*
	����:
		�豸�豸��ͬ��ʱ�䣬�����豸�Ǵ�����ͬ��������
	�������:
		@time: ʱ��
		@type: PHONE_TIME,NTP_TIME
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
u_int8_t set_sec(u_int32_t time, u_int8_t type);
/*
	����:
		�豸��ʱ��ͬ��ʱ�䣬�����豸�Ǵ�����ͬ��������
	�������:
		@time: ʱ��
		@type: PHONE_TIME,NTP_TIME
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
rs_t set_sec_with_timetone(u_int32_t time, u_int8_t type, int timetome);

/*
	����:
		�õ�UTCʱ��
	�������:
	�������:
	����ֵ:
		u_int32_t: UTCʱ�� ��λ��
	˵��:
*/

u_int32_t get_sec(void);

/*
	����:
		����ʱ��ṹ��ʼ��
	�������:
	�������:
	����ֵ:
	˵��:
*/

void iw_local_time_init(void);

/*
	����:
		�ڲ�����ʱ��������������ʱ����
	�������:
		@time: ����
		@tm: ת����������ʱ����
	�������:
	����ֵ:
	˵��:
*/
void localtimes(time_t time, struct tm *tm);

/*
	����:
		�ڲ���ʱ��������������ʱ����
	�������:
		@time: ����
		@tm: ת����������ʱ����
	�������:
	����ֵ:
	˵��:
*/
void localtimes_withtone(time_t time, struct tm *tm);


//ʱ��ת��
/*
	����:
		utcСʱת��Ϊ����ʱ��Сʱ
	�������:
		@hour: utcСʱ
		@zone: ʱ���
	�������:
	����ֵ:
		int : ����ʱ�� ��λСʱ
	˵��:
*/
int hour_utc_2_local(int hour,int zone);

/*
	����:
		����ʱ��Сʱת��ΪutcСʱ
	�������:
		@hour: ����ʱ��Сʱ
		@zone: ʱ���
	�������:
	����ֵ:
		int : utcСʱ ��λСʱ
	˵��:
*/
int hour_local_2_utc(int hour,int zone);
/*
	����:
		utcʱ��ת���ɵ���ʱ�������
	�������:
		@hour: utcСʱ
		@min: utc����
		@sec: utc��
	�������:
	����ֵ:
		u_int32_t :����ʱ�������
	˵��:
*/
u_int32_t get_local_time_sec(u_int8_t hour, u_int8_t min, u_int8_t sec);
/*
	����:
		utcʱ��ת����utcʱ�������
	�������:
		@hour: utcСʱ
		@min: utc����
		@sec: utc��
	�������:
	����ֵ:
		u_int32_t :utcʱ�������
	˵��:
*/
u_int32_t get_utc_time_sec(u_int8_t hour, u_int8_t min, u_int8_t sec);

/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* IW_SYS_TIME_H */

