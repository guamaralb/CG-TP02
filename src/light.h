#pragma once
#include "vec3.h"

struct Light {
    Vec3 position;
    Vec3 color;
    double kc, kl, kq;
};
