#include "DFT.h"
#include <math.h>
#include <stdlib.h>
#include <string.h> // for memset

/**
 * @param length The length of the input vectors.
 * @param realArray 1D array containing the real part of the incoming vector.
 * This array will contain the end result of the real part of the DFT
 * @param imaginaryArray 1D array containing the imaginary part of the incoming
 * vector. This array will contain the end result of the imaginary part of the
 * DFT
 */
void DFT(const unsigned length, dft_real* realArray, dft_real* imaginaryArray) {

  dft_real outputReals[length];
  dft_real outputImaginaries[length];
  memset(outputReals, 0.0, length * sizeof(dft_real));
  memset(outputReals, 0.0, length * sizeof(dft_real));

  for (unsigned i = 0; i < length; ++i) {
    dft_real exponentBase = -2.0 * M_PI * (dft_real)i / (dft_real)length;
    for (unsigned k = 0; k < length; ++k) {
      dft_real wCos = cos(k * exponentBase); // real part
      dft_real wSin = sin(k * exponentBase); // imaginary part
      outputReals[i] += (realArray[k] * wCos - imaginaryArray[k] * wSin); // real
      outputImaginaries[i] += (realArray[k] * wSin + imaginaryArray[k] * wCos); // imaginary
    }
  }

  // place result in arrays
  for (unsigned i = 0; i < length; ++i) {
    realArray[i] = outputReals[i];
    imaginaryArray[i] = outputImaginaries[i];
  }
}
