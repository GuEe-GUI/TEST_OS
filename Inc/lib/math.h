/*
    File:       math.h

    Contains:   Provide mathematical calculation function and constant

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __LIB_MATH__
#define __LIB_MATH__

/* Const number */
#define PI 3.14159265358979323846
#define E  2.71828182845904523536

#define MAX_S(x, y) ({              \
    const typeof(x) _x = (x);       \
    const typeof(y) _y = (y);       \
    (void)(&_x == &_y);             \
    _x > _y ? _x : _y; })

#define TMAX_S(type, x, y) ({           \
    type _x = (x);                      \
    type _y = (y);                      \
    _x > _y ? _x: _y; })

#define Max(a, b, c) (((a) > (b) ? (a) : (b)) > (c) ? ((a) > (b) ? (a) : (b)) : (c))
#define Mid(a, b, c) ((((a) < (b) ? (a) : (b)) > (c) ? ((a) < (b) ? (a) : (b)) : (c)) < ((a) > (b) ? (a) : (b)) ? (((a) < (b) ? (a) : (b)) > (c) ? ((a) < (b) ? (a) : (b)) : (c)) : ((a) > (b) ? (a) : (b)))
#define Min(a, b, c) (((a) < (b) ? (a) : (b)) < (c) ? ((a) < (b) ? (a) : (b)) : (c))
#define Abs(x) ((x) < 0 ? -(x) : (x))

int    abs(int Number);
double sqrt(double x);
float  InvSqrt(float x);

#endif
