#pragma once
#include "scene.h"
#include "ray.h"
#include "objects.h"

Vec3 traceRay(const Scene &scene, const Ray &ray, int depth);
