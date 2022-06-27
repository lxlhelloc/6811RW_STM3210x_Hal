################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/bms_caiji.c \
../APP/bms_control.c \
../APP/bms_soc.c \
../APP/drv_LTC6811.c \
../APP/drv_LTC6811_hw.c \
../APP/drv_LTC6811_proto.c 

OBJS += \
./APP/bms_caiji.o \
./APP/bms_control.o \
./APP/bms_soc.o \
./APP/drv_LTC6811.o \
./APP/drv_LTC6811_hw.o \
./APP/drv_LTC6811_proto.o 

C_DEPS += \
./APP/bms_caiji.d \
./APP/bms_control.d \
./APP/bms_soc.d \
./APP/drv_LTC6811.d \
./APP/drv_LTC6811_hw.d \
./APP/drv_LTC6811_proto.d 


# Each subdirectory must supply rules for building sources it contributes
APP/bms_caiji.o: ../APP/bms_caiji.c APP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/bms_caiji.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APP/bms_control.o: ../APP/bms_control.c APP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/bms_control.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APP/bms_soc.o: ../APP/bms_soc.c APP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/bms_soc.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APP/drv_LTC6811.o: ../APP/drv_LTC6811.c APP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/drv_LTC6811.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APP/drv_LTC6811_hw.o: ../APP/drv_LTC6811_hw.c APP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/drv_LTC6811_hw.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APP/drv_LTC6811_proto.o: ../APP/drv_LTC6811_proto.c APP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/drv_LTC6811_proto.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

