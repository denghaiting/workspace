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
#include "osapi.h"
#include <stdio.h>
#include <stdarg.h>
#include "iw_debug.h"
#include "iw_debug_net.h"

/* Macro constant definitions. */
#define IW_DEBUG_MAX_BUF  (256)

/* Type definitions. */


/* Local function declarations. */
#ifndef HAS_NO_PRINTF
static void iw_debug_vprintf(char *fmt, va_list ap);
#endif
/* Macro API definitions. */

/* Global variable declarations. */
int ets_vsprintf(char *str, const char *format, va_list argptr);
int ets_vsnprintf(char *buffer, size_t sizeOfBuffer,  const char *format, va_list argptr);



void ICACHE_FLASH_ATTR NST_DbgTimestamp(void)
{
    u_int32_t tv;

	//tv = get_up_msec();
    tv = get_up_sec();
	os_printf("[%u] |",tv);
}

void ICACHE_FLASH_ATTR  DBGPRINT_MGMT(u_int8_t level, char *fmt, ...)
{
    va_list ap;

	if (level >= DEBUG_ALL) {
		return;
	} 
	
	va_start(ap, fmt);

	/* 网络打印 */
	iw_debug_net(level, fmt, ap);
	
	/*通过串口打印调试*/
#ifndef HAS_NO_PRINTF
	NST_DbgTimestamp();
	iw_debug_vprintf(fmt, ap);
#endif

	va_end(ap); 

}

#ifndef HAS_NO_PRINTF
static void ICACHE_FLASH_ATTR iw_debug_vprintf(char *fmt, va_list ap)
{
	static char buf[IW_DEBUG_MAX_BUF] = {0};
	
	ets_vsnprintf(buf, IW_DEBUG_MAX_BUF, fmt, ap);

	os_printf("%s", buf);
}
#endif


#if  (DEBUG_LEVEL > 2)
void ICACHE_FLASH_ATTR hex_dump(char *str, unsigned char *pSrcBufVA, unsigned int SrcBufLen)
{
	unsigned char *pt;
	int x;

	pt = pSrcBufVA;
	os_printf("%s: %p, len = %d\n", str, pSrcBufVA, SrcBufLen);
	for (x = 0; x < SrcBufLen; x++) {
		if (x % 16 == 0)
			os_printf("0x%04x : ", x);
		os_printf("%02x ", ((unsigned char)pt[x]));
		if (x % 16 == 15)
			os_printf("\n");
	}
	os_printf("\n");
}

#endif


