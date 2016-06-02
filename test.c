#include "easyppm.h"

int main() {
    ppmstruct ppm = easyppm_create(100, 100, IMAGETYPE_PPM, ORIGIN_UPPERLEFT);

    easyppm_clear(&ppm, easyppm_rgb(0,0,200));
    easyppm_write(&ppm, "blue.ppm");
    easyppm_gamma_correct(&ppm, 2.2);
    easyppm_write(&ppm, "blue-corrected.ppm");

    easyppm_destroy(&ppm);

    return 0;
}
