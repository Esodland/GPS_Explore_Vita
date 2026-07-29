# Go!Explore 2.0 (PS Vita Homebrew)

> 🟢 **Statut du projet : Succès / Preuve de concept validée** 🟢
> *Le projet a abouti. Les limitations matérielles et logicielles ont été identifiées et contournées avec succès.*

Ce dépôt contient le code source de l'application homebrew **GPS_Explore** (Go!Explore 2.0) pour PlayStation Vita, développée comme un démonstrateur technique pour accéder au module `SceLocation` de la console.

## État de la Recherche (Août 2026)

Après s'être heurtée à des blocages au niveau du noyau de la console, la recherche a finalement abouti. L'API `sceLocationOpen` refusait initialement de s'ouvrir pour les applications Homebrew (Fake-Signed SELF).

Voici les codes d'erreur qui documentaient nos recherches initiales :

*   **`0x8010124F`** : Retourné systématiquement par `sceLocationOpen` pour les méthodes Wi-Fi (`SCE_LOCATION_LMETHOD_WIFI`) et mixtes (AGPS/3G).
    *   *Cause :* Service distant injoignable ou bloqué.
*   **`0x80101244`** : Retourné par `sceLocationOpen` pour la méthode GPS seul (`SCE_LOCATION_LMETHOD_GPS`).
    *   *Cause :* L'OS bloque l'accès matériel direct à la puce GPS en raison d'un manque de privilèges de l'exécutable.

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

## Phase 2 : Rétro-ingénierie et Déblocage Matériel (Victoire !)

L'objectif ultime d'activer la *vraie* puce GPS matérielle a été atteint ! En décryptant (`FAGDec`) et décompilant (`vitadecompiler-mod`) le module `liblocation.suprx`, la cause exacte du blocage (`0x80101244`) a été identifiée :

1. **Vérification de Privilège** : Dans l'API `sceLocationOpen` (NID `0xDD271661`), le système vérifie une variable globale de privilège située dans le segment de données (Segment 1) à l'offset `0x30`.
2. **Le Blocage Homebrew** : Pour les Homebrews (Fake-Signed), le gestionnaire d'applications initialise cette variable à `0`, ce qui bloque tout accès matériel. Le système exige une valeur de `2`.
3. **Le Patch RAM (TaiHEN)** : Le plugin a été adapté pour injecter la valeur `2` (`taiInjectData(modid, 1, 0x30, &val, 4)`) directement dans la mémoire de `liblocation` juste avant d'appeler l'API officielle. 

Grâce à ce patch mémoire, les sécurités internes de `SceLocation` considèrent l'application comme ayant les privilèges maximums, débloquant l'utilisation de la puce 3G/GPS et la récupération des vraies données.

Ce dépôt sert maintenant de preuve de concept complète. L'interface graphique avec `vita2d` est pleinement fonctionnelle et reçoit désormais les véritables données GPS de la console !
