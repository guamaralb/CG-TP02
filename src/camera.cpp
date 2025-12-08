#include "camera.h"
#include "constants.h"

void buildCamera(Camera &cam) {
    Vec3 forward = normalize(cam.center - cam.eye);
    Vec3 up = normalize(cam.up_hint);
    if (length(up) < EPS) {
        up = Vec3(0,1,0);
    }
    Vec3 right = normalize(cross(forward, up));
    Vec3 trueUp = cross(right, forward);

    cam.w = forward;
    cam.u = right;
    cam.v = trueUp;
}
