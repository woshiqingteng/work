GNU ?= riscv64-unknown-elf

COPS += -g -O0 -Wall -nostdlib -nostdinc -Iinclude -mcmodel=medany -mabi=lp64 -march=rv64imafd -fno-PIE -fomit-frame-pointer


#
# System environment variable.
#
#
# System environment variable.
#
ifeq ($(OS), Windows_NT)
	HOSTOS		:= windows
else
	ifneq (,$(findstring Linux, $(shell uname -a)))
		HOSTOS	:= linux
	endif
endif

ifeq ($(strip $(HOSTOS)), linux)
MKSUNXI		:= tools/linux/mksunxi
endif
ifeq ($(strip $(HOSTOS)), windows)
MKSUNXI		:= tools/windows/mksunxi
endif



all : app.bin

app.bin : start.o main.o
	$(GNU)-ld -T link.ld -o app.elf  start.o main.o
	$(GNU)-objcopy app.elf -O binary app.bin
	$(GNU)-objdump -D app.elf > app.dis
	$(MKSUNXI) app.bin

start.o : start.S
	$(GNU)-gcc $(COPS) -c $< -o $@

main.o : main.c
	$(GNU)-gcc $(COPS) -c $< -o $@

burn:
	xfel spinor write 0 app.bin

clean:
	rm *.o rm *.elf rm *.bin

