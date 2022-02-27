#+++++++++++++++++++++++++++++++++++++++
# Configuration
#+++++++++++++++++++++++++++++++++++++++
# Select the board to build for.
ifeq ($(BOARD),)
  $(error You must provide a BOARD parameter)
else
  ifeq ($(wildcard boards/$(BOARD)/.),)
    $(error Invalid BOARD specified)
  endif
endif

# If the build directory is not given, make it reflect the board name.
BUILD ?= build-$(BOARD)

#---------------------------------------
# Board-specific
#---------------------------------------
include boards/$(BOARD)/configboard.mk

#---------------------------------------
# ASF4
#---------------------------------------
include asf4.mk

#---------------------------------------
# Application
#---------------------------------------
INC_APP = \
	-Iinclude \
	-Iasf4_conf/$(CHIP_FAMILY)

SRC_APP = \
	src/samsee.c

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Build setup
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CC    = arm-none-eabi-gcc
OBJCOPY   = arm-none-eabi-objcopy

INC = $(INC_APP) $(INC_QPC) $(INC_ASF4)
SRC = $(SRC_APP) $(SRC_QPC) $(SRC_ASF4)
OBJ = $(addprefix $(BUILD)/, $(SRC:.c=.o))

TARGET_BIN   = $(BUILD)/$(BOARD).bin
TARGET_ELF   = $(BUILD)/$(BOARD).elf

COMMON_FLAGS = -mthumb -mcpu=cortex-m0plus -Os -g3 -D__$(CHIP_VARIANT)__ -D__ARM_ARCH=6 -DCONF_DFLL_USBCRM=0
WFLAGS = -Wall
CFLAGS = -c $(COMMON_FLAGS) $(WFLAGS) $(INC)
LFLAGS = $(COMMON_FLAGS) $(WFLAGS) \
	-T$(LINKER_SCRIPT) -mfpu=vfp -mfloat-abi=softfp \
	-specs=nosys.specs -specs=nano.specs \
	-Wl,-Map,$(BUILD)/$(BOARD).map,--cref,--gc-sections


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# R U L E S
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
STEPECHO = @echo

all: $(TARGET_BIN)

# bin is just an objcopy of elf
$(TARGET_BIN): $(TARGET_ELF)
	$(STEPECHO) "Create $@"
	$(OBJCOPY) -O binary -j .vectors -j .text -j .data $^ $@

# elf is all objs linked
$(TARGET_ELF): $(OBJ)
	$(STEPECHO) "LINK $@"
	$(CC) -o $@ $(LFLAGS) $(OBJ)

# objs are compiled from .c's
$(BUILD)/%.o: %.c
	$(STEPECHO) "CC $<"
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -MD -o $@ $<

flash:
	jlink -CommandFile flash.jlink