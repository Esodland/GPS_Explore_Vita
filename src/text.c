#include "text.h"
#include "gfx.h"

#include <stdlib.h>
#include <string.h>
#include <psp2/pgf.h>
#include <psp2/sysmodule.h>
#include <vitaGL.h>

#define ATLAS_SIZE 512
#define GLYPH_COUNT 256   /* ASCII + Latin-1 */

typedef struct {
    int state;            /* 0 = pas encore chargé, 1 = prêt, -1 = absent de la police */
    float u0, v0, u1, v1;
    int w, h, left, top;
    float advance;
} Glyph;

static SceFontLibHandle font_lib;
static SceFontHandle font;
static GLuint atlas_tex;
static Glyph glyphs[GLYPH_COUNT];
static int pen_x, pen_y, row_h;

static void *pgf_alloc(void *userdata, unsigned int size) {
    (void)userdata;
    return malloc(size);
}

static void pgf_free(void *userdata, void *p) {
    (void)userdata;
    free(p);
}

int text_init(void) {
    unsigned int err = 0;
    /* ScePgf n'est pas chargé par défaut : sans lui, sceFontNewLib saute à l'adresse 0. */
    int ret = sceSysmoduleLoadModule(SCE_SYSMODULE_PGF);
    if (ret < 0) return ret;
    SceFontNewLibParams params = { NULL, 4, NULL, pgf_alloc, pgf_free, NULL, NULL, NULL, NULL, NULL, NULL };
    font_lib = sceFontNewLib(&params, &err);
    if (err) return (int)err;

    SceFontStyle style;
    memset(&style, 0, sizeof(style));
    style.fontFamily = SCE_FONT_FAMILY_SANS_SERIF;
    style.fontStyle = SCE_FONT_STYLE_REGULAR;
    style.fontLanguage = SCE_FONT_LANGUAGE_LATIN;
    int index = sceFontFindOptimumFont(font_lib, &style, &err);
    if (err || index < 0) index = 0;
    font = sceFontOpen(font_lib, index, 0, &err);
    if (err) return (int)err;

    glGenTextures(1, &atlas_tex);
    glBindTexture(GL_TEXTURE_2D, atlas_tex);
    void *blank = calloc(ATLAS_SIZE * ATLAS_SIZE, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLAS_SIZE, ATLAS_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, blank);
    free(blank);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    pen_x = pen_y = 1;
    row_h = 0;
    return 0;
}

void text_fini(void) {
    if (atlas_tex) glDeleteTextures(1, &atlas_tex);
    atlas_tex = 0;
    if (font) sceFontClose(font);
    if (font_lib) sceFontDoneLib(font_lib);
    font = NULL;
    font_lib = NULL;
    sceSysmoduleUnloadModule(SCE_SYSMODULE_PGF);
}

/* Décode un point de code UTF-8 (1 ou 2 octets suffisent pour le Latin-1). */
static unsigned next_codepoint(const char **s) {
    const unsigned char *p = (const unsigned char *)*s;
    unsigned c = *p++;
    if ((c & 0xE0) == 0xC0 && (*p & 0xC0) == 0x80) {
        c = ((c & 0x1F) << 6) | (*p++ & 0x3F);
    } else if (c >= 0x80) {
        while ((*p & 0xC0) == 0x80) p++;
        c = '?';
    }
    *s = (const char *)p;
    return c;
}

/* Rastérise un glyphe dans l'atlas. Hors glBegin/glEnd. */
static Glyph *load_glyph(unsigned c) {
    if (c >= GLYPH_COUNT) c = '?';
    Glyph *g = &glyphs[c];
    if (g->state != 0) return g;

    SceFontCharInfo info;
    if (!font || sceFontGetCharInfo(font, c, &info) < 0) {
        g->state = -1;
        return g;
    }
    g->w = info.bitmapWidth;
    g->h = info.bitmapHeight;
    g->left = info.bitmapLeft;
    g->top = info.bitmapTop;
    g->advance = info.sfp26AdvanceH / 64.0f;
    g->state = 1;
    if (g->w == 0 || g->h == 0) return g;   /* espace : avance seulement */

    if (pen_x + g->w + 1 > ATLAS_SIZE) {
        pen_x = 1;
        pen_y += row_h + 1;
        row_h = 0;
    }
    if (pen_y + g->h + 1 > ATLAS_SIZE) {   /* atlas plein : glyphe ignoré */
        g->w = g->h = 0;
        return g;
    }

    unsigned char *gray = calloc(g->w * g->h, 1);
    unsigned int *rgba = malloc(g->w * g->h * 4);
    if (!gray || !rgba) {
        free(gray);
        free(rgba);
        g->w = g->h = 0;
        return g;
    }
    SceFontGlyphImage img;
    memset(&img, 0, sizeof(img));
    img.pixelFormat = SCE_FONT_PIXELFORMAT_8;
    img.bufWidth = g->w;
    img.bufHeight = g->h;
    img.bytesPerLine = g->w;
    img.bufferPtr = (unsigned int)gray;
    sceFontGetCharGlyphImage(font, c, &img);
    for (int i = 0; i < g->w * g->h; ++i)
        rgba[i] = RGBA8(255, 255, 255, gray[i]);

    glBindTexture(GL_TEXTURE_2D, atlas_tex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, pen_x, pen_y, g->w, g->h, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    free(gray);
    free(rgba);

    g->u0 = (float)pen_x / ATLAS_SIZE;
    g->v0 = (float)pen_y / ATLAS_SIZE;
    g->u1 = (float)(pen_x + g->w) / ATLAS_SIZE;
    g->v1 = (float)(pen_y + g->h) / ATLAS_SIZE;
    pen_x += g->w + 1;
    if (g->h > row_h) row_h = g->h;
    return g;
}

float text_width(float scale, const char *str) {
    float w = 0;
    while (*str) {
        Glyph *g = load_glyph(next_codepoint(&str));
        if (g->state == 1) w += g->advance * scale;
    }
    return w;
}

void text_draw(float x, float y, unsigned int color, float scale, const char *str) {
    if (!atlas_tex) return;
    /* Rastériser d'abord les glyphes manquants : glTexSubImage2D est interdit dans glBegin. */
    for (const char *p = str; *p;) load_glyph(next_codepoint(&p));

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, atlas_tex);
    gfx_set_color(color);
    glBegin(GL_QUADS);
    float pen = x;
    while (*str) {
        Glyph *g = load_glyph(next_codepoint(&str));
        if (g->state != 1) continue;
        if (g->w > 0) {
            float gx = pen + g->left * scale;
            float gy = y - g->top * scale;
            float gw = g->w * scale, gh = g->h * scale;
            glTexCoord2f(g->u0, g->v0); glVertex3f(gx, gy, 0);
            glTexCoord2f(g->u1, g->v0); glVertex3f(gx + gw, gy, 0);
            glTexCoord2f(g->u1, g->v1); glVertex3f(gx + gw, gy + gh, 0);
            glTexCoord2f(g->u0, g->v1); glVertex3f(gx, gy + gh, 0);
        }
        pen += g->advance * scale;
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
