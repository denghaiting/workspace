/**************************************************************************
**  Copyright (c) 2010 GalaxyWind, Ltd.
**
**  Project: iw_uart.c
**  File:    iw_uart.c
**  Author:  
**  Date:    06/09/2015
**
**  Purpose:
**    I+ common serial proc.
**************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "iw_uart_active.h"
#include "iw_pt_uart.h"
#include "iw_uart_ijuartpro.h"
#include "iw_event.h"
#include "iw_pt_uart.h"

/* Macro constant definitions. */


/* Type definitions. */


/* Local function declarations. */


/* Macro API definitions. */


/* Global variable declarations. */
save_info_t save_info;

extern void  BSP_ActiveUartPutstr(const char *str);


static rs_t ICACHE_FLASH_ATTR iw_uart_2comm(iw_event_info_t *INOUT_info)
{

	if(INOUT_info == NULL || INOUT_info->event != IW_EV_UARTR_2COMMON) {
		BSP_ActiveUartPutstr("not find event IW_EV_UARTR_2COMMON \n");
		return RS_ERROR;
	}
	
	iw_event_call(IW_EV_UARTR_2ACTIVE, INOUT_info);
	iw_event_call(IW_EV_UARTR_2PRETEST, INOUT_info);
	iw_event_call(IW_EV_UARTR_2IJPROTO, INOUT_info);

	return RS_OK;
}

void ICACHE_FLASH_ATTR iw_uart_event_init(void)
{
#if 0
    flash_read_new(macbeesave_offset, 2, (uint8_t*)&save_info);
    if (save_info.reset_en == 0xac) {
        save_info.reset_en = 0;
        flash_write(macbeesave_offset, 2, (uint8_t*)&save_info);
        if (save_info.freq_flag == 0xae) {
            save_info.freq_flag = 0x00;	
            flash_write(macbeesave_offset, 2, (uint8_t*)&save_info);
            uart_init(BIT_RATE_28800, BIT_RATE_28800); 
        }
    }
#endif
    iw_event_register(IW_EV_UARTR_2COMMON, iw_uart_2comm);

	//串口激活模块
	iw_uart_active_init();
	// 生产测试模块
	iw_uart_pretest_init();

	//具体产品串口接收处理函数，led灯项目还没有使用
	iw_uart_ijuartproto_init();
}


