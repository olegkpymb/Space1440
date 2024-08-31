# Makefile
TARGET = firmware

# Define the linker script location and chip architecture.
LD_SCRIPT = STM32G431KBTX_FLASH.ld
STARTUP_FILE = ./Src/Startup/startup_stm32g431kbtx.s
MCU_SPEC  = cortex-m4
CHIP_NUMBER = STM32G431xx
# Find the Chip Number in inc/stm32g4xx.h lines 106-125

# Toolchain definitions (ARM bare metal defaults)
TOOLCHAIN = /usr
CC = $(TOOLCHAIN)/bin/arm-none-eabi-gcc
AS = $(TOOLCHAIN)/bin/arm-none-eabi-as
LD = $(TOOLCHAIN)/bin/arm-none-eabi-ld
OC = $(TOOLCHAIN)/bin/arm-none-eabi-objcopy
OD = $(TOOLCHAIN)/bin/arm-none-eabi-objdump
OS = $(TOOLCHAIN)/bin/arm-none-eabi-size

# Defines
DEFS       = -DUSE_HAL_DRIVER

# Assembly directives.
ASFLAGS += -c
ASFLAGS += -O0
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb
ASFLAGS += -Wall
# (Set error messages to appear on a single line.)
ASFLAGS += -fmessage-length=0

# C compilation directives
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
CFLAGS += -Wall
CFLAGS += -g
# (Set error messages to appear on a single line.)
CFLAGS += -fmessage-length=0
# (Set system to ignore semihosted junk)
CFLAGS += --specs=nosys.specs
# (Set defines)
CFLAGS += $(DEFS)

# Linker directives.
LSCRIPT = ./$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
LFLAGS += -Wall
LFLAGS += --specs=nosys.specs
LFLAGS += -nostdlib
LFLAGS += -lgcc
LFLAGS += -T$(LSCRIPT)

INCLUDE  = -I./Src/CMSIS/Core/Include/
INCLUDE += -I./Src/CMSIS/Device/ST/STM32G4xx/Include/
INCLUDE += -I./Src/Driver/Inc/
INCLUDE += -I./Src/HAL/
INCLUDE += -I./Src/HAL/Inc/
INCLUDE += -I./Src/HAL/Inc/Legacy/
INCLUDE += -I./Src/Main/

AS_SRC   = $(STARTUP_FILE)
C_SRC    = ./Src/main/main.c
C_SRC   += ./Src/CMSIS/Device/ST/STM32G4xx/Source/system_stm32g4xx.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_cortex.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_dma.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_dma_ex.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_exti.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_gpio.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_pwr.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_pwr_ex.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_rcc.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_rcc_ex.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_tim.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_tim_ex.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_uart.c
C_SRC   += ./Src/HAL/Src/stm32g4xx_hal_uart_ex.c
C_SRC   += ./Src/Driver/Src/clock.c
C_SRC   += ./Src/Driver/Src/exti.c
C_SRC   += ./Src/Driver/Src/gpio.c
C_SRC   += ./Src/Driver/Src/hal.c
C_SRC   += ./Src/Driver/Src/it.c
C_SRC   += ./Src/Driver/Src/timer.c
C_SRC   += ./Src/Driver/Src/uart.c

OBJS += $(AS_SRC:.s=.o)
OBJS += $(C_SRC:.c=.o)

.PHONY: all
all: $(TARGET).hex $(TARGET).bin

%.o: %.s
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -D$(CHIP_NUMBER) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).hex: $(TARGET).elf
	$(OC) -O ihex $< $@
	$(OS) $<

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf
	rm -f $(TARGET).hex
	rm -f $(TARGET).bin
