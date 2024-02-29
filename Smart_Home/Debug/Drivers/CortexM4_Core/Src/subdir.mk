################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CortexM4_Core/Src/NVIC.c 

OBJS += \
./Drivers/CortexM4_Core/Src/NVIC.o 

C_DEPS += \
./Drivers/CortexM4_Core/Src/NVIC.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CortexM4_Core/Src/%.o Drivers/CortexM4_Core/Src/%.su Drivers/CortexM4_Core/Src/%.cyclo: ../Drivers/CortexM4_Core/Src/%.c Drivers/CortexM4_Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I"D:/MCTR Softwares/STMCube/STMCube IDE/STM32CubeIDE_1.14.0/STM32CubeIDE/Projects/Smart_Home/Drivers/ECU_Drivers" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CortexM4_Core-2f-Src

clean-Drivers-2f-CortexM4_Core-2f-Src:
	-$(RM) ./Drivers/CortexM4_Core/Src/NVIC.cyclo ./Drivers/CortexM4_Core/Src/NVIC.d ./Drivers/CortexM4_Core/Src/NVIC.o ./Drivers/CortexM4_Core/Src/NVIC.su

.PHONY: clean-Drivers-2f-CortexM4_Core-2f-Src

