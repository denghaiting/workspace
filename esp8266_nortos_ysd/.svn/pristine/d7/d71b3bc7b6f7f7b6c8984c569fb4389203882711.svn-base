/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: mu
**  File:    uuid.c
**  Author:  cailiyang
**  Date:    08/08/2014
**
**  Purpose:
**    Uuid.
**************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "iw_user_config.h"


/* Macro constant definitions. */
#define UUID_MAGIC  0x55554944 /* UUID*/

/* Type definitions. */


/* Local function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */

nvm_uuid_t g_uuid;


RS uuid_write(u_int8_t *uuid, u_int32_t uuid_len)
{
//	int i,page;
	u_int32_t len;
	UINT8 *pbuf;

	if(NULL == uuid) {
		return RS_ERROR;
	}

	g_uuid.magic = UUID_MAGIC;
	g_uuid.valid = TRUE;
	os_memcpy(g_uuid.uuid, uuid, uuid_len);

	pbuf = (UINT8*)&g_uuid;

	len = sizeof(nvm_uuid_t);

	if (flash_write(SPI_FLASH_UUID, len, pbuf) == false) {
		ERROR("uuid_write failed\n");
		return RS_ERROR;
	}

	return RS_OK;
}

RS uuid_read(void)
{
	u_int32_t len;
	nvm_uuid_t uuid;

	len = sizeof(nvm_uuid_t);

	flash_read_new(SPI_FLASH_UUID, len, (u_int8_t*)&uuid);

	if ((uuid.magic != UUID_MAGIC) || (!uuid.valid)){
		g_uuid.magic = 0;
		g_uuid.valid = FALSE;
		return RS_ERROR;
	}

	os_memcpy(&g_uuid, (u_int8_t*)&uuid, len);

	hex_dump("uuid", (unsigned char *)g_uuid.uuid, 16);

	return RS_OK;
}

