#ifndef GPS_ASSIST_H
#define GPS_ASSIST_H

/* Journalisation fournie par l'appelant (même format que log_event). */
typedef void (*gps_assist_log_fn)(const char *fmt, ...);

/* Charge gps_assist.skprx et interroge la puce (commandes GET seulement). */
int gps_assist_start(gps_assist_log_fn log);
/* Injecte l'heure système (TimeSet, ioctl 0x5B001001). */
int gps_assist_inject_time(void);
/* Injecte GPS_ASSIST_XTRA_PATH par blocs (SET_XTRA, ioctl 0x5B001000). */
int gps_assist_inject_xtra(void);
/* Décharge le module noyau (le crochet disparaît). */
void gps_assist_stop(void);

#endif
