/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: 联动
**  File:    linkage_conf.c
**  Author:  cailiyang
**  Date:    12/24/2015
**
**  Purpose:
**    恢复出厂保存.
**************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "types_def.h"
#include "user_config.h"
#include "iw_linkage_conf.h"
#include "iw_func_support.h"

/* Macro constant definitions. */


/* Type definitions. */


/* Local function declarations. */


/* Macro API definitions. */
//#define LINKAGE_DEBUG_
#ifdef LINKAGE_DEBUG_
#define LINKAGE_INFO(x...)  INFO(x)
#define LINKAGE_DEBUG(x...) DEBUG(x)
#define LINKAGE_ERROR(x...)  ERROR(x)
#else
#define LINKAGE_DEBUG(x...)
#define LINKAGE_INFO(x...)
#define LINKAGE_ERROR(x...) ERROR(x)
#endif


/* Global variable declarations. */

//恢复出厂的总次数
static u_int16_t reset_count = 0;

void ICACHE_FLASH_ATTR lc_init(void)
{
	config_t *conf = NULL;

	conf = conf_fix_get(FACTORY_RESET_COUNT);
	if (NULL == conf || conf->len < sizeof(reset_count)){
		reset_count = 0;
		LINKAGE_ERROR("reset_count:%d\r\n", reset_count);
		return;
	}

	reset_count = *(u_int16_t*)conf->value;

	LINKAGE_ERROR("reset_count:%d\r\n", reset_count);
}

u_int16_t ICACHE_FLASH_ATTR lc_get_reset_count(void)
{
	return reset_count;
}

void ICACHE_FLASH_ATTR lc_reset_count_increased()
{
	reset_count++;

	conf_fix_set(FACTORY_RESET_COUNT, (UINT8 *)&reset_count, sizeof(reset_count));
}

#ifdef LINKAGE_SUPPORT
u_int32_t get_home_id()
{
	config_t *config = NULL;
	u_int32_t home_id = 0;
	
	//读取home_id
	config = conf_user_get(HOME_ID);
	if( config != NULL ){
		home_id = *(u_int32_t *)config->value;
	}else{
		home_id = 0;
	}

	return home_id;	
}
#endif

