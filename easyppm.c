#include "easyppm.h"

#include <stdio.h>
#include <stdlib.h>

ppmstruct easyppm_create(int width, int height, imagetype itype, origin otype) {
    ppmstruct ppm;

    if (width <= 0)
        easyppm_abort(NULL, "Passed negative width");
    if (height <= 0)
        easyppm_abort(NULL, "Passed negative height");

    ppm.width  = width;
    ppm.height = height;
    ppm.image  = (uint8_t*)malloc(sizeof(uint8_t) * width*height);
    ppm.otype  = otype;
    ppm.itype  = itype;

    return ppm;
}

void easyppm_clear(ppmstruct* ppm, color c) {
    int x, y;
    for (x = 0; x < ppm->width; x++)
        for (y = 0; y < ppm->height; y++)
            easyppm_set(ppm, x, y, c);
}

/*
 * Write image to file
 */
void easyppm_write(ppmstruct* ppm, const char* path) {
    FILE* fp;
    int x, y;

    fp = fopen(path, "w");
    fprintf(fp, ppm->itype == IMAGETYPE_PGM ? "P2" : "P3");
    fprintf(fp, "%d %d %d\n", ppm->width, ppm->height, 255);

    for (x = 0; x < ppm->width; x++) {
        for (y = 0; y < ppm->height; y++) {
            color c = easyppm_get(ppm, x, y);
            if (ppm->itype == IMAGETYPE_PGM) {
                fprintf(fp, "%d\n", c.r);
            } else {
                fprintf(fp, "%d %d %d\n", c.r, c.g, c.b);
            }
            fprintf(fp, "%d %d %d\n", c.r, c.g, c.b);
        }
    }

    fclose(fp);
}

/*
 * Destroy ppmstruct
 */
void easyppm_destroy(ppmstruct* ppm) {
    if (ppm)
        free(ppm);
}

/*
 * Cleanup resources and abort program
 */
static void easyppm_abort(ppmstruct* ppm, const char* msg) {
    fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "Aborting\n");
    easyppm_destroy(ppm);
    exit(EXIT_FAILURE);
}
