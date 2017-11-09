#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#ifdef DEBUG
    #define DEBUG_MSG(MSG, ...)                                                                                                             \
    do {                                                                                                                                    \
        fprintf(stderr, "\n\t[DEBUG] File : %s - Line : %d - Function : %s() : " MSG "\n", __FILE__, __LINE__, __func__, ## __VA_ARGS__);   \
    } while(0);
#else
    #define DEBUG_MSG(MSG, ...)
#endif

#ifdef DEBUG
    #define DEBUG_PRINTF(MSG, ...)               \
    do {                                         \
        fprintf(stderr, MSG, ## __VA_ARGS__);    \
    } while(0);
#else
    #define DEBUG_PRINTF(MSG, ...)
#endif

/* Move .h */
#pragma pack(push, 1) /* Remove this F*cking padding */
typedef struct _bitmap_header {
    uint16_t type;                 /* Magic identifier            */
    uint32_t size;                       /* File size in bytes          */
    uint32_t reserved;
    uint32_t offset;                     /* Offset to image data, bytes */
} bmp_header;

typedef struct _bitmap_info{
    uint32_t size;               /* Header size in bytes      */
     int32_t width, height;               /* Width and height of image */
    uint16_t planes;       /* Number of colour planes   */
    uint16_t bits;         /* Bits per pixel            */

    int32_t compression;
    int32_t imgsize;
    int32_t xpix;
    int32_t ypix;
    int32_t color;
    int32_t colimp;
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
    uint8_t r;
    uint8_t g;
    uint8_t b;
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
    if ( sscanf(_hexa_value, "%02hhx%02hhx%02hhx", &pixel.r, &pixel.g, &pixel.b) != 3 ) 
        exit(EXIT_FAILURE);

    return pixel;
}

void printPixel(char *_txt, rgb_pixel _pixel) {
    printf("%s [R: %d, G: %d, B: %d]\n", _txt, _pixel.r, _pixel.g, _pixel.b);
}

void set_pixel(char *_pixels, int _x, int _y, int _sizex, int _sizey, int _size ) {
    int x, y;
    for ( y = _y; y < _sizey; y++ )
        for ( x = _x; x < _sizex; x++ )
            _pixels[x + y * _size] = 1;
}

void create_sierpinski(char *_pixels, int _iteration, int _x, int _y, int _size, int _img_size) {
    int x, y, size;
    
    size = _size / 3;
    set_pixel(_pixels, _x, _y, _x + _size, _y + _size, _img_size);
    if ( _iteration < 2 ) return;

    for ( x = -1; x <= 1; x++ )
        for  ( y = -1; y <= 1; y++ )
            if ( x != 0 || y != 0 )
                create_sierpinski(_pixels, _iteration - 1, _x + ( x * _size) + size, _y + (y * _size) + size, size, _img_size);
}

/* Move .c > main.c */
int main(int argc, char *argv[]) {
    FILE *f = NULL;
    char *filename = NULL;

    int i = 0, j = 0;
    int width = 12;
    int square = 0;
    int iteration = 1;

    int padding = 0;
    rgb_pixel background = {0, 0, 0};
    rgb_pixel frontcolor = {255, 255, 255};

    char *pixels;

    clock_t t;

    switch(argc) {
        case 6:
            background = toRGB(argv[5]);
            __attribute__ ((fallthrough));
        case 5:
            frontcolor = toRGB(argv[4]);
            __attribute__ ((fallthrough));
        case 4:
            width = atoi(argv[3]);
            __attribute__ ((fallthrough));
        case 3:
            iteration = atoi(argv[2]); 
            __attribute__ ((fallthrough));
        case 2:
            filename = argv[1];
            break;
        default: 
            printf("%s [filename] [iteration] [size] [frontcolor] [background]\n", argv[0]);
            exit(EXIT_FAILURE);
    }
    
    printf("Image [%s] [%dx%d] - %d iteration.\n", filename, width, width, iteration);
    printPixel("Background color : ", background);
    printPixel("Front color      : ", frontcolor);

    if ( (pixels = calloc(width * width, sizeof(char))) == NULL ) exit(EXIT_FAILURE);

    square = width / 3;
    t = clock();
    create_sierpinski(pixels, iteration, square, square, square, width);
    printf("Time taken       :  %f\n", ((double) clock() - t) / CLOCKS_PER_SEC);

    /* All the header part */
    f = fopen(filename, "wb");
    writeBmpHeader(f, newBmpHeader(sizeof(bmp_header) + sizeof(bmp_info), width * width * 24 / 8));
    writeBmpInfo(f, newBmpInfo(width, width, 24));
    
    padding = 4 - (width * 3 % 4);

    /* Writing the matrix */
    for ( i = 0; i < width; i++ ) {
        for ( j = 0; j < width; j++) {
            if ( (square = pixels[j + i * width]) )
                fwrite(&background, 1, sizeof(rgb_pixel), f);
            else
                fwrite(&frontcolor, 1, sizeof(rgb_pixel), f);
        
            DEBUG_PRINTF("%c ", square ? '1' : '0');
        }
        DEBUG_PRINTF("\n");
        
        if ( padding != 4 )                     /* Padding necessary lets go*/
            fwrite(&background, 1, padding, f); /* Write whatever you want for padding */
    }
    
    fclose(f);
    free(pixels);
    
    printf("Image successfully created [%s]", filename);
    exit(EXIT_SUCCESS);
}
