################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS_Kernel/croutine.c \
../FreeRTOS_Kernel/event_groups.c \
../FreeRTOS_Kernel/list.c \
../FreeRTOS_Kernel/queue.c \
../FreeRTOS_Kernel/stream_buffer.c \
../FreeRTOS_Kernel/tasks.c \
../FreeRTOS_Kernel/timers.c 

OBJS += \
./FreeRTOS_Kernel/croutine.o \
./FreeRTOS_Kernel/event_groups.o \
./FreeRTOS_Kernel/list.o \
./FreeRTOS_Kernel/queue.o \
./FreeRTOS_Kernel/stream_buffer.o \
./FreeRTOS_Kernel/tasks.o \
./FreeRTOS_Kernel/timers.o 

C_DEPS += \
./FreeRTOS_Kernel/croutine.d \
./FreeRTOS_Kernel/event_groups.d \
./FreeRTOS_Kernel/list.d \
./FreeRTOS_Kernel/queue.d \
./FreeRTOS_Kernel/stream_buffer.d \
./FreeRTOS_Kernel/tasks.d \
./FreeRTOS_Kernel/timers.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS_Kernel/%.o FreeRTOS_Kernel/%.su FreeRTOS_Kernel/%.cyclo: ../FreeRTOS_Kernel/%.c FreeRTOS_Kernel/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"C:/Vault/EmbeddedSTMWorkspace/RC_Car_Using_FreeRTOS/Project/FreeRTOS_Kernel/include" -I"C:/Vault/EmbeddedSTMWorkspace/RC_Car_Using_FreeRTOS/Project/FreeRTOS_Kernel/portable/GCC/ARM_CM4F" -I"C:/Vault/EmbeddedSTMWorkspace/RC_Car_Using_FreeRTOS/Project/Car/Public" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeRTOS_Kernel

clean-FreeRTOS_Kernel:
	-$(RM) ./FreeRTOS_Kernel/croutine.cyclo ./FreeRTOS_Kernel/croutine.d ./FreeRTOS_Kernel/croutine.o ./FreeRTOS_Kernel/croutine.su ./FreeRTOS_Kernel/event_groups.cyclo ./FreeRTOS_Kernel/event_groups.d ./FreeRTOS_Kernel/event_groups.o ./FreeRTOS_Kernel/event_groups.su ./FreeRTOS_Kernel/list.cyclo ./FreeRTOS_Kernel/list.d ./FreeRTOS_Kernel/list.o ./FreeRTOS_Kernel/list.su ./FreeRTOS_Kernel/queue.cyclo ./FreeRTOS_Kernel/queue.d ./FreeRTOS_Kernel/queue.o ./FreeRTOS_Kernel/queue.su ./FreeRTOS_Kernel/stream_buffer.cyclo ./FreeRTOS_Kernel/stream_buffer.d ./FreeRTOS_Kernel/stream_buffer.o ./FreeRTOS_Kernel/stream_buffer.su ./FreeRTOS_Kernel/tasks.cyclo ./FreeRTOS_Kernel/tasks.d ./FreeRTOS_Kernel/tasks.o ./FreeRTOS_Kernel/tasks.su ./FreeRTOS_Kernel/timers.cyclo ./FreeRTOS_Kernel/timers.d ./FreeRTOS_Kernel/timers.o ./FreeRTOS_Kernel/timers.su

.PHONY: clean-FreeRTOS_Kernel

