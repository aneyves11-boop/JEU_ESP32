#ifndef GAME_TICTACTOE_H
#define GAME_TICTACTOE_H

#include <pgmspace.h>

const char GAME_TICTACTOE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Morpion Cyber IA - ESP32 Arcade</title>
  <style>
    :root {
      --bg: #090b12; --card: rgba(22, 27, 46, 0.9); --card-border: rgba(0, 243, 255, 0.25);
      --cyan: #00f3ff; --pink: #ff0055; --yellow: #ffe600; --green: #00ff66; --text: #f0f4fc; --dim: #8b9bb4;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body { background: var(--bg); color: var(--text); font-family: system-ui, sans-serif; min-height: 100vh; display: flex; flex-direction: column; align-items: center; padding: 10px; }
    header { width: 100%; max-width: 360px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 12px; }
    .btn-back { background: var(--card); border: 1px solid var(--card-border); color: #fff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.85rem; font-weight: bold; }
    .score-panel { display: flex; gap: 8px; }
    .score-box { background: var(--card); border: 1px solid rgba(255,255,255,0.1); padding: 4px 10px; border-radius: 8px; text-align: center; min-width: 65px; }
    .score-box .lbl { font-size: 0.65rem; color: var(--dim); text-transform: uppercase; }
    .score-box .val { font-size: 1.1rem; font-weight: 800; color: var(--cyan); }
    .stage { width: 330px; max-width: 92vw; background: var(--card); border: 2px solid var(--card-border); border-radius: 16px; padding: 16px; box-shadow: 0 0 25px rgba(0,243,255,0.2); }
    .status-turn { text-align: center; font-size: 1.1rem; font-weight: 800; color: var(--cyan); margin-bottom: 14px; }
    .grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; }
    .cell {
      aspect-ratio: 1; background: rgba(255,255,255,0.06); border: 1px solid rgba(0,243,255,0.3);
      border-radius: 12px; display: flex; align-items: center; justify-content: center;
      font-size: 2.8rem; font-weight: 900; cursor: pointer; transition: all 0.15s;
    }
    .cell:active { transform: scale(0.95); }
    .cell.x { color: var(--cyan); text-shadow: 0 0 15px var(--cyan); }
    .cell.o { color: var(--pink); text-shadow: 0 0 15px var(--pink); }
    .cell.win { background: rgba(0,255,102,0.25); border-color: var(--green); }
    .btn-reset { width: 100%; max-width: 330px; height: 48px; margin-top: 14px; background: linear-gradient(135deg, var(--cyan), #0077ff); border: none; border-radius: 12px; color: #000; font-weight: 900; font-size: 1rem; cursor: pointer; }
  </style>
<script src="/pause.js"></script></head>
<body>
  <header>
    <a href="/hub" class="btn-back">◀ Menu</a>
    <div class="score-panel">
      <div class="score-box"><div class="lbl">Victoires</div><div class="val" id="wins">0</div></div>
      <div class="score-box"><div class="lbl">Défaites</div><div class="val" id="losses" style="color:var(--pink);">0</div></div>
    </div>
  </header>

  <div class="stage">
    <div class="status-turn" id="status">Ton tour (X)</div>
    <div class="grid" id="grid"></div>
  </div>

  <button class="btn-reset" onclick="init()">NOUVELLE MANCHE 🔄</button>

  <script>
    let b = Array(9).fill(null), gameOver = false, wins = 0, losses = 0;
    const gridEl = document.getElementById('grid'), statusEl = document.getElementById('status');
    const winsEl = document.getElementById('wins'), lossesEl = document.getElementById('losses');

    function sound(f) {
      try {
        const AC = window.AudioContext || window.webkitAudioContext;
        if(!AC) return;
        const ac = new AC(), o = ac.createOscillator(), g = ac.createGain();
        o.type = 'sine'; o.frequency.value = f;
        g.gain.setValueAtTime(0.1, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + 0.1);
        o.connect(g); g.connect(ac.destination);
        o.start(); o.stop(ac.currentTime + 0.1);
      } catch(e){}
    }

    function init() {
      b = Array(9).fill(null); gameOver = false;
      statusEl.textContent = "À toi de jouer (X) !"; statusEl.style.color = "var(--cyan)";
      render();
    }

    function playerMove(i) {
      if(gameOver || b[i]) return;
      b[i] = 'X'; sound(600);
      render();

      const winCombo = checkWin('X');
      if(winCombo) {
        gameOver = true; wins++; winsEl.textContent = wins;
        statusEl.textContent = "Victoire ! 🎉"; statusEl.style.color = "var(--green)";
        sound(900); highlight(winCombo); return;
      }

      if(b.every(x => x)) {
        gameOver = true; statusEl.textContent = "Égalité ! 🤝"; statusEl.style.color = "var(--yellow)";
        sound(400); return;
      }

      // AI Move
      statusEl.textContent = "L'IA réfléchit...";
      setTimeout(aiMove, 300);
    }

    function aiMove() {
      if(gameOver) return;
      // Smart AI
      let bestMove = findBestMove();
      b[bestMove] = 'O'; sound(450);
      render();

      const winCombo = checkWin('O');
      if(winCombo) {
        gameOver = true; losses++; lossesEl.textContent = losses;
        statusEl.textContent = "L'IA a gagné ! 🤖"; statusEl.style.color = "var(--pink)";
        sound(200); highlight(winCombo); return;
      }

      if(b.every(x => x)) {
        gameOver = true; statusEl.textContent = "Égalité ! 🤝"; statusEl.style.color = "var(--yellow)";
        return;
      }

      statusEl.textContent = "À toi de jouer (X) !";
    }

    function findBestMove() {
      // 1. Can AI win?
      for(let i=0; i<9; i++) {
        if(!b[i]) {
          b[i] = 'O'; if(checkWin('O')) { b[i] = null; return i; }
          b[i] = null;
        }
      }
      // 2. Block player?
      for(let i=0; i<9; i++) {
        if(!b[i]) {
          b[i] = 'X'; if(checkWin('X')) { b[i] = null; return i; }
          b[i] = null;
        }
      }
      // 3. Center
      if(!b[4]) return 4;
      // 4. Random corner or free
      const free = [];
      b.forEach((v, i) => { if(!v) free.push(i); });
      return free[Math.floor(Math.random()*free.length)];
    }

    const COMBOS = [
      [0,1,2],[3,4,5],[6,7,8],
      [0,3,6],[1,4,7],[2,5,8],
      [0,4,8],[2,4,6]
    ];

    function checkWin(p) {
      for(let c of COMBOS) {
        if(b[c[0]]===p && b[c[1]]===p && b[c[2]]===p) return c;
      }
      return null;
    }

    function highlight(combo) {
      const cells = gridEl.querySelectorAll('.cell');
      combo.forEach(idx => cells[idx].classList.add('win'));
    }

    function render() {
      gridEl.innerHTML = '';
      b.forEach((v, i) => {
        const el = document.createElement('div');
        el.className = 'cell' + (v ? ' ' + v.toLowerCase() : '');
        el.textContent = v || '';
        el.onclick = () => playerMove(i);
        gridEl.appendChild(el);
      });
    }

    init();
  </script>
</body>
</html>
)rawliteral";

#endif
