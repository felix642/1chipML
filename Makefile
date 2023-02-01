CC=gcc

CFLAGS += -g # adds debugging information
CFLAGS += -Wall # turns on most compiler warnings
CFLAGS += -std=c99 # C99
CFLAGS += -I./src # included header files

# loaded libraries
LDLIBS += -lm # Math library

all: linear_congruential_random_generator gauss_elimination poly_interpolation genetic genetic_low_memory gradient_descent monte_carlo jacobi

test: all run_all_tests

genetic : ./tests/test_genetic.c ./src/genetic.c  ./src/linear_congruential_random_generator.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

genetic_low_memory : ./tests/test_genetic_low_memory.c ./src/genetic_low_memory.c  ./src/linear_congruential_random_generator.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

linear_congruential_random_generator: ./tests/test_linear_congruential_random_generator.c ./src/linear_congruential_random_generator.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

gauss_elimination: ./tests/test_gauss_elimination.c ./src/gauss_elimination.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

poly_interpolation: ./tests/test_poly_interpolation.c ./src/poly_interpolation.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

gradient_descent: ./tests/test_gradient_descent.c ./src/gradient_descent.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

monte_carlo: ./tests/test_monte_carlo.c ./src/monte_carlo.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

jacobi: ./tests/test_jacobi.c ./src/jacobi.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

run_all_tests:
	./test_linear_congruential_random_generator.out
	./test_gauss_elimination.out
	./test_poly_interpolation.out
	./test_gradient_descent.out
	./test_genetic.out
	./test_genetic_low_memory.out
	./test_jacobi.out
	./test_monte_carlo.out

clean:
	rm -rf test_*.out
