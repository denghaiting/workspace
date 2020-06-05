#ifndef __IWUOS_FLASH_HEADER__
#define __IWUOS_FLASH_HEADER__

#include "licence.h"
#include "udp_ctrl.h"
#include "user_esp_platform.h"
#include "c_types.h"
#include "types_def.h"




enum {
        FLASH_ID_WIFI_PARAM = 1,                /* WiFi ���� */
        FLASH_ID_LICENSE,                            /*  ֤�� */
        FLASH_ID_UUID,                                 /*  uuid */
        FLASH_ID_WIFI_CFG,                     /* WiFi ���� */
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

/* 0xFD000 ֤��*/
#define SPI_FLASH_LICENSE                     (SPI_FLASH_WIFI_PARAM + SPI_FLASH_4K_SIZE)

/* 0xFE000 uuid */
#define SPI_FLASH_UUID                        (SPI_FLASH_LICENSE + SPI_FLASH_4K_SIZE)

/* 0xFF000 wifi ����*/
#define SPI_FLASH_WIFI_CFG                    (SPI_FLASH_UUID + SPI_FLASH_4K_SIZE)

/* 0x100000 USER����*/
#define SPI_FLASH_USER_CFG1                   (SPI_FLASH_WIFI_CFG + SPI_FLASH_4K_SIZE)
#define SPI_FLASH_USER_CFG2                   (SPI_FLASH_USER_CFG1 + SPI_FLASH_8K_SIZE)

/* 0x104000 DEV����*/
#define SPI_FLASH_DEV_CFG1                    (SPI_FLASH_USER_CFG2 + SPI_FLASH_8K_SIZE)
#define SPI_FLASH_DEV_CFG2                    (SPI_FLASH_DEV_CFG1 + SPI_FLASH_8K_SIZE)
#define SPI_FLASH_DEV_CFG3                    (SPI_FLASH_DEV_CFG2 + SPI_FLASH_8K_SIZE)
#define SPI_FLASH_DEV_CFG4                    (SPI_FLASH_DEV_CFG3 + SPI_FLASH_8K_SIZE)
#define SPI_FLASH_DEV_CFG5                    (SPI_FLASH_DEV_CFG4 + SPI_FLASH_8K_SIZE)

#define STM_IMAGE_OFFSET		(0x113000)

/* 0x10E000 ����������Ϣ���ָ��������������*/
#define FIX_CONFIG_FLASH_OFFSET    			  (SPI_FLASH_DEV_CFG5 + SPI_FLASH_8K_SIZE)   /* FIXS */
#define macbeesave_offset                       (0x118000 )//����macbee����Ϣ��

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
	u_int32_t flash_offset;		/* ����洢��Ԫ��ʼ��ַ */
	u_int16_t size;				/* �洢��Ԫ��С,4K�������� */
	u_int16_t save_count;		/* һ��������ٴ� */
	u_int16_t fail_count;		/* ����ʧ�ܵ��ܴ��� */
	u_int16_t continue_failed;	/* ��������ʧ�ܵĴ���,�����ֵ���ж�����洢��Ԫ�Ƿ��� */
}block_unit_stat_t;

typedef struct block_s {
	char *name;
	u_int32_t magic;
	config_t  cache[MF_MAX_CACHE_ENTRY];	//���õĻ�����
	u_int32_t flash_offset;		/* flash�Ŀ�ʼ��ַ*/
	u_int32_t flash_max_len;	/* �������ֵ�����������ʱû����*/
	u_int16_t max_id;
	u_int16_t unit_size;		/* �洢��Ԫ��С,4K�������� */
	u_int16_t unit_num;			/* ���ٸ��洢��Ԫ������һ���洢��Ԫ��Ϊ���൥Ԫ */
	u_int16_t valid_unit;		/* ��Ч�ĵ�Ԫ����Ч��Ԫ�����0��ʼ */

	char valid;			/* ���ģ���Ƿ���Ч */
	char dirty;		   /* ������Ƿ��޸Ĺ�����Ҫ����flash*/

	block_unit_stat_t *unit_stat;
} block_t;

/* External function declarations. */


/* Macro API definitions. */

// TODO: �������ӿ�
/* Global variable declarations. */

/*
	����:
		дflash
	�������:
		@start_addr: flash��ַ
		@len: ���ݳ���
		@value: ����
	�������:
	����ֵ:
		true: �ɹ�
		flase ʧ��
	˵��:
*/
bool  flash_write(u_int32_t start_addr, u_int32_t len, u_int8_t *value);

/*
	����:
		��flash
	�������:
		@start_addr: flash��ַ
		@len: ���ݳ���
		@value: ����
	�������:
	����ֵ:
		true: �ɹ�
		flase ʧ��
	˵��:
*/
bool flash_read_new(u_int32_t start_addr, u_int32_t len, u_int8_t *value);

/*
	����:
		дһҳflash
	�������:
		@addr: flash��ַ
		@pbuf: ����
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
extern RS  flash_write_page(u_int32_t addr, u_int8_t *pbuf);

/*
	����:
		���ݿ��ʼ��
	�������:
		@block:�����ݽṹ
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS fb_block_init(block_t *block);

/*
	����:
		���ݿ鱣��
	�������:
		@block:�����ݽṹ
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS fb_block_save(block_t *block);

/*
	����:
		���ݿ鸴λ
	�������:
		@block:�����ݽṹ
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
void fb_block_reset(block_t *block);

/*
	����:
		ͨ��id�����ݿ��еõ��������Ϣ
	�������:
		@block:�����ݽṹ
		@id:��Ϣid
	�������:
	����ֵ:
		config_t *: id��Ӧ������
	˵��:
*/
config_t * fb_get(block_t *block, u_int16_t id);

/*
	����:
		�����ݿ���id��λ�ñ�����Ҫ�������Ϣ
	�������:
		@block:�����ݽṹ
		@id:��Ϣid
		@buff:����
		@len:���ݳ���
		@save:�Ƿ����ϱ��浽flash��
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
RS fb_set(block_t *block, u_int16_t id, UINT8 *buff, u_int16_t len, bool save);

#endif
