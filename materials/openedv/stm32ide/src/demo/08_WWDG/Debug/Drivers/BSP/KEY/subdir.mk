################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/KEY/key.c 

OBJS += \
./Drivers/BSP/KEY/key.o 

C_DEPS += \
./Drivers/BSP/KEY/key.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/KEY/%.o Drivers/BSP/KEY/%.su Drivers/BSP/KEY/%.cyclo: ../Drivers/BSP/KEY/%.c Drivers/BSP/KEY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-KEY

clean-Drivers-2f-BSP-2f-KEY:
	-$(RM) ./Drivers/BSP/KEY/key.cyclo ./Drivers/BSP/KEY/key.d ./Drivers/BSP/KEY/key.o ./Drivers/BSP/KEY/key.su

.PHONY: clean-Drivers-2f-BSP-2f-KEY

