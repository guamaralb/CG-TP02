#pragma once
#include <cmath>
#include "constants.h"

struct Vec3 {
    double x, y, z;

    Vec3(double v = 0.0) : x(v), y(v), z(v) {}
    Vec3(double X, double Y, double Z) : x(X), y(Y), z(Z) {}

    Vec3 operator+(const Vec3 &o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
    Vec3 operator-(const Vec3 &o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
    Vec3 operator*(double s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 operator/(double s) const { return Vec3(x / s, y / s, z / s); }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    Vec3& operator+=(const Vec3 &o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vec3& operator*=(double s) { x *= s; y *= s; z *= s; return *this; }
};

inline Vec3 operator*(double s, const Vec3 &v) { return Vec3(v.x * s, v.y * s, v.z * s); }
inline Vec3 operator*(const Vec3 &a, const Vec3 &b) { return Vec3(a.x*b.x, a.y*b.y, a.z*b.z); }

inline double dot(const Vec3 &a, const Vec3 &b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vec3 cross(const Vec3 &a, const Vec3 &b) {
    return Vec3(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

inline double length(const Vec3 &v) {
    return std::sqrt(dot(v, v));
}

inline Vec3 normalize(const Vec3 &v) {
    double len = length(v);
    if (len < EPS) return Vec3(0,0,0);
    return v / len;
}

inline Vec3 reflect(const Vec3 &I, const Vec3 &N) {
    return I - 2.0 * dot(I, N) * N;
}
