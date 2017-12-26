#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "error.h"
#include "function.h"

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
