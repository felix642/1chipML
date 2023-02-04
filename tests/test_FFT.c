#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// project includes
#include <1chipml.h>
#include "bitmap.h"

// Golden model
static int compareFT(const unsigned length, fft_real* incomingReals, 
  fft_real* incomingImgs, fft_real* expectedReals, fft_real* expectedImgs) {

  const fft_real epsilon = 1e-20;

  for (unsigned i = 0; i < length; ++i) {
    if (abs(incomingReals[i] - expectedReals[i]) > epsilon ||
        abs(incomingImgs[i] - expectedImgs[i]) > epsilon) {
      return -1;
    }
  }

  return 0;
}

static void randomTestingFFT() {

  // limit length to 8 for faster testing
  const unsigned length = (unsigned)pow(2, rand() % 9);

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

  clock_t start, end;
  double algorithmTime = 0.0;

  // Algorithm time approximation
  start = clock();
  FFT(length, fftReals, fftImgs, 1);
  end = clock();
  algorithmTime = (double)(end - start) / CLOCKS_PER_SEC;
  printf("The elapsed time for the FFT is %f seconds\n", algorithmTime);

  start = clock();
  DFT(length, dftReals, dftImgs, 1);
  end = clock();
  algorithmTime = (double)(end - start) / CLOCKS_PER_SEC;
  printf("The elapsed time for the DFT is %f seconds\n", algorithmTime);
  
  int isSimilar = compareFT(length, fftReals, fftImgs, dftReals, dftImgs);
  
  printf("Random testing FFT: is the FFT working as intended? ");
  if (isSimilar == 0)
    printf("true\n");
  else
    printf("false\n");
  
}

static void randomTestingFFTI() {

  // limit length to 8 for faster testing
  const unsigned length = (unsigned)pow(2, rand() % 9);

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

  DFT(length, fftReals, fftImgs, 1);
  FFT(length, fftReals, fftImgs, -1);
  
  int isSimilar = compareFT(length, reals, imgs, fftReals, fftImgs);
  
  printf("Random testing inverse FFT: is the FFT working as intended? ");
  if (isSimilar == 0)
    printf("true\n");
  else
    printf("false\n");
  
}

static void knownTestingFFT() {
  const unsigned length = 4;

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
  if (isSimilar == 0)
    printf("true\n");
  else
    printf("false\n");

}

static void knownTestingFFTI() {
  const unsigned length = 4;

  // allocate arrays for testing
  fft_real dftReals[] = {8, 4, 8, 0};
  fft_real dftImgs[] = {0, 0, 0, 0};
  fft_real fftReals[] = {20, 0, 12, 0};
  fft_real fftImgs[] = {0, -4, 0, 4};

  FFT(length, fftReals, fftImgs, -1);

  int isSimilar = compareFT(length, fftReals, fftImgs, dftReals, dftImgs);

  printf("Known testing inverse FFT: is the FFT working as intended? ");
  if (isSimilar == 0)
    printf("true\n");
  else
    printf("false\n");

}

