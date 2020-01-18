#ifndef __EE_FLASH_H__
#define __EE_FLASH_H__

#include <libopencm3/stm32/flash.h>
#include "main.h"

#define ACTIVE_ADDR				0x0800FC00
#define FLASH_PAGE_SIZE			(1024)

typedef struct {
	uint8_t cmd;
	uint8_t seq;
	uint16_t crc;
	uint32_t id;
}gw_cmd_t;

extern void ee_flash_read_n(uint32_t addr, uint32_t *buff, uint32_t len);
extern int ee_flash_erase_pages(uint32_t page_address, uint8_t page_num);
extern uint32_t ee_flash_program_data(uint32_t start_address, uint8_t *input_data, uint16_t num_elements);

#endif
