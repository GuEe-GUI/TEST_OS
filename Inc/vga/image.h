/*
    File:       image.h

    Contains:   Provide drawing images interface

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __VGA_IMAGE__
#define __VGA_IMAGE__

typedef struct image_t {
    int Rows;
    int Cols;
} image_t;

typedef struct cur_t {
    image_t Base;
} cur_t;

typedef struct ico_t {
    image_t Base;
} ico_t;

typedef struct Mat_t {
    image_t Base;
} Mat_t;

#endif
