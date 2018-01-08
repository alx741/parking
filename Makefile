# This Makefile is generated by the made script:
# https://github.com/alx741/made
#
# Replace FILE with your filename
# Replace MMCU with the target chip model
#
# Dependencies:
# 	avr-gcc
# 	avr-binutils
# 	avrdude


FILE=main
MMCU=atmega328p
AVRDUDE_PART=m328p
AVRDUDE_PROGRAMMER=usbasp
F_CPU=16000000


CFLAGS =
CFLAGS += -mmcu=$(MMCU)
CFLAGS += -DF_CPU=$(F_CPU)UL
CFLAGS += -Os
CFLAGS += -g


$(FILE).hex: $(FILE).elf
	avr-objcopy -j .text -j .data -O ihex $< $@

$(FILE).elf: $(FILE).c uart.c servo.c board.c
	avr-gcc $(CFLAGS) $^ -o $@

clean:
	rm $(FILE).elf $(FILE).hex

burn: $(FILE).hex
	sudo avrdude -p $(AVRDUDE_PART) -c $(AVRDUDE_PROGRAMMER) -U flash:w:$(FILE).hex

test:
	sudo picocom -c -b 9600 /dev/ttyUSB1