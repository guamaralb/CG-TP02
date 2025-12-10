#pragma once
#include <vector>
#include <limits>
#include <memory>
#include <cmath>
#include "vec3.h"
#include "ray.h"
#include "constants.h"

struct Intersection {
    bool hit = false;
    double t = std::numeric_limits<double>::max();
    Vec3 point;
    Vec3 normal;
    int pigmentIndex = -1;
    int finishIndex = -1;
};

struct Object {
    int pigmentIndex;
    int finishIndex;
    virtual ~Object() {}
    virtual bool intersect(const Ray &ray, double &tHit, Vec3 &normalOut) const = 0;
};

struct Sphere : public Object {
    Vec3 center;
    double radius;

    Sphere(const Vec3 &c, double r, int pig, int fin) {
        center = c;
        radius = r;
        pigmentIndex = pig;
        finishIndex = fin;
    }

    bool intersect(const Ray &ray, double &tHit, Vec3 &normalOut) const override {
        Vec3 oc = ray.orig - center;
        double a = dot(ray.dir, ray.dir);
        double b = 2.0 * dot(oc, ray.dir);
        double c = dot(oc, oc) - radius*radius;
        double disc = b*b - 4*a*c;
        if (disc < 0.0) return false;
        double sqrtDisc = std::sqrt(disc);
        double t0 = (-b - sqrtDisc) / (2*a);
        double t1 = (-b + sqrtDisc) / (2*a);
        double t = t0;
        if (t < EPS) t = t1;
        if (t < EPS) return false;

        tHit = t;
        Vec3 p = ray.orig + ray.dir * t;
        normalOut = normalize(p - center);
        return true;
    }
};

struct Plane {
    Vec3 n;
    double d;
};

struct Polyhedron : public Object {
    std::vector<Plane> planes;

    Polyhedron(int pig, int fin) {
        pigmentIndex = pig;
        finishIndex = fin;
    }

    bool intersect(const Ray &ray, double &tHit, Vec3 &normalOut) const override {
        double tEnter = 0.0;
        double tExit = std::numeric_limits<double>::max();
        Vec3 bestNormal(0,0,0);

        double oDist, denom, tPlane;

        for (const auto &pl : planes) {
            denom = dot(pl.n, ray.dir);
            oDist = dot(pl.n, ray.orig) + pl.d;

            if (std::fabs(denom) < EPS) {
                if (oDist < 0) {
                    return false;
                }
                continue;
            }

            tPlane = -oDist / denom;
            if (denom < 0) {
                if (tPlane > tEnter) {
                    tEnter = tPlane;
                    bestNormal = pl.n;
                }
            } else {
                if (tPlane < tExit) {
                    tExit = tPlane;
                }
            }

            if (tEnter > tExit) {
                return false;
            }
        }

        if (tExit < EPS) return false;

        tHit = (tEnter > EPS) ? tEnter : tExit;
        normalOut = normalize(bestNormal);
        return true;
    }
};
