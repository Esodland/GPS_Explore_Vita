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

> ⚠️ **ATTENTION — cette « Solution » est INCORRECTE / INSUFFISANTE.** Voir la mise à jour du 11 septembre 2026 ci-dessous, qui la corrige sur la base du code décompilé relu ligne par ligne **et d'une vérification sur le vrai matériel (PCH-1100 3G)**.

---
*Ce document a été mis à jour le 30 juillet 2026 suite à la découverte du patch mémoire.*

---

## MISE À JOUR — 11 septembre 2026 (relecture du décompilé + vérif matériel)

Analyse refaite en lisant réellement `liblocation.suprx.elf.c` et `liblocation_internal.suprx.elf.c`, puis **confirmée sur la console 3G** via une sonde TaiHEN (`plugin/probe.c` → `ux0:data/gps_probe.txt`).

### 1. Le bon code d'erreur : `0x8010124F` (pas `0x80101244`)
La boucle de l'appli (`src/main.c`) teste les méthodes `i = 0..10`. En recoupant avec `gps_log.txt` :
- **méthodes 0–5 (valides)** → **`0x8010124F`** ← *le vrai mur*
- **méthodes 6–10 (hors plage)** → `0x80101244` (simple « méthode invalide », `loc_810000A8`)

Toute la narration précédente autour de `0x80101244` visait donc le mauvais code.

### 2. Le global de privilège est réel : `0x81004030` = **segment 1 + 0x30**
En-têtes ELF : seg0 (texte) `0x81000000`, seg1 (données RW) `0x81004000` (FileSiz `0x1C`, MemSiz `0x40`). Le global est donc en `.bss` (défaut `0`).
- `sub_81001186` (chemin normal de `sceLocationOpen`) : **`if (*0x81004030 == 0) return 0x8010124F;`** ← origine exacte de l'erreur de l'appli.
- `sub_81001C7E` (chemin bit `0x100` de la méthode, non emprunté par l'appli) : succès **seulement si `== 2`**.
- Donc injecter `2` satisfait les deux vérifications : le patch visait bien la bonne variable.

### 3. Pourquoi `taiInjectData(2)` seul NE SUFFIT PAS
Juste après le guard, le code déréférence un **pointeur de contexte en seg1+0x00 (`0x81004000`)** (champs `+0x38` client IPMI, `+0x5C` refcount, `+0x60`, `+0x6C`, `+0x70`…). Pour un homebrew ce pointeur est **`NULL`** → injecter le privilège **fait passer le guard puis crashe** sur le NULL-deref.

### 4. L'init privilégié et son absence d'appelant
`SceLibLocation_7F74AA94(ctx)` = setter : `contexte = ctx; global = 2`.
Il est appelé par `SceLibLocationInternal_3500A98C` (crée le contexte + client IPMI, puis appelle le setter). **`3500A98C` n'a AUCUN appelant** dans les 4 modules location décompilés : c'est un export invoqué par une entité **externe** (serveur IPMI de localisation / daemon système) lors d'un handshake privilégié. Dans le processus d'un homebrew non privilégié, il **n'est jamais invoqué** → contexte NULL, global 0.

### 5. Vérification matérielle (sonde, console PCH-1100 3G)
```
module 'SceLibLocation' uid=0x4001011B
seg[0] text vaddr=0x81088A50 memsz=0x33D0            (= statique ✓)
seg[1] data vaddr=0x8108CC70 memsz=0x40 filesz=0x1C  (= statique ✓)
context (seg1+0x00) = 0x00000000  (NULL)   ← confirmé
priv    (seg1+0x30) = 0x00000000  (0)      ← confirmé  → 0x8010124F
```

### 6. Conclusion
Le déblocage **user-mode de `liblocation` est un cul-de-sac** : le contexte n'existe pas, et l'enforcement réel est **côté serveur/noyau via IPMI**, fonction de l'authid/des capabilities du **processus**. La vraie voie (Phase 3) passe par le **serveur IPMI de localisation** (qui pilote la puce et applique l'ACL), pas par un patch de la variable locale.

*Artefacts : `plugin/probe.c`, log `ux0:data/gps_probe.txt`.*

