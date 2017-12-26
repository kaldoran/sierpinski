#include <stdio.h>
#include <stdlib.h>

#include "function.h"

/** 
 * Create a new header struct for bitmap file.
 */
bmp_header newBmpHeader(unsigned int _header_size, unsigned int _data_size) {
    bmp_header bmp_h;

    bmp_h.type = 0x4D42;
    bmp_h.size = _header_size + _data_size;
    bmp_h.reserved = 0;
    bmp_h.offset = _header_size;

    return bmp_h;
}

/* Version 3.0 header */
bmp_info newBmpInfo(int _width, int _height, unsigned short int _bits) {
    bmp_info bmp_i;

    bmp_i.size = sizeof(bmp_info);
    bmp_i.width = _width;
    bmp_i.height = _height;
    bmp_i.planes = 1;
    bmp_i.bits = _bits;

    bmp_i.compression = 0;
    bmp_i.imgsize = _width * _height * _bits/8;
    bmp_i.xpix = 0x130B;
    bmp_i.ypix = 0x130B;
    bmp_i.color = 0;
    bmp_i.colimp = 0;
    return bmp_i;
}

void writeBmpHeader(FILE* _f, bmp_header _header) {
   fwrite(&_header, 1, sizeof(bmp_header), _f);
}

void writeBmpInfo(FILE *_f, bmp_info _info) {
   fwrite(&_info, 1, sizeof(bmp_info), _f);
}
