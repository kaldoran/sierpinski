#include "rgb.h"
#include "bitmap_header.h"

rgb_pixel toRGB(char *_hexa_value);
void printPixel(char *_txt, rgb_pixel _pixel);
void set_pixel(char *_pixels, int _x, int _y, int _sizex, int _sizey, int _size );

bmp_header newBmpHeader(unsigned int _header_size, unsigned int _data_size);
bmp_info newBmpInfo(int _width, int _height, unsigned short int _bits);
void writeBmpHeader(FILE* _f, bmp_header _header);
void writeBmpInfo(FILE *_f, bmp_info _info);


void create_sierpinski(char *_pixels, int _iteration, int _x, int _y, int _size, int _img_size);
