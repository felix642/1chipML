#include "bitmap.h"
#include <stdint.h>
#include <stdio.h>

typedef struct ImageProperties {
  unsigned int height;
  unsigned int width;
  unsigned int paddingSize;
  unsigned int bytesPerPixel;
} ImageProperties;

static void writeBitmapHeaderRGB(const ImageProperties properties,
                                 FILE* imageFile);
static void writeBitmapHeaderGrey(const ImageProperties properties,
                                  FILE* imageFile);
static void writeGenericHeader(const unsigned int numberOfColors,
                               const ImageProperties properties,
                               FILE* imageFile);
static inline unsigned int getPaddingSize(const unsigned int width,
                                          const unsigned int bytesPerPixel);
static void writeImageToFile(const unsigned char* image,
                             const ImageProperties properties, FILE* imageFile);


/**
 * @brief Creates a RGB bitmap image from data
 * @param image The image data. One byte per channel (R, G, B) is expected
 * @param height The image height, in pixels
 * @param width The image width, in pixels
 * @param imageFileName The image file name, with the bitmap extension
*/
void generateBitmapImageRGB(unsigned char* image, const unsigned int height,
                            const unsigned int width, char* imageFileName) {

  const unsigned int paddingSize = getPaddingSize(width, RGB_BYTES);
  ImageProperties imageProperties = {height, width, paddingSize, RGB_BYTES};

  FILE* imageFile = fopen(imageFileName, "wb");
  writeBitmapHeaderRGB(imageProperties, imageFile);
  writeImageToFile(image, imageProperties, imageFile);
  fclose(imageFile);
}

/**
 * @brief Creates a Grayscale bitmap image from data, with a single 8-bit channel
 * @param image The image data. One byte per pixel is expected
 * @param height The image height, in pixels
 * @param width The image width, in pixels
 * @param imageFileName The image file name, with the bitmap extension
*/
void generateBitmapImageGrey(unsigned char* image, const unsigned int height,
                             const unsigned int width, char* imageFileName) {
  const unsigned int paddingSize = getPaddingSize(width, GREY_BYTES);
  ImageProperties imageProperties = {height, width, paddingSize, GREY_BYTES};

  FILE* imageFile = fopen(imageFileName, "wb");
  writeBitmapHeaderGrey(imageProperties, imageFile);
  writeImageToFile(image, imageProperties, imageFile);
  fclose(imageFile);
}

static void writeBitmapHeaderRGB(const ImageProperties properties,
                                 FILE* imageFile) {

  const unsigned int numberOfColors = 0; // default
  writeGenericHeader(numberOfColors, properties, imageFile);
}

static void writeBitmapHeaderGrey(const ImageProperties properties,
                                  FILE* imageFile) {

  const unsigned int numberOfColors = 256;
  writeGenericHeader(numberOfColors, properties, imageFile);

  // write the color palette for greyscale
  for (unsigned int i = 0; i < numberOfColors; ++i) {
    fprintf(imageFile, "%c", (unsigned char)i);
    fprintf(imageFile, "%c", (unsigned char)i);
    fprintf(imageFile, "%c", (unsigned char)i);
    fprintf(imageFile, "%c", (unsigned char)0);
  }
}

static void writeGenericHeader(const unsigned int numberOfColors,
                               const ImageProperties properties,
                               FILE* imageFile) {

  const unsigned int paddedWidthInBytes = properties.width * properties.bytesPerPixel + properties.paddingSize;
      
  const unsigned int imageSize = paddedWidthInBytes * properties.height;
  const unsigned int totalHeaderSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + numberOfColors * 4;
  const unsigned int fileSize = totalHeaderSize + imageSize;

  uint32_t largeHeaders[12] = {
      fileSize,          // filesize (bfSize)
      0,                 // reserved (bfReserved1 and bfReserved2)
      totalHeaderSize,   // start pixel of the array (bfOffBits)
      INFO_HEADER_SIZE,  // bitmap info header size (biSize)
      properties.width,  // image width (biWidth)
      properties.height, // image height (biHeight)
                         // Here would go the number of color planes (biPlanes)
                         // Here would go the bits per pixel (biBitCount)
      0,                 // compression (biCompression)
      imageSize,         // image size (biSizeImage)
      0,                 // horizontal resolution (biXPelsPerMeter)
      0,                 // vertical resolution (biYPelsPerMeter)
      numberOfColors,    // colors in color table (biClrUsed)
      0                  // important color count (biClrImportant)
  };

  uint16_t biPlanes = 1; // number of color planes (biPlanes)
  uint16_t biBitCount =
      properties.bytesPerPixel * 8; // bits per pixel (biBitCount)

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

static void writeImageToFile(const unsigned char* image,
                             const ImageProperties properties,
                             FILE* imageFile) {

  const unsigned int widthInBytes = properties.width * properties.bytesPerPixel;
  const unsigned char padding[3] = {0, 0, 0};

  for (unsigned int i = 0; i < properties.height; ++i) {
    fwrite(image + (i * widthInBytes), properties.bytesPerPixel,
           properties.width, imageFile);
    fwrite(padding, 1, properties.paddingSize, imageFile);
  }
}

static inline unsigned int getPaddingSize(const unsigned int width,
                                          const unsigned int bytesPerPixel) {

  const unsigned int widthInBytes = width * bytesPerPixel;
  return (4U - widthInBytes % 4U) % 4U;
}
