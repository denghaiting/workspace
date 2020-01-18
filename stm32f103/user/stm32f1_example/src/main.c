#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/scb.h>
#include <stdio.h>
#include "main.h"

uint8_t rx_buf[32];
uint8_t testbuf[9]= {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99};

uint8_t read_buf[9];
void delay(uint32_t us)
{
	while(us--){
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
	}
}


void gpio_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);	
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

void SystemInit(void)
{
	rcc_clock_setup_in_hsi_out_48mhz();
	gpio_init();
	uart1_init(115200);
	IIC_Init();
}
int main(void)
{
	uint32_t tick = 0;
	
	SystemInit();
	if(AT24CXX_Check()) {
		PrintString("iic check error\r\n");
	}
	else {
		PrintString("iic check right\r\n");
	}
	while(1) {
		//ee_flash_example();
		tick++;
		gpio_set(GPIOC, GPIO13);
		delay(800000);
		gpio_clear(GPIOC, GPIO13);
		delay(800000);
		//PrintString("hello stm32f1\r\n");
		//debug_out("tick:", (uint8_t*)&tick, 4);
		AT24CXX_Write(0, testbuf, sizeof(testbuf));
		AT24CXX_Read(0, read_buf,sizeof(testbuf));
		debug_out("read_buf:", read_buf, 9);
		imemset(read_buf, 0, sizeof(read_buf));
	}
}
