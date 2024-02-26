################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ADC_Handler.c \
../Core/Src/Application.c \
../Core/Src/CAN_Handler.c \
../Core/Src/Gearbox.c \
../Core/Src/adc.c \
../Core/Src/can.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/opamp.c \
../Core/Src/stm32f3xx_hal_msp.c \
../Core/Src/stm32f3xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f3xx.c \
../Core/Src/tim.c 

OBJS += \
./Core/Src/ADC_Handler.o \
./Core/Src/Application.o \
./Core/Src/CAN_Handler.o \
./Core/Src/Gearbox.o \
./Core/Src/adc.o \
./Core/Src/can.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/opamp.o \
./Core/Src/stm32f3xx_hal_msp.o \
./Core/Src/stm32f3xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f3xx.o \
./Core/Src/tim.o 

C_DEPS += \
./Core/Src/ADC_Handler.d \
./Core/Src/Application.d \
./Core/Src/CAN_Handler.d \
./Core/Src/Gearbox.d \
./Core/Src/adc.d \
./Core/Src/can.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/opamp.d \
./Core/Src/stm32f3xx_hal_msp.d \
./Core/Src/stm32f3xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f3xx.d \
./Core/Src/tim.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/Documents/STM32Cube/GMU_2024/Core/LIB_CAN-master/CAN_LIB/inc" -I../Drivers/CAN_LIB/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ADC_Handler.cyclo ./Core/Src/ADC_Handler.d ./Core/Src/ADC_Handler.o ./Core/Src/ADC_Handler.su ./Core/Src/Application.cyclo ./Core/Src/Application.d ./Core/Src/Application.o ./Core/Src/Application.su ./Core/Src/CAN_Handler.cyclo ./Core/Src/CAN_Handler.d ./Core/Src/CAN_Handler.o ./Core/Src/CAN_Handler.su ./Core/Src/Gearbox.cyclo ./Core/Src/Gearbox.d ./Core/Src/Gearbox.o ./Core/Src/Gearbox.su ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/can.cyclo ./Core/Src/can.d ./Core/Src/can.o ./Core/Src/can.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/opamp.cyclo ./Core/Src/opamp.d ./Core/Src/opamp.o ./Core/Src/opamp.su ./Core/Src/stm32f3xx_hal_msp.cyclo ./Core/Src/stm32f3xx_hal_msp.d ./Core/Src/stm32f3xx_hal_msp.o ./Core/Src/stm32f3xx_hal_msp.su ./Core/Src/stm32f3xx_it.cyclo ./Core/Src/stm32f3xx_it.d ./Core/Src/stm32f3xx_it.o ./Core/Src/stm32f3xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f3xx.cyclo ./Core/Src/system_stm32f3xx.d ./Core/Src/system_stm32f3xx.o ./Core/Src/system_stm32f3xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su

.PHONY: clean-Core-2f-Src

