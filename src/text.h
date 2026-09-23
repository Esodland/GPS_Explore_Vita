#ifndef GPS_EXPLORE_TEXT_H
#define GPS_EXPLORE_TEXT_H

/* Texte avec la police système PGF, rastérisée à la demande dans un atlas GL.
 * Chaînes UTF-8 ; seuls l'ASCII et le Latin-1 (accents français) sont rendus. */
int text_init(void);
void text_fini(void);

/* (x, y) = début de la ligne de base, comme vita2d_pgf_draw_text. À appeler en mode 2D. */
void text_draw(float x, float y, unsigned int color, float scale, const char *str);
float text_width(float scale, const char *str);

#endif
