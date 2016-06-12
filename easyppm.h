#ifndef EASYPPM_H_
#define EASYPPM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define EASYPPM_NUM_CHANNELS 3
#define EASYPPM_MAX_CHANNEL_VALUE 255

#define PPMBYTE unsigned char

typedef struct {
    PPMBYTE r;  
    PPMBYTE g;  
    PPMBYTE b;  
} ppmcolor;

typedef enum {
    IMAGETYPE_PBM,
    IMAGETYPE_PGM,
    IMAGETYPE_PPM
} imagetype;

typedef struct {
    int       width;
    int       height;
    PPMBYTE*  image;
    imagetype itype;
} PPM;

PPM      easyppm_create(int width, int height, imagetype itype);
void     easyppm_clear(PPM* ppm, ppmcolor c);
void     easyppm_set(PPM* ppm, int x, int y, ppmcolor c);
ppmcolor easyppm_get(PPM* ppm, int x, int y);
ppmcolor easyppm_rgb(PPMBYTE r, PPMBYTE g, PPMBYTE b);
ppmcolor easyppm_grey(PPMBYTE gr);
ppmcolor easyppm_black_white(int bw);
void     easyppm_gamma_correct(PPM* ppm, float gamma);
void     easyppm_invert_y(PPM* ppm);
void     easyppm_read(PPM* ppm, const char* path);
void     easyppm_write(PPM* ppm, const char* path);
void     easyppm_destroy(PPM* ppm);

#ifdef __cplusplus
}
#endif

#endif
