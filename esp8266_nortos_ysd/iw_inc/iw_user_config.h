/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: �û������ļ�
**  File:    user_config.h
**  Author:  liubenlong
**  Date:    07/26/2014
**
**  Purpose:
**    �û������ļ�.
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


/* wifi�����ı���ֵ */
enum {
	WIFI_SSID_INDEX 		= 1,		//wifi ssid 
	WIFI_PASSWD_INDEX 		= 2,		//wifi passwd
	WIFI_AUTHMODE_INDEX 	= 3,		//wifi authmode
	WIFI_ENC_INDEX 			= 4,		//wifi encype
	WIFI_MAX_ID
};

/* �û�������Ϣ���豸һ���Ա�����Ϣ */
enum {
	USER_VENDOR_INDEX 		= 1,		//����id
	USER_CLONE_INDEX 		= 2,		//�Ƿ��ǿ�¡��
	USER_SYS_HOSTNAME 		= 3, 		//�豸�ǳƣ�
	USER_PASSWD_MD5 		= 4,		//����md5ֵ���森
	USER_TIMEZONE 			= 5, 		//ʱ������
	USER_AC_TIMER			= 6,			//�յ���ʱ��
	USER_IA_FLAG 			= 7,		//ia����������سɹ���־������ʾδ�䣬����ʾ�ɹ�
	USER_HARD_LED_INDEX 	= 8, 		//led����
	USER_AC_SMART_ON_INDEX 	= 9,  		//���ܿ�
	USER_AC_SMART_OFF_INDEX = 10, 		//���ܹ�
	USER_AC_SMART_SLEEP_INDEX = 11,		//����˯��
	USER_HARD_LEDCOLOR_INDEX 	= 12, 	//led color����
	USER_IA_DELTA			= 13,		// ƥ�����ƶȵ�DELTAֵ
	USER_IA_LEVELT_COUNT2 = 14,			// ���ܵĶ̱����ƽ����
	#ifdef LINKAGE_SUPPORT
	SERVER_DOMAIN_NAME		=15,			//����������
	HOME_ID					=16,			//��ͥID
	#endif
	SET_ICE_DOMAIN = 17,					//���õķ���������(��������)
	USER_MAX_ID
};

/* �豸��״̬��Ϣ���� */
enum {
	DEV_STA_ELE_TIME_INDEX 		= 1, 	//���ֵ����͵�ʱ��Σ����ƽ
	DEV_STA_ELE_PRICE_INDEX 	= 2,	//���ֵ����͵ļ�Ǯ
	DEV_STA_ELE_MONTH_INDEX 	= 3,	//���ֵ�ĵ��� ???
	DEV_AC_WORK_INDEX 			= 4,	//�յ�����
	DEV_STAT_LAST_YEAR 			= 5,	//���������¼�ĵ�ǰ���
	DEV_STAT_LAST_MONTH 		= 6,	//���������¼�ĵ�ǰ�·ݣ��������·ݻ���괦����
	DEV_HLOCATION 				= 7,	//�ҵ���������
	DEV_LIGHT_SAMPLE 			= 8,     //˯��ģʽ����ǿ�Ȳ���
	DEV_TOTAL_ELE				= 9,	// �ܵ���
	DEV_PHASE_ELE				= 10,	//�׶ε���
	DEV_MONTH_TYPE				=11,	//�µ���
	DEV_DETECT_CUR_PARAM		=12,    //�յ��������ķ�ֵ
	DEV_LEFT_HEAT_AREA_CONFIG	= 13,	// �ʺ����̺��������������
	DEV_RIGHT_HEAT_AREA_CONFIG	= 14,	// �ʺ����̺�Ҳ������������
	DEV_EB_WORK					=15,	//e������
	DEV_ONEDAY_ELE				=16,	//����ĵ������ֽױ���
	DEV_PUSH2SEV_HISTORY_TIME	=17,	//�ϱ����������ʱ��
	DEV_AUTO_TEMPAJUST			=18,	//�Զ��¶ȵ��ڲ���
	DEV_OPENDEV_CTCHECK			=19,	//�����������
	DEV_AC_CURRENT_DEF			=20,	//���������ֵ
	DEV_AC_TEMP_RANGE				=21,	//�¿ط�ֵ
	DEV_PERSONIR_CTRL				=22,	//�������̽��
	DEV_POWER_EACHDAY           = 23,		// ÿ���õ���	
	DEV_CONFIG_YL				=24,	//�����¿�������	
	DEV_EXCEPTION				= 25,		// ���30���쳣����	DEV_MAX_ID
	DEV_TOTAL_ELE_2				= 26, //Ϊ�˼��ݵ���64λ��������� = �ܵ���
	DEV_PHASE_ELE_2				= 27,	//�׶ε���	
	DEV_MONTH_TYPE_2			= 28,	//�µ���	
	DEV_POWER_EACHDAY1		= 29,	//
	DEV_POWER_EACHDAY2		= 30,	// 
	DEV_POWER_EACHDAY3		= 31,	// 
#ifdef LINKAGE_SUPPORT
	DEV_TRAP_REGISTER			= 32,//�豸�澯�¼�ע��
#endif	
	DEV_LEDE_COLOR_CONFIG = 33,
	DEV_SERVER_DONAME_INDEX = 34,	
	DEV_POWERONOFF_TIMES = 35,
	DEV_SERVER_PORT_INDEX  = 36, //�豸���ӷ����������Ŷ˿��±�,���������
	DEV_SERVER_YSD_STATE	 = 37,
	DEV_MAX_ID,
};


