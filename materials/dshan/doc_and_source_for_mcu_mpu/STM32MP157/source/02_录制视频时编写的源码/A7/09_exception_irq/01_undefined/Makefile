PREFIX=arm-linux-gnueabihf-
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
AR=$(PREFIX)ar
OBJCOPY=$(PREFIX)objcopy
OBJDUMP=$(PREFIX)objdump

CFLAGS = 

objs = start.o uart.o string.o init.o exception.o main.o

dep_files := $(patsubst %,.%.d, $(objs))
dep_files := $(wildcard $(dep_files))

led.img : $(objs)	
	$(LD) -T stm32mp157.lds -g $^ -o led.elf 
	
	$(OBJCOPY) -O binary -S led.elf  led.bin
	$(OBJDUMP) -D -m arm  led.elf  > led.dis	
	mkimage -A arm -T firmware -C none -O u-boot -a 0xC0100000 -e 0 -d led.bin led.img
	mkimage -T stm32image -a 0xC0100000 -e 0xC0100000 -d led.bin led.stm32

ifneq ($(dep_files),)
include $(dep_files)
endif

%.o : %.c
	$(PREFIX)gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d

%.o : %.S
	$(PREFIX)gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d

clean:
	rm -f led.dis  led.bin led.elf led.img *.o *.log .*.d
	
