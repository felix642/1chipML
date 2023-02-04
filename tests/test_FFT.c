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

static void fftShift(unsigned int height, unsigned int width, double** reals, double** imgs) {
  // shift fft matrix
  // Move horizontally
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width / 2; ++j) {
      double tmpReal = reals[i][j];
      double tmpImg = imgs[i][j];
      reals[i][j] = reals[i][j + width / 2];
      imgs[i][j] = imgs[i][j + width / 2];
      reals[i][j + width / 2] = tmpReal;
      imgs[i][j + width / 2] = tmpImg;
    }
  }

  // Move vertically
  for (int i = 0; i < height / 2; ++i) {
    for (int j = 0; j < width; ++j) {
      double tmpReal = reals[i][j];
      double tmpImg = imgs[i][j];
      reals[i][j] = reals[i + height / 2][j];
      imgs[i][j] = imgs[i + height / 2][j];
      reals[i + height / 2][j] = tmpReal;
      imgs[i + height / 2][j] = tmpImg;
    }
  }
}

static int fft2D(unsigned int height, unsigned int width, double** reals, double** imgs, int dir) {
  int success = 1;
  
  // start with the rows
  for (int i = 0; i < height; i++) {
    success = success && FFT(width, reals[i], imgs[i], dir);
  }

  // end with columns
  for (int i = 0; i < width; i++) {
    double nextReals[height];
    double nextImgs[height];
    for (int j = 0; j < height; j++) {
      nextReals[j] = reals[j][i];
      nextImgs[j] = imgs[j][i];
    }

    success = success && FFT(height, nextReals, nextImgs, dir);

    for (int j = 0; j < height; j++) {
      reals[j][i] = nextReals[j];
      imgs[j][i] = nextImgs[j];
    }
  }
  return success;
}

int main() {

  BITMAPINFOHEADER bitmapInfoHeader;
  unsigned char* imageData;
  imageData = readBitmapImage("cameraman-image.bmp", &bitmapInfoHeader);

  

  int height = 512;
  int width = 512;
  unsigned char image[height][width][RGB_BYTES];
  unsigned char imageG[height][width][GREY_BYTES];

  double** imageReals = malloc(height * sizeof(double*));
  double** imageImgs = malloc(height * sizeof(double*));
  double** kernelReals = malloc(height * sizeof(double*));
  double** kernelImgs = malloc(height * sizeof(double*));
  for (int i = 0; i < height; ++i) {
    imageReals[i] = malloc(width * sizeof(double));
    imageImgs[i] = malloc(width * sizeof(double));
    kernelReals[i] = malloc(width * sizeof(double));
    kernelImgs[i] = malloc(width * sizeof(double));
  }

  //double imageImgs[height][width];
  // char* imageFileName = (char*) "bitmapImageRGB.bmp";

  /*
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

          imageG[i][j][0] = (unsigned char) ( y ); 
          imageReals[i][j] = imageG[i][j][0];
          imageImgs[i][j] = 0;
      }
  }
  */

  for (int i = 0; i < bitmapInfoHeader.biHeight; ++i) {
    for (int j = 0; j < bitmapInfoHeader.biWidth; ++j) {
      double y = ( imageData[i* bitmapInfoHeader.biWidth * 3 + j * 3] * 0.299) 
      + ( imageData[i* bitmapInfoHeader.biWidth * 3 + j * 3 + 1] * 0.587)	
      + ( imageData[i* bitmapInfoHeader.biWidth * 3 + j * 3 + 2] * 0.114);

      imageG[i][j][0] = (unsigned char) ( y ); 
      imageReals[i][j] = imageG[i][j][0];
      imageImgs[i][j] = 0;
    }
  }

  generateBitmapImageRGB((unsigned char*) image, height, width, "bitmapImageRGB.bmp");
  generateBitmapImageGrey((unsigned char*) imageG, height, width, "bitmapImageGrey.bmp");

  // Compute 2D fft
  int success = fft2D(height, width, imageReals, imageImgs, 1);
  //success = fft2D(height, width, imageReals, imageImgs, -1);
  printf("FFT result %d\n", success);
  


  // convolution matrix generation
  unsigned int kernelHeight = 11;
  unsigned int kernelWidth = 11;


  unsigned int totalPaddingVertical = height - kernelHeight;
  unsigned int totalPaddingHorizontal = width - kernelWidth;
  unsigned int paddingTop = (totalPaddingVertical + 1) / 2;
  unsigned int paddingBottom = totalPaddingVertical / 2;
  unsigned int paddingLeft = (totalPaddingHorizontal + 1) / 2;
  unsigned int paddingRight = totalPaddingHorizontal / 2;

  for(int i = 0; i < height; ++i) {
    for(int j = 0; j < width; ++j) {
      kernelImgs[i][j] = 0;
      if (i >= paddingTop && i < paddingTop + kernelHeight && j >= paddingLeft && j < paddingLeft + kernelWidth) {
        kernelReals[i][j] = 1.0 / (kernelHeight * kernelWidth);
      }
      else {
        kernelReals[i][j] = 0;
      }
    }
  }

  // shift the kernel
  fftShift(height, width, kernelReals, kernelImgs);

  // 2d fft of the kernel
  fft2D(height, width, kernelReals, kernelImgs, 1);

  // element-wise multiplication between the kernel and the image
  for(int i = 0; i < height; ++i) {
    for(int j = 0; j < width; ++j) {
      double tmpImageReal = imageReals[i][j];
      imageReals[i][j] = tmpImageReal * kernelReals[i][j] - imageImgs[i][j] * kernelImgs[i][j];
      imageImgs[i][j] = tmpImageReal * kernelImgs[i][j] + imageImgs[i][j] * kernelReals[i][j];
    }
  }

  // inverse fft on result
  fft2D(height, width, imageReals, imageImgs, -1);
  // Then the result is obtained!

  // Shift 2D fft center
  //fftShift(height, width, imageReals, imageImgs);

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

  // free all
  free(imageAbs);

  for (int i = 0; i < height; ++i) {
    free(imageReals[i]);
    free(imageImgs[i]);
    free(kernelReals[i]);
    free(kernelImgs[i]);
  }
  free(imageReals);
  free(imageImgs);
  free(kernelReals);
  free(kernelImgs);

  free(imageData);


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
