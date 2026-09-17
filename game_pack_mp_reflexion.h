#ifndef GAME_PACK_MP_REFLEXION_H
#define GAME_PACK_MP_REFLEXION_H

#include <pgmspace.h>

// ==================================================================================
//   ESP32 ARCADE - 5 JEUX MULTIJOUEURS DE REFLEXION & STRATEGIE (1V1 LOCAL WI-FI)
//   1. PUISSANCE 4  |  2. BATAILLE NAVALE  |  3. MORPION CYBER
//   4. DUEL DE MEMOIRE  |  5. MASTERMIND DUEL
// ==================================================================================

// ----------------------------------------------------------------------------------
//   1. PUISSANCE 4 EN LIGNE (CHASSIS PLASTIQUE MOULE, JETONS 3D A EFFET DE CHUTE)
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

  <script>
    const gridWrap = document.getElementById('grid-wrap');
    const statusEl = document.getElementById('status');
    const badge = document.getElementById('badge');
    const ovActions = document.getElementById('ov-actions');
    const btnRematch = document.getElementById('btn-rematch');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    const COLS = 7, ROWS = 6;
    let board = Array(ROWS).fill(0).map(() => Array(COLS).fill(0));
    let role = 0, myTurn = false, playing = false;
    let myRematch = false, oppRematch = false;

    for (let r = 0; r < ROWS; r++) {
      for (let c = 0; c < COLS; c++) {
        const s = document.createElement('div');
        s.className = 'slot'; s.dataset.col = c;
        s.onclick = () => dropToken(c);
        gridWrap.appendChild(s);
      }
    }

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "c4", name: myPseudo }));
    ws.onmessage = (e) => {
      const d = JSON.parse(e.data);
      if (d.t === "joined") {
        role = d.role || d.p;
        badge.textContent = role === 1 ? "🟡 Jeton Or (J1)" : "🔴 Jeton Rubis (J2)";
      } else if (d.t === "start") {
        playing = true;
        role = d.role || role || 1;
        myTurn = (role === 1);
        ovActions.style.display = "none";
        updateStatus();
      } else if (d.t === "move") {
        applyMove(d.col, role === 1 ? 2 : 1);
        myTurn = true;
        updateStatus();
      } else if (d.t === "rematch") {
        oppRematch = true;
        if (myRematch) {
          restartC4();
        } else {
          btnRematch.textContent = "⚡ L'adversaire veut rejouer ! [ACCEPTER]";
          btnRematch.style.background = "linear-gradient(180deg, #00ff66, #00cc44)";
          btnRematch.style.color = "#000";
        }
      } else if (d.t === "left" || d.t === "opp_left") {
        playing = false;
        statusEl.textContent = "Adversaire déconnecté";
        ovActions.style.display = "none";
      }
    };

    function updateStatus() {
      if (!playing) return;
      statusEl.textContent = myTurn ? "🟢 C'est à ton tour de jouer !" : "⏳ L'adversaire réfléchit...";
      statusEl.style.color = myTurn ? "#00ff66" : "#8b9bb4";
    }

    function dropToken(col) {
      if (!playing || !myTurn) return;
      if (applyMove(col, role)) {
        ws.send(JSON.stringify({ t: "move", col: col }));
        myTurn = false;
        updateStatus();
      }
    }

    function applyMove(col, player) {
      for (let r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] === 0) {
          board[r][col] = player;
          const idx = r * COLS + col;
          gridWrap.children[idx].className = 'slot ' + (player === 1 ? 'p1' : 'p2');
          checkWin(player);
          return true;
        }
      }
      return false;
    }

    function checkWin(p) {
      const check = (r, c, dr, dc) => {
        let count = 0;
        for (let i = 0; i < 4; i++) {
          const nr = r + dr * i, nc = c + dc * i;
          if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && board[nr][nc] === p) count++;
          else break;
        }
        return count === 4;
      };
      for (let r = 0; r < ROWS; r++) {
        for (let c = 0; c < COLS; c++) {
          if (check(r,c,0,1) || check(r,c,1,0) || check(r,c,1,1) || check(r,c,-1,1)) {
            playing = false;
            const won = (p === role);
            statusEl.textContent = won ? "🏆 VICTOIRE ! 4 Alignés !" : "💀 DÉFAITE !";
            statusEl.style.color = won ? "#ffe600" : "#ff0055";
            ovActions.style.display = "flex";
            btnRematch.disabled = false;
            btnRematch.textContent = "🔄 Rejouer (Revanche)";
            btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
            btnRematch.style.color = "#000";
            return;
          }
        }
      }
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.disabled = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) restartC4();
    };

    function restartC4() {
      myRematch = false; oppRematch = false;
      board = Array(ROWS).fill(0).map(() => Array(COLS).fill(0));
      for (let i = 0; i < gridWrap.children.length; i++) {
        gridWrap.children[i].className = 'slot';
      }
      ovActions.style.display = "none";
      btnRematch.disabled = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
      btnRematch.style.color = "#000";
      playing = true;
      myTurn = (role === 1);
      updateStatus();
    }
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   2. BATAILLE NAVALE 1V1 (SONAR SOUS-MARIN & RADAR PHOSPHORESCENT)
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
      background: #040812; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center;
      min-height: 100vh; padding: 12px; overflow-x: hidden;
    }
    header { width: 100%; max-width: 360px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .badge { font-size: 0.8rem; font-weight: 800; padding: 4px 12px; border-radius: 20px; background: rgba(0,243,255,0.15); color: #00f3ff; border: 1px solid #00f3ff; }
    .status-text { font-size: 0.98rem; font-weight: 800; margin-bottom: 10px; min-height: 26px; text-align: center; }
    .grid {
      display: grid; grid-template-columns: repeat(5, 58px); grid-template-rows: repeat(5, 58px); gap: 6px;
      margin-bottom: 14px; background: #071224; padding: 10px; border-radius: 18px; border: 2px solid #00f3ff;
      box-shadow: 0 0 30px rgba(0,243,255,0.25), inset 0 0 15px rgba(0,0,0,0.8);
      position: relative; overflow: hidden;
    }
    .cell {
      background: linear-gradient(145deg, #0f1c38, #091326); border-radius: 10px; display: flex; align-items: center; justify-content: center;
      font-size: 1.5rem; cursor: pointer; border: 1px solid rgba(0,243,255,0.2);
      box-shadow: inset 0 1px 0 rgba(255,255,255,0.1); transition: all 0.15s;
    }
    .cell:active { transform: scale(0.92); }
    .cell.ship { background: linear-gradient(145deg, #0077cc, #003366); border-color: #00f3ff; box-shadow: 0 0 12px rgba(0,243,255,0.5); }
    .cell.hit { background: linear-gradient(145deg, #ff0055, #990033); border-color: #ff4d79; box-shadow: 0 0 15px #ff0055; animation: hitPulse 1s infinite alternate; }
    @keyframes hitPulse { from { box-shadow: 0 0 10px #ff0055; } to { box-shadow: 0 0 22px #ff0055; } }
    .cell.miss { background: #1a233a; color: #00f3ff; border-color: rgba(0,243,255,0.1); }
    .btn-ready {
      width: 100%; max-width: 300px; padding: 14px; font-size: 1rem; font-weight: 900;
      background: linear-gradient(180deg, #00f3ff, #0088ff); border: none; border-radius: 12px; color: #000;
      cursor: pointer; box-shadow: 0 4px 15px rgba(0,243,255,0.4);
    }
    .act-btn { padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem; cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; transition: all 0.15s; }
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 15px rgba(0,243,255,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="badge" id="badge">Connexion...</span>
  </header>

  <div class="status-text" id="status">Place tes 4 navires secrets sur le radar !</div>
  <div class="grid" id="board"></div>
  <button class="btn-ready" id="ready-btn">Valider ma Flotte (0/4)</button>

  <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:300px; margin-top:14px;">
    <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
    <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
  </div>

  <script>
    const boardEl = document.getElementById('board');
    const statusEl = document.getElementById('status');
    const readyBtn = document.getElementById('ready-btn');
    const badge = document.getElementById('badge');
    const ovActions = document.getElementById('ov-actions');
    const btnRematch = document.getElementById('btn-rematch');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, phase = "place";
    let myShips = new Set();
    let myHits = 0, oppHits = 0;
    let myTurn = false, myReady = false, oppReady = false;
    let myRematch = false, oppRematch = false;

    for (let i = 0; i < 25; i++) {
      const c = document.createElement('div');
      c.className = 'cell'; c.dataset.i = i;
      c.onclick = () => handleCellClick(i, c);
      boardEl.appendChild(c);
    }

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "naval", name: myPseudo }));
    ws.onmessage = (e) => {
      const d = JSON.parse(e.data);
      if (d.t === "joined") {
        role = d.role || d.p;
        badge.textContent = role === 1 ? "🔵 Amiral J1" : "🔴 Amiral J2";
      } else if (d.t === "start") {
        role = d.role || role || 1;
      } else if (d.t === "ready_naval") {
        oppReady = true;
        if (myReady) startBattle();
      } else if (d.t === "start_battle") {
        startBattle();
      } else if (d.t === "shot") {
        const hit = myShips.has(d.i);
        ws.send(JSON.stringify({ t: "shot_result", i: d.i, hit: hit }));
        if (hit) {
          myHits++;
          if (myHits >= 4) {
            statusEl.textContent = "💀 Défaite ! Toute ta flotte a coulé.";
            phase = "end";
            showGameOver();
          }
        }
        myTurn = true;
        if (phase !== "end") statusEl.textContent = "🎯 À toi de tirer !";
      } else if (d.t === "shot_result") {
        const c = boardEl.children[d.i];
        if (d.hit) {
          c.className = "cell hit"; c.textContent = "💥";
          oppHits++;
          if (oppHits >= 4) {
            statusEl.textContent = "👑 VICTOIRE ! Tu as coulé la flotte ennemie !";
            phase = "end";
            showGameOver();
          }
        } else {
          c.className = "cell miss"; c.textContent = "🌊";
        }
        myTurn = false;
        if (phase !== "end") statusEl.textContent = "⏳ L'adversaire riposte...";
      } else if (d.t === "rematch") {
        oppRematch = true;
        if (myRematch) {
          restartNaval();
        } else {
          btnRematch.textContent = "⚡ L'adversaire veut rejouer ! [ACCEPTER]";
          btnRematch.style.background = "linear-gradient(180deg, #00ff66, #00cc44)";
          btnRematch.style.color = "#000";
        }
      } else if (d.t === "left" || d.t === "opp_left") {
        statusEl.textContent = "Adversaire déconnecté";
        phase = "end";
        ovActions.style.display = "none";
      }
    };

    function showGameOver() {
      ovActions.style.display = "flex";
      btnRematch.disabled = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
      btnRematch.style.color = "#000";
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.disabled = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) restartNaval();
    };

    function restartNaval() {
      myRematch = false; oppRematch = false;
      phase = "place";
      myShips.clear();
      myHits = 0; oppHits = 0;
      myTurn = false; myReady = false; oppReady = false;
      renderRadar();
      readyBtn.style.display = "block";
      readyBtn.disabled = false;
      readyBtn.textContent = "Valider ma Flotte (0/4)";
      statusEl.textContent = "Nouvelle partie ! Place tes 4 navires secrets sur le radar !";
      ovActions.style.display = "none";
      btnRematch.disabled = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
      btnRematch.style.color = "#000";
    }

    function startBattle() {
      phase = "battle";
      myTurn = (role === 1);
      statusEl.textContent = myTurn ? "🎯 À toi de tirer ! Touche le radar ennemi !" : "⏳ L'adversaire vise ta flotte...";
      readyBtn.style.display = "none";
      renderRadar();
    }

    function handleCellClick(i, cell) {
      if (phase === "place") {
        if (myShips.has(i)) { myShips.delete(i); cell.classList.remove('ship'); cell.textContent = ''; }
        else if (myShips.size < 4) { myShips.add(i); cell.classList.add('ship'); cell.textContent = '🚢'; }
        readyBtn.textContent = `Valider ma Flotte (${myShips.size}/4)`;
      } else if (phase === "battle" && myTurn) {
        if (cell.classList.contains('hit') || cell.classList.contains('miss')) return;
        cell.textContent = "🎯";
        myTurn = false;
        ws.send(JSON.stringify({ t: "shot", i: i }));
      }
    }

    readyBtn.onclick = () => {
      if (myShips.size !== 4) return alert("Place exactement 4 navires !");
      myReady = true;
      readyBtn.disabled = true;
      readyBtn.textContent = "En attente de l'adversaire...";
      statusEl.textContent = "Flotte prête ! Attente du joueur adverse...";
      ws.send(JSON.stringify({ t: "ready_naval" }));
      if (oppReady) {
        ws.send(JSON.stringify({ t: "start_battle" }));
        startBattle();
      }
    };

    function renderRadar() {
      for (let i = 0; i < 25; i++) {
        const c = boardEl.children[i];
        c.className = 'cell'; c.textContent = '';
      }
    }
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   3. MORPION CYBER 1V1 (DALLES DE CARBONE & GRAVURE LASER NEON)
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
      background: #060812; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      min-height: 100vh; padding: 14px;
    }
    header { width: 100%; max-width: 340px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 12px; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .status-text { font-size: 1.25rem; font-weight: 900; margin-bottom: 16px; min-height: 28px; text-align: center; }
    .grid { display: grid; grid-template-columns: repeat(3, 98px); grid-template-rows: repeat(3, 98px); gap: 12px; }
    .cell {
      background: linear-gradient(145deg, #12182c, #090d18); border: 2px solid rgba(0,243,255,0.3); border-radius: 18px;
      display: flex; align-items: center; justify-content: center; font-size: 3.2rem; font-weight: 900;
      cursor: pointer; transition: transform 0.1s, border-color 0.15s;
      box-shadow: 0 8px 20px rgba(0,0,0,0.5), inset 0 1px 0 rgba(255,255,255,0.1);
    }
    .cell:active { transform: scale(0.93); }
    .cell.x { color: #00f3ff; text-shadow: 0 0 15px #00f3ff, 0 0 30px rgba(0,243,255,0.6); border-color: #00f3ff; }
    .cell.o { color: #ff0055; text-shadow: 0 0 15px #ff0055, 0 0 30px rgba(255,0,85,0.6); border-color: #ff0055; }
    .act-btn { padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem; cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; transition: all 0.15s; }
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 15px rgba(0,243,255,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span id="badge" style="color:#00f3ff; font-weight:900; font-size:0.85rem;">Connexion...</span>
  </header>
  <div class="status-text" id="status">En attente d'un adversaire...</div>
  <div class="grid" id="grid"></div>

  <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:308px; margin-top:16px;">
    <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
    <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
  </div>

  <script>
    const gridEl = document.getElementById('grid');
    const statusEl = document.getElementById('status');
    const badge = document.getElementById('badge');
    const ovActions = document.getElementById('ov-actions');
    const btnRematch = document.getElementById('btn-rematch');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let board = Array(9).fill('');
    let role = 0, myTurn = false, playing = false;
    let myRematch = false, oppRematch = false;

    for (let i = 0; i < 9; i++) {
      const c = document.createElement('div');
      c.className = 'cell';
      c.onclick = () => playCell(i);
      gridEl.appendChild(c);
    }

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "ttt", name: myPseudo }));
    ws.onmessage = (e) => {
      const d = JSON.parse(e.data);
      if (d.t === "joined") {
        role = d.role || d.p;
        badge.textContent = role === 1 ? "✖️ Joueur X (J1)" : "⭕ Joueur O (J2)";
      } else if (d.t === "start") {
        playing = true;
        role = d.role || role || 1;
        myTurn = (role === 1);
        ovActions.style.display = "none";
        updateStatus();
      } else if (d.t === "move") {
        makeMove(d.i, role === 1 ? 'O' : 'X');
        myTurn = true;
        updateStatus();
      } else if (d.t === "rematch") {
        oppRematch = true;
        if (myRematch) {
          restartTTT();
        } else {
          btnRematch.textContent = "⚡ L'adversaire veut rejouer ! [ACCEPTER]";
          btnRematch.style.background = "linear-gradient(180deg, #00ff66, #00cc44)";
          btnRematch.style.color = "#000";
        }
      } else if (d.t === "left" || d.t === "opp_left") {
        playing = false;
        statusEl.textContent = "Adversaire déconnecté";
        statusEl.style.color = "#ff0055";
        ovActions.style.display = "none";
      }
    };

    function updateStatus() {
      if (!playing) return;
      statusEl.textContent = myTurn ? "🟢 À ton tour de jouer !" : "⏳ L'adversaire joue...";
      statusEl.style.color = myTurn ? "#00ff66" : "#8b9bb4";
    }

    function playCell(i) {
      if (!playing || !myTurn || board[i] !== '') return;
      makeMove(i, role === 1 ? 'X' : 'O');
      ws.send(JSON.stringify({ t: "move", i: i }));
      myTurn = false;
      updateStatus();
    }

    function makeMove(i, sym) {
      board[i] = sym;
      const cell = gridEl.children[i];
      cell.textContent = sym;
      cell.className = 'cell ' + sym.toLowerCase();
      checkWin();
    }

    function checkWin() {
      const wins = [[0,1,2],[3,4,5],[6,7,8],[0,3,6],[1,4,7],[2,5,8],[0,4,8],[2,4,6]];
      for (const w of wins) {
        if (board[w[0]] && board[w[0]] === board[w[1]] && board[w[0]] === board[w[2]]) {
          playing = false;
          const mySym = role === 1 ? 'X' : 'O';
          const won = (board[w[0]] === mySym);
          statusEl.textContent = won ? "🏆 VICTOIRE !" : "💀 DÉFAITE !";
          statusEl.style.color = won ? "#00f3ff" : "#ff0055";
          ovActions.style.display = "flex";
          btnRematch.disabled = false;
          btnRematch.textContent = "🔄 Rejouer (Revanche)";
          btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
          btnRematch.style.color = "#000";
          return;
        }
      }
      if (board.every(c => c !== '')) {
        playing = false;
        statusEl.textContent = "🤝 MATCH NUL !";
        statusEl.style.color = "#ffe600";
        ovActions.style.display = "flex";
        btnRematch.disabled = false;
        btnRematch.textContent = "🔄 Rejouer (Revanche)";
        btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
        btnRematch.style.color = "#000";
      }
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.disabled = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) restartTTT();
    };

    function restartTTT() {
      myRematch = false; oppRematch = false;
      board = Array(9).fill('');
      for (let i = 0; i < 9; i++) {
        gridEl.children[i].textContent = '';
        gridEl.children[i].className = 'cell';
      }
      ovActions.style.display = "none";
      btnRematch.disabled = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
      btnRematch.style.color = "#000";
      playing = true;
      myTurn = (role === 1);
      updateStatus();
    }
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   4. DUEL DE MEMOIRE 1V1 (MEMORY CARDS 16 TUILES EN DIRECT)
// ----------------------------------------------------------------------------------
const char MP_MEMORY_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Duel de Mémoire 1v1</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: #060914; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      min-height: 100vh; padding: 12px;
    }
    header { width: 100%; max-width: 360px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .scores { font-size: 1.25rem; font-weight: 900; margin-bottom: 8px; display: flex; gap: 20px; }
    .status-text { font-size: 1rem; font-weight: 800; min-height: 24px; margin-bottom: 12px; text-align: center; }
    .grid { display: grid; grid-template-columns: repeat(4, 72px); grid-template-rows: repeat(4, 72px); gap: 8px; }
    .card {
      background: linear-gradient(145deg, #18223c, #0d1424); border: 2px solid rgba(0,243,255,0.3);
      border-radius: 12px; display: flex; align-items: center; justify-content: center;
      font-size: 2rem; cursor: pointer; transition: transform 0.2s, background 0.2s;
      box-shadow: 0 4px 12px rgba(0,0,0,0.5);
    }
    .card.flipped { background: linear-gradient(145deg, #0055aa, #002255); border-color: #00f3ff; box-shadow: 0 0 15px rgba(0,243,255,0.5); }
    .card.matched { background: linear-gradient(145deg, #006633, #00331a); border-color: #00ff66; opacity: 0.7; pointer-events: none; }
    .act-btn { padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem; cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; transition: all 0.15s; }
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 15px rgba(0,243,255,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span id="badge" style="color:#00f3ff; font-weight:800; font-size:0.85rem;">Connexion...</span>
  </header>

  <div class="scores">
    <span style="color:#00f3ff;">J1 : <span id="s1">0</span></span>
    <span>-</span>
    <span style="color:#ff0055;">J2 : <span id="s2">0</span></span>
  </div>

  <div class="status-text" id="status">En attente d'un adversaire...</div>
  <div class="grid" id="grid"></div>

  <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:308px; margin-top:14px;">
    <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
    <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
  </div>

  <script>
    const gridEl = document.getElementById('grid');
    const statusEl = document.getElementById('status');
    const badge = document.getElementById('badge');
    const s1El = document.getElementById('s1'), s2El = document.getElementById('s2');
    const ovActions = document.getElementById('ov-actions');
    const btnRematch = document.getElementById('btn-rematch');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    const ICONS = ['🎮', '👾', '🚀', '💣', '💎', '👑', '⚡', '🔔'];
    let deck = [];
    let role = 0, myTurn = false, playing = false;
    let s1 = 0, s2 = 0;
    let flippedCards = []; // max 2
    let lockInput = false;
    let myRematch = false, oppRematch = false;

    function buildGrid() {
      gridEl.innerHTML = '';
      for (let i = 0; i < 16; i++) {
        const c = document.createElement('div');
        c.className = 'card'; c.dataset.i = i;
        c.onclick = () => onCardClick(i);
        gridEl.appendChild(c);
      }
    }
    buildGrid();

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "memory", name: myPseudo }));
    ws.onmessage = (e) => {
      const d = JSON.parse(e.data);
      if (d.t === "joined") {
        role = d.role || d.p;
        badge.textContent = role === 1 ? "🔵 Joueur 1 (Hôte)" : "🔴 Joueur 2";
      } else if (d.t === "start") {
        role = d.role || role || 1;
        if (role === 1) {
          // Generate deterministic deck and send to P2
          deck = [...ICONS, ...ICONS].sort(() => Math.random() - 0.5);
          ws.send(JSON.stringify({ t: "deck", d: deck }));
          startGame();
        }
      } else if (d.t === "deck") {
        deck = d.d;
        startGame();
      } else if (d.t === "flip") {
        handleCardReveal(d.i, false);
      } else if (d.t === "rematch") {
        oppRematch = true;
        if (myRematch) restartMemory();
        else {
          btnRematch.textContent = "⚡ L'adversaire veut rejouer ! [ACCEPTER]";
          btnRematch.style.background = "linear-gradient(180deg, #00ff66, #00cc44)";
        }
      } else if (d.t === "left" || d.t === "opp_left") {
        statusEl.textContent = "Adversaire déconnecté";
        playing = false;
      }
    };

    function startGame() {
      playing = true; myTurn = (role === 1);
      s1 = 0; s2 = 0; s1El.textContent = 0; s2El.textContent = 0;
      ovActions.style.display = "none";
      updateTurnStatus();
    }

    function updateTurnStatus() {
      if (!playing) return;
      statusEl.textContent = myTurn ? "🟢 À ton tour de trouver une paire !" : "⏳ L'adversaire cherche une paire...";
      statusEl.style.color = myTurn ? "#00ff66" : "#8b9bb4";
    }

    function onCardClick(i) {
      if (!playing || !myTurn || lockInput) return;
      const el = gridEl.children[i];
      if (el.classList.contains('flipped') || el.classList.contains('matched')) return;

      handleCardReveal(i, true);
      ws.send(JSON.stringify({ t: "flip", i: i }));
    }

    function handleCardReveal(i, isSelf) {
      const el = gridEl.children[i];
      el.classList.add('flipped');
      el.textContent = deck[i];
      flippedCards.push({ i: i, sym: deck[i] });

      if (flippedCards.length === 2) {
        lockInput = true;
        const [c1, c2] = flippedCards;
        if (c1.sym === c2.sym) {
          // Pair found!
          setTimeout(() => {
            gridEl.children[c1.i].className = 'card matched';
            gridEl.children[c2.i].className = 'card matched';
            flippedCards = [];
            lockInput = false;
            if (myTurn) s1++; else s2++;
            s1El.textContent = s1; s2El.textContent = s2;
            checkGameOver();
          }, 600);
        } else {
          // No match: flip back and switch turn
          setTimeout(() => {
            gridEl.children[c1.i].className = 'card'; gridEl.children[c1.i].textContent = '';
            gridEl.children[c2.i].className = 'card'; gridEl.children[c2.i].textContent = '';
            flippedCards = [];
            lockInput = false;
            myTurn = !myTurn;
            updateTurnStatus();
          }, 1200);
        }
      }
    }

    function checkGameOver() {
      if (s1 + s2 >= 8) {
        playing = false;
        const myScore = role === 1 ? s1 : s2;
        const oppScore = role === 1 ? s2 : s1;
        if (myScore > oppScore) statusEl.textContent = `🏆 VICTOIRE (${myScore} à ${oppScore}) !`;
        else if (myScore < oppScore) statusEl.textContent = `💀 DÉFAITE (${myScore} à ${oppScore}) !`;
        else statusEl.textContent = `🤝 ÉGALITÉ PARFAITE (${myScore} partout) !`;
        statusEl.style.color = myScore >= oppScore ? "#00f3ff" : "#ff0055";
        ovActions.style.display = "flex";
        btnRematch.disabled = false;
        btnRematch.textContent = "🔄 Rejouer (Revanche)";
        btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
      }
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.disabled = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) restartMemory();
    };

    function restartMemory() {
      myRematch = false; oppRematch = false;
      buildGrid();
      if (role === 1) {
        deck = [...ICONS, ...ICONS].sort(() => Math.random() - 0.5);
        ws.send(JSON.stringify({ t: "deck", d: deck }));
      }
      startGame();
    }
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   5. MASTERMIND DUEL 1V1 (DECODAGE EN SIMULTANE DE LA COMBINAISON SECRETE)
// ----------------------------------------------------------------------------------
const char MP_MASTERMIND_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Mastermind Duel 1v1</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: #060914; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      min-height: 100vh; padding: 12px;
    }
    header { width: 100%; max-width: 380px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .status-text { font-size: 1.05rem; font-weight: 900; margin-bottom: 8px; min-height: 26px; text-align: center; }
    #board-wrap {
      width: 100%; max-width: 360px; background: #0c1224; border: 2px solid #00f3ff;
      border-radius: 18px; padding: 14px; box-shadow: 0 10px 30px rgba(0,0,0,0.6);
      display: flex; flex-direction: column; gap: 8px; max-height: 320px; overflow-y: auto;
    }
    .row { display: flex; justify-content: space-between; align-items: center; background: #131b33; padding: 8px 12px; border-radius: 10px; }
    .pegs { display: flex; gap: 8px; }
    .peg { width: 28px; height: 28px; border-radius: 50%; border: 2px solid rgba(255,255,255,0.2); box-shadow: inset 0 2px 4px rgba(0,0,0,0.5); }
    .clues { display: flex; gap: 4px; font-size: 0.8rem; font-weight: 800; }
    .clue-dot { width: 12px; height: 12px; border-radius: 50%; }
    .clue-exact { background: #ff0055; box-shadow: 0 0 6px #ff0055; }
    .clue-color { background: #fff; box-shadow: 0 0 6px #fff; }
    
    .picker-area { width: 100%; max-width: 360px; margin-top: 10px; display: flex; flex-direction: column; gap: 8px; }
    .current-guess { display: flex; justify-content: center; gap: 12px; }
    .c-slot { width: 38px; height: 38px; border-radius: 50%; border: 2px dashed #00f3ff; cursor: pointer; }
    .palette { display: flex; justify-content: center; gap: 8px; }
    .pal-btn { width: 36px; height: 36px; border-radius: 50%; border: none; cursor: pointer; box-shadow: 0 3px 8px rgba(0,0,0,0.4); }
    .btn-submit { padding: 12px; background: linear-gradient(180deg, #00f3ff, #0088ff); color: #000; font-weight: 900; font-size: 1rem; border: none; border-radius: 10px; cursor: pointer; }
    .act-btn { padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem; cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; transition: all 0.15s; }
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 15px rgba(0,243,255,0.4); }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span id="badge" style="color:#00f3ff; font-weight:800; font-size:0.85rem;">Connexion...</span>
  </header>

  <div class="status-text" id="status">Trouve le code secret à 4 couleurs !</div>
  <div id="board-wrap"></div>

  <div class="picker-area" id="ctrl-area">
    <div class="current-guess">
      <div class="c-slot" id="slot-0" onclick="clearSlot(0)"></div>
      <div class="c-slot" id="slot-1" onclick="clearSlot(1)"></div>
      <div class="c-slot" id="slot-2" onclick="clearSlot(2)"></div>
      <div class="c-slot" id="slot-3" onclick="clearSlot(3)"></div>
    </div>
    <div class="palette">
      <button class="pal-btn" style="background:#ff0055;" onclick="pickColor('#ff0055')"></button>
      <button class="pal-btn" style="background:#00f3ff;" onclick="pickColor('#00f3ff')"></button>
      <button class="pal-btn" style="background:#00ff66;" onclick="pickColor('#00ff66')"></button>
      <button class="pal-btn" style="background:#ffe600;" onclick="pickColor('#ffe600')"></button>
      <button class="pal-btn" style="background:#cc00ff;" onclick="pickColor('#cc00ff')"></button>
      <button class="pal-btn" style="background:#ff8800;" onclick="pickColor('#ff8800')"></button>
    </div>
    <button class="btn-submit" onclick="submitGuess()">Valider la combinaison (4)</button>
  </div>

  <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:360px; margin-top:14px;">
    <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
    <a href="/hub" class="act-btn btn-quit" style="background:rgba(255,255,255,0.08);color:#8b9bb4;text-decoration:none;border:1px solid rgba(255,255,255,0.2);padding:10px;border-radius:10px;text-align:center;">◀ Quitter au Salon</a>
  </div>

  <script>
    const boardWrap = document.getElementById('board-wrap');
    const statusEl = document.getElementById('status');
    const badge = document.getElementById('badge');
    const ovActions = document.getElementById('ov-actions');
    const btnRematch = document.getElementById('btn-rematch');
    const ctrlArea = document.getElementById('ctrl-area');

    const COLORS = ['#ff0055', '#00f3ff', '#00ff66', '#ffe600', '#cc00ff', '#ff8800'];
    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false;
    let secretCode = [];
    let currentSlots = [null, null, null, null];
    let myRematch = false, oppRematch = false;

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "mastermind", name: myPseudo }));
    ws.onmessage = (e) => {
      const d = JSON.parse(e.data);
      if (d.t === "joined") {
        role = d.role || d.p;
        badge.textContent = role === 1 ? "🔵 Décodeur J1" : "🔴 Décodeur J2";
      } else if (d.t === "start") {
        role = d.role || role || 1;
        if (role === 1) {
          secretCode = [0,1,2,3].map(() => COLORS[Math.floor(Math.random() * COLORS.length)]);
          ws.send(JSON.stringify({ t: "code", c: secretCode }));
          startMatch();
        }
      } else if (d.t === "code") {
        secretCode = d.c;
        startMatch();
      } else if (d.t === "opp_win") {
        handleEnd(false, d.by);
      } else if (d.t === "rematch") {
        oppRematch = true;
        if (myRematch) restartMastermind();
        else {
          btnRematch.textContent = "⚡ L'adversaire veut rejouer ! [ACCEPTER]";
          btnRematch.style.background = "linear-gradient(180deg, #00ff66, #00cc44)";
        }
      } else if (d.t === "left" || d.t === "opp_left") {
        statusEl.textContent = "Adversaire déconnecté";
        playing = false;
      }
    };

    function startMatch() {
      playing = true;
      boardWrap.innerHTML = '';
      currentSlots = [null, null, null, null];
      updateSlotDisplay();
      ctrlArea.style.display = "flex";
      ovActions.style.display = "none";
      statusEl.textContent = "Trouve le code à 4 couleurs avant l'adversaire !";
    }

    function pickColor(col) {
      for (let i = 0; i < 4; i++) {
        if (!currentSlots[i]) {
          currentSlots[i] = col;
          updateSlotDisplay();
          return;
        }
      }
    }
    function clearSlot(i) {
      currentSlots[i] = null;
      updateSlotDisplay();
    }
    function updateSlotDisplay() {
      for (let i = 0; i < 4; i++) {
        const sl = document.getElementById('slot-' + i);
        if (currentSlots[i]) {
          sl.style.background = currentSlots[i];
          sl.style.borderStyle = "solid";
        } else {
          sl.style.background = "transparent";
          sl.style.borderStyle = "dashed";
        }
      }
    }

    function submitGuess() {
      if (!playing || currentSlots.some(s => !s)) return alert("Sélectionne les 4 couleurs !");

      let exact = 0, colorMatch = 0;
      let sCode = [...secretCode];
      let gCode = [...currentSlots];

      // Exact match
      for (let i = 0; i < 4; i++) {
        if (gCode[i] === sCode[i]) {
          exact++;
          sCode[i] = null;
          gCode[i] = 'used';
        }
      }
      // Color match
      for (let i = 0; i < 4; i++) {
        if (gCode[i] !== 'used') {
          const idx = sCode.indexOf(gCode[i]);
          if (idx !== -1) {
            colorMatch++;
            sCode[idx] = null;
          }
        }
      }

      addHistoryRow(currentSlots, exact, colorMatch);
      currentSlots = [null, null, null, null];
      updateSlotDisplay();

      if (exact === 4) {
        ws.send(JSON.stringify({ t: "opp_win", by: myPseudo }));
        handleEnd(true, myPseudo);
      }
    }

    function addHistoryRow(guess, exact, colorMatch) {
      const row = document.createElement('div');
      row.className = 'row';
      let pegsHtml = '<div class="pegs">';
      guess.forEach(c => { pegsHtml += `<div class="peg" style="background:${c};"></div>`; });
      pegsHtml += '</div>';

      let cluesHtml = '<div class="clues">';
      for (let e = 0; e < exact; e++) cluesHtml += '<div class="clue-dot clue-exact" title="Exact"></div>';
      for (let cm = 0; cm < colorMatch; cm++) cluesHtml += '<div class="clue-dot clue-color" title="Couleur"></div>';
      cluesHtml += ` <span style="font-size:0.75rem;color:#889;margin-left:4px;">(${exact}B / ${colorMatch}W)</span></div>`;

      row.innerHTML = pegsHtml + cluesHtml;
      boardWrap.prepend(row);
    }

    function handleEnd(won, by) {
      playing = false;
      ctrlArea.style.display = "none";
      ovActions.style.display = "flex";
      statusEl.textContent = won ? "🏆 VICTOIRE ! Code déchiffré en premier !" : `💀 DÉFAITE ! ${by} a trouvé le code !`;
      statusEl.style.color = won ? "#00f3ff" : "#ff0055";
      btnRematch.disabled = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.disabled = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) restartMastermind();
    };

    function restartMastermind() {
      myRematch = false; oppRematch = false;
      if (role === 1) {
        secretCode = [0,1,2,3].map(() => COLORS[Math.floor(Math.random() * COLORS.length)]);
        ws.send(JSON.stringify({ t: "code", c: secretCode }));
      }
      startMatch();
    }
  </script>
</body>
</html>
)rawliteral";

#endif
