MCU=atmega328p

# all: count-sw.hex light-3.hex light-8.hex

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

%.flash: %.hex
	avrdude -p $(MCU) -c usbasp -U flash:w:$<

%.o: %.c peri.h
	avr-gcc -mmcu=$(MCU) -O2 -c -o $@ $<

%.elf: %.o peri.o
	avr-gcc -mmcu=$(MCU) -O2 -o $@ $^

clean:
	rm -f *~ *.o *.elf *.hex

# chase.hex: chase.elf
# 	avr-objcopy -j .text -j .data -O ihex chase.elf chase.hex

# flase: chase.hex
# 	avrdude -p atmega328p -c usbasp -U flash:w:chase.hex

# chase.elf: chase.o peri.o
# 	avr-gcc -mmcu=atmega328p -O2 -o chase.elf chase.o peri.o

# chase.o: chase.c
# 	avr-gcc -mmcu=atmega328p -O2 -o -c chase.o chase.c

# peri.o: peri.c
# 	avr-gcc -mmcu=atmega328p -O2 -o -c peri.o peri.c

	