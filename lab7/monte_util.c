#include <stdio.h>
#include <stdlib.h>

#include "monte.h"

double rand_range(double low, double up) {
    double range = up - low;
    double div = RAND_MAX / range;

    return low + (rand() / div);
}

bool f(point_t* pt) {
    return pt->x*pt->x + pt->y*pt->y < 1;
}

point_t* new_point(double x, double y) {
    point_t* pt = malloc(sizeof(point_t));
    pt->x = x;
    pt->y = y;

    return pt;
}

point_t* rand_pt() {
    return new_point(rand_range(-1., 1.), rand_range(-1, 1.));
}
