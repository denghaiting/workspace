#!/bin/bash

USER_SPI_SPEED=40
USER_SPI_MODE=QIO
USER_SPI_SIZE_MAP=3

make clean
make APP=1 SPI_SPEED=$USER_SPI_SPEED SPI_MODE=$USER_SPI_MODE SPI_SIZE_MAP=$USER_SPI_SIZE_MAP
if [ $? -ne 0 ]
then
	echo "make user1 error"
	exit
else
	echo "make user1 success!"
fi

make clean
make APP=2 SPI_SPEED=$USER_SPI_SPEED SPI_MODE=$USER_SPI_MODE SPI_SIZE_MAP=$USER_SPI_SIZE_MAP
if [ $? -ne 0 ]
then
	echo "make user2 error"
	exit
else
	echo "make user2 success!"
fi

cd tools
python gen_update_flash.py $USER_SPI_SPEED $USER_SPI_MODE $USER_SPI_SIZE_MAP

if [ -f "update.bin" ]
then
    echo "update.bin making success!"
	cp update.bin ../bin/OUTPUT/update.bin
	rm update.bin
else
    echo "update.bin making faid!"
fi

if [ -f "flash.bin" ]
then
    echo "flash.bin making success!"
	cp flash.bin ../bin/OUTPUT/flash.bin
	rm flash.bin
else
    echo "flash.bin making faid!"
fi

