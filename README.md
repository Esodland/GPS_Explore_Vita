# Go!Explore 2.0 (PS Vita Homebrew)

> ⚠️ **Statut du projet : Développement Actif** ⚠️
> *Ce projet est toujours en cours de développement. L'interface et la logique de l'application fonctionnent (Preuve de concept de spoofing validée), mais l'accès aux véritables données de la puce GPS matérielle reste à accomplir.*

Ce dépôt contient le code source de l'application homebrew **GPS_Explore** (Go!Explore 2.0) pour PlayStation Vita, développée comme un démonstrateur technique pour accéder au module `SceLocation` de la console.

## État de la Recherche (Août 2026)

Malgré une implémentation logicielle complète, l'application se heurte à des blocages au niveau du noyau de la console. L'API `sceLocationOpen` refuse de s'ouvrir pour les applications Homebrew (Fake-Signed SELF).

Voici les codes d'erreur documentés de nos recherches :

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

## Phase 2 : Rétro-ingénierie et Déblocage Matériel (En cours)

L'objectif ultime reste d'activer la *vraie* puce GPS matérielle. En décryptant (`FAGDec`) et décompilant (`vitadecompiler-mod`) le module `liblocation.suprx`, nous avons pu identifier la cause exacte du blocage (`0x80101244`) :

1. **Vérification de Privilège** : Dans l'API `sceLocationOpen` (NID `0xDD271661`), le système vérifie une variable globale de privilège située dans le segment de données (Segment 1) à l'offset `0x30`.
2. **Le Blocage Homebrew** : Pour les Homebrews (Fake-Signed), l'OS initialise cette variable à `0`, ce qui bloque tout accès matériel. Le système exige une valeur de `2`.
3. **Piste de Solution (Le Patch RAM)** : Il devrait être possible de développer un patch pour injecter la valeur `2` (`taiInjectData(modid, 1, 0x30, &val, 4)`) directement dans la mémoire de `liblocation` juste avant d'appeler l'API officielle. 

Tant que ce patch mémoire n'est pas développé et validé avec succès sur du vrai matériel (sans spoofing), le projet n'est pas considéré comme terminé. 

Ce dépôt sert de base de départ. L'interface graphique avec `vita2d` est pleinement fonctionnelle (comme prouvé par notre spoofer) et servira de réceptacle aux vraies données GPS lorsque le patch de déblocage matériel sera opérationnel !
