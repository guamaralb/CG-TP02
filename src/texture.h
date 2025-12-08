#pragma once
#include <vector>
#include <string>
#include "vec3.h"

struct Texture {
    int width = 0;
    int height = 0;
    std::vector<Vec3> data;
    bool valid = false;

    Vec3 sample(double u, double v) const;
};

Texture loadPPM(const std::string &filename);
