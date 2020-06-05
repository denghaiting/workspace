/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: esp8266.h
**  File:    iw_ntp_time_syn.h
**  Author:  fangfuhan
**  Date:    03/23/2016
**
**  Purpose:
**    Ntpʱ��ͬ��.
**************************************************************************/


#ifndef IW_NTP_TIME_SYN_H
#define IW_NTP_TIME_SYN_H	


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "iw_api.h"
#include "udp_ctrl.h"
/* Macro constant definitions. */


/* Type definitions. */
typedef rs_t (* syn_time_ok_callback_t)(u_int8_t type);

bool iw_ntp_get_init_state();
/* External function declarations. */
/*
	����:
		ʱ��ͬ����ʼ��
	�������:
	�������:
	����ֵ:
	˵��:
*/
void iw_ntp_time_syn_init();
/*
	����:
		���ֻ���ͬ��ʱ�䣬 ��������ͬ���ˣ��ֻ��Ͳ���ͬ����
	�������:
		@t: ʱ����Ϣ
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
		t->year �����������ֽ�
*/
RS ntp_from_phone(uc_time_t *t);
/*
	����:
		�ӷ�������ͬ��ʱ�䣬 ��������ͬ���ˣ��ֻ��Ͳ���ͬ����
	�������:
	�������:
		@t: ʱ����Ϣ
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
		t->year �����������ֽ�
*/
RS ntp_from_ice(uc_time_t *t);
/*
*	����:�ж�ʱ���Ƿ�ͬ�����
*	����ֵ:true ͬ�����
*		 false û��ͬ�����
*/
bool time_syn_ok(void);
/*
	����:
		ʱ��ͬ�����ʱ��Ҫ�ص��ĺ���ע��
	�������:
		@callback: �ص�
		@name: ����
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
rs_t syn_time_event_register(syn_time_ok_callback_t callback, u_int8_t *name);
/* Macro API definitions. */


/* Global variable declarations. */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* IW_NTP_TIME_SYN_H */

