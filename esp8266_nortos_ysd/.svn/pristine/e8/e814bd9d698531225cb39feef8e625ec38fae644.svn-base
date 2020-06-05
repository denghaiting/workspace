#ifndef __IWUOS_DEBUG_HEADER__
#define __IWUOS_DEBUG_HEADER__

#define DEBUG_ERROR     1
#define DEBUG_WARN      2
#define DEBUG_TRACE     2
#define DEBUG_INFO      2
#define DEBUG_DEBUG     3
#define DEBUG_ALL      4

#define DEBUG_LEVEL 2
//#define HAS_NO_PRINTF
#ifdef HAS_NO_PRINTF
#define UART_PRI_PORT 1
#else
#define UART_PRI_PORT 0
#endif


#ifndef HAS_NO_PRINTF
#define _DBGPRINT_MGMT(level, fmt...) do{\
	if (level >= DEBUG_ALL) {break;}\
	u_int32_t time;\
	time = get_sec();\
	os_printf_plus("%d |", time);\
	os_printf_plus(fmt);\
}while(0);
#else
#define _DBGPRINT_MGMT(level, fmt...)
#endif

void  DBGPRINT_MGMT(u_int8_t level, char *fmt, ...);

#if (DEBUG_LEVEL == 0) /* 发布版 */

#define DEBUG_LINE()

#define	DEBUG(fmt...)
#define	INFO(fmt...)
#define	ERROR(fmt...)
#define	DBGPRINT(level, fmt...)
#define	hex_dump(str, addr, len)

#elif (DEBUG_LEVEL == 1) /* 只打印错误 */

#define DEBUG_LINE()

#define	DEBUG(fmt...)
#define	INFO(fmt, ...)
#define	ERROR(fmt...)	DBGPRINT_MGMT(DEBUG_ERROR, fmt)
#define	DBGPRINT(level, fmt...)	DBGPRINT_MGMT(DEBUG_ERROR, fmt)
#define	hex_dump(str, addr, len)

#elif (DEBUG_LEVEL == 2) /* 打印错误和重要信息 */

#define DEBUG_LINE()

#define	DEBUG(fmt...)
#define	INFO(fmt...)	DBGPRINT_MGMT(DEBUG_INFO, fmt)
#define	ERROR(fmt...)	DBGPRINT_MGMT(DEBUG_ERROR, fmt)
#define	DBGPRINT(level, fmt...)	DBGPRINT_MGMT(DEBUG_ERROR, fmt)
#define	hex_dump(str, addr, len)

#else /* 全打印调试版 */

#define	DBG_UDP_CTRL_PKT	1

extern void hex_dump(char *str, unsigned char *pSrcBufVA, unsigned int SrcBufLen);

#define DEBUG_LINE()		os_printf_plus("enter func=%s line=%d\r\n", __FUNCTION__, __LINE__);

#define	DEBUG(fmt...) DBGPRINT_MGMT(DEBUG_DEBUG, fmt)
#define	INFO(fmt...)	DBGPRINT_MGMT(DEBUG_INFO, fmt)
#define	ERROR(fmt...)	DBGPRINT_MGMT(DEBUG_ERROR, fmt)
#define	DBGPRINT(level, fmt...)	DBGPRINT_MGMT(DEBUG_ERROR, fmt)

#endif

#define ASSERT_ON         0   // control assert on/off

#if ASSERT_ON
#define ASSERT(exp) do { if(!(exp))   DBGPRINT_MGMT("!!! ERR: %s(%d) ASSERT FAILED!\n", __FILE__, __LINE__);} while (0)
#else
/* ASSERT could cause segmentation fault on GCC3.1, use empty instead */
#define ASSERT(exp)
#endif              /* GCC_VERSION > 30100 */

#endif
