#include <stdio.h>

#include "easyppm.h"

void assert(int pred, const char* msg) {
    if (!pred)
        fprintf(stderr, "ERROR: %s\n", msg);
}

int images_equal(PPM* ppm1, PPM* ppm2) {
    int x, y;
    ppmcolor c1, c2;

    assert(ppm1 != NULL, "NULL ppm1 passed in images_equal()");
    assert(ppm2 != NULL, "NULL ppm2 passed in images_equal()");

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

void draw_rect_ltrb(PPM* ppm, int left, int top, int right, int bottom) {
    int x, y;

    assert(ppm != NULL, "NULL ppm passed in draw_rect");

    assert(0 <= left   && left   <= ppm->width,  "Invalid left value");
    assert(0 <= top    && top    <= ppm->height, "Invalid top value");
    assert(0 <= right  && right  <= ppm->width,  "Invalid right value");
    assert(0 <= bottom && bottom <= ppm->height, "Invalid bottom value");

    for (x = left; x < right; x++) {
        for (y = top; y < bottom; y++) {
            easyppm_set(ppm, x, y, easyppm_rgb(255, 0, 0));
        }
    }
}

int main() {
    PPM ppm1 = easyppm_create(200, 100, IMAGETYPE_PPM);
    PPM ppm2 = easyppm_create(200, 100, IMAGETYPE_PPM);

    easyppm_clear(&ppm1, easyppm_rgb(255, 255, 255));
    easyppm_clear(&ppm2, easyppm_rgb(255, 255, 255));

    draw_rect_ltrb(&ppm1, 0, 0, 40, 40);
    easyppm_write(&ppm1, "before.ppm");

    draw_rect_ltrb(&ppm2, 0, 60, 40, 100);
    easyppm_invert_y(&ppm2);
    easyppm_write(&ppm2, "after.ppm");

    assert(images_equal(&ppm1, &ppm2), "Images are not equal");

    easyppm_destroy(&ppm1);
    easyppm_destroy(&ppm2);

    return 0;
}
