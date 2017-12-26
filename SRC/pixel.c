#include <stdio.h>
#include <stdlib.h>

#include "function.h"

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
