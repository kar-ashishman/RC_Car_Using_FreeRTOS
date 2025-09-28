################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../thirdparty/FreeRTOS/portable/MemMang/heap_4.c 

OBJS += \
./thirdparty/FreeRTOS/portable/MemMang/heap_4.o 

C_DEPS += \
./thirdparty/FreeRTOS/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/FreeRTOS/portable/MemMang/%.o thirdparty/FreeRTOS/portable/MemMang/%.su thirdparty/FreeRTOS/portable/MemMang/%.cyclo: ../thirdparty/FreeRTOS/portable/MemMang/%.c thirdparty/FreeRTOS/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/FreeRTOS/portable/GCC/ARM_CM4F" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/FreeRTOS/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-thirdparty-2f-FreeRTOS-2f-portable-2f-MemMang

clean-thirdparty-2f-FreeRTOS-2f-portable-2f-MemMang:
	-$(RM) ./thirdparty/FreeRTOS/portable/MemMang/heap_4.cyclo ./thirdparty/FreeRTOS/portable/MemMang/heap_4.d ./thirdparty/FreeRTOS/portable/MemMang/heap_4.o ./thirdparty/FreeRTOS/portable/MemMang/heap_4.su

.PHONY: clean-thirdparty-2f-FreeRTOS-2f-portable-2f-MemMang

