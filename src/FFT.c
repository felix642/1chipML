#include "FFT.h"
#include <math.h>
#include <stdlib.h>

static void bitReversal(unsigned length, fft_real* realArray, fft_real* imaginaryArray) {
  
  const unsigned N2 = length >> 1;
  unsigned j = 0;
  for (unsigned i = 0; i < length - 1; ++i) {
    if (i < j) {
      fft_real tmpReal = realArray[i];
      fft_real tmpImaginary = imaginaryArray[i];
      realArray[i] = realArray[j];
      imaginaryArray[i] = imaginaryArray[j];
      realArray[j] = tmpReal;
      imaginaryArray[j] = tmpImaginary;
    }

    unsigned k = N2;
    while (k <= j) {
      j -= k;
      k >>= 1;
    }

    j += k;
  }
}

static int isPowerOfTwo(const unsigned value) {
  return (value != 0) && ((value & (value - 1)) == 0);
}

/**
 * @param length The length of the input vectors. Must be a power of 2
 * @param realArray 1D array containing the real part of the incoming vector.
 * This array will contain the end result of the real part of the FFT
 * @param imaginaryArray 1D array containing the imaginary part of the incoming
 * vector. This array will contain the end result of the imaginary part of the
 * FFT
 * @return 0 if an error occured, 1 otherwise
 *
 */
int FFT(const unsigned length, fft_real* realArray, fft_real* imaginaryArray) {
  if (!isPowerOfTwo(length) || realArray == NULL || imaginaryArray == NULL) {
    return 0;
  }

  bitReversal(length, realArray, imaginaryArray);

  unsigned depth = 1;
  for (unsigned n = 1; n < length; n <<= 1) { // for the levels

    unsigned nElements = depth;
    depth <<= 1;

    // factors for trigonometric recurrence formula
    fft_real wtempSin = sin(M_PI / depth);
    fft_real wRealFactor = -2.0 * wtempSin * wtempSin;
    fft_real wImaginaryFactor = -sin(2.0 * M_PI / depth);

    fft_real wReal = 1.0;
    fft_real wImaginary = 0.0;
    for (unsigned branchElement = 0; branchElement < nElements; ++branchElement) { // branch element

      for (unsigned branch = branchElement; branch < length; branch += depth) { // current group on depth level

        // Complex multiplications, using a butterfly operation
        unsigned i1 = branch + nElements;
        fft_real tReal = wReal * realArray[i1] - wImaginary * imaginaryArray[i1];
        fft_real tImaginary = wReal * imaginaryArray[i1] + wImaginary * realArray[i1];
        realArray[i1] = realArray[branch] - tReal;
        imaginaryArray[i1] = imaginaryArray[branch] - tImaginary;
        realArray[branch] += tReal;
        imaginaryArray[branch] += tImaginary;
      }

      // update twiddle factors using trigonometric recurrence formula
      fft_real wtempReal = wReal;
      wReal += wReal * wRealFactor - wImaginary * wImaginaryFactor;
      wImaginary += wImaginary * wRealFactor + wtempReal * wImaginaryFactor;
    }
  }

  return 1;
}
