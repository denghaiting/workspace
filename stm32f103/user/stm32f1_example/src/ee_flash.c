#include "ee_flash.h"

static gw_cmd_t gw_cmd;
static gw_cmd_t gw;

void ee_flash_read_n(uint32_t addr, uint32_t *buff, uint32_t len)
{
	memcpy((uint8_t*)buff, (uint8_t*)addr, len);
}

int ee_flash_erase_pages(uint32_t page_address, uint8_t page_num)
{
	uint8_t idx;
	uint32_t status;
	
	flash_unlock();
	for(idx=0; idx<page_num; idx++,page_address+=FLASH_PAGE_SIZE) {
		flash_erase_page(page_address);
		status =  flash_get_status_flags();
		if(status != FLASH_SR_EOP) {
			return 0;
		}
	}
	return 1;
}

uint32_t ee_flash_program_data(uint32_t start_address, uint8_t *input_data, uint16_t num_elements)
{
	uint16_t idx;
	uint32_t status;
	
	flash_unlock();
	for(idx=0; idx<num_elements; idx+=4) {
		flash_program_word(start_address+idx, *(uint32_t*)(input_data+idx));
		status =  flash_get_status_flags();
		if(status != FLASH_SR_EOP) {
			return 1;
		}
		if (*(uint32_t*)(start_address+idx) != *(uint32_t*)(input_data+idx)) {
			return 0;
		}
	}
	return 1;
}

void ee_flash_example(void)
{
	gw_cmd.cmd =0xfd;
	gw_cmd.seq = 0x32;
	gw_cmd.crc = 0x1f2c;
	gw_cmd.id = 0x12345678;
	imemset(rx_buf, 0,sizeof(rx_buf));
	imemset((uint8_t*)&gw, 0, sizeof(gw));
	ee_flash_erase_pages(ACTIVE_ADDR, 1);
	ee_flash_program_data(ACTIVE_ADDR, (uint8_t*)&gw_cmd, sizeof(gw_cmd));
	ee_flash_read_n(ACTIVE_ADDR, (uint32_t*)rx_buf, sizeof(gw_cmd));
	ee_flash_read_n(ACTIVE_ADDR, (uint32_t*)&gw, sizeof(gw_cmd));
	debug_out("rx_buf:", rx_buf, sizeof(gw_cmd));
	printf("gw.cmd=%d\r\n", gw.cmd);
	printf("gw.seq=%d\r\n", gw.seq);
	printf("gw.crc=%d\r\n", gw.crc);
	printf("gw.id=%d\r\n", gw.id);
}

