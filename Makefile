
CC = xc8-cc
CHIP = 12f675
PG = ardpicprog
DIST = ./out
OSCCAL = FF 
CCARGS = -ansi -mc90lib -O2 -fasmfile -fshort-float -mno-osccal
SERIALPORT = /dev/ttyUSB0
PGARGS = --quiet --pic-serial-port $(SERIALPORT)


main.hex: main.c
	$(CC) $(CCARGS) -moscval=$(OSCCAL) -mcpu=$(CHIP) -o $(DIST)/main.hex main.c


.PHONY: clean
clean: 
	rm $(DIST)/*


.PHONY: program
program: main.hex
	$(PG) $(PGARGS) --erase --burn --force-calibration --input-hexfile $(DIST)/main.hex


.PHONY: read
read:
	$(PG) $(PGARGS) --ihx8m --output-hexfile OUTPUT 

