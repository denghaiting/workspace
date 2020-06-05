/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    reset_already_finish.c
**  Author:  fangfuhan
**  Date:    05/21/2016
**
**  Purpose:
**    保证恢复出厂设置彻底完成.
**************************************************************************/


/* Include files. */
#include "iw_reset_already_finish.h"
#include "iw_priv.h"
#include "iw_user_config.h"
#include "c_types.h"

/* Macro constant definitions. */
#define NEED_RESET_AGAIN 1

/* Type definitions. */


/* Local function declarations. */


/* Macro API definitions. */

//#define RESET_DEBUG_
#ifdef RESET_DEBUG_
#define	RESET_DEBUG(fmt...) DEBUG(fmt)
#define	RESET_INFO(fmt...)  INFO(fmt)
#define	RESET_ERROR(fmt...) ERROR(fmt)
#else
#define	RESET_DEBUG(fmt...)
#define	RESET_INFO(fmt...)
#define	RESET_ERROR(fmt...) ERROR(fmt)
#endif


/* Global variable declarations. */
reset_finish_t my_reset;

void ICACHE_FLASH_ATTR set_reset_state(u_int8_t state)
{

	RESET_ERROR("set reset state = %u\r\n", state);
	my_reset.reset_finish = state;

	conf_fix_set(FACTORY_ERASE_SIGN, (u_int8_t *)&my_reset, sizeof(my_reset));
}

static u_int8_t ICACHE_FLASH_ATTR get_reset_state(void)
{
	return my_reset.reset_finish;
}

static void ICACHE_FLASH_ATTR reset_state_init(void)
{
	config_t *config = NULL;
	reset_finish_t *data;
	
	memset(&my_reset, 0, sizeof(my_reset));
	
	config = conf_fix_get( FACTORY_ERASE_SIGN );
	if(config == NULL || config->len < sizeof(my_reset)){
		RESET_ERROR("my_reset is NULL\r\n");
		return;
	}

	data = (reset_finish_t *)config->value;

	my_reset.reset_finish = data->reset_finish;
	RESET_ERROR("reset finish state = [%u]\r\n", my_reset.reset_finish);
}

void ICACHE_FLASH_ATTR check_reset_state(void)
{
	u_int8_t reset_state = 0;
	
	reset_state_init();
	reset_state = get_reset_state();
	if(NEED_RESET_AGAIN == reset_state){
		RESET_ERROR("continue reset_state\r\n");
		config_reset_two();
	}	
}

