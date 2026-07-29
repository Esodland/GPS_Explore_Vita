# 🚀 Projet PS Vita GPS "Go!Explore 2.0"
### Spécifications Techniques & Roadmap de Développement

---

## 📌 1. Vue d'ensemble du Projet

Le projet **"Go!Explore 2.0"** vise à créer une application *homebrew* de navigation GPS native pour la **Sony PlayStation Vita 1000 (modèle PCH-1100 3G/Wi-Fi)**. 

Profitant du matériel GPS intégré inutilisé depuis la fermeture des services officiels (Near, Cartes), cette application permet d'exploiter le récepteur satellite en mode hors-ligne (*offline mapping*) avec des cartes vectorielles ou basées sur des tuiles **OpenStreetMap (OSM)**.

```
+-----------------------------------------------------------------+
|                    PS VITA 1000 (PCH-1100)                      |
|                                                                 |
|   +-------------------+    +--------------------------------+   |
|   |  Hardware GPS     |    |  Stockage MicroSD (SD2Vita)    |   |
|   |  Qualcomm Modem   |    |  Tuiles OSM / Base de données  |   |
|   +---------+---------+    +---------------+----------------+   |
|             |                              |                    |
|             v                              v                    |
|   +---------------------------------------------------------+   |
|   |                  SceLocation API (vitasdk)              |   |
|   +----------------------------+----------------------------+   |
|                                |                                |
|                                v                                |
|   +---------------------------------------------------------+   |
|   |            Moteur App C/C++ (Go!Explore Vita)           |   |
|   |  - Parsing NMEA / Coordonnées Lat/Lon                   |   |
|   |  - Projection de Mercator (Lat/Lon -> X/Y)              |   |
|   |  - Rendu Graphique (Vita2D / VitaGXM 60fps)             |   |
|   +----------------------------+----------------------------+   |
|                                |                                |
|                                v                                |
|   +---------------------------------------------------------+   |
|   |                 Écran OLED (960 x 544)                  |   |
|   +---------------------------------------------------------+   |
+-----------------------------------------------------------------+
```

---

## ⚙️ 2. Architecture Technique & Architecture logicielle

### 2.1 Composants Logiciels Requis
* **Toolchain :** `VitaSDK` (C/C++ pour PS Vita)
* **API Système :** `SceLocation` (`#include <psp2/location.h>`)
* **Moteur Graphique :** `vita2dlib` ou `vitaGXM`
* **Cartographie :** OpenStreetMap (Slippy Map Tiles / Projection Mercator Web)

### 2.2 Dépendances VitaSDK
Les bibliothèques requises dans le `CMakeLists.txt` :
```cmake
target_link_libraries(vita_gps
    SceLocation_stub
    SceDisplay_stub
    SceGxt_stub
    vita2d
    ScePgf_stub
    SceSysmodule_stub
)
```

---

## 🛠️ 3. Roadmap de Développement (4 Phases)

### Phase 1 : Proof of Concept - Récupération du Signal GPS (PoC)
- [ ] Charger le module système `SCE_SYSMODULE_LOCATION`.
- [ ] Ouvrir l'instance de localisation via `sceLocationOpen()`.
- [ ] Forcer l'utilisation du récepteur GPS matériel (`SCE_LOCATION_METHOD_GPS`).
- [ ] Implémenter la boucle de lecture des coordonnées (`SceLocationLocation`).
- [ ] Afficher en texte brut à l'écran : **Latitude**, **Longitude**, **Altitude**, **Vitesse**, **Cap**.

### Phase 2 : Rendu Cartographique Hors-Ligne
- [ ] Implémenter l'algorithme de conversion de coordonnées Mercator :
  $$	ext{tile}_x = \lfloor rac{	ext{lon} + 180}{360} \cdot 2^z floor$$
  $$	ext{tile}_y = \lfloor (1 - rac{\ln(	an(	ext{lat} \cdot rac{\pi}{180}) + rac{1}{\cos(	ext{lat} \cdot rac{\pi}{180})})}{\pi}) \cdot 2^{z-1} floor$$
- [ ] Charger les tuiles cartographiques PNG depuis `ux0:data/vita_gps/tiles/{z}/{x}/{y}.png`.
- [ ] Centrer la carte dynamique sur la position actuelle du curseur GPS.

### Phase 3 : Interface Utilisateur & Contrôles Go!Explore
- [ ] Design UI style rétro-futuriste inspiré de *Go!Explore PSP*.
- [ ] Zoom In/Out via les gâchettes L/R ou le pinch-to-zoom de l'écran tactile.
- [ ] Mode "Suivi de cap" (rotation dynamique de la carte selon le cap de déplacement).

