#ifndef __MY_IIC_H__
#define __MY_IIC_H__

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "main.h"

#define SDA_IN()  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_INPUT_FLOAT, GPIO7)
#define SDA_OUT() gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO7)

//IO²Ù×÷º¯Êý 
#define IIC_SCL_LOW		gpio_clear(GPIOB, GPIO6)
#define IIC_SCL_HIGH	gpio_set(GPIOB, GPIO6)
//#define IIC_SDA   OUT
#define IIC_SDA_LOW		gpio_clear(GPIOB, GPIO7)
#define IIC_SDA_HIGH	gpio_set(GPIOB, GPIO7)

#define READ_SDA   get_iovalue()
extern void delay_ms(uint32_t ms);


#endif
