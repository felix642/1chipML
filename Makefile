CC=gcc
TEST_FOLDER = tests
BUILD_FOLDER = $(TEST_FOLDER)/build

CFLAGS += -g # adds debugging information
CFLAGS += -Wall # turns on most compiler warnings
CFLAGS += -std=c99 # C99
CFLAGS += -I./src # included header files

# loaded libraries
LDLIBS += -lm # Math library

all: build_folder linear_congruential_random_generator gauss_elimination poly_interpolation jacobi

test: all run_all_tests

linear_congruential_random_generator: ./$(TEST_FOLDER)/test_linear_congruential_random_generator.c ./src/linear_congruential_random_generator.c | build_folder
	$(CC) $(CFLAGS) $^ -o $(BUILD_FOLDER)/test_$@.out $(LDLIBS)

gauss_elimination: ./$(TEST_FOLDER)/test_gauss_elimination.c ./src/gauss_elimination.c | build_folder
	$(CC) $(CFLAGS) $^ -o $(BUILD_FOLDER)/test_$@.out $(LDLIBS)

poly_interpolation: ./$(TEST_FOLDER)/test_poly_interpolation.c ./src/poly_interpolation.c | build_folder
	$(CC) $(CFLAGS) $^ -o $(BUILD_FOLDER)/test_$@.out $(LDLIBS)

jacobi: ./tests/test_jacobi.c ./src/jacobi.c
	$(CC) $(CFLAGS) $^ -o test_$@.out $(LDLIBS)

run_all_tests:
	./$(BUILD_FOLDER)/test_linear_congruential_random_generator.out
	./$(BUILD_FOLDER)/test_gauss_elimination.out
	./$(BUILD_FOLDER)/test_poly_interpolation.out

build_folder:
	mkdir -p $(BUILD_FOLDER)

clean:
	rm -rf $(BUILD_FOLDER)