### 7. Cible Phase 3 identifiée : le serveur IPMI **`"SceLocationLoc"`** (12/09/2026)
Analyse des 4 modules décryptés (y compris `liblocation_extension` désassemblé à la main : simple wrapper de l'API publique, n'importe pas `SceLibLocationInternal`, n'appelle pas l'init).

- Le client IPMI est créé par **`liblocation.suprx` elle-même** (`liblocation_internal` importe `SceIpmi` mais ne l'appelle jamais) via `SceLibKernel_C458D60A(..., "SceLocationLoc", 0x10, ...)` puis `SceIpmi_4E255C31`.
- `liblocation_factory.suprx` (export unique `SceLibLocationFactory_9E020010`) crée **son propre** client IPMI vers **le même serveur `"SceLocationLoc"`** (heap `SceLocationDebugClient`), **sans jamais toucher le global seg1+0x30**. C'est un chemin distinct à tester, mais il vise probablement le même serveur et subira la même ACL.
- **Cible à trouver/décrypter :** le **processus système qui enregistre le serveur IPMI `"SceLocationLoc"`** (celui qui pilote la puce 3G/GPS et applique l'ACL par authid/capabilities). Méthode : décrypter les modules service système et `grep "SceLocationLoc"` (côté serveur = enregistrement du serveur, pas simple client).

### 8. Tests matériels de la piste B « factory » + init direct (12/09/2026)
Deux plugins de test exécutés sur la console 3G (logs `ux0:data/gps_factory.txt`, `gps_init.txt`).

**B.1 — Appel de l'export factory `SceLibLocationFactory_9E020010` :**
- `sceKernelLoadStartModule("vs0:sys/external/liblocation_factory.suprx")` **réussit** depuis le homebrew.
- La table `tbb` de l'export : `a1` = commande, valides = {4,5,13,14,16,19} (sinon `0x8010124B`) ; toutes passent par `sub_8100007A` (connexion IPMI).
- Résultat (pour les 6 commandes) : **`SceIpmi_4E255C31` → `0xC4` (handle valide)**, **`SceIpmi_B282B430` (transport) → `0x0` (OK)**, mais **retour serveur = `0x8010124B`**, buffers de sortie vides.
- **Conclusion B.1 :** la **connexion IPMI à `SceLocationLoc` est AUTORISÉE** pour un homebrew (l'ACL de connexion n'est PAS le mur). Mais le serveur répond **`0x8010124B` (« opération non supportée » — code utilisé uniquement côté factory, jamais dans l'API normale)** : **les méthodes factory/debug sont désactivées sur firmware retail.** Piste factory = cul-de-sac pour lire la position.

**B.2 — Appel direct de l'init `SceLibLocationInternal_3500A98C(a1∈[1,8], 0x10000)` :**
- `taiGetModuleInfo("SceLibLocationInternal")` **échoue** → **`liblocation_internal` n'est PAS chargé dans le processus de l'appli** (seul `SceLibLocation` l'est ; confirmé par l'énumération des modules).
- Charger une 2e copie puis appeler son init → `0x8010124D` (échec à une étape d'init réelle).
- **Conclusion B.2 :** le **moteur `liblocation_internal` + l'init `3500A98C` tournent dans le processus SERVEUR**, pas dans le client. On ne peut pas exécuter l'init côté client.

### 9. Architecture consolidée & pistes restantes
- **Client (appli)** : charge seulement `liblocation` (API). Elle possède un **gate local `global==0`** qui l'empêche même de se connecter sur le chemin normal.
- **Serveur** (processus qui enregistre `SceLocationLoc`, avec `liblocation_internal` + moteur) : pilote la puce, applique l'ACL. **Accepte la connexion transport** d'un homebrew, mais refuse les méthodes factory.

Pistes restantes (toutes plus lourdes) :
1. **Répliquer le protocole IPMI NORMAL** (méthodes réelles de `liblocation.suprx` : ids + structs des messages passés à `B282B430`) par-dessus une connexion à `SceLocationLoc` — la transport est ouverte ; reste à voir si le serveur sert les méthodes *normales* à un process non privilégié (peut renvoyer une erreur de privilège distincte).
2. **Patcher le serveur / le noyau** : trouver le module qui enregistre `SceLocationLoc`, localiser son ACL, la contourner (plugin kernel).

## PERCÉE — 12/09/2026 : déblocage de la session client (piste 1)

Séquence d'activation trouvée par RE puis **validée sur matériel** (`plugin/unlock_test.c`, log `ux0:data/gps_unlock.txt`) :

1. **Deux flags dans les données de `liblocation` (segment 1)** gouvernent l'init :
   - `seg1+0x30` (u32) : global de privilège (`0`→ `0x8010124F`). Setter : `sub_81000932`(=1), `7F74AA94`/`822BBF06`(=2).
   - `seg1+0x34` (u8) : lu par l'export **`SceLibLocation_5C7185D2()`** (setter, = 1) et testé par `SceLibLocation_2311B24A()` ; s'il vaut `0`, l'init `3500A98C` échoue à `0x8010124D`.
2. **Séquence qui marche** (test matériel) :
   ```
   écrire seg1+0x30 = 2 ; seg1+0x34 = 1
   charger liblocation_internal ; résoudre l'export SceLibLocationInternal_3500A98C
   appeler 3500A98C(1, 0x10000)   -> retour 0x0 (SUCCÈS)
   => contexte (seg1+0x00) passe de NULL à un objet valide : SESSION CLIENT ÉTABLIE
   ```
   Résolution d'un export non documenté = walk de la table d'exports via `taiGetModuleInfo` (voir `resolve_export` dans `unlock_test.c`).

3. **Conséquence sur `sceLocationOpen`** : l'erreur passe de `0x8010124F` (mur client) à **`0x80101249`** (méthodes 1–5). Ce code est **la réponse du SERVEUR** : `sub_81001324` (cœur de l'open) appelle `sub_81001F54` (invocation IPMI *register* vers `SceLocationLoc`), et **teste `== 0x80101249`** → marque `contexte+0x58 = 1`.
   - `0x80101249` ≠ `0x8010124B` (« non supporté » factory) ≠ `0x8010124F` (mur client). Il appartient à la **famille dialogue/consentement** (`0x80101246/47/48`, tests de flags `0x200`/`0x80` = résultats de dialogue dans `sub_81001324`).
   - **Interprétation :** on atteint le vrai service de localisation avec le protocole normal ; il exige un **consentement utilisateur** (dialogue de confirmation) ou la localisation est **désactivée dans les Réglages**. Ce n'est PAS un blocage privilège/noyau.

**Bilan piste 1 :** le blocage *client-side* est **levé**. Reste l'étape de **consentement/confirmation** (flux normal `open → sceLocationConfirm (dialogue) → getLocation`), à faire tourner dans l'appli réelle (qui possède déjà ce flux) — et/ou vérifier le réglage de localisation de la console.

### Prochaine étape
Transformer `unlock_test` en **plugin d'activation propre** (écrit les flags + appelle `3500A98C` au chargement de LOCATION, avant l'open de l'appli), retirer le spoofer, et lancer le vrai flux `open → confirm → getLocation` avec l'UI (l'utilisateur accepte le dialogue, ou injection via vita-bridge). Vérifier aussi le réglage « données de localisation » dans les Réglages système.

## PERCÉE #2 — 12/09/2026 : autorisation de l'appli (piste A) + étape dialogue

Après le déblocage de session, `sceLocationOpen` renvoyait `0x80101249` = **l'appli n'est pas autorisée** dans la liste « Données géographiques » (le serveur `SceLocationLoc` refuse le register). Le `param.sfo` ne gouverne PAS ça (prouvé : `ecolibrium`, le vrai jeu GPS, est un jeu normal `ATTRIBUTE=2`).

**Trouvé dans `ur0:shell/db/app.db`** (table `tbl_appinfo`, store clé-valeur par appli) : la clé **`2840145610`** marque la capacité localisation. Distribution : présente sur ~10 applis dont **`PCSF00092` (Ecolibrium, jeu GPS) = `1`**, `NPXS10004` (Photos) = `3`, `NPXS10000` (near) = `0x80000003` ; **absente sur `GPSX00001`**.

**Action validée sur matériel :**
`INSERT OR REPLACE INTO tbl_appinfo(titleId,key,val) VALUES('GPSX00001',2840145610,1)` → upload `app.db` + reboot.
Résultat : **`0x80101249` disparaît** ; `sceLocationOpen` renvoie désormais **`0x80024302`** (domaine `0x8002` = dialogue/système, externe aux modules location).

**Interprétation :** l'appli est maintenant **autorisée** ; `open` atteint l'**étape du dialogue de confirmation**. Le `0x80024302` vient de ce que le plugin appelle `open` trop tôt (hook au chargement du module, sous-système dialogue pas prêt). Le vrai flux doit tourner dans le contexte runtime de l'appli.

**Progression des erreurs (chaque palier franchi) :**
`0x8010124F` (gate client) → *flags+init* → `0x80101249` (appli non autorisée) → *app.db clé 2840145610* → `0x80024302` (dialogue de confirmation).

### Prochaine étape (#2)
Plugin = activation seule (flags + `3500A98C` au chargement LOCATION) ; retirer le spoofer ; l'appli exécute `open → sceLocationConfirm (dialogue) → getLocation` après son init complète, l'utilisateur accepte le dialogue.

## PERCÉE #3 — 12/09/2026 : autorisation complète + dialogue ACCEPTÉ (piste 1 aboutie côté auth)

Avec (a) l'activation (flags + `3500A98C`, plugin `gps_activate`) et (b) l'autorisation `app.db` (clé `2840145610=1` pour GPSX00001), l'appli exécute enfin le vrai flux :

```
sceLocationOpen(method=0 NONE, hmethod=0) -> 0x00000000   (handle 0x80000000)
sceLocationConfirm(handle)                -> 0x00000000
  => DIALOGUE SYSTÈME AFFICHÉ : « Autoriser cette application à utiliser
     les données géographiques ? » -> UTILISATEUR ACCEPTE
sceLocationGetLocation(handle)            -> 0x80101244  (méthode NONE = aucune donnée)
```

**Toute la chaîne d'autorisation homebrew est donc franchie** : gate client → autorisation appli → consentement utilisateur. Le dialogue système s'affiche et est accepté normalement.

### Dernier mur : mémoire physique à l'ouverture d'une VRAIE méthode GPS
En ouvrant avec une vraie méthode (1=AGPS+3G+WIFI, 2=GPS+WIFI, 5=GPS…) au lieu de 0=NONE :
```
sceLocationOpen(method=5 GPS) -> 0x80024302 = SCE_KERNEL_ERROR_NO_FREE_PHYSICAL_PAGE
```
- Se produit **même après reboot propre** et **avant l'init de vita2d** → ce n'est pas l'UI ni de l'accumulation.
- La méthode 0 (NONE) réussit (n'alloue rien) ; les vraies méthodes doivent allouer de la **mémoire physique** (buffers IPMI/moteur) que le **budget du processus homebrew** n'a pas.
- Cause de fond : en forçant l'init `3500A98C` dans NOTRE processus, le stack client de localisation s'exécute côté client (budget homebrew limité) au lieu du serveur système. L'alloc qui échoue est interne à `SceIpmi` (création client / buffers) — non localisable sans le module serveur.

**Progression complète des erreurs :**
`0x8010124F` (gate client) → *flags+init* → `0x80101249` (appli non autorisée) → *app.db 2840145610=1* → `0x80024302 / NO_FREE_PHYSICAL_PAGE` (budget mémoire physique).

**Note :** le texte à l'écran « Vérifiez vos paramètres… même Photos… » est **codé en dur dans le homebrew** (`src/main.c`), pas un message système.

### Pistes pour le dernier mur (mémoire)
1. Augmenter le budget mémoire physique du processus homebrew (attributs param.sfo / budget mémoire), en particulier PHYCONT.
2. Instrumenter la mémoire libre au moment de l'open (sceKernelGetFreeMemorySize / budget) pour identifier le pool épuisé.
3. Réduire les autres allocations du processus avant l'open.
