#ifndef GAME_MOTSCROISES_H
#define GAME_MOTSCROISES_H

#include <pgmspace.h>

const char GAME_MOTSCROISES_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Mots Croisés Cyber - 100 Niveaux</title>
<style>
  :root {
    --bg: #090b12;
    --card: #11162a;
    --card-border: rgba(0, 243, 255, 0.25);
    --cyan: #00f3ff;
    --pink: #ff0055;
    --yellow: #ffe600;
    --green: #00ff66;
    --purple: #a855f7;
    --dim: #8b9bb4;
  }
  * { box-sizing: border-box; margin: 0; padding: 0; -webkit-tap-highlight-color: transparent; }
  body {
    background: var(--bg);
    color: #f0f4fc;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
    display: flex;
    flex-direction: column;
    align-items: center;
    min-height: 100vh;
    padding: 10px 10px 30px;
    touch-action: manipulation;
  }
  header {
    width: 100%;
    max-width: 440px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 8px;
    gap: 6px;
  }
  .btn-back {
    color: var(--cyan);
    text-decoration: none;
    font-weight: 700;
    font-size: 0.88rem;
    padding: 6px 10px;
    border-radius: 8px;
    border: 1px solid rgba(0,243,255,0.3);
    background: rgba(0,243,255,0.08);
    white-space: nowrap;
  }
  .lvl-nav-group {
    display: flex;
    align-items: center;
    gap: 4px;
  }
  .btn-lvl-step {
    background: var(--card);
    border: 1px solid var(--card-border);
    color: var(--cyan);
    font-weight: bold;
    font-size: 0.9rem;
    width: 32px;
    height: 32px;
    border-radius: 8px;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
  }
  .btn-lvl-step:active { transform: scale(0.92); }
  .btn-lvl-modal {
    background: linear-gradient(135deg, rgba(20,26,48,0.9), rgba(11,15,28,0.9));
    border: 1px solid var(--cyan);
    color: #fff;
    padding: 6px 10px;
    border-radius: 8px;
    font-size: 0.85rem;
    font-weight: 800;
    cursor: pointer;
    display: flex;
    align-items: center;
    gap: 4px;
    box-shadow: 0 0 10px rgba(0,243,255,0.25);
  }
  .timer-pill {
    background: rgba(255,230,0,0.1);
    border: 1px solid rgba(255,230,0,0.3);
    color: var(--yellow);
    padding: 6px 8px;
    border-radius: 8px;
    font-family: monospace;
    font-size: 0.92rem;
    font-weight: bold;
    white-space: nowrap;
  }
  .clue-banner {
    width: 100%;
    max-width: 440px;
    background: linear-gradient(135deg, rgba(20,26,48,0.95), rgba(11,15,28,0.95));
    border: 1px solid var(--card-border);
    border-left: 4px solid var(--cyan);
    border-radius: 10px;
    padding: 8px 12px;
    margin-bottom: 8px;
    min-height: 48px;
    display: flex;
    flex-direction: column;
    justify-content: center;
    cursor: pointer;
    box-shadow: 0 4px 15px rgba(0,0,0,0.4);
  }
  .clue-meta {
    font-size: 0.72rem;
    color: var(--cyan);
    text-transform: uppercase;
    font-weight: 800;
    letter-spacing: 0.5px;
    margin-bottom: 2px;
  }
  .clue-text {
    font-size: 0.92rem;
    color: #fff;
    font-weight: 600;
    line-height: 1.25;
  }
  .board-container {
    width: 100%;
    max-width: 360px;
    background: rgba(14, 18, 34, 0.8);
    border: 1px solid rgba(0, 243, 255, 0.2);
    border-radius: 16px;
    padding: 10px;
    box-shadow: 0 8px 30px rgba(0,0,0,0.6);
    margin-bottom: 8px;
  }
  .cw-grid {
    display: grid;
    gap: 5px;
    width: 100%;
  }
  .cw-cell {
    aspect-ratio: 1;
    background: #151b30;
    border: 1px solid rgba(0, 243, 255, 0.25);
    border-radius: 8px;
    position: relative;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 1.35rem;
    font-weight: 900;
    color: #fff;
    cursor: pointer;
    user-select: none;
    transition: all 0.12s ease;
  }
  .cw-cell.black {
    background: #080a12;
    border-color: rgba(255, 255, 255, 0.03);
    cursor: default;
  }
  .cw-cell .cell-num {
    position: absolute;
    top: 2px;
    left: 4px;
    font-size: 0.62rem;
    font-weight: 700;
    color: var(--dim);
    line-height: 1;
    pointer-events: none;
  }
  .cw-cell.in-word {
    background: rgba(0, 243, 255, 0.15);
    border-color: rgba(0, 243, 255, 0.6);
  }
  .cw-cell.active-focus {
    background: rgba(255, 230, 0, 0.25) !important;
    border-color: var(--yellow) !important;
    box-shadow: 0 0 14px rgba(255, 230, 0, 0.5);
    transform: scale(1.02);
    z-index: 2;
  }
  .cw-cell.cell-correct {
    border-color: var(--green) !important;
    color: var(--green);
  }
  .cw-cell.cell-wrong {
    border-color: var(--pink) !important;
    color: var(--pink);
  }

  .action-bar {
    width: 100%;
    max-width: 440px;
    display: flex;
    gap: 8px;
    margin-bottom: 8px;
  }
  .btn-act {
    flex: 1;
    padding: 8px 6px;
    border-radius: 10px;
    font-size: 0.82rem;
    font-weight: 700;
    border: 1px solid rgba(255,255,255,0.15);
    background: rgba(20, 26, 48, 0.9);
    color: #fff;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 4px;
    transition: all 0.15s;
  }
  .btn-act:active { transform: scale(0.96); }
  .btn-check { border-color: var(--cyan); color: var(--cyan); }
  .btn-hint { border-color: var(--yellow); color: var(--yellow); }
  .btn-next { border-color: #a855f7; color: #c084fc; }

  /* Virtual Keyboard */
  .v-kb {
    width: 100%;
    max-width: 440px;
    display: flex;
    flex-direction: column;
    gap: 4px;
    margin-bottom: 10px;
    user-select: none;
  }
  .kb-row {
    display: flex;
    gap: 4px;
    justify-content: center;
    width: 100%;
  }
  .kb-key {
    flex: 1;
    max-width: 38px;
    height: 38px;
    background: #171d33;
    border: 1px solid rgba(255,255,255,0.12);
    border-radius: 6px;
    color: #fff;
    font-size: 0.95rem;
    font-weight: 800;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    box-shadow: 0 2px 4px rgba(0,0,0,0.4);
    transition: background 0.1s;
  }
  .kb-key:active {
    background: var(--cyan);
    color: #000;
    transform: scale(0.95);
  }
  .kb-key.wide {
    flex: 1.5;
    max-width: 58px;
    font-size: 0.8rem;
    background: #202742;
  }

  /* Clues Container */
  .clues-wrapper {
    width: 100%;
    max-width: 440px;
    background: var(--card);
    border: 1px solid var(--card-border);
    border-radius: 12px;
    padding: 8px 12px;
  }
  .clues-nav {
    display: flex;
    gap: 8px;
    margin-bottom: 6px;
    border-bottom: 1px solid rgba(255,255,255,0.1);
    padding-bottom: 4px;
  }
  .clue-tab-btn {
    background: none;
    border: none;
    color: var(--dim);
    font-weight: 800;
    font-size: 0.82rem;
    padding: 4px 8px;
    cursor: pointer;
  }
  .clue-tab-btn.active {
    color: var(--cyan);
    border-bottom: 2px solid var(--cyan);
  }
  .clue-item {
    font-size: 0.8rem;
    padding: 5px 8px;
    border-radius: 6px;
    cursor: pointer;
    margin-bottom: 3px;
    display: flex;
    gap: 6px;
    color: #c5d1e8;
  }
  .clue-item.active {
    background: rgba(0, 243, 255, 0.15);
    color: #fff;
    font-weight: bold;
    border-left: 3px solid var(--cyan);
  }
  .clue-item.solved {
    text-decoration: line-through;
    opacity: 0.6;
  }

  /* Level Selector Modal (100 Levels) */
  .modal-overlay {
    display: none;
    position: fixed;
    top: 0; left: 0; right: 0; bottom: 0;
    background: rgba(4, 6, 12, 0.88);
    backdrop-filter: blur(6px);
    z-index: 100;
    align-items: center;
    justify-content: center;
    padding: 16px;
  }
  .modal-box {
    background: linear-gradient(145deg, #131a33, #0d1224);
    border: 2px solid var(--cyan);
    border-radius: 20px;
    padding: 20px 16px;
    max-width: 440px;
    width: 100%;
    max-height: 85vh;
    display: flex;
    flex-direction: column;
    box-shadow: 0 0 40px rgba(0,243,255,0.35);
  }
  .modal-head {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 12px;
    border-bottom: 1px solid rgba(255,255,255,0.1);
    padding-bottom: 8px;
  }
  .modal-head h2 {
    font-size: 1.15rem;
    color: var(--cyan);
    font-weight: 900;
  }
  .btn-close-modal {
    background: none;
    border: none;
    color: var(--dim);
    font-size: 1.5rem;
    cursor: pointer;
  }
  .levels-grid {
    display: grid;
    grid-template-columns: repeat(5, 1fr);
    gap: 6px;
    overflow-y: auto;
    padding-right: 4px;
    max-height: 60vh;
  }
  .lvl-btn {
    aspect-ratio: 1;
    background: #171d33;
    border: 1px solid rgba(255,255,255,0.15);
    border-radius: 10px;
    color: #fff;
    font-size: 0.85rem;
    font-weight: 800;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    position: relative;
    transition: all 0.15s;
  }
  .lvl-btn:active { transform: scale(0.95); }
  .lvl-btn.active {
    border-color: var(--yellow) !important;
    background: rgba(255, 230, 0, 0.2) !important;
    color: var(--yellow);
    box-shadow: 0 0 10px rgba(255,230,0,0.4);
  }
  .lvl-btn.completed {
    border-color: var(--green);
    background: rgba(0, 255, 102, 0.12);
  }
  .lvl-btn .star-mark {
    font-size: 0.65rem;
    color: var(--yellow);
    margin-top: 1px;
  }

  /* Victory Modal */
  .victory-box {
    background: linear-gradient(145deg, #131a33, #0d1224);
    border: 2px solid var(--green);
    border-radius: 20px;
    padding: 24px;
    max-width: 360px;
    width: 100%;
    text-align: center;
    box-shadow: 0 0 40px rgba(0,255,102,0.4);
  }
  .victory-title {
    font-size: 1.45rem;
    color: var(--yellow);
    font-weight: 900;
    margin-bottom: 8px;
    text-transform: uppercase;
  }
  .victory-text {
    color: var(--dim);
    font-size: 0.9rem;
    margin-bottom: 18px;
  }
  .victory-btn {
    width: 100%;
    padding: 12px;
    background: linear-gradient(135deg, var(--green), #00bbff);
    border: none;
    border-radius: 12px;
    color: #000;
    font-weight: 900;
    font-size: 1rem;
    cursor: pointer;
    box-shadow: 0 4px 15px rgba(0,255,102,0.4);
  }
</style>
<script src="/pause.js"></script>
</head>
<body>

<header>
  <a href="/hub" class="btn-back">◀ Menu</a>
  
  <div class="lvl-nav-group">
    <button class="btn-lvl-step" onclick="prevGrid()" title="Niveau précédent">◀</button>
    <button class="btn-lvl-modal" onclick="openLevelModal()" title="Choisir un niveau">
      🏆 <span id="cur-lvl-num">1</span>/100 ▾
    </button>
    <button class="btn-lvl-step" onclick="nextGrid()" title="Niveau suivant">▶</button>
  </div>

  <div class="timer-pill" id="timer">00:00</div>
</header>

<div class="clue-banner" id="clue-banner" onclick="toggleDirection()">
  <div class="clue-meta" id="clue-meta">1 HORIZONTAL (5 LETTRES)</div>
  <div class="clue-text" id="clue-text">Définition du mot...</div>
</div>

<div class="board-container">
  <div class="cw-grid" id="cw-grid"></div>
</div>

<div class="action-bar">
  <button class="btn-act btn-check" onclick="checkAnswers()">🔍 Vérifier</button>
  <button class="btn-act btn-hint" onclick="revealCurrentCell()">💡 Indice</button>
  <button class="btn-act btn-next" onclick="nextWord()">⏭️ Suivant</button>
</div>

<!-- Virtual Keyboard -->
<div class="v-kb">
  <div class="kb-row">
    <div class="kb-key" onclick="handleVirtualKey('A')">A</div>
    <div class="kb-key" onclick="handleVirtualKey('Z')">Z</div>
    <div class="kb-key" onclick="handleVirtualKey('E')">E</div>
    <div class="kb-key" onclick="handleVirtualKey('R')">R</div>
    <div class="kb-key" onclick="handleVirtualKey('T')">T</div>
    <div class="kb-key" onclick="handleVirtualKey('Y')">Y</div>
    <div class="kb-key" onclick="handleVirtualKey('U')">U</div>
    <div class="kb-key" onclick="handleVirtualKey('I')">I</div>
    <div class="kb-key" onclick="handleVirtualKey('O')">O</div>
    <div class="kb-key" onclick="handleVirtualKey('P')">P</div>
  </div>
  <div class="kb-row">
    <div class="kb-key" onclick="handleVirtualKey('Q')">Q</div>
    <div class="kb-key" onclick="handleVirtualKey('S')">S</div>
    <div class="kb-key" onclick="handleVirtualKey('D')">D</div>
    <div class="kb-key" onclick="handleVirtualKey('F')">F</div>
    <div class="kb-key" onclick="handleVirtualKey('G')">G</div>
    <div class="kb-key" onclick="handleVirtualKey('H')">H</div>
    <div class="kb-key" onclick="handleVirtualKey('J')">J</div>
    <div class="kb-key" onclick="handleVirtualKey('K')">K</div>
    <div class="kb-key" onclick="handleVirtualKey('L')">L</div>
    <div class="kb-key" onclick="handleVirtualKey('M')">M</div>
  </div>
  <div class="kb-row">
    <div class="kb-key wide" onclick="handleVirtualKey('DEL')">⌫</div>
    <div class="kb-key" onclick="handleVirtualKey('W')">W</div>
    <div class="kb-key" onclick="handleVirtualKey('X')">X</div>
    <div class="kb-key" onclick="handleVirtualKey('C')">C</div>
    <div class="kb-key" onclick="handleVirtualKey('V')">V</div>
    <div class="kb-key" onclick="handleVirtualKey('B')">B</div>
    <div class="kb-key" onclick="handleVirtualKey('N')">N</div>
    <div class="kb-key wide" onclick="toggleDirection()">🔄 H/V</div>
  </div>
</div>

<!-- Clues Accordion/Tabs -->
<div class="clues-wrapper">
  <div class="clues-nav">
    <button class="clue-tab-btn active" id="tab-across-btn" onclick="setClueTab('across')">➡️ Horizontales</button>
    <button class="clue-tab-btn" id="tab-down-btn" onclick="setClueTab('down')">⬇️ Verticales</button>
  </div>
  <div id="clues-list"></div>
</div>

<!-- Modal 100 Niveaux -->
<div class="modal-overlay" id="levels-modal">
  <div class="modal-box">
    <div class="modal-head">
      <h2>🏆 100 Niveaux de Mots Croisés</h2>
      <button class="btn-close-modal" onclick="closeLevelModal()">✕</button>
    </div>
    <div class="levels-grid" id="levels-grid"></div>
  </div>
</div>

<!-- Victory Modal -->
<div class="modal-overlay" id="victory-modal">
  <div class="victory-box">
    <div style="font-size:3rem;margin-bottom:6px;">🎉</div>
    <div class="victory-title">Niveau Complété !</div>
    <div class="victory-text" id="victory-stats">Bravo ! Niveau résolu avec succès.</div>
    <button class="victory-btn" onclick="nextGridAfterWin()">Niveau Suivant ⏭️</button>
  </div>
</div>

<script>
// Base de données complète des 100 niveaux
const GRIDS_RAW = [{"id": 1, "title": "1. Rétro Arcade", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "SONAR", "Appareil acoustique de localisation sous-marine"], [5, 4, 0, "TASSE", "Récipient à anse pour boisson chaude"]], "d": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [2, 0, 2, "BONUS", "Récompense ou points supplémentaires dans un jeu"], [3, 0, 4, "TERRE", "Notre planète bleue d'origine"]]}, {"id": 2, "title": "2. Cyber & Tech", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "SONAR", "Appareil acoustique de localisation sous-marine"], [5, 4, 0, "TESTE", "Met à l'épreuve pour vérifier"]], "d": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [2, 0, 2, "BONUS", "Récompense ou points supplémentaires dans un jeu"], [3, 0, 4, "TERRE", "Notre planète bleue d'origine"]]}, {"id": 3, "title": "3. Espace & Énergie", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [5, 4, 0, "TASSE", "Récipient à anse pour boisson chaude"]], "d": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [2, 0, 2, "BLOCS", "Gros morceaux solides de pierre ou de bois"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 4, "title": "4. Océan & Mystère", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [5, 4, 0, "TESTE", "Met à l'épreuve pour vérifier"]], "d": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [2, 0, 2, "BLOCS", "Gros morceaux solides de pierre ou de bois"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 5, "title": "5. Forêt & Aventure", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "OUEST", "Point cardinal où le soleil se couche"]], "d": [[1, 0, 0, "RADIO", "Diffusion d'émissions sonores par ondes hertziennes"], [2, 0, 2, "BALLE", "Sphère de jeu rebondissante"], [3, 0, 4, "TRAIT", "Ligne tracée au crayon ou caractéristique"]]}, {"id": 6, "title": "6. Damier Électronique", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "OCEAN", "Immense étendue d'eau salée"]], "d": [[1, 0, 0, "RADIO", "Diffusion d'émissions sonores par ondes hertziennes"], [2, 0, 2, "BALLE", "Sphère de jeu rebondissante"], [3, 0, 4, "TRAIN", "Suite de wagons tractés par une locomotive"]]}, {"id": 7, "title": "7. Vitesse & Circuit", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "BALLE", "Sphère de jeu rebondissante"], [3, 0, 4, "TRACE", "Empreinte laissée par le passage de quelqu'un"]]}, {"id": 8, "title": "8. Laboratoire Secret", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "BALLE", "Sphère de jeu rebondissante"], [3, 0, 4, "TRACE", "Empreinte laissée par le passage de quelqu'un"]]}, {"id": 9, "title": "9. Forteresse Médiévale", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "SCOUT", "Éclaireur dans la nature"], [5, 4, 0, "TASSE", "Récipient à anse pour boisson chaude"]], "d": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [2, 0, 2, "BLOCS", "Gros morceaux solides de pierre ou de bois"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 10, "title": "10. Galaxie Lointaine", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "SCOUT", "Éclaireur dans la nature"], [5, 4, 0, "TESTE", "Met à l'épreuve pour vérifier"]], "d": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [2, 0, 2, "BLOCS", "Gros morceaux solides de pierre ou de bois"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 11, "title": "11. Pixel Art", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "DAMES", "Jeu de pions sur damier noir et blanc"], [5, 4, 0, "OCEAN", "Immense étendue d'eau salée"]], "d": [[1, 0, 0, "RADIO", "Diffusion d'émissions sonores par ondes hertziennes"], [2, 0, 2, "BOMBE", "Engin explosif ou arme de jeu rétro"], [3, 0, 4, "TISON", "Morceau de bois qui brûle dans l'âtre"]]}, {"id": 12, "title": "12. Exploration Lunaire", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "DAMES", "Jeu de pions sur damier noir et blanc"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "BOMBE", "Engin explosif ou arme de jeu rétro"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 13, "title": "13. Alchimie Ancienne", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "DAMES", "Jeu de pions sur damier noir et blanc"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "BOMBE", "Engin explosif ou arme de jeu rétro"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 14, "title": "14. Mission Spatiale", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "DAMES", "Jeu de pions sur damier noir et blanc"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "BOMBE", "Engin explosif ou arme de jeu rétro"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 15, "title": "15. Profondeurs Marines", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "DAMES", "Jeu de pions sur damier noir et blanc"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "BOMBE", "Engin explosif ou arme de jeu rétro"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 16, "title": "16. Île Mystérieuse", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "POIDS", "Force de pesanteur mesurée en kilogrammes"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "BOITE", "Récipient fermé pour ranger des objets"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 17, "title": "17. Code Source", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "POIDS", "Force de pesanteur mesurée en kilogrammes"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "BOITE", "Récipient fermé pour ranger des objets"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 18, "title": "18. Réseau Cybernétique", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "POIDS", "Force de pesanteur mesurée en kilogrammes"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "BOITE", "Récipient fermé pour ranger des objets"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 19, "title": "19. Robotique Avancée", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "POIDS", "Force de pesanteur mesurée en kilogrammes"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "BOITE", "Récipient fermé pour ranger des objets"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 20, "title": "20. Constellation", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "POINT", "Marque minuscule ou unité gagnée dans un jeu"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "BOITE", "Récipient fermé pour ranger des objets"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 21, "title": "21. Temple Oublié", "rows": 5, "cols": 5, "a": [[1, 0, 0, "ROBOT", "Machine autonome programmable"], [4, 2, 0, "POINT", "Marque minuscule ou unité gagnée dans un jeu"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "BOITE", "Récipient fermé pour ranger des objets"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 22, "title": "22. Sanctuaire Zen", "rows": 5, "cols": 5, "a": [[1, 0, 0, "TASSE", "Récipient à anse pour boisson chaude"], [4, 2, 0, "MANGA", "Bande dessinée japonaise"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "TEMPS", "Durée mesurée par les secondes et les heures"], [2, 0, 2, "SINGE", "Primate agile grimpant dans les arbres"], [3, 0, 4, "ETAPE", "Phase d'un voyage ou palier d'un tour"]]}, {"id": 23, "title": "23. Chemin des Étoiles", "rows": 5, "cols": 5, "a": [[1, 0, 0, "TASSE", "Récipient à anse pour boisson chaude"], [4, 2, 0, "MANGA", "Bande dessinée japonaise"], [5, 4, 0, "SERIE", "Suite ordonnée d'éléments ou d'épisodes"]], "d": [[1, 0, 0, "TEMPS", "Durée mesurée par les secondes et les heures"], [2, 0, 2, "SONAR", "Appareil acoustique de localisation sous-marine"], [3, 0, 4, "ETAPE", "Phase d'un voyage ou palier d'un tour"]]}, {"id": 24, "title": "24. Jardin Botanique", "rows": 5, "cols": 5, "a": [[1, 0, 0, "TASSE", "Récipient à anse pour boisson chaude"], [4, 2, 0, "MANGA", "Bande dessinée japonaise"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "TEMPS", "Durée mesurée par les secondes et les heures"], [2, 0, 2, "SINGE", "Primate agile grimpant dans les arbres"], [3, 0, 4, "ETAPE", "Phase d'un voyage ou palier d'un tour"]]}, {"id": 25, "title": "25. Base Polaire", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [5, 4, 0, "OUTIL", "Ustensile manuel servant à travailler"]], "d": [[1, 0, 0, "RESTO", "Lieu convivial où l'on partage un repas"], [2, 0, 2, "SCOUT", "Éclaireur dans la nature"], [3, 0, 4, "TOTAL", "Montant cumulé de tous les éléments réunis"]]}, {"id": 26, "title": "26. Vaisseau Mère", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "CANAL", "Bande de fréquence de transmission radio"], [5, 4, 0, "ECRAN", "Surface vitrée diffusant images et jeux"]], "d": [[1, 0, 0, "RUCHE", "Maison des abeilles et du miel"], [2, 0, 2, "SONAR", "Appareil acoustique de localisation sous-marine"], [3, 0, 4, "TALON", "Partie arrière de la chaussure ou du pied"]]}, {"id": 27, "title": "27. Planète Sauvage", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "CANAL", "Bande de fréquence de transmission radio"], [5, 4, 0, "ECRAN", "Surface vitrée diffusant images et jeux"]], "d": [[1, 0, 0, "ROCHE", "Masse minérale solide formant les montagnes"], [2, 0, 2, "SONAR", "Appareil acoustique de localisation sous-marine"], [3, 0, 4, "TALON", "Partie arrière de la chaussure ou du pied"]]}, {"id": 28, "title": "28. Cité Futuriste", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "PIONS", "Pièces déplacées sur un damier"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SCORE", "Total de points enregistrés dans un jeu"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 29, "title": "29. Royaume des Brumes", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "PIONS", "Pièces déplacées sur un damier"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SCORE", "Total de points enregistrés dans un jeu"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 30, "title": "30. Énigme d'Orient", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "PIONS", "Pièces déplacées sur un damier"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SCORE", "Total de points enregistrés dans un jeu"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 31, "title": "31. Puits Quantique", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "PIONS", "Pièces déplacées sur un damier"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SCORE", "Total de points enregistrés dans un jeu"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 32, "title": "32. Oasis Enchantée", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "OUEST", "Point cardinal où le soleil se couche"]], "d": [[1, 0, 0, "RADIO", "Diffusion d'émissions sonores par ondes hertziennes"], [2, 0, 2, "SALLE", "Pièce fermée d'un bâtiment"], [3, 0, 4, "TRAIT", "Ligne tracée au crayon ou caractéristique"]]}, {"id": 33, "title": "33. Monde Virtuel", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "OUEST", "Point cardinal où le soleil se couche"]], "d": [[1, 0, 0, "RADIO", "Diffusion d'émissions sonores par ondes hertziennes"], [2, 0, 2, "SOLDE", "Prix réduit ou montant restant"], [3, 0, 4, "TRAIT", "Ligne tracée au crayon ou caractéristique"]]}, {"id": 34, "title": "34. Dôme de Cristal", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "OCEAN", "Immense étendue d'eau salée"]], "d": [[1, 0, 0, "RADIO", "Diffusion d'émissions sonores par ondes hertziennes"], [2, 0, 2, "SALLE", "Pièce fermée d'un bâtiment"], [3, 0, 4, "TRAIN", "Suite de wagons tractés par une locomotive"]]}, {"id": 35, "title": "35. Machine à Vapeur", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "OCEAN", "Immense étendue d'eau salée"]], "d": [[1, 0, 0, "RADIO", "Diffusion d'émissions sonores par ondes hertziennes"], [2, 0, 2, "SOLDE", "Prix réduit ou montant restant"], [3, 0, 4, "TRAIN", "Suite de wagons tractés par une locomotive"]]}, {"id": 36, "title": "36. Soleil de Minuit", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "SALLE", "Pièce fermée d'un bâtiment"], [3, 0, 4, "TRACE", "Empreinte laissée par le passage de quelqu'un"]]}, {"id": 37, "title": "37. Canyon Écarlate", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "SOLDE", "Prix réduit ou montant restant"], [3, 0, 4, "TRACE", "Empreinte laissée par le passage de quelqu'un"]]}, {"id": 38, "title": "38. Signal Radio", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "SERIE", "Suite ordonnée d'éléments ou d'épisodes"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "SOLAR", "Relatif au soleil et à son rayonnement"], [3, 0, 4, "TRACE", "Empreinte laissée par le passage de quelqu'un"]]}, {"id": 39, "title": "39. Horizon Infini", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "SALLE", "Pièce fermée d'un bâtiment"], [3, 0, 4, "TRACE", "Empreinte laissée par le passage de quelqu'un"]]}, {"id": 40, "title": "40. Aurore Polaire", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "DELTA", "Embouchure de fleuve ou lettre grecque"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "RADIS", "Petit légume rouge croquant et piquant"], [2, 0, 2, "SOLDE", "Prix réduit ou montant restant"], [3, 0, 4, "TRACE", "Empreinte laissée par le passage de quelqu'un"]]}, {"id": 41, "title": "41. Hangar Spatial", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "COURS", "Leçon d'un professeur ou flux d'eau"], [5, 4, 0, "EPICE", "Substance aromatique pour la cuisine"]], "d": [[1, 0, 0, "RUCHE", "Maison des abeilles et du miel"], [2, 0, 2, "SOURI", "Petit rongeur agile"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 42, "title": "42. Vallée Silencieuse", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "COURS", "Leçon d'un professeur ou flux d'eau"], [5, 4, 0, "EPICE", "Substance aromatique pour la cuisine"]], "d": [[1, 0, 0, "RUCHE", "Maison des abeilles et du miel"], [2, 0, 2, "SOURI", "Petit rongeur agile"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 43, "title": "43. Labyrinthe d'Or", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "COURS", "Leçon d'un professeur ou flux d'eau"], [5, 4, 0, "EPICE", "Substance aromatique pour la cuisine"]], "d": [[1, 0, 0, "ROCHE", "Masse minérale solide formant les montagnes"], [2, 0, 2, "SOURI", "Petit rongeur agile"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 44, "title": "44. Récif de Corail", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "COURS", "Leçon d'un professeur ou flux d'eau"], [5, 4, 0, "EPICE", "Substance aromatique pour la cuisine"]], "d": [[1, 0, 0, "ROCHE", "Masse minérale solide formant les montagnes"], [2, 0, 2, "SOURI", "Petit rongeur agile"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 45, "title": "45. Station Orbitale", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "SMART", "Intelligent, élégant ou connecté"], [5, 4, 0, "OUTIL", "Ustensile manuel servant à travailler"]], "d": [[1, 0, 0, "RESTO", "Lieu convivial où l'on partage un repas"], [2, 0, 2, "START", "Point de départ d'une course"], [3, 0, 4, "TOTAL", "Montant cumulé de tous les éléments réunis"]]}, {"id": 46, "title": "46. Mine de Diamant", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "PLOMB", "Métal gris très lourd"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SCORE", "Total de points enregistrés dans un jeu"], [3, 0, 4, "TABLE", "Meuble à plateau plat reposant sur des pieds"]]}, {"id": 47, "title": "47. Forêt d'Émeraude", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "PLOMB", "Métal gris très lourd"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SCORE", "Total de points enregistrés dans un jeu"], [3, 0, 4, "TABLE", "Meuble à plateau plat reposant sur des pieds"]]}, {"id": 48, "title": "48. Bouclier Plasma", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "SOEUR", "Fille née des mêmes parents"], [5, 4, 0, "ORDRE", "Organisation méthodique ou commandement"]], "d": [[1, 0, 0, "RESTO", "Lieu convivial où l'on partage un repas"], [2, 0, 2, "SPEED", "Vitesse vive et soutenue dans l'action"], [3, 0, 4, "TERRE", "Notre planète bleue d'origine"]]}, {"id": 49, "title": "49. Vortex Temporel", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "PAGES", "Feuilles reliées d'un livre"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SIGNE", "Geste ou symbole porteur de signification"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 50, "title": "50. Sommet Enneigé", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "PAGES", "Feuilles reliées d'un livre"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SIGNE", "Geste ou symbole porteur de signification"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 51, "title": "51. Cœur d'Énergie", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "PAGES", "Feuilles reliées d'un livre"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SIGNE", "Geste ou symbole porteur de signification"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 52, "title": "52. Ruines Antiques", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "PAGES", "Feuilles reliées d'un livre"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SIGNE", "Geste ou symbole porteur de signification"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 53, "title": "53. Rivage Bleu", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "MANGA", "Bande dessinée japonaise"], [5, 4, 0, "ECRAN", "Surface vitrée diffusant images et jeux"]], "d": [[1, 0, 0, "RAMPE", "Plan incliné facilitant la montée"], [2, 0, 2, "SONAR", "Appareil acoustique de localisation sous-marine"], [3, 0, 4, "TRAIN", "Suite de wagons tractés par une locomotive"]]}, {"id": 54, "title": "54. Sphère Céleste", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "POIDS", "Force de pesanteur mesurée en kilogrammes"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SUITE", "Succession logique d'éléments coordonnés"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 55, "title": "55. Sentier Perdu", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "POIDS", "Force de pesanteur mesurée en kilogrammes"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SUITE", "Succession logique d'éléments coordonnés"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 56, "title": "56. Satellite Secret", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "POIDS", "Force de pesanteur mesurée en kilogrammes"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SUITE", "Succession logique d'éléments coordonnés"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 57, "title": "57. Terre d'Aventure", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "POIDS", "Force de pesanteur mesurée en kilogrammes"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SUITE", "Succession logique d'éléments coordonnés"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 58, "title": "58. Passage Secret", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "POINT", "Marque minuscule ou unité gagnée dans un jeu"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SUITE", "Succession logique d'éléments coordonnés"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 59, "title": "59. Désert Doré", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "POINT", "Marque minuscule ou unité gagnée dans un jeu"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "REPAS", "Nourriture prise à des heures régulières"], [2, 0, 2, "SUITE", "Succession logique d'éléments coordonnés"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 60, "title": "60. Cratère Lunaire", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "STEAM", "Vapeur d'eau sous pression ou boutique de jeux"], [5, 4, 0, "OREES", "Lisières d'une forêt"]], "d": [[1, 0, 0, "RESTO", "Lieu convivial où l'on partage un repas"], [2, 0, 2, "SCENE", "Plateau de théâtre ou espace de jeu"], [3, 0, 4, "TEMPS", "Durée mesurée par les secondes et les heures"]]}, {"id": 61, "title": "61. Flamme Éternelle", "rows": 5, "cols": 5, "a": [[1, 0, 0, "RESET", "Réinitialisation d'un système à son départ"], [4, 2, 0, "STEAM", "Vapeur d'eau sous pression ou boutique de jeux"], [5, 4, 0, "OREES", "Lisières d'une forêt"]], "d": [[1, 0, 0, "RESTO", "Lieu convivial où l'on partage un repas"], [2, 0, 2, "SIEGE", "Meuble fait pour s'asseoir"], [3, 0, 4, "TEMPS", "Durée mesurée par les secondes et les heures"]]}, {"id": 62, "title": "62. Jungle Tropicale", "rows": 5, "cols": 5, "a": [[1, 0, 0, "BONUS", "Récompense ou points supplémentaires dans un jeu"], [4, 2, 0, "OMBRE", "Zone obscure protégée de la lumière"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "BLOCS", "Gros morceaux solides de pierre ou de bois"], [2, 0, 2, "NOBLE", "De sang aristocratique ou d'esprit élevé"], [3, 0, 4, "SIEGE", "Meuble fait pour s'asseoir"]]}, {"id": 63, "title": "63. Porte des Mondes", "rows": 5, "cols": 5, "a": [[1, 0, 0, "BONUS", "Récompense ou points supplémentaires dans un jeu"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "BETES", "Animaux vivants ou personnes peu avisées"], [2, 0, 2, "NOBLE", "De sang aristocratique ou d'esprit élevé"], [3, 0, 4, "STAGE", "Niveau de progression dans un jeu vidéo"]]}, {"id": 64, "title": "64. Titan d'Acier", "rows": 5, "cols": 5, "a": [[1, 0, 0, "BONUS", "Récompense ou points supplémentaires dans un jeu"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "BETES", "Animaux vivants ou personnes peu avisées"], [2, 0, 2, "NOBLE", "De sang aristocratique ou d'esprit élevé"], [3, 0, 4, "STAGE", "Niveau de progression dans un jeu vidéo"]]}, {"id": 65, "title": "65. Comète Brillante", "rows": 5, "cols": 5, "a": [[1, 0, 0, "BONUS", "Récompense ou points supplémentaires dans un jeu"], [4, 2, 0, "TABLE", "Meuble à plateau plat reposant sur des pieds"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "BETES", "Animaux vivants ou personnes peu avisées"], [2, 0, 2, "NOBLE", "De sang aristocratique ou d'esprit élevé"], [3, 0, 4, "SIEGE", "Meuble fait pour s'asseoir"]]}, {"id": 66, "title": "66. Boussole Magique", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TERRE", "Notre planète bleue d'origine"], [5, 4, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"]], "d": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [2, 0, 2, "CARGO", "Grand navire de transport de fret"], [3, 0, 4, "OUEST", "Point cardinal où le soleil se couche"]]}, {"id": 67, "title": "67. Royaume Aquatique", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TERRE", "Notre planète bleue d'origine"], [5, 4, 0, "SCOUT", "Éclaireur dans la nature"]], "d": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [2, 0, 2, "CARGO", "Grand navire de transport de fret"], [3, 0, 4, "OUEST", "Point cardinal où le soleil se couche"]]}, {"id": 68, "title": "68. Volcan Actif", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "FARCE", "Tour comique ou petite plaisanterie"]], "d": [[1, 0, 0, "MOTIF", "Dessin répété ou raison d'agir"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "ORAGE", "Perturbation météo avec éclairs et tonnerre"]]}, {"id": 69, "title": "69. Ciel Étoilé", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "FARCE", "Tour comique ou petite plaisanterie"]], "d": [[1, 0, 0, "MOTIF", "Dessin répété ou raison d'agir"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "OVALE", "En forme d'œuf allongé"]]}, {"id": 70, "title": "70. Îlot Isolé", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [2, 0, 2, "CABLE", "Faisceau de fils conducteurs sous gaine"], [3, 0, 4, "ORAGE", "Perturbation météo avec éclairs et tonnerre"]]}, {"id": 71, "title": "71. Sentinelle Cyber", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "SCENE", "Plateau de théâtre ou espace de jeu"]], "d": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [2, 0, 2, "CABLE", "Faisceau de fils conducteurs sous gaine"], [3, 0, 4, "OVALE", "En forme d'œuf allongé"]]}, {"id": 72, "title": "72. Archipel Éloigné", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "SERIE", "Suite ordonnée d'éléments ou d'épisodes"]], "d": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "ORAGE", "Perturbation météo avec éclairs et tonnerre"]]}, {"id": 73, "title": "73. Canyon Profond", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "SERIE", "Suite ordonnée d'éléments ou d'épisodes"]], "d": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "OVALE", "En forme d'œuf allongé"]]}, {"id": 74, "title": "74. Cascade Cachée", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [2, 0, 2, "CABLE", "Faisceau de fils conducteurs sous gaine"], [3, 0, 4, "ORAGE", "Perturbation météo avec éclairs et tonnerre"]]}, {"id": 75, "title": "75. Matrice Neuronale", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "SIEGE", "Meuble fait pour s'asseoir"]], "d": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [2, 0, 2, "CABLE", "Faisceau de fils conducteurs sous gaine"], [3, 0, 4, "OVALE", "En forme d'œuf allongé"]]}, {"id": 76, "title": "76. Station Sous-Marine", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "HERBE", "Végétation verte des prairies"]], "d": [[1, 0, 0, "MATCH", "Rencontre sportive ou duel de jeu"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "ORAGE", "Perturbation météo avec éclairs et tonnerre"]]}, {"id": 77, "title": "77. Cité Suspendue", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "HERBE", "Végétation verte des prairies"]], "d": [[1, 0, 0, "MATCH", "Rencontre sportive ou duel de jeu"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "OVALE", "En forme d'œuf allongé"]]}, {"id": 78, "title": "78. Brume Mystique", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "FORCE", "Puissance musculaire ou physique"]], "d": [[1, 0, 0, "MOTIF", "Dessin répété ou raison d'agir"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "ORAGE", "Perturbation météo avec éclairs et tonnerre"]]}, {"id": 79, "title": "79. Grotte de Cristal", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "FORCE", "Puissance musculaire ou physique"]], "d": [[1, 0, 0, "MOTIF", "Dessin répété ou raison d'agir"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "OVALE", "En forme d'œuf allongé"]]}, {"id": 80, "title": "80. Étoile Filante", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "FORME", "Contour géométrique d'un corps"]], "d": [[1, 0, 0, "MOTIF", "Dessin répété ou raison d'agir"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "ORAGE", "Perturbation météo avec éclairs et tonnerre"]]}, {"id": 81, "title": "81. Forêt de Séquoias", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TIBIA", "Grand os de la jambe"], [5, 4, 0, "FORME", "Contour géométrique d'un corps"]], "d": [[1, 0, 0, "MOTIF", "Dessin répété ou raison d'agir"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "OVALE", "En forme d'œuf allongé"]]}, {"id": 82, "title": "82. Château Fort", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TRAIT", "Ligne tracée au crayon ou caractéristique"], [5, 4, 0, "HOTEL", "Établissement offrant des chambres aux voyageurs"]], "d": [[1, 0, 0, "MATCH", "Rencontre sportive ou duel de jeu"], [2, 0, 2, "CHANT", "Émission musicale de la voix"], [3, 0, 4, "OUTIL", "Ustensile manuel servant à travailler"]]}, {"id": 83, "title": "83. Phare de la Nuit", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "NOBLE", "De sang aristocratique ou d'esprit élevé"], [5, 4, 0, "ECRAN", "Surface vitrée diffusant images et jeux"]], "d": [[1, 0, 0, "MONDE", "La planète Terre et l'humanité"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "OCEAN", "Immense étendue d'eau salée"]]}, {"id": 84, "title": "84. Ciel Boréal", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MICRO", "Microprocesseur ou capteur de son"], [4, 2, 0, "TABLE", "Meuble à plateau plat reposant sur des pieds"], [5, 4, 0, "FORET", "Vaste espace boisé peuplé d'arbres"]], "d": [[1, 0, 0, "MOTIF", "Dessin répété ou raison d'agir"], [2, 0, 2, "CYBER", "Relatif à l'informatique et aux réseaux"], [3, 0, 4, "OUEST", "Point cardinal où le soleil se couche"]]}, {"id": 85, "title": "85. Monde Sous-Marin", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "OUEST", "Point cardinal où le soleil se couche"], [5, 4, 0, "TASSE", "Récipient à anse pour boisson chaude"]], "d": [[1, 0, 0, "SCOUT", "Éclaireur dans la nature"], [2, 0, 2, "OREES", "Lisières d'une forêt"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 86, "title": "86. Dune Infinie", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "OUEST", "Point cardinal où le soleil se couche"], [5, 4, 0, "TESTE", "Met à l'épreuve pour vérifier"]], "d": [[1, 0, 0, "SCOUT", "Éclaireur dans la nature"], [2, 0, 2, "OREES", "Lisières d'une forêt"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 87, "title": "87. Miroir d'Eau", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "OUEST", "Point cardinal où le soleil se couche"], [5, 4, 0, "ETAPE", "Phase d'un voyage ou palier d'un tour"]], "d": [[1, 0, 0, "SCORE", "Total de points enregistrés dans un jeu"], [2, 0, 2, "OPERA", "Spectacle musical grandiose théâtral"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 88, "title": "88. Chemin de Fer", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "OUEST", "Point cardinal où le soleil se couche"], [5, 4, 0, "TRACE", "Empreinte laissée par le passage de quelqu'un"]], "d": [[1, 0, 0, "SCOUT", "Éclaireur dans la nature"], [2, 0, 2, "OPERA", "Spectacle musical grandiose théâtral"], [3, 0, 4, "TITRE", "Nom d'une œuvre ou trophée de vainqueur"]]}, {"id": 89, "title": "89. Tour d'Ivoire", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "OPERA", "Spectacle musical grandiose théâtral"], [5, 4, 0, "TASSE", "Récipient à anse pour boisson chaude"]], "d": [[1, 0, 0, "SCOUT", "Éclaireur dans la nature"], [2, 0, 2, "OREES", "Lisières d'une forêt"], [3, 0, 4, "TRACE", "Empreinte laissée par le passage de quelqu'un"]]}, {"id": 90, "title": "90. Horloge Céleste", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "OPERA", "Spectacle musical grandiose théâtral"], [5, 4, 0, "TISON", "Morceau de bois qui brûle dans l'âtre"]], "d": [[1, 0, 0, "SCOUT", "Éclaireur dans la nature"], [2, 0, 2, "OREES", "Lisières d'une forêt"], [3, 0, 4, "TRAIN", "Suite de wagons tractés par une locomotive"]]}, {"id": 91, "title": "91. Sphère d'Énergie", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "OPERA", "Spectacle musical grandiose théâtral"], [5, 4, 0, "TESTE", "Met à l'épreuve pour vérifier"]], "d": [[1, 0, 0, "SCOUT", "Éclaireur dans la nature"], [2, 0, 2, "OREES", "Lisières d'une forêt"], [3, 0, 4, "TRACE", "Empreinte laissée par le passage de quelqu'un"]]}, {"id": 92, "title": "92. Cœur de Lion", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "OREES", "Lisières d'une forêt"], [5, 4, 0, "ETAPE", "Phase d'un voyage ou palier d'un tour"]], "d": [[1, 0, 0, "SCORE", "Total de points enregistrés dans un jeu"], [2, 0, 2, "OPERA", "Spectacle musical grandiose théâtral"], [3, 0, 4, "TASSE", "Récipient à anse pour boisson chaude"]]}, {"id": 93, "title": "93. Fleur de Lotus", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "OREES", "Lisières d'une forêt"], [5, 4, 0, "ETAPE", "Phase d'un voyage ou palier d'un tour"]], "d": [[1, 0, 0, "SCORE", "Total de points enregistrés dans un jeu"], [2, 0, 2, "OPERA", "Spectacle musical grandiose théâtral"], [3, 0, 4, "TESTE", "Met à l'épreuve pour vérifier"]]}, {"id": 94, "title": "94. Vapeur Cyber", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "ACTES", "Actions réelles ou parties d'une pièce de théâtre"], [5, 4, 0, "TALON", "Partie arrière de la chaussure ou du pied"]], "d": [[1, 0, 0, "SMART", "Intelligent, élégant ou connecté"], [2, 0, 2, "OUTIL", "Ustensile manuel servant à travailler"], [3, 0, 4, "TISON", "Morceau de bois qui brûle dans l'âtre"]]}, {"id": 95, "title": "95. Sanctuaire Sacré", "rows": 5, "cols": 5, "a": [[1, 0, 0, "SPORT", "Activité physique ou compétition comme l'e-sport"], [4, 2, 0, "ACTES", "Actions réelles ou parties d'une pièce de théâtre"], [5, 4, 0, "TALON", "Partie arrière de la chaussure ou du pied"]], "d": [[1, 0, 0, "START", "Point de départ d'une course"], [2, 0, 2, "OUTIL", "Ustensile manuel servant à travailler"], [3, 0, 4, "TISON", "Morceau de bois qui brûle dans l'âtre"]]}, {"id": 96, "title": "96. Boussole Dorée", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [4, 2, 0, "CARGO", "Grand navire de transport de fret"], [5, 4, 0, "OUEST", "Point cardinal où le soleil se couche"]], "d": [[1, 0, 0, "MACRO", "De grande dimension ou commande groupée"], [2, 0, 2, "TERRE", "Notre planète bleue d'origine"], [3, 0, 4, "SPORT", "Activité physique ou compétition comme l'e-sport"]]}, {"id": 97, "title": "97. Vague Géante", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [4, 2, 0, "CARGO", "Grand navire de transport de fret"], [5, 4, 0, "OUEST", "Point cardinal où le soleil se couche"]], "d": [[1, 0, 0, "MACRO", "De grande dimension ou commande groupée"], [2, 0, 2, "TERRE", "Notre planète bleue d'origine"], [3, 0, 4, "SCOUT", "Éclaireur dans la nature"]]}, {"id": 98, "title": "98. Couronne Royale", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [4, 2, 0, "CYBER", "Relatif à l'informatique et aux réseaux"], [5, 4, 0, "ORAGE", "Perturbation météo avec éclairs et tonnerre"]], "d": [[1, 0, 0, "MACRO", "De grande dimension ou commande groupée"], [2, 0, 2, "TIBIA", "Grand os de la jambe"], [3, 0, 4, "SERIE", "Suite ordonnée d'éléments ou d'épisodes"]]}, {"id": 99, "title": "99. Éclipse Solaire", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [4, 2, 0, "CYBER", "Relatif à l'informatique et aux réseaux"], [5, 4, 0, "OVALE", "En forme d'œuf allongé"]], "d": [[1, 0, 0, "MACRO", "De grande dimension ou commande groupée"], [2, 0, 2, "TIBIA", "Grand os de la jambe"], [3, 0, 4, "SERIE", "Suite ordonnée d'éléments ou d'épisodes"]]}, {"id": 100, "title": "100. Maître des Mots", "rows": 5, "cols": 5, "a": [[1, 0, 0, "MOTOS", "Véhicules rapides à deux roues"], [4, 2, 0, "CABLE", "Faisceau de fils conducteurs sous gaine"], [5, 4, 0, "ORAGE", "Perturbation météo avec éclairs et tonnerre"]], "d": [[1, 0, 0, "MACRO", "De grande dimension ou commande groupée"], [2, 0, 2, "TIBIA", "Grand os de la jambe"], [3, 0, 4, "SCENE", "Plateau de théâtre ou espace de jeu"]]}];

// Mapping vers le format standard
const GRIDS = GRIDS_RAW.map(g => ({
  id: g.id,
  title: g.title,
  rows: g.rows,
  cols: g.cols,
  across: g.a.map(item => ({ num: item[0], r: item[1], c: item[2], word: item[3], clue: item[4] })),
  down: g.d.map(item => ({ num: item[0], r: item[1], c: item[2], word: item[3], clue: item[4] }))
}));

let curGridIdx = parseInt(localStorage.getItem('cw_last_level') || '0');
if (curGridIdx >= GRIDS.length) curGridIdx = 0;
let curGrid = GRIDS[curGridIdx];

let boardLetters = [];
let solutionLetters = [];
let cellNumbers = [];
let activeRow = 0, activeCol = 0;
let activeDir = 'across';
let curClueTab = 'across';
let timerSec = 0, timerTimer = null;
let gameWon = false;

// Audio Synth Procedural
const AudioCtx = window.AudioContext || window.webkitAudioContext;
let audioCtx = null;
function playTone(freq, dur, type='sine') {
  try {
    if (!audioCtx) audioCtx = new AudioCtx();
    const osc = audioCtx.createOscillator();
    const gain = audioCtx.createGain();
    osc.type = type;
    osc.frequency.setValueAtTime(freq, audioCtx.currentTime);
    gain.gain.setValueAtTime(0.08, audioCtx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, audioCtx.currentTime + dur);
    osc.connect(gain);
    gain.connect(audioCtx.destination);
    osc.start();
    osc.stop(audioCtx.currentTime + dur);
  } catch(e){}
}

function playWinSound() {
  const notes = [523.25, 659.25, 783.99, 1046.50];
  notes.forEach((freq, idx) => {
    setTimeout(() => playTone(freq, 0.28, 'triangle'), idx * 120);
  });
}

function initGrid() {
  curGrid = GRIDS[curGridIdx];
  localStorage.setItem('cw_last_level', curGridIdx);
  document.getElementById('cur-lvl-num').textContent = curGrid.id;

  const R = curGrid.rows, C = curGrid.cols;
  boardLetters = Array(R).fill(null).map(() => Array(C).fill(null));
  solutionLetters = Array(R).fill(null).map(() => Array(C).fill(null));
  cellNumbers = Array(R).fill(null).map(() => Array(C).fill(null));
  gameWon = false;

  curGrid.across.forEach(item => {
    cellNumbers[item.r][item.c] = item.num;
    for (let i = 0; i < item.word.length; i++) {
      solutionLetters[item.r][item.c + i] = item.word[i];
      boardLetters[item.r][item.c + i] = '';
    }
  });

  curGrid.down.forEach(item => {
    if (!cellNumbers[item.r][item.c]) cellNumbers[item.r][item.c] = item.num;
    for (let i = 0; i < item.word.length; i++) {
      solutionLetters[item.r + i][item.c] = item.word[i];
      boardLetters[item.r + i][item.c] = '';
    }
  });

  outer: for (let r = 0; r < R; r++) {
    for (let c = 0; c < C; c++) {
      if (solutionLetters[r][c] !== null) {
        activeRow = r;
        activeCol = c;
        break outer;
      }
    }
  }

  activeDir = 'across';
  renderBoard();
  renderClues();
  updateClueBanner();

  clearInterval(timerTimer);
  timerSec = 0;
  updateTimerDisplay();
  timerTimer = setInterval(() => {
    timerSec++;
    updateTimerDisplay();
  }, 1000);
}

function updateTimerDisplay() {
  const m = String(Math.floor(timerSec / 60)).padStart(2, '0');
  const s = String(timerSec % 60).padStart(2, '0');
  document.getElementById('timer').textContent = `${m}:${s}`;
}

function prevGrid() {
  curGridIdx = (curGridIdx - 1 + GRIDS.length) % GRIDS.length;
  initGrid();
}

function nextGrid() {
  curGridIdx = (curGridIdx + 1) % GRIDS.length;
  initGrid();
}

function nextGridAfterWin() {
  document.getElementById('victory-modal').style.display = 'none';
  nextGrid();
}

function selectGrid(idx) {
  curGridIdx = idx;
  closeLevelModal();
  initGrid();
}

function openLevelModal() {
  const grid = document.getElementById('levels-grid');
  grid.innerHTML = '';
  const completed = JSON.parse(localStorage.getItem('cw_completed_levels') || '[]');

  GRIDS.forEach((g, idx) => {
    const btn = document.createElement('button');
    btn.className = 'lvl-btn';
    if (idx === curGridIdx) btn.classList.add('active');
    const isDone = completed.includes(g.id);
    if (isDone) btn.classList.add('completed');

    btn.innerHTML = `<span>${g.id}</span>${isDone ? '<span class="star-mark">⭐</span>' : ''}`;
    btn.onclick = () => selectGrid(idx);
    grid.appendChild(btn);
  });

  document.getElementById('levels-modal').style.display = 'flex';
}

function closeLevelModal() {
  document.getElementById('levels-modal').style.display = 'none';
}

function getWordAt(r, c, dir) {
  const list = (dir === 'across') ? curGrid.across : curGrid.down;
  for (const item of list) {
    if (dir === 'across') {
      if (r === item.r && c >= item.c && c < item.c + item.word.length) return item;
    } else {
      if (c === item.c && r >= item.r && r < item.r + item.word.length) return item;
    }
  }
  return null;
}

function getWordCells(wordItem, dir) {
  const cells = [];
  for (let i = 0; i < wordItem.word.length; i++) {
    if (dir === 'across') cells.push({ r: wordItem.r, c: wordItem.c + i });
    else cells.push({ r: wordItem.r + i, c: wordItem.c });
  }
  return cells;
}

function handleCellClick(r, c) {
  if (solutionLetters[r][c] === null) return;

  if (activeRow === r && activeCol === c) {
    const hasAcross = getWordAt(r, c, 'across') !== null;
    const hasDown = getWordAt(r, c, 'down') !== null;
    if (hasAcross && hasDown) {
      activeDir = (activeDir === 'across') ? 'down' : 'across';
    }
  } else {
    activeRow = r;
    activeCol = c;
    if (!getWordAt(r, c, activeDir)) {
      activeDir = (activeDir === 'across') ? 'down' : 'across';
    }
  }

  playTone(400, 0.05, 'triangle');
  renderBoard();
  updateClueBanner();
  renderClues();
}

function toggleDirection() {
  const hasAcross = getWordAt(activeRow, activeCol, 'across') !== null;
  const hasDown = getWordAt(activeRow, activeCol, 'down') !== null;
  if (hasAcross && hasDown) {
    activeDir = (activeDir === 'across') ? 'down' : 'across';
  } else if (hasAcross) {
    activeDir = 'across';
  } else if (hasDown) {
    activeDir = 'down';
  }
  playTone(450, 0.05, 'sine');
  renderBoard();
  updateClueBanner();
  renderClues();
}

function updateClueBanner() {
  const curWord = getWordAt(activeRow, activeCol, activeDir);
  if (!curWord) return;
  const dirName = (activeDir === 'across') ? 'HORIZONTAL' : 'VERTICAL';
  document.getElementById('clue-meta').textContent = `NIV. ${curGrid.id} • ${curWord.num} ${dirName} (${curWord.word.length} LETTRES)`;
  document.getElementById('clue-text').textContent = curWord.clue;
}

function renderBoard() {
  const R = curGrid.rows, C = curGrid.cols;
  const gridEl = document.getElementById('cw-grid');
  gridEl.style.gridTemplateColumns = `repeat(${C}, 1fr)`;
  gridEl.innerHTML = '';

  const curWord = getWordAt(activeRow, activeCol, activeDir);
  const wordCells = curWord ? getWordCells(curWord, activeDir) : [];

  for (let r = 0; r < R; r++) {
    for (let c = 0; c < C; c++) {
      const cell = document.createElement('div');
      cell.className = 'cw-cell';

      if (solutionLetters[r][c] === null) {
        cell.classList.add('black');
      } else {
        if (cellNumbers[r][c]) {
          const numSpan = document.createElement('span');
          numSpan.className = 'cell-num';
          numSpan.textContent = cellNumbers[r][c];
          cell.appendChild(numSpan);
        }

        const isFocus = (r === activeRow && c === activeCol);
        const isInWord = wordCells.some(cellCoord => cellCoord.r === r && cellCoord.c === c);

        if (isFocus) cell.classList.add('active-focus');
        else if (isInWord) cell.classList.add('in-word');

        cell.appendChild(document.createTextNode(boardLetters[r][c] || ''));
        cell.onclick = () => handleCellClick(r, c);
      }
      gridEl.appendChild(cell);
    }
  }
}

function renderClues() {
  const listEl = document.getElementById('clues-list');
  listEl.innerHTML = '';

  const curWord = getWordAt(activeRow, activeCol, activeDir);
  const items = (curClueTab === 'across') ? curGrid.across : curGrid.down;

  items.forEach(item => {
    const div = document.createElement('div');
    div.className = 'clue-item';
    const isActive = (curWord && curWord.num === item.num && activeDir === curClueTab);
    if (isActive) div.classList.add('active');

    const cells = getWordCells(item, curClueTab);
    const userStr = cells.map(pt => boardLetters[pt.r][pt.c]).join('');
    if (userStr === item.word) div.classList.add('solved');

    div.innerHTML = `<strong>${item.num}.</strong> <span>${item.clue}</span> <span style="margin-left:auto;color:var(--dim);font-size:0.75rem;">(${item.word.length})</span>`;
    div.onclick = () => {
      activeRow = item.r;
      activeCol = item.c;
      activeDir = curClueTab;
      playTone(420, 0.05, 'triangle');
      renderBoard();
      updateClueBanner();
      renderClues();
    };
    listEl.appendChild(div);
  });
}

function setClueTab(tab) {
  curClueTab = tab;
  document.getElementById('tab-across-btn').classList.toggle('active', tab === 'across');
  document.getElementById('tab-down-btn').classList.toggle('active', tab === 'down');
  renderClues();
}

function handleVirtualKey(key) {
  if (gameWon) return;
  if (solutionLetters[activeRow][activeCol] === null) return;

  if (key === 'DEL') {
    if (boardLetters[activeRow][activeCol] === '') {
      stepCell(-1);
    }
    boardLetters[activeRow][activeCol] = '';
    playTone(250, 0.06, 'square');
    renderBoard();
    return;
  }

  const char = key.toUpperCase();
  boardLetters[activeRow][activeCol] = char;
  playTone(550, 0.05, 'sine');
  renderBoard();

  stepCell(1);
  checkFullWin();
}

function stepCell(delta) {
  const curWord = getWordAt(activeRow, activeCol, activeDir);
  if (!curWord) return;
  const cells = getWordCells(curWord, activeDir);
  const curIdx = cells.findIndex(pt => pt.r === activeRow && pt.c === activeCol);

  if (curIdx !== -1) {
    const nextIdx = curIdx + delta;
    if (nextIdx >= 0 && nextIdx < cells.length) {
      activeRow = cells[nextIdx].r;
      activeCol = cells[nextIdx].c;
      renderBoard();
      updateClueBanner();
      renderClues();
    }
  }
}

function nextWord() {
  const list = (activeDir === 'across') ? curGrid.across : curGrid.down;
  const curWord = getWordAt(activeRow, activeCol, activeDir);
  let nextIdx = 0;
  if (curWord) {
    const idx = list.findIndex(w => w.num === curWord.num);
    if (idx !== -1 && idx < list.length - 1) nextIdx = idx + 1;
    else {
      activeDir = (activeDir === 'across') ? 'down' : 'across';
      const otherList = (activeDir === 'across') ? curGrid.across : curGrid.down;
      activeRow = otherList[0].r;
      activeCol = otherList[0].c;
      renderBoard();
      updateClueBanner();
      renderClues();
      return;
    }
  }
  activeRow = list[nextIdx].r;
  activeCol = list[nextIdx].c;
  renderBoard();
  updateClueBanner();
  renderClues();
}

function revealCurrentCell() {
  if (gameWon) return;
  if (solutionLetters[activeRow][activeCol] === null) return;
  boardLetters[activeRow][activeCol] = solutionLetters[activeRow][activeCol];
  playTone(600, 0.1, 'sine');
  renderBoard();
  stepCell(1);
  checkFullWin();
}

function checkAnswers() {
  const R = curGrid.rows, C = curGrid.cols;
  const gridCells = document.querySelectorAll('.cw-cell:not(.black)');
  let idx = 0;
  let allCorrect = true;

  for (let r = 0; r < R; r++) {
    for (let c = 0; c < C; c++) {
      if (solutionLetters[r][c] !== null) {
        const domCell = gridCells[idx];
        const val = boardLetters[r][c];
        if (val) {
          if (val === solutionLetters[r][c]) {
            domCell.classList.add('cell-correct');
          } else {
            domCell.classList.add('cell-wrong');
            allCorrect = false;
          }
        } else {
          allCorrect = false;
        }
        idx++;
      }
    }
  }

  if (allCorrect) {
    triggerWin();
  } else {
    playTone(200, 0.2, 'sawtooth');
    setTimeout(() => {
      document.querySelectorAll('.cell-correct, .cell-wrong').forEach(el => {
        el.classList.remove('cell-correct', 'cell-wrong');
      });
    }, 1800);
  }
}

function checkFullWin() {
  const R = curGrid.rows, C = curGrid.cols;
  for (let r = 0; r < R; r++) {
    for (let c = 0; c < C; c++) {
      if (solutionLetters[r][c] !== null) {
        if (boardLetters[r][c] !== solutionLetters[r][c]) return;
      }
    }
  }
  triggerWin();
}

function triggerWin() {
  if (gameWon) return;
  gameWon = true;
  clearInterval(timerTimer);
  playWinSound();

  // Save completion
  const completed = JSON.parse(localStorage.getItem('cw_completed_levels') || '[]');
  if (!completed.includes(curGrid.id)) {
    completed.push(curGrid.id);
    localStorage.setItem('cw_completed_levels', JSON.stringify(completed));
  }

  const m = String(Math.floor(timerSec / 60)).padStart(2, '0');
  const s = String(timerSec % 60).padStart(2, '0');
  document.getElementById('victory-stats').textContent = `Félicitations ! Vous avez résolu le niveau ${curGrid.title} en ${m}:${s}.`;
  document.getElementById('victory-modal').style.display = 'flex';
}

window.addEventListener('keydown', (e) => {
  if (gameWon) return;
  const k = e.key;
  if (/^[a-zA-Z]$/.test(k)) {
    handleVirtualKey(k.toUpperCase());
    e.preventDefault();
  } else if (k === 'Backspace') {
    handleVirtualKey('DEL');
    e.preventDefault();
  } else if (k === ' ' || k === 'Enter') {
    toggleDirection();
    e.preventDefault();
  } else if (k === 'Tab') {
    nextWord();
    e.preventDefault();
  } else if (k === 'ArrowLeft') {
    if (activeCol > 0 && solutionLetters[activeRow][activeCol - 1] !== null) {
      activeCol--;
      renderBoard(); updateClueBanner(); renderClues();
    }
    e.preventDefault();
  } else if (k === 'ArrowRight') {
    if (activeCol < curGrid.cols - 1 && solutionLetters[activeRow][activeCol + 1] !== null) {
      activeCol++;
      renderBoard(); updateClueBanner(); renderClues();
    }
    e.preventDefault();
  } else if (k === 'ArrowUp') {
    if (activeRow > 0 && solutionLetters[activeRow - 1][activeCol] !== null) {
      activeRow--;
      renderBoard(); updateClueBanner(); renderClues();
    }
    e.preventDefault();
  } else if (k === 'ArrowDown') {
    if (activeRow < curGrid.rows - 1 && solutionLetters[activeRow + 1][activeCol] !== null) {
      activeRow++;
      renderBoard(); updateClueBanner(); renderClues();
    }
    e.preventDefault();
  }
});

initGrid();
</script>
</body>
</html>
)rawliteral";

#endif
