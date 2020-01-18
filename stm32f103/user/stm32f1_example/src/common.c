#include "common.h"

void imemset(uint8_t *s, uint8_t c, uint8_t count)
{
	while (count--) {
		*s++ = c;
	}
}

void imemcpy(uint8_t* dst, uint8_t* src, uint8_t len)
{
	while (len--) {
		dst[len] = src[len];
	}
}

uint8_t imemcmp(uint8_t* dst, uint8_t* src, uint8_t len)
{
	while (len--) {
		if(dst[len] != src[len])
			return 1;
	}
	return 0;
}

int fputc(int ch, FILE *f)
{
    usart_send_blocking(USART1,ch);//向串口1打印
    return ch;
}

void PrintString(uint8_t * fmt)
{
	uint8_t i;

	for (i = 0; fmt[i]; i++) {
		usart_send_blocking(USART1,fmt[i]);
	}
}

#if 0
void printbuf(uint8_t * buf, int len)
{
	uint8_t tmp;
	int i;

	for (i = 0; i < len; i++) {
		tmp = buf[i];
		tmp = tmp >> 4;
		tmp = (tmp & 0xf) + 48;
		if(tmp > '9') {
			tmp += 'A' - '9' - 1;
		}
		usart_send_blocking(USART1,tmp);
		tmp = buf[i];
		tmp = (tmp & 0xf) + 48;
		if(tmp > '9') {
			tmp += 'A' - '9' - 1;
		}
		usart_send_blocking(USART1,tmp);
		usart_send_blocking(USART1,' ');		
	}
}

#else
uint8_t chartable[17] = "0123456789abcdef";

void printbuf(uint8_t * buf, uint16_t len)
{
	uint16_t i;

	for (i = 0; i < len; i++) {
		usart_send_blocking(USART1, chartable[(buf[i] & 0xf0) >> 4]);
		usart_send_blocking(USART1, chartable[(buf[i] & 0xf)]);
		usart_send_blocking(USART1, ' ');
	}
}

#endif
void debug_out(uint8_t * prefix, uint8_t * buf, uint8_t len)
{
	if (prefix)
		PrintString(prefix);
	printbuf(buf, len);
	PrintString("\r\n");
	
}

static void uart1_clk_setup(void)
{
	/* Enable GPIOA clock for  USARTs. */
	rcc_periph_clock_enable(RCC_GPIOA);	

	/* Enable clocks for USART1. */	
	rcc_periph_clock_enable(RCC_USART1);
}


static void uart1_gpio_setup(void)
{

	/* Setup GPIO pins for USART1 transmit. */	
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO10);

}

void uart1_init(uint32_t baud)
{
	/*step1: init clk*/
	uart1_clk_setup();
	
	 /*step2: init GPIO*/
	uart1_gpio_setup();
	//RCC_CFGR3 |= RCC_CFGR3_USART1SW_SYSCLK;//选择系统时钟源作为串口时钟，不选默认为PCLK
	/* step3: Setup USART2 parameters. */
	usart_set_baudrate(USART1, baud);//设置波特率
	usart_set_databits(USART1, 8);//8位数据
	usart_set_parity(USART1, USART_PARITY_NONE);//无校验
	usart_set_stopbits(USART1, USART_STOPBITS_1);	//1位停止位
	usart_set_mode(USART1, USART_MODE_TX_RX);//允许串口接收和发送
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);//无流控制
	USART_CR1(USART1) |= USART_CR1_RXNEIE;
	/* step4: enable the USART. */
	usart_enable(USART1);
	
	/* step5: Enable dev Receive interrupt. */		
	usart_enable_rx_interrupt(USART1);
	
	/* step6: Enable and set USART Interrupt */	
	nvic_set_priority(NVIC_USART1_IRQ, 0);
	nvic_enable_irq(NVIC_USART1_IRQ);	
}

