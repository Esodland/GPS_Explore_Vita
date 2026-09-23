# Go!Explore 2.0 (PS Vita Homebrew)

> **Statut : autorisation débloquée ; les méthodes GPS échouent encore — appel fautif identifié.** Le 13/09/2026, `gps.skprx` a été extrait de `bootimage.skprx` et désassemblé (outillage installé localement : `pyelftools` + `capstone`). L'appel exact a été localisé : `ksceKernelCreateHeap("SceGps", 0x2000, NULL)` — une requête de seulement 8 Kio sur le pool de mémoire physique **réservé au kernel** (distinct des pools USER/CDRAM/PHYCONT du client). Un test terrain (Photos échoue aussi en extérieur à ciel dégagé) confirme qu'il s'agit d'un problème **systémique**, pas d'un défaut de privilège homebrew. Un essai de `config.txt` réduit pour tester la piste « pression mémoire kernel » a été suspendu après plusieurs incidents en cascade (tous récupérés) ; **baseline reconfirmée stable** (`NONE` réussit, méthodes réelles → `0x80024302`).

Application homebrew **GPS_Explore** (Go!Explore 2.0) pour PlayStation Vita : démonstrateur d'accès au module `SceLocation`. Cible matérielle : **PCH-1100 (OLED 3G)** — le seul modèle Vita avec une puce GPS.

Reverse engineering détaillé et vérifié sur matériel : voir **`SceLocation_Reverse_Engineering.md`**.

---

## Phase 1 — Architecture logicielle + spoofer (validé)

L'appli (`src/main.c`, interface `vita2d`) implémente le flux complet `open → confirm → getLocation`. Un **plugin TaiHEN spoofer** (`plugin/` → `gps_spoofer.suprx`) a prouvé que l'appli traite correctement les données en injectant de fausses coordonnées (Tour Eiffel) — validant toute l'architecture logicielle.

## Phase 2 — Reverse engineering vérifié sur matériel

Analyse ligne à ligne des 4 modules décryptés (`liblocation`, `liblocation_internal`, `liblocation_factory`, `liblocation_extension`) **confirmée sur la console 3G** via des sondes TaiHEN.

Correction des premières notes (qui étaient erronées) :
- Le vrai mur de l'appli est **`0x8010124F`** (pas `0x80101244`, qui n'est que « méthode invalide »).
- Il vient de `sub_81001186` : `if (*(seg1+0x30) == 0) return 0x8010124F;`. Le **global de privilège** est en **segment 1 + 0x30** (VADDR `0x81004030`), `0` pour un homebrew.
- Injecter `2` seul **ne suffit pas** : le code déréférence ensuite un **pointeur de contexte en seg1+0x00, `NULL`** chez un homebrew → crash.
- Le contexte est créé par l'init `SceLibLocationInternal_3500A98C`, normalement invoqué côté serveur — **jamais dans un processus homebrew**.

Sonde matérielle (`plugin/probe.c`, PCH-1100) : `context=0`, `priv=0`, layout des segments identique au statique. ✔

Le serveur de localisation est un **serveur IPMI nommé `"SceLocationLoc"`** (contacté par `liblocation.suprx` et `liblocation_factory.suprx`).
- Piste factory testée : la **connexion IPMI est acceptée** pour un homebrew (`SceIpmi_4E255C31`→handle, transport OK), mais les méthodes debug renvoient `0x8010124B` (« non supporté » sur retail) = cul-de-sac.
- **Fait clé : l'ACL de connexion n'est pas le mur.**

## Phase 3 — Déblocage réel (chaîne franchie)

Chaque palier vérifié sur la console 3G :

| Palier | Erreur | Solution appliquée |
|---|---|---|
| Gate client-side | `0x8010124F` | Écrire `seg1+0x30=2` + `seg1+0x34=1` puis appeler l'init `SceLibLocationInternal_3500A98C(1, 0x10000)` → **contexte/session créés** |
| Autorisation appli | `0x80101249` | Ajouter la clé **`2840145610=1`** pour `GPSX00001` dans `ur0:shell/db/app.db` (`tbl_appinfo`) |
| Consentement | — | Le **dialogue système** « Autoriser cette application à utiliser les données géographiques ? » s'affiche et est **accepté** ✅ |
| `open(NONE)` + `confirm` | `0x0` / `0x0` | — |

