#ifndef _JACOBI_LIB_
#define _JACOBI_LIB_

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITERATION 100
#define DIGITS_PRECISION 15

typedef double real_number;
typedef unsigned int vec_size;

#ifndef DIGITS_PRECISION
    #define DIGITS_PRECISION 15
#endif

#ifdef __cplusplus
extern "C" {
#endif

void jacobiMaxIndex(real_number* matrix, vec_size size, vec_size* maxX, vec_size* maxY);
void jacobiMatrixMultiply(real_number* firstMatrix, real_number* secondMatrix, vec_size size[3], real_number* output, vec_size transposeFirstMatrix);
void jacobiCreateRotationMatrix(real_number* input, vec_size row, vec_size col, real_number* output, vec_size size);
void jacobi(real_number* inputMatrix, vec_size size, real_number* outputMatrix, vec_size iterations, vec_size cyclic);

#ifdef __cplusplus
}
#endif

#endif