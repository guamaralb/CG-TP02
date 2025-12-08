#pragma once
#include <string>
#include <vector>
#include "vec3.h"

void savePPM(const std::string &filename, const std::vector<Vec3> &fb, int width, int height);
