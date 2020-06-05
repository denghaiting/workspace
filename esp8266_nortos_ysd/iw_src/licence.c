/**************************************************************************
**  Copyright (c) 2010 GalaxyWind, Ltd.
**
**  Project: licence.c
**  File:    licence.c
**  Author:
**  Date:    06/10/2015
**
**  Purpose:
**    Licence .
**************************************************************************/


/* Include files. */
#include "licence.h"
#include "ds_proto.h"
#include "iw_priv.h"


/* Macro constant definitions. */


/* Type definitions. */
#define LCC_DEF_PASSWD "123456"
#define LCC_DEF_VENDOR "galaxywind"


/* Local function declarations. */


/* Macro API definitions. */
//#define LIC_DEBUG_CTRL
#ifdef LIC_DEBUG_CTRL
#define LIC_DEBUG(fmt...) DEBUG(fmt)
#define LIC_INFO(fmt...)  INFO(fmt)
#define LIC_ERROR(fmt...) ERROR(fmt)
#else
#define LIC_DEBUG(fmt...)
#define LIC_INFO(fmt...)
#define LIC_ERROR(fmt...) ERROR(fmt)
#endif


/* Global variable declarations. */

devlic_t g_dev;

extern int rand(void);

static u_int32_t  ICACHE_FLASH_ATTR lcc_get_rand(void)
{
	return rand();
}

static u_int64_t ICACHE_FLASH_ATTR lcc_get_rand_sn(void)
{
	u_int64_t rand_sn;
	u_int32_t rand_num;

	rand_num = lcc_get_rand();
	rand_sn = 99llu * 100000000 + rand_num;

	return rand_sn;
}


static void ICACHE_FLASH_ATTR lcc_init(void)
{
	u_int32_t rand_digit;
	u_int8_t *randp;

	os_memset(&g_dev, 0, sizeof(devlic_t));

	rand_digit = lcc_get_rand();

	g_dev.active = FALSE;
	g_dev.sn = lcc_get_rand_sn();
	g_dev.dev_type = IJ_TEST_PRODUCT;
	g_dev.ext_type = 2;
	os_strcpy((char*)g_dev.df_pwd, LCC_DEF_PASSWD);
	os_strcpy((char*)g_dev.vendor, LCC_DEF_VENDOR);
	g_dev.mac[0] = 0x00;
	g_dev.mac[1] = 0x11;
	g_dev.mac[2] = 0xff;
	randp = (u_int8_t*)&rand_digit;
	g_dev.mac[3] = randp[0];
	g_dev.mac[4] = randp[1];
	g_dev.mac[5] = randp[2];
}

static int  ICACHE_FLASH_ATTR lcc_sn_hex_to_u64(char *buf, u_int64_t *out)
{
	u_int64_t sum = 0;
	unsigned char high = 0;
	unsigned char low = 0;
	unsigned char val;
	int i = 0, j = 0;

	u_int64_t tmp = 0xBC00000000;

	int e = 10;

	if(!buf || !out) {
		return RS_ERROR;
	}

	*out = 0;
	for(i = 0; i < 6; i++) {
		val = buf[i];
		low = val % 16;
		high = val / 16;
		tmp = (low + 16 * high);
		for(j = 0; j < e; j++) {
			tmp = tmp * 16;
		}
		e = e - 2;
		sum = sum + tmp ;
	}

	*out = sum;
	return  RS_OK;

}

static void ICACHE_FLASH_ATTR lcc_print_dev(void)
{
	char buf[128] = {0};

	LIC_ERROR("active:%d\n", g_dev.active);
	LIC_ERROR(buf, "%04u%04u%04u", (u_int32_t)(g_dev.sn / 100000000), (u_int32_t)((g_dev.sn / 10000) % 10000),
	          (u_int32_t)(g_dev.sn % 10000));
	LIC_ERROR("sn:%s\n", buf);
	LIC_ERROR("pwd:%s\n", g_dev.df_pwd);

}

RS ICACHE_FLASH_ATTR lcc_read(void)
{
	lcc_nvm_key_t nvm_key;
	lcc_key_t *key;

	lcc_init();

	if(flash_read_new(SPI_FLASH_LICENSE, sizeof(lcc_nvm_key_t), (u_int8_t *)&nvm_key) != true) {
		g_dev.active = 0;
		return RS_OK;
	}

	if (nvm_key.magic != LCC_MAGIC || ! nvm_key.valid) {
		g_dev.active = 0;
		return RS_OK;
	}

	key = &nvm_key.key;
	g_dev.active = 1;
	lcc_sn_hex_to_u64((char*)key->sn, &g_dev.sn);

	g_dev.dev_type = key->dev_type;
	g_dev.ext_type = key->ext_type;

	os_memcpy(g_dev.mac, key->mac, LCC_MAC_LEN);
	os_strcpy((char*)g_dev.df_pwd, (char*)key->df_pwd);
	os_strcpy((char*)g_dev.vendor, (char*)key->vendor);
	os_memcpy(g_dev.preshare_key, key->preshare_key, LCC_PRESHARE_KEY_LEN);
	lcc_print_dev();
	return RS_OK;
}

RS ICACHE_FLASH_ATTR lcc_write(lcc_nvm_key_t * nvm_key)
{
	int len;
	UINT8 *pbuf;

	if (NULL == nvm_key) {
		return RS_ERROR;
	}

	len = sizeof(lcc_nvm_key_t);
	pbuf = (UINT8 *)nvm_key;
	if (flash_write(SPI_FLASH_LICENSE, len, pbuf) == false) {
		LIC_DEBUG("lcc_write failed\n");
		return RS_ERROR;
	}

	return RS_OK;
}

bool ICACHE_FLASH_ATTR dev_is_active(void)
{
	return g_dev.active;
}

