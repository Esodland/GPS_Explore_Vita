# Reverse Engineering du Module SceLocation (PS Vita)

> Dernière vérification matérielle (12/09/2026, reprise Codex) : `0x80024302` est une **réponse serveur**, avec transport IPMI réussi. Le client dispose encore de 121 Mio USER, 112 Mio CDRAM et 26 Mio PHYCONT. L'attribution antérieure à un budget homebrew épuisé est une hypothèse invalidée comme conclusion établie. Voir `diagnostics/README.md` et les journaux associés. La sonde IPMI a été suivie d'une erreur console ; le plugin original a été restauré et son fonctionnement revérifié.

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

## PERCÉE #4 — 12/09/2026 : `0x80024302` localisé côté serveur

Une reprise instrumentée a mesuré les trois pools exposés au client avant et après l'activation, chaque `sceLocationOpen`, le `close(NONE)` et l'initialisation vita2d :

- avant vita2d : USER `126877696` octets, CDRAM `117440512`, PHYCONT `27262976` ;
- aucune variation visible autour de `open(NONE)` ou des échecs des méthodes 1–5 ;
- après vita2d + police : USER `116391936`, CDRAM `93323264`, PHYCONT inchangé.

Une sonde expérimentale a ensuite intercepté `sub_81001F54` (register normal, méthode IPMI `0x12340000`) et la fonction virtuelle de transport à `vtable+0x14`. Résultat vérifié dans le journal :

```
NONE       : transport=0x00000000, serveur=0x00000000
GPS 5      : transport=0x00000000, serveur=0x80024302
GPS+WiFi 2 : transport=0x00000000, serveur=0x80024302
AGPS 1     : transport=0x00000000, serveur=0x80024302
WiFi 3     : transport=0x00000000, serveur=0x80024302
3G 4       : transport=0x00000000, serveur=0x80024302
```

Le transport IPMI fonctionne donc et le serveur renvoie lui-même `SCE_KERNEL_ERROR_NO_FREE_PHYSICAL_PAGE`. Cela invalide comme conclusion établie l'hypothèse « budget physique du processus homebrew épuisé » ; les données actuelles ne désignent ni le pool, ni la taille, ni les contraintes de l'allocation du serveur.

La sonde a été suivie d'un message système au rallumage (« Erreur survenue au cours de la dernière utilisation »). Le plugin original sauvegardé a été restauré sur `ur0:tai/gps_activate.suprx`, relu par FTP et vérifié par SHA-256. Un relancement avec ce plugin a terminé normalement et laissé le FTP accessible. La console conserve l'eboot de diagnostic mémoire et le plugin d'activation original ; la sonde IPMI n'est pas active.

Artefacts : `diagnostics/2026-09-12-memory.txt`, `diagnostics/2026-09-12-ipmi.txt`, `diagnostics/2026-09-12-restored.txt`, `diagnostics/README.md`. La prochaine cible est le processus/module serveur qui traite `SceLocationLoc`, puis l'allocation fautive dans le chemin register `0x12340000`.

### 8. Modules `vsh/shell` déchiffrés (13/09/2026) : `liblocation_provider` écarté, cible reportée sur `shell.self`

Deux modules chargés par `shell.self` (le processus `*main`/SceShell, distinct des 4 modules `liblocation*` déjà décompilés) ont été identifiés via FTP (`vs0:vsh/shell/`) puis déchiffrés sur console avec **FAGDec** (`ux0:app/VDEC00001`, build personnalisé ciblant `vs0:/vsh/shell/liblocation_provider.suprx` dans `main.c`) : `liblocation_provider.suprx.elf` (619 Ko) et `liblocation_permission.suprx.elf` (35 Ko), intégrité vérifiée par SHA-256 (`diagnostics/server-modules/`).

