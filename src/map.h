#ifndef GPS_EXPLORE_MAP_H
#define GPS_EXPLORE_MAP_H

#include <vita2d.h>

#define MAP_ZOOM_MIN 3
#define MAP_ZOOM_MAX 18
#define MAP_ZOOM_DEFAULT 16

/* Charge le module HTTP et démarre le téléchargement des tuiles en arrière-plan.
 * Le réseau (sceNetInit / sceNetCtlInit) doit déjà être initialisé. */
int map_init(void);

/* Dessine la carte centrée sur (lat, lon) dans le rectangle donné. Appelé depuis
 * la boucle d'affichage : ne bloque jamais (les tuiles absentes arrivent plus tard).
 * accuracy <= 0 : pas de cercle de précision. */
void map_draw(vita2d_pgf *font, int x, int y, int w, int h,
              double lat, double lon, float accuracy, int zoom);

/* Panneau vide de même taille, affiché tant qu'il n'y a pas de fix. */
void map_draw_placeholder(vita2d_pgf *font, int x, int y, int w, int h, const char *text);

void map_fini(void);

#endif
