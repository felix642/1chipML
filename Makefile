# Change this section as needed
# target microcontroller unit.
MCU ?= atmega328p # (Arduino UNO)
CLOCK_FREQUENCY ?= 16000000 # 16Mhz clock frequency (Arduino UNO)
DEBUG ?= 1

# the default build target executable:
TARGET ?= gauss_elimination

# the compiler: gcc for C program
GCC = gcc
AVRGCC = avr-gcc

# helpers
AVRSIZE = avr-size

ifeq ($(DEBUG), 1)
	CFLAGS += -g # adds debugging information
endif
CFLAGS += -Wall -Werror # turns on most compiler warnings
CFLAGS += -std=c99 # C99
CFLAGS += -MMD # Get header files dependencies

# avr compiler flags:
AVRCFLAGS += $(CFLAGS)
AVRCFLAGS += -ffunction-sections -fdata-sections # Required for -Wl and --gc-sections
AVRCFLAGS += -Os # Optimize executable size before speed
AVRCFLAGS += -DF_CPU=$(CLOCK_FREQUENCY) # clock frequency
AVRCFLAGS += -flto -fno-fat-lto-objects # link time optimizer (LTO)

# Preprocessor flags
AVRCPPFLAGS += -mmcu=$(MCU) # target

# Extra flags for the linker
ifeq ($(DEBUG), 1)
	AVRLDFLAGS += -g # adds debugging information
endif
AVRLDFLAGS += -mmcu=$(MCU) # target
AVRLDFLAGS += -Wl,--gc-sections # reduce code size by cutting unused sections
AVRLDFLAGS += -Os # Optimize executable size before speed
AVRLDFLAGS += -flto -fuse-linker-plugin # Ensure that the library participates in the LTO optimization process

# loaded libraries
LDLIBS += -lm # Math library

# src variables
SRCDIR = src
SRCBINDIR = $(SRCDIR)/bin
SRCBUILDDIR = $(SRCDIR)/build
SRCOBJDIR = $(SRCBUILDDIR)/gcc
SRCAVROBJDIR = $(SRCBUILDDIR)/avrgcc

SRCFILES = $(notdir $(wildcard $(SRCDIR)/*.c))
DEPFILES = $(addprefix $(SRCOBJDIR)/, $(SRCFILES:.c=.d))
AVRDEPFILES = $(addprefix $(SRCAVROBJDIR)/, $(SRCFILES:.c=.d))

# test variables
TESTDIR = tests
TESTBINDIR = $(TESTDIR)/bin
TESTBUILDDIR = $(TESTDIR)/build

TESTSRCFILES = $(notdir $(wildcard $(TESTDIR)/*.c))
TESTDEPFILES = $(addprefix $(TESTBUILDDIR)/, $(TESTSRCFILES:.c=.d))

all: $(DEPFILES:.d=.o) $(AVRDEPFILES:.d=.o) $(TESTDEPFILES:.d=.o)

validate: build verify

# gcc commands
build: $(SRCOBJDIR)/$(TARGET).o | create_srcbindir
	$(GCC) $^ -o $(SRCBINDIR)/$(TARGET) $(LDLIBS)

# default rule for gcc object files
$(SRCOBJDIR)/%.o: $(SRCDIR)/%.c | create_srcobjdir
	$(GCC) -c $(CFLAGS) $< -o $@


# avr specific commands
verify: $(SRCAVROBJDIR)/$(TARGET).o | create_srcbindir
	$(AVRGCC) $(AVRLDFLAGS) $^ -o $(SRCBINDIR)/$(TARGET).elf $(LDLIBS)
	$(AVRSIZE) -C --mcu=$(MCU) $(SRCBINDIR)/$(TARGET).elf

# default rule for avrgcc object files
$(SRCAVROBJDIR)/%.o: $(SRCDIR)/%.c | create_srcavrobjdir
	$(AVRGCC) -c $(AVRCFLAGS) $(AVRCPPFLAGS) $< -o $@




# test section
test: test_linear_congruential_random_generator test_gauss_elimination test_poly_interpolation
	$(TESTBINDIR)/test_linear_congruential_random_generator
	$(TESTBINDIR)/test_gauss_elimination
	$(TESTBINDIR)/test_poly_interpolation

test_linear_congruential_random_generator: $(TESTBUILDDIR)/test_linear_congruential_random_generator.o $(SRCOBJDIR)/linear_congruential_random_generator.o | create_testbindir
	$(GCC) $^ -o $(TESTBINDIR)/test_linear_congruential_random_generator $(LDLIBS)

test_gauss_elimination: $(TESTBUILDDIR)/test_gauss_elimination.o $(SRCOBJDIR)/gauss_elimination.o | create_testbindir
	$(GCC) $^ -o $(TESTBINDIR)/test_gauss_elimination $(LDLIBS)

test_poly_interpolation: $(TESTBUILDDIR)/test_poly_interpolation.o $(SRCOBJDIR)/poly_interpolation.o | create_testbindir
	$(GCC) $^ -o $(TESTBINDIR)/test_poly_interpolation $(LDLIBS)

# default rule for all test object files
$(TESTBUILDDIR)/%.o: $(TESTDIR)/%.c | create_testbuilddir
	$(GCC) -c $(CFLAGS) $< -o $@



# include dependencies
-include $(DEPFILES)
-include $(AVRDEPFILES)
-include $(TESTDEPFILES)

create_srcbindir:
	@(mkdir -p $(SRCBINDIR))

create_srcobjdir:
	@(mkdir -p $(SRCOBJDIR))

create_srcavrobjdir:
	@(mkdir -p $(SRCAVROBJDIR))

create_testbindir:
	@(mkdir -p $(TESTBINDIR))

create_testbuilddir:
	@(mkdir -p $(TESTBUILDDIR))

clean:
	@(rm -rf $(SRCBUILDDIR))
	@(rm -rf $(SRCBINDIR))
	@(rm -rf $(TESTBUILDDIR))
	@(rm -rf $(TESTBINDIR))

.PHONY: all validate build verify \
test test_linear_congruential_random_generator test_gauss_elimination test_poly_interpolation \
create_srcbindir create_srcobjdir create_srcavrobjdir create_testbindir create_testbuilddir \
clean
