/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: 用户配置文件
**  File:    user_config.c
**  Author:  liubenlong
**  Date:    07/26/2014
**
**  Purpose:
**    用户配置文件.
*
	0x00000    -----------------------------------
			|						|
			|    		   boot             	|  4K
			|						|
	0x1000     -----------------------------------
			|                   			|
			|     	user1.bin     		|  492K
			|                   			|
	0x7C000    -----------------------------------
			|                   			|
			|    		user param 	 		|  16K
			|                   			|
	0x80000     -----------------------------------
			|						|
			|   		reserve         		|  4K
			|						|
	0x81000    -----------------------------------
			|						|
			|	  	user2.bin     		|  492K
			|						|
	0xFC000	-----------------------------------
			|						|
			|  		reserve         		|  4k
			|						|
	0xFD000	-----------------------------------
			|						|
			|   		licence        		| 4K  
			|						|
	0xFE000	-----------------------------------
			|						|
			|  		 uuid          		| 4K 
			|						|
	0xFF000	-----------------------------------
			|						|
			|   		wifi     			| 4K  
			|						|
	0x100000	-----------------------------------
			|						|
			|   		user config1     		| 8K  
			|						|
	0x102000	-----------------------------------
			|						|
			|   		user config2     		| 8K  
			|						|
	0x104000	-----------------------------------
			|						|
			|   		user config1           | 8K
			|						|
	0x106000	-----------------------------------
			|						|
			|   		user config2           | 8K
			|						|
	0x108000	-----------------------------------
			|						|
			|  		 dev config1           | 8K
			|						|
	0x10A000	-----------------------------------
			|						|
			|   		dev config2            | 8K
			|						|
	0x10C000	-----------------------------------
			|						|
			|   		dev config3            | 8K
			|						|
	0x10E000	-----------------------------------
			|						|
			|   		dev config4            | 8K
			|						|
	0x110000	-----------------------------------
			|						|
			|   		dev config5            | 8K
			|						|
	0x112000	-----------------------------------
			|						|
			|   		fix config       		| 8K
			|						|
	0x114000	-----------------------------------
			|						|
			| 		reserver			| 928K
			|						|
	0x1fc000	-----------------------------------
			|						|
			|   esp_init_data_default.bin	| 8K
			|						|
	0x1fe000	-----------------------------------
			|						|
			|  		 blank.bin         	| 8K
			|						|
	0x200000	-----------------------------------
***************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "iw_user_config.h"
#include "licence.h"
#include "iw_linkage_conf.h"
#include "iw_reset_already_finish.h"

/* Macro constant definitions. */
#define WIFI_CONFIG_MAGIC	0x87737073   /* WIFI */
#define USER_CONFIG_MAGIC	0x85836982   /* USER */
#define DEV_CONFIG_MAGIC    0x68698683   /* DEVS */
#define FIX_CONFIG_MAGIC    0x12345678   /* FIXS */


/* Type definitions. */


/* Local function declarations. */


/* Macro API definitions. */
//#define UC_DEBUG_TEST
#ifdef UC_DEBUG_TEST
#define UC_DEBUG(fmt...) ERROR(fmt)
#define UC_INFO(fmt...)  ERROR(fmt)
#define UC_ERROR(fmt...) ERROR(fmt)
#define UC_TEST(fmt...)  ERROR(fmt)
#else
#define UC_DEBUG(fmt...) DEBUG(fmt)
#define UC_INFO(fmt...)  INFO(fmt)
#define UC_ERROR(fmt...) ERROR(fmt)
#define UC_TEST(fmt...)
#endif

/* Global variable declarations. */

// TODO: 添加一个激活字段
/* wifi 只会配置一次,使用一个4K就可以 */
block_t wifi_config = {
	"wifi",						//name
	WIFI_CONFIG_MAGIC,			//magic
	{{0, 0, 0}},				// cache
	SPI_FLASH_WIFI_CFG, 		/* flash_offset 0x00083000 */
	FLASH_SECTOR_SIZE,		// flash_max_len
	WIFI_MAX_ID,			//max_id
	FLASH_SECTOR_SIZE,		//unit_size
	1,						//unit_num
	0,						//valid_unit
	0,						//valid
	0,						//dirty
	NULL					//unit_stat
};

/* 用户配置使用两个4K, 使用两个单元，这样不会丢失配置 */
block_t user_config = {
	"user",					//name
	USER_CONFIG_MAGIC,		//magic
	{{0, 0, 0}},			// cache
	SPI_FLASH_USER_CFG1, /* flash_offset 0x00084000 */
	FLASH_SECTOR_SIZE * 2 * 2,	// flash_max_len
	USER_MAX_ID,			//max_id
	2 * FLASH_SECTOR_SIZE,	//unit_size
	2,						//unit_num
	0,						//valid_unit
	0,						//valid
	0,						//dirty
	NULL					//unit_stat

};

/* 设备状态信息保存,保存比较平凡，使用3个单元，增加使用寿命 */
block_t dev_config = {
	"dev",					//name
	DEV_CONFIG_MAGIC,		//magic
	{{0, 0, 0}},				// cache
	SPI_FLASH_DEV_CFG1, 	/* flash_offset 0x00086000 */
	FLASH_SECTOR_SIZE * 2 * 5,		// flash_max_len
	DEV_MAX_ID,				//max_id
	2 * FLASH_SECTOR_SIZE,		//unit_size
	5,						//unit_num
	0,						//valid_unit
	0,						//valid
	0,						//dirty
	NULL					//unit_stat

};
/*保存设备状态，恢复出厂不进行清楚的数据块*/
block_t fix_config = {
	"fix",					//name
	FIX_CONFIG_MAGIC,		//magic
	{{0, 0, 0}},			// cache
	FIX_CONFIG_FLASH_OFFSET, /* flash_offset 0x00073000 */
	FLASH_SECTOR_SIZE,		// flash_max_len
	FIX_MAX_ID,				//max_id
	FLASH_SECTOR_SIZE,		//unit_size
	2,						//unit_num
	0,						//valid_unit
	0,						//valid
	0,						//dirty
	NULL					//unit_stat

};

