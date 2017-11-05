#include <stdio.h>
#include <stdlib.h>

typedef struct _pixel {
    char r;
    char g;
    char b;
} rgb_pixel;

/** Convert pixel from Hexa value to Structure value
 *
 *  %param hexa_value : VAlue of the color in hexa
 *
 *  %return Pixel in RGB format
 */
rgb_pixel colorConvert(int hexa_value) {
    rgb_pixel pixel;
    pixel.r = (char) ((hexa_value >> 16) & 0xFF) / 255.;
    pixel.g = (char) ((hexa_value >> 8) & 0xFF) / 255.;
    pixel.b = (char) ((hexa_value) & 0xFF) / 255.;

    return pixel;
}

int main(int argc, char *argv[]) {
    int iteration = 0;
    char *filename = NULL;

    rgb_pixel background;
    rgb_pixel frontcolor;

    switch(argc) {
        case 5:
            background = colorConvert(atoi(argv[4]));
        case 4:
            frontcolor = colorConvert(atoi(argv[3]));
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
