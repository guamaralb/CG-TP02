#include "raytracer.h"
#include "pigment.h"
#include "material.h"
#include "constants.h"
#include <algorithm>
#include <cmath>

static bool isInShadow(const Scene &scene, const Vec3 &point, const Vec3 &Ldir, double maxDist) {
    Ray shadowRay(point + Ldir * EPS, Ldir);
    for (const auto &objPtr : scene.objects) {
        double t;
        Vec3 n;
        if (objPtr->intersect(shadowRay, t, n)) {
            if (t > EPS && t < maxDist - EPS) {
                return true;
            }
        }
    }
    return false;
}

static Vec3 shade(const Scene &scene, const Intersection &isect, const Ray &ray, int depth);

Vec3 traceRay(const Scene &scene, const Ray &ray, int depth) {
    if (depth > MAX_DEPTH) {
        return Vec3(0,0,0);
    }

    Intersection isect;
    isect.t = std::numeric_limits<double>::max();

    for (const auto &objPtr : scene.objects) {
        double t;
        Vec3 n;
        if (objPtr->intersect(ray, t, n)) {
            if (t > EPS && t < isect.t) {
                isect.hit = true;
                isect.t = t;
                isect.point = ray.orig + ray.dir * t;
                isect.normal = n;
                isect.pigmentIndex = objPtr->pigmentIndex;
                isect.finishIndex = objPtr->finishIndex;
            }
        }
    }

    if (!isect.hit) {
        return Vec3(0,0,0); // fundo preto
    }

    return shade(scene, isect, ray, depth);
}

static Vec3 shade(const Scene &scene, const Intersection &isect, const Ray &ray, int depth) {
    const Pigment &pig = scene.pigments[isect.pigmentIndex];
    const Finish  &fin = scene.finishes[isect.finishIndex];

    Vec3 N = isect.normal;
    if (dot(N, ray.dir) > 0) N = N * -1.0;

    Vec3 baseColor = getPigmentColor(pig, isect.point);

    Vec3 result(0,0,0);

    // Luz ambiente: primeira luz do arquivo é a ambient
    if (!scene.lights.empty()) {
        const Light &amb = scene.lights[0];
        Vec3 ambColor = amb.color;
        result += fin.ka * (baseColor * ambColor);
    }

    // Luzes pontuais
    for (size_t i = 1; i < scene.lights.size(); ++i) {
        const Light &Lsrc = scene.lights[i];
        Vec3 Lvec = Lsrc.position - isect.point;
        double dist = length(Lvec);
        Vec3 Ldir = normalize(Lvec);

        if (isInShadow(scene, isect.point, Ldir, dist)) {
            continue;
        }

        double att = 1.0 / (Lsrc.kc + Lsrc.kl * dist + Lsrc.kq * dist * dist);

        double NdotL = std::max(0.0, dot(N, Ldir));
        Vec3 diffuse = fin.kd * NdotL * (baseColor * Lsrc.color) * att;

        Vec3 V = normalize(scene.camera.eye - isect.point);
        Vec3 R = reflect(-Ldir, N);
        double RdotV = std::max(0.0, dot(R, V));
        double specFactor = (RdotV > 0.0) ? std::pow(RdotV, fin.alpha) : 0.0;
        Vec3 specular = fin.ks * specFactor * Lsrc.color * att;

        result += diffuse + specular;
    }

    // Reflexão
    if (fin.kr > 0.0 && depth < MAX_DEPTH) {
        Vec3 Rdir = reflect(ray.dir, N);
        Ray reflRay(isect.point + Rdir * EPS, Rdir);
        Vec3 reflColor = traceRay(scene, reflRay, depth + 1);
        result += fin.kr * reflColor;
    }

    // Refração
    if (fin.kt > 0.0 && depth < MAX_DEPTH) {
        double eta = fin.ior;
        Vec3 Nn = N;
        double cosi = -dot(Nn, ray.dir);
        if (cosi < 0) {
            cosi = -cosi;
            Nn = Nn * -1.0;
            eta = 1.0 / eta;
        }

        double k = 1.0 - eta*eta * (1.0 - cosi*cosi);
        if (k >= 0.0) {
            Vec3 Tdir = eta * ray.dir + (eta * cosi - std::sqrt(k)) * Nn;
            Tdir = normalize(Tdir);
            Ray refrRay(isect.point + Tdir * EPS, Tdir);
            Vec3 refrColor = traceRay(scene, refrRay, depth + 1);
            result += fin.kt * refrColor;
        }
    }

    result.x = std::max(0.0, std::min(1.0, result.x));
    result.y = std::max(0.0, std::min(1.0, result.y));
    result.z = std::max(0.0, std::min(1.0, result.z));
    return result;
}