RS ICACHE_FLASH_ATTR conf_fix_init()
{
	return fb_block_init(&fix_config);
}

void ICACHE_FLASH_ATTR conf_fix_reset()
{
	fb_block_reset(&fix_config);
}

config_t* ICACHE_FLASH_ATTR conf_fix_get( u_int16_t id)
{
	return fb_get(&fix_config, id);
}

RS ICACHE_FLASH_ATTR conf_fix_set(u_int16_t id, UINT8 *buff, u_int16_t len)
{

	if (! g_dev.active) {
		UC_ERROR("dev has not acitve. not save configure\r\n");
		return RS_ERROR;
	}

	return fb_set(&fix_config, id, buff, len, false);
}

RS ICACHE_FLASH_ATTR conf_fix_commit()
{
	return fb_block_save(&fix_config);
}

RS ICACHE_FLASH_ATTR conf_wifi_init()
{
	return fb_block_init(&wifi_config);
}

void ICACHE_FLASH_ATTR conf_wifi_reset()
{
	fb_block_reset(&wifi_config);
}

config_t* ICACHE_FLASH_ATTR conf_wifi_get( u_int16_t id)
{
	return fb_get(&wifi_config, id);
}

RS ICACHE_FLASH_ATTR conf_wifi_set(u_int16_t id, UINT8 *buff, u_int16_t len)
{
	if (! g_dev.active) {
		UC_TEST("dev has not acitve. not save configure\r\n");
		return RS_ERROR;
	}
	return fb_set(&wifi_config, id, buff, len, false);
}

RS ICACHE_FLASH_ATTR conf_wifi_commit()
{
	return fb_block_save(&wifi_config);
}


RS ICACHE_FLASH_ATTR conf_user_init()
{
	return fb_block_init(&user_config);
}

void ICACHE_FLASH_ATTR conf_user_reset()
{
	fb_block_reset(&user_config);
}

config_t* ICACHE_FLASH_ATTR conf_user_get(u_int16_t id)
{
	return fb_get(&user_config, id);
}

RS ICACHE_FLASH_ATTR conf_user_set(u_int16_t id, UINT8 *buff, u_int16_t len)
{
	if (! g_dev.active) {
		UC_TEST("dev has not acitve. not save configure\r\n");
		return RS_ERROR;
	}

	fb_set(&user_config, id, buff, len, false);

	iw_request_event(EVENT_SYS_SAVE_CONFIG);

	return RS_OK;
}

RS ICACHE_FLASH_ATTR conf_user_commit()
{
	return fb_block_save(&user_config);
}

RS ICACHE_FLASH_ATTR conf_dev_init()
{
	return fb_block_init(&dev_config);
}

void ICACHE_FLASH_ATTR conf_dev_reset()
{
	fb_block_reset(&dev_config);
}

config_t *ICACHE_FLASH_ATTR conf_dev_get(u_int16_t id)
{
	return fb_get(&dev_config, id);
}

RS ICACHE_FLASH_ATTR conf_dev_set(u_int16_t id, UINT8*buff, u_int16_t len)
{
	if (! g_dev.active) {
		UC_TEST("dev has not acitve. not save configure\r\n");
		return RS_ERROR;
	}

	fb_set(&dev_config, id, buff, len, false);

	//配置调用耗时且多，放起来统一处理
	iw_request_event(EVENT_SYS_SAVE_CONFIG);

	return RS_OK;
}

RS ICACHE_FLASH_ATTR conf_dev_commit()
{
	return fb_block_save(&dev_config);
}

void ICACHE_FLASH_ATTR config_save(void)
{
	if (! g_dev.active) {
		UC_TEST("dev has not acitve. not save configure\r\n");
		return;
	}

	if(conf_user_commit() != RS_OK) {
		iw_request_event(EVENT_SYS_SAVE_CONFIG);
	}

	if (conf_dev_commit() != RS_OK) {
		iw_request_event(EVENT_SYS_SAVE_CONFIG);
	}
}

void ICACHE_FLASH_ATTR config_reset_two(void)
{
	conf_wifi_reset();
	conf_user_reset();
	conf_dev_reset();

	//保存恢复出厂是否彻底
	set_reset_state(0);
	
	//保存固定配置
	conf_fix_commit();

	UC_ERROR("reset finish\n");
}

static void ICACHE_FLASH_ATTR lock_fix(void)
{
	//保存恢复出厂设置
	lc_reset_count_increased();
	
	//保存恢复出厂是否彻底标志
	set_reset_state(1);
	
	//保存固定配置
	conf_fix_commit();	
}

static void ICACHE_FLASH_ATTR unlock_fix(void)
{
	//保存恢复出厂是否彻底
	set_reset_state(0);
	
	//保存固定配置
	conf_fix_commit();	
}

void ICACHE_FLASH_ATTR config_reset(void)
{
	lock_fix();

	conf_wifi_reset();
	conf_dev_reset();
	conf_user_reset();

	unlock_fix();
	UC_ERROR("reset now\n");
}

