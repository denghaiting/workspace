/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: ħ��
**  File:    domain.h
**  Author:  fangfuhan
**  Date:    06/12/2016
**
**  Purpose:
**    Dns����.
**************************************************************************/


#ifndef DOMAIN_H
#define DOMAIN_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */


/* Macro constant definitions. */


/* Type definitions. */
/* �������� */
typedef enum{	
	DM_NONE,  /* ��֪���� */	
	DM_PROBE, /* �Զ�̽�� */	
	DM_SET,	  /* �ⲿ���� */
	DM_MAX,
}dm_type_e;

/* External function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */
char *domain_get_name(void);
int domain_set_name(char *domain, int len, dm_type_e type);
void  domain_clear_ip(void);
int domain_set_ip(u_int32_t *ip, int ip_num);
u_int32_t domain_get_ip();
u_int32_t domain_get_next_ip();
void domain_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* DOMAIN_H */

