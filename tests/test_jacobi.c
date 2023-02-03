#include "../src/jacobi.h"
#include "stdio.h"

#define PI 3.141592653589793238
#define EPSILON 0.001

int testMaximumIndex(real_number* input, vec_size size, vec_size expectedRow, vec_size expectedCol) {

    vec_size maxRow;
    vec_size maxCol;
    jacobiMaxIndex(input, size, &maxRow, &maxCol);

    if (maxRow == expectedRow && maxCol == expectedCol) {
        printf("Success : Test maximum index\n");
        return 0;
    } else {
        printf("Fail : Test maximum index. Row should be 1 and column should be 0, but row is %d and column is %d\n", maxRow, maxCol);
        return 1;
    }
}

int testJacobi(real_number* input, real_number* expectedEigenValues, real_number* expectedEigenVectors, vec_size size, vec_size iterations, vec_size cyclic) {

    vec_size squaredSize = size * size;

    real_number mat1[squaredSize];
    memcpy(mat1, input, sizeof(real_number) * squaredSize);

    real_number output[squaredSize];
    memset(output, 0, sizeof(real_number) * squaredSize);

    jacobi(mat1, size, output, iterations, cyclic);

    for (vec_size i = 0; i < squaredSize; ++i) {
        real_number diff = fabs(output[i] - expectedEigenVectors[i]);
        if (diff > EPSILON) {
            vec_size x = i % size;
            vec_size y = i / size;
            printf("Fail : Test jacobi with cyclic = %d. Value of eigenvector matrix at (%d, %d) should be %f, but is %f (difference is %f, but must be less than %f)\n", cyclic, x, y, expectedEigenVectors[i], output[i], diff, EPSILON);
            return 1;
        }
    }

    for (vec_size i = 0; i < squaredSize; ++i) {
        real_number diff = fabs(mat1[i] - expectedEigenValues[i]);
        if (diff > EPSILON) {
            vec_size x = i % size;
            vec_size y = i / size;
            printf("Fail : Test jacobi with cyclic = %d. Value of eigenvalues matrix at (%d, %d) should be %f, but is %f (difference is %f, but must be less than %f)\n", cyclic, x, y, expectedEigenValues[i], mat1[i], diff, EPSILON);
            return 1;
        }
    }

    printf("Success : Test jacobi method with cyclic = %d\n", cyclic);

    return 0;
}

int main() {

    //////////////////////////////////////////////////
    // Test Maximum Index
    //////////////////////////////////////////////////

    real_number matTestMaxIndex[9] = {
        0, 1, 2, 
        8, 4, 2, 
        4, 5, 6
    };

    int fail = testMaximumIndex(matTestMaxIndex, 3, 1, 0);

    //////////////////////////////////////////////////
    // Test Jacobi
    //////////////////////////////////////////////////

    real_number matTestJacobi[9] = {
        1, 2, 1,
        2, 1, 2,
        1, 2, 1
    };

    real_number expectedEigenValues[9] = {
        4.372281,   0.000000,   0.000000,
        0.000000,   0.000000,   0.000000,
        0.000000,   0.000000,   -1.372281,
    };    

    real_number expectedEigenVectors[9] = {
        0.541774,   -0.707107,  0.454401 ,
        0.642621,   0.000000 ,  -0.766185,
        0.541774,   0.707107 ,  0.454401 ,
    };

    fail |= testJacobi(matTestJacobi, expectedEigenValues, expectedEigenVectors, 3, 10, 1);
    fail |= testJacobi(matTestJacobi, expectedEigenValues, expectedEigenVectors, 3, 10, 0);

    return fail;
}