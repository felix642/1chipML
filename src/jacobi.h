#ifndef _JACOBI_LIB_
#define _JACOBI_LIB_

#include <stdbool.h>

typedef double real_number;
typedef unsigned char vec_size;

#ifdef __cplusplus
extern "C" {
#endif

void jacobi(real_number* input_mat, vec_size* size, real_number* output_mat, bool cyclic);

#ifdef __cplusplus
}
#endif

#endif