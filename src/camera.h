#pragma once
#include "vec3.h"

struct Camera {
    Vec3 eye;
    Vec3 center;
    Vec3 up_hint;
    double fovY;
    // base ortonormal
    Vec3 u, v, w; // w = forward, u = right, v = up
};

void buildCamera(Camera &cam);
