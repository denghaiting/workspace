#ifndef __IWUOS_FLASH_HEADER__
#define __IWUOS_FLASH_HEADER__

#include "licence.h"
#include "udp_ctrl.h"
#include "user_esp_platform.h"
#include "c_types.h"
#include "types_def.h"




enum {
        FLASH_ID_WIFI_PARAM = 1,                /* WiFi 参数 */
        FLASH_ID_LICENSE,                            /*  证书 */
        FLASH_ID_UUID,                                 /*  uuid */
        FLASH_ID_WIFI_CFG,                     /* WiFi 配置 */
        FLASH_ID_USER_CFG1,                       /*  user config1 */ 
        FLASH_ID_USER_CFG2,                       /*  user config2 */ 
        FLASH_ID_DEV_CFG1,                        /*  dev config1 */ 
        FLASH_ID_DEV_CFG2,                        /*  dev config2 */ 
        FLASH_ID_DEV_CFG3,                        /*  dev config3 */ 
        FLASH_ID_DEV_CFG4,                        /*  dev config4 */ 
        FLASH_ID_DEV_CFG5,                        /*  dev config5 */ 
        FLASH_ID_MAX
};

#define	min(a, b)	(((a) < (b)) ? (a) : (b))
#define FLASH_WRITE_FAIL_MAX_NUM 5

typedef struct flash_uuid_s {
        u_int8_t vaild;
        u_int8_t pad[3];
        u_int8_t uuid[MAX_UUID_BIN_LEN];
}flash_uuid_t;

typedef struct cf_wifi_s {
	u_int8_t ssid[IW_MAX_WIFI_SSID];
	u_int8_t passwd[IW_MAX_WIFI_PASSWD];
}cf_wifi_t;

typedef struct {
        u_int16_t vaild;
        u_int16_t len;
}flash_config_t;

typedef struct {
	u_int16_t vaild;
    u_int16_t len;
	u_int8_t  value[256];
}cf_dev_t;

#define FLASH_PAGE_SIZE         (256)
#define FLASH_SECTOR_SIZE (4096)
#define SPI_FLASH_4K_SIZE  (0x1000)
#define SPI_FLASH_8K_SIZE  (0x2000)


#define SPI_FLASH_WIFI_PARAM                  (0xFC000)

/* 0xFD000 证书*/
#define SPI_FLASH_LICENSE                     (SPI_FLASH_WIFI_PARAM + SPI_FLASH_4K_SIZE)

/* 0xFE000 uuid */
#define SPI_FLASH_UUID                        (SPI_FLASH_LICENSE + SPI_FLASH_4K_SIZE)

/* 0xFF000 wifi 配置*/
#define SPI_FLASH_WIFI_CFG                    (SPI_FLASH_UUID + SPI_FLASH_4K_SIZE)

/* 0x100000 USER配置*/
#define SPI_FLASH_USER_CFG1                   (SPI_FLASH_WIFI_CFG + SPI_FLASH_4K_SIZE)
#define SPI_FLASH_USER_CFG2                   (SPI_FLASH_USER_CFG1 + SPI_FLASH_8K_SIZE)

/* 0x104000 DEV配置*/
#define SPI_FLASH_DEV_CFG1                    (SPI_FLASH_USER_CFG2 + SPI_FLASH_8K_SIZE)
#define SPI_FLASH_DEV_CFG2                    (SPI_FLASH_DEV_CFG1 + SPI_FLASH_8K_SIZE)
#define SPI_FLASH_DEV_CFG3                    (SPI_FLASH_DEV_CFG2 + SPI_FLASH_8K_SIZE)
#define SPI_FLASH_DEV_CFG4                    (SPI_FLASH_DEV_CFG3 + SPI_FLASH_8K_SIZE)
#define SPI_FLASH_DEV_CFG5                    (SPI_FLASH_DEV_CFG4 + SPI_FLASH_8K_SIZE)

#define STM_IMAGE_OFFSET		(0x113000)

/* 0x10E000 保存永久信息，恢复出厂不进行清楚*/
#define FIX_CONFIG_FLASH_OFFSET    			  (SPI_FLASH_DEV_CFG5 + SPI_FLASH_8K_SIZE)   /* FIXS */
#define macbeesave_offset                       (0x118000 )//保存macbee灯信息用

/* Macro constant definitions. */
#define MF_MAX_CACHE_ENTRY 100

/* Type definitions. */

typedef struct config_head_s {
	u_int16_t id;
	u_int16_t len;
}config_head_t;


typedef struct config_s {
	u_int16_t id;
	u_int16_t len;
	u_int8_t *value;
}config_t;


typedef struct block_unit_stat_s{
	u_int32_t flash_offset;		/* 这个存储单元开始地址 */
	u_int16_t size;				/* 存储单元大小,4K的整数倍 */
	u_int16_t save_count;		/* 一共保存多少次 */
	u_int16_t fail_count;		/* 保存失败的总次数 */
	u_int16_t continue_failed;	/* 连续保存失败的次数,用这个值来判断这个存储单元是否坏了 */
}block_unit_stat_t;

typedef struct block_s {
	char *name;
	u_int32_t magic;
	config_t  cache[MF_MAX_CACHE_ENTRY];	//配置的缓存项
	u_int32_t flash_offset;		/* flash的开始地址*/
	u_int32_t flash_max_len;	/* 这块的最大值，这个变量暂时没有用*/
	u_int16_t max_id;
	u_int16_t unit_size;		/* 存储单元大小,4K的整数倍 */
	u_int16_t unit_num;			/* 多少个存储单元，大于一个存储单元作为冗余单元 */
	u_int16_t valid_unit;		/* 生效的单元，有效单元编号是0开始 */

	char valid;			/* 这个模块是否有效 */
	char dirty;		   /* 这个块是否修改过，需要保存flash*/

	block_unit_stat_t *unit_stat;
} block_t;

/* External function declarations. */


/* Macro API definitions. */

// TODO: 不公布接口
/* Global variable declarations. */

/*
	功能:
		写flash
	输入参数:
		@start_addr: flash地址
		@len: 数据长度
		@value: 数据
	输出参数:
	返回值:
		true: 成功
		flase 失败
	说明:
*/
bool  flash_write(u_int32_t start_addr, u_int32_t len, u_int8_t *value);

/*
	功能:
		读flash
	输入参数:
		@start_addr: flash地址
		@len: 数据长度
		@value: 数据
	输出参数:
	返回值:
		true: 成功
		flase 失败
	说明:
*/
bool flash_read_new(u_int32_t start_addr, u_int32_t len, u_int8_t *value);

/*
	功能:
		写一页flash
	输入参数:
		@addr: flash地址
		@pbuf: 数据
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
extern RS  flash_write_page(u_int32_t addr, u_int8_t *pbuf);

/*
	功能:
		数据块初始化
	输入参数:
		@block:块数据结构
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS fb_block_init(block_t *block);

/*
	功能:
		数据块保存
	输入参数:
		@block:块数据结构
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS fb_block_save(block_t *block);

/*
	功能:
		数据块复位
	输入参数:
		@block:块数据结构
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
void fb_block_reset(block_t *block);

/*
	功能:
		通过id在数据块中得到保存的信息
	输入参数:
		@block:块数据结构
		@id:信息id
	输出参数:
	返回值:
		config_t *: id对应的数据
	说明:
*/
config_t * fb_get(block_t *block, u_int16_t id);

/*
	功能:
		在数据块中id的位置保存需要保存的信息
	输入参数:
		@block:块数据结构
		@id:信息id
		@buff:数据
		@len:数据长度
		@save:是否马上保存到flash中
	输出参数:
	返回值:
		RS_OK: 成功
		其他: 失败
	说明:
*/
RS fb_set(block_t *block, u_int16_t id, UINT8 *buff, u_int16_t len, bool save);

#endif
