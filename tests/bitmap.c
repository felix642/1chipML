#include "bitmap.h"
#include <stdint.h>
#include <stdio.h>

void writeBitmapHeader(int height, int width, int stride, FILE* imageFile);

void generateBitmapImage(unsigned char* image, const unsigned int height,
                         const unsigned int width, char* imageFileName) {
  
  const unsigned int widthInBytes = width * BYTES_PER_PIXEL;
  const unsigned char padding[3] = {0, 0, 0};
  const unsigned int paddingSize = (4 - widthInBytes % 4) % 4;
  const unsigned int paddedWidthInBytes = (widthInBytes) + paddingSize;

  FILE* imageFile = fopen(imageFileName, "wb");
  writeBitmapHeader(height, width, paddedWidthInBytes, imageFile);

  for (unsigned int i = 0; i < height; ++i) {
    fwrite(image + (i * widthInBytes), BYTES_PER_PIXEL, width, imageFile);
    fwrite(padding, 1, paddingSize, imageFile);
  }

  fclose(imageFile);
}

void writeBitmapHeader(const int height, const int width,
                       const int paddedWidthInBytes, FILE* imageFile) {

  const unsigned int imageSize = paddedWidthInBytes * height;
  const unsigned int totalHeaderSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE;
  const unsigned int fileSize = totalHeaderSize + imageSize;

  uint32_t largeHeaders[12] = {
      fileSize,             // filesize (bfSize)
      0,                    // reserved (bfReserved1 and bfReserved2)
      totalHeaderSize,      // start pixel of the array (bfOffBits)
      INFO_HEADER_SIZE,     // bitmap info header size (biSize)
      width,                // image width (biWidth)
      height,               // image height (biHeight)
      0,                    // compression (biCompression)
      imageSize,            // image size (biSizeImage)
      0,                    // horizontal resolution (biXPelsPerMeter)
      0,                    // vertical resolution (biYPelsPerMeter)
      0,                    // colors in color table (biClrUsed)
      0                     // important color count (biClrImportant)
  };

  uint16_t biPlanes = 1; // number of color planes (biPlanes)
  uint16_t biBitCount = BYTES_PER_PIXEL * 8; // bits per pixel (biBitCount)

  // Write the header
  // Write one byte at a time to take into account big-endian and little-endian
  fprintf(imageFile, "BM");

  // write bfSize, reserved, bfOffBits, biSize, biWidth and biHeight
  for (int i = 0; i < 6; ++i) {
    fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i]));
    fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i] >> 8));
    fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i] >> 16));
    fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i] >> 24));
  }

  // write biPlanes
  fprintf(imageFile, "%c", (unsigned char)(biPlanes));
  fprintf(imageFile, "%c", (unsigned char)(biPlanes >> 8));

  // write biBitCount
  fprintf(imageFile, "%c", (unsigned char)(biBitCount));
  fprintf(imageFile, "%c", (unsigned char)(biBitCount >> 8));

  // write biCompression, biSizeImage, biXPelsPerMeter, biYPelsPerMeter,
  // biClrUsed, biClrImportant
  for (int i = 6; i < 12; ++i) {
    fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i]));
    fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i] >> 8));
    fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i] >> 16));
    fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i] >> 24));
  }
}
