#ifndef EASYPPM_H_
#define EASYPPM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    uint8_t r;  
    uint8_t g;  
    uint8_t b;  
} color;

typedef enum {
    ORIGIN_LOWERLEFT,
    ORIGIN_UPPERLEFT
} origin;

typedef enum {
    IMAGETYPE_PPM,
    IMAGETYPE_PGM
} imagetype;

typedef struct {
    int       width;
    int       height;
    color*    image;
    origin    otype;
    imagetype itype;
} ppmstruct;

ppmstruct easyppm_create(int width, int height, imagetype itype, origin otype);
void      easyppm_clear(ppmstruct* ppm, color c);
void      easyppm_set(ppmstruct* ppm, int x, int y, color c);
color     easyppm_get(ppmstruct* ppm, int x, int y);
color     easyppm_rgb(uint8_t r, uint8_t g, uint8_t b);
color     easyppm_rgb_float(float r, float g, float b);
color     easyppm_grey(uint8_t gr);
color     easyppm_grey_float(float gr);
void      easyppm_read(ppmstruct* ppm, const char* path, origin otype);
void      easyppm_write(ppmstruct* ppm, const char* path);
void      easyppm_destroy(ppmstruct* ppm);

#ifdef __cplusplus
}
#endif

#endif
