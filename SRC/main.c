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
bmp_header newBmpHeader(unsigned int header_size, unsigned int data_size) {
    bmp_header bmp_h;

    bmp_h.type = 0x4D42;
    bmp_h.size = header_size + data_size;
    bmp_h.reserved = 0;
    bmp_h.offset = header_size;

    return bmp_h;
}

bmp_info newBmpInfo(int width, int height, unsigned short int bits) {
    bmp_info bmp_i;

    bmp_i.size = sizeof(bmp_info);
    bmp_i.width = width;
    bmp_i.height = height;
    bmp_i.planes = 1;
    bmp_i.bits = bits;

    bmp_i.compression = 0;
    bmp_i.imgsize = width * height * bits/8;
    bmp_i.xpix = 0x130B;
    bmp_i.ypix = 0x130B;
    bmp_i.color = 0;
    bmp_i.colimp = 0;
    return bmp_i;
}

void writeBmpHeader(FILE* f, bmp_header header) {
   fwrite(&header, 1, sizeof(bmp_header), f);
}

void writeBmpInfo(FILE *f, bmp_info info) {
   fwrite(&info, 1, sizeof(bmp_info), f);
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

int toBGRHexa(rgb_pixel rgb) {
    return ((rgb.b & 0xff) << 16) + ((rgb.g & 0xff) << 8) + (rgb.r & 0xff);
}

void printPixel(rgb_pixel pixel) {
    printf("[R: %d, G: %d, B: %d]", pixel.r, pixel.g, pixel.b);
}

/* Move .c > main.c */
int main(int argc, char *argv[]) {
    FILE *f = NULL;
    int i = 0;
    int width = 8;
    int height = 8;
    int iteration = 1;
    char *filename = NULL;

    bmp_header b;
    rgb_pixel background = {0, 0, 0};
    rgb_pixel frontcolor = {255, 255, 255};

    switch(argc) {
        case 7:
            background = toRGB(atoi(argv[6]));
        case 6:
            frontcolor = toRGB(atoi(argv[5]));
        case 5:
            height = atoi(argv[4]);
        case 4:
            width = atoi(argv[3]);
        case 3:
            iteration = atoi(argv[2]); 
        case 2:
            filename = argv[1];
            break;
        default: 
            printf("%s [filename] [iteration] [frontcolor] [background]\n", argv[0]);
            exit(EXIT_FAILURE);
    }

    printf("%d _ %d\n", sizeof(bmp_header), sizeof(bmp_info));
    printf("%d - %d - %d - %d\n", sizeof(b.type), sizeof(b.size), sizeof(b.reserved), sizeof(b.offset));
    f = fopen(filename, "wb");
    writeBmpHeader(f, newBmpHeader(sizeof(bmp_header) + sizeof(bmp_info), width * height * 24 / 8));
    writeBmpInfo(f, newBmpInfo(width, height, 24));
    
    for ( i = 0; i < width * height; i++)
        fprintf(f, "%d", toBGRHexa(frontcolor));

    fclose(f);
    exit(EXIT_SUCCESS);
}
