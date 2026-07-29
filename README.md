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

## Victoire : Le Plugin Spoofer (TaiHEN)

Pour prouver que l'application `GPS_Explore` était parfaitement codée et capable de traiter les données, nous avons développé un **Plugin TaiHEN en mode utilisateur (`gps_spoofer.suprx`)**.

Ce plugin intercepte les appels réseau et matériels du module `SceLocation` :
1.  **`sceLocationOpen`** : Hooké pour renvoyer un faux jeton d'accès (`0x1337`) et contourner l'erreur `0x80101244`.
2.  **`sceLocationGetLocation`** : Hooké pour injecter des coordonnées GPS falsifiées (celles de la Tour Eiffel à 18 km/h).

**Résultat :** L'application a instantanément traité les données falsifiées, validant complètement l'architecture logicielle de notre Homebrew. Le code source du plugin se trouve dans le dossier `plugin/` de ce dépôt.

## Phase 2 : Rétro-ingénierie du Kernel (En cours)

L'objectif ultime reste d'activer la *vraie* puce GPS matérielle (si elle est encore fonctionnelle malgré l'absence de réseau Data).
Pour faire tomber le blocage matériel du noyau :
1.  **Décryptage** : Utilisation de `FAGDec` sur la console pour décrypter le module système `vs0:sys/external/liblocation.suprx` en un fichier `.elf` analysable.
2.  **Analyse (Ghidra/IDA)** : Trouver l'instruction assembleur ARM précise qui rejette l'autorisation (probablement via un check de flag FSELF).
3.  **Patch RAM** : Développer un plugin Kernel `.skprx` pour patcher dynamiquement l'instruction et forcer l'OS à allumer la puce GPS pour les Homebrews.

Ce dépôt sert de base de départ. L'interface graphique avec `vita2d` est pleinement fonctionnelle et servira de réceptacle aux données GPS lorsque le plugin Kernel sera prêt !
