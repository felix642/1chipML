#ifndef BITMAP_H
#define BITMAP_H

#define RGB_BYTES 3 // rgb
#define GREY_BYTES 1
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

#ifdef __cplusplus
extern "C" {
#endif

void generateBitmapImageRGB(unsigned char* image, const unsigned int height,
                            const unsigned int width, char* imageFileName);

void generateBitmapImageGrey(unsigned char* image, const unsigned int height,
                             const unsigned int width, char* imageFileName);

#ifdef __cplusplus
}
#endif

#endif // BITMAP_H
