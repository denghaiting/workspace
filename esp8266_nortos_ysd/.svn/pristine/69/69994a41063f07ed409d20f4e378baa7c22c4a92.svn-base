 /******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_plug.c
 *
 * Description: plug demo's function realization
 *
 * Modification history:
 *     2014/5/1, v1.0 create this file.
*******************************************************************************/

#include "iw_priv.h"


void * ICACHE_FLASH_ATTR 
iw_malloc(u_int32_t size)
{
	return (void*)os_malloc(size);
}

void * ICACHE_FLASH_ATTR
iw_calloc(u_int32_t size, u_int32_t n)
{
	return (void*)os_zalloc(size*n);
}
void ICACHE_FLASH_ATTR
iw_free(void *p)
{
	if(p)
		os_free(p);
}


