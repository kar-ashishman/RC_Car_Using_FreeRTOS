################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../thirdparty/FreeRTOS/croutine.c \
../thirdparty/FreeRTOS/event_groups.c \
../thirdparty/FreeRTOS/list.c \
../thirdparty/FreeRTOS/queue.c \
../thirdparty/FreeRTOS/stream_buffer.c \
../thirdparty/FreeRTOS/tasks.c \
../thirdparty/FreeRTOS/timers.c 

OBJS += \
./thirdparty/FreeRTOS/croutine.o \
./thirdparty/FreeRTOS/event_groups.o \
./thirdparty/FreeRTOS/list.o \
./thirdparty/FreeRTOS/queue.o \
./thirdparty/FreeRTOS/stream_buffer.o \
./thirdparty/FreeRTOS/tasks.o \
./thirdparty/FreeRTOS/timers.o 

C_DEPS += \
./thirdparty/FreeRTOS/croutine.d \
./thirdparty/FreeRTOS/event_groups.d \
./thirdparty/FreeRTOS/list.d \
./thirdparty/FreeRTOS/queue.d \
./thirdparty/FreeRTOS/stream_buffer.d \
./thirdparty/FreeRTOS/tasks.d \
./thirdparty/FreeRTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/FreeRTOS/%.o thirdparty/FreeRTOS/%.su thirdparty/FreeRTOS/%.cyclo: ../thirdparty/FreeRTOS/%.c thirdparty/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/FreeRTOS/portable/GCC/ARM_CM4F" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/niral/OneDrive/Desktop/workspace/RTOS_workspace/FreeRTOS_projects/thirdparty/FreeRTOS/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-thirdparty-2f-FreeRTOS

clean-thirdparty-2f-FreeRTOS:
	-$(RM) ./thirdparty/FreeRTOS/croutine.cyclo ./thirdparty/FreeRTOS/croutine.d ./thirdparty/FreeRTOS/croutine.o ./thirdparty/FreeRTOS/croutine.su ./thirdparty/FreeRTOS/event_groups.cyclo ./thirdparty/FreeRTOS/event_groups.d ./thirdparty/FreeRTOS/event_groups.o ./thirdparty/FreeRTOS/event_groups.su ./thirdparty/FreeRTOS/list.cyclo ./thirdparty/FreeRTOS/list.d ./thirdparty/FreeRTOS/list.o ./thirdparty/FreeRTOS/list.su ./thirdparty/FreeRTOS/queue.cyclo ./thirdparty/FreeRTOS/queue.d ./thirdparty/FreeRTOS/queue.o ./thirdparty/FreeRTOS/queue.su ./thirdparty/FreeRTOS/stream_buffer.cyclo ./thirdparty/FreeRTOS/stream_buffer.d ./thirdparty/FreeRTOS/stream_buffer.o ./thirdparty/FreeRTOS/stream_buffer.su ./thirdparty/FreeRTOS/tasks.cyclo ./thirdparty/FreeRTOS/tasks.d ./thirdparty/FreeRTOS/tasks.o ./thirdparty/FreeRTOS/tasks.su ./thirdparty/FreeRTOS/timers.cyclo ./thirdparty/FreeRTOS/timers.d ./thirdparty/FreeRTOS/timers.o ./thirdparty/FreeRTOS/timers.su

.PHONY: clean-thirdparty-2f-FreeRTOS

