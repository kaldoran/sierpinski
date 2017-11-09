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
rgb_pixel toRGB(char *hexa_value) {
    
    rgb_pixel pixel = {0};
    if ( sscanf(hexa_value, "%02x%02x%02x", &pixel.r, &pixel.g, &pixel.b) != 3 ) 
        exit(EXIT_FAILURE);

    return pixel;
}

void printPixel(char *txt, rgb_pixel pixel) {
    printf("%s [R: %d, G: %d, B: %d]\n", txt, pixel.r, pixel.g, pixel.b);
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
    printPixel("Front collor : ", frontcolor);

    f = fopen(filename, "wb");
    width *= width;
    writeBmpHeader(f, newBmpHeader(sizeof(bmp_header) + sizeof(bmp_info), width * 24 / 8));
    writeBmpInfo(f, newBmpInfo(width, width, 24));
    
    for ( i = 0; i < width; i++)
        for ( j = 0; j < width; j++)
            fwrite(&frontcolor, 1, sizeof(rgb_pixel), f);
     
    fclose(f);

    printf("Image successfully created [%s]", filename);
    exit(EXIT_SUCCESS);
}
