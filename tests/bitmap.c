#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"

void generateBitmapImage(unsigned char* image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int stride);
unsigned char* createBitmapInfoHeader(int height, int width);
void writeBitmapHeader(int height, int width, int stride, FILE* imageFile);

/*
int main ()
{
    int height = 361;
    int width = 867;
    unsigned char image[height][width][BYTES_PER_PIXEL];
    char* imageFileName = (char*) "bitmapImage.bmp";

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            image[i][j][2] = (unsigned char) ( i * 255 / height );             ///red
            image[i][j][1] = (unsigned char) ( j * 255 / width );              ///green
            image[i][j][0] = (unsigned char) ( (i+j) * 255 / (height+width) ); ///blue
        }
    }

    generateBitmapImage((unsigned char*) image, height, width, imageFileName);
    printf("Image generated!!");
}
*/

void generateBitmapImage (unsigned char* image, int height, int width, char* imageFileName)
{
    int widthInBytes = width * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - widthInBytes % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(height, stride);
    //fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    //fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);
    writeBitmapHeader(height, width, stride, imageFile);

    for (int i = 0; i < height; ++i) {
        fwrite(image + (i*widthInBytes), BYTES_PER_PIXEL, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader (int height, int width)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

    return infoHeader;
}


void writeBitmapHeader(int height, int width, int stride, FILE* imageFile) {
    
    const int imageSize = stride * height;
    const int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + imageSize;

    uint32_t largeHeaders[12] = {
        fileSize, // filesize (bfSize)
        0, // reserved (bfReserved1 and bfReserved2)
        FILE_HEADER_SIZE + INFO_HEADER_SIZE, // start pixel of the array (bfOffBits)
        INFO_HEADER_SIZE, // bitmap info header size (biSize)
        width, // image width (biWidth)
        height, // image height (biHeight)
        0, // compression (biCompression)
        imageSize, // image size (biSizeImage)
        0, // horizontal resolution (biXPelsPerMeter)
        0, // vertical resolution (biYPelsPerMeter)
        0, // colors in color table (biClrUsed)
        0 // important color count (biClrImportant)
    };

    uint16_t biPlanes = 1; // number of color planes (biPlanes)
    uint16_t biBitCount = BYTES_PER_PIXEL * 8; // bits per pixel (biBitCount)

    // Write the header
    // Write one byte at a time to take into account big-endian and little-endian
    fprintf(imageFile, "BM");

    // write bfSize, reserved, bfOffBits, biSize, biWidth and biHeight
    for (int i = 0; i < 6; ++i) {
        fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i]      ));
        fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i] >> 8 ));
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
        fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i]      ));
        fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i] >> 8 ));
        fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i] >> 16));
        fprintf(imageFile, "%c", (unsigned char)(largeHeaders[i] >> 24));
    }

}
