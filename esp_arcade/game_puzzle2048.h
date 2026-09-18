#ifndef GAME_PUZZLE2048_H
#define GAME_PUZZLE2048_H

#include <pgmspace.h>

const char GAME_PUZZLE2048_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>2048 Cyber - ESP32 Arcade</title>
  <style>
    :root {
      --bg-color: #0d0f18;
      --card-bg: rgba(26, 31, 51, 0.9);
      --card-border: rgba(0, 243, 255, 0.25);
      --neon-cyan: #00f3ff;
      --neon-pink: #ff0055;
      --neon-yellow: #ffe600;
      --text-main: #f0f4fc;
      --text-dim: #8b9bb4;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; touch-action: manipulation; }
    body {
      background: var(--bg-color); color: var(--text-main); font-family: system-ui, sans-serif;
      min-height: 100vh; display: flex; flex-direction: column; align-items: center; padding: 10px;
    }
    header {
      width: 100%; max-width: 340px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 12px;
    }
    .back-btn {
      background: var(--card-bg); border: 1px solid var(--card-border); color: #fff;
      padding: 8px 12px; border-radius: 8px; text-decoration: none; font-size: 0.85rem; font-weight: bold;
    }
    .score-panel { display: flex; gap: 8px; }
    .score-box {
      background: var(--card-bg); border: 1px solid rgba(255, 255, 255, 0.1);
      padding: 4px 10px; border-radius: 8px; text-align: center; min-width: 65px;
    }
    .score-box .label { font-size: 0.65rem; color: var(--text-dim); text-transform: uppercase; }
    .score-box .val { font-size: 1.1rem; font-weight: 800; color: var(--neon-cyan); }
    .score-box.best .val { color: var(--neon-yellow); }
    .grid {
      width: 330px; height: 330px; max-width: 90vw; max-height: 90vw;
      background: #111422; border: 2px solid var(--card-border); border-radius: 16px;
      padding: 8px; display: grid; grid-template-columns: repeat(4, 1fr); grid-template-rows: repeat(4, 1fr); gap: 8px;
      position: relative; box-shadow: 0 0 20px rgba(0, 243, 255, 0.2);
    }
    .tile {
      background: rgba(255, 255, 255, 0.05); border-radius: 8px;
      display: flex; align-items: center; justify-content: center;
      font-size: 1.5rem; font-weight: 800; color: #fff;
    }
    .t-2 { background: #1f293d; color: #00f3ff; }
    .t-4 { background: #1c3d5a; color: #00f3ff; }
    .t-8 { background: #b45309; color: #fff; }
    .t-16 { background: #d97706; color: #fff; }
    .t-32 { background: #dc2626; color: #fff; }
    .t-64 { background: #e11d48; color: #fff; }
    .t-128 { background: #7c3aed; color: #ffe600; font-size: 1.3rem; }
    .t-256 { background: #9333ea; color: #ffe600; font-size: 1.3rem; }
    .t-512 { background: #059669; color: #fff; font-size: 1.2rem; }
    .t-1024 { background: #00b4d8; color: #fff; font-size: 1rem; }
    .t-2048 { background: linear-gradient(135deg, #ffe600, #ff0055); color: #000; font-size: 1rem; }
    .overlay {
      position: absolute; inset: 0; background: rgba(13, 15, 24, 0.9);
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      gap: 14px; text-align: center; border-radius: 14px; z-index: 5;
    }
    .overlay.hidden { display: none; }
    .overlay h2 { font-size: 1.8rem; color: var(--neon-pink); }
    .btn-play {
      background: linear-gradient(135deg, var(--neon-cyan), #0077ff);
      color: #000; font-weight: 800; border: none; padding: 12px 24px; border-radius: 10px; font-size: 1rem; cursor: pointer;
    }
    .dpad { display: flex; flex-direction: column; align-items: center; gap: 6px; margin-top: 10px; }
    .dpad-row { display: flex; gap: 10px; }
    .dpad-btn {
      width: 58px; height: 48px; background: rgba(255, 255, 255, 0.08);
      border: 1px solid rgba(255, 255, 255, 0.2); border-radius: 10px;
      color: #fff; font-size: 1.2rem; display: flex; align-items: center; justify-content: center;
    }
    .dpad-btn:active { background: rgba(0, 243, 255, 0.3); }
  </style>
<script src="/pause.js"></script></head>
<body>
  <header>
    <a href="/hub" class="back-btn">◀ Menu</a>
    <div class="score-panel">
      <div class="score-box"><div class="label">Score</div><div class="val" id="score">0</div></div>
      <div class="score-box best"><div class="label">Record</div><div class="val" id="best">0</div></div>
    </div>
  </header>

  <div style="position:relative;">
    <div class="grid" id="grid"></div>
    <div id="overlay" class="overlay hidden">
      <h2 id="title">FIN DE PARTIE</h2>
      <p id="msg"></p>
      <button class="btn-play" id="btn-restart">Rejouer ↺</button>
    </div>
  </div>

  <div class="dpad">
    <div class="dpad-row"><button class="dpad-btn" id="u">▲</button></div>
    <div class="dpad-row">
      <button class="dpad-btn" id="l">◀</button>
      <button class="dpad-btn" id="d">▼</button>
      <button class="dpad-btn" id="r">▶</button>
    </div>
  </div>

  <script>
    const gridEl = document.getElementById('grid'), scoreEl = document.getElementById('score'), bestEl = document.getElementById('best');
    const overlay = document.getElementById('overlay'), title = document.getElementById('title'), msg = document.getElementById('msg');
    const btnRestart = document.getElementById('btn-restart');

    let b = Array(4).fill(null).map(() => Array(4).fill(0)), score = 0;
    let bestScore = parseInt(localStorage.getItem('2048_highscore') || '0', 10);
    bestEl.textContent = bestScore;

    function sound(f) {
      try {
        const A = window.AudioContext || window.webkitAudioContext;
        if(!A) return;
        const ac = new A(), o = ac.createOscillator(), g = ac.createGain();
        o.type = 'sine'; o.frequency.value = f;
        g.gain.setValueAtTime(0.1, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + 0.08);
        o.connect(g); g.connect(ac.destination);
        o.start(); o.stop(ac.currentTime + 0.08);
      } catch(e){}
    }

    function init() {
      b = Array(4).fill(null).map(() => Array(4).fill(0));
      score = 0; scoreEl.textContent = '0'; overlay.classList.add('hidden');
      spawn(); spawn(); render();
    }

    function spawn() {
      const free = [];
      for(let r=0;r<4;r++) for(let c=0;c<4;c++) if(!b[r][c]) free.push({r,c});
      if(!free.length) return;
      const spot = free[Math.floor(Math.random()*free.length)];
      b[spot.r][spot.c] = Math.random()<0.9 ? 2 : 4;
    }

    function render() {
      gridEl.innerHTML = '';
      for(let r=0;r<4;r++) {
        for(let c=0;c<4;c++) {
          const v = b[r][c], d = document.createElement('div');
          d.className = 'tile';
          if(v) { d.classList.add(`t-${v}`); d.textContent = v; }
          gridEl.appendChild(d);
        }
      }
    }

    function slide(row) {
      let arr = row.filter(v => v !== 0), gain = 0;
      for(let i=0; i<arr.length-1; i++) {
        if(arr[i] === arr[i+1]) {
          arr[i] *= 2; gain += arr[i]; arr.splice(i+1, 1);
        }
      }
      while(arr.length < 4) arr.push(0);
      return { row: arr, gain };
    }

    function move(dir) {
      let moved = false, gained = 0;
      if(dir==='L') {
        for(let r=0;r<4;r++){
          const res = slide(b[r]);
          if(res.row.join(',')!==b[r].join(',')) moved = true;
          b[r] = res.row; gained += res.gain;
        }
      } else if(dir==='R') {
        for(let r=0;r<4;r++){
          const res = slide([...b[r]].reverse());
          const newRow = res.row.reverse();
          if(newRow.join(',')!==b[r].join(',')) moved = true;
          b[r] = newRow; gained += res.gain;
        }
      } else if(dir==='U') {
        for(let c=0;c<4;c++){
          const col = [b[0][c],b[1][c],b[2][c],b[3][c]];
          const res = slide(col);
          for(let r=0;r<4;r++){
            if(b[r][c]!==res.row[r]) moved = true;
            b[r][c] = res.row[r];
          }
          gained += res.gain;
        }
      } else if(dir==='D') {
        for(let c=0;c<4;c++){
          const col = [b[3][c],b[2][c],b[1][c],b[0][c]];
          const res = slide(col);
          const rev = res.row.reverse();
          for(let r=0;r<4;r++){
            if(b[r][c]!==rev[r]) moved = true;
            b[r][c] = rev[r];
          }
          gained += res.gain;
        }
      }

      if(moved) {
        if(gained>0) {
          score += gained; scoreEl.textContent = score; sound(600);
          if(score>bestScore){ bestScore = score; bestEl.textContent = bestScore; localStorage.setItem('2048_highscore', bestScore); }
        }
        spawn(); render();
        if(isOver()) {
          title.textContent = "PARTIE TERMINÉE"; msg.textContent = "Score final : " + score;
          overlay.classList.remove('hidden');
        }
      }
    }

    function isOver() {
      for(let r=0;r<4;r++) for(let c=0;c<4;c++) {
        if(!b[r][c]) return false;
        if(c<3 && b[r][c]===b[r][c+1]) return false;
        if(r<3 && b[r][c]===b[r+1][c]) return false;
      }
      return true;
    }

    window.addEventListener('keydown', e => {
      if(e.key==='ArrowLeft'||e.key==='q') move('L');
      if(e.key==='ArrowRight'||e.key==='d') move('R');
      if(e.key==='ArrowUp'||e.key==='z') move('U');
      if(e.key==='ArrowDown'||e.key==='s') move('D');
    });

    document.getElementById('u').onclick = () => move('U');
    document.getElementById('d').onclick = () => move('D');
    document.getElementById('l').onclick = () => move('L');
    document.getElementById('r').onclick = () => move('R');
    btnRestart.onclick = init;
    init();
  </script>
</body>
</html>
)rawliteral";

#endif
