################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Inc/Gearbox.c 

OBJS += \
./App/Inc/Gearbox.o 

C_DEPS += \
./App/Inc/Gearbox.d 


# Each subdirectory must supply rules for building sources it contributes
App/Inc/%.o App/Inc/%.su App/Inc/%.cyclo: ../App/Inc/%.c App/Inc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Inc

clean-App-2f-Inc:
	-$(RM) ./App/Inc/Gearbox.cyclo ./App/Inc/Gearbox.d ./App/Inc/Gearbox.o ./App/Inc/Gearbox.su

.PHONY: clean-App-2f-Inc

