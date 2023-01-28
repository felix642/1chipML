#include "../src/FFT.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Golden model
static void DFT(const unsigned length, fft_real* incomingReals, fft_real* incomingImaginaries) {

  fft_real* outputReals = calloc(length, sizeof(fft_real));
  fft_real* outputImaginaries = calloc(length, sizeof(fft_real));

  for (unsigned i = 0; i < length; ++i) {
    fft_real exponentBase = -2.0 * M_PI * (fft_real)i / (fft_real)length;
    for (unsigned k = 0; k < length; ++k) {
      fft_real wCos = cos(k * exponentBase); // real part
      fft_real wSin = sin(k * exponentBase); // imaginary part
      outputReals[i] += (incomingReals[k] * wCos - incomingImaginaries[k] * wSin); // real
      outputImaginaries[i] += (incomingReals[k] * wSin + incomingImaginaries[k] * wCos); // imaginary
    }
  }

  for (unsigned i = 0; i < length; ++i) {
    incomingReals[i] = outputReals[i];
    incomingImaginaries[i] = outputImaginaries[i];
  }

  free(outputReals);
  free(outputImaginaries);
}

static int compareFT(const unsigned length, fft_real* incomingReals, 
  fft_real* incomingImgs, fft_real* expectedReals, fft_real* expectedImgs) {

  const fft_real epsilon = 1e-20;

  for (unsigned i = 0; i < length; ++i) {
    if (abs(incomingReals[i] - expectedReals[i]) > epsilon ||
        abs(incomingImgs[i] - expectedImgs[i]) > epsilon) {
      return 0;
    }
  }

  return 1;
}

static void randomTesting() {

  // limit length to 8 for faster testing
  unsigned length = (unsigned)pow(2, rand() % 9);

  // allocate arrays for testing
  fft_real* fftReals = calloc(length, sizeof(fft_real));
  fft_real* fftImgs = calloc(length, sizeof(fft_real));
  fft_real* dftReals = calloc(length, sizeof(fft_real));
  fft_real* dftImgs = calloc(length, sizeof(fft_real));

  for (unsigned i = 0; i < length; ++i) {
    fftReals[i] = (float)rand() / RAND_MAX;
    dftReals[i] = fftReals[i];
  }

  FFT(length, fftReals, fftImgs);
  DFT(length, dftReals, dftImgs);
  
  int isSimilar = compareFT(length, fftReals, fftImgs, dftReals, dftImgs);
  
  printf("Random testing: is the FFT working as intended? ");
  if (isSimilar)
    printf("true\n");
  else
    printf("false\n");

  // free remaining memory
  free(fftReals);
  free(fftImgs);
  free(dftReals);
  free(dftImgs);
}

static void knownTesting() {
  // limit length to 8 for faster testing
  unsigned length = 4;

  // allocate arrays for testing
  fft_real* fftReals = calloc(length, sizeof(fft_real));
  fft_real* fftImgs = calloc(length, sizeof(fft_real));
  fft_real* dftReals = calloc(length, sizeof(fft_real));
  fft_real* dftImgs = calloc(length, sizeof(fft_real));

  fftReals[0] = 8;
  fftReals[1] = 4;
  fftReals[2] = 8;
  fftReals[3] = 0;

  dftReals[0] = 20;
  dftReals[1] = 0;
  dftReals[2] = 12;
  dftReals[3] = 0;

  dftImgs[0] = 0;
  dftImgs[1] = -4;
  dftImgs[2] = 0;
  dftImgs[3] = 4;

  FFT(length, fftReals, fftImgs);
  printf("FFT output:\n");
  for (unsigned i = 0; i < length; ++i) {
    printf("real: %f , img: %f\n", fftReals[i], fftImgs[i]);
  }

  int isSimilar = compareFT(length, fftReals, fftImgs, dftReals, dftImgs);

  printf("Known testing: is the FFT working as intended? ");
  if (isSimilar)
    printf("true\n");
  else
    printf("false\n");

  // free remaining memory
  free(fftReals);
  free(fftImgs);
  free(dftReals);
  free(dftImgs);
}

int main() {

  int seed = time(NULL);
  printf("seed used : %d\n", seed);
  srand(seed);

  randomTesting();
  knownTesting();

  return 0;
}
