################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Car/Src/app_main.c 

OBJS += \
./Car/Src/app_main.o 

C_DEPS += \
./Car/Src/app_main.d 


# Each subdirectory must supply rules for building sources it contributes
Car/Src/%.o Car/Src/%.su Car/Src/%.cyclo: ../Car/Src/%.c Car/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"C:/Vault/EmbeddedSTMWorkspace/RC_Car_Using_FreeRTOS/Project/FreeRTOS_Kernel/include" -I"C:/Vault/EmbeddedSTMWorkspace/RC_Car_Using_FreeRTOS/Project/FreeRTOS_Kernel/portable/GCC/ARM_CM4F" -I"C:/Vault/EmbeddedSTMWorkspace/RC_Car_Using_FreeRTOS/Project/Car/Public" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Car-2f-Src

clean-Car-2f-Src:
	-$(RM) ./Car/Src/app_main.cyclo ./Car/Src/app_main.d ./Car/Src/app_main.o ./Car/Src/app_main.su

.PHONY: clean-Car-2f-Src

