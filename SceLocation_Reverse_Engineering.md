# Reverse Engineering du Module SceLocation (PS Vita)

Ce document résume l'analyse des modules de localisation de la PlayStation Vita, extraits et décompilés afin de comprendre les blocages liés à l'utilisation du GPS (comme l'erreur `0x80101244` sur les Homebrews).

## 1. Modules analysés

Les modules suivants ont été décryptés à l'aide de **FAGDec** depuis le système de la console (`vs0:`/`ur0:`), puis décompilés en pseudo-C via **vitadecompiler-mod** :

- `liblocation.suprx` : L'API publique, utilisée par les jeux et applications standard.
- `liblocation_internal.suprx` : Le module interne gérant la communication bas niveau et les callbacks.
- `liblocation_factory.suprx` : Outils d'usine et de debug.

*(Note: `liblocation_extension.suprx` n'a pas pu être décompilé correctement, probablement en raison de sa petite taille ou d'une structure ELF non standard générée par FAGDec).*

## 2. Découvertes initiales

### A. L'API Publique (`liblocation.suprx`)
Le fichier `liblocation.suprx.elf.c` contient l'ensemble des fonctions accessibles à l'utilisateur (module export `SceLibLocation`). 
Puisque le SDK officiel (`vitasdk/vita-headers`) ne documente pas complètement ces NIDs (les noms de fonctions sont sous la forme `SceLibLocation_XXXXXXXX`), un travail de correspondance (mapping) sera nécessaire pour identifier avec certitude :
- `sceLocationOpen`
- `sceLocationGetLocation`
- `sceLocationConfirm`

**Points d'intérêt identifiés dans le code :**
- Des appels à `SceLocationCallback` et `SceLocationHead`.
- L'utilisation intensive du noyau via `SceLibKernel`.
- L'enregistrement du client via `SceLocationClient` (lié à `SceSysmem`).

### B. Le cœur du système (`liblocation_internal.suprx`)
Ce module est le véritable moteur de la localisation. Il exporte `SceLibLocationInternal`.
Il contient des routines de gestion de la mémoire système et interagit avec le matériel (puce GPS/3G).
- Initialise des tâches avec le nom interne `SceLocationCallback`.
- Gère le client interne `SceLocationClient`.
- C'est probablement ici que se trouve la vérification de sécurité qui bloque les applications de type `Fake-Signed SELF` (Homebrews) et qui renvoie l'erreur `0x80101244`.

### C. Le mode Usine/Debug (`liblocation_factory.suprx`)
Ce module exporte `SceLibLocationFactory` et se connecte à un client spécial nommé `SceLocationDebugClient`. Il pourrait permettre d'injecter de fausses coordonnées sans avoir à hooker l'application finale, mais il est fort probable qu'il nécessite des privilèges de type `factory` ou `dex` (DevKit).

## 3. Objectifs pour le projet GPS_Explore

D'après le `README.md` et les logs du projet, l'API `sceLocationOpen` bloque l'accès au GPS. En décompilant, nous avons identifié le problème :

1. **Le NID de `sceLocationOpen`** : L'API est exportée sous le NID brut `0xDD271661` (`SceLibLocation_DD271661`).
2. **La cause du blocage (`0x80101244`)** : Dans `SceLibLocation_DD271661`, le système vérifie le niveau de privilège de l'application via une variable globale située dans le segment de données (Segment 1) à l'offset `0x30`.
   - Si cette valeur est `0`, toute tentative de démarrer le module ou d'utiliser le GPS renverra une erreur (dont `0x8010124F` dans les appels asynchrones).
   - Pour les Homebrews (Fake-Signed), le gestionnaire d'applications initialise cette variable à `0`.
   - Le système exige une valeur de `2` pour un accès complet.

### La Solution : Patch Mémoire avec TaiHEN
Pour que l'application Homebrew puisse utiliser la VRAIE puce matérielle (au lieu de simuler des fausses coordonnées), il suffit de créer un plugin TaiHEN qui :
1. Hook `sceLocationOpen` (NID `0xDD271661`).
2. Lorsqu'il est appelé, récupère l'identifiant du module `SceLibLocation`.
3. Écrit la valeur `2` à l'aide de `taiInjectData(modid, 1, 0x30, &val, 4)`.
4. Appelle la vraie fonction `sceLocationOpen`.

Avec ce patch, toutes les sécurités internes de `SceLocation` considèrent l'application comme ayant les privilèges maximums, ce qui débloque la puce 3G/GPS et la récupération des vraies données.

---
*Ce document a été mis à jour le 30 juillet 2026 suite à la découverte du patch mémoire.*
