CC=gcc

CFLAGS += -g # adds debugging information
CFLAGS += -Wall # turns on most compiler warnings
CFLAGS += -std=c99 # C99
CFLAGS += -I./src # included header files

# loaded libraries
LDLIBS += -lm # Math library

all: linear_congruential_random_generator gauss_elimination poly_interpolation FFT

test: all run_all_tests

linear_congruential_random_generator: ./tests/test_linear_congruential_random_generator.c ./src/linear_congruential_random_generator.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

gauss_elimination: ./tests/test_gauss_elimination.c ./src/gauss_elimination.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

poly_interpolation: ./tests/test_poly_interpolation.c ./src/poly_interpolation.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

FFT:
	$(CC) ./tests/test_FFT.c ./src/FFT.c -lm -Wall -std=c99 -I./src -o test_FFT.out



run_all_tests:
	./test_linear_congruential_random_generator.out
	./test_gauss_elimination.out
	./test_poly_interpolation.out
	./test_FFT.out

clean:
	rm -rf test_*.out
