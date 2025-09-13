PREFIX=arm-linux-gnueabihf-
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
AR=$(PREFIX)ar
OBJCOPY=$(PREFIX)objcopy
OBJDUMP=$(PREFIX)objdump

led.img : start.S
	$(CC) -nostdlib -g -c -o start.o start.S
	
	$(LD) -T stm32mp157.lds -g start.o -o led.elf 
	
	$(OBJCOPY) -O binary -S led.elf  led.bin
	$(OBJDUMP) -D -m arm  led.elf  > led.dis	
	mkimage -A arm -T firmware -C none -O u-boot -a 0xC0100000 -e 0 -d led.bin led.img
	mkimage -T stm32image -a 0xC0100000 -e 0xC0100000 -d led.bin led.stm32

clean:
	rm -f led.dis  led.bin led.elf led.img *.o *.log
	
