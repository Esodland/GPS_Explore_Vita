# Go!Explore 2.0 (PS Vita Homebrew)

> ⚠️ **Statut : recherche active — chaîne d'autorisation ENTIÈREMENT débloquée, dernier obstacle = mémoire physique** ⚠️
> Un homebrew fake-signed est passé de « `sceLocationOpen` bloqué net » à **pleinement autorisé, dialogue système de consentement accepté, `open`+`confirm` réussis**. Il reste une seule barrière technique (allocation de mémoire physique à l'ouverture d'une vraie méthode GPS), qui n'est **ni** un blocage privilège **ni** noyau.

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
- Méthode 0 (NONE) réussit (n'alloue rien) ; les vraies méthodes doivent allouer de la **mémoire physique** (buffers IPMI / mapping du buffer partagé GPS) que le **budget du processus homebrew** n'a pas.
- Cause de fond : en forçant l'init dans NOTRE processus (au lieu du serveur système), le stack client de localisation utilise le budget homebrew (limité). L'alloc qui échoue est interne à `SceIpmi`.

C'est un problème d'**ingénierie mémoire**, pas d'autorisation/privilège/noyau.

### Progression complète des erreurs
`0x8010124F` (gate client) → *flags + init `3500A98C`* → `0x80101249` (appli non autorisée) → *`app.db` clé `2840145610`* → `0x80024302 / NO_FREE_PHYSICAL_PAGE` (budget mémoire physique).

### Prochaines pistes (mémoire)
1. Instrumenter la mémoire libre au moment de l'`open` (main / PHYCONT / CDRAM) pour identifier le pool épuisé.
2. Augmenter le budget mémoire physique du processus (attributs `param.sfo`, notamment PHYCONT).
3. Réduire les allocations du processus avant l'`open`.

---

## Contenu du dépôt

- `src/main.c` — l'application (vita2d), ouvre en priorité une vraie méthode GPS (5/2/1/3/4).
- `plugin/` — plugins TaiHEN de recherche : `probe.c` (sonde mémoire), `factory_test.c` (test ACL serveur), `init_test.c` / `unlock_test.c` (déblocage session), **`gps_activate.c`** (plugin d'activation propre : flags + init), et le `gps_spoofer` d'origine. `device_config.txt` = config taiHEN de la console.
- `liblocation*.suprx.elf.*` — modules décryptés + décompilés.
- `SceLocation_Reverse_Engineering.md` — analyse complète et historique daté des percées.

## Déploiement / test à distance

Console de dev accessible via **vitacompanion** (FTP `1337`, commandes `1338` : `launch`/`destroy`/`reboot`). Voir `deploy.ps1`. L'activation se charge via `ur0:tai/config.txt` (`*GPSX00001` → `ur0:tai/gps_activate.suprx`) ; l'autorisation appli via la clé `app.db` ci-dessus.

> Note : le message d'erreur affiché par l'appli (« Vérifiez vos paramètres… ») est **codé en dur dans le homebrew** (`src/main.c`), ce n'est pas un message système.
