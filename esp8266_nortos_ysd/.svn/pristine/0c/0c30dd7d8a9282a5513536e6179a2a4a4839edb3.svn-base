#ifndef __IWUOS_PLATFORM_HEADER__
#define __IWUOS_PLATFORM_HEADER__

#define PLT_8266 0x101
#define PLT_6621 0x102

#define IW_PLATFORM PLT_8266


#if (IW_PLATFORM==PLT_8266)

#include "iw_8266.h"

#elif (IW_PLATFORM==PLT_6621)

#include "iw_6621.h"

#else
#error "Please define IW_PLATFORM"
#endif


#endif
