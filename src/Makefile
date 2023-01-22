# This makefile is intended to be used only by developpers

# target microcontroller unit
MCU = atmega328p # (Arduino UNO)
CLOCK_FREQUENCY = 16000000 # 16Mhz clock frequency (Arduino UNO)

# the compiler: gcc for C program, define as g++ for C++
GCC = gcc
AVRGCC = avr-gcc

# helpers
AVRSIZE = avr-size

# compiler flags:
CFLAGS += -Wall # turns on most compiler warnings
CFLAGS += -g # adds debugging information
CFLAGS += -std=c99 # C99
# CFLAGS += -ffunction-sections -fdata-sections # Required for -Wl and --gc-sections
# CFLAGS += -Os # Optimize executable size before speed
CFLAGS += -DF_CPU=$(CLOCK_FREQUENCY) # clock frequency
CFLAGS += -flto -fno-fat-lto-objects # link time optimizer (LTO): faster compile time and forces LTO

# Preprocessor flags
CPPFLAGS += -mmcu=$(MCU) # target

# Extra flags for the linker
LDFLAGS += -g # adds debugging information
LDFLAGS += -mmcu=$(MCU) # target
# LDFLAGS += -Wl,--print-memory-usage # print memory usage
# LDFLAGS += -Wl,--gc-sections # reduce code size by cutting unused sections
# LDFLAGS += -Os # Optimize executable size before speed
LDFLAGS += -flto -fuse-linker-plugin # Ensure that the library participates in the LTO optimization process

# loaded libraries
LDLIBS += -lm # Math library

# the build target executable:
VERIFY_TARGET = gauss_elimination

test:
	$(AVRGCC) --version

verify: $(VERIFY_TARGET).o
	$(AVRGCC) $(LDFLAGS) $^ -o $(VERIFY_TARGET).elf $(LDLIBS)
	$(AVRSIZE) -C -x --mcu=$(MCU) $(VERIFY_TARGET).elf

$(VERIFY_TARGET).o: $(VERIFY_TARGET).c $(VERIFY_TARGET).h
	$(AVRGCC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

gauss_elimination.o: gauss_elimination.c gauss_elimination.h
	$(AVRGCC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf *.o
	rm -rf *.elf