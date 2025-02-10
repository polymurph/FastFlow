################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hal/buzzer.c \
../hal/display.c \
../hal/encoder.c \
../hal/hardware.c \
../hal/max31865.c \
../hal/oven.c \
../hal/pcf8575.c \
../hal/tempSensor.c 

OBJS += \
./hal/buzzer.o \
./hal/display.o \
./hal/encoder.o \
./hal/hardware.o \
./hal/max31865.o \
./hal/oven.o \
./hal/pcf8575.o \
./hal/tempSensor.o 

C_DEPS += \
./hal/buzzer.d \
./hal/display.d \
./hal/encoder.d \
./hal/hardware.d \
./hal/max31865.d \
./hal/oven.d \
./hal/pcf8575.d \
./hal/tempSensor.d 


# Each subdirectory must supply rules for building sources it contributes
hal/%.o hal/%.su hal/%.cyclo: ../hal/%.c hal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../app -I../hardware -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-hal

clean-hal:
	-$(RM) ./hal/buzzer.cyclo ./hal/buzzer.d ./hal/buzzer.o ./hal/buzzer.su ./hal/display.cyclo ./hal/display.d ./hal/display.o ./hal/display.su ./hal/encoder.cyclo ./hal/encoder.d ./hal/encoder.o ./hal/encoder.su ./hal/hardware.cyclo ./hal/hardware.d ./hal/hardware.o ./hal/hardware.su ./hal/max31865.cyclo ./hal/max31865.d ./hal/max31865.o ./hal/max31865.su ./hal/oven.cyclo ./hal/oven.d ./hal/oven.o ./hal/oven.su ./hal/pcf8575.cyclo ./hal/pcf8575.d ./hal/pcf8575.o ./hal/pcf8575.su ./hal/tempSensor.cyclo ./hal/tempSensor.d ./hal/tempSensor.o ./hal/tempSensor.su

.PHONY: clean-hal

