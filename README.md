# 🕹️ ESP32 Arcade Station : 50 Jeux Solo & 15 Jeux Multijoueur 1v1

Plateforme de jeux d'arcade rétro et multijoueur en temps réel hébergée **100% hors-ligne** dans la mémoire Flash d'un microcontrôleur **ESP32** (ESP32-WROOM-DA ou ESP32 Dev Module).

Zéro bibliothèque tierce requise : utilise uniquement le core Arduino officiel pour ESP32.

---

## 🌟 Fonctionnalités Principales

- **🎮 50 Mini-Jeux Solo Complets :**
  - **Arcade & Classiques :** Snake, Flappy Drone, Space Invaders, Cyber Racer, Casse-Briques, 2048, Piano Tiles, Tower Stacker, Knife Master, Démineur, Morpion IA, Pong IA.
  - **Action & Tir :** Asteroids Blaster, Missile Defense, Cyber Turret, Galaxian Dive, Tank 2D Battle, Alien Swarm.
  - **Puzzle & Logique :** Lights Out, Taquin 15, Color Flood, Match-3 Blitz, Code Breaker, Mini Sudoku 4x4.
  - **Société & Cartes :** Puissance 4 IA, Blackjack 21, Memory Flip 16, PFC Duel, Plus ou Moins, Bataille Navale IA.
  - **Sport :** Tirs au But, Basketball Shoot, Mini Golf 2D, Ski Slalom, Tir à l'Arc, Air Hockey IA.
  - **Rythme & Réflexes :** Beat Tap, Simon Mémoire, Note Drop, Tempo Tap, Guitar Hero Pocket.
  - **Précision & Adresse :** Fil Électrique, Fruit Slice, Coin Jar, Grappin Swing, Balance Ball.
  - **Rétro Iconique :** Tetris Cyber, Pac-Maze, Lunar Lander, Cannon Siege.

- **⚔️ Salon Multijoueur 1v1 en Direct (15 Jeux Wi-Fi Local) :**
  - **Serveur WebSocket Natif (Port 81) :** Handshake RFC 6455 et relai ultra-faible latence (< 5 ms) 100% C++ natif sans bibliothèque externe.
  - **Lobby en temps réel :** Détection automatique des smartphones connectés, liste des joueurs en direct, envoi et acceptation de défis 1v1.
  - **Bouton « 🔄 Rejouer (Revanche) » :** Rematch instantané sans recharger la page ni perdre la connexion socket.
  - **3 Catégories de Jeux 1v1 :**
    - 🧠 **5 Jeux de Réflexion :** Puissance 4, Bataille Navale, Morpion Cyber, Duel de Mémoire (16 cartes), Mastermind Duel (4 couleurs).
    - ⚡ **5 Jeux de Réflexe :** Pong 1v1 Laser, Air Hockey Arcade, Tank Battle 2D, Duel de Réflexes (feux tricolores), Tir à la Corde / Tap Duel.
    - 📝 **5 Jeux de Baccalauréat & Mots :** Le Petit Bac Classic (4 catégories avec bouton STOP), Course aux Anagrammes, La Bombe à Mots (Word Bomb), Chaîne de Mots (Shiritori), Le Pendu Duel.

- **🔒 Sécurité, Captive Portal & Personnalisation :**
  - Mot de passe d'accès modifiable sauvegardé en mémoire permanente **NVS** (Preferences).
  - Personnalisation du **Pseudo Joueur** dans les paramètres (/settings).
  - Gestion du Wi-Fi (Point d'accès autonome ESP32-Arcade ou connexion au point d'accès du smartphone).
  - Redirection automatique Captive Portal DNS sur http://192.168.4.1/.

---

## 📁 Architecture du Code

`	ext
esp_pages/
├── esp_arcade/
│   ├── esp_arcade.ino              # Serveur Web HTTP (Port 80), DNS Captive Portal, 65 routes
│   ├── mp_server.h                 # Serveur WebSocket 1v1 (Port 81) C++ RFC 6455 natif
│   ├── hub_page.h                  # Hub d'accueil Solo & Salon Multijoueur avec Lobby WebSocket
│   ├── login_page.h                # Page de connexion sécurisée
│   ├── settings_page.h             # Page de réglages (Pseudo, Mot de passe, Wi-Fi NVS)
│   ├── game_pack_multiplayer.h     # En-tête parapluie incluant les 3 packs multijoueurs
│   ├── game_pack_mp_reflexion.h    # 5 jeux multijoueurs de Réflexion (PROGMEM)
│   ├── game_pack_mp_reflexe.h      # 5 jeux multijoueurs de Réflexe (PROGMEM)
│   ├── game_pack_mp_mots.h         # 5 jeux multijoueurs de Baccalauréat & Mots (PROGMEM)
│   ├── game_snake.h ...            # Fichiers des 50 jeux solos individuels et packs
└── README.md
`

---

## 🚀 Configuration & Téléversement (Arduino IDE)

1. Ouvrez le fichier esp_arcade/esp_arcade.ino dans l'Arduino IDE.
2. Connectez votre carte ESP32 en USB.
3. Dans le menu **Outils (Tools)** :
   - **Type de Carte :** ESP32 Dev Module (ou votre modèle ESP32).
   - **Partition Scheme :** Choisissez **Huge APP (3MB No OTA/1MB SPIFFS)** *(indispensable pour accueillir les 65 jeux en Flash)*.
   - **Upload Speed :** 921600 (ou 115200).
   - **Port :** Sélectionnez le port COM de votre ESP32.
4. Cliquez sur **Téléverser**.

---

## 📱 Utilisation

1. Allumez l'ESP32.
2. Sur votre smartphone, connectez-vous au réseau Wi-Fi :
   - **SSID :** ESP32-Arcade
   - *(Pas de mot de passe Wi-Fi par défaut)*.
3. Ouvrez votre navigateur sur **http://192.168.4.1**.
4. Entrez le mot de passe d'accès : **rcade123**.
5. Jouez en solo ou rejoignez l'onglet **⚔️ Salon Multijoueur** pour défier un ami connecté sur un 2ème téléphone !