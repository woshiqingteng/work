################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/SYSTEM/sys/subdir.mk
-include Drivers/SYSTEM/delay/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/LED/subdir.mk
-include Drivers/BSP/LCD/subdir.mk
-include Drivers/BSP/KEY/subdir.mk
-include Drivers/BSP/BEEP/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := 39_1_DSP_BasicMath
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
39_1_DSP_BasicMath.elf \

MAP_FILES += \
39_1_DSP_BasicMath.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
39_1_DSP_BasicMath.list \

OBJCOPY_HEX += \
39_1_DSP_BasicMath.hex \

OBJCOPY_BIN += \
39_1_DSP_BasicMath.bin \


# All Target
all: main-build

# Main-build Target
main-build: 39_1_DSP_BasicMath.elf secondary-outputs

# Tool invocations
39_1_DSP_BasicMath.elf 39_1_DSP_BasicMath.map: $(OBJS) $(USER_OBJS) H:\cubeide\STM32CubeIDE_DEMO\39_1_DSP_BasicMath\STM32F103ZETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "39_1_DSP_BasicMath.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"H:\cubeide\STM32CubeIDE_DEMO\39_1_DSP_BasicMath\STM32F103ZETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="39_1_DSP_BasicMath.map" -Wl,--gc-sections -static "H:\cubeide\STM32CubeIDE_DEMO\39_1_DSP_BasicMath\Drivers\CMSIS\Lib\GCC\libarm_cortexM3l_math.a" --specs=nano.specs -mfloat-abi=soft -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

39_1_DSP_BasicMath.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "39_1_DSP_BasicMath.list"
	@echo 'Finished building: $@'
	@echo ' '

39_1_DSP_BasicMath.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "39_1_DSP_BasicMath.hex"
	@echo 'Finished building: $@'
	@echo ' '

39_1_DSP_BasicMath.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "39_1_DSP_BasicMath.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) 39_1_DSP_BasicMath.bin 39_1_DSP_BasicMath.elf 39_1_DSP_BasicMath.hex 39_1_DSP_BasicMath.list 39_1_DSP_BasicMath.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