### Détail des découvertes clés
- **Séquence d'activation** (plugin `plugin/gps_activate.c`) : deux flags dans les données de `liblocation` gouvernent l'init — `seg1+0x30` (privilège, setter interne) et `seg1+0x34` (flag lu par l'export `SceLibLocation_2311B24A`, posé par `SceLibLocation_5C7185D2`). Écrits + init `3500A98C` → contexte créé (retour `0x0`).
- **Autorisation par appli** : elle n'est **pas** dans le `param.sfo` (prouvé : `ecolibrium`, le vrai jeu GPS, est un jeu normal `ATTRIBUTE=2`). Elle est dans `app.db`, clé `2840145610` (Ecolibrium `PCSF00092`=1, Photos=3, near=`0x80000003`). L'ajout à `GPSX00001` élimine `0x80101249`.
- **Résolution d'exports non documentés** : walk de la table d'exports via `taiGetModuleInfo` (voir `resolve_export`).

### Dernier obstacle : mémoire physique
Ouvrir une **vraie** méthode GPS (1=AGPS+3G+WIFI, 2=GPS+WIFI, 5=GPS…) au lieu de 0=NONE :

```
sceLocationOpen(method=5 GPS) -> 0x80024302 = SCE_KERNEL_ERROR_NO_FREE_PHYSICAL_PAGE
```

- Se produit **même après reboot propre** et **avant l'init de vita2d** → ni l'UI ni de l'accumulation.
- Méthode 0 (NONE) réussit ; les méthodes 1–5 échouent. Aucune baisse nette des pools client n'est mesurée autour de ces appels.
- Mesures client avant vita2d : **121 Mio USER, 112 Mio CDRAM, 26 Mio PHYCONT**, disponibles avant et après chaque ouverture.
- La trace de la commande register `0x12340000` distingue **transport = 0**, **réponse serveur = 0x80024302**. L'attribution antérieure à un budget homebrew insuffisant ou à une allocation locale interne à SceIpmi n'était pas démontrée.

Le code désigne une erreur de mémoire physique, mais ne révèle pas encore le pool, la taille ni les contraintes de l'allocation côté serveur. Journaux et limites : **`diagnostics/README.md`**. La sonde IPMI expérimentale a été suivie d'une erreur console ; le plugin précédent a été restauré et vérifié.

### Progression complète des erreurs
`0x8010124F` (gate client) → *flags + init `3500A98C`* → `0x80101249` (appli non autorisée) → *`app.db` clé `2840145610`* → `0x80024302 / NO_FREE_PHYSICAL_PAGE` (réponse du serveur ; ressource exacte inconnue).

### Piste `vsh/shell` (13/09/2026) : serveur trouvé dans `shell.self`, cible = `sceGpsOpen()`

`liblocation_provider.suprx`, `liblocation_permission.suprx` et `shell.self` (chargé par le process `*main`/SceShell) ont été déchiffrés via **FAGDec** et analysés (`diagnostics/server-modules/`) :
- `liblocation_permission.suprx` confirme le mécanisme `app.db`/`tbl_appinfo`/clé `SceLocationPerm` déjà exploité — rien de nouveau.
- `liblocation_provider.suprx` est le **moteur de positionnement Skyhook WPS** (fusion GPS/Wi-Fi/cellulaire), pas le serveur bas niveau — piste écartée.
- **`shell.self`** contient la chaîne `SceLocationLoc` et le vrai serveur : classes `PositionManager`/`ClientManager`/`DriverController`, trois machines à états en cascade pour l'ouverture GPS (`GpsCtrlStateMachine` → `FlyingStartStateMachine` → `ProviderGpsStateMachine`), et l'appel bas niveau **`sceGpsOpen()`** (module d'import `SceGps`, distinct de l'API publique) — candidat le plus probable pour l'origine de `0x80024302`.
- Le module qui exporte `SceGps` n'est pas encore localisé (absent de `os0:kd/` sous ce nom).

### Piste `bootimage.skprx` (13/09/2026) : appel fautif identifié par désassemblage

`syscon.skprx`/`lowio.skprx` écartés (aucune trace `Gps`). Les 44 modules de `os0:kd/` déchiffrés en bloc révèlent que `bootimage.skprx` embarque `gps.skprx`, `bbmc.skprx`, `wlanbt.skprx`… compressés, absents de tout listing `os0:kd/` classique.

**Extraction et désassemblage réussis (sans manipulation console)** : `bootimage_extract` (TeamFAPS/PSVita-RE-tools) a extrait `gps.elf`/`bbmc.elf` en clair depuis le `bootimage.skprx.elf` déjà déchiffré. `pyelftools` + `capstone` (installés via `pip`) ont permis de parser la table d'imports SCE et de désassembler le code ARM Thumb-2. Les NID résolus via `Vita3K/Vita3K` (base NID incluant les exports kernel `ForDriver`) révèlent l'appel exact :

```c
ksceKernelCreateMutex(...);
ksceKernelCreateHeap("SceGps", 0x2000, NULL);   // 8 Kio — nom confirmé en clair dans le binaire
```

**Avant ça**, un test terrain a changé l'hypothèse de travail : **Photos échoue aussi** à obtenir un fix GPS en extérieur à ciel dégagé (le 3G fonctionne, donc pas de panne totale de la puce Qualcomm). Combiné à la découverte que l'appel fautif ne demande que 8 Kio sur le **pool mémoire réservé au kernel** (pas les pools USER/CDRAM/PHYCONT du client), l'hypothèse retenue est une **pression systémique sur la mémoire kernel** — possiblement liée à l'accumulation des plugins taiHEN — plutôt qu'un problème de privilège spécifique à notre homebrew.

### Essai `config.txt` minimal (13/09/2026) : suspendu après incidents, baseline reconfirmée

Tentative de retirer les plugins taiHEN non essentiels (après recherche publique sur chacun) pour tester la piste « pression mémoire kernel ». Trois incidents en cascade, tous récupérés :  spoof de version Enso cassé par une première réduction trop large ; régression complète (`0x80101249`/nouveau code `0x80101286`) après un retrait plus prudent — cause : la clé `app.db` de permission avait disparu (re-scan shell au boot) ; sa réinsertion directe a fait disparaître l'appli de la liste Réglages (cache shell désynchronisé, fichier non corrompu — `PRAGMA integrity_check` = `ok`). Un redémarrage a rétabli le comportement API exact de la baseline. Détails complets : `SceLocation_Reverse_Engineering.md` §12.

**Leçon :** ne plus éditer `app.db` par écriture directe pendant que le shell tourne ; ne retirer qu'un seul plugin de `config.txt` à la fois, avec vérification après chaque reboot.

### Prochaines pistes
1. Mesurer l'état réel du pool mémoire kernel (au-delà des chiffres USER/CDRAM/PHYCONT déjà obtenus côté client).
2. Retenter le test `config.txt` minimal, mais un seul plugin à la fois avec vérification systématique.
3. Adapter les ressources seulement après cette identification ; les mesures actuelles ne justifient pas une augmentation du budget du client.

---

## Contenu du dépôt

- `src/main.c` — l'application (vita2d), ouvre en priorité une vraie méthode GPS (5/2/1/3/4).
- `plugin/` — plugins TaiHEN de recherche : `probe.c` (sonde mémoire), `factory_test.c` (test ACL serveur), `init_test.c` / `unlock_test.c` (déblocage session), **`gps_activate.c`** (plugin d'activation propre : flags + init), et le `gps_spoofer` d'origine. `device_config.txt` = config taiHEN de la console.
- `liblocation*.suprx.elf.*` — modules décryptés + décompilés.
- `SceLocation_Reverse_Engineering.md` — analyse complète et historique daté des percées.
- `diagnostics/` — journaux matériels du 12/09/2026, distinction transport/réponse serveur et procédure de reprise sûre.

## Déploiement / test à distance

Console de dev accessible via **vitacompanion** (FTP `1337`, commandes `1338` : `launch`/`destroy`/`reboot`). Voir `deploy.ps1`. L'activation se charge via `ur0:tai/config.txt` (`*GPSX00001` → `ur0:tai/gps_activate.suprx`) ; l'autorisation appli via la clé `app.db` ci-dessus.

> Note : le message d'erreur affiché par l'appli (« Vérifiez vos paramètres… ») est **codé en dur dans le homebrew** (`src/main.c`), ce n'est pas un message système.
