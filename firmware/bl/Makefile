# Makefile for STM32F405 using CMSIS and GNU toolchain.

NAME = bl

# Files to include
C_SRC  = $(wildcard *.c)
#C_SRC += $(wildcard usb/*.c)
#C_SRC += $(wildcard lib/STM32F4xx_StdPeriph_Driver/src/*.c)
#C_SRC += $(wildcard lib/STM32_USB_Device_Driver/src/*.c)
#C_SRC := $(filter-out lib/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fmc.c, $(C_SRC))
#C_INC  = $(wildcard *.h)

OBJECTS := $(patsubst %.c,%.o,$(C_SRC))

OBJECTS += startup.o
OBJECTS := $(addprefix build/,$(OBJECTS))

# Compiler, etc
PREFIX = arm-none-eabi-
CC =      $(PREFIX)gcc
LD =      $(PREFIX)gcc
AS =      $(PREFIX)as
OBJCOPY = $(PREFIX)objcopy
GDB =     $(PREFIX)gdb

#LIBRARIES = lib/CMSIS/Lib/ARM/arm_cortexM4lf_math.lib

#INC  = -I. -Iusb
#INC += -Ilib/CMSIS/Include -Ilib/CMSIS/Device/ST/STM32F4xx/Include
#INC += -Ilib/STM32F4xx_StdPeriph_Driver/inc
#INC += -Ilib/STM32_USB_Device_Driver/inc

# Inject the git revision into the compiled code as a version number
GITREV = $(shell git log --pretty=format:'%h' -n 1 )
GITSUFFIX = $(shell git status|grep "Changes.* commit.*:">/dev/null && echo "+")

# Assem${NAME}er, compiler, and linker flags
AFLAGS	= -mlittle-endian -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS	= $(AFLAGS) -g -O2 -flto $(INC) -std=c99 -DUSE_STDPERIPH_DRIVER -Wall -Wundef
CFLAGS += -DARM_MATH_CM4 -D'__FPU_PRESENT=1'
CFLAGS += -D'HSE_VALUE=((uint32_t)8000000)'
CFLAGS += -DUSE_STDPERIPH_DRIVER -DSTM32F40_41xxx
CFLAGS += -D'_REVISION="$(GITREV)$(GITSUFFIX)"'
CFLAGS += -fdata-sections -ffunction-sections
VERSION ?= 0
CFLAGS += -D'_VERSION="$(VERSION)"'
LFLAGS = $(CFLAGS)
LFLAGS += -nostartfiles -Tflash.ld -Xlinker -Map=${NAME}.map -Xlinker --gc-sections

.PHONY: all directories clean

# Targets
all: ${NAME}.bin
	$(info Revision $(GITREV)$(GITSUFFIX))
	$(info Version $(VERSION))

directories: build \
             build/usb \
             build/lib/STM32F4xx_StdPeriph_Driver/src \
             build/lib/STM32_USB_Device_Driver/src

build:
	mkdir -p build
build/usb:
	mkdir -p build/usb
build/lib/STM32F4xx_StdPeriph_Driver/src:
	mkdir -p build/lib/STM32F4xx_StdPeriph_Driver/src
build/lib/STM32_USB_Device_Driver/src:
	mkdir -p build/lib/STM32_USB_Device_Driver/src

clean:
	-rm -rf build
	-rm -f *.elf *.bin *.map
flashrom: ${NAME}.bin
	dfu-util -d 0483:df11 -a 0 -s 0x0800C000:leave -D ${NAME}.bin
gdb: ${NAME}.elf
	$(GDB)
load: ${NAME}.elf
	$(GDB) -ex load
loadquit: ${NAME}.elf
	$(GDB) -batch -ex load
${NAME}.bin: ${NAME}.elf
	$(OBJCOPY) -O binary ${NAME}.elf ${NAME}.bin
${NAME}.elf: $(OBJECTS)
	$(info LD $@)
	@$(LD) $(LFLAGS) -o $@ $^ $(LIBRARIES)
build/startup.o: startup.s
	@make directories > /dev/null
	$(info AS $<)
	@$(AS) $(AFLAGS) $< -o $@
build/%.o: %.c $(C_INC)
	@make directories > /dev/null
	$(info CC $<)
	@$(CC) $(CFLAGS) -c -o $@ $<
