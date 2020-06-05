/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: iw_pt_uart.h
**  File:    iw_pt_uart.h
**  Author:  xushengjian
**  Date:    06/09/2015
**
**  Purpose:
**
**************************************************************************/


#ifndef IW_PT_UART
#define IW_PT_UART


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */

/* Macro constant definitions. */
#define	MAX_UART_IN_BD	3
#define UART_BUFF_MAX_SIZE 	512

/* Type definitions. */
#pragma pack(push,1)

typedef struct {
	u_int8_t rid;
	u_int8_t cmd;
	u_int16_t param_len;
	u_int8_t ret;
	u_int8_t checksum;
} pt_uart_hdr_t;

#pragma pack(pop)

typedef struct {
	u_int8_t has_data;
	u_int8_t pad;
	// ��������
	u_int16_t param_len;
	pt_uart_hdr_t head;
	u_int8_t param[UART_BUFF_MAX_SIZE];
} pt_uart_bd_t;

/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */

/*
	����:
		�������wifi��
	�������:
	�������:
	����ֵ:
		true: ��
		����: ����
	˵��:
		���жϺ������ã���Ҫ��פ���ڴ�
		��������Ǵ��ڲ���ı�־��ͨ�����ڷ���ssid��pwd
*/
bool  is_test_ssid(void);

/*
	����:
		��������ģ���ʼ��
	�������:
	�������:
	����ֵ:
	˵��:
*/
void  iw_uart_pretest_init(void);
extern void ICACHE_FLASH_ATTR delay_10ms(uint8_t count);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* IW_PT_UART_H */