- **`liblocation_permission.suprx`** confirme la piste `app.db` : il exporte `SceLibLocationPermission`, exécute lui-même les requêtes SQL sur `ur0:shell/db/app.db` (`SELECT val FROM tbl_appinfo WHERE titleId=? AND key=?`) et contient la chaîne `SceLocationPerm` — cohérent avec la clé `2840145610` déjà utilisée pour lever `0x80101249`. Aucune piste nouvelle ici, mais confirmation directe du mécanisme.
- **`liblocation_provider.suprx`** n'est **pas** le serveur IPMI bas niveau recherché : c'est le **moteur de positionnement Skyhook WPS** (chaînes `WPS.API.*`, `WPS.SPI.PspGPSAdapter`, `WPS.SPI.PspWifiAdapter`, cache de scan Wi-Fi/cellulaire, historique de fix GPS/`HistoryScanManager`). Il calcule/fusionne une position à partir de données déjà acquises ; aucune chaîne `PHYSICAL`, `NO_FREE`, allocation mémoire ou dispatch IPMI n'y figure. Cette piste est écartée pour `0x80024302`.
- **Conséquence :** le code qui enregistre le serveur IPMI `"SceLocationLoc"` et effectue l'allocation mémoire physique fautive n'est dans aucun des modules `.suprx` chargés en tant que bibliothèques — il est très probablement dans **`shell.self`** lui-même (3,1 Mo, l'exécutable principal de SceShell qui charge `liblocation_provider`/`liblocation_permission`). Prochaine cible de déchiffrement : `vs0:/vsh/shell/shell.self`.
- Limite d'outillage : pas de `strings`/`objdump`/`readelf`/Python disponibles dans cette session pour une désassemblage fin ; l'analyse s'est faite par extraction de chaînes imprimables (`tr -c '[:print:]' '\n' | grep`). Les tables d'imports (NID) ne sont pas résolues en noms de fonctions ici, contrairement aux 4 modules `liblocation*` d'origine (dont la résolution avait été faite via un outil externe non retrouvé dans ce dépôt).

### 9. `shell.self` déchiffré : le serveur `SceLocationLoc` trouvé, cible réduite à `sceGpsOpen()` (13/09/2026)

`vs0:/vsh/shell/shell.self` (6,7 Mo décompressé, `diagnostics/server-modules/shell.self.elf`, SHA-256 vérifié) contient bien le serveur cherché.

- La chaîne **`SceLocationLoc`** y figure directement, entourée de `SceLocationSession`, **`SceLocationServer`**, `SceHeadingServer` — confirmation que `shell.self` héberge le process qui enregistre ce nom IPMI.
- Classes/threads identifiés par les chaînes : `PositionManager` (façade `GetLocation`/`GetQueryLocation`/`CancelGetLocation`…), `ClientManager` (gestion des clients IPMI, `SceLocationClientList`), `DriverController` (`SetPowerStatus`, `[driver_man] changing gps power %d`).
- **Trois machines à états en cascade pour l'ouverture GPS**, dans l'ordre logique d'un `sceLocationOpen(méthode réelle)` :
  1. `GpsCtrlStateMachine` : `GPS_CTRL_STATE_CLOSED → OPENED → START_REQ → STARTED`
  2. `FlyingStartStateMachine` : `FLYING_START_STATE_IDLE → OPEN_REQUESTED → START_REQUESTED → STARTED` (log `FlyingStartGpsStartResult failed to start. retrying`)
  3. `ProviderGpsStateMachine` : `PROVIDER_GPS_STATE_NO_DEVICE → WAIT_OPEN → WAIT_START → REQ_START → …`
- **Appel bas niveau identifié : `sceGpsOpen()`** (module d'import `SceGps`, distinct de l'API publique `sceLocationOpen`) — logué directement `[GPS] sceGpsOpen()=0x%08x` / `[COM] sceGpsOpen()=0x%08x`, avec les frères `sceGpsClose/sceGpsStart/sceGpsStop/sceGpsIoctl(RESET|SET_XTRA|TimeSet)`. C'est le candidat le plus probable pour être **l'appel qui retourne `0x80024302`** : NONE ne l'invoque jamais (pas de vrai chip à ouvrir), les méthodes 1–5 oui.
- Le module qui exporte `SceGps` reste à localiser : absent de `os0:kd/` (aucun fichier nommé gps) et absent des chaînes `shell.self` sous forme `SceGpsForDriver`/`SceGpsForKernel` — probablement hébergé dans un module déjà listé sous un autre nom (`syscon.skprx` gère l'alimentation carte/GPS-3G sur les autres plateformes Sony, `lowio.skprx` regroupe des bus bas niveau divers) ou exposé uniquement en mémoire sans nom de module séparé.
- Cache XTRA/AGPS confirmé : `ux0:liblocation/xtra.bin`, `http://xtra{1,2,3}.gpsonextra.net/xtra.bin`, `ur0:/liblocation/gps/` — explique le contenu du dossier `ur0:liblocation` trouvé par ailleurs (cache de données d'assistance AGPS/Wi-Fi, pas lié directement à `0x80024302`).
- Aucune chaîne littérale `PHYSICAL`/`NO_FREE`/`8002` dans `shell.self` (attendu : erreur numérique, pas formatée en texte dédié).

**Prochaine étape concrète :** localiser, parmi les modules déjà présents dans `os0:kd/` (ou un module non encore inventorié), celui qui exporte le NID de `sceGpsOpen`/`SceGps` — nécessite soit une désassemblage plus poussé de `shell.self` (table d'imports/NID, outillage absent de cette session), soit un dump ciblé des modules `os0:kd` candidats (`syscon.skprx`, `lowio.skprx`) via FAGDec pour y chercher la même chaîne `SceGps`.

### 10. `SceGps` localisé dans `bootimage.skprx` — mur du format conteneur (13/09/2026)

`syscon.skprx` et `lowio.skprx` déchiffrés et grepés : **aucune trace de `Gps`**, pistes écartées.

Les 44 modules de `os0:kd/` ont ensuite été déchiffrés en bloc (incident FTP au passage : voir `diagnostics/README.md`, sans rapport avec la sonde IPMI du 12/09 — surcharge réseau, pas un crash noyau). Résultat :

- **`bootimage.skprx`** (3,1 Mo) contient le **manifeste complet de tous les modules kernel de la console**, bien au-delà des ~40 fichiers présents individuellement sur `os0:kd/` — dont `os0:kd/gps.skprx`, `os0:kd/bbmc.skprx`, `os0:kd/wlanbt.skprx`, `os0:kd/power.skprx`, `os0:kd/motion.skprx`, etc. Ces modules ne sont **pas des fichiers persistants séparés** : ils sont empaquetés/compressés à l'intérieur de `bootimage.skprx` et décompressés en RAM au démarrage — d'où leur absence de tout listing FTP classique de `os0:kd/`.
- Symboles confirmés dans `bootimage.skprx` : **`SceGps`, `SceGpsForDriver`, `SceGpsApi`, `SceGpsData`, `SceGpsd`** — c'est bien `gps.skprx` (via son export kernel `SceGpsForDriver`) qui héberge `sceGpsOpen`.
- **`bbmc.skprx`** (BaseBand Modem Controller) apparaît étroitement lié : symbole **`SceBbmcGpsInfLock`** (verrou dédié à l'info GPS partagée avec le modem), entouré de chaînes **`QCOM high speed protocol`**, `OSBL`, `SwdlPort*`, `PARTITION` — confirme que la puce GPS/3G est un **modem Qualcomm** (GPS intégré au chipset baseband), avec un protocole de téléchargement de firmware Qualcomm classique au boot.
- **`sysmem.skprx`** (gestionnaire kernel de mémoire physique) ne contient aucune chaîne nommée pour une partition GPS/BBMC — les tailles de partitions sont probablement dans une table numérique, pas des chaînes lisibles. Piste fermée pour l'approche par extraction de chaînes.

**Mur atteint :** extraire le binaire réel de `gps.skprx`/`bbmc.skprx` depuis `bootimage.skprx` nécessite de comprendre son format conteneur (table d'offsets/tailles/compression par module embarqué) — non documenté dans ce dépôt, et cette session ne dispose d'aucun désassembleur ARM (`objdump`/`readelf`/IDA/Ghidra absents) pour aller plus loin même une fois le binaire extrait. La suite demande soit un outil d'extraction de `bootimage` (à écrire ou trouver dans la communauté homebrew Vita), soit un environnement d'analyse binaire complet.

### 11. Mur franchi : extraction locale + désassemblage → appel exact identifié (13/09/2026)

Contrairement à la conclusion précédente, le mur d'outillage a été franchi **sans aucune manipulation de la console** :

- **`bootimage_extract`** (TeamFAPS/PSVita-RE-tools, par CelesteBlue/zecoxao) extrait proprement chaque module embarqué depuis `bootimage.skprx.elf` (celui déjà déchiffré par FAGDec). `gps.elf` (19 384 octets) et `bbmc.elf` (354 368 octets) obtenus en clair localement.
- Python + `pyelftools` + **`capstone`** (installés via pip, `python` fonctionne — seul l'alias `python3` est capté par le Store Windows) ont permis de parser la table d'imports SCE (`SceModuleInfo`/`SceLibStubTable`, structures documentées dans `sce_module_info.h` du même outil) et de désassembler le code ARM Thumb-2 de `gps.skprx`.
- Les 7 NID importés de `SceSysmemForDriver` par `gps.skprx` ont été résolus via `Vita3K/Vita3K` (`vita3k/nids/include/nids/nids.inc`, base de NID complète y compris kernel/`ForDriver`, absente des headers publics vitasdk) :

  | NID | Fonction |
  |---|---|
  | `0x9328E0E8` | `ksceKernelCreateHeap` |
  | `0x7B4CB60A` | `ksceKernelAllocHeapMemory` |
  | `0x3EBCE343` | `ksceKernelFreeHeapMemory` |
  | `0xD6437637` | `ksceKernelDeleteHeap` |
  | `0x6D88EF8A` | `ksceKernelMemcpyKernelToUser` |
  | `0xBC996A7A` | `ksceKernelMemcpyUserToKernel` |
  | `0x45D22597` | `kscePUIDtoGUID` |

- **Appel identifié et désassemblé** (début du code de `gps.skprx`, probablement le chemin d'init appelé par `sceGpsOpen()`) :
  ```c
  ksceKernelCreateMutex(...);                      // si < 0, saut d'erreur
  ksceKernelCreateHeap("SceGps", 0x2000, NULL);     // 8 Kio — nom de tas confirmé en mémoire à 0x81004000
  ```
  Arguments désassemblés : `r0 = &"SceGps"` (chaîne confirmée en clair dans le binaire), `r1 = 0x2000` (taille), `r2 = 0` (opt = NULL).

**Interprétation :** `ksceKernelCreateHeap` puise dans le pool de pages physiques **réservé au kernel**, distinct des pools USER/CDRAM/PHYCONT visibles côté client (ceux-là restent intacts d'après le diagnostic du 12/09). Une requête de 8 Kio qui échoue en `NO_FREE_PHYSICAL_PAGE` n'est pas un problème de taille — c'est le signe que ce pool kernel est épuisé ou fragmenté **au niveau système**, indépendamment du process appelant. Ceci concorde avec :
- Photos échoue aussi en extérieur à ciel dégagé (donc pas un problème de signal ni de privilège applicatif) ;
- le 3G fonctionne normalement (son init a lieu au boot, avant toute pression mémoire éventuelle liée aux plugins taiHEN chargés ensuite) ;
- l'échec de `sceLocationOpen` sur méthode réelle est immédiat et déterministe, jamais lié à l'environnement.

**Hypothèse de travail retenue :** pression sur le pool de mémoire kernel réservé, potentiellement liée à l'accumulation des plugins kernel taiHEN chargés au démarrage (`storagemgr`, `nonpdrm`, `0syscall6`, `reAuth`, `vitacheat360`, `vitabright`, `itls`, `vitabridge`, `psmove_bt`, `gps_activate`, etc. — voir `plugin/device_config.txt`), plutôt qu'un problème spécifique au GPS ou à un manque de privilège homebrew.

**Prochaine étape concrète :** mesurer l'état réel du pool mémoire kernel (pas seulement USER/CDRAM/PHYCONT côté client) — soit via un export kernel de statistiques mémoire (`ksceKernelGetMemBlockInfo`/équivalent, à identifier), soit empiriquement en testant `sceLocationOpen` sur une méthode réelle juste après un redémarrage propre avec un `config.txt` réduit au strict minimum (sans les plugins non essentiels au projet), pour voir si `0x80024302` disparaît.

### 12. Essai de `config.txt` minimal (13/09/2026) : abandonné après incidents en cascade, baseline restaurée

Tentative de tester l'hypothèse « pression mémoire kernel liée aux plugins taiHEN » en réduisant `config.txt` au strict minimum. Trois incidents en cascade, tous récupérés :

1. **Première réduction trop agressive** : a retiré par erreur les entrées `*NPXS10015`/`*NPXS10016` (`ur0:tai/henkaku.suprx`, spoof de version) — cassait l'apparence d'Enso (signalé immédiatement par l'utilisateur). Restauré depuis la sauvegarde + reboot.
2. **Deuxième tentative, recherche préalable faite** sur chaque plugin (`storagemgr` = monte `ux0:`, essentiel, conservé sur alerte de l'utilisateur ; `nonpdrm`/`0syscall6`/`reAuth`/`vitacheat360`/`vitabright`/`itls`/`vbanim`/`InfiniteNet`/`vdb_daemon`/`vitabridge` = retirés après vérification publique de leur rôle). Après reboot avec ce config réduit : régression complète vers `0x80101249`/`0x80101286` (nouveau code jamais vu) sur toutes les méthodes, y compris NONE (qui réussissait toujours jusqu'ici).
3. **Restauration du `config.txt` complet + reboot : la régression persiste.** Diagnostic : la clé de permission `app.db` (`2840145610`) pour `GPSX00001` avait disparu (probablement un re-scan de SceShell au boot, indépendant de nos plugins). Réinsérée directement en base (édition SQLite locale + upload FTP du fichier `app.db`) — la clé était bien présente après upload, mais **l'appli a alors disparu de la liste dans Réglages**. Vérification : `PRAGMA integrity_check` = `ok`, mode journal = `delete` (pas de désynchronisation WAL possible), 29 lignes cohérentes pour `GPSX00001` — le fichier n'était pas corrompu, seul le cache mémoire du shell était désynchronisé. Un redémarrage a résolu l'affichage réel (comportement API) : retour exact à la baseline (`NONE` → `0x0`/handle `0x80000000`, méthodes réelles → `0x80024302`). L'appli reste cependant absente de la liste affichée dans Réglages (cosmétique, sans effet observé sur le comportement de l'API).

**Leçon retenue :** `app.db` ne doit plus être édité par écriture directe du fichier pendant que le shell tourne — même avec un fichier SQLite valide, le cache vivant de SceShell peut se désynchroniser visuellement. Et `config.txt` ne doit être modifié qu'un plugin à la fois avec un reboot de vérification entre chaque retrait, jamais en lot, même après recherche préalable — le rôle documenté publiquement d'un plugin ne garantit pas l'absence de dépendance cachée avec notre propre séquence d'activation.

**Décision :** essai suspendu pour cette session. La baseline (`0x80024302` sur méthodes réelles) est confirmée stable et reproductible. La piste « pression mémoire kernel » reste ouverte mais nécessite un protocole plus prudent (retrait d'un seul plugin à la fois, vérification systématique après chaque reboot) avant d'être retentée.

### 13. Vraie source de `0x80024302` : échec du chargement de `liblocation_provider.suprx` dans SceShell (23/09/2026)

Analyse statique uniquement (aucune manipulation console). Elle **invalide l'hypothèse de la section 11** (tas kernel `SceGps` épuisé) :

- **`gps.skprx` n'alloue rien à l'ouverture.** `_sceGpsOpen` (syscall `SceGps`, NID `0x6EAC2C6D`, `0x81002DC4`) vérifie seulement l'appelant (`ksceSblACMgrIsSystemProgram`, sinon QAF/PostSs/`kscePmMgrGetProductMode`), convertit l'UID du callback (`kscePUIDtoGUID`), contrôle l'état et stocke le callback. Toutes ses erreurs sont en `0x8039xxxx`, et la constante `0x80024302` n'apparaît pas dans le module. Le `ksceKernelCreateHeap("SceGps", 0x2000)` de la section 11 appartient à l'init du module (au boot), pas à `sceGpsOpen`.
- **Chemin serveur dans `shell.self`** : la fonction `0x8118BECE` charge dynamiquement le moteur de positionnement dans le processus SceShell :
  ```c
  if (!provider_loaded && (methodMask & 0xF | otherMask & 7))   // NONE → masque 0 → jamais chargé
      uid = sceKernelLoadStartModule("vs0:vsh/shell/liblocation_provider.suprx", ...);
      if (uid < 0) return uid;                                     // code brut propagé au client IPMI
  ```
  Ses trois appelants (`0x8118C5D0`, `0x8118C822`, `0x8118CB7E`) propagent ce retour. Cela reproduit exactement l'observation : NONE réussit, et **les cinq méthodes réelles, Wi-Fi et 3G compris, échouent avec le même code** avant tout accès à la puce GPS.
- `liblocation_provider.suprx` occupe environ **533 Kio** en mémoire (segments `0x84260` + `0x1514`), auxquels s'ajoutent ses tas et threads au démarrage. `SCE_KERNEL_ERROR_NO_FREE_PHYSICAL_PAGE` est le code attendu quand le budget mémoire du **processus SceShell** ne peut plus fournir ces pages. Ce budget est partagé avec les plugins taiHEN `*main` : `vdb_daemon`, `henkaku.suprx`, `shellbat`, `custom_warning`, `vitacompanion`, `vitabridge`.
- Cohérence : Photos échoue aussi (même serveur, même SceShell), et les pools USER/CDRAM/PHYCONT de l'application restent stables (ce n'est pas son budget).

**Journal intégré découvert :** la clé de registre **`/CONFIG/LOCATION/enable_log`** (int, lue par `sceRegMgrGetKeyInt` à l'init du serveur `0x811892E0`) active l'écriture de `ux0:liblocation/liblocation_AAAAMMJJ_hhmmss_mmm.log` (catégories `[skyhook] [driver_if] [nmea] [location] [ext] [etc]`). Le bit 4 de la valeur est masqué ; `1` suffit. Le serveur l'active aussi à chaud via une commande interne (`0x8118E0B4`). Cette clé est utile pour confirmer le point d'échec et suivre la suite (`sceGpsOpen`, XTRA, NMEA).

**Solution proposée :** libérer de la mémoire dans SceShell pour que le provider se charge. On retire les plugins `*main` non essentiels **un seul à la fois**, avec un reboot et un test après chaque retrait (leçon de la section 12). Ordre : `vdb_daemon` (démon VitaDB), puis `vitabridge` sous `*main` (on garde l'entrée `*VBRG00001`). `henkaku.suprx` et `vitacompanion` sont conservés. Critère de succès : `sceLocationOpen(méthode 5)` ne renvoie plus `0x80024302`.

## PERCÉE #5 — 23/09/2026 : `0x80024302` levé en désactivant `vdb_daemon` dans `*main`

Test matériel sur la PCH-1100, avec une seule modification : `ux0:data/VitaDB/vdb_daemon.suprx` commenté sous `*main` dans `ur0:tai/config.txt`. Ancien fichier sauvegardé dans `build/diagnostic-backup/2026-09-23/config.txt`, SHA-256 `3f5d1558…`. Le nouveau fichier a été relu à l'identique après téléversement. Reboot, puis clé app.db `2840145610=1` vérifiée avant et après.

Résultat (`diagnostics/2026-09-23-no-vdb-daemon.txt`) : **NONE, 5, 2, 1, 3 et 4 renvoient tous `sceLocationOpen = 0x00000000`**, avec le handle `0x80000000`. Les pools client sont inchangés. Cela confirme la section 13 : le démon VitaDB occupait assez de mémoire dans SceShell pour empêcher le chargement de `liblocation_provider.suprx` (≈533 Kio).

**Configuration requise :** ne pas charger `vdb_daemon` sous `*main`. Prochaine étape : un fix réel en extérieur via le flux interactif (CROIX), puis `GetLocation`.

## PERCÉE #6 — 23/09/2026 : PREMIER FIX GPS RÉEL 🛰️

Flux interactif sur GPSX00001 (méthode 5 = GPS), sur la même configuration sans `vdb_daemon` :

```
sceLocationOpen(5)        = 0x00000000  handle 0x80000000
sceLocationConfirm        = 0x00000000
ConfirmGetResult          = 0x00000000, résultat 2 (SCE_LOCATION_DIALOG_RESULT_ENABLE)
sceLocationGetLocation    = 0x00000000  (Frame 120 → 540, position réelle)
```

- Coordonnées réelles, cohérentes avec l'emplacement physique de la console et variant de quelques mètres entre deux lectures (bruit GPS normal). Ce ne sont pas les coordonnées simulées de la Tour Eiffel, et aucun plugin spoofer n'est chargé dans `config.txt`. Le journal brut est conservé hors Git (`build/diagnostic-backup/2026-09-23/gps_log_first_fix.txt`) parce qu'il contient une position personnelle.
- Effets de bord côté système, signes que la pile Sony tourne entièrement : `ur0:liblocation/gps/xtra.bin` a été téléchargé (assistance XTRA/AGPS, 41 Ko, 18:44) et le moteur Skyhook a écrit `ur0:liblocation/.wps/Sony.preferences` (18:43).
- `sceLocationGetLocation` **bloque** jusqu'au premier fix. L'interface de l'appli est figée pendant ce temps. Prévoir `sceLocationGetLocationWithTimeout` ou un thread dédié.

**Recette complète pour activer le GPS sur PCH-1100 (HENkaku/Enso) :**
1. Plugin `gps_activate.suprx` sous `*GPSX00001` (flags `seg1+0x30=2`, `seg1+0x34=1`, puis `SceLibLocationInternal_3500A98C(1,0x10000)`).
2. Clé app.db `2840145610` autorisée pour le titleId (initialement `1` ; le 24/09, la valeur `0x80000003` a aussi permis l'ouverture et le consentement). Ne pas modifier le fichier SQLite pendant que SceShell tourne.
3. Mémoire SceShell suffisante pour charger `liblocation_provider.suprx` (≈533 Kio) : pas de `vdb_daemon` sous `*main`.
4. Accepter le dialogue de consentement, puis `GetLocation`.

## 24/09/2026 — Succès intermittents, puis zéro satellite : diagnostic GPS isolé

Le journal complet `ux0:/data/gps_log.txt` a été récupéré sans publier les coordonnées personnelles. Il contient deux premières sessions avec **263 puis 519 retours `sceLocationGetLocation=0`**, tous proches du même emplacement réel (à moins d'un kilomètre). La deuxième session relève des précisions variables d'environ 9 à 85 m. **L'utilisateur est rentré à l'intérieur juste après le 519e succès** : les quatre `0x80101200` qui terminent cette session correspondent donc à une perte de signal attendue. Le fait inexpliqué est que **dès le lancement suivant, de nouveau dehors**, les 42 appels ont tous retourné `0x80101200`, puis que les autres sessions n'ont pas retrouvé de position. Le succès initial était prolongé et cohérent spatialement ; son origine satellite exacte n'a pas été enregistrée à ce moment-là.

L'échec de **réacquisition au lancement suivant dehors précède vitaGL** : le journal Sony capturé vers 21:41 heure locale le 23/09 indique déjà 107 fois `Satellite Num [0]` et 107 trames GGA avec zéro satellite utilisé. Le commit de passage à vitaGL est daté de 23:08. Revenir à vita2d n'a pas rétabli la position, ce qui concorde avec cette chronologie. Le changement d'environnement intérieur explique la perte en cours de session, mais pas l'échec persistant dehors après relancement. **Seule l'application a été fermée et rouverte ; la console n'a pas été redémarrée.**

La version à thread GPS introduite avant la perte du fix avait un risque de fermeture : `stop_location_worker` attendait cinq secondes après `sceLocationCancelGetLocation`, puis le code fermait le handle même si le thread n'avait pas terminé. Ce n'est pas une cause démontrée de l'échec de réacquisition, faute de journal de fermeture de cette session. Le code local a été corrigé le 24/09 pour journaliser le résultat de l'attente et ne fermer/décharger SceLocation que si le thread est effectivement arrêté ; ce nouveau build n'a pas été déployé après l'incident de la sonde minimale. Un essai discriminant futur doit conserver **la même session ouverte** en passant de dehors à dedans puis de nouveau dehors : si le fix revient, examiner spécifiquement la fermeture/réouverture ; s'il ne revient pas, examiner d'abord la réception et l'état du pilote.

Le 24/09, une sonde dédiée sans moteur graphique, HTTP ou balayage des méthodes a ouvert uniquement GPS 5. `Open=0`, consentement accepté, puis **23 appels** `GetLocation=0x80101200`. Elle a relevé **317 observations NMEA** avec zéro satellite utilisé et zéro satellite signalé par le pilote, y compris durant environ trois minutes après que la console a été placée dehors. La pile GPS produisait toujours des trames : le problème observable est l'absence de satellites reçus ou déclarés, pas un échec d'ouverture de SceLocation.

Vers 23:00, la console a cessé de répondre ; au redémarrage, elle a affiché un message d'erreur de dernière utilisation. Batterie ensuite à 49 %, fonctionnement normal. La cause de cet incident n'est pas établie ; aucun dump nouveau daté de cet essai n'a été trouvé. L'eboot original a été immédiatement restauré et vérifié par SHA-256. La version suivante de la sonde, qui empêche la veille, a été compilée mais **pas déployée** après l'incident. Détails, limites et sauvegardes : `diagnostics/minimal-gps-protocol.md`.

**Suite prudente :** analyser le dernier journal Sony hors ligne lorsqu'il peut être récupéré sans perturber la console, réduire la verbosité de `enable_log=0xFF`, puis comparer un essai GPS pur avec une méthode assistée. Ne conclure ni à une panne matérielle ni à une cause vitaGL sur les seules données actuelles.
