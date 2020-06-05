/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 定时器
**  File:    iw_dev_timer.h
**  Author:  liubenlong
**  Date:    07/29/2014
**
**  Purpose:
**    定时器.
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
//整个测试定时器数据结构，以后真实要用时自己改
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
	u_int8_t on_valid;			/* 是否有下次定时开机 */
	u_int8_t on_day;			/* 距最近一次定时开机天数 */
	u_int8_t on_hour;			/* 距最近一次定时开机小时数 */
	u_int8_t on_min;			/* 距最近一次定时开机分钟数 */
	u_int16_t on_duration;		/* 最近一次定时开机持续分钟数 */
	u_int8_t off_valid;			/* 是否有下次定时关机 */
	u_int8_t off_day;			/* 距最近一次定时关机天数 */
	u_int8_t off_hour;			/* 距最近一次定时关机小时数*/
	u_int8_t off_min;			/* 距最近一次定时关机分钟数 */
	u_int16_t off_duration;		/* 最近一次定时关机持续分钟数 */
}period_timer_head_t;

typedef void (* uc_timer_func_t)(void *arg);
typedef struct {
	u_int8_t id;			/* 策略ID */
	u_int8_t enable;		/* 是否生效(手机设置) 或者已经无效(设备返回) */	
	u_int8_t week;			/* bit 0-6位对应星期1到星期天 */	
	u_int8_t hours;			/* 小时 0-23 */
	u_int8_t minute;		/* 分钟 0-59 */
	u_int8_t on;
	u_int8_t repeat;
	u_int8_t pad;			/* 保留 */
} net_timer_t;

typedef struct {
	struct stlc_list_head link;
	u_int8_t id;
	u_int8_t enable;
	u_int8_t week;			/* bit 0-6位对应星期天到星期六*/
	u_int8_t hours;			/* 小时 0-23 */
	u_int8_t minute;		/* 分钟 0-59 */	
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


/*串口时间设置*/
#define SERIAL_TIME_CMD_GET		0
#define SERIAL_TIME_CMD_SET		1

typedef struct serial_time_s{
	u_int32_t cmd;
	uc_time_t time;
}serial_time_t;


typedef struct {
	u_int8_t id;				/* 策略ID */
	u_int8_t hours;			/* 小时 0-23 */
	u_int8_t minute;			/* 分钟 0-59 */
	u_int8_t week;				/* bit 0-6位对应星期天到星期六 */
	u_int8_t enable;			/* 是否生效(手机设置) 或者已经无效(设备返回) */
    	u_int8_t on;				/* 是否是定时开机 */
	u_int16_t duration;			/* 持续分钟数 */
    	void * ext_data;	
} net_period_timer_t;

typedef struct {
	struct stlc_list_head link;
	net_period_timer_t timer;	/* 定时器 */
    	uc_timer_func_t start_proc;		/* 定时器触发后的回调函数 */
    	uc_timer_func_t finish_proc;		/* 持续时间到后回调函数 */
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


//添加一个定时器
extern int ucd_timer_add(uc_timer_func_t start_proc, uc_timer_func_t finish_proc, u_int8_t id,  net_period_timer_t *expir);

//根据id号删除一个定时器
extern RS ucd_timer_del(u_int8_t id);

//根据id号调整一个定时器
extern RS ucd_timer_mod(u_int8_t id, net_period_timer_t *expir);

//添加一个定时器
extern int ucd_timer_add_ext(uc_timer_func_t start_proc, uc_timer_func_t finish_proc, u_int8_t id,  net_period_timer_t *expir,
			void * ext_data, int data_len);
//根据id号删除一个定时器
extern RS ucd_timer_del_ext(u_int8_t id);

//根据id号调整一个定时器
extern RS ucd_timer_mod_ext(u_int8_t id, net_period_timer_t *expir, void * ext_data, int data_len);

/*
	功能:
		系统定时链表初始化
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void iw_ucd_timer_init(void);

/*
	功能:
		启动设备定时器检查， 
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void ucd_timer_rebuild(void);

/*
	功能:
		保存已经添加的定时器
	输入参数:
	输出参数:
	返回值:
	说明:
*/
void config_timer_save(void);

/*
	功能:
		获取系统信息
	输入参数:
		@start_proc: 定时器定时时间到回调
		@finish_proc: 定时器持续时间到回调
	输出参数:
	返回值:
	说明:
*/
void config_timer_init(uc_timer_func_t start_proc, uc_timer_func_t finish_proc);


struct stlc_list_head  period_timer_head;
#define PERIOD_TIMER	(period_timer_head)

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* UC_DEV_TIMER_H */

