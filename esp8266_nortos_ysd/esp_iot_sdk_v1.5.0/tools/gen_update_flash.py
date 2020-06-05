#!/usr/bin/python

import string
import sys
import os
import re
import binascii
import struct
import zlib

def write_file(file_name,data):
	if file_name is None:
		print 'file_name cannot be none\n'
		sys.exit(0)

	fp = open(file_name,'ab')

	if fp:
		fp.seek(0,os.SEEK_END)
		fp.write(data)
		fp.close()
	else:
		print '%s write fail\n'%(file_name)


def making_update_bin():
	update_bin_name='update.bin'

	data_len_count=0x1000
	data_str = ['FF']*data_len_count
	data_bin = binascii.a2b_hex(''.join(data_str))
	write_file(update_bin_name,data_bin)
	#print data_len_count
	
	fp = file('../bin/OUTPUT/user1.2048.new.3.bin')
	if fp is None:
		print "open sym file error\n"
		sys.exit(0)		
	fp.seek(0,os.SEEK_END)
	data_len = fp.tell()	
	#print data_len	
	fp.seek(0,os.SEEK_SET)
	data_bin = fp.read(data_len)
	write_file(update_bin_name,data_bin)
	fp.close()

	data_len_count = 0x81000-0x1000-data_len
	data_str = ['FF']*data_len_count
	data_bin = binascii.a2b_hex(''.join(data_str))
	write_file(update_bin_name,data_bin)	
	#print data_len_count

	fp = file('../bin/OUTPUT/user2.2048.new.3.bin')
	if fp is None:
		print "open sym file error\n"
		sys.exit(0)	
	fp.seek(0,os.SEEK_END)
	data_len = fp.tell()	
	#print data_len	
	fp.seek(0,os.SEEK_SET)
	data_bin = fp.read(data_len)
	write_file(update_bin_name,data_bin)
	fp.close()
	
def making_flash_bin():
	spi_mode = sys.argv[2]
	flash_size = sys.argv[3]
	spi_speed = sys.argv[1]	
	flash_bin_name='flash.bin'

	fp = file('../bin/boot_v1.4(b1).bin')
	if fp is None:
		print "open sym file error\n"
		sys.exit(0)		
	fp.seek(0,os.SEEK_END)
	data_len = fp.tell()	
	#print data_len	
	fp.seek(0,os.SEEK_SET)
	data_bin = fp.read(2)
	write_file(flash_bin_name,data_bin)
	fp.close()
	
	#print spi_mode
	if spi_mode == 'QIO':
		mode = 0x0
	elif spi_mode == 'QOUT':
		mode = 0x1
	elif spi_mode == 'DIO':
		mode = 0x2
	else:
		mode = 0x3		
	#print spi_speed
	if spi_speed == '40':
		speed = 0x0
	elif spi_speed == '26.7':
		speed = 0x1
	elif spi_speed == '20':
		speed = 0x2
	else:
		speed = 0xF
	#print 'mode == %d, speed==%d, flash_size==%d'%(int(mode), int(speed), int(flash_size))	
	byte2=int(mode)&0xff
	byte3=(((int(flash_size)<<4)| int(speed))&0xff)	
	data_bin = struct.pack('<BB',byte2,byte3)
	write_file(flash_bin_name,data_bin)	

	fp = file('../bin/boot_v1.4(b1).bin')
	if fp is None:
		print "open sym file error\n"
		sys.exit(0)		
	fp.seek(0,os.SEEK_END)
	data_len = fp.tell()	
	#print data_len	
	fp.seek(4)
	data_bin = fp.read(data_len-4)
	write_file(flash_bin_name,data_bin)
	fp.close()
	
	sum_size = 0x1000-data_len
	data_str = ['FF']*sum_size
	data_bin = binascii.a2b_hex(''.join(data_str))
	write_file(flash_bin_name,data_bin)	
	#print sum_size

	fp = file('../bin/OUTPUT/user1.2048.new.3.bin')
	if fp is None:
		print "open sym file error\n"
		sys.exit(0)		
	fp.seek(0,os.SEEK_END)
	data_len = fp.tell()	
	#print data_len	
	fp.seek(0,os.SEEK_SET)
	data_bin = fp.read(data_len)
	write_file(flash_bin_name,data_bin)
	fp.close()	

	data_len_count = 0x81000-0x1000-data_len
	data_str = ['FF']*data_len_count
	data_bin = binascii.a2b_hex(''.join(data_str))
	write_file(flash_bin_name,data_bin)	
	#print data_len_count

	fp = file('../bin/OUTPUT/user2.2048.new.3.bin')
	if fp is None:
		print "open sym file error\n"
		sys.exit(0)	
	fp.seek(0,os.SEEK_END)
	data_len = fp.tell()	
	#print data_len	
	fp.seek(0,os.SEEK_SET)
	data_bin = fp.read(data_len)
	write_file(flash_bin_name,data_bin)
	fp.close()
	
	data_len_count = 0x1fc000-0x81000-data_len
	data_str = ['FF']*data_len_count
	data_bin = binascii.a2b_hex(''.join(data_str))
	write_file(flash_bin_name,data_bin)	
	#print data_len_count	
	
	fp = file('../bin/esp_init_data_default.bin')
	if fp is None:
		print "open sym file error\n"
		sys.exit(0)	
	fp.seek(0,os.SEEK_END)
	data_len = fp.tell()	
	#print data_len	
	fp.seek(0,os.SEEK_SET)
	data_bin = fp.read(data_len)
	write_file(flash_bin_name,data_bin)
	fp.close()	
	
	data_len_count = 0x1fe000-0x1fc000-data_len
	data_str = ['FF']*data_len_count
	data_bin = binascii.a2b_hex(''.join(data_str))
	write_file(flash_bin_name,data_bin)	
	#print data_len_count	

	fp = file('../bin/blank.bin')
	if fp is None:
		print "open sym file error\n"
		sys.exit(0)	
	fp.seek(0,os.SEEK_END)
	data_len = fp.tell()	
	#print data_len	
	fp.seek(0,os.SEEK_SET)
	data_bin = fp.read(data_len)
	write_file(flash_bin_name,data_bin)
	fp.close()	

def gen_appbin():
	print 'making flash.bin'
	making_flash_bin()
	
	print 'making update.bin'
	making_update_bin()
	
if __name__=='__main__':
    gen_appbin()
	