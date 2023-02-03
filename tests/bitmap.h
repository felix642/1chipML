#ifndef BITMAP_H
#define BITMAP_H

#define BYTES_PER_PIXEL 3 //rgb
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40



#ifdef __cplusplus
extern "C" {
#endif

void generateBitmapImage (unsigned char* image, int height, int width, char* imageFileName);

#ifdef __cplusplus
}
#endif

#endif // BITMAP_H
