# Sonde GPS minimale — 24 septembre 2026

Objectif : distinguer une ouverture du service, une position retournée par l'API et un fix satellite confirmé, sans charger vita2d, vitaGL, carte, HTTP ou la sonde des six méthodes.

Le build `GPS_MINIMAL_DIAGNOSTIC=ON` utilise le même title ID `GPSX00001`, donc la chaîne d'autorisation existante (plugin `gps_activate.suprx`, clé `2840145610=1` dans `app.db`, `vdb_daemon` désactivé sous `*main`). Il ouvre **une seule fois** la méthode 5 (`GPS`), demande le consentement, appelle `sceLocationGetLocation` dans un thread, puis lit les nouvelles trames NMEA du journal Sony. Il n'écrit ni dans `app.db`, ni dans `config.txt`, ni dans le registre.

Build local :

```powershell
$env:VITASDK='C:/Users/Julien/PSVITA/vitasdk'
$env:PATH='C:\Users\Julien\PSVITA\tools\cmake\bin;C:\Users\Julien\PSVITA\tools;C:\Users\Julien\PSVITA\vitasdk\bin;'+$env:PATH
cmake -S . -B build/minimal -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/Users/Julien/PSVITA/vitasdk/share/vita.toolchain.cmake -DGPS_MINIMAL_DIAGNOSTIC=ON
cmake --build build/minimal --target eboot.bin
```

Artefact : `build/minimal/eboot.bin`. L'application est volontairement **sans interface graphique** : l'écran peut rester noir derrière le dialogue système. Elle maintient la console éveillée pendant le test. START termine l'essai ; sinon il s'arrête après dix minutes. Les résultats sont ajoutés à `ux0:/data/gps_minimal_diag.txt` avec date et heure locales. Chaque lancement est délimité par `BEGIN` et `END`. Ce fichier contient les coordonnées personnelles : le garder hors Git.

Avant tout déploiement, sauvegarder `ux0:/app/GPSX00001/eboot.bin` de la console, puis vérifier qu'on peut le restaurer. Ne téléverser que l'eboot de diagnostic ; conserver le `param.sfo`, le plugin et les permissions existants. Une seule connexion FTP à la fois. Après l'essai, restaurer l'eboot sauvegardé et vérifier son SHA-256 par relecture.

Lignes utiles dans le journal :

- `OPEN`, `CONFIRM_RESULT` : succès de l'autorisation et de l'ouverture GPS.
- `API` : retour de `sceLocationGetLocation`, durée d'attente, précision et heure d'acquisition de la position.
- `NMEA` : heure UTC de la trame GGA, qualité déclarée, satellites utilisés et compteur du pilote Sony. Seules les **nouvelles données écrites après le lancement** sont prises en compte ; une ancienne capture n'est pas considérée comme une mesure fraîche.
- `HEARTBEAT` : sonde encore active, même si l'API attend toujours.

Si aucune ligne `NMEA` n'apparaît, vérifier que le journal Sony `ux0:/liblocation/liblocation_*.log` est actif ; la sonde n'active pas elle-même `/CONFIG/LOCATION/enable_log`. Un retour API `0` sans satellites utilisés signifie « position disponible », pas « fix satellite confirmé ». Comparer plusieurs sessions après démarrage à froid et à chaud, au même endroit dégagé, avec et sans réseau si possible. Relever la durée avant le premier `API` réussi et avant le premier `NMEA sats_used>0`.

## Premier essai interrompu (24/09/2026)

L'ancien `eboot.bin` installé a été sauvegardé dans `build/diagnostic-backup/2026-09-24/eboot-before-minimal.bin`, SHA-256 `77D946E479E069B847D28281ADCBE65C375BEE71D286E6908A6FD67D6CCDD3ED`. La première sonde, vérifiée par relecture FTP, a démarré à 22:53:55. Ouverture GPS 5 et consentement : succès. À 22:56:33, l'utilisateur a placé la console dehors. Jusqu'à la dernière lecture à 22:59:46, **298 observations NMEA** indiquaient `sats_used=0` et `driver_sats=0`; **21 appels API** avaient tous retourné `0x80101200` (position indéterminée), avec environ 15 secondes d'attente chacun. Les trames NMEA continuaient à arriver : la pile GPS tournait.

La console a ensuite disparu du réseau vers 23:00:07, et l'utilisateur l'a trouvée éteinte. Au redémarrage, elle a affiché un **message d'erreur de dernière utilisation** : il faut traiter cet arrêt comme un incident possible, pas comme une simple veille. La cause (application, SceShell, batterie ou autre) n'est pas établie. Une nouvelle compilation appelle `sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND)` chaque seconde, **mais elle n'a pas été déployée** après ce message d'erreur. La session complète récupérée dans `build/diagnostic-backup/2026-09-24/gps-minimal-full.txt` s'arrête sans ligne `END` : 317 observations NMEA à zéro satellite et 23 appels API `0x80101200`.

L'eboot original a été restauré depuis la sauvegarde et relu : SHA-256 `77D946E479E069B847D28281ADCBE65C375BEE71D286E6908A6FD67D6CCDD3ED`, identique avant et après. Le plugin, `config.txt` et `app.db` n'ont pas été modifiés. La sauvegarde de `app.db` prise avant l'essai a passé `PRAGMA integrity_check` et montrait la clé GPSX00001 `2840145610=0x80000003` ; l'ouverture et le consentement ont réussi avec cette valeur.

Ne pas télécharger le journal Sony volumineux pendant l'acquisition : une tentative de lecture de ses métadonnées a reçu une réinitialisation de connexion, suivie de la perte du réseau, sans relation causale établie. Aucun nouveau dump daté de cet essai n'a été identifié dans `ux0:/data` ou `ur0:/data` après redémarrage. Avant un nouvel essai, vérifier l'état de la batterie, limiter le journal Sony aux catégories utiles et confirmer la stabilité de la console avec l'eboot original.
