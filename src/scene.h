#pragma once
#include <vector>
#include <memory>
#include "camera.h"
#include "light.h"
#include "pigment.h"
#include "material.h"
#include "objects.h"

struct Scene {
    Camera camera;
    std::vector<Light> lights;
    std::vector<Pigment> pigments;
    std::vector<Finish> finishes;
    std::vector<std::unique_ptr<Object>> objects;
};
