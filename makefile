SERIALPORT = COM9
# Specify the name of your source code here:
SOURCEFILE = test.c
#_____________________________________________________________________
# Don't change anything below unless you know what you're doing....
CLOCKSPEED = 16000000	
PROGRAMMER = arduino

begin:	main.hex

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=atmega328p main.elf

main.elf: $(SOURCEFILE)
	avr-gcc -Wall -Os -DF_CPU=$(CLOCKSPEED) -mmcu=atmega328p -o main.elf $(SOURCEFILE)

flash:	begin
	avrdude -c $(PROGRAMMER) -b 115200 -P $(SERIALPORT) -p atmega328p -U flash:w:main.hex:i

clean: 
	rm -f main.elf main.hex
