#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "scene_loader.h"
#include "raytracer.h"
#include "image_io.h"
#include "scene.h"
#include "ray.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <entrada.in> <saida.ppm> [largura altura]\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    int width = 800;
    int height = 600;
    if (argc >= 5) {
        width = std::atoi(argv[3]);
        height = std::atoi(argv[4]);
    }

    Scene scene;
    if (!loadScene(inputFile, scene)) {
        cerr << "Erro ao carregar cena.\n";
        return 1;
    }

    cout << "Renderizando " << width << "x" << height << "...\n";

    std::vector<Vec3> framebuffer(width * height);

    double fovYrad = scene.camera.fovY * M_PI / 180.0;
    double imagePlaneHeight = 2.0 * std::tan(fovYrad * 0.5);
    double imagePlaneWidth = imagePlaneHeight * (static_cast<double>(width) / height);

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            double px = ((i + 0.5) / (double)width  - 0.5) * imagePlaneWidth;
            double py = (((height - 1 - j) + 0.5) / (double)height - 0.5) * imagePlaneHeight;

            Vec3 dir = normalize(scene.camera.w + scene.camera.u * px + scene.camera.v * py);
            Ray ray(scene.camera.eye, dir);

            Vec3 color = traceRay(scene, ray, 0);
            framebuffer[j * width + i] = color;
        }
        cout << "\rLinha " << (j+1) << "/" << height << std::flush;
    }
    cout << "\n";

    savePPM(outputFile, framebuffer, width, height);
    cout << "Imagem salva em " << outputFile << endl;

    return 0;
}
