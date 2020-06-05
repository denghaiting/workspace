/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: NL6621
**  File:    version.h
**  Author:  cailiyang
**  Date:    12/23/2014
**
**  Purpose:
**    Version.
**************************************************************************/


#ifndef VERSION_H
#define VERSION_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */

#define VERTION(maj, min, rev)		((maj&0xff) << 24| (min&0xff) << 16| (rev&0xff) <<8)

/*
*	SOFT_VER_STRING SOFT_VER  UPGRADE_SOFT_VER �������궨��İ汾����һ�¡�
*	���� SOFT_VER_STRING ��������ͷ��ʱ��Ҫ����Ϣ�� SOFT_VER�Ǿ����ڲ��İ汾��
*
*/
#define SOFT_VER_STRING  "1.1.16"
#define SOFT_VER	VERTION(1,1,16)
#define UPGRADE_SOFT_VER	VERTION(1,1,16)


#define WIFI_SDK_VER	VERTION(1,5,0)
#define HARD_VER	VERTION(1,0,0)

#define MAJ_VER(x)    	(((x)>>24)&0xff)
#define MIN_VER(x)    	(((x)>>16)&0xff)
#define REV_VER(x)    	(((x)>>8)&0xff)

/* Type definitions. */


/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* VERSION_H */
