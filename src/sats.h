#ifndef GPS_EXPLORE_SATS_H
#define GPS_EXPLORE_SATS_H

/*
 * Moniteur de satellites. L'API SceLocation ne donne pas d'informations sur les
 * satellites ; on lit donc les trames NMEA que SceShell écrit dans son journal
 * interne (ux0:liblocation/liblocation_*.log). Ce journal doit être activé avec la
 * clé de registre /CONFIG/LOCATION/enable_log (catégorie NMEA = 4), puis un
 * redémarrage de la console.
 */

#define SATS_MAX 32

typedef struct {
    int available;          /* au moins une trame NMEA lue */
    int has_gsv;            /* la puce publie des trames GSV (liste des satellites visibles) */
    int driver_sats;        /* "Satellite Num" du pilote Sony (scePosGpsGetData), -1 si inconnu */
    int in_view;            /* satellites visibles (GSV, toutes constellations) */
    int tracked;            /* satellites avec un signal (SNR > 0) */
    int used;               /* satellites utilisés pour le fix (GGA) */
    int fix_quality;        /* GGA : 0 = pas de fix, 1 = GPS, 2 = DGPS */
    int fix_type;           /* GSA : 1 = pas de fix, 2 = 2D, 3 = 3D */
    int count;              /* nombre d'entrées dans snr[] */
    int snr[SATS_MAX];      /* dB-Hz, 0 si non capté */
    char last_time[16];     /* horodatage de la dernière trame (heure du journal) */
} SatInfo;

int sats_start(void);
void sats_stop(void);
void sats_get(SatInfo *out);

#endif
