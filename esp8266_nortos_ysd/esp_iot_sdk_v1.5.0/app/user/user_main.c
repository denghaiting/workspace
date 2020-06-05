/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: ħ��
**  File:    user_main.c
**  Author:  fangfuhan
**  Date:    04/08/2016
**
**  Purpose:
**    ��ں���.
**************************************************************************/

/* Include files. */
#include "iw_reset_already_finish.h"
#include "ets_sys.h"
#include "osapi.h"
#include "driver/uart.h"
#include "user_interface.h"
#include "ds_proto.h"
#include "udp_ctrl.h"
#include "iw_api.h"
#include "iw_priv.h"
#include "iw_smartconfig.h"
#include "licence.h"
#include "iw_sys_time.h"
#include "iw_uart_ijuartpro.h"


/* Macro constant definitions. */



/* Type definitions. */

/* Local function declarations. */


/* Macro API definitions. */
//#define USERIN_DEBUG_
#ifdef USERIN_DEBUG_
#define USERIN_INFO(x...)   INFO(x)
#define USERIN_DEBUG(x...)  DEBUG(x)
#define USERIN_ERROR(x...)  ERROR(x)
#else
#define USERIN_DEBUG(x...)
#define USERIN_INFO(x...)	INFO(x)
#define USERIN_ERROR(x...)  ERROR(x)
#endif



/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
 
    iw_init_t wii;
   
    uart_init(BIT_RATE_115200, BIT_RATE_115200);
    select_frequency();
    USERIN_INFO("SDK version:%s\n", system_get_sdk_version());

    if(wifi_get_opmode() != STATION_MODE){	
		volatile int ret = wifi_set_opmode(STATION_MODE);
		ret = ret;
        USERIN_INFO("set wifi to station mode return %d\n", ret);
    }
	
	if (RS_OK == lcc_read()) {//��������ʱҪ�ж�active��־�� ���Էŵ�ǰ���ȳ�ʼ��һ��
		if (false == wifi_set_macaddr(STATION_IF, g_dev.mac)) {
			USERIN_ERROR("set mac failed\n");
		}
	}

	//����������ó�ʼ������ǰ��
	conf_fix_init();

	//���һ���Ƿ���Ҫ�����ָ�����
	check_reset_state();

	conf_wifi_init();
    conf_user_init();
    conf_dev_init();
    
	uuid_read();

	os_strcpy(wii.desc, "iwos_dev");
	os_strcpy(wii.vendor, "galaxywind");

	//�ڴ�ϵͳ��һЩֵ�ĳ�ʼ��
	init_mu_sys_config();	
	
	// iwos ��ʼ��
	iw_sys_init(&wii);
	
}
