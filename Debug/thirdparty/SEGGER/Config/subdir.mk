################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../thirdparty/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.c 

OBJS += \
./thirdparty/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.o 

C_DEPS += \
./thirdparty/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/SEGGER/Config/%.o thirdparty/SEGGER/Config/%.su thirdparty/SEGGER/Config/%.cyclo: ../thirdparty/SEGGER/Config/%.c thirdparty/SEGGER/Config/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/SEGGER/Config" -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/SEGGER/SEGGER" -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/FreeRTOS" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-thirdparty-2f-SEGGER-2f-Config

clean-thirdparty-2f-SEGGER-2f-Config:
	-$(RM) ./thirdparty/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.cyclo ./thirdparty/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.d ./thirdparty/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.o ./thirdparty/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.su

.PHONY: clean-thirdparty-2f-SEGGER-2f-Config

