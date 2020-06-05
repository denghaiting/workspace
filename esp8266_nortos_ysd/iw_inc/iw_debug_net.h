/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: iWulink
**  File:    iw_debug_net.h
**  Author:  cailiyang
**  Date:    03/11/2016
**
**  Purpose:
**    ÍøÂçµ÷ÊÔ.
**************************************************************************/


#ifndef IW_DEBUG_NET_H
#define IW_DEBUG_NET_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include <stdarg.h>
//#include "iw_c_types.h"

/* Macro constant definitions. */


/* Type definitions. */


/* External function declarations. */
void ucds_net_debug_notify_init(void);

void iw_debug_net(u_int8_t level, char *fmt, va_list ap);



/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* IW_DEBUG_NET_H */

