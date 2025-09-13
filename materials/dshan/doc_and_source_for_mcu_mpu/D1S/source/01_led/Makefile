
CROSS ?= riscv64-unknown-elf-
CFLAGS += -g -O0 -Wall -nostdlib -nostdinc -Iinclude -mcmodel=medany -mabi=lp64 -march=rv64imafd -fno-PIE -fomit-frame-pointer

#MKSUNXI		:= tools/linux/mksunxi
MKSUNXI		:= tools/windows/mksunxi

TARGET = led

$(TARGET).bin: start.o main.o
	$(CROSS)ld -T link.lds -o $(TARGET).elf $^
	$(CROSS)objcopy $(TARGET).elf -O binary $@
	$(CROSS)objdump -D $(TARGET).elf > $(TARGET).dis
	$(MKSUNXI) $@

%.o : %.S
	$(CROSS)gcc $(CFLAGS) -c $< -o $@

%.o : %.c
	$(CROSS)gcc $(CFLAGS) -c $< -o $@

burn:
	xfel spinor write 0 $(TARGET).bin

clean:
	rm *.elf *.bin *.o *.dis

