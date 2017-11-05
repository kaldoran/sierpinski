#include <stdio.h>
#include <stdlib.h>

/* Move .h */
typedef struct _bitmap_header {
   unsigned short int type;                 /* Magic identifier            */
   unsigned int size;                       /* File size in bytes          */
   unsigned short int reserved1, reserved2;
   unsigned int offset;                     /* Offset to image data, bytes */
} bmp_header;

typedef struct _bitmap_info{
   unsigned int size;               /* Header size in bytes      */
   int width, height;               /* Width and height of image */
   unsigned short int planes;       /* Number of colour planes   */
   unsigned short int bits;         /* Bits per pixel            */
} bmp_info;

/* Move .c */
bmp_header newBmpHeader(unsigned int header_size, unsigned int data_size) {
    bmp_header bmp_h;

    bmp_h.type = 0x4D42;
    bmp_h.size = header_size + data_size;
    bmp_h.reserved1 = 0;
    bmp_h.reserved2 = 0;
    bmp_h.offset = header_size;

    return bmp_h;
}

bmp_info newBmpInfo(int width, int height, unsigned short int bits) {
    bmp_info bmp_i;

    bmp_i.size = 12;
    bmp_i.width = width;
    bmp_i.height = height;
    bmp_i.planes = 1;
    bmp_i.bits = bits;

    return bmp_i;
}

/* Move .h */
typedef struct _pixel {
    char r;
    char g;
    char b;
} rgb_pixel;

/* Move .c*/
/** Convert pixel from Hexa value to Structure value
 *
 *  %param hexa_value : VAlue of the color in hexa
 *
 *  %return Pixel in RGB format
 */
rgb_pixel toRGB(int hexa_value) {
    rgb_pixel pixel;
    pixel.r = (char) ((hexa_value >> 16) & 0xFF) / 255.;
    pixel.g = (char) ((hexa_value >> 8) & 0xFF) / 255.;
    pixel.b = (char) ((hexa_value) & 0xFF) / 255.;

    return pixel;
}

int toBRGHexa(rgb_pixel rgb) {
    return ((rgb.b & 0xff) << 16) + ((rgb.r & 0xff) << 8) + (rgb.g & 0xff);
}

void printPixel(rgb_pixel pixel) {
    printf("[R: %d, G: %d, B: %d]", pixel.r, pixel.g, pixel.b);
}

/* Move .c > main.c */
int main(int argc, char *argv[]) {
    int iteration = 0;
    char *filename = NULL;

    rgb_pixel background;
    rgb_pixel frontcolor;

    switch(argc) {
        case 5:
            background = toRGB(atoi(argv[4]));
        case 4:
            frontcolor = toRGB(atoi(argv[3]));
        case 3:
            iteration = atoi(argv[2]); 
        case 2:
            filename = argv[1];
            break;
        default: 
            printf("%s [filename] [iteration] [frontcolor] [background]\n", argv[0]);
            exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
