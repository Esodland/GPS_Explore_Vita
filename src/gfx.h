#ifndef GPS_EXPLORE_GFX_H
#define GPS_EXPLORE_GFX_H

#define SCREEN_W 960
#define SCREEN_H 544

/* Même encodage que vita2d : 0xAABBGGRR */
#define RGBA8(r, g, b, a) ((((a) & 0xFF) << 24) | (((b) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((r) & 0xFF))

/* Initialise vitaGL (écran 960x544, MSAA 4x). */
int gfx_init(void);
void gfx_fini(void);

void gfx_frame_begin(unsigned int clear_color);
void gfx_frame_end(void);

/* Repère 2D plein écran, origine en haut à gauche, sans profondeur. */
void gfx_begin_2d(void);

void gfx_set_color(unsigned int color);
void gfx_rect(float x, float y, float w, float h, unsigned int color);
void gfx_rect_vgradient(float x, float y, float w, float h, unsigned int top, unsigned int bottom);
void gfx_circle(float x, float y, float radius, unsigned int color);

#endif
