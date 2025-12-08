#include "pigment.h"
#include <cmath>

Vec3 getPigmentColor(const Pigment &p, const Vec3 &point) {
    if (p.type == PigmentType::SOLID) {
        return p.color1;
    } else if (p.type == PigmentType::CHECKER) {
        double s = p.size;
        if (s <= 0) s = 1.0;
        int ix = static_cast<int>(std::floor(point.x / s));
        int iy = static_cast<int>(std::floor(point.y / s));
        int iz = static_cast<int>(std::floor(point.z / s));
        int parity = (ix + iy + iz) & 1;
        return (parity == 0) ? p.color1 : p.color2;
    } else { // TEXMAP
        double x = point.x, y = point.y, z = point.z;
        double s = p.P0[0]*x + p.P0[1]*y + p.P0[2]*z + p.P0[3]*1.0;
        double r = p.P1[0]*x + p.P1[1]*y + p.P1[2]*z + p.P1[3]*1.0;

        double u = s;
        double v = r;
        return p.tex.sample(u, v);
    }
}
