#ifndef ROOTS_UTILS_H
#define ROOTS_UTILS_H

#include "stdlib.h"

int utils_random_int(int min, int maxInclusiv) {
    int spread = (maxInclusiv+1)-min;
    int divisor = RAND_MAX/(spread);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > spread);

    return min + retval;
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