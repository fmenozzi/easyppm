#include "easyppm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

ppmstruct easyppm_create(int width, int height, imagetype itype, origin otype);
void      easyppm_clear(ppmstruct* ppm, color c);
void      easyppm_set(ppmstruct* ppm, int x, int y, color c);
color     easyppm_get(ppmstruct* ppm, int x, int y);
color     easyppm_rgb(uint8_t r, uint8_t g, uint8_t b);
color     easyppm_rgb_float(float r, float g, float b);
color     easyppm_grey(uint8_t gr);
color     easyppm_grey_float(float gr);
void      easyppm_gamma_correct(ppmstruct* ppm, float gamma);
void      easyppm_read(ppmstruct* ppm, const char* path, origin otype);
void      easyppm_write(ppmstruct* ppm, const char* path);
void      easyppm_destroy(ppmstruct* ppm);

static void easyppm_abort(ppmstruct* ppm, const char* msg);
static void easyppm_check_extension(ppmstruct* ppm, const char* path);

ppmstruct easyppm_create(int width, int height, imagetype itype, origin otype) {
    ppmstruct ppm;

    if (width <= 0)
        easyppm_abort(NULL, "Passed negative width");
    if (height <= 0)
        easyppm_abort(NULL, "Passed negative height");

    ppm.width  = width;
    ppm.height = height;
    if (itype == IMAGETYPE_PGM) {
        ppm.image  = (uint8_t*)malloc(sizeof(*ppm.image) * width*height);
    } else {
        ppm.image  = (uint8_t*)malloc(sizeof(*ppm.image) * width*height*EASYPPM_NUM_CHANNELS);
    }
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
    int i = x + y*ppm->width;

    if (ppm->itype == IMAGETYPE_PGM) {
        ppm->image[i] = c.r;
    } else {
        ppm->image[EASYPPM_NUM_CHANNELS*i + 0] = c.r;
        ppm->image[EASYPPM_NUM_CHANNELS*i + 1] = c.g;
        ppm->image[EASYPPM_NUM_CHANNELS*i + 2] = c.b;
    }
}

color easyppm_get(ppmstruct* ppm, int x, int y) {
    color c;
    int i = x + y*ppm->width;

    if (ppm->itype == IMAGETYPE_PGM) {
        c.r = ppm->image[i];
        c.g = ppm->image[i];
        c.b = ppm->image[i];
    } else {
        c.r = ppm->image[EASYPPM_NUM_CHANNELS*i + 0];
        c.g = ppm->image[EASYPPM_NUM_CHANNELS*i + 1];
        c.b = ppm->image[EASYPPM_NUM_CHANNELS*i + 2];
    }
    return c;
}

color easyppm_rgb(uint8_t r, uint8_t g, uint8_t b) {
    color c;

    c.r = r;
    c.g = g;
    c.b = b;

    return c;
}

color easyppm_rgb_float(float r, float g, float b) {
    color c;

    c.r = r * 255;
    c.g = g * 255;
    c.b = b * 255;

    return c;
}

color easyppm_grey(uint8_t gr) {
    color c;

    c.r = gr;
    c.g = gr;
    c.b = gr;

    return c;
}


color easyppm_grey_float(float gr) {
    color c;

    c.r = gr * 255;
    c.g = gr * 255;
    c.b = gr * 255;

    return c;
}

void easyppm_gamma_correct(ppmstruct* ppm, float gamma) {
    int x, y;
    float r, g, b;
    float exp;
    color c;

    for (x = 0; x < ppm->width; x++) {
        for (y = 0; y < ppm->height; y++) {
            c = easyppm_get(ppm, x, y);

            exp = 1 / gamma;

            r = powf(c.r / 255.f, exp);
            g = powf(c.g / 255.f, exp);
            b = powf(c.b / 255.f, exp);

            easyppm_set(ppm, x, y, easyppm_rgb_float(r,g,b));
        }
    }
}

/*
 * Read image from file
 */
void easyppm_read(ppmstruct* ppm, const char* path, origin otype) {
    FILE* fp;
    char itypestr[3];
    int width, height, dummy;
    int x, y;
    int gr;
    int r, g, b;
    color c;

    easyppm_check_extension(ppm, path);

    easyppm_destroy(ppm);

    ppm->otype = otype;

    fp = fopen(path, "r");
    if (!fp)
        easyppm_abort(NULL, "Could not open file for reading");

    fscanf(fp, "%s\n", itypestr);
    if (strcmp(itypestr, "P2") == 0) {
        ppm->itype = IMAGETYPE_PGM;
    } else {
        ppm->itype = IMAGETYPE_PPM;
    }

    fscanf(fp, "%d %d %d\n", &width, &height, &dummy);
    ppm->width  = width;
    ppm->height = height;
    if (width <= 0) {
        fclose(fp);
        easyppm_abort(NULL, "Passed negative width");
    }
    if (height <= 0) {
        fclose(fp);
        easyppm_abort(NULL, "Passed negative height");
    }

    if (ppm->itype == IMAGETYPE_PGM) {
        ppm->image = (uint8_t*)malloc(sizeof(*ppm->image) * width*height);
    } else {
        ppm->image = (uint8_t*)malloc(sizeof(*ppm->image) * width*height*EASYPPM_NUM_CHANNELS);
    }

    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            if (ppm->itype == IMAGETYPE_PGM) {
                fscanf(fp, "%d\n", &gr);
                c.r = gr;
                c.g = gr;
                c.b = gr;
            } else {
                fscanf(fp, "%d %d %d\n", &r, &g, &b);
                c.r = r;
                c.g = g;
                c.b = b;
            }
            easyppm_set(ppm, x, y, c);
        }
    }

    fclose(fp);
}

/*
 * Write image to file
 */
void easyppm_write(ppmstruct* ppm, const char* path) {
    FILE* fp;
    int x, y;

    easyppm_check_extension(ppm, path);

    fp = fopen(path, "w");
    if (!fp)
        easyppm_abort(ppm, "Could not open file for writing");

    fprintf(fp, ppm->itype == IMAGETYPE_PGM ? "P2\n" : "P3\n");
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

static void easyppm_check_extension(ppmstruct* ppm, const char* path) {
    const char* extension;
    int i;

    for (i = 0; i < strlen(path); i++)
        if (path[i] == '.')
            extension = &path[i];
    if (ppm->itype == IMAGETYPE_PGM && strcmp(extension, ".pgm") != 0)
        easyppm_abort(ppm, "File path for PGM file does not end in .pgm");
    if (ppm->itype == IMAGETYPE_PPM && strcmp(extension, ".ppm") != 0)
        easyppm_abort(ppm, "File path for PPM file does not end in .ppm");
}
