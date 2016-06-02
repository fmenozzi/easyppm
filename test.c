#include <stdio.h>

#include "easyppm.h"

void assert(int pred, const char* msg) {
    if (!pred)
        fprintf(stderr, "ERROR: %s\n", msg);
}

int images_equal(PPM* ppm1, PPM* ppm2) {
    int x, y;
    color c1, c2;

    if (ppm1->width != ppm2->width || ppm1->height != ppm2->height)
        return 0;

    for (x = 0; x < ppm1->width; x++) {
        for (y = 0; y < ppm1->height; y++) {
            c1 = easyppm_get(ppm1, x, y);
            c2 = easyppm_get(ppm2, x, y);
            if (c1.r != c2.r || c1.g != c2.g || c1.b != c2.b)
                return 0;
        }
    }

    return 1;
}

int main() {
    PPM ppm1 = easyppm_create(100, 100, IMAGETYPE_PPM, ORIGIN_UPPERLEFT);
    PPM ppm2 = easyppm_create(100, 100, IMAGETYPE_PPM, ORIGIN_UPPERLEFT);
    PPM pbm1 = easyppm_create(100, 100, IMAGETYPE_PBM, ORIGIN_UPPERLEFT);
    PPM pbm2 = easyppm_create(100, 100, IMAGETYPE_PBM, ORIGIN_UPPERLEFT);

    easyppm_clear(&ppm1, easyppm_rgb(255,0,0));
    easyppm_write(&ppm1, "red.ppm");

    easyppm_read(&ppm2, "red.ppm", ORIGIN_UPPERLEFT);

    assert(images_equal(&ppm1, &ppm2), "Images are not equal");

    easyppm_clear(&pbm1, easyppm_black_white(1));
    easyppm_write(&pbm1, "black.pbm");
    easyppm_read(&pbm2, "black.pbm", ORIGIN_UPPERLEFT);
    easyppm_clear(&pbm2, easyppm_black_white(0));
    easyppm_write(&pbm2, "white.pbm");

    easyppm_destroy(&ppm1);
    easyppm_destroy(&ppm2);
    easyppm_destroy(&pbm1);
    easyppm_destroy(&pbm2);

    return 0;
}
