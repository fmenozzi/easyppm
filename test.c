#include "easyppm.h"

int main() {
    ppmstruct ppm = easyppm_create(100, 100, IMAGETYPE_PPM, ORIGIN_UPPERLEFT);
    ppmstruct pgm = easyppm_create(100, 100, IMAGETYPE_PGM, ORIGIN_UPPERLEFT);

    easyppm_clear(&ppm, easyppm_rgb(255,0,0));
    easyppm_write(&ppm, "red-in.ppm");
    easyppm_read(&ppm, "red-in.ppm", ORIGIN_UPPERLEFT);
    easyppm_write(&ppm, "red-out.ppm");

    easyppm_clear(&pgm, easyppm_grey(100));
    easyppm_write(&pgm, "grey-in.pgm");
    easyppm_read(&pgm, "grey-in.pgm", ORIGIN_UPPERLEFT);
    easyppm_write(&pgm, "grey-out.pgm");

    easyppm_destroy(&ppm);
    easyppm_destroy(&pgm);

    return 0;
}
