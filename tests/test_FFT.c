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

static void randomTestingFFT() {

  // limit length to 8 for faster testing
  unsigned length = (unsigned)pow(2, rand() % 9);

  // allocate arrays for testing
  fft_real fftReals[length];
  fft_real fftImgs[length];
  fft_real dftReals[length];
  fft_real dftImgs[length];

  for (unsigned i = 0; i < length; ++i) {
    fftReals[i] = (float)rand() / RAND_MAX;
    fftImgs[i] = 0;
    dftReals[i] = fftReals[i];
    dftImgs[i] = fftImgs[i];
  }

  FFT(length, fftReals, fftImgs, 1);
  DFT(length, dftReals, dftImgs);
  
  int isSimilar = compareFT(length, fftReals, fftImgs, dftReals, dftImgs);
  
  printf("Random testing FFT: is the FFT working as intended? ");
  if (isSimilar)
    printf("true\n");
  else
    printf("false\n");
  
}

static void randomTestingFFTI() {

  // limit length to 8 for faster testing
  unsigned length = (unsigned)pow(2, rand() % 9);

  // allocate arrays for testing
  fft_real reals[length];
  fft_real imgs[length];
  fft_real fftReals[length];
  fft_real fftImgs[length];

  for (unsigned i = 0; i < length; ++i) {
    reals[i] = (float)rand() / RAND_MAX;
    imgs[i] = 0;
    fftReals[i] = reals[i];
    fftImgs[i] = imgs[i];
  }

  DFT(length, fftReals, fftImgs);
  FFT(length, fftReals, fftImgs, -1);
  
  int isSimilar = compareFT(length, reals, imgs, fftReals, fftImgs);
  
  printf("Random testing inverse FFT: is the FFT working as intended? ");
  if (isSimilar)
    printf("true\n");
  else
    printf("false\n");
  
}

static void knownTestingFFT() {
  unsigned length = 4;

  // allocate arrays for testing
  fft_real fftReals[] = {8, 4, 8, 0};
  fft_real fftImgs[] = {0, 0, 0, 0};
  fft_real dftReals[] = {20, 0, 12, 0};
  fft_real dftImgs[] = {0, -4, 0, 4};

  FFT(length, fftReals, fftImgs, 1);
  printf("FFT output:\n");
  for (unsigned i = 0; i < length; ++i) {
    printf("real: %f , img: %f\n", fftReals[i], fftImgs[i]);
  }

  int isSimilar = compareFT(length, fftReals, fftImgs, dftReals, dftImgs);

  printf("Known testing FFT: is the FFT working as intended? ");
  if (isSimilar)
    printf("true\n");
  else
    printf("false\n");

}

static void knownTestingFFTI() {
  unsigned length = 4;

  // allocate arrays for testing
  fft_real dftReals[] = {8, 4, 8, 0};
  fft_real dftImgs[] = {0, 0, 0, 0};
  fft_real fftReals[] = {20, 0, 12, 0};
  fft_real fftImgs[] = {0, -4, 0, 4};

  FFT(length, fftReals, fftImgs, -1);

  int isSimilar = compareFT(length, fftReals, fftImgs, dftReals, dftImgs);

  printf("Known testing inverse FFT: is the FFT working as intended? ");
  if (isSimilar)
    printf("true\n");
  else
    printf("false\n");

}

int main() {

  int seed = 1675474412;//time(NULL);
  printf("seed used : %d\n", seed);
  srand(seed);

  knownTestingFFT();
  knownTestingFFTI();
  randomTestingFFT();
  randomTestingFFTI();

  return 0;
}
