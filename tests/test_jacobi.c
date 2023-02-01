#include "../src/jacobi.h"
#include "../src/jacobi.c"
#include "stdio.h"

void testMaximumIndex() {

    real_number* mat = {
        0, 1, 2, 
        8, 4, 2, 
        4, 5, 6
    };

    vec_size maxX;
    vec_size maxY;
    maxIndex(mat, 3, &maxX, &maxY);

    if (maxX == 0 && maxY == 1) {
        printf("Test of maximum index passed\n");
    } else {
        printf("Test of maximum index failed\n");
    }
}


int main() {
    testMaximumIndex();

}