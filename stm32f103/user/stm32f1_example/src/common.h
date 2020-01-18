#ifndef __COMMON_H__
#define __COMMON_H__

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include "main.h"
#include <stdio.h>

extern uint8_t imemcmp(uint8_t* dst, uint8_t* src, uint8_t len);
extern void imemcpy(uint8_t* dst, uint8_t* src, uint8_t len);
extern void imemset(uint8_t *s, uint8_t c, uint8_t count);
extern void uart1_init(uint32_t baud);

#endif