int main() {

  int height = 512;
  int width = 512;
  unsigned char image[height][width][RGB_BYTES];
  unsigned char imageG[height][width][GREY_BYTES];
  double imageReals[height][width];
  double imageImgs[height][width];
  // char* imageFileName = (char*) "bitmapImageRGB.bmp";


  for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
          image[i][j][2] = (unsigned char) ( i * 255 / height );             ///red
          image[i][j][1] = (unsigned char) ( j * 255 / width );              ///green
          image[i][j][0] = (unsigned char) ( (i+j) * 255 / (height+width) ); ///blue

          double y = ( image[i][j][0] * 0.299) + ( image[i][j][1] * 0.587)	+ (image[i][j][2] * 0.114);
          if (i > 128 && i < 384 && j > 128 && j < 384) {
            imageG[i][j][0] = (unsigned char) ( 255 ); 
          }
          else {
            imageG[i][j][0] = (unsigned char) ( 0 ); 
          }

          //imageG[i][j][0] = (unsigned char) ( y ); 
          imageReals[i][j] = imageG[i][j][0];
          imageImgs[i][j] = 0;
      }
  }

  generateBitmapImageRGB((unsigned char*) image, height, width, "bitmapImageRGB.bmp");
  generateBitmapImageGrey((unsigned char*) imageG, height, width, "bitmapImageGrey.bmp");

  /*******2D FFT*********/
  int success = 1;
  
  // start with the rows
  for (int i = 0; i < height; i++) {
    success = success && FFT(width, imageReals[i], imageImgs[i], 1);
  }

  // end with columns
  for (int i = 0; i < width; i++) {
    double nextReals[height];
    double nextImgs[height];
    for (int j = 0; j < height; j++) {
      nextReals[j] = imageReals[j][i];
      nextImgs[j] = imageImgs[j][i];
    }

    success = success && FFT(height, nextReals, nextImgs, 1);

    for (int j = 0; j < height; j++) {
      imageReals[j][i] = nextReals[j];
      imageImgs[j][i] = nextImgs[j];
    }
  }
  /*******END OF 2D FFT*********/
  
  // shift fft matrix
  // Move horizontally
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width / 2; ++j) {
      double tmpReal = imageReals[i][j];
      double tmpImg = imageImgs[i][j];
      imageReals[i][j] = imageReals[i][j + width / 2];
      imageImgs[i][j] = imageImgs[i][j + width / 2];
      imageReals[i][j + width / 2] = tmpReal;
      imageImgs[i][j + width / 2] = tmpImg;
    }
  }

  // Move vertically
  for (int i = 0; i < height / 2; ++i) {
    for (int j = 0; j < width; ++j) {
      double tmpReal = imageReals[i][j];
      double tmpImg = imageImgs[i][j];
      imageReals[i][j] = imageReals[i + height / 2][j];
      imageImgs[i][j] = imageImgs[i + height / 2][j];
      imageReals[i + height / 2][j] = tmpReal;
      imageImgs[i + height / 2][j] = tmpImg;
    }
  }



  // get magnitude
  double* imageAbs = malloc(height * width *sizeof(double));
  for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
          double abs = sqrt(pow(imageReals[i][j], 2) + pow(imageImgs[i][j], 2));
          imageAbs[i * width + j] = abs;
      }
  }

  // get maximum magnitude
  double max = imageAbs[0];
  for (int i = 1; i < height * width; ++i) {
    if (imageAbs[i] > max) {
      max = imageAbs[i];
    }
  }

  // log scalling magnitude and color coding
  double greyColor = 255 / log10(1 + max);
  for (int i = 1; i < height * width; ++i) {
    imageAbs[i] = greyColor * log10(1 + imageAbs[i]);
  }

  printf("FFT result %d\n", success);

  unsigned char imageGreyScaleR[height][width][GREY_BYTES];
  unsigned char imageGreyScaleI[height][width][GREY_BYTES];
  unsigned char imageGreyScaleMag[height][width][GREY_BYTES];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      imageGreyScaleR[i][j][0] = (unsigned char) abs(imageReals[i][j]);
      imageGreyScaleI[i][j][0] = (unsigned char) abs(imageImgs[i][j]);
      imageGreyScaleMag[i][j][0] = (unsigned char) (imageAbs[i * width + j]);
    }
  }


  generateBitmapImageGrey((unsigned char*) imageGreyScaleR, height, width, "real.bmp");
  generateBitmapImageGrey((unsigned char*) imageGreyScaleI, height, width, "imaginary.bmp");
  generateBitmapImageGrey((unsigned char*) imageGreyScaleMag, height, width, "magnitude.bmp");

  printf("Image generated!!\n");

  free(imageAbs);

  return 0;

  int seed = time(NULL);
  printf("seed used : %d\n", seed);
  srand(seed);

  knownTestingFFT();
  knownTestingFFTI();
  randomTestingFFT();
  randomTestingFFTI();

  return 0;
}
