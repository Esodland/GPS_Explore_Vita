#include "gfx.h"

#include <math.h>
#include <vitaGL.h>

/* Taille de la mémoire des dialogues communs (valeur interne de vitaGL, mem_utils.h). */
#define CDIALOG_MEM_SIZE 0x8C6000

int gfx_init(void) {
    /* 4 Mio pour le mode immédiat (glBegin/glEnd) : largement assez pour la carte et le texte.
     * Par défaut vitaGL prend toute la CDRAM et toute la PHYCONT du processus : on laisse
     * des réserves aux bibliothèques système chargées dans l'appli (localisation, HTTP).
     * Attention : vglInit* ne signale pas d'échec ; GL_TRUE veut seulement dire que la
     * résolution demandée a été réduite au maximum de l'écran. */
    vglInitWithCustomThreshold(4 * 1024 * 1024, SCREEN_W, SCREEN_H,
                               32 * 1024 * 1024,   /* RAM utilisateur laissée libre */
                               16 * 1024 * 1024,   /* CDRAM */
                               8 * 1024 * 1024,    /* PHYCONT */
                               CDIALOG_MEM_SIZE,   /* toute la mémoire des dialogues reste libre */
                               SCE_GXM_MULTISAMPLE_4X);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return 0;
}

void gfx_fini(void) {
    /* Cette version de vitaGL n'expose pas de fonction de fermeture : la sortie du
     * processus libère le contexte GXM. On attend seulement la fin du rendu. */
    glFinish();
}

void gfx_frame_begin(unsigned int c) {
    glViewport(0, 0, SCREEN_W, SCREEN_H);
    glDisable(GL_SCISSOR_TEST);
    glClearColor((c & 0xFF) / 255.0f, ((c >> 8) & 0xFF) / 255.0f, ((c >> 16) & 0xFF) / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gfx_frame_end(void) {
    vglSwapBuffers(GL_FALSE);
}

void gfx_begin_2d(void) {
    glViewport(0, 0, SCREEN_W, SCREEN_H);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_W, SCREEN_H, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void gfx_set_color(unsigned int c) {
    glColor4ub(c & 0xFF, (c >> 8) & 0xFF, (c >> 16) & 0xFF, (c >> 24) & 0xFF);
}

void gfx_rect(float x, float y, float w, float h, unsigned int color) {
    gfx_rect_vgradient(x, y, w, h, color, color);
}

void gfx_rect_vgradient(float x, float y, float w, float h, unsigned int top, unsigned int bottom) {
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLE_STRIP);
    gfx_set_color(top);
    glVertex3f(x, y, 0);
    glVertex3f(x + w, y, 0);
    gfx_set_color(bottom);
    glVertex3f(x, y + h, 0);
    glVertex3f(x + w, y + h, 0);
    glEnd();
}

void gfx_circle(float x, float y, float radius, unsigned int color) {
    int segments = radius < 16 ? 16 : 40;
    glDisable(GL_TEXTURE_2D);
    gfx_set_color(color);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y, 0);
    for (int i = 0; i <= segments; ++i) {
        float a = (float)i / segments * 2.0f * (float)M_PI;
        glVertex3f(x + cosf(a) * radius, y + sinf(a) * radius, 0);
    }
    glEnd();
}
