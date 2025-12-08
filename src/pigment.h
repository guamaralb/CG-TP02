#pragma once
#include "vec3.h"
#include "texture.h"

enum class PigmentType {
    SOLID,
    CHECKER,
    TEXMAP
};

struct Pigment {
    PigmentType type = PigmentType::SOLID;
    Vec3 color1;   // para solid/checker
    Vec3 color2;   // para checker
    double size = 1.0; // tamanho do quadrado/cubo do checker
    // texmap
    Texture tex;
    double P0[4] = {0,0,0,0};
    double P1[4] = {0,0,0,0};
};

Vec3 getPigmentColor(const Pigment &p, const Vec3 &point);
