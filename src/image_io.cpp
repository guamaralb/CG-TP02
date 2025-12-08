#include "image_io.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

using std::cerr;
using std::endl;

void savePPM(const std::string &filename, const std::vector<Vec3> &fb, int width, int height) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        cerr << "Nao foi possivel abrir arquivo PPM para escrita: " << filename << endl;
        return;
    }

    out << "P3\n" << width << " " << height << "\n255\n";
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            const Vec3 &c = fb[j*width + i];
            int r = static_cast<int>(std::round(std::max(0.0, std::min(1.0, c.x)) * 255.0));
            int g = static_cast<int>(std::round(std::max(0.0, std::min(1.0, c.y)) * 255.0));
            int b = static_cast<int>(std::round(std::max(0.0, std::min(1.0, c.z)) * 255.0));
            out << r << " " << g << " " << b << "\n";
        }
    }
}
