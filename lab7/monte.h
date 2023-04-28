#ifndef MONTE
#define MONTE

#include <stdbool.h>

typedef struct {
    double x;
    double y;
} point_t;

bool f(point_t*);
double rand_range(double, double);
point_t* new_point(double, double);

#endif
