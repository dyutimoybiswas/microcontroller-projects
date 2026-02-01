TIVAWARE := $(CURDIR)/tivaware
FREERTOS_VERSION := 11.1.0
FREERTOS := FreeRTOS-Kernel-$(FREERTOS_VERSION)
BUILD    := build
PROJECT_NAME := tiva_vital_monitor

CC := arm-none-eabi-gcc
LD := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy

INCLUDES := -Iinc \
            -I$(TIVAWARE) \
            -I$(TIVAWARE)/inc \
            -I$(FREERTOS)/include \
            -I$(FREERTOS)/portable/GCC/ARM_CM4F

CFLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
          -O0 -g -ffunction-sections -fdata-sections \
          -DPART_TM4C123GH6PM -DTARGET_IS_TM4C123_RB1 \
          $(INCLUDES)

LDFLAGS := -T tm4c.ld -Wl,-Map=$(BUILD)/$(PROJECT_NAME).map -Wl,--gc-sections \
	-L$(TIVAWARE)/driverlib/gcc -ldriver -lm -lc -lnosys

# Source files
SRCS := $(wildcard src/*.c)
FREERTOS_SRCS := $(FREERTOS)/tasks.c \
                 $(FREERTOS)/queue.c \
                 $(FREERTOS)/list.c \
                 $(FREERTOS)/portable/GCC/ARM_CM4F/port.c \
                 $(FREERTOS)/portable/MemMang/heap_4.c

OBJS     := $(SRCS:src/%.c=$(BUILD)/%.o)
FREERTOS_OBJS := $(BUILD)/freertos_tasks.o \
                 $(BUILD)/freertos_queue.o \
                 $(BUILD)/freertos_list.o \
                 $(BUILD)/freertos_port.o \
                 $(BUILD)/freertos_heap_4.o
ELF      := $(BUILD)/$(PROJECT_NAME).elf
BIN      := $(BUILD)/$(PROJECT_NAME).bin

# Default target
all: $(FREERTOS) $(BUILD) $(ELF) $(BIN)

$(BUILD):
	mkdir -p $(BUILD)

# Download FreeRTOS Kernel
$(FREERTOS):
	@echo "Downloading FreeRTOS Kernel $(FREERTOS_VERSION)..."
	curl -L https://github.com/FreeRTOS/FreeRTOS-Kernel/archive/refs/tags/V$(FREERTOS_VERSION).tar.gz | tar -xz

# Compile project sources
$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile FreeRTOS sources
$(BUILD)/freertos_tasks.o: $(FREERTOS)/tasks.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/freertos_queue.o: $(FREERTOS)/queue.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/freertos_list.o: $(FREERTOS)/list.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/freertos_port.o: $(FREERTOS)/portable/GCC/ARM_CM4F/port.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/freertos_heap_4.o: $(FREERTOS)/portable/MemMang/heap_4.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(ELF): $(OBJS) $(FREERTOS_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(FREERTOS_OBJS) -o $@ $(LDFLAGS)

# Convert ELF → BIN
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

size: $(ELF)
	arm-none-eabi-size $(ELF)

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -rf $(FREERTOS)

.PHONY: all clean distclean size
