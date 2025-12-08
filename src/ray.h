#pragma once
#include "vec3.h"

struct Ray {
    Vec3 orig;
    Vec3 dir;
    Ray() {}
    Ray(const Vec3 &o, const Vec3 &d) : orig(o), dir(d) {}
};
