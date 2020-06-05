/*
 * Copyright (c) 2010 Espressif System
 */

#ifndef _OSAPI_H_
#define _OSAPI_H_

#include <string.h>
#include "user_config.h"
#include "driver/uart.h"
#include "os_type.h"

#if 1
typedef void (*func)(char c);
typedef void (* call_func)(void *timer_arg);


extern void ets_timer_disarm(os_timer_t *crc_timer);
extern void ets_timer_setfn(os_timer_t *crc_timer, call_func func, void *arg);
extern void ets_timer_arm_new(os_timer_t *crc_timer, size_t n, size_t time, size_t t);
extern int ets_sprintf( const char *format , ... );
int os_printf_plus(const char *format , ...);
extern void ets_install_putc1(func f);
extern void ets_delay_us(int t);
#endif

#if 0
extern int ets_memcmp(const void *s1, const void *s2, size_t n);
extern int ets_strncmp(const char *s1, const char *s2, size_t n);
extern size_t ets_strlen(const char *s);

extern void *ets_memcpy(void *dest, const void *src, size_t n);
extern void *ets_memmove(void *dest, const void *src, size_t n);
extern void *ets_memset(void *s, int c, size_t n);

extern int ets_strcmp(const char *s1, const char *s2);
extern char *ets_strcpy(char *dest, const char *src);
extern char *ets_strncpy(char *dest, const char *src, size_t n);
extern char *ets_strstr(const char *haystack, const char *needle);
extern void ets_bzero(void *s, size_t n);

extern char *strcat(char *dest, const char *src);
extern char *strchr(const char *s, int c);
#define os_bzero ets_bzero
#define os_delay_us ets_delay_us
#define os_install_putc1 ets_install_putc1

#define os_memcmp ets_memcmp
#define os_memcpy ets_memcpy
#define os_memmove ets_memmove
#define os_memset ets_memset
#define os_strcat strcat
#define os_strchr strchr
#define os_strcmp ets_strcmp
#define os_strcpy ets_strcpy
#define os_strlen ets_strlen
#define os_strncmp ets_strncmp
#define os_strncpy ets_strncpy
#define os_strstr ets_strstr
#else

#define os_bzero bzero
#define os_delay_us ets_delay_us
#define os_install_putc1 ets_install_putc1

#define os_memcmp memcmp
#define os_memcpy memcpy
#define os_memmove memmove
#define os_memset memset
#define os_strcat strcat
#define os_strchr strchr
#define os_strcmp strcmp
#define os_strcpy strcpy
#define os_strlen strlen
#define os_strncmp strncmp
#define os_strncpy strncpy
#define os_strstr strstr
#endif

#ifdef USE_US_TIMER
#define os_timer_arm_us(a, b, c) ets_timer_arm_new(a, b, c, 0)
#endif
#define os_timer_arm(a, b, c) ets_timer_arm_new(a, b, c, 1)
#define os_timer_disarm ets_timer_disarm
#define os_timer_setfn ets_timer_setfn

#define os_sprintf  ets_sprintf

#ifdef USE_OPTIMIZE_PRINTF
#define os_printf(fmt, ...) do {	\
	static const char flash_str[] ICACHE_RODATA_ATTR STORE_ATTR = fmt;	\
	os_printf_plus(flash_str, ##__VA_ARGS__);	\
	} while(0)
#else
#define os_printf	os_printf_plus
#endif

unsigned long os_random(void);
int os_get_random(unsigned char *buf, size_t len);

#endif

