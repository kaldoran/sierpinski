#include <stdio.h>

#include "function.h"

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
