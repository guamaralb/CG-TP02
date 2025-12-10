#pragma once
#include "vec3.h"

struct Camera {
    Vec3 eye;
    Vec3 center;
    Vec3 up_hint;
    double fovY;
    Vec3 u, v, w;
};

void buildCamera(Camera &cam);
