/**
* Demonstration of the eigenvalues / eigenvectors algorithm
* using PCA algorithm
*
* This example will load a dataset of Iris flower and perform
* a PCA on it to reduce the space while keeping the most important information
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lanczos.h>
#include <jacobi.h>

int loadSample(char* sampleName, uint8_t nbDataPoints, uint8_t nbDims, float* outputArray)
{
  FILE* file = fopen(sampleName, "r");
  if(file == NULL)
  {
    return 1;
  }
  
  size_t currentInx = 0;
  char str[25];
  while(fgets(str, 25, file) != NULL)
  {
    // If the string is a comment
    if(str[0] == '%')
    {
      continue;
    }
    char* ptr = strtok(str, "\t");
    while(ptr != NULL)
    {
      double value = atof(ptr);
      ptr = strtok(NULL, "\t");
      // We store values in uint8_t to save space
      outputArray[currentInx++] = value;
    }
  }

  fclose(file);
  return 0;
}

void printMatrix(float* ptr, int row, int column)
{
  for(int i = 0; i < row; ++i)
  {
    for(int j = 0; j < column; ++j)
    {
      printf("%f ", ptr[i * column + j]);
    }
    printf("\n");
  }
}

float findMean(float* ptr, uint8_t nbValues, uint8_t offset, uint8_t stride)
{
  double mean = 0;
  for(int i = 0; i < nbValues; i++)
  {
    mean += ptr[i * stride + offset];
  }
  return mean / nbValues;
}


void computeCovarianceMatrix(float* ptr, uint8_t nbElemsCov, uint8_t nbElems, float* output)
{
  float means[nbElemsCov];
  const uint8_t stride = 4;
  for(int i = 0; i < nbElemsCov; ++i)
  {
    means[i] = findMean(ptr, nbElems, i, stride);
  }

  for(int j = 0; j < nbElemsCov; ++j)
  {
    for(int k = 0; k < nbElemsCov; ++k)
    {
      float tmp = 0;
      for(int i = 0; i < nbElems; ++i)
      {
        tmp += ((ptr[i * stride + j] - means[j]) * (ptr[i * stride + k] - means[k]));
      }
      tmp /= (nbElems - 1);
      output[j * nbElemsCov + k] = tmp * 0.01f; 
    }
  }
}

void writeDataCsv(float* data, int nbRows, int nbColumns, char* fileName)
{
  FILE* file = fopen(fileName, "w");
  for(int i = 0; i < nbRows; ++i)
  {
      fprintf(file, "%f, %f\n", data[i * nbColumns], data[i * nbColumns + 1]);
  }

  fclose(file);
}


int main()
{
  float covarianceMatrix[4 * 4];
  float sample[100 * 4];
  // load sample from text file and compute covariance Matrix
  loadSample("irisdata.txt", 100, 4, sample);
  computeCovarianceMatrix(sample, 4, 100, covarianceMatrix); 

  float tMatrix[4 * 4];
  memset(tMatrix, 0, 4 * 4 * sizeof(float));
  float vMatrix[4 * 4];
  // Apply lanczos for tridiagonalization
  lanczos(covarianceMatrix, 4, 4, NULL, tMatrix, vMatrix);

  float eigenVectors[4 * 4];
  // Apply jacobi to find eigenvalues and eigenvectors of matrix T
  jacobi(tMatrix, 4, eigenVectors, 25, 1);
  float realEigenVectors[4 * 4];
  uint_least8_t size[] = {4, 4, 4};
  // Transform eigenvectors from Matrix T to covariance Matrix using matrix V
  matrixMultiply(vMatrix, eigenVectors, size, realEigenVectors);
  
  float transposeRealVectors[4 * 4];
  // Transpose matrix to extract eigenvectors more easily
  transpose(realEigenVectors, transposeRealVectors, size);

  float w_matrix[2 * 4];
  memcpy(w_matrix, transposeRealVectors, 4 * sizeof(float));
  memcpy(&w_matrix[4], &transposeRealVectors[4 * 2], 4 * sizeof(float));

  float wTranspose[2 * 4];
  uint_least8_t size_2[] = {2, 4};
  transpose(w_matrix, wTranspose, size_2);


  float outputPCA[100 * 2];
  uint_least8_t size_final[] = {100, 4, 2};
  matrixMultiply(sample, w_matrix, size_final, outputPCA);

  printMatrix(outputPCA, 100, 2);

  writeDataCsv(outputPCA, 100, 2, "output.csv");
  
  
  return 0;
}
