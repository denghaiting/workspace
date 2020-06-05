/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: lede������ļ�
**  File:    esp8266_lede.h
**  Author:  zhaogang
**  Date:    06/02/2015
**
**  Purpose:
**  lede������ļ�
**************************************************************************/


#ifndef ESP8266_LEDE_H
#define ESP8266_LEDE_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */
//�豸����
#define DEV_HOST_NAME			""

//֧��LEDE LED
#define HAS_LEDE_LED
#define HAS_TIMER_EXT
#define TIMER_EXT_DATA_LEN	8

#define HAS_STM_UPGRADE
//��ʱ������
//#define HAS_TIMER


//֧�ֶྵ��ѡ��
//#define HAS_BOOTLOAD

//Ӳ��֧�ְ汾��Ϣ
//#define HAS_HARD_VERSION

// ֧��AP���
#define HAS_UAM

//ʹ�ô��ڲ�������
//#define HAS_UART_PT


/* Type definitions. */


/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif