#ifndef GAME_PACK_MP_REFLEXION_H
#define GAME_PACK_MP_REFLEXION_H

#include <pgmspace.h>

// ==================================================================================
//   ESP32 ARCADE - 5 JEUX DE REFLEXION & STRATEGIE (1V1 WI-FI & SOLO VS IA)
//   1. PUISSANCE 4  |  2. BATAILLE NAVALE  |  3. MORPION CYBER
//   4. DUEL DE MEMOIRE  |  5. MASTERMIND DUEL
// ==================================================================================

// ----------------------------------------------------------------------------------
//   1. PUISSANCE 4 (1V1 & VS IA STRATEGIQUE)
// ----------------------------------------------------------------------------------
const char MP_CONNECT4_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Puissance 4 Pro</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: #050814; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      min-height: 100vh; padding: 12px;
    }
    header { width: 100%; max-width: 360px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .turn-status { font-size: 1.15rem; font-weight: 900; margin-bottom: 12px; min-height: 28px; text-align: center; }
    #grid-wrap {
      background: linear-gradient(180deg, #0044cc, #002277); padding: 10px; border-radius: 20px;
      border: 3px solid #00f3ff; box-shadow: 0 10px 35px rgba(0,0,0,0.8), 0 0 25px rgba(0,243,255,0.3);
      display: grid; grid-template-columns: repeat(7, 44px); gap: 6px;
    }
    .slot {
      width: 44px; height: 44px; border-radius: 50%; background: #050814;
      box-shadow: inset 0 4px 8px rgba(0,0,0,0.9), 0 1px 1px rgba(255,255,255,0.15);
      cursor: pointer; position: relative;
    }
    .slot.p1 {
      background: radial-gradient(circle at 35% 35%, #ffffff 0%, #ffe600 45%, #ff9900 85%, #b36b00 100%);
      box-shadow: 0 4px 12px rgba(255,230,0,0.5), inset 0 0 6px rgba(0,0,0,0.4);
      animation: dropPuck 0.25s cubic-bezier(0.25, 1, 0.5, 1);
    }
    .slot.p2 {
      background: radial-gradient(circle at 35% 35%, #ffffff 0%, #ff0055 45%, #cc0033 85%, #800020 100%);
      box-shadow: 0 4px 12px rgba(255,0,85,0.5), inset 0 0 6px rgba(0,0,0,0.4);
      animation: dropPuck 0.25s cubic-bezier(0.25, 1, 0.5, 1);
    }
    @keyframes dropPuck {
      from { transform: translateY(-120px) scale(0.8); opacity: 0; }
      to { transform: translateY(0) scale(1); opacity: 1; }
    }
    .act-btn { padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem; cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; transition: all 0.15s; }
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 15px rgba(0,243,255,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-ai { background: linear-gradient(180deg, #a855f7, #6b21a8); color: #fff; box-shadow: 0 4px 15px rgba(168,85,247,0.4); margin-top: 8px; }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span id="badge" style="color:#ffe600; font-weight:900; font-size:0.85rem;">Connexion...</span>
  </header>
  <div class="turn-status" id="status">En attente d'un adversaire...</div>
  <div id="grid-wrap"></div>

  <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:308px; margin-top:16px;">
    <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
    <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
  </div>

  <div id="ai-opt" style="margin-top:14px; width:100%; max-width:308px;">
    <button id="btn-ai" class="act-btn btn-ai" onclick="startAiMode()">🤖 Jouer contre l'IA</button>
  </div>

  <script>
    const gridWrap = document.getElementById('grid-wrap');
    const statusEl = document.getElementById('status');
    const badge = document.getElementById('badge');
    const ovActions = document.getElementById('ov-actions');
    const btnRematch = document.getElementById('btn-rematch');
    const aiOpt = document.getElementById('ai-opt');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    const COLS = 7, ROWS = 6;
    let board = Array(ROWS).fill(0).map(() => Array(COLS).fill(0));
    let role = 0, myTurn = false, playing = false;
    let myRematch = false, oppRematch = false;
    let isAi = new URLSearchParams(window.location.search).get('vs') === 'ai';

    for (let r = 0; r < ROWS; r++) {
      for (let c = 0; c < COLS; c++) {
        const s = document.createElement('div');
        s.className = 'slot'; s.dataset.col = c;
        s.onclick = () => dropToken(c);
        gridWrap.appendChild(s);
      }
    }

    function startAiMode() {
      isAi = true;
      aiOpt.style.display = 'none';
      badge.textContent = "🤖 Solo vs IA";
      badge.style.color = "#a855f7";
      role = 1; playing = true; myTurn = true;
      statusEl.textContent = "🟡 À vous de jouer !";
      statusEl.style.color = "#ffe600";
    }

    function renderBoard() {
      const slots = gridWrap.children;
      for (let r = 0; r < ROWS; r++) {
        for (let c = 0; c < COLS; c++) {
          const idx = r * COLS + c;
          slots[idx].className = 'slot' + (board[r][c] === 1 ? ' p1' : board[r][c] === 2 ? ' p2' : '');
        }
      }
    }

    function dropToken(col) {
      if (!playing || !myTurn) return;
      for (let r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] === 0) {
          board[r][col] = role;
          renderBoard();
          if (checkWin(r, col, role)) {
            endGame(role);
          } else if (board[0].every(c => c !== 0)) {
            endGame(0);
          } else {
            myTurn = false;
            updateStatus();
            if (isAi) {
              setTimeout(aiTurn, 600);
            } else {
              ws.send(JSON.stringify({ t: "c4_move", c: col, r: r }));
            }
          }
          break;
        }
      }
    }

    function aiTurn() {
      if (!playing) return;
      // Simple smart AI: 1. Win if can, 2. Block player if can, 3. Center preference
      let bestCol = -1;
      for (let c = 0; c < COLS; c++) {
        const r = getOpenRow(c);
        if (r >= 0) {
          board[r][c] = 2;
          if (checkWin(r, c, 2)) { bestCol = c; board[r][c] = 0; break; }
          board[r][c] = 0;
        }
      }
      if (bestCol < 0) {
        for (let c = 0; c < COLS; c++) {
          const r = getOpenRow(c);
          if (r >= 0) {
            board[r][c] = 1;
            if (checkWin(r, c, 1)) { bestCol = c; board[r][c] = 0; break; }
            board[r][c] = 0;
          }
        }
      }
      if (bestCol < 0) {
        const pref = [3, 2, 4, 1, 5, 0, 6];
        for (let c of pref) {
          if (getOpenRow(c) >= 0) { bestCol = c; break; }
        }
      }
      if (bestCol >= 0) {
        const r = getOpenRow(bestCol);
        board[r][bestCol] = 2;
        renderBoard();
        if (checkWin(r, bestCol, 2)) {
          endGame(2);
        } else if (board[0].every(c => c !== 0)) {
          endGame(0);
        } else {
          myTurn = true;
          updateStatus();
        }
      }
    }

    function getOpenRow(col) {
      for (let r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] === 0) return r;
      }
      return -1;
    }

    function checkWin(r, c, p) {
      const dirs = [[0,1], [1,0], [1,1], [1,-1]];
      for (let [dr, dc] of dirs) {
        let cnt = 1;
        for (let s = 1; s <= 3; s++) {
          let nr = r + dr*s, nc = c + dc*s;
          if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && board[nr][nc] === p) cnt++; else break;
        }
        for (let s = 1; s <= 3; s++) {
          let nr = r - dr*s, nc = c - dc*s;
          if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && board[nr][nc] === p) cnt++; else break;
        }
        if (cnt >= 4) return true;
      }
      return false;
    }

    function updateStatus() {
      if (myTurn) {
        statusEl.textContent = (role === 1 ? "🟡 Votre tour (Jaune)" : "🔴 Votre tour (Rouge)");
        statusEl.style.color = (role === 1 ? "#ffe600" : "#ff0055");
      } else {
        statusEl.textContent = isAi ? "🤖 L'IA réfléchit..." : "⏳ Tour de l'adversaire...";
        statusEl.style.color = "#8b9bb4";
      }
    }

    function endGame(winner) {
      playing = false;
      if (winner === 0) {
        statusEl.textContent = "🤝 Match nul !";
        statusEl.style.color = "#ffe600";
      } else if (winner === role) {
        statusEl.textContent = "🏆 VICTOIRE ÉCLATANTE !";
        statusEl.style.color = "#00ff66";
      } else {
        statusEl.textContent = isAi ? "💀 L'IA a gagné !" : "💀 Défaite...";
        statusEl.style.color = "#ff0055";
      }
      ovActions.style.display = "flex";
    }

    function resetGame() {
      board = Array(ROWS).fill(0).map(() => Array(COLS).fill(0));
      renderBoard();
      myRematch = false; oppRematch = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "";
      ovActions.style.display = "none";
      playing = true; myTurn = (role === 1);
      updateStatus();
    }

    btnRematch.onclick = () => {
      if (isAi) { resetGame(); return; }
      myRematch = true;
      btnRematch.textContent = "⏳ En attente...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) resetGame();
    };

    let ws = null;
    if (isAi) {
      startAiMode();
    } else {
      ws = new WebSocket('ws://' + location.hostname + ':81/');
      ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "c4", name: myPseudo }));
      ws.onmessage = (e) => {
        try {
          const d = JSON.parse(e.data);
          if (d.t === "joined") {
            role = d.role || d.p;
            badge.textContent = role === 1 ? "🟡 Joueur 1" : "🔴 Joueur 2";
          } else if (d.t === "start") {
            aiOpt.style.display = 'none';
            role = d.role || role || 1;
            playing = true;
            myTurn = (role === 1);
            badge.textContent = "⚡ En Direct"; badge.style.color = "#00ff66";
            updateStatus();
          } else if (d.t === "c4_move") {
            board[d.r][d.c] = (role === 1 ? 2 : 1);
            renderBoard();
            if (checkWin(d.r, d.c, board[d.r][d.c])) {
              endGame(board[d.r][d.c]);
            } else if (board[0].every(c => c !== 0)) {
              endGame(0);
            } else {
              myTurn = true;
              updateStatus();
            }
          } else if (d.t === "rematch") {
            oppRematch = true;
            if (myRematch) resetGame();
          } else if (d.t === "opp_left") {
            playing = false;
            statusEl.textContent = "Adversaire déconnecté";
            ovActions.style.display = "flex";
          }
        } catch(err){}
      };
    }
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   2. BATAILLE NAVALE (1V1 & VS IA TACTIQUE)
// ----------------------------------------------------------------------------------
const char MP_NAVAL_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Bataille Navale Pro</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: #040814; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: flex-start;
      min-height: 100vh; padding: 10px;
    }
    header { width: 100%; max-width: 380px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 6px; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .status-box { font-size: 1rem; font-weight: 900; margin-bottom: 8px; text-align: center; min-height: 24px; color: #00f3ff; }
    .grids-wrap { display: flex; flex-direction: column; gap: 12px; width: 100%; max-width: 340px; align-items: center; }
    .grid-title { font-size: 0.82rem; font-weight: 800; color: #8b9bb4; text-transform: uppercase; margin-bottom: 4px; }
    .grid {
      display: grid; grid-template-columns: repeat(6, 38px); gap: 4px;
      background: rgba(0, 30, 80, 0.5); padding: 8px; border-radius: 14px;
      border: 2px solid rgba(0,243,255,0.3); box-shadow: 0 6px 20px rgba(0,0,0,0.6);
    }
    .cell {
      width: 38px; height: 38px; border-radius: 8px; background: #081028;
      border: 1px solid rgba(255,255,255,0.1); cursor: pointer; display: flex; align-items: center; justify-content: center;
      font-size: 1.1rem; font-weight: 900; transition: all 0.1s;
    }
    .cell.ship { background: #0055bb; border-color: #00f3ff; box-shadow: inset 0 0 8px rgba(0,243,255,0.5); }
    .cell.hit { background: #cc0033; border-color: #ff0055; box-shadow: 0 0 10px #ff0055; color: #fff; }
    .cell.miss { background: #1a233a; color: #8b9bb4; font-size: 0.8rem; }
    .act-btn { padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem; cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; transition: all 0.15s; }
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 15px rgba(0,243,255,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-ai { background: linear-gradient(180deg, #a855f7, #6b21a8); color: #fff; box-shadow: 0 4px 15px rgba(168,85,247,0.4); margin-top: 8px; }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span id="badge" style="color:#00f3ff; font-weight:800; font-size:0.85rem;">Connexion...</span>
  </header>
  <div class="status-box" id="status">Placez 3 navires sur votre grille (3 cases)</div>

  <div class="grids-wrap">
    <div>
      <div class="grid-title">🎯 Tirs Radar (Grille Adverse)</div>
      <div class="grid" id="radar-grid"></div>
    </div>
    <div>
      <div class="grid-title">🛡️ Votre Flotte</div>
      <div class="grid" id="my-grid"></div>
    </div>
  </div>

  <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:300px; margin-top:14px;">
    <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
    <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
  </div>

  <div id="ai-opt" style="margin-top:10px; width:100%; max-width:300px;">
    <button class="act-btn btn-ai" onclick="startAiMode()">🤖 Jouer contre l'IA</button>
  </div>

  <script>
    const radarGrid = document.getElementById('radar-grid');
    const myGrid = document.getElementById('my-grid');
    const statusEl = document.getElementById('status'), badge = document.getElementById('badge');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');
    const aiOpt = document.getElementById('ai-opt');

    const SIZE = 6;
    let myShips = new Set(), aiShips = new Set();
    let myShots = new Set(), oppShots = new Set();
    let phase = 'place'; // 'place' ou 'battle'
    let role = 1, myTurn = false, playing = false;
    let isAi = new URLSearchParams(window.location.search).get('vs') === 'ai';
    let myRematch = false, oppRematch = false;

    for (let i = 0; i < SIZE * SIZE; i++) {
      const c1 = document.createElement('div');
      c1.className = 'cell'; c1.dataset.idx = i;
      c1.onclick = () => shoot(i);
      radarGrid.appendChild(c1);

      const c2 = document.createElement('div');
      c2.className = 'cell'; c2.dataset.idx = i;
      c2.onclick = () => togglePlace(i);
      myGrid.appendChild(c2);
    }

    function togglePlace(idx) {
      if (phase !== 'place') return;
      if (myShips.has(idx)) {
        myShips.delete(idx);
      } else if (myShips.size < 4) {
        myShips.add(idx);
      }
      renderMyGrid();
      if (myShips.size === 4) {
        phase = 'battle';
        if (isAi) {
          setupAiShips();
          playing = true; myTurn = true;
          statusEl.textContent = "⚔️ À vous d'ouvrir le feu !";
          statusEl.style.color = "#00ff66";
        } else {
          statusEl.textContent = "Flotte prête ! En attente de l'adversaire...";
          ws.send(JSON.stringify({ t: "nav_ready" }));
        }
      } else {
        statusEl.textContent = "Placez encore " + (4 - myShips.size) + " navires (cases)";
      }
    }

    function renderMyGrid() {
      for (let i = 0; i < SIZE * SIZE; i++) {
        const cell = myGrid.children[i];
        if (oppShots.has(i)) {
          cell.className = 'cell ' + (myShips.has(i) ? 'hit' : 'miss');
          cell.textContent = myShips.has(i) ? '💥' : '•';
        } else if (myShips.has(i)) {
          cell.className = 'cell ship';
          cell.textContent = '🚢';
        } else {
          cell.className = 'cell'; cell.textContent = '';
        }
      }
    }

    function renderRadar() {
      for (let i = 0; i < SIZE * SIZE; i++) {
        const cell = radarGrid.children[i];
        if (myShots.has(i)) {
          const isHit = isAi ? aiShips.has(i) : cell.classList.contains('hit');
          cell.className = 'cell ' + (isHit ? 'hit' : 'miss');
          cell.textContent = isHit ? '💥' : '•';
        }
      }
    }

    function startAiMode() {
      isAi = true;
      aiOpt.style.display = 'none';
      badge.textContent = "🤖 Solo vs IA"; badge.style.color = "#a855f7";
      phase = 'place';
      statusEl.textContent = "Placez 4 navires sur votre grille (cliquez)";
    }

    function setupAiShips() {
      aiShips.clear();
      while (aiShips.size < 4) {
        aiShips.add(Math.floor(Math.random() * (SIZE * SIZE)));
      }
    }

    function shoot(idx) {
      if (!playing || !myTurn || myShots.has(idx)) return;
      myShots.add(idx);

      if (isAi) {
        const hit = aiShips.has(idx);
        const cell = radarGrid.children[idx];
        if (hit) {
          cell.className = 'cell hit'; cell.textContent = '💥';
          let allHit = true;
          for (let s of aiShips) { if (!myShots.has(s)) allHit = false; }
          if (allHit) { endGame(true); return; }
        } else {
          cell.className = 'cell miss'; cell.textContent = '•';
        }
        myTurn = false;
        statusEl.textContent = "🤖 L'IA vise votre flotte...";
        statusEl.style.color = "#ffaa00";
        setTimeout(aiShoot, 800);
      } else {
        myTurn = false;
        statusEl.textContent = "Tir envoyé... Analyse radar...";
        ws.send(JSON.stringify({ t: "nav_shot", idx: idx }));
      }
    }

    function aiShoot() {
      if (!playing) return;
      let target = -1;
      // Pick random unshot cell
      const available = [];
      for (let i = 0; i < SIZE * SIZE; i++) {
        if (!oppShots.has(i)) available.push(i);
      }
      if (available.length === 0) return;
      target = available[Math.floor(Math.random() * available.length)];
      oppShots.add(target);
      renderMyGrid();

      let allPlayerHit = true;
      for (let s of myShips) { if (!oppShots.has(s)) allPlayerHit = false; }
      if (allPlayerHit) {
        endGame(false);
      } else {
        myTurn = true;
        statusEl.textContent = "⚔️ À votre tour de tirer !";
        statusEl.style.color = "#00ff66";
      }
    }

    function endGame(iWon) {
      playing = false;
      statusEl.textContent = iWon ? "🏆 VICTOIRE ! FLOTTE ENNEMIE ANÉANTIE !" : "💀 DÉFAITE ! VOTRE FLOTTE A COULÉ...";
      statusEl.style.color = iWon ? "#00ff66" : "#ff0055";
      ovActions.style.display = "flex";
    }

    function resetGame() {
      myShips.clear(); aiShips.clear();
      myShots.clear(); oppShots.clear();
      phase = 'place';
      ovActions.style.display = "none";
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      for (let i = 0; i < SIZE * SIZE; i++) {
        radarGrid.children[i].className = 'cell'; radarGrid.children[i].textContent = '';
        myGrid.children[i].className = 'cell'; myGrid.children[i].textContent = '';
      }
      statusEl.textContent = "Placez 4 navires sur votre grille";
      statusEl.style.color = "#00f3ff";
    }

    btnRematch.onclick = () => {
      if (isAi) { resetGame(); return; }
      myRematch = true;
      btnRematch.textContent = "⏳ En attente...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) resetGame();
    };

    let ws = null;
    if (isAi) {
      startAiMode();
    } else {
      ws = new WebSocket('ws://' + location.hostname + ':81/');
      ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "naval", name: myPseudo }));
      ws.onmessage = (e) => {
        try {
          const d = JSON.parse(e.data);
          if (d.t === "joined") {
            role = d.role || d.p;
            badge.textContent = role === 1 ? "🔵 Amiral 1" : "🔴 Amiral 2";
          } else if (d.t === "start") {
            aiOpt.style.display = 'none';
            playing = true;
            badge.textContent = "⚡ En Direct"; badge.style.color = "#00ff66";
            myTurn = (role === 1);
            statusEl.textContent = myTurn ? "⚔️ À votre tour de tirer !" : "⏳ L'adversaire vise...";
          } else if (d.t === "nav_shot") {
            oppShots.add(d.idx);
            renderMyGrid();
            const hit = myShips.has(d.idx);
            ws.send(JSON.stringify({ t: "nav_res", idx: d.idx, hit: hit }));
            let allHit = true;
            for (let s of myShips) { if (!oppShots.has(s)) allHit = false; }
            if (allHit) {
              ws.send(JSON.stringify({ t: "nav_over" }));
              endGame(false);
            } else {
              myTurn = true;
              statusEl.textContent = "⚔️ À votre tour de tirer !";
            }
          } else if (d.t === "nav_res") {
            const cell = radarGrid.children[d.idx];
            cell.className = 'cell ' + (d.hit ? 'hit' : 'miss');
            cell.textContent = d.hit ? '💥' : '•';
            statusEl.textContent = d.hit ? "💥 TOUCHÉ ! À l'adversaire..." : "🌊 Dans l'eau ! À l'adversaire...";
          } else if (d.t === "nav_over") {
            endGame(true);
          } else if (d.t === "rematch") {
            oppRematch = true;
            if (myRematch) resetGame();
          }
        } catch(err){}
      };
    }
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   3. MORPION CYBER (1V1 & VS IA MINIMAX)
// ----------------------------------------------------------------------------------
const char MP_TICTACTOE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Morpion Cyber Pro</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: #060914; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      min-height: 100vh; padding: 16px;
    }
    header { width: 100%; max-width: 320px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 12px; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .turn-status { font-size: 1.25rem; font-weight: 900; margin-bottom: 16px; text-align: center; }
    #board {
      display: grid; grid-template-columns: repeat(3, 90px); gap: 10px;
      background: rgba(0, 243, 255, 0.08); padding: 12px; border-radius: 20px;
      border: 2px solid rgba(0, 243, 255, 0.4); box-shadow: 0 0 30px rgba(0,243,255,0.2);
    }
    .cell {
      width: 90px; height: 90px; border-radius: 14px; background: #0c1224;
      border: 2px solid rgba(255,255,255,0.12); display: flex; align-items: center; justify-content: center;
      font-size: 3rem; font-weight: 900; cursor: pointer; transition: all 0.15s;
    }
    .cell.x { color: #00f3ff; text-shadow: 0 0 20px #00f3ff; }
    .cell.o { color: #ff0055; text-shadow: 0 0 20px #ff0055; }
    .act-btn { padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem; cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; transition: all 0.15s; }
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 15px rgba(0,243,255,0.4); }
    .btn-ai { background: linear-gradient(180deg, #a855f7, #6b21a8); color: #fff; box-shadow: 0 4px 15px rgba(168,85,247,0.4); margin-top: 8px; }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span id="badge" style="color:#00f3ff; font-weight:800; font-size:0.85rem;">Connexion...</span>
  </header>
  <div class="turn-status" id="status">En attente d'un adversaire...</div>
  <div id="board"></div>

  <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:290px; margin-top:16px;">
    <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
    <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
  </div>

  <div id="ai-opt" style="margin-top:10px; width:100%; max-width:290px;">
    <button class="act-btn btn-ai" onclick="startAiMode()">🤖 Jouer contre l'IA</button>
  </div>

  <script>
    const boardEl = document.getElementById('board');
    const statusEl = document.getElementById('status'), badge = document.getElementById('badge');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');
    const aiOpt = document.getElementById('ai-opt');

    let grid = Array(9).fill('');
    let role = 1, myTurn = false, playing = false;
    let isAi = new URLSearchParams(window.location.search).get('vs') === 'ai';
    let myRematch = false, oppRematch = false;

    for (let i = 0; i < 9; i++) {
      const cell = document.createElement('div');
      cell.className = 'cell'; cell.dataset.idx = i;
      cell.onclick = () => makeMove(i);
      boardEl.appendChild(cell);
    }

    function startAiMode() {
      isAi = true;
      aiOpt.style.display = 'none';
      badge.textContent = "🤖 Solo vs IA"; badge.style.color = "#a855f7";
      role = 1; playing = true; myTurn = true;
      updateStatus();
    }

    function makeMove(i) {
      if (!playing || !myTurn || grid[i] !== '') return;
      const sym = (role === 1 ? 'X' : 'O');
      grid[i] = sym;
      renderBoard();

      if (checkWin(sym)) {
        endGame(sym);
      } else if (grid.every(c => c !== '')) {
        endGame('draw');
      } else {
        myTurn = false;
        updateStatus();
        if (isAi) {
          setTimeout(aiMove, 500);
        } else {
          ws.send(JSON.stringify({ t: "ttt_move", idx: i, sym: sym }));
        }
      }
    }

    function aiMove() {
      if (!playing) return;
      const aiSym = 'O', userSym = 'X';
      // 1. Win if possible
      let move = findWinningMove(aiSym);
      // 2. Block user
      if (move < 0) move = findWinningMove(userSym);
      // 3. Center
      if (move < 0 && grid[4] === '') move = 4;
      // 4. Random available
      if (move < 0) {
        const free = grid.map((v, i) => v === '' ? i : -1).filter(v => v >= 0);
        if (free.length > 0) move = free[Math.floor(Math.random() * free.length)];
      }

      if (move >= 0) {
        grid[move] = aiSym;
        renderBoard();
        if (checkWin(aiSym)) {
          endGame(aiSym);
        } else if (grid.every(c => c !== '')) {
          endGame('draw');
        } else {
          myTurn = true;
          updateStatus();
        }
      }
    }

    function findWinningMove(sym) {
      const lines = [
        [0,1,2],[3,4,5],[6,7,8],
        [0,3,6],[1,4,7],[2,5,8],
        [0,4,8],[2,4,6]
      ];
      for (let l of lines) {
        const vals = [grid[l[0]], grid[l[1]], grid[l[2]]];
        if (vals.filter(v => v === sym).length === 2 && vals.includes('')) {
          return l[vals.indexOf('')];
        }
      }
      return -1;
    }

    function renderBoard() {
      for (let i = 0; i < 9; i++) {
        const c = boardEl.children[i];
        c.textContent = grid[i];
        c.className = 'cell ' + (grid[i] === 'X' ? 'x' : grid[i] === 'O' ? 'o' : '');
      }
    }

    function checkWin(s) {
      const lines = [[0,1,2],[3,4,5],[6,7,8],[0,3,6],[1,4,7],[2,5,8],[0,4,8],[2,4,6]];
      return lines.some(l => grid[l[0]] === s && grid[l[1]] === s && grid[l[2]] === s);
    }

    function updateStatus() {
      if (myTurn) {
        statusEl.textContent = (role === 1 ? "⚡ À vous de jouer (X)" : "⚡ À vous de jouer (O)");
        statusEl.style.color = (role === 1 ? "#00f3ff" : "#ff0055");
      } else {
        statusEl.textContent = isAi ? "🤖 L'IA réfléchit..." : "⏳ Tour de l'adversaire...";
        statusEl.style.color = "#8b9bb4";
      }
    }

    function endGame(winner) {
      playing = false;
      if (winner === 'draw') {
        statusEl.textContent = "🤝 Match nul !";
        statusEl.style.color = "#ffe600";
      } else {
        const mySym = (role === 1 ? 'X' : 'O');
        const iWon = (winner === mySym);
        statusEl.textContent = iWon ? "🏆 VICTOIRE !" : (isAi ? "💀 L'IA a gagné !" : "💀 Défaite...");
        statusEl.style.color = iWon ? "#00ff66" : "#ff0055";
      }
      ovActions.style.display = "flex";
    }

    function resetGame() {
      grid = Array(9).fill('');
      renderBoard();
      ovActions.style.display = "none";
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      playing = true; myTurn = (role === 1);
      updateStatus();
    }

    btnRematch.onclick = () => {
      if (isAi) { resetGame(); return; }
      myRematch = true;
      btnRematch.textContent = "⏳ En attente...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) resetGame();
    };

    let ws = null;
    if (isAi) {
      startAiMode();
    } else {
      ws = new WebSocket('ws://' + location.hostname + ':81/');
      ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "ttt", name: localStorage.getItem('arcade_pseudo') || 'Joueur' }));
      ws.onmessage = (e) => {
        try {
          const d = JSON.parse(e.data);
          if (d.t === "joined") {
            role = d.role || d.p;
            badge.textContent = role === 1 ? "🔵 Joueur 1 (X)" : "🔴 Joueur 2 (O)";
          } else if (d.t === "start") {
            aiOpt.style.display = 'none';
            playing = true;
            role = d.role || role || 1;
            myTurn = (role === 1);
            badge.textContent = "⚡ En Direct"; badge.style.color = "#00ff66";
            updateStatus();
          } else if (d.t === "ttt_move") {
            grid[d.idx] = d.sym;
            renderBoard();
            if (checkWin(d.sym)) {
              endGame(d.sym);
            } else if (grid.every(c => c !== '')) {
              endGame('draw');
            } else {
              myTurn = true;
              updateStatus();
            }
          } else if (d.t === "rematch") {
            oppRematch = true;
            if (myRematch) resetGame();
          }
        } catch(err){}
      };
    }
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   4. DUEL DE MEMOIRE (1V1 & VS IA COGNITIVE)
// ----------------------------------------------------------------------------------
const char MP_MEMORY_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Duel de Mémoire</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: radial-gradient(circle at 50% 20%, #150928 0%, #06030d 100%);
      color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: flex-start;
      min-height: 100vh; padding: 12px;
    }
    header { width: 100%; max-width: 360px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .status-badge { font-size: 0.8rem; font-weight: 800; padding: 4px 12px; border-radius: 20px; background: rgba(168,85,247,0.15); color: #a855f7; border: 1px solid #a855f7; }
    .score-banner {
      display: flex; justify-content: space-between; width: 100%; max-width: 340px;
      padding: 8px 16px; background: rgba(255,255,255,0.04); border-radius: 14px; border: 1px solid rgba(255,255,255,0.08);
      margin-bottom: 10px; font-weight: 900; font-size: 1.1rem;
    }
    .turn-label { font-size: 1.05rem; font-weight: 900; margin-bottom: 12px; min-height: 26px; }
    .deck-grid {
      display: grid; grid-template-columns: repeat(4, 72px); gap: 10px;
    }
    .card {
      width: 72px; height: 80px; border-radius: 12px; background: linear-gradient(145deg, #241240, #130922);
      border: 2px solid rgba(168,85,247,0.4); display: flex; align-items: center; justify-content: center;
      font-size: 2rem; cursor: pointer; transition: transform 0.2s, background 0.2s;
      box-shadow: 0 4px 12px rgba(0,0,0,0.6);
    }
    .card:active { transform: scale(0.95); }
    .card.revealed { background: #381b66; border-color: #00f3ff; box-shadow: 0 0 15px rgba(0,243,255,0.4); }
    .card.matched { background: #004422; border-color: #00ff66; opacity: 0.85; pointer-events: none; }
    .act-btn { padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem; cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; transition: all 0.15s; }
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 15px rgba(0,243,255,0.4); }
    .btn-ai { background: linear-gradient(180deg, #a855f7, #6b21a8); color: #fff; box-shadow: 0 4px 15px rgba(168,85,247,0.4); margin-top: 8px; }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="status-badge" id="badge">Connexion...</span>
  </header>

  <div class="score-banner">
    <span style="color:#00f3ff;"><span id="p1-name">Vous</span> : <span id="s1">0</span></span>
    <span style="color:#ff0055;"><span id="p2-name">Adversaire</span> : <span id="s2">0</span></span>
  </div>

  <div class="turn-label" id="status">En attente d'un adversaire...</div>
  <div class="deck-grid" id="deck"></div>

  <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:300px; margin-top:16px;">
    <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
    <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
  </div>

  <div id="ai-opt" style="margin-top:10px; width:100%; max-width:300px;">
    <button class="act-btn btn-ai" onclick="startAiMode()">🤖 Jouer contre l'IA</button>
  </div>

  <script>
    const deckEl = document.getElementById('deck');
    const statusEl = document.getElementById('status'), badge = document.getElementById('badge');
    const s1El = document.getElementById('s1'), s2El = document.getElementById('s2');
    const p1Name = document.getElementById('p1-name'), p2Name = document.getElementById('p2-name');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');
    const aiOpt = document.getElementById('ai-opt');

    const EMOJIS = ['🚀', '💎', '👾', '⚡', '🔥', '👑', '⭐', '🍀'];
    let cards = [];
    let role = 1, myTurn = false, playing = false;
    let score1 = 0, score2 = 0;
    let flippedIndices = [];
    let isAi = new URLSearchParams(window.location.search).get('vs') === 'ai';
    let aiMemory = {}; // card value -> index seen
    let myRematch = false, oppRematch = false;

    function buildDeck() {
      cards = [...EMOJIS, ...EMOJIS].sort(() => Math.random() - 0.5);
      deckEl.innerHTML = '';
      cards.forEach((val, idx) => {
        const c = document.createElement('div');
        c.className = 'card'; c.dataset.idx = idx;
        c.onclick = () => flipCard(idx);
        deckEl.appendChild(c);
      });
    }

    function startAiMode() {
      isAi = true;
      aiOpt.style.display = 'none';
      badge.textContent = "🤖 Solo vs IA"; badge.style.color = "#a855f7";
      p2Name.textContent = "IA Bot";
      role = 1; playing = true; myTurn = true;
      buildDeck();
      updateStatus();
    }

    function flipCard(idx) {
      if (!playing || !myTurn || flippedIndices.length >= 2) return;
      const el = deckEl.children[idx];
      if (el.classList.contains('revealed') || el.classList.contains('matched')) return;

      doFlip(idx);
      if (isAi) {
        if (flippedIndices.length === 2) checkPair(1);
      } else {
        ws.send(JSON.stringify({ t: "mem_flip", idx: idx }));
      }
    }

    function doFlip(idx) {
      flippedIndices.push(idx);
      const el = deckEl.children[idx];
      el.textContent = cards[idx];
      el.classList.add('revealed');
      aiMemory[cards[idx]] = idx;
    }

    function checkPair(activeRole) {
      const [i1, i2] = flippedIndices;
      if (cards[i1] === cards[i2]) {
        // Matched!
        setTimeout(() => {
          deckEl.children[i1].classList.replace('revealed', 'matched');
          deckEl.children[i2].classList.replace('revealed', 'matched');
          flippedIndices = [];
          if (activeRole === 1) score1++; else score2++;
          s1El.textContent = score1; s2El.textContent = score2;
          checkGameEnd();
          if (playing) {
            if (activeRole === 1) { myTurn = true; updateStatus(); }
            else if (isAi) setTimeout(aiTurn, 800);
          }
        }, 600);
      } else {
        // Not matched
        setTimeout(() => {
          deckEl.children[i1].classList.remove('revealed'); deckEl.children[i1].textContent = '';
          deckEl.children[i2].classList.remove('revealed'); deckEl.children[i2].textContent = '';
          flippedIndices = [];
          if (activeRole === 1) {
            myTurn = false;
            updateStatus();
            if (isAi) setTimeout(aiTurn, 800);
          } else {
            myTurn = true;
            updateStatus();
          }
        }, 1000);
      }
    }

    function aiTurn() {
      if (!playing) return;
      statusEl.textContent = "🤖 L'IA cherche une paire...";
      statusEl.style.color = "#a855f7";

      const unrevealed = [];
      for (let i = 0; i < 16; i++) {
        if (!deckEl.children[i].classList.contains('matched')) unrevealed.push(i);
      }
      if (unrevealed.length < 2) return;

      // Pick first card
      const c1 = unrevealed[Math.floor(Math.random() * unrevealed.length)];
      doFlip(c1);

      setTimeout(() => {
        // Pick second card
        const remaining = unrevealed.filter(i => i !== c1);
        const c2 = remaining[Math.floor(Math.random() * remaining.length)];
        doFlip(c2);
        checkPair(2);
      }, 700);
    }

    function updateStatus() {
      if (myTurn) {
        statusEl.textContent = "⚡ À vous de retourner 2 cartes !";
        statusEl.style.color = "#00f3ff";
      } else {
        statusEl.textContent = isAi ? "🤖 L'IA joue..." : "⏳ Tour de l'adversaire...";
        statusEl.style.color = "#8b9bb4";
      }
    }

    function checkGameEnd() {
      if (score1 + score2 >= 8) {
        playing = false;
        if (score1 > score2) {
          statusEl.textContent = "🏆 VICTOIRE ÉCLATANTE !";
          statusEl.style.color = "#00ff66";
        } else if (score2 > score1) {
          statusEl.textContent = isAi ? "💀 L'IA gagne le duel !" : "💀 Défaite...";
          statusEl.style.color = "#ff0055";
        } else {
          statusEl.textContent = "🤝 Égalité parfaite !";
          statusEl.style.color = "#ffe600";
        }
        ovActions.style.display = "flex";
      }
    }

    function resetGame() {
      score1 = 0; score2 = 0;
      s1El.textContent = "0"; s2El.textContent = "0";
      flippedIndices = []; aiMemory = {};
      buildDeck();
      ovActions.style.display = "none";
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      playing = true; myTurn = (role === 1);
      updateStatus();
    }

    btnRematch.onclick = () => {
      if (isAi) { resetGame(); return; }
      myRematch = true;
      btnRematch.textContent = "⏳ En attente...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) resetGame();
    };

    let ws = null;
    if (isAi) {
      startAiMode();
    } else {
      buildDeck();
      ws = new WebSocket('ws://' + location.hostname + ':81/');
      ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "memory", name: localStorage.getItem('arcade_pseudo') || 'Joueur' }));
      ws.onmessage = (e) => {
        try {
          const d = JSON.parse(e.data);
          if (d.t === "joined") {
            role = d.role || d.p;
            badge.textContent = role === 1 ? "🔵 Joueur 1" : "🔴 Joueur 2";
          } else if (d.t === "start") {
            aiOpt.style.display = 'none';
            playing = true;
            role = d.role || role || 1;
            myTurn = (role === 1);
            badge.textContent = "⚡ En Direct"; badge.style.color = "#00ff66";
            updateStatus();
          } else if (d.t === "mem_flip") {
            doFlip(d.idx);
            if (flippedIndices.length === 2) checkPair(role === 1 ? 2 : 1);
          } else if (d.t === "rematch") {
            oppRematch = true;
            if (myRematch) resetGame();
          }
        } catch(err){}
      };
    }
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   5. MASTERMIND DUEL (1V1 & VS IA DEDUCTIVE)
// ----------------------------------------------------------------------------------
const char MP_MASTERMIND_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Mastermind Duel</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: radial-gradient(circle at 50% 20%, #0d1a30 0%, #040813 100%);
      color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: flex-start;
      min-height: 100vh; padding: 12px;
    }
    header { width: 100%; max-width: 360px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .status-badge { font-size: 0.8rem; font-weight: 800; padding: 4px 12px; border-radius: 20px; background: rgba(0,243,255,0.15); color: #00f3ff; border: 1px solid #00f3ff; }
    .board-wrap {
      width: 100%; max-width: 340px; background: rgba(14,24,52,0.6); border: 2px solid rgba(0,243,255,0.3);
      border-radius: 18px; padding: 14px; display: flex; flex-direction: column; gap: 10px;
      box-shadow: 0 10px 30px rgba(0,0,0,0.8);
    }
    .history-box {
      display: flex; flex-direction: column; gap: 6px; max-height: 220px; overflow-y: auto; padding-right: 4px;
    }
    .row {
      display: flex; justify-content: space-between; align-items: center; background: rgba(0,0,0,0.4);
      padding: 6px 10px; border-radius: 10px; border: 1px solid rgba(255,255,255,0.08);
    }
    .pegs-row { display: flex; gap: 8px; }
    .peg {
      width: 24px; height: 24px; border-radius: 50%; border: 1px solid rgba(255,255,255,0.2);
    }
    .peg-c0 { background: #ff0055; box-shadow: 0 0 8px #ff0055; }
    .peg-c1 { background: #00f3ff; box-shadow: 0 0 8px #00f3ff; }
    .peg-c2 { background: #00ff66; box-shadow: 0 0 8px #00ff66; }
    .peg-c3 { background: #ffe600; box-shadow: 0 0 8px #ffe600; }
    .peg-c4 { background: #a855f7; box-shadow: 0 0 8px #a855f7; }
    .peg-c5 { background: #ff7700; box-shadow: 0 0 8px #ff7700; }

    .hints { display: flex; gap: 4px; }
    .hint-black { width: 10px; height: 10px; border-radius: 50%; background: #ff0055; box-shadow: 0 0 4px #ff0055; }
    .hint-white { width: 10px; height: 10px; border-radius: 50%; background: #fff; box-shadow: 0 0 4px #fff; }

    .current-guess {
      display: flex; justify-content: space-between; align-items: center;
      background: rgba(0,243,255,0.08); padding: 10px; border-radius: 12px; border: 1px dashed rgba(0,243,255,0.4);
    }
    .active-slot {
      width: 36px; height: 36px; border-radius: 50%; background: #060b18;
      border: 2px dashed rgba(255,255,255,0.3); display: flex; align-items: center; justify-content: center; cursor: pointer;
    }
    .palette { display: flex; gap: 8px; justify-content: center; margin-top: 8px; }
    .palette-btn {
      width: 34px; height: 34px; border-radius: 50%; cursor: pointer; border: 2px solid #fff;
    }
    .act-btn { padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem; cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; transition: all 0.15s; }
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 15px rgba(0,243,255,0.4); }
    .btn-ai { background: linear-gradient(180deg, #a855f7, #6b21a8); color: #fff; box-shadow: 0 4px 15px rgba(168,85,247,0.4); margin-top: 8px; }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="status-badge" id="badge">Connexion...</span>
  </header>

  <div style="font-size:1rem; font-weight:900; color:#ffe600; margin-bottom:8px;" id="status">Décodez le code de 4 couleurs !</div>

  <div class="board-wrap">
    <div class="history-box" id="history"></div>

    <div class="current-guess">
      <div style="display:flex; gap:8px;">
        <div class="active-slot" onclick="clearSlot(0)"></div>
        <div class="active-slot" onclick="clearSlot(1)"></div>
        <div class="active-slot" onclick="clearSlot(2)"></div>
        <div class="active-slot" onclick="clearSlot(3)"></div>
      </div>
      <button class="act-btn btn-rematch" style="width:auto; padding:8px 16px; font-size:0.85rem;" onclick="submitGuess()">Valider</button>
    </div>

    <div class="palette">
      <div class="palette-btn peg-c0" onclick="pickColor(0)"></div>
      <div class="palette-btn peg-c1" onclick="pickColor(1)"></div>
      <div class="palette-btn peg-c2" onclick="pickColor(2)"></div>
      <div class="palette-btn peg-c3" onclick="pickColor(3)"></div>
      <div class="palette-btn peg-c4" onclick="pickColor(4)"></div>
      <div class="palette-btn peg-c5" onclick="pickColor(5)"></div>
    </div>
  </div>

  <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:300px; margin-top:14px;">
    <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
    <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
  </div>

  <div id="ai-opt" style="margin-top:10px; width:100%; max-width:300px;">
    <button class="act-btn btn-ai" onclick="startAiMode()">🤖 Jouer contre l'IA</button>
  </div>

  <script>
    const historyEl = document.getElementById('history');
    const statusEl = document.getElementById('status'), badge = document.getElementById('badge');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');
    const aiOpt = document.getElementById('ai-opt');
    const slots = document.querySelectorAll('.active-slot');

    let secretCode = [];
    let curGuess = [-1, -1, -1, -1];
    let playing = false, isAi = new URLSearchParams(window.location.search).get('vs') === 'ai';
    let aiTimer = null, aiProgress = 0;
    let myRematch = false, oppRematch = false;

    function initSecret() {
      secretCode = [];
      for (let i = 0; i < 4; i++) secretCode.push(Math.floor(Math.random() * 6));
    }

    function startAiMode() {
      isAi = true;
      aiOpt.style.display = 'none';
      badge.textContent = "🤖 Solo vs IA"; badge.style.color = "#a855f7";
      initSecret();
      playing = true;
      statusEl.textContent = "⚡ Course au décodage : bats l'IA !";
      // AI solves in 5 to 7 attempts (every 7 seconds)
      aiProgress = 0;
      clearInterval(aiTimer);
      aiTimer = setInterval(() => {
        if (!playing) return;
        aiProgress++;
        if (aiProgress >= 6) {
          endGame(false);
        }
      }, 7000);
    }

    function pickColor(c) {
      if (!playing) return;
      for (let i = 0; i < 4; i++) {
        if (curGuess[i] === -1) {
          curGuess[i] = c;
          slots[i].className = 'active-slot peg-c' + c;
          break;
        }
      }
    }

    function clearSlot(i) {
      curGuess[i] = -1;
      slots[i].className = 'active-slot';
    }

    function submitGuess() {
      if (!playing || curGuess.includes(-1)) return;
      const b = evalHints(curGuess, secretCode);
      addHistoryRow(curGuess, b.exact, b.colorOnly);

      if (b.exact === 4) {
        endGame(true);
      } else {
        curGuess = [-1, -1, -1, -1];
        slots.forEach(s => s.className = 'active-slot');
        if (!isAi && ws) ws.send(JSON.stringify({ t: "mm_step", exact: b.exact }));
      }
    }

    function evalHints(guess, target) {
      let exact = 0, colorOnly = 0;
      let gUsed = [false, false, false, false], tUsed = [false, false, false, false];
      for (let i = 0; i < 4; i++) {
        if (guess[i] === target[i]) { exact++; gUsed[i] = true; tUsed[i] = true; }
      }
      for (let i = 0; i < 4; i++) {
        if (!gUsed[i]) {
          for (let j = 0; j < 4; j++) {
            if (!tUsed[j] && guess[i] === target[j]) { colorOnly++; tUsed[j] = true; break; }
          }
        }
      }
      return { exact, colorOnly };
    }

    function addHistoryRow(guess, exact, colorOnly) {
      const row = document.createElement('div');
      row.className = 'row';
      let pegsHtml = '<div class="pegs-row">';
      guess.forEach(c => pegsHtml += `<div class="peg peg-c${c}"></div>`);
      pegsHtml += '</div><div class="hints">';
      for (let i = 0; i < exact; i++) pegsHtml += '<div class="hint-black"></div>';
      for (let i = 0; i < colorOnly; i++) pegsHtml += '<div class="hint-white"></div>';
      pegsHtml += '</div>';
      row.innerHTML = pegsHtml;
      historyEl.appendChild(row);
      historyEl.scrollTop = historyEl.scrollHeight;
    }

    function endGame(iWon) {
      playing = false;
      clearInterval(aiTimer);
      statusEl.textContent = iWon ? "🏆 BRAVO ! CODE DÉCODÉ EN PREMIER !" : "💀 L'IA A DÉCODÉ LE CODE AVANT TOI !";
      statusEl.style.color = iWon ? "#00ff66" : "#ff0055";
      ovActions.style.display = "flex";
    }

    function resetGame() {
      historyEl.innerHTML = '';
      curGuess = [-1, -1, -1, -1];
      slots.forEach(s => s.className = 'active-slot');
      ovActions.style.display = "none";
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      startAiMode();
    }

    btnRematch.onclick = () => {
      if (isAi) { resetGame(); return; }
      myRematch = true;
      btnRematch.textContent = "⏳ En attente...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) resetGame();
    };

    let ws = null;
    if (isAi) {
      startAiMode();
    } else {
      initSecret();
      ws = new WebSocket('ws://' + location.hostname + ':81/');
      ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "mastermind", name: localStorage.getItem('arcade_pseudo') || 'Joueur' }));
      ws.onmessage = (e) => {
        try {
          const d = JSON.parse(e.data);
          if (d.t === "start") {
            aiOpt.style.display = 'none';
            playing = true;
            badge.textContent = "⚡ En Direct"; badge.style.color = "#00ff66";
          } else if (d.t === "mm_step" && d.exact === 4) {
            endGame(false);
          } else if (d.t === "rematch") {
            oppRematch = true;
            if (myRematch) resetGame();
          }
        } catch(err){}
      };
    }
  </script>
</body>
</html>
)rawliteral";

#endif // GAME_PACK_MP_REFLEXION_H
