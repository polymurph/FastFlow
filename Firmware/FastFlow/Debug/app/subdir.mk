################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/fastFlow.c \
../app/tinyfsm.c \
../app/ui.c 

OBJS += \
./app/fastFlow.o \
./app/tinyfsm.o \
./app/ui.o 

C_DEPS += \
./app/fastFlow.d \
./app/tinyfsm.d \
./app/ui.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o app/%.su app/%.cyclo: ../app/%.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../app -I../hardware -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app

clean-app:
	-$(RM) ./app/fastFlow.cyclo ./app/fastFlow.d ./app/fastFlow.o ./app/fastFlow.su ./app/tinyfsm.cyclo ./app/tinyfsm.d ./app/tinyfsm.o ./app/tinyfsm.su ./app/ui.cyclo ./app/ui.d ./app/ui.o ./app/ui.su

.PHONY: clean-app

