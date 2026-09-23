#ifndef GAME_MOTSCROISES_H
#define GAME_MOTSCROISES_H

#include <pgmspace.h>

const char GAME_MOTSCROISES_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Mots Croisés Cyber</title>
<style>
  :root {
    --bg: #090b12;
    --card: #11162a;
    --card-border: rgba(0, 243, 255, 0.25);
    --cyan: #00f3ff;
    --pink: #ff0055;
    --yellow: #ffe600;
    --green: #00ff66;
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
    margin-bottom: 10px;
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
  .grid-selector {
    flex: 1;
    max-width: 210px;
    background: var(--card);
    color: #fff;
    border: 1px solid var(--card-border);
    padding: 6px 8px;
    border-radius: 8px;
    font-size: 0.82rem;
    font-weight: bold;
    outline: none;
    cursor: pointer;
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
    margin-bottom: 10px;
    min-height: 50px;
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
    margin-bottom: 10px;
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
    margin-bottom: 10px;
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
    margin-bottom: 12px;
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
    height: 40px;
    background: #171d33;
    border: 1px solid rgba(255,255,255,0.12);
    border-radius: 6px;
    color: #fff;
    font-size: 1rem;
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
    font-size: 0.82rem;
    background: #202742;
  }

  /* Clues Container */
  .clues-wrapper {
    width: 100%;
    max-width: 440px;
    background: var(--card);
    border: 1px solid var(--card-border);
    border-radius: 12px;
    padding: 10px 12px;
  }
  .clues-nav {
    display: flex;
    gap: 8px;
    margin-bottom: 8px;
    border-bottom: 1px solid rgba(255,255,255,0.1);
    padding-bottom: 6px;
  }
  .clue-tab-btn {
    background: none;
    border: none;
    color: var(--dim);
    font-weight: 800;
    font-size: 0.85rem;
    padding: 4px 8px;
    cursor: pointer;
  }
  .clue-tab-btn.active {
    color: var(--cyan);
    border-bottom: 2px solid var(--cyan);
  }
  .clue-item {
    font-size: 0.82rem;
    padding: 6px 8px;
    border-radius: 6px;
    cursor: pointer;
    margin-bottom: 4px;
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

  /* Victory Modal */
  .modal-overlay {
    display: none;
    position: fixed;
    top: 0; left: 0; right: 0; bottom: 0;
    background: rgba(4, 6, 12, 0.85);
    backdrop-filter: blur(5px);
    z-index: 100;
    align-items: center;
    justify-content: center;
    padding: 20px;
  }
  .modal-box {
    background: linear-gradient(145deg, #131a33, #0d1224);
    border: 2px solid var(--cyan);
    border-radius: 20px;
    padding: 24px;
    max-width: 360px;
    width: 100%;
    text-align: center;
    box-shadow: 0 0 40px rgba(0,243,255,0.4);
    animation: popIn 0.3s cubic-bezier(0.175, 0.885, 0.32, 1.275);
  }
  @keyframes popIn {
    0% { transform: scale(0.7); opacity: 0; }
    100% { transform: scale(1); opacity: 1; }
  }
  .modal-title {
    font-size: 1.5rem;
    color: var(--yellow);
    font-weight: 900;
    margin-bottom: 8px;
    text-transform: uppercase;
  }
  .modal-text {
    color: var(--dim);
    font-size: 0.9rem;
    margin-bottom: 18px;
  }
  .modal-btn {
    width: 100%;
    padding: 12px;
    background: linear-gradient(135deg, var(--cyan), #0088ff);
    border: none;
    border-radius: 12px;
    color: #000;
    font-weight: 900;
    font-size: 1rem;
    cursor: pointer;
    box-shadow: 0 4px 15px rgba(0,243,255,0.4);
  }
</style>
<script src="/pause.js"></script>
</head>
<body>

<header>
  <a href="/hub" class="btn-back">◀ Menu</a>
  <select id="grid-select" class="grid-selector" onchange="switchGrid(parseInt(this.value))">
    <option value="0">1. Rétro Arcade</option>
    <option value="1">2. Cyber & Tech</option>
    <option value="2">3. Espace & Énergie</option>
    <option value="3">4. Océan & Mystère</option>
    <option value="4">5. Forêt & Aventure</option>
    <option value="5">6. Carré Magique Express</option>
  </select>
  <div class="timer-pill" id="timer">00:00</div>
</header>

<div class="clue-banner" id="clue-banner" onclick="toggleDirection()">
  <div class="clue-meta" id="clue-meta">1 HORIZONTAL (5 LETTRES)</div>
  <div class="clue-text" id="clue-text">Machine autonome programmable</div>
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

<!-- Victory Modal -->
<div class="modal-overlay" id="victory-modal">
  <div class="modal-box">
    <div style="font-size:3rem;margin-bottom:8px;">🏆</div>
    <div class="modal-title">Grille Complétée !</div>
    <div class="modal-text" id="victory-stats">Bravo ! Vous avez résolu la grille.</div>
    <button class="modal-btn" onclick="nextGrid()">Grille Suivante ⏭️</button>
  </div>
</div>

<script>
const GRIDS = [
  {
    id: 1, title: '1. Rétro Arcade', rows: 5, cols: 5,
    across: [
      { num: 1, r: 0, c: 0, word: 'ROBOT', clue: 'Machine autonome programmable' },
      { num: 4, r: 2, c: 0, word: 'SONAR', clue: 'Appareil acoustique de détection' },
      { num: 5, r: 4, c: 0, word: 'TASSE', clue: 'Récipient à anse pour boisson chaude' }
    ],
    down: [
      { num: 1, r: 0, c: 0, word: 'RESET', clue: 'Bouton de réinitialisation système' },
      { num: 2, r: 0, c: 2, word: 'BONUS', clue: 'Récompense supplémentaire dans un jeu' },
      { num: 3, r: 0, c: 4, word: 'TERRE', clue: 'Notre planète d\'origine' }
    ]
  },
  {
    id: 2, title: '2. Cyber & Tech', rows: 5, cols: 5,
    across: [
      { num: 1, r: 0, c: 0, word: 'MICRO', clue: 'Abréviation de microprocesseur ou micro' },
      { num: 4, r: 2, c: 0, word: 'TERRE', clue: 'La planète des humains' },
      { num: 5, r: 4, c: 0, word: 'SPORT', clue: 'Discipline physique ou compétition e-sport' }
    ],
    down: [
      { num: 1, r: 0, c: 0, word: 'MOTOS', clue: 'Véhicules rapides à deux roues' },
      { num: 2, r: 0, c: 2, word: 'CARGO', clue: 'Grand navire de transport de fret' },
      { num: 3, r: 0, c: 4, word: 'OUEST', clue: 'Point cardinal où le soleil se couche' }
    ]
  },
  {
    id: 3, title: '3. Espace & Énergie', rows: 5, cols: 5,
    across: [
      { num: 1, r: 0, c: 0, word: 'VOLTS', clue: 'Unité de mesure de tension électrique' },
      { num: 4, r: 2, c: 0, word: 'CANAL', clue: 'Bande de fréquence de transmission radio' },
      { num: 5, r: 4, c: 0, word: 'LASER', clue: 'Rayon lumineux cohérent et puissant' }
    ],
    down: [
      { num: 1, r: 0, c: 0, word: 'VOCAL', clue: 'Relatif à la voix humaine' },
      { num: 2, r: 0, c: 2, word: 'LUNES', clue: 'Satellites naturels en orbite planétaire' },
      { num: 3, r: 0, c: 4, word: 'SOLAR', clue: 'Relatif au soleil et à l\'énergie' }
    ]
  },
  {
    id: 4, title: '4. Océan & Mystère', rows: 5, cols: 5,
    across: [
      { num: 1, r: 0, c: 0, word: 'RAMPE', clue: 'Plan incliné facilitant la montée' },
      { num: 4, r: 2, c: 0, word: 'SONAR', clue: 'Détecteur acoustique des fonds marins' },
      { num: 5, r: 4, c: 0, word: 'OCEAN', clue: 'Immense étendue d\'eau salée' }
    ],
    down: [
      { num: 1, r: 0, c: 0, word: 'RESTO', clue: 'Lieu convivial où l\'on partage un repas' },
      { num: 2, r: 0, c: 2, word: 'MONDE', clue: 'L\'ensemble de notre planète Terre' },
      { num: 3, r: 0, c: 4, word: 'ECRAN', clue: 'Surface vitrée diffusant images et jeux' }
    ]
  },
  {
    id: 5, title: '5. Forêt & Aventure', rows: 5, cols: 5,
    across: [
      { num: 1, r: 0, c: 0, word: 'FORET', clue: 'Vaste espace boisé peuplé d\'arbres' },
      { num: 4, r: 2, c: 0, word: 'RADIS', clue: 'Petit légume rouge croquant et piquant' },
      { num: 5, r: 4, c: 0, word: 'ECRAN', clue: 'Moniteur d\'affichage d\'un ordinateur' }
    ],
    down: [
      { num: 1, r: 0, c: 0, word: 'FARCE', clue: 'Tour comique ou petite plaisanterie' },
      { num: 2, r: 0, c: 2, word: 'RADAR', clue: 'Système d\'écho-ondes pour repérer les avions' },
      { num: 3, r: 0, c: 4, word: 'TISON', clue: 'Morceau de bois qui rougeoie dans l\'âtre' }
    ]
  },
  {
    id: 6, title: '6. Carré Magique Express', rows: 4, cols: 4,
    across: [
      { num: 1, r: 0, c: 0, word: 'ETAT', clue: 'Nation souveraine ou condition physique' },
      { num: 2, r: 1, c: 0, word: 'TOUR', clue: 'Édifice élevé ou ronde de garde' },
      { num: 3, r: 2, c: 0, word: 'AUTO', clue: 'Véhicule roulant automobile' },
      { num: 4, r: 3, c: 0, word: 'TROT', clue: 'Allure vive cadencée d\'un cheval' }
    ],
    down: [
      { num: 1, r: 0, c: 0, word: 'ETAT', clue: 'Condition d\'une chose ou pays' },
      { num: 2, r: 0, c: 1, word: 'TOUR', clue: 'Château fortifié ou circuit' },
      { num: 3, r: 0, c: 2, word: 'AUTO', clue: 'Voiture à moteur' },
      { num: 4, r: 0, c: 3, word: 'TROT', clue: 'Allure entre le pas et le galop' }
    ]
  }
];

let curGridIdx = 0;
let curGrid = GRIDS[0];
let boardLetters = [];
let solutionLetters = [];
let cellNumbers = [];
let activeRow = 0, activeCol = 0;
let activeDir = 'across';
let curClueTab = 'across';
let timerSec = 0, timerTimer = null;
let gameWon = false;

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

function switchGrid(idx) {
  curGridIdx = idx;
  document.getElementById('grid-select').value = idx;
  initGrid();
}

function nextGrid() {
  document.getElementById('victory-modal').style.display = 'none';
  curGridIdx = (curGridIdx + 1) % GRIDS.length;
  document.getElementById('grid-select').value = curGridIdx;
  initGrid();
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
  document.getElementById('clue-meta').textContent = `${curWord.num} ${dirName} (${curWord.word.length} LETTRES)`;
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

  const m = String(Math.floor(timerSec / 60)).padStart(2, '0');
  const s = String(timerSec % 60).padStart(2, '0');
  document.getElementById('victory-stats').textContent = `Félicitations ! Vous avez résolu « ${curGrid.title} » en ${m}:${s}.`;
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
