################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../thirdparty/SEGGER/SEGGER/SEGGER_RTT.c \
../thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.c \
../thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
../thirdparty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./thirdparty/SEGGER/SEGGER/SEGGER_RTT.o \
./thirdparty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.o \
./thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./thirdparty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./thirdparty/SEGGER/SEGGER/SEGGER_RTT.d \
./thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.d \
./thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/SEGGER/SEGGER/%.o thirdparty/SEGGER/SEGGER/%.su thirdparty/SEGGER/SEGGER/%.cyclo: ../thirdparty/SEGGER/SEGGER/%.c thirdparty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/SEGGER/Config" -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/SEGGER/SEGGER" -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/FreeRTOS" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
thirdparty/SEGGER/SEGGER/%.o: ../thirdparty/SEGGER/SEGGER/%.S thirdparty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/SEGGER/Config" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-thirdparty-2f-SEGGER-2f-SEGGER

clean-thirdparty-2f-SEGGER-2f-SEGGER:
	-$(RM) ./thirdparty/SEGGER/SEGGER/SEGGER_RTT.cyclo ./thirdparty/SEGGER/SEGGER/SEGGER_RTT.d ./thirdparty/SEGGER/SEGGER/SEGGER_RTT.o ./thirdparty/SEGGER/SEGGER/SEGGER_RTT.su ./thirdparty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./thirdparty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.cyclo ./thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.d ./thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.o ./thirdparty/SEGGER/SEGGER/SEGGER_RTT_printf.su ./thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.cyclo ./thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.d ./thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.o ./thirdparty/SEGGER/SEGGER/SEGGER_SYSVIEW.su

.PHONY: clean-thirdparty-2f-SEGGER-2f-SEGGER

