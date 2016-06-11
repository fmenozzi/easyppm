#include "easyppm.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

PPM   easyppm_create(int width, int height, imagetype itype);
void  easyppm_clear(PPM* ppm, color c);
void  easyppm_set(PPM* ppm, int x, int y, color c);
color easyppm_get(PPM* ppm, int x, int y);
color easyppm_rgb(uint8_t r, uint8_t g, uint8_t b);
color easyppm_grey(uint8_t gr);
color easyppm_black_white(int bw);
void  easyppm_gamma_correct(PPM* ppm, float gamma);
void  easyppm_invert_y(PPM* ppm);
void  easyppm_read(PPM* ppm, const char* path);
void  easyppm_write(PPM* ppm, const char* path);
void  easyppm_destroy(PPM* ppm);

static void easyppm_abort(PPM* ppm, const char* fmt, ...);
static void easyppm_check_extension(PPM* ppm, const char* path);
static int  easyppm_is_grey(color c);
static int  easyppm_is_black_white(color c);

/*
 * Creates new PPM from args, aborts if dimensions are invalid
 */
PPM easyppm_create(int width, int height, imagetype itype) {
    PPM ppm;

    if (width <= 0 || height <= 0)
        easyppm_abort(NULL, "Invalid image dimensions (%d, %d)\n", width, height);

    ppm.width  = width;
    ppm.height = height;
    if (itype == IMAGETYPE_PBM || itype == IMAGETYPE_PGM) {
        ppm.image = (uint8_t*)malloc(sizeof(*ppm.image) * width*height);
    } else {
        ppm.image = (uint8_t*)malloc(sizeof(*ppm.image) * width*height*EASYPPM_NUM_CHANNELS);
    }
    ppm.itype  = itype;

    return ppm;
}

/*
 * Fill entire color buffer with specified color
 */
void easyppm_clear(PPM* ppm, color c) {
    int x, y;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_clear()\n");

    if (ppm->itype == IMAGETYPE_PBM && !easyppm_is_black_white(c))
        easyppm_abort(ppm, "Passed invalid color to easyppm_clear() for PBM image\n");
    if (ppm->itype == IMAGETYPE_PGM && !easyppm_is_grey(c))
        easyppm_abort(ppm, "Passed invalid color to easyppm_clear() for PGM image\n");

    for (y = 0; y < ppm->height; y++)
        for (x = 0; x < ppm->width; x++)
            easyppm_set(ppm, x, y, c);
}

/*
 * Set appropriate color (RGB or greyscale)
 */
void easyppm_set(PPM* ppm, int x, int y, color c) {
    int i;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_set()\n");

    i = x + y*ppm->width;

    if (ppm->itype == IMAGETYPE_PBM || ppm->itype == IMAGETYPE_PGM) {
        ppm->image[i] = c.r;
    } else {
        ppm->image[EASYPPM_NUM_CHANNELS*i + 0] = c.r;
        ppm->image[EASYPPM_NUM_CHANNELS*i + 1] = c.g;
        ppm->image[EASYPPM_NUM_CHANNELS*i + 2] = c.b;
    }
}

/*
 * Get appropriate color (RGB or greyscale)
 */
