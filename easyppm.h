#ifndef EASYPPM_H_
#define EASYPPM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define EASYPPM_NUM_CHANNELS 3
#define EASYPPM_MAX_CHANNEL_VALUE 255

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
    IMAGETYPE_PBM,
    IMAGETYPE_PGM,
    IMAGETYPE_PPM
} imagetype;

typedef struct {
    int       width;
    int       height;
    uint8_t*  image;
    origin    otype;
    imagetype itype;
} PPM;

PPM   easyppm_create(int width, int height, imagetype itype, origin otype);
void  easyppm_clear(PPM* ppm, color c);
void  easyppm_set(PPM* ppm, int x, int y, color c);
color easyppm_get(PPM* ppm, int x, int y);
color easyppm_rgb(uint8_t r, uint8_t g, uint8_t b);
color easyppm_grey(uint8_t gr);
color easyppm_black_white(int bw);
void  easyppm_gamma_correct(PPM* ppm, float gamma);
void  easyppm_read(PPM* ppm, const char* path, origin otype);
void  easyppm_write(PPM* ppm, const char* path);
void  easyppm_destroy(PPM* ppm);

#ifdef __cplusplus
}
#endif

#endif
