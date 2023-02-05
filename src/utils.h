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

#endif