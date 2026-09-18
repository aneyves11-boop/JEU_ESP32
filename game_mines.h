#ifndef GAME_MINES_H
#define GAME_MINES_H

#include <pgmspace.h>

const char GAME_MINES_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Démineur Néon - ESP32 Arcade</title>
  <style>
    :root {
      --bg: #090b12; --card: rgba(22, 27, 46, 0.9); --card-border: rgba(0, 243, 255, 0.25);
      --cyan: #00f3ff; --pink: #ff0055; --yellow: #ffe600; --green: #00ff66; --text: #f0f4fc; --dim: #8b9bb4;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body { background: var(--bg); color: var(--text); font-family: system-ui, sans-serif; min-height: 100vh; display: flex; flex-direction: column; align-items: center; padding: 10px; }
    header { width: 100%; max-width: 360px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
    .btn-back { background: var(--card); border: 1px solid var(--card-border); color: #fff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.85rem; font-weight: bold; }
    .score-panel { display: flex; gap: 8px; }
    .score-box { background: var(--card); border: 1px solid rgba(255,255,255,0.1); padding: 4px 10px; border-radius: 8px; text-align: center; min-width: 65px; }
    .score-box .lbl { font-size: 0.65rem; color: var(--dim); text-transform: uppercase; }
    .score-box .val { font-size: 1.1rem; font-weight: 800; color: var(--cyan); }
    .stage { width: 340px; max-width: 92vw; background: var(--card); border: 2px solid var(--card-border); border-radius: 14px; padding: 10px; box-shadow: 0 0 20px rgba(0,243,255,0.2); }
    .board-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 10px; padding: 0 4px; }
    .smiley { font-size: 1.6rem; cursor: pointer; background: rgba(255,255,255,0.08); border-radius: 50%; width: 40px; height: 40px; display: flex; align-items: center; justify-content: center; }
    .grid { display: grid; grid-template-columns: repeat(9, 1fr); gap: 4px; }
    .cell {
      aspect-ratio: 1; background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.15);
      border-radius: 6px; display: flex; align-items: center; justify-content: center;
      font-weight: 900; font-size: 1rem; cursor: pointer; transition: background 0.1s;
    }
    .cell.revealed { background: #0c0f1d; border-color: rgba(0,243,255,0.1); cursor: default; }
    .cell.mine { background: #ff0055 !important; }
    .cell.flag { color: #ffe600; }
    .c-1 { color: #00f3ff; } .c-2 { color: #00ff66; } .c-3 { color: #ff0055; } .c-4 { color: #ffe600; }
    .controls { width: 100%; max-width: 340px; margin-top: 10px; display: flex; gap: 8px; }
    .btn-mode { flex: 1; height: 48px; background: rgba(255,255,255,0.08); border: 1px solid var(--cyan); border-radius: 10px; color: #fff; font-weight: bold; font-size: 0.95rem; cursor: pointer; }
    .btn-mode.active { background: linear-gradient(135deg, var(--cyan), #0077ff); color: #000; font-weight: 900; }
  </style>
<script src="/pause.js"></script></head>
<body>
  <header>
    <a href="/hub" class="btn-back">◀ Menu</a>
    <div class="score-panel">
      <div class="score-box"><div class="lbl">Mines</div><div class="val" id="mines-left">10</div></div>
      <div class="score-box"><div class="lbl">Temps</div><div class="val" id="timer">0</div></div>
    </div>
  </header>

  <div class="stage">
    <div class="board-header">
      <div style="font-weight:bold;color:var(--dim);font-size:0.85rem;">Grille 9x9</div>
      <div class="smiley" id="smiley" onclick="init()">😎</div>
      <div style="font-weight:bold;color:var(--yellow);font-size:0.85rem;">10 Bombes</div>
    </div>
    <div class="grid" id="grid"></div>
  </div>

  <div class="controls">
    <button class="btn-mode active" id="btn-dig" onclick="setMode('dig')">⛏️ Creuser</button>
    <button class="btn-mode" id="btn-flag" onclick="setMode('flag')">🚩 Poser Drapeau</button>
  </div>

  <script>
    const ROWS = 9, COLS = 9, TOTAL_MINES = 10;
    let board = [], mode = 'dig', gameOver = false, startTime = 0, timerId = null;
    const gridEl = document.getElementById('grid'), minesEl = document.getElementById('mines-left'), timerEl = document.getElementById('timer'), smileyEl = document.getElementById('smiley');

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
      board = []; gameOver = false; smileyEl.textContent = '😎';
      if(timerId) clearInterval(timerId);
      startTime = Date.now();
      timerId = setInterval(() => {
        if(!gameOver) timerEl.textContent = Math.floor((Date.now() - startTime)/1000);
      }, 1000);

      // Create cells
      for(let r=0; r<ROWS; r++) {
        board[r] = [];
        for(let c=0; c<COLS; c++) {
          board[r][c] = { mine: false, revealed: false, flag: false, count: 0 };
        }
      }

      // Place mines
      let placed = 0;
      while(placed < TOTAL_MINES) {
        let r = Math.floor(Math.random()*ROWS), c = Math.floor(Math.random()*COLS);
        if(!board[r][c].mine) {
          board[r][c].mine = true; placed++;
        }
      }

      // Count adjacent
      for(let r=0; r<ROWS; r++) {
        for(let c=0; c<COLS; c++) {
          if(!board[r][c].mine) {
            let count = 0;
            for(let dr=-1; dr<=1; dr++) for(let dc=-1; dc<=1; dc++) {
              let nr = r+dr, nc = c+dc;
              if(nr>=0 && nr<ROWS && nc>=0 && nc<COLS && board[nr][nc].mine) count++;
            }
            board[r][c].count = count;
          }
        }
      }

      render();
    }

    function setMode(m) {
      mode = m;
      document.getElementById('btn-dig').className = 'btn-mode' + (m==='dig'?' active':'');
      document.getElementById('btn-flag').className = 'btn-mode' + (m==='flag'?' active':'');
    }

    function clickCell(r, c) {
      if(gameOver || board[r][c].revealed) return;

      if(mode === 'flag') {
        board[r][c].flag = !board[r][c].flag;
        sound(550);
        render();
        return;
      }

      if(board[r][c].flag) return; // Protected

      if(board[r][c].mine) {
        // Boom!
        board[r][c].revealed = true;
        gameOver = true;
        smileyEl.textContent = '💥';
        sound(150);
        revealAll();
        return;
      }

      reveal(r, c);
      sound(650);
      checkWin();
      render();
    }

    function reveal(r, c) {
      if(r<0 || r>=ROWS || c<0 || c>=COLS || board[r][c].revealed || board[r][c].flag) return;
      board[r][c].revealed = true;
      if(board[r][c].count === 0 && !board[r][c].mine) {
        for(let dr=-1; dr<=1; dr++) for(let dc=-1; dc<=1; dc++) {
          reveal(r+dr, c+dc);
        }
      }
    }

    function revealAll() {
      for(let r=0; r<ROWS; r++) for(let c=0; c<COLS; c++) board[r][c].revealed = true;
      render();
    }

    function checkWin() {
      let unrevealedSafe = 0;
      for(let r=0; r<ROWS; r++) for(let c=0; c<COLS; c++) {
        if(!board[r][c].mine && !board[r][c].revealed) unrevealedSafe++;
      }
      if(unrevealedSafe === 0) {
        gameOver = true; smileyEl.textContent = '🎉'; sound(950);
        localStorage.setItem('mines_highscore', (parseInt(localStorage.getItem('mines_highscore')||'0',10)+1));
      }
    }

    function render() {
      gridEl.innerHTML = '';
      let flags = 0;
      for(let r=0; r<ROWS; r++) {
        for(let c=0; c<COLS; c++) {
          const cell = board[r][c];
          if(cell.flag) flags++;
          const el = document.createElement('div');
          el.className = 'cell';
          if(cell.revealed) {
            el.classList.add('revealed');
            if(cell.mine) {
              el.classList.add('mine'); el.textContent = '💣';
            } else if(cell.count > 0) {
              el.classList.add(`c-${cell.count}`); el.textContent = cell.count;
            }
          } else if(cell.flag) {
            el.classList.add('flag'); el.textContent = '🚩';
          }
          el.onclick = () => clickCell(r, c);
          gridEl.appendChild(el);
        }
      }
      minesEl.textContent = TOTAL_MINES - flags;
    }

    init();
  </script>
</body>
</html>
)rawliteral";

#endif