//�ָ���������������ݿ�
enum {
	FACTORY_RESET_COUNT = 1,	//�ָ������Ĵ���
	FACTORY_ERASE_SIGN = 2,	
	FIX_MAX_ID
};

//֤��
#define MU_LICENCE_LEN			48
//����id
#define MU_VENDOR_LEN			16
//�Ƿ��ǿ�¡��
#define MU_CLONE_LEN			1
//�Ƿ񼤻�
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
//led����
#define MU_DEV_LED_LEN			2


/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */
/* Wi-Fi���ÿ�ӿں��� */
/*
	����:
		WIFI���ݳ�ʼ��
	�������:
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_wifi_init(void);
/*
	����:
		��λwifi����
	�������:
	�������:
	����ֵ:
	˵��:
*/
void conf_wifi_reset(void);
/*
	����:
		ͨ��id�õ�wifi����
	�������:
		@id: ����id
	�������:
		@info: ����ϵͳ��Ϣ
	����ֵ:
		config_t*: �ɹ�
		NULL: ʧ��
	˵��:
*/
config_t* conf_wifi_get( u_int16_t id);

/*
	����:
		ͨ��id�����ݴ�ŵ�wifi���ݿ�
	�������:
		@id: ����id
		@buff: ����
		@len: ���ݳ���
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_wifi_set(u_int16_t id, UINT8 *buff, u_int16_t len);
/*
	����:
		��wifi���ݿ��е�����д��flash��
	�������:
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_wifi_commit(void);


/*
	����:
		user���ݳ�ʼ��
	�������:
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_user_init(void);
/*
	����:
		��λuser����
	�������:
	�������:
	����ֵ:
	˵��:
*/
void conf_user_reset(void);
/*
	����:
		ͨ��id�õ�user����
	�������:
		@id: ����id
	�������:
		@info: ����ϵͳ��Ϣ
	����ֵ:
		config_t*: �ɹ�
		NULL: ʧ��
	˵��:
*/
config_t *conf_user_get(u_int16_t id);
/*
	����:
		ͨ��id�����ݴ�ŵ�user���ݿ�
	�������:
		@id: ����id
		@buff: ����
		@len: ���ݳ���
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_user_set(u_int16_t id, UINT8*buff, u_int16_t len);
/*
	����:
		���û����ݿ��е�����д��flash��
	�������:
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_user_commit(void);


/*
	����:
		dev���ݳ�ʼ��
	�������:
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_dev_init(void);
/*
	����:
		��λdev����
	�������:
	�������:
	����ֵ:
	˵��:
*/
void conf_dev_reset(void);
/*
	����:
		ͨ��id�õ�dev����
	�������:
		@id: ����id
	�������:
		@info: ����ϵͳ��Ϣ
	����ֵ:
		config_t*: �ɹ�
		NULL: ʧ��
	˵��:
*/
config_t *conf_dev_get(u_int16_t id);
/*
	����:
		ͨ��id�����ݴ�ŵ�dev���ݿ�
	�������:
		@id: ����id
		@buff: ����
		@len: ���ݳ���
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_dev_set(u_int16_t id, UINT8*buff, u_int16_t len);
/*
	����:
		���豸���ݿ��е�����д��flash��
	�������:
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_dev_commit(void);

/*
	����:
		�̶����ݳ�ʼ��
	�������:
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_fix_init(void);
/*
	����:
		��λfix����
	�������:
	�������:
	����ֵ:
	˵��:
*/
void conf_fix_reset(void);
/*
	����:
		ͨ��id�õ�fix����
	�������:
		@id: ����id
	�������:
		@info: ����ϵͳ��Ϣ
	����ֵ:
		config_t*: �ɹ�
		NULL: ʧ��
	˵��:
*/
config_t* conf_fix_get( u_int16_t id);
/*
	����:
		ͨ��id�����ݴ�ŵ�fix���ݿ�
	�������:
		@id: ����id
		@buff: ����
		@len: ���ݳ���
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_fix_set(u_int16_t id, UINT8 *buff, u_int16_t len);
/*
	����:
		�����ñ������ݿ��е�����д��flash��
	�������:
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS conf_fix_commit(void);


/*�ظ������Ľӿ�*/
/*
	����:
		��λdev��user�� wifi���ݿ��е�����
	�������:
		@info: ���ϵͳ��Ϣ���ڴ��
	�������:
		@info: ����ϵͳ��Ϣ
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
void config_reset(void);

/*
	����:
		�����豸�û����ݿ��е����ݵ�flash��
	�������:
	�������:
	����ֵ:
	˵��:
*/
void config_save(void);

/*
	����:
		��λdev��user�� wifi���ݿ��е�����, �����reset_count����һ
	�������:
		@info: ���ϵͳ��Ϣ���ڴ��
	�������:
		@info: ����ϵͳ��Ϣ
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
void config_reset_two(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* USER_CONFIG_H */
