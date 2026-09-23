#ifndef GPS_EXPLORE_MAP_H
#define GPS_EXPLORE_MAP_H

#define MAP_ZOOM_MIN 3
#define MAP_ZOOM_MAX 18
#define MAP_ZOOM_DEFAULT 17

#define MAP_PITCH_2D 89.5f   /* vue de dessus */
#define MAP_PITCH_3D 50.0f   /* inclinaison par défaut de la vue 3D */
#define MAP_PITCH_MIN 25.0f

typedef struct {
    int x, y, w, h;          /* panneau à l'écran (pixels, origine en haut à gauche) */
    double lat, lon;
    float accuracy;          /* mètres ; <= 0 : pas de cercle */
    int zoom;
    float heading;           /* degrés, 0 = nord, sens horaire : direction de la caméra */
    float pitch;             /* degrés au-dessus de l'horizon ; MAP_PITCH_2D = vue de dessus */
    int show_arrow;          /* flèche de cap (en mouvement) plutôt qu'un point */
    float arrow_heading;     /* degrés, cap de déplacement */
} MapView;

/* Charge le module HTTP et démarre le téléchargement des tuiles en arrière-plan.
 * Le réseau (sceNetInit / sceNetCtlInit) doit déjà être initialisé. */
int map_init(void);

/* Dessine la carte (vitaGL). Ne bloque jamais : les tuiles absentes arrivent plus tard. */
void map_draw(const MapView *view);

/* Panneau vide de même taille, affiché tant qu'il n'y a pas de fix. */
void map_draw_placeholder(int x, int y, int w, int h, const char *text);

void map_fini(void);

#endif
