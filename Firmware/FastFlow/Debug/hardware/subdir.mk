################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/buzzer.c \
../hardware/encoder.c \
../hardware/hardware.c \
../hardware/oven.c 

OBJS += \
./hardware/buzzer.o \
./hardware/encoder.o \
./hardware/hardware.o \
./hardware/oven.o 

C_DEPS += \
./hardware/buzzer.d \
./hardware/encoder.d \
./hardware/hardware.d \
./hardware/oven.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/%.o hardware/%.su hardware/%.cyclo: ../hardware/%.c hardware/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../app -I../hardware -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-hardware

clean-hardware:
	-$(RM) ./hardware/buzzer.cyclo ./hardware/buzzer.d ./hardware/buzzer.o ./hardware/buzzer.su ./hardware/encoder.cyclo ./hardware/encoder.d ./hardware/encoder.o ./hardware/encoder.su ./hardware/hardware.cyclo ./hardware/hardware.d ./hardware/hardware.o ./hardware/hardware.su ./hardware/oven.cyclo ./hardware/oven.d ./hardware/oven.o ./hardware/oven.su

.PHONY: clean-hardware

