CC = /Applications/ArmGNUToolchain/13.2.Rel1/arm-none-eabi/bin/arm-none-eabi-gcc
AS = /Applications/ArmGNUToolchain/13.2.Rel1/arm-none-eabi/bin/arm-none-eabi-as
OBJCOPY = /Applications/ArmGNUToolchain/13.2.Rel1/arm-none-eabi/bin/arm-none-eabi-objcopy

# basic compiler flags
CFLAGS = -mcpu=cortex-m7 -mthumb -O2 -lc -lm -specs=nano.specs -specs=nosys.specs
# debug information
CFLAGS += -g
# enable HAL (hardware abstraction library)
CFLAGS += -DSTM32H753xx -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER
# optimize resulting binary
CFLAGS += -fdata-sections -ffunction-sections -Wl,--gc-sections
LDFLAGS = -T STM32H743ZITX_FLASH.ld

INCLUDES =  -IInc -IDrivers/BSP/STM32H7xx_Nucleo
INCLUDES += -IDrivers/CMSIS/Include -IDrivers/CMSIS/Device/ST/STM32H7xx/Include
INCLUDES += -IDrivers/STM32H7xx_HAL_Driver/Inc

SOURCES = main.c system_stm32h7xx.c
SOURCES +=  Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_utils.c \
			Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_rcc.c
OBJECTS = $(SOURCES:.c=.o) startup_stm32h753xx.o

OPENOCD_FLAGS = -f interface/stlink.cfg -f target/stm32h7x.cfg 

PROJECT_NAME = hal_demo
all: $(PROJECT_NAME).bin

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.s
	$(AS) $< -o $@

$(PROJECT_NAME).elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(PROJECT_NAME).bin: $(PROJECT_NAME).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f $(OBJECTS) $(PROJECT_NAME).elf $(PROJECT_NAME).bin

# when flashing .bin, the start address is needed, because
# unlike the .elf, it does not contain that information
flash: $(PROJECT_NAME).bin
	/Users/alexkimmel/Uni/Raumfahrt/xpack-openocd-0.12.0-3/bin/openocd $(OPENOCD_FLAGS) -c "program $(PROJECT_NAME).bin 0x08000000 verify reset exit"

# opens minicom for UART/serial input/output
serial:
	sudo minicom --device /dev/tty.usbmodem103 --baudrate 115200
