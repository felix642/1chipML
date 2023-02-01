#include "jacobi.h"

/**
 * Converts a 2d coordinate in a square matrix to an index in a 2d array
*/
static inline vec_size coordToIndex(vec_size x, vec_size y, vec_size size) {
    return x * size + y;
}

/**
 * Finds the index of the element with the highest absolute value
 * @param matrix must be a square matrix
 * @param size size of the matrix
 * @param maxX pointer where the x coordinate of the maximum element will be stored
 * @param maxY pointer where the y coordinate of the maximum element will be stored 
*/
static void maxIndex(real_number* matrix, vec_size size, vec_size* maxX, vec_size* maxY) {
    vec_size maxElement = matrix[1]; // Cannot be first element since it's on the diagonal
    vec_size currentMaxX = 0;
    vec_size currentMaxY = 1;

    // Multiplication could overflow if vec_size is not defined properly
    // Iteration starts at 2 since we assume that the first element is the max at that point
    for (vec_size i = 2; i < size * size; ++i) { 
        vec_size currentX = i % size;
        vec_size currentY = i / size;
        real_number elem = abs(matrix[coordToIndex(currentX, currentY, size)]);

        if (currentX != currentY && elem > maxElement) {
            currentMaxX = currentX;
            currentMaxY = currentY;
            maxElement = elem;
        }
    }

    *maxX = currentMaxX;
    *maxY = currentMaxY;
}


/**
 * Multiplies 2 matrices together using this equation : output = firstMatrix x secondMatrix
 * @param firstMatrix first matrix to multiply of size m * n
 * @param secondMatrix second matrix to multiply of size n * p
 * @param size array of size 3 containing m, n and p
 * @param output matrix resulting of the matrix multiplication of the given matrices
 */
static void matrixMultiply(real_number* firstMatrix, real_number* secondMatrix, vec_size size[3], real_number* output) {
    for (vec_size i = 0; i < size[0]; ++i) {
        for (vec_size j = 0; j < size[2]; ++j) {
            for (vec_size k = 0; k < size[1]; ++k) {
                output[coordToIndex(i, j, size[2])] += firstMatrix[coordToIndex(i, k, size[1])] * secondMatrix[coordToIndex(k, j, size[1])];
            }
        }
    }
}

/**
 * Computes the jacobi method to find the eigen values and eigenvectors of the input matrix
 * @param input_mat matrix whose eigenvalues and eigenvectors we are looking for (will contain the eigenvalues after this function has been called)
 * @param size size of the matrix (array of size 2)
 * @param output_mat matrix containing every eigenvector of the input matrix (same size as the input matrix)
*/
void jacobi(real_number* input_mat, vec_size* size, real_number* output_mat, bool cyclic) {



}