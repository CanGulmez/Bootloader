# Bootloader Build Recipies for STM32F446RE

# Required toolchain and other programs
CC				:= arm-none-eabi-gcc
LD				:= arm-none-eabi-ld
OBJCOPY			:= arm-none-eabi-objcopy
SIZE			:= arm-none-eabi-size -Gd
RM				:= rm
FILE			:= file
AR				:= ar rcs

# Microcontroller-specific definitions
DEVICE_FAMILY	:= STM32F4xx
DEVICE_MODEL	:= STM32F446xx
DEVICE_VARIANT	:= STM32F446RETx

# Compiler and linker flags
WARN_FLAGS		:= -Wall -Wextra -Wconversion -Wshadow -Wundef
CORTEX_FLAGS	:= -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
COMMON_FLAGS	:= -std=gnu2x -Os $(WARN_FLAGS) -ffunction-sections -fdata-sections
AS_FLAGS		:= -x assembler-with-cpp

# Include paths
BL_INC			:= src
APP_INC			:= example
CMSIS_INC		:= drivers/CMSIS/Include
CMSIS_DEV_INC	:= drivers/CMSIS/Device/ST/STM32F4xx/Include
HAL_INC			:= drivers/STM32F4xx_HAL_Driver/Inc
INCS			:= -I$(CMSIS_DEV_INC) -I$(CMSIS_INC) -I$(HAL_INC)
BL_INCS			:= -I$(BL_INC) $(INCS)
APP_INCS		:= -I$(APP_INC) $(INCS)

# Source and special files
BL_SRCS			:= $(wildcard src/*.c)
APP_SRCS		:= $(wildcard example/*.c)
HAL_SRCS		:= $(wildcard drivers/STM32F4xx_HAL_Driver/Src/*.c)
STARTUP_CODE	:= drivers/startup.s
BL_SYSTEM		:= drivers/bl_system.c
BL_LINKER		:= drivers/bl_linker.ld
APP_SYSTEM		:= drivers/app_system.c
APP_LINKER		:= drivers/app_linker.ld

# Output artifact definitions
BIN_DIR			:= bin
LIB_DIR			:= lib
BL_ELF			:= $(BIN_DIR)/bl.elf
BL_BIN			:= $(BIN_DIR)/bl.bin
BL_MAP			:= $(BIN_DIR)/bl.map
APP_ELF			:= $(BIN_DIR)/app.elf
APP_BIN			:= $(BIN_DIR)/app.bin
APP_MAP			:= $(BIN_DIR)/app.map
BL_LIB			:= $(LIB_DIR)/libbl.a
APP_LIB			:= $(LIB_DIR)/libapp.a

# Sorthands and build definititions
DEFINES			:= -D$(DEVICE_FAMILY) -D$(DEVICE_MODEL) -D$(DEVICE_VARIANT) \
					-DUSE_HAL_DRIVER

BL_DRIVER_SRCS	:= $(BL_SYSTEM) $(HAL_SRCS)
BL_DRIVER_OBJS	:= startup.o $(notdir $(patsubst %.c,%.o,$(BL_DRIVER_SRCS)))

APP_DRIVER_SRCS	:= $(APP_SYSTEM) $(HAL_SRCS)
APP_DRIVER_OBJS	:= startup.o $(notdir $(patsubst %.c,%.o,$(APP_DRIVER_SRCS)))

BL_OBJS			:= $(notdir $(patsubst %.c,%.o,$(BL_SRCS)))
APP_OBJS		:= $(notdir $(patsubst %.c,%.o,$(APP_SRCS)))

AFLAGS			:= $(CORTEX_FLAGS) $(AS_FLAGS)

BL_CFLAGS		:= $(CORTEX_FLAGS) $(COMMON_FLAGS) $(DEFINES) $(BL_INCS)
BL_LDFLAGS		:= $(CORTEX_FLAGS) -T $(BL_LINKER) \
					-Wl,--gc-sections,-Map=$(BL_MAP),--print-memory-usage,--relax \
					--specs=nano.specs --specs=nosys.specs \
					-Wl,--start-group -lc -lm -Wl,--end-group

APP_CFLAGS		:= $(CORTEX_FLAGS) $(COMMON_FLAGS) $(DEFINES) $(APP_INCS)
APP_LDFLAGS		:= $(CORTEX_FLAGS) -T $(APP_LINKER) \
					-Wl,--gc-sections,-Map=$(APP_MAP),--print-memory-usage,--relax \
					--specs=nano.specs --specs=nosys.specs \
					-Wl,--start-group -lc -lm -Wl,--end-group

.PHONY: bl-bin bl-lib app-bin app-lib uploader

# Build recipe for the bootloader library.
bl-lib:
	@echo "\n--> Compiling the bootloader library sources"
	$(CC) $(AFLAGS) -c $(STARTUP_CODE)
	$(CC) $(BL_CFLAGS) -c $(BL_DRIVER_SRCS)

	@echo "\n--> Archiving the bootloader library objects"
	$(AR) $(BL_LIB) $(BL_DRIVER_OBJS)

	@echo "\n--> Removing the bootloader library objects"
	$(RM) $(BL_DRIVER_OBJS)

# Build recipe for the bootloader program
bl-bin:
	@echo "\n--> Compiling the bootloader program sources"
	$(CC) -g3 $(BL_CFLAGS) -c $(BL_SRCS)

	@echo "\n--> Linking the bootloader program objects"
	$(CC) $(BL_LDFLAGS) $(BL_OBJS) -o $(BL_ELF) -lbl -L$(LIB_DIR)

	@echo "\n--> Stripping the bootloader program"
	$(OBJCOPY) -O binary $(BL_ELF) $(BL_BIN)

	@echo "\n--> Removing the bootloader program objects"
	$(RM) $(BL_OBJS)

	@echo "\n--> The bootloader program section sizes"
	@$(SIZE) $(BL_ELF)

	@echo "\n--> The bootloader program format"
	@$(FILE) $(BL_ELF)

# Build recipe for the application library
app-lib:
	@echo "\n--> Compiling the application library sources"
	$(CC) $(AFLAGS) -c $(STARTUP_CODE)
	$(CC) $(APP_CFLAGS) -c $(APP_DRIVER_SRCS)

	@echo "\n--> Archiving the application library objects"
	$(AR) $(APP_LIB) $(APP_DRIVER_OBJS)

	@echo "\n--> Removing the application library objects"
	$(RM) $(APP_DRIVER_OBJS)

# Build recipe for the application program
app-bin:
	@echo "\n--> Compiling the application program sources"
	$(CC) -g3 $(APP_CFLAGS) -c $(APP_SRCS)

	@echo "\n--> Linking the application program objects"
	$(CC) $(APP_LDFLAGS) $(APP_OBJS) -o $(APP_ELF) -lapp -L$(LIB_DIR)

	@echo "\n--> Stripping the application program"
	$(OBJCOPY) -O binary $(APP_ELF) $(APP_BIN)

	@echo "\n--> Removing the application program objects"
	$(RM) $(APP_OBJS)

	@echo "\n--> The application program section sizes"
	@$(SIZE) $(APP_ELF)

	@echo "\n--> The application program format"
	@$(FILE) $(APP_ELF)

# Build receipe for the uploader program
uploader:
	@echo "\n--> Compiling the uploader program"
	gcc -Os -std=gnu2x -march=native $(WARN_FLAGS) utility/*.c -o bin/uploader
