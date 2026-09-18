#ifndef HUB_PAGE_H
#define HUB_PAGE_H

#include <pgmspace.h>

const char HUB_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>ESP32 Arcade - Solo & Salon Multijoueur</title>
  <style>
    :root {
      --bg: #060811;
      --card: rgba(18, 23, 40, 0.88);
      --card-border: rgba(0, 243, 255, 0.22);
      --cyan: #00f3ff;
      --pink: #ff0055;
      --yellow: #ffe600;
      --green: #00ff66;
      --text: #f0f4fc;
      --dim: #8b9bb4;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: var(--bg);
      background-image: 
        radial-gradient(circle at 15% 15%, rgba(0, 243, 255, 0.12) 0%, transparent 45%),
        radial-gradient(circle at 85% 85%, rgba(255, 0, 85, 0.10) 0%, transparent 45%),
        repeating-linear-gradient(0deg, rgba(255, 255, 255, 0.015) 0px, rgba(255, 255, 255, 0.015) 1px, transparent 1px, transparent 32px);
      color: var(--text);
      font-family: system-ui, -apple-system, sans-serif;
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    header {
      width: 100%; max-width: 900px; padding: 14px 18px;
      display: flex; justify-content: space-between; align-items: center;
      position: sticky; top: 0; background: rgba(7, 9, 18, 0.92);
      backdrop-filter: blur(12px); -webkit-backdrop-filter: blur(12px); z-index: 20;
      border-bottom: 1px solid rgba(0, 243, 255, 0.15);
      box-shadow: 0 4px 20px rgba(0,0,0,0.6);
    }
    .logo {
      display: flex; align-items: center; gap: 8px;
      font-size: 1.3rem; font-weight: 900; color: var(--cyan);
      text-shadow: 0 0 14px rgba(0, 243, 255, 0.5); text-transform: uppercase;
      letter-spacing: 1px;
    }
    .header-btns { display: flex; gap: 8px; }
    .btn-h {
      background: rgba(255,255,255,0.07); border: 1px solid rgba(255,255,255,0.2);
      color: #fff; padding: 7px 14px; border-radius: 20px; font-size: 0.8rem; font-weight: 700;
      text-decoration: none; display: inline-flex; align-items: center; gap: 5px;
      box-shadow: 0 2px 8px rgba(0,0,0,0.3); transition: all 0.15s;
    }
    .btn-h:active { transform: scale(0.95); }
    .container {
      width: 100%; max-width: 900px; padding: 16px 16px 45px;
      display: flex; flex-direction: column; gap: 16px;
    }
    .hero {
      background: linear-gradient(145deg, rgba(20, 26, 48, 0.9), rgba(11, 15, 28, 0.9));
      border: 1px solid rgba(0, 243, 255, 0.25);
      border-top: 1px solid rgba(255,255,255,0.2);
      border-radius: 20px; padding: 20px; text-align: center;
      box-shadow: 0 10px 35px rgba(0,0,0,0.6), inset 0 1px 0 rgba(255,255,255,0.1);
      position: relative; overflow: hidden;
    }
    .hero::before {
      content: ''; position: absolute; top: 0; left: 20%; right: 20%; height: 2px;
      background: linear-gradient(90deg, transparent, var(--cyan), transparent);
    }
    .hero h1 {
      font-size: 1.65rem; font-weight: 900; letter-spacing: 1px; margin-bottom: 4px;
      background: linear-gradient(135deg, #fff 10%, var(--cyan) 60%, var(--pink) 100%);
      -webkit-background-clip: text; -webkit-text-fill-color: transparent;
      text-transform: uppercase;
    }
    .hero p { color: var(--dim); font-size: 0.88rem; margin-bottom: 14px; }
    
    .mode-nav {
      display: flex; gap: 12px; width: 100%; margin-bottom: 12px;
    }
    .btn-mode {
      flex: 1; padding: 12px 16px; border-radius: 12px; font-size: 0.95rem; font-weight: 900;
      border: 1px solid rgba(255,255,255,0.12); background: rgba(18, 22, 38, 0.8);
      color: var(--dim); cursor: pointer; transition: all 0.2s;
      box-shadow: 0 4px 12px rgba(0,0,0,0.4), inset 0 1px 0 rgba(255,255,255,0.1);
      display: flex; align-items: center; justify-content: center; gap: 8px;
    }
    .btn-mode:active { transform: scale(0.97); }
    .btn-mode#tab-solo.active {
      background: linear-gradient(135deg, rgba(0,243,255,0.22), rgba(0,120,255,0.22));
      border-color: var(--cyan); color: #fff;
      box-shadow: 0 0 22px rgba(0,243,255,0.35), inset 0 1px 0 rgba(255,255,255,0.3);
    }
    .btn-mode#tab-mp.active {
      background: linear-gradient(135deg, rgba(255,0,85,0.22), rgba(255,85,0,0.22));
      border-color: var(--pink); color: #fff;
      box-shadow: 0 0 22px rgba(255,0,85,0.35), inset 0 1px 0 rgba(255,255,255,0.3);
    }
    
    .search-box {
      width: 100%; background: #080b16; border: 1px solid rgba(0,243,255,0.3);
      border-radius: 12px; padding: 12px 16px; color: #fff; font-size: 0.95rem;
      outline: none; transition: all 0.2s;
    }
    .search-box:focus {
      border-color: var(--cyan);
      box-shadow: 0 0 18px rgba(0,243,255,0.3);
      background: #0b1020;
    }
    
    .grid {
      display: grid; grid-template-columns: repeat(auto-fill, minmax(255px, 1fr));
      gap: 14px;
    }
    .game-card {
      background: linear-gradient(145deg, rgba(22, 28, 48, 0.95), rgba(12, 16, 28, 0.95));
      border: 1px solid rgba(0, 243, 255, 0.18);
      border-top: 1px solid rgba(255, 255, 255, 0.18);
      border-radius: 16px; padding: 16px; display: flex; flex-direction: column;
      justify-content: space-between; text-decoration: none; color: inherit;
      position: relative; overflow: hidden;
      box-shadow: 0 8px 24px rgba(0,0,0,0.5), inset 0 1px 0 rgba(255,255,255,0.08);
      transition: transform 0.2s cubic-bezier(0.34, 1.56, 0.64, 1), box-shadow 0.2s, border-color 0.2s;
    }
    .game-card:hover, .game-card:active {
      transform: translateY(-4px) scale(1.01);
      border-color: var(--cyan);
      box-shadow: 0 14px 30px rgba(0, 0, 0, 0.7), 0 0 20px rgba(0, 243, 255, 0.25);
    }
    .card-top { display: flex; align-items: center; gap: 12px; margin-bottom: 8px; }
    .card-icon {
      font-size: 1.8rem; width: 46px; height: 46px; display: flex;
      align-items: center; justify-content: center; background: rgba(255,255,255,0.06);
      border-radius: 12px; border: 1px solid rgba(255,255,255,0.12); flex-shrink: 0;
      box-shadow: 0 3px 10px rgba(0,0,0,0.3);
    }
    .card-info h2 { font-size: 1.02rem; font-weight: 800; margin-bottom: 2px; }
    .card-info p { font-size: 0.76rem; color: var(--dim); line-height: 1.3; }
    .card-bot {
      display: flex; justify-content: space-between; align-items: center;
      margin-top: 12px; padding-top: 10px; border-top: 1px solid rgba(255,255,255,0.08);
      font-size: 0.8rem; font-weight: 700; color: var(--dim);
    }
    .play-pill {
      background: linear-gradient(180deg, #00f3ff, #0077ee);
      color: #000; padding: 5px 12px; border-radius: 20px;
      font-size: 0.75rem; font-weight: 900; letter-spacing: 0.5px;
      box-shadow: 0 3px 10px rgba(0, 243, 255, 0.35);
      transition: all 0.15s;
    }
    .play-pill:active { transform: scale(0.92); }

    /* --- LOBBY STYLES --- */
    #mp-container { display: flex; flex-direction: column; gap: 14px; width: 100%; }
    .lobby-box {
      background: linear-gradient(145deg, rgba(20, 26, 48, 0.9), rgba(11, 15, 28, 0.9));
      border: 1px solid rgba(255, 0, 85, 0.25);
      border-top: 1px solid rgba(255, 255, 255, 0.15);
      border-radius: 18px; padding: 18px; display: flex; flex-direction: column; gap: 12px;
      box-shadow: 0 10px 35px rgba(0,0,0,0.6);
    }
    .lobby-header { display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid rgba(255,255,255,0.08); padding-bottom: 12px; }
    .lobby-header h2 { font-size: 1.15rem; color: #fff; font-weight: 800; display: flex; align-items: center; gap: 8px; }
    .pulse-dot { width: 10px; height: 10px; border-radius: 50%; background: #00ff66; box-shadow: 0 0 10px #00ff66; display: inline-block; animation: pulse 1.5s infinite alternate; }
    
    .players-list { display: flex; flex-direction: column; gap: 8px; }
    .player-row {
      background: linear-gradient(145deg, #0e1324, #080b16);
      border: 1px solid rgba(0,243,255,0.15); border-left: 4px solid var(--dim);
      border-radius: 12px; padding: 12px 16px; display: flex;
      justify-content: space-between; align-items: center;
      box-shadow: 0 4px 15px rgba(0,0,0,0.4);
      transition: all 0.2s;
    }
    .player-row.you {
      border-color: rgba(0,243,255,0.4); border-left-color: var(--cyan);
      background: linear-gradient(145deg, rgba(0,243,255,0.08), #080b16);
    }
    .player-info { display: flex; align-items: center; gap: 12px; }
    .player-name { font-weight: 800; font-size: 1.05rem; }
    .player-status { font-size: 0.78rem; color: var(--dim); margin-top: 2px; }
    .btn-defier {
      background: linear-gradient(180deg, #ff0055, #cc0033); border: none;
      color: #fff; font-weight: 900; font-size: 0.88rem; padding: 8px 16px;
      border-radius: 10px; cursor: pointer;
      box-shadow: 0 4px 14px rgba(255,0,85,0.4);
      transition: all 0.15s;
    }
    .btn-defier:active { transform: scale(0.95); }

    /* MODAL DÉFI */
    .modal-backdrop {
      position: fixed; inset: 0; background: rgba(0,0,0,0.85);
      backdrop-filter: blur(8px); -webkit-backdrop-filter: blur(8px);
      display: none; align-items: center; justify-content: center;
      z-index: 100; padding: 20px;
    }
    .modal-box {
      background: #10152a; border: 2px solid var(--cyan);
      border-radius: 22px; max-width: 420px; width: 100%; padding: 22px;
      text-align: center; box-shadow: 0 0 40px rgba(0,243,255,0.35);
      display: flex; flex-direction: column; gap: 14px;
    }
    .modal-box h3 { font-size: 1.35rem; color: var(--cyan); font-weight: 900; }
    .game-pick-list { display: grid; grid-template-columns: 1fr; gap: 8px; max-height: 280px; overflow-y: auto; }
    .btn-pick-game {
      background: #182038; border: 1px solid rgba(0,243,255,0.25);
      border-radius: 12px; padding: 12px 16px; color: #fff; font-size: 0.95rem; font-weight: 800;
      display: flex; align-items: center; justify-content: space-between; cursor: pointer;
      transition: all 0.15s;
    }
    .btn-pick-game:hover, .btn-pick-game:active {
      background: var(--cyan); color: #000;
      box-shadow: 0 0 15px rgba(0,243,255,0.4);
    }
    .btn-cancel {
      background: rgba(255,255,255,0.1); border: none; color: var(--dim);
      padding: 10px; border-radius: 10px; cursor: pointer; font-weight: 800;
    }

    /* ALERT INVITATION */
    #invite-modal .modal-box { border-color: var(--pink); box-shadow: 0 0 40px rgba(255,0,85,0.45); }
    .invite-actions { display: flex; gap: 12px; margin-top: 12px; }
    .btn-accept { flex: 1; padding: 14px; border-radius: 12px; background: linear-gradient(180deg, #00ff66, #00cc44); border: none; color: #000; font-weight: 900; font-size: 1rem; cursor: pointer; box-shadow: 0 4px 15px rgba(0,255,102,0.4); }
    .btn-decline { flex: 1; padding: 14px; border-radius: 12px; background: linear-gradient(180deg, #ff0055, #cc0033); border: none; color: #fff; font-weight: 900; font-size: 1rem; cursor: pointer; box-shadow: 0 4px 15px rgba(255,0,85,0.4); }

    footer { margin-top: auto; padding: 20px; color: var(--dim); font-size: 0.82rem; text-align: center; }
  </style>
</head>
<body>
  <header>
    <div class="logo">🕹️ ESP32 Arcade</div>
    <div class="header-btns">
      <a href="/settings" class="btn-h">⚙️ Réglages</a>
      <a href="/logout" class="btn-h" style="border-color:var(--pink);color:#ff4d79;">🔒 Quitter</a>
    </div>
  </header>

  <main class="container">
    <div class="hero">
      <h1 id="hero-title">🎮 ESP32 ARCADE STATION</h1>
      <p id="hero-sub">50+ Jeux Solo, FPS 3D WebGL & Multijoueur 1v1 / 3-8 Joueurs !</p>
      
      <div class="mode-nav">
        <button class="btn-mode active" id="tab-solo" onclick="setMode('solo')">🕹️ Solo & FPS 3D</button>
        <button class="btn-mode" id="tab-mp" onclick="setMode('mp')">⚔️ Salon Multijoueur (1v1 & 3-8J)</button>
      </div>

      <input type="text" id="search" class="search-box" placeholder="🔍 Rechercher un jeu solo ou duel IA..." oninput="filterGames()">
    </div>

    <!-- VUE 1 : JEUX SOLO & 3D -->
    <div class="grid" id="game-grid">
      <!-- NOUVEAUTÉ VEDETTE : FPS 3D WEBGL -->
      <a href="/game_fps3d" class="game-card" data-title="cyber fps 3d webgl retro doom wolfenstein shooter" style="border-color:var(--cyan); box-shadow: 0 0 20px rgba(0,243,255,0.3); grid-column: 1 / -1;">
        <div class="card-top">
          <div class="card-icon" style="color:var(--cyan); border-color:var(--cyan); font-size:1.9rem; background:rgba(0,243,255,0.15);">🕶️</div>
          <div class="card-info">
            <h2 style="color:var(--cyan); font-size:1.15rem;">🔥 Cyber-FPS 3D (Moteur WebGL 60 FPS)</h2>
            <p>FPS 3D Rétro style Doom / Wolfenstein 3D. Drones ennemis, blaster plasma, radar & 60 FPS !</p>
          </div>
        </div>
        <div class="card-bot">
          <span style="color:var(--cyan); font-weight:bold;">🕶️ Moteur 3D Temps Réel</span>
          <span class="play-pill" style="background:var(--cyan); color:#000;">Lancer la Mission 3D ▶</span>
        </div>
      </a>

      <!-- 1-12 LES 12 ORIGINAUX -->
      <a href="/game_snake" class="game-card" data-title="snake retro"><div class="card-top"><div class="card-icon">🐍</div><div class="card-info"><h2>1. Snake Retro</h2><p>Mange les orbes néon et évite les murs.</p></div></div><div class="card-bot"><span>🏆 Arcade</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_flappy" class="game-card" data-title="flappy drone"><div class="card-top"><div class="card-icon">🐦</div><div class="card-info"><h2>2. Flappy Drone</h2><p>Fais planer le drone entre les pylônes.</p></div></div><div class="card-bot"><span>🏆 Arcade</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_invaders" class="game-card" data-title="space invaders"><div class="card-top"><div class="card-icon">👾</div><div class="card-info"><h2>3. Space Invaders</h2><p>Tire sur les vagues d'aliens qui descendent.</p></div></div><div class="card-bot"><span>🚀 Shooter</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_racer" class="game-card" data-title="cyber racer"><div class="card-top"><div class="card-icon">🏎️</div><div class="card-info"><h2>4. Cyber Racer</h2><p>Double le trafic urbain à plus de 200 km/h.</p></div></div><div class="card-bot"><span>🏎️ Course</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_breakout" class="game-card" data-title="casse briques"><div class="card-top"><div class="card-icon">🧱</div><div class="card-info"><h2>5. Casse-Briques</h2><p>Détruis toutes les briques colorées.</p></div></div><div class="card-bot"><span>🧱 Rétro</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_2048" class="game-card" data-title="2048 neon"><div class="card-top"><div class="card-icon">🔢</div><div class="card-info"><h2>6. 2048 Neon</h2><p>Fusionne les tuiles numérotées.</p></div></div><div class="card-bot"><span>🧩 Puzzle</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_pianotiles" class="game-card" data-title="piano tiles"><div class="card-top"><div class="card-icon">🎹</div><div class="card-info"><h2>7. Piano Tiles</h2><p>Touche les notes noires en rythme.</p></div></div><div class="card-bot"><span>🎵 Musique</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_stacker" class="game-card" data-title="tower stacker"><div class="card-top"><div class="card-icon">🏢</div><div class="card-info"><h2>8. Tower Stacker</h2><p>Empile les blocs pour bâtir la tour.</p></div></div><div class="card-bot"><span>🎯 Adresse</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_knife" class="game-card" data-title="knife master"><div class="card-top"><div class="card-icon">🔪</div><div class="card-info"><h2>9. Knife Master</h2><p>Plante tes lames dans la cible tournante.</p></div></div><div class="card-bot"><span>🎯 Précision</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_mines" class="game-card" data-title="demineur neon"><div class="card-top"><div class="card-icon">💣</div><div class="card-info"><h2>10. Démineur Néon</h2><p>Dévoile les cases sans exploser.</p></div></div><div class="card-bot"><span>🧩 Réflexion</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_tictactoe" class="game-card" data-title="morpion cyber ia"><div class="card-top"><div class="card-icon">❌</div><div class="card-info"><h2>11. Morpion Cyber IA</h2><p>Duel tactique contre l'ordinateur.</p></div></div><div class="card-bot"><span>🎲 Société</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_pong" class="game-card" data-title="pong vs ia pro"><div class="card-top"><div class="card-icon">🏓</div><div class="card-info"><h2>12. Pong vs IA Pro</h2><p>Tennis de table rétro avec effets.</p></div></div><div class="card-bot"><span>🏓 Sport</span><span class="play-pill">Jouer ▶</span></div></a>

      <!-- 13-18 PACK ACTION -->
      <a href="/game_asteroids" class="game-card" data-title="asteroids blaster"><div class="card-top"><div class="card-icon">☄️</div><div class="card-info"><h2>13. Asteroids Blaster</h2><p>Vaisseau libre 360° et tirs d'astéroïdes.</p></div></div><div class="card-bot"><span>🚀 Shooter</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_missile" class="game-card" data-title="missile defense"><div class="card-top"><div class="card-icon">🛡️</div><div class="card-info"><h2>14. Missile Defense</h2><p>Intercepte les bombes avant l'impact.</p></div></div><div class="card-bot"><span>🚀 Shooter</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_turret" class="game-card" data-title="cyber turret"><div class="card-top"><div class="card-icon">🤖</div><div class="card-info"><h2>15. Cyber Turret</h2><p>Tourelle rotative détruisant les robots.</p></div></div><div class="card-bot"><span>🚀 Action</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_galaxian" class="game-card" data-title="galaxian dive"><div class="card-top"><div class="card-icon">🛸</div><div class="card-info"><h2>16. Galaxian Dive</h2><p>Élimine les vaisseaux attaquant en piqué.</p></div></div><div class="card-bot"><span>🚀 Shooter</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_tank" class="game-card" data-title="tank 2d battle"><div class="card-top"><div class="card-icon">🛡️</div><div class="card-info"><h2>17. Tank 2D Battle</h2><p>Duel de blindés avec tirs de canon.</p></div></div><div class="card-bot"><span>🚀 Action</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_swarm" class="game-card" data-title="alien swarm"><div class="card-top"><div class="card-icon">💥</div><div class="card-info"><h2>18. Alien Swarm</h2><p>Survis face à l'essaim alien rapide.</p></div></div><div class="card-bot"><span>🚀 Action</span><span class="play-pill">Jouer ▶</span></div></a>

      <!-- 19-24 PACK PUZZLE -->
      <a href="/game_lightsout" class="game-card" data-title="lights out"><div class="card-top"><div class="card-icon">💡</div><div class="card-info"><h2>19. Lights Out</h2><p>Éteins toutes les lampes de la matrice.</p></div></div><div class="card-bot"><span>🧩 Puzzle</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_slide15" class="game-card" data-title="taquin 15"><div class="card-top"><div class="card-icon">🧩</div><div class="card-info"><h2>20. Taquin Slide 15</h2><p>Remets les tuiles en ordre de 1 à 15.</p></div></div><div class="card-bot"><span>🧩 Puzzle</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_colorflood" class="game-card" data-title="color flood"><div class="card-top"><div class="card-icon">🎨</div><div class="card-info"><h2>21. Color Flood</h2><p>Inonde la grille d'une seule couleur.</p></div></div><div class="card-bot"><span>🧩 Puzzle</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_match3" class="game-card" data-title="match 3 blitz"><div class="card-top"><div class="card-icon">🔮</div><div class="card-info"><h2>22. Match-3 Blitz</h2><p>Aligne 3 gemmes pour marquer des points.</p></div></div><div class="card-bot"><span>🧩 Puzzle</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_codebreaker" class="game-card" data-title="code breaker"><div class="card-top"><div class="card-icon">🧠</div><div class="card-info"><h2>23. Code Breaker</h2><p>Trouve la combinaison secrète à 4 couleurs.</p></div></div><div class="card-bot"><span>🧩 Logique</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_sudoku" class="game-card" data-title="sudoku 4x4"><div class="card-top"><div class="card-icon">🔢</div><div class="card-info"><h2>24. Mini Sudoku 4x4</h2><p>Complète la grille sans doublons.</p></div></div><div class="card-bot"><span>🧩 Logique</span><span class="play-pill">Jouer ▶</span></div></a>

      <!-- 25-30 PACK BOARD -->
      <a href="/game_connect4" class="game-card" data-title="puissance 4"><div class="card-top"><div class="card-icon">🔴</div><div class="card-info"><h2>25. Puissance 4</h2><p>Aligne 4 jetons contre l'ordinateur.</p></div></div><div class="card-bot"><span>🎲 Société</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_blackjack" class="game-card" data-title="blackjack 21"><div class="card-top"><div class="card-icon">🃏</div><div class="card-info"><h2>26. Blackjack 21</h2><p>Bats la banque sans dépasser 21.</p></div></div><div class="card-bot"><span>🎲 Cartes</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_memcards" class="game-card" data-title="memory 16"><div class="card-top"><div class="card-icon">🎴</div><div class="card-info"><h2>27. Memory Flip 16</h2><p>Trouve les 8 paires de cartes cachées.</p></div></div><div class="card-bot"><span>🧠 Mémoire</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_pfc" class="game-card" data-title="pfc duel"><div class="card-top"><div class="card-icon">✂️</div><div class="card-info"><h2>28. PFC Duel</h2><p>Pierre Feuille Ciseaux contre l'IA.</p></div></div><div class="card-bot"><span>🎲 Duel</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_highlow" class="game-card" data-title="high low"><div class="card-top"><div class="card-icon">🃏</div><div class="card-info"><h2>29. Plus ou Moins</h2><p>Devine la prochaine carte tirée.</p></div></div><div class="card-bot"><span>🎲 Cartes</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_battleship" class="game-card" data-title="bataille navale"><div class="card-top"><div class="card-icon">🚢</div><div class="card-info"><h2>30. Bataille Navale</h2><p>Coule la flotte ennemie cachée.</p></div></div><div class="card-bot"><span>🎲 Stratégie</span><span class="play-pill">Jouer ▶</span></div></a>

      <!-- 31-36 PACK SPORTS -->
      <a href="/game_penalty" class="game-card" data-title="penalty"><div class="card-top"><div class="card-icon">⚽</div><div class="card-info"><h2>31. Tirs au But</h2><p>Marque des buts face au robot gardien.</p></div></div><div class="card-bot"><span>⚽ Sport</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_basket" class="game-card" data-title="basket dunk"><div class="card-top"><div class="card-icon">🏀</div><div class="card-info"><h2>32. Basket Dunk</h2><p>Règle la parabole et rentre le panier.</p></div></div><div class="card-bot"><span>🏀 Sport</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_dash" class="game-card" data-title="sprint 100m"><div class="card-top"><div class="card-icon">🏃</div><div class="card-info"><h2>33. Sprint 100m</h2><p>Alterne G/D pour courir à toute vitesse.</p></div></div><div class="card-bot"><span>🏃 Sport</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_ski" class="game-card" data-title="ski slalom"><div class="card-top"><div class="card-icon">🎿</div><div class="card-info"><h2>34. Ski Slalom</h2><p>Passe entre les portes sans toucher.</p></div></div><div class="card-bot"><span>🎿 Sport</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_archery" class="game-card" data-title="tir a l arc"><div class="card-top"><div class="card-icon">🏹</div><div class="card-info"><h2>35. Tir à l'Arc</h2><p>Tire au centre de la cible mouvante.</p></div></div><div class="card-bot"><span>🎯 Précision</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_airhockey" class="game-card" data-title="air hockey"><div class="card-top"><div class="card-icon">🏒</div><div class="card-info"><h2>36. Air Hockey</h2><p>Marque des buts avec le palet glissant.</p></div></div><div class="card-bot"><span>🏒 Sport</span><span class="play-pill">Jouer ▶</span></div></a>

      <!-- 37-41 PACK RHYTHM -->
      <a href="/game_beattap" class="game-card" data-title="beat tap"><div class="card-top"><div class="card-icon">🥁</div><div class="card-info"><h2>37. Beat Tap</h2><p>Frappe le rythme exact sur la barre.</p></div></div><div class="card-bot"><span>🎵 Rythme</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_simon" class="game-card" data-title="simon"><div class="card-top"><div class="card-icon">🔊</div><div class="card-info"><h2>38. Simon Musical</h2><p>Mémorise la séquence de 4 notes.</p></div></div><div class="card-bot"><span>🎵 Mémoire</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_notedrop" class="game-card" data-title="note drop"><div class="card-top"><div class="card-icon">🎼</div><div class="card-info"><h2>39. Note Drop Rain</h2><p>Rattrape les notes de musique qui tombent.</p></div></div><div class="card-bot"><span>🎵 Rythme</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_tempo" class="game-card" data-title="tempo tapper"><div class="card-top"><div class="card-icon">🎧</div><div class="card-info"><h2>40. Tempo Tapper</h2><p>Maintiens un tempo de 60 BPM régulier.</p></div></div><div class="card-bot"><span>🎵 Musique</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_guitar" class="game-card" data-title="guitar mini"><div class="card-top"><div class="card-icon">🎸</div><div class="card-info"><h2>41. Guitar Hero Mini</h2><p>Actionne les 3 frettes au passage des notes.</p></div></div><div class="card-bot"><span>🎵 Rythme</span><span class="play-pill">Jouer ▶</span></div></a>

      <!-- 42-46 PACK PRECISION -->
      <a href="/game_wire" class="game-card" data-title="fil electrique"><div class="card-top"><div class="card-icon">⚡</div><div class="card-info"><h2>42. Fil Électrique</h2><p>Glisse sans toucher le fil sous tension.</p></div></div><div class="card-bot"><span>🎯 Précision</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_slice" class="game-card" data-title="perfect slice"><div class="card-top"><div class="card-icon">✂️</div><div class="card-info"><h2>43. Perfect Slice</h2><p>Coupe le bloc au centre parfait (50/50).</p></div></div><div class="card-bot"><span>🎯 Précision</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_coinjar" class="game-card" data-title="coin jar"><div class="card-top"><div class="card-icon">🪙</div><div class="card-info"><h2>44. Coin in the Jar</h2><p>Lâche la pièce pour la loger dans le bocal.</p></div></div><div class="card-bot"><span>🎯 Adresse</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_hook" class="game-card" data-title="grappin harpoon"><div class="card-top"><div class="card-icon">🪝</div><div class="card-info"><h2>45. Grappin Trésor</h2><p>Lance le harpon pour hisser les diamants.</p></div></div><div class="card-bot"><span>🎯 Adresse</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_balance" class="game-card" data-title="balance marble"><div class="card-top"><div class="card-icon">⚖️</div><div class="card-info"><h2>46. Balance Marble</h2><p>Garde la bille en équilibre sur la barre.</p></div></div><div class="card-bot"><span>🎯 Adresse</span><span class="play-pill">Jouer ▶</span></div></a>

      <!-- 47-50 PACK RETRO -->
      <a href="/game_tetris" class="game-card" data-title="tetris"><div class="card-top"><div class="card-icon">🧱</div><div class="card-info"><h2>47. Cyber Tetris</h2><p>Emboîte les formes et efface les lignes.</p></div></div><div class="card-bot"><span>👾 Rétro</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_pacmaze" class="game-card" data-title="pac maze"><div class="card-top"><div class="card-icon">🟡</div><div class="card-info"><h2>48. Cyber Pac-Maze</h2><p>Mange les orbes et fuis les fantômes.</p></div></div><div class="card-bot"><span>👾 Rétro</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_lunar" class="game-card" data-title="lunar lander"><div class="card-top"><div class="card-icon">🚀</div><div class="card-info"><h2>49. Lunar Lander</h2><p>Dose les rétro-fusées pour alunir en douceur.</p></div></div><div class="card-bot"><span>👾 Rétro</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/game_cannon" class="game-card" data-title="cannon blaster"><div class="card-top"><div class="card-icon">💣</div><div class="card-info"><h2>50. Cannon Blaster</h2><p>Tire au canon pour détruire les châteaux.</p></div></div><div class="card-bot"><span>👾 Rétro</span><span class="play-pill">Jouer ▶</span></div></a>

      <!-- SECTION 15 JEUX DUEL VS IA -->
      <div style="grid-column: 1 / -1; margin: 16px 0 6px 0; border-top: 1px solid rgba(255,255,255,0.1); padding-top: 16px;">
        <h2 style="font-size: 1.25rem; color: #a855f7; display: flex; align-items: center; gap: 8px;">
          🤖 Les 15 Jeux de Duel vs IA (Entraînement Solo)
        </h2>
        <p style="font-size: 0.85rem; color: var(--dim); margin-top: 4px;">
          Défiez une intelligence artificielle réactive sur les 15 jeux multijoueurs !
        </p>
      </div>

      <!-- 5 Réflexion vs IA -->
      <a href="/mp_connect4?vs=ai" class="game-card" data-title="puissance 4 ia duel"><div class="card-top"><div class="card-icon" style="color:#ffe600;border-color:#ffe600;">🔴</div><div class="card-info"><h2>Puissance 4 vs IA</h2><p>Alignez 4 jetons contre l'IA stratégique.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_naval?vs=ai" class="game-card" data-title="bataille navale ia duel"><div class="card-top"><div class="card-icon" style="color:#0088ff;border-color:#0088ff;">🚢</div><div class="card-info"><h2>Bataille Navale vs IA</h2><p>Détruisez la flotte secrète de l'ordinateur.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_tictactoe?vs=ai" class="game-card" data-title="morpion cyber ia duel"><div class="card-top"><div class="card-icon" style="color:#ff0055;border-color:#ff0055;">❌</div><div class="card-info"><h2>Morpion Cyber vs IA</h2><p>Croix vs Rond contre le bot intelligent.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_memory?vs=ai" class="game-card" data-title="duel memoire ia"><div class="card-top"><div class="card-icon" style="color:#a855f7;border-color:#a855f7;">🃏</div><div class="card-info"><h2>Duel de Mémoire vs IA</h2><p>Trouvez plus de paires que l'IA.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_mastermind?vs=ai" class="game-card" data-title="mastermind duel ia"><div class="card-top"><div class="card-icon" style="color:#00f3ff;border-color:#00f3ff;">🔢</div><div class="card-info"><h2>Mastermind vs IA</h2><p>Décodez les 4 couleurs avant l'IA.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>

      <!-- 5 Réflexe vs IA -->
      <a href="/mp_pong?vs=ai" class="game-card" data-title="pong laser ia duel"><div class="card-top"><div class="card-icon" style="color:#00f3ff;border-color:#00f3ff;">🏓</div><div class="card-info"><h2>Pong Laser vs IA</h2><p>Tennis de table laser face au bot.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_airhockey?vs=ai" class="game-card" data-title="air hockey ia duel"><div class="card-top"><div class="card-icon" style="color:#ff0055;border-color:#ff0055;">🏒</div><div class="card-info"><h2>Air Hockey vs IA</h2><p>Frapper le palet contre le maillet IA.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_tank?vs=ai" class="game-card" data-title="tank battle 2d ia duel"><div class="card-top"><div class="card-icon" style="color:#00ff66;border-color:#00ff66;">🛡️</div><div class="card-info"><h2>Tank Battle vs IA</h2><p>Détruisez le blindé ennemi autonome.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_reflex?vs=ai" class="game-card" data-title="duel reflexes ia"><div class="card-top"><div class="card-icon" style="color:#ffe600;border-color:#ffe600;">⚡</div><div class="card-info"><h2>Duel de Réflexes vs IA</h2><p>Dégainez plus vite que le processeur en ms.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_tapduel?vs=ai" class="game-card" data-title="tir a la corde tap duel ia"><div class="card-top"><div class="card-icon" style="color:#ff5500;border-color:#ff5500;">🪢</div><div class="card-info"><h2>Tir à la Corde vs IA</h2><p>Duel de clics frénétiques contre l'IA.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>

      <!-- 5 Mots & Baccalauréat vs IA -->
      <a href="/mp_petitbac?vs=ai" class="game-card" data-title="le petit bac ia duel"><div class="card-top"><div class="card-icon" style="color:#00f3ff;border-color:#00f3ff;">📝</div><div class="card-info"><h2>Le Petit Bac vs IA</h2><p>Remplissez les 4 catégories avant le bot.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_anagram?vs=ai" class="game-card" data-title="anagrammes ia duel"><div class="card-top"><div class="card-icon" style="color:#ffe600;border-color:#ffe600;">🔤</div><div class="card-info"><h2>Anagrammes vs IA</h2><p>Trouvez le mot mélangé plus vite que l'IA.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_wordbomb?vs=ai" class="game-card" data-title="bombe mots ia duel"><div class="card-top"><div class="card-icon" style="color:#ff0055;border-color:#ff0055;">💣</div><div class="card-info"><h2>Bombe à Mots vs IA</h2><p>Passez la bombe avant l'explosion !</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_wordchain?vs=ai" class="game-card" data-title="chaine mots shiritori ia duel"><div class="card-top"><div class="card-icon" style="color:#00ff66;border-color:#00ff66;">🔗</div><div class="card-info"><h2>Chaîne de Mots vs IA</h2><p>Enchaînez les mots avec la dernière lettre.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>
      <a href="/mp_hangman?vs=ai" class="game-card" data-title="pendu duel ia"><div class="card-top"><div class="card-icon" style="color:#a855f7;border-color:#a855f7;">🪢</div><div class="card-info"><h2>Pendu Duel vs IA</h2><p>Devinez le mot secret avant 6 erreurs.</p></div></div><div class="card-bot"><span style="color:#a855f7;">🤖 Duel IA</span><span class="play-pill">Jouer ▶</span></div></a>

    </div>

    <!-- VUE 2 : SALON MULTIJOUEUR AVEC DÉFIS -->
    <div id="mp-container" style="display:none;">
      <div class="lobby-box">
        <div class="lobby-header">
          <h2><span class="pulse-dot"></span> Joueurs Connectés dans le Salon (<span id="player-count">0</span>)</h2>
          <span style="font-size:0.75rem; color:#00ff66; font-weight:700;">🟢 En Direct</span>
        </div>

        <div class="players-list" id="players-list">
          <div style="color:var(--dim); font-size:0.88rem; text-align:center; padding:16px;">
            ⏳ Connexion au salon...
          </div>
        </div>
      </div>

      <!-- CATALOGUE DES JEUX DISPONIBLES EN MULTIJOUEUR -->
      <div style="margin-top:14px; display:flex; flex-direction:column; gap:16px;">

        <!-- SALLE DE GROUPE : 3 À 8 JOUEURS -->
        <div style="background:linear-gradient(145deg, rgba(168,85,247,0.18), rgba(0,243,255,0.12)); border:2px solid var(--purple); border-radius:18px; padding:16px; box-shadow: 0 0 25px rgba(168,85,247,0.25);">
          <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:8px;">
            <h3 style="font-size:1.1rem; color:#fff; display:flex; align-items:center; gap:8px;">
              👥 SALLE DE GROUPE MULTIJOUEUR (3 À 8 JOUEURS)
            </h3>
            <span style="background:var(--purple); color:#fff; font-size:0.75rem; font-weight:900; padding:3px 10px; border-radius:12px;">3-8 JOUEURS</span>
          </div>
          <p style="font-size:0.85rem; color:var(--dim); margin-bottom:12px;">
            Jouez tous ensemble sur vos smartphones connectés au Wi-Fi de l'ESP32 !
          </p>
          <a href="/mp_undercover" class="game-card" style="border-color:var(--purple); background:rgba(18,22,38,0.95); text-decoration:none;">
            <div class="card-top">
              <div class="card-icon" style="color:var(--purple); border-color:var(--purple); font-size:1.8rem; background:rgba(168,85,247,0.2);">🕵️</div>
              <div class="card-info">
                <h2 style="color:#fff;">🕵️ Undercover : Déduction & Bluff</h2>
                <p>Civils vs Undercovers vs Mr. White. Rôles secrets, votes en direct et devinettes !</p>
              </div>
            </div>
            <div class="card-bot">
              <span style="color:var(--purple); font-weight:800;">👥 3 à 8 Joueurs Wi-Fi</span>
              <span class="play-pill" style="background:var(--purple); color:#fff;">Rejoindre la Salle ▶</span>
            </div>
          </a>
        </div>

        <!-- PACK 1: REFLEXION -->
        <div>
          <h3 style="font-size:1.05rem; color:#00f3ff; margin-bottom:10px; display:flex; align-items:center; gap:8px;">
            🧠 5 JEUX DE RÉFLEXION & STRATÉGIE (1v1)
          </h3>
          <div class="grid">
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#ffe600;border-color:#ffe600;">🔴</div><div class="card-info"><h2>1. Puissance 4</h2><p>Alignez 4 jetons sur grille 7x6.</p></div></div><div class="card-bot"><span style="color:#ffe600;">1v1 Réseau</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#0088ff;border-color:#0088ff;">🚢</div><div class="card-info"><h2>2. Bataille Navale</h2><p>Grille tactique et tirs radar.</p></div></div><div class="card-bot"><span style="color:#0088ff;">1v1 Stratégie</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#ff0055;border-color:#ff0055;">❌</div><div class="card-info"><h2>3. Morpion Cyber</h2><p>Duel Croix vs Rond en 3x3.</p></div></div><div class="card-bot"><span style="color:#ff0055;">1v1 Tactique</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#a855f7;border-color:#a855f7;">🃏</div><div class="card-info"><h2>4. Duel de Mémoire</h2><p>16 cartes retournées au tour par tour.</p></div></div><div class="card-bot"><span style="color:#a855f7;">1v1 Mémoire</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#00f3ff;border-color:#00f3ff;">🔢</div><div class="card-info"><h2>5. Mastermind Duel</h2><p>Décodez le code secret de 4 couleurs.</p></div></div><div class="card-bot"><span style="color:#00f3ff;">1v1 Déduction</span></div></div>
          </div>
        </div>

        <!-- PACK 2: REFLEXE -->
        <div>
          <h3 style="font-size:1.05rem; color:#ffaa00; margin-bottom:10px; display:flex; align-items:center; gap:8px;">
            ⚡ 5 JEUX DE RÉFLEXE & ACTION
          </h3>
          <div class="grid">
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#00f3ff;border-color:#00f3ff;">🏓</div><div class="card-info"><h2>6. Pong 1v1 Laser</h2><p>Raquettes en direct à 60 FPS.</p></div></div><div class="card-bot"><span style="color:#00f3ff;">1v1 Temps Réel</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#ff0055;border-color:#ff0055;">🏒</div><div class="card-info"><h2>7. Air Hockey Arcade</h2><p>Palet dynamique et poussoirs 3D.</p></div></div><div class="card-bot"><span style="color:#ff0055;">1v1 Physique</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#00ff66;border-color:#00ff66;">🛡️</div><div class="card-info"><h2>8. Tank Battle 2D</h2><p>Combat de blindés et tirs à rebonds.</p></div></div><div class="card-bot"><span style="color:#00ff66;">1v1 Combat</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#ffe600;border-color:#ffe600;">⚡</div><div class="card-info"><h2>9. Duel de Réflexes</h2><p>Qui dégaine le plus vite en ms ?</p></div></div><div class="card-bot"><span style="color:#ffe600;">1v1 Vitesse</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#ff5500;border-color:#ff5500;">🪢</div><div class="card-info"><h2>10. Tir à la Corde</h2><p>Duel de taps intenses et jauge d'énergie.</p></div></div><div class="card-bot"><span style="color:#ff5500;">1v1 Cadence</span></div></div>
          </div>
        </div>

        <!-- PACK 3: BACCALAUREAT & MOTS -->
        <div>
          <h3 style="font-size:1.05rem; color:#00ff66; margin-bottom:10px; display:flex; align-items:center; gap:8px;">
            📝 5 JEUX DE BACCALAURÉAT & MOTS
          </h3>
          <div class="grid">
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#00f3ff;border-color:#00f3ff;">📝</div><div class="card-info"><h2>11. Le Petit Bac</h2><p>4 catégories avec lettre imposée & stop.</p></div></div><div class="card-bot"><span style="color:#00f3ff;">1v1 Baccalauréat</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#ffe600;border-color:#ffe600;">🔤</div><div class="card-info"><h2>12. Anagrammes Flash</h2><p>Course pour décoder le mot mélangé.</p></div></div><div class="card-bot"><span style="color:#ffe600;">1v1 Vocabulaire</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#ff0055;border-color:#ff0055;">💣</div><div class="card-info"><h2>13. La Bombe à Mots</h2><p>Passez la bombe avant l'explosion !</p></div></div><div class="card-bot"><span style="color:#ff0055;">1v1 Tic-Tac</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#00ff66;border-color:#00ff66;">🔗</div><div class="card-info"><h2>14. Chaîne de Mots</h2><p>Enchaînez avec la dernière lettre.</p></div></div><div class="card-bot"><span style="color:#00ff66;">1v1 Shiritori</span></div></div>
            <div class="game-card"><div class="card-top"><div class="card-icon" style="color:#a855f7;border-color:#a855f7;">🪢</div><div class="card-info"><h2>15. Le Pendu Duel</h2><p>Choisis le mot secret pour piéger l'autre.</p></div></div><div class="card-bot"><span style="color:#a855f7;">1v1 Lettres</span></div></div>
          </div>
        </div>

      </div>
    </div>
  </main>

  <!-- MODAL : CHOIX DU JEU POUR LE DÉFI -->
  <div class="modal-backdrop" id="challenge-modal">
    <div class="modal-box">
      <h3>⚔️ Défier <span id="target-player-name" style="color:#fff;"></span></h3>
      <p style="font-size:0.85rem; color:var(--dim);">Choisis le jeu pour lancer la partie :</p>
      <div class="game-pick-list">
        <div style="font-size:0.75rem; color:#00f3ff; font-weight:800; text-align:left; padding:4px 0;">🧠 RÉFLEXION (5 JEUX)</div>
        <button class="btn-pick-game" onclick="sendChallenge('c4')"><span>🔴 Puissance 4 1v1</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('naval')"><span>🚢 Bataille Navale 1v1</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('ttt')"><span>❌ Morpion Cyber 1v1</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('memory')"><span>🃏 Duel de Mémoire</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('mastermind')"><span>🔢 Mastermind Duel</span><span>▶</span></button>

        <div style="font-size:0.75rem; color:#ffaa00; font-weight:800; text-align:left; padding:8px 0 4px 0;">⚡ RÉFLEXE (5 JEUX)</div>
        <button class="btn-pick-game" onclick="sendChallenge('pong')"><span>🏓 Pong 1v1 Laser</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('hockey')"><span>🏒 Air Hockey Arcade</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('tank')"><span>🛡️ Tank Battle 2D</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('reflex')"><span>⚡ Duel de Réflexes</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('tapduel')"><span>🪢 Tir à la Corde (Tap)</span><span>▶</span></button>

        <div style="font-size:0.75rem; color:#00ff66; font-weight:800; text-align:left; padding:8px 0 4px 0;">📝 BACCALAURÉAT & MOTS (5 JEUX)</div>
        <button class="btn-pick-game" onclick="sendChallenge('petitbac')"><span>📝 Le Petit Bac</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('anagram')"><span>🔤 Anagrammes 1v1</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('wordbomb')"><span>💣 Bombe à Mots</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('wordchain')"><span>🔗 Chaîne de Mots</span><span>▶</span></button>
        <button class="btn-pick-game" onclick="sendChallenge('hangman')"><span>🪢 Pendu Duel</span><span>▶</span></button>
      </div>
      <button class="btn-cancel" onclick="closeChallengeModal()">Annuler</button>
    </div>
  </div>

  <!-- MODAL : DÉFI ENVOYÉ (ATTENTE) -->
  <div class="modal-backdrop" id="waiting-modal">
    <div class="modal-box">
      <h3>⏳ Défi Envoyé !</h3>
      <p style="font-size:0.95rem; color:#fff;" id="waiting-msg">En attente de la réponse...</p>
      <button class="btn-cancel" onclick="closeWaitingModal()">Annuler le défi</button>
    </div>
  </div>

  <!-- MODAL : INVITATION REÇUE D'UN AUTRE JOUEUR -->
  <div class="modal-backdrop" id="invite-modal">
    <div class="modal-box">
      <h3 style="color:var(--pink);">⚔️ DÉFI REÇU !</h3>
      <p style="font-size:1.1rem; font-weight:800; color:#fff;" id="invite-from-msg">Joueur vous défie !</p>
      <p style="font-size:0.9rem; color:var(--yellow);" id="invite-game-msg">Jeu : Pong 1v1</p>
      <div class="invite-actions">
        <button class="btn-accept" onclick="acceptIncomingChallenge()">✅ ACCEPTER</button>
        <button class="btn-decline" onclick="declineIncomingChallenge()">❌ REFUSER</button>
      </div>
    </div>
  </div>

  <footer>
    ESP32 Web Arcade • 50 Jeux Solo + 15 Jeux Multijoueur 1v1 • Flash PROGMEM
  </footer>

  <script>
    let currentMode = 'solo';
    let myPseudo = localStorage.getItem('arcade_pseudo');
    if (!myPseudo) {
      myPseudo = 'Joueur_' + Math.floor(100 + Math.random() * 900);
    }

    function setMode(mode) {
      currentMode = mode;
      document.getElementById('tab-solo').classList.toggle('active', mode === 'solo');
      document.getElementById('tab-mp').classList.toggle('active', mode === 'mp');
      document.getElementById('game-grid').style.display = (mode === 'solo') ? 'grid' : 'none';
      document.getElementById('mp-container').style.display = (mode === 'mp') ? 'flex' : 'none';
      
      const search = document.getElementById('search');
      search.style.display = (mode === 'solo') ? 'block' : 'none';
      if (mode === 'mp') initLobbyWs();
    }

    function filterGames() {
      const q = document.getElementById('search').value.toLowerCase().trim();
      document.querySelectorAll('#game-grid .game-card').forEach(card => {
        const title = card.getAttribute('data-title') || '';
        card.style.display = (title.includes(q) || q === '') ? 'flex' : 'none';
      });
    }

    // --- WEBSOCKET LOBBY ENGINE ---
    let ws = null;
    let myId = -1;
    let targetPlayerId = -1;
    let incomingChallenge = null;

    function initLobbyWs() {
      if (ws && (ws.readyState === WebSocket.OPEN || ws.readyState === WebSocket.CONNECTING)) return;
      ws = new WebSocket('ws://' + location.hostname + ':81/');

      ws.onopen = () => {
        ws.send(JSON.stringify({ t: "hello", name: myPseudo }));
      };

      ws.onmessage = (e) => {
        try {
          const d = JSON.parse(e.data);
          if (d.t === "welcome") {
            myId = d.my_id;
          } else if (d.t === "players") {
            renderPlayerList(d.list);
          } else if (d.t === "challenged") {
            incomingChallenge = d;
            showIncomingInvite(d);
          } else if (d.t === "declined") {
            closeWaitingModal();
            alert("❌ " + d.by + " a refusé le défi.");
          } else if (d.t === "launch_match") {
            closeWaitingModal();
            closeChallengeModal();
            // Lancement du jeu jumelé !
            const gameUrls = {
              "c4": "/mp_connect4", "naval": "/mp_naval", "ttt": "/mp_tictactoe",
              "memory": "/mp_memory", "mastermind": "/mp_mastermind",
              "pong": "/mp_pong", "hockey": "/mp_airhockey", "tank": "/mp_tank",
              "reflex": "/mp_reflex", "tapduel": "/mp_tapduel",
              "petitbac": "/mp_petitbac", "anagram": "/mp_anagram",
              "wordbomb": "/mp_wordbomb", "wordchain": "/mp_wordchain",
              "hangman": "/mp_hangman"
            };
            const url = gameUrls[d.game] || "/mp_pong";
            window.location.href = url;
          }
        } catch(err){}
      };

      ws.onclose = () => {
        // Reconnexion automatique si dans l'onglet MP
        if (currentMode === 'mp') setTimeout(initLobbyWs, 2000);
      };
    }

    function renderPlayerList(list) {
      const el = document.getElementById('players-list');
      document.getElementById('player-count').textContent = list.length;
      if (!list || list.length === 0) {
        el.innerHTML = '<div style="color:var(--dim); font-size:0.88rem; text-align:center; padding:16px;">⏳ Aucun autre joueur pour l\'instant. Connecte un autre smartphone !</div>';
        return;
      }
      let html = '';
      list.forEach(p => {
        const isYou = (p.id === myId || p.name === myPseudo);
        html += `<div class="player-row ${isYou ? 'you' : ''}">`;
        html += `  <div class="player-info">`;
        html += `    <span style="font-size:1.4rem;">${isYou ? '👑' : '🎮'}</span>`;
        html += `    <div><div class="player-name">${p.name} ${isYou ? '<span style="color:var(--cyan);font-size:0.8rem;">(Vous)</span>' : ''}</div>`;
        html += `    <div class="player-status">${p.status === 'in_game' ? '⚔️ En plein match' : '🟢 Disponible'}</div></div>`;
        html += `  </div>`;
        if (!isYou) {
          if (p.status !== 'in_game') {
            html += `<button class="btn-defier" onclick="openChallengeModal(${p.id}, '${p.name}')">⚔️ Défier</button>`;
          } else {
            html += `<span style="font-size:0.8rem; color:var(--pink); font-weight:700;">Occupé</span>`;
          }
        }
        html += `</div>`;
      });
      el.innerHTML = html;
    }

    function openChallengeModal(id, name) {
      targetPlayerId = id;
      document.getElementById('target-player-name').textContent = name;
      document.getElementById('challenge-modal').style.display = 'flex';
    }
    function closeChallengeModal() {
      document.getElementById('challenge-modal').style.display = 'none';
    }

    function sendChallenge(gameKey) {
      closeChallengeModal();
      document.getElementById('waiting-msg').textContent = "Invitation envoyée... En attente de la réponse...";
      document.getElementById('waiting-modal').style.display = 'flex';
      if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({ t: "challenge", to: targetPlayerId, game: gameKey }));
      }
    }
    function closeWaitingModal() {
      document.getElementById('waiting-modal').style.display = 'none';
    }

    function showIncomingInvite(d) {
      const names = {
        "c4": "🔴 Puissance 4 1v1", "naval": "🚢 Bataille Navale 1v1", "ttt": "❌ Morpion Cyber 1v1",
        "memory": "🃏 Duel de Mémoire", "mastermind": "🔢 Mastermind Duel",
        "pong": "🏓 Pong 1v1", "hockey": "🏒 Air Hockey 1v1", "tank": "🛡️ Tank Battle 1v1",
        "reflex": "⚡ Duel de Réflexes", "tapduel": "🪢 Tir à la Corde (Tap)",
        "petitbac": "📝 Le Petit Bac", "anagram": "🔤 Anagrammes 1v1",
        "wordbomb": "💣 Bombe à Mots", "wordchain": "🔗 Chaîne de Mots",
        "hangman": "🪢 Pendu Duel"
      };
      document.getElementById('invite-from-msg').textContent = `⚔️ ${d.from_name} vous défie !`;
      document.getElementById('invite-game-msg').textContent = `Jeu choisi : ${names[d.game] || d.game}`;
      document.getElementById('invite-modal').style.display = 'flex';
      if (navigator.vibrate) navigator.vibrate([100, 50, 100]);
    }

    function acceptIncomingChallenge() {
      document.getElementById('invite-modal').style.display = 'none';
      if (incomingChallenge && ws && ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({ t: "accept_challenge", from: incomingChallenge.from, game: incomingChallenge.game }));
      }
    }

    function declineIncomingChallenge() {
      document.getElementById('invite-modal').style.display = 'none';
      if (incomingChallenge && ws && ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({ t: "decline_challenge", from: incomingChallenge.from }));
      }
      incomingChallenge = null;
    }
  </script>
</body>
</html>
)rawliteral";

#endif
