#ifndef RT_MAIN_H
#define RT_MAIN_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <thread>

using std::sqrt;
using std::make_shared;
using std::shared_ptr;
using std::fabs;
using std::fmax;
using std::fmin;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max-min) * random_double();
}


inline double degrees_to_radians(double degrees){
	return degrees * pi / 180.0;
}

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif