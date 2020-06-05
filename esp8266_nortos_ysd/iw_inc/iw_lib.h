/**************************************************************************
**  Copyright (c) 2010 GalaxyWind, Ltd.
**
**  Project: iw_lib.h
**  File:    iw_lib.h
**  Author:  lgp
**  Date:    06/03/2015
**
**  Purpose:
**    Common function or define.
**************************************************************************/


#ifndef IW_LIB_H
#define IW_LIB_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */


/* Type definitions. */

#undef max
#undef min
#define	max(a, b)	(((a) > (b)) ? (a) : (b))
#define	min(a, b)	(((a) < (b)) ? (a) : (b))

#undef	IS_SAME_STR
#define	IS_SAME_STR(a, b) ((a) == (b) || ((a) != NULL && (b) != NULL && strcmp((a), (b)) == 0))

#undef IP_SHOW
#define IP_SHOW(IP) (((u_int32_t)(IP))>>24)&0xFF, (((u_int32_t)(IP))>>16)&0xFF, (((u_int32_t)(IP))>>8)&0xFF, ((u_int32_t)(IP))&0xFF
#undef MAC_SHOW
#define	MAC_SHOW(MAC) (MAC)[0], (MAC)[1], (MAC)[2], (MAC)[3], (MAC)[4], (MAC)[5]
#undef TM_SHOW
#define	TM_SHOW(tm) tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec


/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */
extern u_int64_t get_random();

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* IW_LIB_H */

