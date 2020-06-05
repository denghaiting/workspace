/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: FOTA
**  File:    upgrade.h
**  Author:  xushengjian
**  Date:    07/20/2015
**
**  Purpose:
**    Firmware upgrade over the air.
**************************************************************************/


#ifndef UPGRADE_H
#define UPGRADE_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */


/* Type definitions. */


/* External function declarations. */
/*
	����:
		���������������
	�������:
		@info: �����Ϣ�ṹ
		@uobj: �������ԣ� ���ݳ���
	�������:
	����ֵ:
	˵��:
*/
void do_ucot_system_upgrade_image(iw_event_info_t *info, iw_obj_t *uobj);
/*
	����:
		ϵͳ����ģ���ʼ��
	�������:
	�������:
	����ֵ:
	˵��:
*/
void sys_upgrade_init(void);


/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* UPGRADE_H */

