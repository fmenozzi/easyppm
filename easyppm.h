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
    ORIGIN_UPPERLEFT,
} origin;

typedef enum {
    IMAGETYPE_PPM,
    IMAGETYPE_PGM,
} imagetype;

typedef struct {
    int      width;
    int      height;
    int      size;
    uint8_t* image;
} ppmstruct;

ppmstruct easyppm_create(int width, int height, imagetype image, origin o);
void easyppm_clear(ppmstruct* ppm, color c);
void easyppm_write(ppmstruct* ppm, const char* path);
void easyppm_destroy(ppmstruct* ppm);

#ifdef __cplusplus
}
#endif

#endif
