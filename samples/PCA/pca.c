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

int loadSample(char* sampleName, uint8_t nbDataPoints, uint8_t nbDims, uint8_t* outputArray)
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
      outputArray[currentInx++] = value * 10;
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

float findMean(uint8_t* ptr, uint8_t nbValues, uint8_t offset, uint8_t stride)
{
  double mean = 0;
  for(int i = 0; i < nbValues; i++)
  {
    mean += ptr[i * stride + offset];
  }
  return mean / nbValues;
}


void computeCovarianceMatrix(uint8_t* ptr, uint8_t nbElemsCov, uint8_t nbElems, float* output)
{
  float means[nbElemsCov];
  const uint8_t stride = 5;
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


int main()
{
  float covarianceMatrix[4 * 4];
  uint8_t sample[150 * 5];
  loadSample("irisdata.txt", 150, 5, sample);
  computeCovarianceMatrix(sample, 4, 150, covarianceMatrix); 

  float tMatrix[4 * 4];
  memset(tMatrix, 0, 4 * 4 * sizeof(float));
  float vMatrix[4 * 4];
  lanczos(covarianceMatrix, 4, 4, NULL, tMatrix, vMatrix);

  float eigenVectors[4 * 4];
  jacobi(tMatrix, 4, eigenVectors, 4, 0);
  
  printMatrix(tMatrix, 4, 4);

  return 0;
}
