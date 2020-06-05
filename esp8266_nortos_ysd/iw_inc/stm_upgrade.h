/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: stm_upgrade.h
**  File:    stm_upgrade.h
**  Author:  liubenlong
**  Date:    12/18/2014
**
**  Purpose:
**    ��Ƭ������.
**************************************************************************/


#ifndef STM_UPGRADE_H
#define STM_UPGRADE_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */
#define STM_UPGRADE_HEADER_SIZE		sizeof(stm_upgrade_header_t)


/* Type definitions. */
#pragma pack(push,1)
/* Type definitions. */
/*
��total-block ���� current-blockʱ��dataΪ���������ݡ�
��total-block ���� current-blockʱ����ʾ���������dataΪ��������md5У��͡�
*/
typedef struct stm_upgrade_header_s{
	u_int16_t total_block;
	u_int16_t current_block;
	//u_int8_t data[0]
}stm_upgrade_header_t;

typedef struct stm_image_hd_s{
	u_int32_t magic;
	u_int32_t len;
	u_int16_t data_crc;
	u_int8_t dev_type;
	u_int8_t ext_type;
	u_int8_t major_ver; //���汾��,����
	u_int8_t minor_ver; //�ΰ汾��,����
	u_int8_t image_type;  //�̼����ͣ����Ρ�1-app��2-rf, 3-stm
	u_int8_t pad;
	u_int32_t magic2;
}stm_image_hd_t;

#pragma pack(pop)

/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */
extern void do_ucot_system_stm_upgrade_image(iw_event_info_t *info, iw_obj_t *uobj);
extern void stm_upgrade_init(void);
extern bool is_stm_upgrade_err(void);
extern void stm_upgrade_err_reset(void);
extern u_int8_t stm_image_is_valid(void);

/***************************************************************/
extern void stm_image_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* STM_UPGRADE_H */