color easyppm_get(PPM* ppm, int x, int y) {
    color c;
    int i;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_get()\n");

    i = x + y*ppm->width;

    if (ppm->itype == IMAGETYPE_PBM || ppm->itype == IMAGETYPE_PGM) {
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

/*
 * Create color from 8-bit RGB values
 */
color easyppm_rgb(uint8_t r, uint8_t g, uint8_t b) {
    color c;

    c.r = r;
    c.g = g;
    c.b = b;

    return c;
}

/*
 * Create color from 8-bit greyscale value
 */
color easyppm_grey(uint8_t gr) {
    color c;

    c.r = gr;
    c.g = gr;
    c.b = gr;

    return c;
}

/*
 * Create color from binary value: 0 for white
 * pixel, 1 for black pixel
 */
color easyppm_black_white(int bw) {
    color c;

    bw = (bw == 0 ? 255 : 0);

    c.r = bw;
    c.g = bw;
    c.b = bw;

    return c;
}

/*
 * Gamma-correct entire image by specified amount
 */
void easyppm_gamma_correct(PPM* ppm, float gamma) {
    int x, y;
    float r, g, b;
    float exp = 1 / gamma;
    color c;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_gamma_correct()\n");

    for (y = 0; y < ppm->height; y++) {
        for (x = 0; x < ppm->width; x++) {
            c = easyppm_get(ppm, x, y);

            r = (uint8_t)(powf(c.r / (float)EASYPPM_MAX_CHANNEL_VALUE, exp) * 255);
            g = (uint8_t)(powf(c.g / (float)EASYPPM_MAX_CHANNEL_VALUE, exp) * 255);
            b = (uint8_t)(powf(c.b / (float)EASYPPM_MAX_CHANNEL_VALUE, exp) * 255);

            easyppm_set(ppm, x, y, easyppm_rgb(r, g, b));
        }
    }
}

/*
 * Invert image y-axis for applications that assume an origin
 * in the lower left corner (easyppm defaults to an origin in
 * the upper left corner)
 */
void easyppm_invert_y(PPM* ppm) {
    int x, yt, yb;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_invert_y()\n");

    for (yt = 0, yb = ppm->height-1; yt <= yb; yt++, yb--) {
        for (x = 0; x < ppm->width; x++) {
            color tmp = easyppm_get(ppm, x, yb);
            easyppm_set(ppm, x, yb, easyppm_get(ppm, x, yt));
            easyppm_set(ppm, x, yt, tmp);
        }
    }
}

/*
 * Read image from file. Aborts if file could not be opened,
 * dimensions are invalid, or the file extension on the path
 * doesn't match the image type (.pbm for PBM files, .pgm for
 * PGM files, and .ppm for PPM files).
 */
void easyppm_read(PPM* ppm, const char* path) {
    FILE* fp;
    char itypestr[3];
    int width, height, dummy;
    int x, y;
    int gr;
    int r, g, b;
    color c;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_read()\n");

    easyppm_check_extension(ppm, path);

    easyppm_destroy(ppm);

    fp = fopen(path, "r");
    if (!fp)
        easyppm_abort(NULL, "Could not open file %s for reading\n", path);

    fscanf(fp, "%s\n", itypestr);
    if (strcmp(itypestr, "P1") == 0) {
        ppm->itype = IMAGETYPE_PBM;
        fscanf(fp, "%d %d\n", &width, &height);
    } else if (strcmp(itypestr, "P2") == 0) {
        ppm->itype = IMAGETYPE_PGM;
        fscanf(fp, "%d %d %d\n", &width, &height, &dummy);
    } else {
        ppm->itype = IMAGETYPE_PPM;
        fscanf(fp, "%d %d %d\n", &width, &height, &dummy);
    }

    ppm->width  = width;
    ppm->height = height;
    if (width <= 0 || height <= 0) {
        fclose(fp);
        easyppm_abort(NULL, "Passed invalid dimensions to easyppm_read() (%d, %d)\n", width, height);
    }

    if (ppm->itype == IMAGETYPE_PBM || ppm->itype == IMAGETYPE_PGM) {
        ppm->image = (uint8_t*)malloc(sizeof(*ppm->image) * width*height);
    } else {
        ppm->image = (uint8_t*)malloc(sizeof(*ppm->image) * width*height*EASYPPM_NUM_CHANNELS);
    }

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (ppm->itype == IMAGETYPE_PBM) {
                fscanf(fp, "%d\n", &gr);
                gr = (gr == 0 ? 1 : 0);
                c.r = gr;
                c.g = gr;
                c.b = gr;
            } else if (ppm->itype == IMAGETYPE_PGM) {
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
 * Write image to file. Aborts if file could not be opened or
 * the file extension on the path doesn't match the image type
 * (.pbm for PBM files, .pgm for PGM files, and .ppm for PPM
 * files).
 */
void easyppm_write(PPM* ppm, const char* path) {
    FILE* fp;
    int x, y;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_write()\n");

    easyppm_check_extension(ppm, path);

    fp = fopen(path, "w");
    if (!fp)
        easyppm_abort(ppm, "Could not open file %s for writing\n", path);

    if (ppm->itype == IMAGETYPE_PBM) {
        fprintf(fp, "P1\n");
        fprintf(fp, "%d %d\n", ppm->width, ppm->height);
    } else if (ppm->itype == IMAGETYPE_PGM) {
        fprintf(fp, "P2\n");
        fprintf(fp, "%d %d %d\n", ppm->width, ppm->height, EASYPPM_MAX_CHANNEL_VALUE);
    } else {
        fprintf(fp, "P3\n");
        fprintf(fp, "%d %d %d\n", ppm->width, ppm->height, EASYPPM_MAX_CHANNEL_VALUE);
    }

    for (y = 0; y < ppm->height; y++) {
        for (x = 0; x < ppm->width; x++) {
            color c = easyppm_get(ppm, x, y);
            if (ppm->itype == IMAGETYPE_PBM) {
                fprintf(fp, "%d\n", c.r == 0 ? 1 : 0);
            } else if (ppm->itype == IMAGETYPE_PGM) {
                fprintf(fp, "%d\n", c.r);
            } else {
                fprintf(fp, "%d %d %d\n", c.r, c.g, c.b);
            }
        }
    }

    fclose(fp);
}

/*
 * Free image buffer
 */
void easyppm_destroy(PPM* ppm) {
    if (ppm && ppm->image)
        free(ppm->image);
    ppm = NULL;
}

/*
 * Cleanup resources and abort program
 */
static void easyppm_abort(PPM* ppm, const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    vfprintf(stderr, fmt, argptr);
    va_end(argptr);
    fprintf(stderr, "Aborting\n");

    easyppm_destroy(ppm);

    exit(EXIT_FAILURE);
}

/*
 * Check that the image type on the PPM matches the file
 * extension on the filepath provided
 */
static void easyppm_check_extension(PPM* ppm, const char* path) {
    const char* extension;
    size_t i;
    int found;
    int ends_pbm, ends_pgm, ends_ppm;

    if (!ppm)
        easyppm_abort(ppm, "Passed NULL PPM to easyppm_check_extension()\n");

    found = 0;
    for (i = 0; i < strlen(path); i++) {
        if (path[i] == '.') {
            extension = &path[i];
            found = 1;
        }
    }

    if (!found)
        easyppm_abort(ppm, "Malformed filepath %s\n", path);

    ends_pbm = strcmp(extension, ".pbm") == 0;
    ends_pgm = strcmp(extension, ".pgm") == 0;
    ends_ppm = strcmp(extension, ".ppm") == 0;

    if (ppm->itype == IMAGETYPE_PBM && !ends_pbm)
        easyppm_abort(ppm, "File path %s for PBM file does not end in .pbm\n", path);
    if (ppm->itype == IMAGETYPE_PGM && !ends_pgm)
        easyppm_abort(ppm, "File path %s for PGM file does not end in .pgm\n", path);
    if (ppm->itype == IMAGETYPE_PPM && !ends_ppm)
        easyppm_abort(ppm, "File path %s for PPM file does not end in .ppm\n", path);
}

/*
 * Determine if color is greyscale
 */
static int easyppm_is_grey(color c) {
    return c.r == c.g && c.g == c.b;
}

/*
 * Determine if color is black or white
 */
static int easyppm_is_black_white(color c) {
    return easyppm_is_grey(c) && (c.r == 255 || c.r == 0);
}
