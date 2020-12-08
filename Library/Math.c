/*
    File:       Graphics.c

    Contains:   Provide mathematical calculation function

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <lib/math.h>

int abs(int Number) {
    return Abs(Number);
}

double sqrt(double x) {
    double last = 0.0, res = 1.0;

    if (x == 0) return 0;

    while (res != last) {
        last = res;
        res = (res + x / res) / 2;
    }

    return res;
}

float quickSqrt(float x) {
    const float threeHalfs = 1.5F;
    float x2 = x * 0.5F, y  = x;
    long i = *(long*) & y;
    i  = 0x5f3759df - (i >> 1);
    y  = *(float*) & i;
    y  *= (threeHalfs - (x2 * y * y));
    y  *= (threeHalfs - (x2 * y * y));

    return y;
}
