#include <1chipml.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Golden model
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
  fft_real* fftReals = malloc(length * sizeof(fft_real));
  fft_real* fftImgs = malloc(length * sizeof(fft_real));
  fft_real* dftReals = malloc(length * sizeof(fft_real));
  fft_real* dftImgs = malloc(length * sizeof(fft_real));

  for (unsigned i = 0; i < length; ++i) {
    fftReals[i] = (float)rand() / RAND_MAX;
    fftImgs[i] = 0;
    dftReals[i] = fftReals[i];
    dftImgs[i] = fftImgs[i];
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
  unsigned length = 4;

  // allocate arrays for testing
  fft_real fftReals[] = {8, 4, 8, 0};
  fft_real fftImgs[] = {0, 0, 0, 0};
  fft_real dftReals[] = {20, 0, 12, 0};
  fft_real dftImgs[] = {0, -4, 0, 4};

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

}

int main() {

  int seed = time(NULL);
  printf("seed used : %d\n", seed);
  srand(seed);

  randomTesting();
  knownTesting();

  return 0;
}
