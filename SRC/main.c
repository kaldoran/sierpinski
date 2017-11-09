#include <stdio.h>
#include <stdlib.h>

/* Move .h */
#pragma pack(push, 1) /* Remove this F*cking padding */
typedef struct _bitmap_header {
    
   unsigned short int type;                 /* Magic identifier            */
   unsigned int size;                       /* File size in bytes          */
   unsigned int reserved;
   unsigned int offset;                     /* Offset to image data, bytes */
} bmp_header;

typedef struct _bitmap_info{
   unsigned int size;               /* Header size in bytes      */
   int width, height;               /* Width and height of image */
   unsigned short int planes;       /* Number of colour planes   */
   unsigned short int bits;         /* Bits per pixel            */
   
   int compression;
   int imgsize;
   int xpix;
   int ypix;
   int color;
   int colimp;
} bmp_info;
#pragma pack(pop)


/* Move .c */
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

/* Move .h */
typedef struct _pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_pixel;

/* Move .c*/
/** Convert pixel from Hexa value to Structure value
 *
 *  %param hexa_value : VAlue of the color in hexa
 *
 *  %return Pixel in RGB format
 */
rgb_pixel toRGB(char *_hexa_value) {
    
    rgb_pixel pixel = {0};
    if ( sscanf(_hexa_value, "%02x%02x%02x", &pixel.r, &pixel.g, &pixel.b) != 3 ) 
        exit(EXIT_FAILURE);

    return pixel;
}

void printPixel(char *_txt, rgb_pixel _pixel) {
    printf("%s [R: %d, G: %d, B: %d]\n", _txt, _pixel.r, _pixel.g, _pixel.b);
}

void set_pixel(char *_pixels, int _x, int _y, int _sizex, int _sizey, int _size ) {
    int x, y;
    int i, j;
    for ( y = _y; y < _sizey; y++ )
        for ( x = _x; x < _sizex; x++ )
            _pixels[x + y * _size] = 1;

    /* Temporary debug */
    for ( i = 0; i < _size; i++ ) {
        for ( j = 0; j < _size; j++ )
            printf("%c ", (_pixels[j + i * _size]) ? '1' : '0');
        printf("\n");
    }
}

void create_sierpinski(char *_pixels, int _iteration, int _img_size) {
    int x, y, size;
    size = _img_size / 3;
    set_pixel(_pixels, size, size, size + size, size + size, _img_size);
}

/* Move .c > main.c */
int main(int argc, char *argv[]) {
    FILE *f = NULL;
    int i = 0, j = 0;
    int width = 8;
    int iteration = 1;
    char *filename = NULL;
    bmp_header b;
    rgb_pixel background = {0, 0, 0};
    rgb_pixel frontcolor = {255, 255, 255};

    char *pixels;

    switch(argc) {
        case 6:
            background = toRGB(argv[5]);
        case 5:
            frontcolor = toRGB(argv[4]);
        case 4:
            width = atoi(argv[3]);
        case 3:
            iteration = atoi(argv[2]); 
        case 2:
            filename = argv[1];
            break;
        default: 
            printf("%s [filename] [iteration] [size] [frontcolor] [background]\n", argv[0]);
            exit(EXIT_FAILURE);
    }

    printf("Generating image [%s] [%dx%d] - %d iteration.\n", filename, width, width, iteration);
    printPixel("Background color : ", background);
    printPixel("Front color : ", frontcolor);

    f = fopen(filename, "wb");

    /* Temporary Calloc */
    pixels = calloc(width * width, sizeof(char));
    create_sierpinski(pixels, iteration, width);
    
    /* All the header part */
    writeBmpHeader(f, newBmpHeader(sizeof(bmp_header) + sizeof(bmp_info), width * width * 24 / 8));
    writeBmpInfo(f, newBmpInfo(width, width, 24));

    /* Writing the matrix */
    for ( i = 0; i < width; i++ ) {
        for ( j = 0; j < width; j++)
            if ( pixels[j + i * width] )
                fwrite(&background, 1, sizeof(rgb_pixel), f);
            else
                fwrite(&frontcolor, 1, sizeof(rgb_pixel), f);
        /* Padding ? */
    }
    fclose(f);

    printf("Image successfully created [%s]", filename);
    exit(EXIT_SUCCESS);
}
