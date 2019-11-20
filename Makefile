
CC = xc8-cc
CHIP = 12f675
PG = ardpicprog
DIST = ./out
OSCCAL = FF 
CCARGS = -ansi -mc90lib -Os -fasmfile -fshort-float -mno-osccal
all: main

clean: 
	rm $(DIST)/*

main: main.c max7219.c
	$(CC) $(CCARGS) -moscval=$(OSCCAL) -mcpu=$(CHIP) -o $(DIST)/main.hex main.c

program: clean main
	$(PG) --quiet --pic-serial-port /dev/ttyUSB0 --erase --burn --force-calibration --input-hexfile $(DIST)/main.hex


	#$(PG) --quiet --pic-serial-port /dev/ttyUSB0 --erase --burn --force-calibration --input-hexfile $(DIST)/main.hex
	
p:
	$(PG) --quiet --pic-serial-port /dev/ttyUSB0 --erase --burn --force-calibration --input-hexfile OUTPUT.hex 


read:
	$(PG) --quiet --pic-serial-port /dev/ttyUSB0 --ihx8m --output-hexfile OUTPUT 


.PHONY: program clean
