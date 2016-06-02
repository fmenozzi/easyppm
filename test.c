#include "easyppm.h"

int main() {
    ppmstruct ppm = easyppm_create(100, 100, IMAGETYPE_PPM, ORIGIN_UPPERLEFT);

    easyppm_clear(&ppm, easyppm_rgb(255, 0, 0));
    easyppm_write(&ppm, "red.ppm");

    easyppm_clear(&ppm, easyppm_rgb(0, 255, 0));
    easyppm_write(&ppm, "green.ppm");

    easyppm_clear(&ppm, easyppm_rgb(0, 0, 255));
    easyppm_write(&ppm, "blue.ppm");

    easyppm_destroy(&ppm);

    return 0;
}
