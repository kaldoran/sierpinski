#ifndef BITMAP_HEADER_H
#define BITMAP_HEADER_H 

#include <stdint.h>

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

#endif
