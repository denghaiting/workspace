/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: ħ��
**  File:    domain.c
**  Author:  fangfuhan
**  Date:    06/12/2016
**
**  Purpose:
**    Dns����.
**************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "ice_domain.h"
#include "domain.h"

/* Macro constant definitions.. */
#define DOMAIN_MAX_LENGTH	(64)
#define DOMAIN_STR_LEN  	(256)
#define DOMAIN_MAX_IP		(3)
#define DOMAIN_MAX_IP_INDEX (DOMAIN_MAX_IP - 1)

/* Type definitions. */
typedef struct ice_domain_s{	
	bool valid;						//�����ַ����Ƿ���Ч	
	dm_type_e type;					//�������ͣ��Զ�̽�⻹���ⲿ���õ�	
	u_int32_t len; 					//�����ĳ���	
	char domain[DOMAIN_STR_LEN];	//�����ַ���	
	bool first;						//��һ�λ�ȡ������ip	
	u_int32_t ip_index;				//ip������ֵ	
	u_int32_t ip_num;           	//������Ӧ��IP��	
	u_int32_t ip[DOMAIN_MAX_IP];   	//�����IP
}ice_domain_t;


/* Local function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */

static ice_domain_t g_ice_domain;
extern void srand(unsigned int seed);
extern int rand(void);

void domain_clear_ip(void);

void ICACHE_FLASH_ATTR domain_init(void)
{
	config_t *pconfig = NULL;

	os_memset(&g_ice_domain, 0, sizeof(g_ice_domain));

	//�滻���������õ�
	pconfig = conf_user_get(SET_ICE_DOMAIN);
	if (!pconfig || (0 == pconfig->len) || pconfig->len > DOMAIN_MAX_LENGTH) {
		return;
	}

	/* �ⲿ���������������� */
	g_ice_domain.valid = true;
	g_ice_domain.type = DM_SET;
	g_ice_domain.len = pconfig->len;
	os_memcpy((u_int8_t *)g_ice_domain.domain, pconfig->value, pconfig->len);
}

char * ICACHE_FLASH_ATTR domain_get_name(void)
{
	if(! g_ice_domain.valid)
		return NULL;

	return g_ice_domain.domain;
}


/*
*	���õ���������Ч��������һ��
*/
static bool ICACHE_FLASH_ATTR domain_same(ice_domain_t *ice_domain, char *domain, int len)
{
	if (! ice_domain->valid)
		return false;

	if (ice_domain->len != len)
		return false;

	return ( (os_memcmp((u_int8_t*)ice_domain->domain, (u_int8_t*)domain, len) == 0) ? true : false);
}



/*
*	ע��:
*		���ַ�����������һ���"\0"��ʾ���������domain���ݽ����ĳ��Ⱦ��Ǵ���'\0',����flash����һ���ַ�������Ӱ������
*		�Զ�̽������������б��棬�ⲿ���õ�������Ҫ����
*/
int ICACHE_FLASH_ATTR domain_set_name(char *domain, int len, dm_type_e type)
{
	if (NULL == domain || len > (DOMAIN_STR_LEN - 1))
		return RS_INVALID_PARAM;

	if (domain_same(&g_ice_domain, domain, len)){
		return RS_DOMAIN_SAME;
	}

	g_ice_domain.valid = true;
	g_ice_domain.type = type;
	os_memcpy((u_int8_t*)g_ice_domain.domain, (u_int8_t*)domain, len);
	g_ice_domain.domain[len] = 0;
	g_ice_domain.len = len + 1;

	if (DM_SET == type){
		conf_user_set(SET_ICE_DOMAIN, (u_int8_t*)g_ice_domain.domain, g_ice_domain.len);
	}
	
	domain_clear_ip();

	return RS_OK;
}


void ICACHE_FLASH_ATTR domain_clear_ip(void)
{	
	g_ice_domain.ip_num = 0;	
	os_memset(g_ice_domain.ip, 0, sizeof(g_ice_domain.ip));
}

int ICACHE_FLASH_ATTR domain_set_ip(u_int32_t *ip, int ip_num)
{
	int i = 0;
	
	if (NULL == ip || ip_num <= 0 || ip_num > DOMAIN_MAX_IP)
		return RS_INVALID_PARAM;

	for(i = 0; i < ip_num; i++){
		g_ice_domain.ip[i] = ip[i];
	}

	g_ice_domain.ip_num = ip_num;
	g_ice_domain.ip_index = 0;
	g_ice_domain.first = true;

	return RS_OK;
}

/*
*	index ��0��ʼ�����Ϊ2
*/
u_int32_t ICACHE_FLASH_ATTR domain_get_ip()
{
	u_int32_t index = 0;

	if (g_ice_domain.ip_num == 0)
		return 0;

	index = g_ice_domain.ip_index;
	
	ERROR("domain_get_ip index = [%d] ip = [%u.%u.%u.%u]\r\n", index, IP_SHOW(g_ice_domain.ip[index]));
	return g_ice_domain.ip[index];
}

u_int64_t get_random(void)
{
	srand(get_up_msec());
	return rand();
}

u_int32_t ICACHE_FLASH_ATTR domain_get_next_ip()
{
	u_int32_t index = 0;
	
	if (g_ice_domain.ip_num == 0)
		return 0;


	if (g_ice_domain.first){
		g_ice_domain.first = false;
		//���ֵ
		g_ice_domain.ip_index = get_random()% g_ice_domain.ip_num;
	}else{
		g_ice_domain.ip_index = (g_ice_domain.ip_index + 1) % g_ice_domain.ip_num;
	}

	index = g_ice_domain.ip_index;

	return g_ice_domain.ip[index];
}


dm_type_e ICACHE_FLASH_ATTR domain_get_type()
{
	return g_ice_domain.type;
}

