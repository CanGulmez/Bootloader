# Bootloader for STM32F446REX

# Toolchain	definitions
CC					:= arm-none-eabi-gcc
OBJCOPY			:= arm-none-eabi-objcopy
SIZE				:= arm-none-eabi-size -Gd
RM					:= rm
FILE				:= file
AR					:= ar rcs

# Microcontroller-specific definitions
DEVICE_FAMILY	:= STM32F4xx
DEVICE_MODEL	:= STM32F446xx
DEVICE_VARIANT	:= STM32F446RETx

# Compiler and linker flags
CORTEX_FLAGS	:= -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
COMMON_FLAGS	:= -std=gnu2x -O0 -Wall -Wextra -ffunction-sections -fdata-sections
AS_FLAGS			:= -x assembler-with-cpp

# Include paths
MAIN_INC			:= src
CMSIS_INC		:= drivers/CMSIS/Include
CMSIS_DEV_INC	:= drivers/CMSIS/Device/ST/STM32F4xx/Include
HAL_INC			:= drivers/STM32F4xx_HAL_Driver/Inc

# Source and special files
MAIN_SRC			:= $(wildcard src/*.c)
HAL_SRC			:= $(wildcard drivers/STM32F4xx_HAL_Driver/Src/*.c)
SYSTEM_SRC		:= drivers/system_stm32f4xx.c
STARTUP_CODE	:= drivers/startup_stm32f446xx.S
LINKER_SCRIPT	:= drivers/STM32F446RETX_FLASH.ld

# Sorthand and build definititions
DEFINES			:= -D$(DEVICE_FAMILY) -D$(DEVICE_MODEL) -D$(DEVICE_VARIANT) \
						-DUSE_HAL_DRIVER
INCLUDES			:= -I$(MAIN_INC) -I$(CMSIS_DEV_INC) -I$(CMSIS_INC) -I$(HAL_INC)
DRIVER_SRCS		:= $(HAL_SRC) $(SYSTEM_SRC)
DRIVER_OBJS		:= $(notdir $(patsubst %.c,%.o,$(DRIVER_SRCS))) startup_stm32f446xx.o
MAIN_OBJS		:= $(notdir $(patsubst %.c,%.o,$(MAIN_SRC)))

CFLAGS			:= $(CORTEX_FLAGS) $(COMMON_FLAGS) $(DEFINES) $(INCLUDES)
AFLAGS			:= $(CORTEX_FLAGS) $(AS_FLAGS)
LDFLAGS			:= $(CORTEX_FLAGS) -T $(LINKER_SCRIPT) \
						-Wl,--gc-sections,--relax --specs=nano.specs --specs=nosys.specs \
						-Wl,--start-group -lc -lm -Wl,--end-group -u _printf_float

# Output artifact definitions
BIN_DIR			:= bin
LIB_DIR			:= lib
FIRMWARE_ELF	:= $(BIN_DIR)/firmware.elf
FIRMWARE_BIN	:= $(BIN_DIR)/firmware.bin
LIBRARY			:= $(LIB_DIR)/lib$(DEVICE_FAMILY).a

.PHONY: lib bin

lib:
	@echo "--> Compiling the source files"
	$(CC) $(AFLAGS) -c $(STARTUP_CODE)
	$(CC) $(CFLAGS) -c $(DRIVER_SRCS)

	@echo "\n--> Archiving the object files"
	$(AR) $(LIBRARY) $(DRIVER_OBJS)

	@echo "\n--> Removing the object files"
	$(RM) $(DRIVER_OBJS)

bin:
	@echo "--> Compiling the source files"
	$(CC) -g $(CFLAGS) -c $(MAIN_SRC)

	@echo "\n--> Linking the object files"
	$(CC) $(LDFLAGS) $(MAIN_OBJS) -o $(FIRMWARE_ELF) -l$(DEVICE_FAMILY) -L$(LIB_DIR)

	@echo "\n--> Stripping the firmware binary"
	$(OBJCOPY) -O binary $(FIRMWARE_ELF) $(FIRMWARE_BIN)

	@echo "\n--> Removing the object files"
	$(RM) $(MAIN_OBJS)

	@echo "\n--> The firmware section sizes"
	@$(SIZE) $(FIRMWARE_ELF)

	@echo "\n--> The firmware binary format"
	@$(FILE) $(FIRMWARE_ELF)
