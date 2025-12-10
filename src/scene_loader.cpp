#include "scene_loader.h"
#include "texture.h"
#include "camera.h"
#include <fstream>
#include <iostream>

using std::cerr;
using std::endl;

bool loadScene(const std::string &filename, Scene &scene) {
    std::ifstream in(filename);
    if (!in) {
        cerr << "Nao foi possivel abrir arquivo de cena: " << filename << endl;
        return false;
    }

    in >> scene.camera.eye.x >> scene.camera.eye.y >> scene.camera.eye.z;
    in >> scene.camera.center.x >> scene.camera.center.y >> scene.camera.center.z;
    in >> scene.camera.up_hint.x >> scene.camera.up_hint.y >> scene.camera.up_hint.z;
    in >> scene.camera.fovY;

    int numLights;
    in >> numLights;
    scene.lights.resize(numLights);
    for (int i = 0; i < numLights; ++i) {
        Light L;
        in >> L.position.x >> L.position.y >> L.position.z;
        in >> L.color.x >> L.color.y >> L.color.z;
        in >> L.kc >> L.kl >> L.kq;
        scene.lights[i] = L;
    }

    int numPig;
    in >> numPig;
    scene.pigments.resize(numPig);

    for (int i = 0; i < numPig; ++i) {
        std::string type;
        in >> type;
        Pigment p;
        if (type == "solid") {
            p.type = PigmentType::SOLID;
            in >> p.color1.x >> p.color1.y >> p.color1.z;
        } else if (type == "checker") {
            p.type = PigmentType::CHECKER;
            in >> p.color1.x >> p.color1.y >> p.color1.z;
            in >> p.color2.x >> p.color2.y >> p.color2.z;
            in >> p.size;
        } else if (type == "texmap") {
            p.type = PigmentType::TEXMAP;
            std::string texfile;
            in >> texfile;
            for (int k = 0; k < 4; ++k) in >> p.P0[k];
            for (int k = 0; k < 4; ++k) in >> p.P1[k];
            p.tex = loadPPM(texfile);
        } else {
            cerr << "Tipo de pigmento desconhecido: " << type << endl;
            return false;
        }
        scene.pigments[i] = p;
    }

    int numFin;
    in >> numFin;
    scene.finishes.resize(numFin);
    for (int i = 0; i < numFin; ++i) {
        Finish f;
        in >> f.ka >> f.kd >> f.ks >> f.alpha >> f.kr >> f.kt >> f.ior;
        scene.finishes[i] = f;
    }

    int numObj;
    in >> numObj;
    for (int i = 0; i < numObj; ++i) {
        int pigIdx, finIdx;
        std::string type;
        in >> pigIdx >> finIdx >> type;

        if (type == "sphere") {
            Vec3 c;
            double r;
            in >> c.x >> c.y >> c.z >> r;
            auto sp = std::make_unique<Sphere>(c, r, pigIdx, finIdx);
            scene.objects.push_back(std::move(sp));
        } else if (type == "polyhedron") {
            int nFaces;
            in >> nFaces;
            auto poly = std::make_unique<Polyhedron>(pigIdx, finIdx);
            for (int j = 0; j < nFaces; ++j) {
                Plane pl;
                in >> pl.n.x >> pl.n.y >> pl.n.z >> pl.d;
                poly->planes.push_back(pl);
            }
            scene.objects.push_back(std::move(poly));
        } else {
            cerr << "Tipo de objeto desconhecido: " << type << endl;
            return false;
        }
    }

    buildCamera(scene.camera);
    return true;
}
