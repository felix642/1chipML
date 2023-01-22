# This makefile is intended to be used only by developpers

# target microcontroller unit
MCU = atmega328p # (Arduino UNO)
CLOCK_FREQUENCY = 16000000 # 16Mhz clock frequency (Arduino UNO)

# the compiler: gcc for C program, define as g++ for C++
GCC = gcc
AVRGCC = avr-gcc

# helpers
AVRSIZE = avr-size


CFLAGS += -Wall # turns on most compiler warnings
CFLAGS += -g # adds debugging information
CFLAGS += -std=c99 # C99
CFLAGS += -MMD # Get header files dependencies

# avr compiler flags:
AVRCFLAGS += $(CFLAGS)
AVRCFLAGS += -ffunction-sections -fdata-sections # Required for -Wl and --gc-sections
AVRCFLAGS += -Os # Optimize executable size before speed
AVRCFLAGS += -DF_CPU=$(CLOCK_FREQUENCY) # clock frequency
AVRCFLAGS += -flto -fno-fat-lto-objects # link time optimizer (LTO): faster compile time and forces LTO

# Preprocessor flags
AVRCPPFLAGS += -mmcu=$(MCU) # target

# Extra flags for the linker
AVRLDFLAGS += -g # adds debugging information
AVRLDFLAGS += -mmcu=$(MCU) # target
AVRLDFLAGS += -Wl,--gc-sections # reduce code size by cutting unused sections
AVRLDFLAGS += -Os # Optimize executable size before speed
AVRLDFLAGS += -flto -fuse-linker-plugin # Ensure that the library participates in the LTO optimization process

# loaded libraries
LDLIBS += -lm # Math library

BINDIR = bin
BUILDDIR = build
OBJDIR = $(BUILDDIR)/gcc
AVROBJDIR = $(BUILDDIR)/avrgcc

SRCFILES = $(wildcard *.c)
DEPFILES = $(addprefix $(OBJDIR)/, $(SRCFILES:.c=.d))
AVRDEPFILES = $(addprefix $(AVROBJDIR)/, $(SRCFILES:.c=.d))

# the default build target executable:
TARGET ?= gauss_elimination

all: build verify

myEcho:
	@echo HeyThere

# gcc commands
build: $(OBJDIR)/$(TARGET).o | create_bindir
	$(GCC) $^ -o $(BINDIR)/$(TARGET) $(LDLIBS)

compile: $(OBJDIR)/$(TARGET).o

# default rules for all files
$(OBJDIR)/%.o: %.c | create_objdir
	$(GCC) -c $(CFLAGS) $< -o $@


# avr specific commands
verify: $(AVROBJDIR)/$(TARGET).o | create_bindir
	$(AVRGCC) $(AVRLDFLAGS) $^ -o $(BINDIR)/$(TARGET).elf $(LDLIBS)
	$(AVRSIZE) -C --mcu=$(MCU) $(BINDIR)/$(TARGET).elf

# default rules for all files
$(AVROBJDIR)/%.o: %.c | create_avrobjdir
	$(AVRGCC) -c $(AVRCFLAGS) $(AVRCPPFLAGS) $< -o $@


# include dependencies
-include $(DEPFILES)
-include $(AVRDEPFILES)

create_bindir:
	mkdir -p $(BINDIR)

create_objdir:
	mkdir -p $(OBJDIR)

create_avrobjdir:
	mkdir -p $(AVROBJDIR)

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(BINDIR)