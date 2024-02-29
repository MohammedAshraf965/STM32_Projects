################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ECU_Drivers/Src/chr_lcd.c \
../Drivers/ECU_Drivers/Src/chr_lcd_cfgType.c \
../Drivers/ECU_Drivers/Src/keypad.c \
../Drivers/ECU_Drivers/Src/keypad_cfgType.c \
../Drivers/ECU_Drivers/Src/led.c \
../Drivers/ECU_Drivers/Src/led_cfgType.c 

OBJS += \
./Drivers/ECU_Drivers/Src/chr_lcd.o \
./Drivers/ECU_Drivers/Src/chr_lcd_cfgType.o \
./Drivers/ECU_Drivers/Src/keypad.o \
./Drivers/ECU_Drivers/Src/keypad_cfgType.o \
./Drivers/ECU_Drivers/Src/led.o \
./Drivers/ECU_Drivers/Src/led_cfgType.o 

C_DEPS += \
./Drivers/ECU_Drivers/Src/chr_lcd.d \
./Drivers/ECU_Drivers/Src/chr_lcd_cfgType.d \
./Drivers/ECU_Drivers/Src/keypad.d \
./Drivers/ECU_Drivers/Src/keypad_cfgType.d \
./Drivers/ECU_Drivers/Src/led.d \
./Drivers/ECU_Drivers/Src/led_cfgType.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ECU_Drivers/Src/%.o Drivers/ECU_Drivers/Src/%.su Drivers/ECU_Drivers/Src/%.cyclo: ../Drivers/ECU_Drivers/Src/%.c Drivers/ECU_Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I"D:/MCTR Softwares/STMCube/STMCube IDE/STM32CubeIDE_1.14.0/STM32CubeIDE/Projects/Smart_Home/Drivers/ECU_Drivers" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-ECU_Drivers-2f-Src

clean-Drivers-2f-ECU_Drivers-2f-Src:
	-$(RM) ./Drivers/ECU_Drivers/Src/chr_lcd.cyclo ./Drivers/ECU_Drivers/Src/chr_lcd.d ./Drivers/ECU_Drivers/Src/chr_lcd.o ./Drivers/ECU_Drivers/Src/chr_lcd.su ./Drivers/ECU_Drivers/Src/chr_lcd_cfgType.cyclo ./Drivers/ECU_Drivers/Src/chr_lcd_cfgType.d ./Drivers/ECU_Drivers/Src/chr_lcd_cfgType.o ./Drivers/ECU_Drivers/Src/chr_lcd_cfgType.su ./Drivers/ECU_Drivers/Src/keypad.cyclo ./Drivers/ECU_Drivers/Src/keypad.d ./Drivers/ECU_Drivers/Src/keypad.o ./Drivers/ECU_Drivers/Src/keypad.su ./Drivers/ECU_Drivers/Src/keypad_cfgType.cyclo ./Drivers/ECU_Drivers/Src/keypad_cfgType.d ./Drivers/ECU_Drivers/Src/keypad_cfgType.o ./Drivers/ECU_Drivers/Src/keypad_cfgType.su ./Drivers/ECU_Drivers/Src/led.cyclo ./Drivers/ECU_Drivers/Src/led.d ./Drivers/ECU_Drivers/Src/led.o ./Drivers/ECU_Drivers/Src/led.su ./Drivers/ECU_Drivers/Src/led_cfgType.cyclo ./Drivers/ECU_Drivers/Src/led_cfgType.d ./Drivers/ECU_Drivers/Src/led_cfgType.o ./Drivers/ECU_Drivers/Src/led_cfgType.su

.PHONY: clean-Drivers-2f-ECU_Drivers-2f-Src

