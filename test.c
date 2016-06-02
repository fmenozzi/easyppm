#include "easyppm.h"

int main() {
    ppmstruct ppm = easyppm_create(100, 100, IMAGETYPE_PPM, ORIGIN_UPPERLEFT);

    easyppm_clear(&ppm, easyppm_rgb(255,255,255));
    easyppm_write(&ppm, "write.ppm");

    easyppm_read(&ppm, "write.ppm", ORIGIN_UPPERLEFT);
    easyppm_write(&ppm, "read.ppm");

    easyppm_destroy(&ppm);

    return 0;
}
