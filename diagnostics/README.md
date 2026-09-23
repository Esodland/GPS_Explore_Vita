# Diagnostic mémoire — 12 septembre 2026

Essai exécuté sur la PCH-1100 via vitacompanion, avec le plugin d'activation précédent et le nouvel eboot. Aucune modification de app.db, param.sfo ou tai/config.txt.

Le journal `2026-09-12-memory.txt` contient les mesures en octets retournées par `sceKernelGetFreeMemorySize` avant/après activation, avant/après chaque open, après close et après vita2d.

| Moment | USER | CDRAM | PHYCONT |
|---|---:|---:|---:|
| Avant/après activation et tous les open | 121 Mio | 112 Mio | 26 Mio |
| Après vita2d et police | 111 Mio | 89 Mio | 26 Mio |

NONE : open réussit avec handle `0x80000000`, close réussit. Méthodes 5, 2, 1, 3, 4 : toutes retournent `0x80024302`, handle `0xFFFFFFFF`, avant vita2d.

Ces chiffres ne prouvent pas la disponibilité d'un bloc contigu suffisamment grand ni celle d'un pool serveur. Ils ne confirment cependant pas l'hypothèse précédente d'un pool client simplement épuisé. La taille, les contraintes et le processus de l'allocation fautive restent inconnus.

## Instrumentation IPMI expérimentale

Le désassemblage brut confirme que `sub_81001F54` prend six arguments et invoque la méthode virtuelle à `vtable+0x14` avec sept arguments. Le retour de transport et le résultat serveur sont séparés. Le pseudo-C généré omet des arguments et ne suffit pas à définir les signatures des hooks.

La première sonde a produit les six résultats d'ouverture avant une perte réseau, puis l'utilisateur a constaté « Erreur survenue au cours de la dernière utilisation » au rallumage. Les logs ont été récupérés après le redémarrage. La sonde est désactivée par défaut (`GPS_TRACE_IPMI`) et n'est pas considérée comme stable.

**Résultat matériel :** pour la commande register `0x12340000`, NONE donne transport=0 et serveur=0 ; les méthodes 5/2/1/3/4 donnent toutes **transport=0, serveur=0x80024302**. Le wrapper propage ce même code. La fonction virtuelle de transport interceptée appartient à `SceShellSvc`, offset texte `0x47CE`. Cela ne prouve pas que ce module héberge le serveur : il s'agit ici de la fonction de transport du processus client.

Le plugin d'activation original a été restauré depuis la sauvegarde puis relu par FTP avec vérification SHA-256. Après relancement de l'application, la série mémoire s'est terminée, vita2d a été initialisé et le FTP est resté accessible. La console conserve l'eboot de diagnostic mémoire et le plugin original, sans la sonde IPMI.

La suite est de trouver le serveur et son allocation fautive. Le transport réussi permet d'écarter un échec de transport local comme source directe de ce retour ; les contraintes mémoire exactes côté serveur restent inconnues.

Compilation du plugin d'activation sans sonde : `./plugin/build-activate.ps1`. L'option `-TraceIpmi` produit un fichier distinct `build/gps_activate_trace.suprx` pour investigation ultérieure.

Les sauvegardes matérielles antérieures aux essais sont dans `build/diagnostic-backup/` (eboot, plugin, configuration et log). Elles sont locales et ignorées par Git. Ne pas les écraser pendant la reprise.

## Application

Au lancement, le diagnostic teste automatiquement NONE puis 5/2/1/3/4 avant vita2d et referme chaque handle ouvert avec succès. CROIX démarre ensuite le flux interactif GPS. Seuls les handles d'un open réussi sont utilisés ; le consentement est vérifié avec GetStatus puis GetResult. START quitte.

Les erreurs affichées correspondent maintenant à l'étape qui échoue. Les anciennes affirmations sur SIM, Skyhook et Photos ont été retirées. La variable d'erreur masquée et le Confirm(1) artificiel ont été supprimés.

## Incident FTP du 13/09/2026 (sans rapport avec la sonde IPMI du 12/09)

Une tentative de récupération en masse de ~40 modules `os0:kd` déchiffrés (via `vitacompanion`, port 1337) a lancé ~80 connexions FTP en parallèle vers la console. Le service `vitacompanion` en a été saturé : la console s'est figée (plus aucune réponse FTP, y compris un simple listing racine), l'utilisateur a dû l'éteindre de force. Aucun rapport avec la sonde IPMI/kernel du 12/09 — cause purement réseau (surcharge du plugin FTP embarqué, pas un crash noyau). Après redémarrage, la console répond normalement.

**Leçon retenue :** ne jamais paralléliser les téléchargements FTP vers `vitacompanion` — une seule requête à la fois, séquentiellement.
