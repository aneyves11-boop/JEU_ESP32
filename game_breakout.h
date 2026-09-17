#ifndef GAME_BREAKOUT_H
#define GAME_BREAKOUT_H

#include <pgmspace.h>

const char GAME_BREAKOUT_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Casse-Briques - ESP32 Arcade</title>
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
      width: 100%; max-width: 360px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 10px;
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
    .canvas-container {
      position: relative; border: 2px solid var(--card-border);
      border-radius: 16px; overflow: hidden; background: #000;
      box-shadow: 0 0 20px rgba(0, 243, 255, 0.2);
    }
    canvas { display: block; width: 340px; height: 420px; max-width: 90vw; max-height: 65vh; }
    .overlay {
      position: absolute; inset: 0; background: rgba(13, 15, 24, 0.9);
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      gap: 14px; text-align: center; padding: 20px;
    }
    .overlay.hidden { display: none; }
    .overlay h2 { font-size: 1.8rem; color: var(--neon-pink); }
    .btn-play {
      background: linear-gradient(135deg, var(--neon-cyan), #0077ff);
      color: #000; font-weight: 800; border: none; padding: 12px 24px; border-radius: 10px;
      font-size: 1rem; cursor: pointer;
    }
    .controls { display: flex; gap: 20px; margin-top: 10px; width: 100%; max-width: 340px; }
    .btn-lr {
      flex: 1; height: 50px; background: rgba(255, 255, 255, 0.08);
      border: 1px solid rgba(255, 255, 255, 0.2); border-radius: 10px;
      color: #fff; font-weight: bold; font-size: 1.1rem;
    }
    .btn-lr:active { background: rgba(0, 243, 255, 0.3); }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="back-btn">◀ Menu</a>
    <div class="score-panel">
      <div class="score-box"><div class="label">Score</div><div class="val" id="score">0</div></div>
      <div class="score-box best"><div class="label">Record</div><div class="val" id="best">0</div></div>
    </div>
  </header>

  <div class="canvas-container">
    <canvas id="c" width="340" height="420"></canvas>
    <div id="overlay" class="overlay">
      <h2 id="title">CASSE-BRIQUES</h2>
      <p id="msg">Glisse ton doigt ou utilise les boutons pour déplacer la raquette</p>
      <button class="btn-play" id="btn-start">Lancer la Balle 🚀</button>
    </div>
  </div>

  <div class="controls">
    <button class="btn-lr" id="b-l">◀ GAUCHE</button>
    <button class="btn-lr" id="b-r">DROITE ▶</button>
  </div>

  <script>
    const c = document.getElementById('c'), ctx = c.getContext('2d');
    const scoreEl = document.getElementById('score'), bestEl = document.getElementById('best');
    const overlay = document.getElementById('overlay'), title = document.getElementById('title');
    const msg = document.getElementById('msg'), btnStart = document.getElementById('btn-start');

    const PW = 70, PH = 10;
    let px = (c.width - PW)/2, bx = c.width/2, by = c.height - 40, bdx = 3, bdy = -3.5;
    let bricks = [], score = 0, isRunning = false, animId = null, leftP = false, rightP = false;
    let bestScore = parseInt(localStorage.getItem('breakout_highscore') || '0', 10);
    bestEl.textContent = bestScore;

    const colors = ['#ff0055', '#ff7700', '#ffe600', '#00ff66', '#00f3ff'];

    function sound(f, dur=0.08) {
      try {
        const A = window.AudioContext || window.webkitAudioContext;
        if(!A) return;
        const ac = new A(), o = ac.createOscillator(), g = ac.createGain();
        o.type = 'square'; o.frequency.value = f;
        g.gain.setValueAtTime(0.1, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + dur);
        o.connect(g); g.connect(ac.destination);
        o.start(); o.stop(ac.currentTime + dur);
      } catch(e){}
    }

    function init() {
      px = (c.width - PW)/2; bx = c.width/2; by = c.height - 40;
      bdx = (Math.random()>0.5?1:-1)*3; bdy = -3.5; score = 0; scoreEl.textContent = '0';
      bricks = [];
      for(let col=0; col<6; col++) {
        bricks[col] = [];
        for(let row=0; row<5; row++) bricks[col][row] = { x: 0, y: 0, status: 1 };
      }
      isRunning = true; overlay.classList.add('hidden');
      if(animId) cancelAnimationFrame(animId);
      loop();
    }

    function loop() {
      if(!isRunning) return;
      if(rightP && px < c.width - PW) px += 6;
      if(leftP && px > 0) px -= 6;

      bx += bdx; by += bdy;
      if(bx + 5 > c.width || bx - 5 < 0) { bdx = -bdx; sound(300); }
      if(by - 5 < 0) { bdy = -bdy; sound(300); }

      const py = c.height - PH - 10;
      if(by + 5 >= py && by - 5 <= py + PH && bx >= px && bx <= px + PW) {
        const hit = (bx - (px + PW/2)) / (PW/2);
        bdx = hit * 4; bdy = -Math.abs(bdy); sound(450);
      }

      if(by - 5 > c.height) {
        gameOver(); return;
      }

      // Bricks collision
      let active = 0;
      const bw = 48, bh = 14, pad = 6, topOff = 35, leftOff = 12;
      for(let col=0; col<6; col++) {
        for(let row=0; row<5; row++) {
          const b = bricks[col][row];
          if(b.status === 1) {
            active++;
            b.x = col*(bw+pad) + leftOff; b.y = row*(bh+pad) + topOff;
            if(bx > b.x && bx < b.x + bw && by > b.y && by < b.y + bh) {
              bdy = -bdy; b.status = 0; score += (5 - row)*10;
              scoreEl.textContent = score; sound(700);
              if(score > bestScore) { bestScore = score; bestEl.textContent = bestScore; localStorage.setItem('breakout_highscore', bestScore); }
            }
          }
        }
      }

      if(active === 0) { victory(); return; }

      // Draw
      ctx.fillStyle = '#0a0d18'; ctx.fillRect(0, 0, c.width, c.height);
      for(let col=0; col<6; col++) {
        for(let row=0; row<5; row++) {
          if(bricks[col][row].status === 1) {
            ctx.fillStyle = colors[row];
            ctx.fillRect(col*(bw+pad) + leftOff, row*(bh+pad) + topOff, bw, bh);
          }
        }
      }
      ctx.fillStyle = '#00f3ff'; ctx.fillRect(px, py, PW, PH);
      ctx.fillStyle = '#ffe600'; ctx.beginPath(); ctx.arc(bx, by, 5, 0, Math.PI*2); ctx.fill();

      animId = requestAnimationFrame(loop);
    }

    function gameOver() {
      isRunning = false; sound(140, 0.3);
      title.textContent = "GAME OVER"; msg.textContent = "Score final : " + score;
      btnStart.textContent = "Rejouer ↺"; overlay.classList.remove('hidden');
    }

    function victory() {
      isRunning = false; sound(880, 0.4);
      title.textContent = "VICTOIRE ! 🎉"; msg.textContent = "Score : " + score;
      btnStart.textContent = "Rejouer ↺"; overlay.classList.remove('hidden');
    }

    window.addEventListener('keydown', e => {
      if(e.key==='ArrowRight'||e.key==='d') rightP = true;
      if(e.key==='ArrowLeft'||e.key==='q') leftP = true;
    });
    window.addEventListener('keyup', e => {
      if(e.key==='ArrowRight'||e.key==='d') rightP = false;
      if(e.key==='ArrowLeft'||e.key==='q') leftP = false;
    });

    c.addEventListener('touchmove', e => {
      if(isRunning && e.touches.length) {
        e.preventDefault();
        const rect = c.getBoundingClientRect();
        px = Math.max(0, Math.min(c.width - PW, (e.touches[0].clientX - rect.left)*(c.width/rect.width) - PW/2));
      }
    }, { passive: false });

    document.getElementById('b-l').ontouchstart = (e) => { e.preventDefault(); leftP = true; };
    document.getElementById('b-l').ontouchend = () => { leftP = false; };
    document.getElementById('b-l').onmousedown = () => { leftP = true; };
    document.getElementById('b-l').onmouseup = () => { leftP = false; };

    document.getElementById('b-r').ontouchstart = (e) => { e.preventDefault(); rightP = true; };
    document.getElementById('b-r').ontouchend = () => { rightP = false; };
    document.getElementById('b-r').onmousedown = () => { rightP = true; };
    document.getElementById('b-r').onmouseup = () => { rightP = false; };

    btnStart.onclick = init;
  </script>
</body>
</html>
)rawliteral";

#endif
