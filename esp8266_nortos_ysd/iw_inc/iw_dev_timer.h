/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: ��ʱ��
**  File:    iw_dev_timer.h
**  Author:  liubenlong
**  Date:    07/29/2014
**
**  Purpose:
**    ��ʱ��.
**************************************************************************/


#ifndef UC_DEV_TIMER_H
#define UC_DEV_TIMER_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/* Macro constant definitions. */
#define ONE_DAY_MIN			(24*60)
#define ONE_DAY_SEC			(24*60*60)
#define ONE_HOUR_SEC		(1*60*60)
#define ONE_MINI_SEC		(1*60)
#define ONE_HOUR_MIN		(1*60)

#define TIMER_ON 		0

/* Type definitions. */
#pragma pack(push,1)
//�������Զ�ʱ�����ݽṹ���Ժ���ʵҪ��ʱ�Լ���
typedef struct  ac_timer_head_s{
	u_int8_t on_valid;
	u_int8_t on_day;
	u_int8_t on_hour;
	u_int8_t on_min;
	u_int8_t off_valid;
	u_int8_t off_day;
	u_int8_t off_hour;
	u_int8_t off_min;
}sys_timer_head_t;

typedef struct {
	u_int8_t on_valid;			/* �Ƿ����´ζ�ʱ���� */
	u_int8_t on_day;			/* �����һ�ζ�ʱ�������� */
	u_int8_t on_hour;			/* �����һ�ζ�ʱ����Сʱ�� */
	u_int8_t on_min;			/* �����һ�ζ�ʱ���������� */
	u_int16_t on_duration;		/* ���һ�ζ�ʱ�������������� */
	u_int8_t off_valid;			/* �Ƿ����´ζ�ʱ�ػ� */
	u_int8_t off_day;			/* �����һ�ζ�ʱ�ػ����� */
	u_int8_t off_hour;			/* �����һ�ζ�ʱ�ػ�Сʱ��*/
	u_int8_t off_min;			/* �����һ�ζ�ʱ�ػ������� */
	u_int16_t off_duration;		/* ���һ�ζ�ʱ�ػ����������� */
}period_timer_head_t;

typedef void (* uc_timer_func_t)(void *arg);
typedef struct {
	u_int8_t id;			/* ����ID */
	u_int8_t enable;		/* �Ƿ���Ч(�ֻ�����) �����Ѿ���Ч(�豸����) */	
	u_int8_t week;			/* bit 0-6λ��Ӧ����1�������� */	
	u_int8_t hours;			/* Сʱ 0-23 */
	u_int8_t minute;		/* ���� 0-59 */
	u_int8_t on;
	u_int8_t repeat;
	u_int8_t pad;			/* ���� */
} net_timer_t;

typedef struct {
	struct stlc_list_head link;
	u_int8_t id;
	u_int8_t enable;
	u_int8_t week;			/* bit 0-6λ��Ӧ�����쵽������*/
	u_int8_t hours;			/* Сʱ 0-23 */
	u_int8_t minute;		/* ���� 0-59 */	
	u_int8_t on;
	void *pdata;
	bool repeat;
	uc_timer_func_t proc;	
	u_int32_t time_sec;	
	u_int32_t time_sec_local;
	u_int8_t pad[4];
} ucd_timer_node_t;

struct time_zone{
	int hour;
	int minute;
};


/*����ʱ������*/
#define SERIAL_TIME_CMD_GET		0
#define SERIAL_TIME_CMD_SET		1

typedef struct serial_time_s{
	u_int32_t cmd;
	uc_time_t time;
}serial_time_t;


typedef struct {
	u_int8_t id;				/* ����ID */
	u_int8_t hours;			/* Сʱ 0-23 */
	u_int8_t minute;			/* ���� 0-59 */
	u_int8_t week;				/* bit 0-6λ��Ӧ�����쵽������ */
	u_int8_t enable;			/* �Ƿ���Ч(�ֻ�����) �����Ѿ���Ч(�豸����) */
    	u_int8_t on;				/* �Ƿ��Ƕ�ʱ���� */
	u_int16_t duration;			/* ���������� */
    	void * ext_data;	
} net_period_timer_t;

typedef struct {
	struct stlc_list_head link;
	net_period_timer_t timer;	/* ��ʱ�� */
    	uc_timer_func_t start_proc;		/* ��ʱ��������Ļص����� */
    	uc_timer_func_t finish_proc;		/* ����ʱ�䵽��ص����� */
	u_int32_t time_sec;
	u_int8_t repeat;
	u_int8_t is_processing;
	u_int16_t pad;
	void *pdata;
} ucd_period_timer_node_t;


#pragma pack(pop)


/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */


//���һ����ʱ��
extern int ucd_timer_add(uc_timer_func_t start_proc, uc_timer_func_t finish_proc, u_int8_t id,  net_period_timer_t *expir);

//����id��ɾ��һ����ʱ��
extern RS ucd_timer_del(u_int8_t id);

//����id�ŵ���һ����ʱ��
extern RS ucd_timer_mod(u_int8_t id, net_period_timer_t *expir);

//���һ����ʱ��
extern int ucd_timer_add_ext(uc_timer_func_t start_proc, uc_timer_func_t finish_proc, u_int8_t id,  net_period_timer_t *expir,
			void * ext_data, int data_len);
//����id��ɾ��һ����ʱ��
extern RS ucd_timer_del_ext(u_int8_t id);

//����id�ŵ���һ����ʱ��
extern RS ucd_timer_mod_ext(u_int8_t id, net_period_timer_t *expir, void * ext_data, int data_len);

/*
	����:
		ϵͳ��ʱ�����ʼ��
	�������:
	�������:
	����ֵ:
	˵��:
*/
void iw_ucd_timer_init(void);

/*
	����:
		�����豸��ʱ����飬 
	�������:
	�������:
	����ֵ:
	˵��:
*/
void ucd_timer_rebuild(void);

/*
	����:
		�����Ѿ���ӵĶ�ʱ��
	�������:
	�������:
	����ֵ:
	˵��:
*/
void config_timer_save(void);

/*
	����:
		��ȡϵͳ��Ϣ
	�������:
		@start_proc: ��ʱ����ʱʱ�䵽�ص�
		@finish_proc: ��ʱ������ʱ�䵽�ص�
	�������:
	����ֵ:
	˵��:
*/
void config_timer_init(uc_timer_func_t start_proc, uc_timer_func_t finish_proc);


struct stlc_list_head  period_timer_head;
#define PERIOD_TIMER	(period_timer_head)

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* UC_DEV_TIMER_H */

