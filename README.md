# Go!Explore 2.0 (PS Vita Homebrew)

> ⚠️ **Statut du projet : Développement Actif** ⚠️
> *Ce projet est toujours en cours de développement actif. Les travaux de recherche et d'ingénierie se poursuivent pour contourner les limitations matérielles et logicielles.*

Ce dépôt contient le code source de l'application homebrew **GPS_Explore** (Go!Explore 2.0) pour PlayStation Vita, développée comme un démonstrateur technique pour accéder au module `SceLocation` de la console.

## État de la Recherche (Août 2026)

Malgré une implémentation logicielle complète, l'application se heurte à des blocages au niveau du noyau de la console. L'API `sceLocationOpen` refuse de s'ouvrir pour les applications Homebrew (Fake-Signed SELF).

Voici les codes d'erreur documentés de nos recherches :

*   **`0x8010124F`** : Retourné systématiquement par `sceLocationOpen` pour les méthodes Wi-Fi (`SCE_LOCATION_LMETHOD_WIFI`) et mixtes (AGPS/3G).
    *   *Cause probable :* Le service Skyhook (utilisé par Sony pour la géolocalisation Wi-Fi) est hors ligne/désactivé. Le noyau bloque l'accès car le service distant ne répondra pas, ou refuse catégoriquement l'autorisation réseau à l'exécutable non officiel.
*   **`0x80101244`** : Retourné par `sceLocationOpen` pour la méthode GPS seul (`SCE_LOCATION_LMETHOD_GPS`).
    *   *Cause probable :* Le noyau identifie l'exécutable comme un FSELF (Homebrew) et verrouille l'accès matériel direct à la puce GPS, ou la baseband 3G est considérée comme inactive (absence de carte SIM valide empêchant l'initialisation du composant).

L'application contourne l'erreur visuelle et tente un `sceLocationConfirm` forcé, qui retourne `0x80101241` (Invalid State), confirmant que l'initialisation du module GPS est strictement protégée.

## Découvertes sur les Permissions

Pour maximiser les chances d'accès, l'application utilise :
*   `ATTRIBUTE2=4112` et `ATTRIBUTE=1` dans le `param.sfo`.
*   Auth ID usurpé : `0x2800000000000018` (Celui utilisé par l'application système officielle Photos `NPXS10004`, au lieu de l'habituel `0x2800000000000001`).
*   Modification de la base `ur0:shell/db/app.db` (Table `tbl_appinfo`, champ de permission 3480795629 mis à 3 pour forcer l'autorisation système).
*   Chargement de `SceNetCtlInit` avant l'accès pour forcer l'activation de la couche réseau.

Rien de cela ne suffit à contourner le blocage du Kernel pour les exécutables FSELF. De plus, même l'application officielle Photos tourne dans le vide sans obtenir de *fix* GPS, ce qui laisse supposer que la couche logicielle GPS (A-GPS) est compromise si les éphémérides Sony ne sont plus joignables.

## Prochaine Étape : Plugin Kernel (TaiHEN)

Pour faire fonctionner le GPS sur les Homebrews, un **Plugin Kernel TaiHEN** est nécessaire. 

**Stratégie du Plugin :**
1.  **Hooks (Détournement) :** Accrocher les fonctions internes de `SceLocation` dans le noyau pour contourner la vérification de sécurité (probablement un appel à `sceKernelGetProcessInfo` ou `sceAppMgrGetInfo` qui vérifie le flag FSELF/Auth ID).
2.  **Mocking (Simulation) :** Si la puce physique refuse réellement de s'allumer, le plugin pourrait créer un faux périphérique GPS et injecter des coordonnées personnalisées dans `sceLocationGetLocation` pour tricher (Location Spoofer).
3.  **A-GPS Wi-Fi :** Une autre approche du plugin consisterait à accrocher `sceLocationGetLocation`, faire une requête Wi-Fi/IP en arrière-plan via une API moderne (ex: Google Geolocation API), et renvoyer les coordonnées à l'application.

Ce dépôt sert de base de départ. L'interface graphique avec `vita2d` est pleinement fonctionnelle et servira de réceptacle aux données GPS lorsque le plugin Kernel sera prêt !
