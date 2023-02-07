#ifndef ROOTS_UTILS_H
#define ROOTS_UTILS_H

#include "stdlib.h"
#include "raymath.h"

int utils_random_int(int min, int maxInclusiv) {
    int spread = (maxInclusiv+1)-min;
    int divisor = RAND_MAX/(spread);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > spread);

    return min + retval;
}

float utils_random_float(float min, float maxInclusiv) {
    float between_zero_and_one = (float)rand() / (float)RAND_MAX;
    return Remap(between_zero_and_one, 0.0f, 1.0f, min, maxInclusiv);
}

int color_are_equal(Color c1, Color c2)
{
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

Sound LoadSoundEx(const char* file, float volume) {
    Sound s = LoadSound(file);
    SetSoundVolume(s, volume);
    return s;
}

#endif
