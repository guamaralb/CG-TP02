#include "texture.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using std::cerr;
using std::endl;

Vec3 Texture::sample(double u, double v) const {
    if (!valid || width <= 0 || height <= 0) {
        return Vec3(1.0, 0.0, 1.0); // magenta (debug)
    }
    // wrap
    u -= std::floor(u);
    v -= std::floor(v);

    int x = static_cast<int>(u * (width  - 1));
    int y = static_cast<int>((1.0 - v) * (height - 1)); // invertendo v

    x = std::max(0, std::min(width  - 1, x));
    y = std::max(0, std::min(height - 1, y));

    return data[y * width + x];
}

Texture loadPPM(const std::string &filename) {
    Texture tex;
    std::ifstream f(filename, std::ios::binary);
    if (!f) {
        cerr << "Nao foi possivel abrir textura " << filename << endl;
        return tex;
    }

    std::string magic;
    f >> magic;
    if (magic != "P3" && magic != "P6") {
        cerr << "Formato PPM nao suportado (esperado P3 ou P6) em " << filename << endl;
        return tex;
    }

    auto skip_comments = [&]() {
        char c;
        f >> std::ws;
        while (f.peek() == '#') {
            std::string line;
            std::getline(f, line);
            f >> std::ws;
        }
    };

    skip_comments();
    int w, h, maxv;
    f >> w; skip_comments();
    f >> h; skip_comments();
    f >> maxv;
    f.get(); // comer um whitespace após maxv

    tex.width = w;
    tex.height = h;
    tex.data.resize(w*h);

    if (magic == "P3") {
        for (int i = 0; i < w*h; ++i) {
            int r, g, b;
            f >> r >> g >> b;
            tex.data[i] = Vec3(r/(double)maxv, g/(double)maxv, b/(double)maxv);
        }
    } else { // P6
        std::vector<unsigned char> buffer(w*h*3);
        f.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        for (int i = 0; i < w*h; ++i) {
            unsigned char r = buffer[3*i+0];
            unsigned char g = buffer[3*i+1];
            unsigned char b = buffer[3*i+2];
            tex.data[i] = Vec3(r/(double)maxv, g/(double)maxv, b/(double)maxv);
        }
    }

    tex.valid = true;
    return tex;
}