### Phase 4 : Routing & Audio (Bonus)
- [ ] Intégration d'un moteur de routing léger (calcul d'itinéraire offline).
- [ ] Lecture de fichiers WAV pour le guidage vocal ("Dans 100 mètres, tournez à gauche").

---

## 💻 4. Code de Départ : Squelette `main.c`

Voici le code minimaliste C pour initialiser la puce GPS de la PS Vita 1000 :

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/sysmodule.h>
#include <psp2/location.h>
#include <vita2d.h>

#define CONVERT_RAD_TO_DEG(rad) ((rad) * (180.0 / 3.14159265358979323846))

int main(int argc, char *argv[]) {
    vita2d_init();
    vita2d_set_clear_color(RGBA8(20, 20, 30, 255));
    
    vita2d_pgf *font = vita2d_load_default_pgf();

    // 1. Charger le module Système Location
    sceSysmoduleLoadModule(SCE_SYSMODULE_LOCATION);

    // 2. Initialiser le service de localisation
    SceLocationDialogParam param;
    memset(&param, 0, sizeof(param));
    
    int handle = sceLocationOpen(SCE_LOCATION_METHOD_GPS);
    if (handle < 0) {
        // Erreur d'ouverture ou modèle Wi-Fi uniquement
    }

    SceLocationLocation location;
    memset(&location, 0, sizeof(location));

    while (1) {
        vita2d_start_drawing();
        vita2d_clear();

        // 3. Récupérer les coordonnées actuelles
        int ret = sceLocationGetLocation(handle, &location);

        if (ret == 0) {
            char buf[128];
            
            snprintf(buf, sizeof(buf), "GPS Status: FIX OBTENU");
            vita2d_pgf_draw_text(font, 20, 50, RGBA8(0, 255, 0, 255), 1.2f, buf);

            snprintf(buf, sizeof(buf), "Latitude  : %f", location.latitude);
            vita2d_pgf_draw_text(font, 20, 100, RGBA8(255, 255, 255, 255), 1.0f, buf);

            snprintf(buf, sizeof(buf), "Longitude : %f", location.longitude);
            vita2d_pgf_draw_text(font, 20, 130, RGBA8(255, 255, 255, 255), 1.0f, buf);

            snprintf(buf, sizeof(buf), "Altitude  : %.1f m", location.altitude);
            vita2d_pgf_draw_text(font, 20, 160, RGBA8(255, 255, 255, 255), 1.0f, buf);

            snprintf(buf, sizeof(buf), "Vitesse   : %.1f km/h", location.speed * 3.6f);
            vita2d_pgf_draw_text(font, 20, 190, RGBA8(255, 255, 255, 255), 1.0f, buf);
        } else {
            vita2d_pgf_draw_text(font, 20, 50, RGBA8(255, 165, 0, 255), 1.2f, "Recherche des satellites (Cold Start)...");
            vita2d_pgf_draw_text(font, 20, 90, RGBA8(200, 200, 200, 255), 1.0f, "Assurez-vous d'etre en exterieur.");
        }

        vita2d_end_drawing();
        vita2d_swap_buffers();
    }

    // Nettoyage
    sceLocationClose(handle);
    sceSysmoduleUnloadModule(SCE_SYSMODULE_LOCATION);
    
    vita2d_free_pgf(font);
    vita2d_final();
    
    sceKernelExitProcess(0);
    return 0;
}
```

---

## ⚠️ 5. Défis & Solutions Techniques

| Défis | Origine du problème | Solution recommandée |
| :--- | :--- | :--- |
| **Temps de Cold Start** | Pas de réseau 3G/A-GPS pour les éphémérides. | Afficher un écran de recherche style radar pendant les 3 à 8 min initiales. |
| **Puce GPS absente** | L'utilisateur lance le homebrew sur Vita 1000 Wi-Fi ou Vita 2000. | Détecter le code d'erreur `sceLocationOpen()` et afficher un message d'avertissement. |
| **Performances Rendu** | Affichage de trop de tuiles simultanément. | Charger/décharger dynamiquement en mémoire RAM les tuiles adjacentes à l'écran (*caching*). |

---

## 👥 6. Notes pour la Communauté

Ce document est destiné à servir de base pour la création du repo GitHub **`GoExplore-Vita`**. 
Toutes les contributions (C/C++, UI Design, Tests Hardware) sont les bienvenues !
