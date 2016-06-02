#include "easyppm.h"

#include <stdio.h>
#include <stdlib.h>

ppmstruct easyppm_create(int width, int height, imagetype itype, origin otype);
void      easyppm_clear(ppmstruct* ppm, color c);
void      easyppm_set(ppmstruct* ppm, int x, int y, color c);
color     easyppm_get(ppmstruct* ppm, int x, int y);
color     easyppm_rgb(uint8_t r, uint8_t g, uint8_t b);
color     easyppm_rgb_float(float r, float g, float b);
color     easyppm_grey(uint8_t gr);
color     easyppm_grey_float(float gr);
void      easyppm_write(ppmstruct* ppm, const char* path);
void      easyppm_destroy(ppmstruct* ppm);

static void easyppm_abort(ppmstruct* ppm, const char* msg);

ppmstruct easyppm_create(int width, int height, imagetype itype, origin otype) {
    ppmstruct ppm;

    /* Reject invalid dimensions */
    if (width <= 0)
        easyppm_abort(NULL, "Passed negative width");
    if (height <= 0)
        easyppm_abort(NULL, "Passed negative height");

    ppm.width  = width;
    ppm.height = height;
    ppm.image  = (color*)malloc(sizeof(*ppm.image) * width*height);
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

void easyppm_set(ppmstruct* ppm, int x, int y, color c) {
    ppm->image[x + y*ppm->width] = c;
}

color easyppm_get(ppmstruct* ppm, int x, int y) {
    return ppm->image[x + y*ppm->width];
}

color easyppm_rgb(uint8_t r, uint8_t g, uint8_t b) {
    color c;

    c.r = r;
    c.g = g;
    c.b = b;

    return c;
}

/* TODO */
color easyppm_rgb_float(float r, float g, float b) {
    color c;

    c.r = 0;
    c.g = 0;
    c.b = 0;

    return c;
}

color easyppm_grey(uint8_t gr) {
    color c;

    c.r = gr;
    c.g = gr;
    c.b = gr;

    return c;
}


/* TODO */
color easyppm_grey_float(float gr) {
    color c;

    c.r = 0;
    c.g = 0;
    c.b = 0;

    return c;
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
        free(ppm->image);
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
