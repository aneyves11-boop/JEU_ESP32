#ifndef GAME_FLAPPY_H
#define GAME_FLAPPY_H

#include <pgmspace.h>

const char GAME_FLAPPY_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Flappy ESP - ESP32 Arcade</title>
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
    canvas { display: block; width: 330px; height: 440px; max-width: 90vw; max-height: 65vh; }
    .overlay {
      position: absolute; inset: 0; background: rgba(13, 15, 24, 0.9);
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      gap: 14px; text-align: center; padding: 20px;
    }
    .overlay.hidden { display: none; }
    .overlay h2 { font-size: 1.8rem; color: var(--neon-pink); }
    .overlay p { color: var(--text-dim); font-size: 0.85rem; }
    .btn-play {
      background: linear-gradient(135deg, var(--neon-cyan), #0077ff);
      color: #000; font-weight: 800; border: none; padding: 12px 24px; border-radius: 10px;
      font-size: 1rem; cursor: pointer;
    }
    .tap-btn {
      width: 100%; max-width: 330px; height: 50px; margin-top: 10px;
      background: linear-gradient(135deg, rgba(0,243,255,0.3), rgba(255,0,85,0.3));
      border: 1px solid var(--neon-cyan); border-radius: 12px;
      color: #fff; font-weight: bold; font-size: 1rem; cursor: pointer;
    }
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

  <div class="canvas-container">
    <canvas id="c" width="330" height="440"></canvas>
    <div id="overlay" class="overlay">
      <h2 id="title">FLAPPY ESP</h2>
      <p id="msg">Touche l'écran ou appuie sur ESPACE pour voler</p>
      <button class="btn-play" id="btn-start">Décoller 🚀</button>
    </div>
  </div>

  <button class="tap-btn" id="btn-tap">VOLER / SAUTER 🚀</button>

  <script>
    const c = document.getElementById('c'), ctx = c.getContext('2d');
    const scoreEl = document.getElementById('score'), bestEl = document.getElementById('best');
    const overlay = document.getElementById('overlay'), title = document.getElementById('title');
    const msg = document.getElementById('msg'), btnStart = document.getElementById('btn-start');
    const btnTap = document.getElementById('btn-tap');

    let bird = { x: 50, y: 200, vy: 0, r: 12 };
    let pipes = [], pipeTimer = 0, score = 0, isRunning = false, animId = null;
    let bestScore = parseInt(localStorage.getItem('flappy_highscore') || '0', 10);
    bestEl.textContent = bestScore;

    function sound(f, dur=0.08) {
      try {
        const A = window.AudioContext || window.webkitAudioContext;
        if(!A) return;
        const ac = new A();
        const o = ac.createOscillator(), g = ac.createGain();
        o.type = 'square'; o.frequency.value = f;
        g.gain.setValueAtTime(0.1, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + dur);
        o.connect(g); g.connect(ac.destination);
        o.start(); o.stop(ac.currentTime + dur);
      } catch(e){}
    }

    function init() {
      bird = { x: 50, y: c.height/2, vy: 0, r: 12 };
      pipes = []; pipeTimer = 0; score = 0; scoreEl.textContent = '0';
      isRunning = true; overlay.classList.add('hidden');
      if(animId) cancelAnimationFrame(animId);
      loop();
    }

    function flap() {
      if(!isRunning) { init(); return; }
      bird.vy = -6.2;
      sound(400, 0.08);
    }

    function loop() {
      if(!isRunning) return;
      bird.vy += 0.35;
      bird.y += bird.vy;

      if(bird.y + bird.r >= c.height - 10 || bird.y - bird.r <= 0) {
        gameOver(); return;
      }

      pipeTimer++;
      if(pipeTimer >= 90) {
        pipeTimer = 0;
        const gap = 120, top = Math.floor(Math.random()*(c.height - gap - 80)) + 40;
        pipes.push({ x: c.width, top: top, bottom: top + gap, passed: false });
      }

      for(let i=pipes.length-1; i>=0; i--) {
        const p = pipes[i]; p.x -= 2.2;
        if(!p.passed && p.x + 45 < bird.x) {
          p.passed = true; score++; scoreEl.textContent = score; sound(750, 0.1);
          if(score > bestScore) { bestScore = score; bestEl.textContent = bestScore; localStorage.setItem('flappy_highscore', bestScore); }
        }
        if(bird.x + bird.r > p.x && bird.x - bird.r < p.x + 45) {
          if(bird.y - bird.r < p.top || bird.y + bird.r > p.bottom) {
            gameOver(); return;
          }
        }
        if(p.x + 45 < 0) pipes.splice(i, 1);
      }

      // Draw
      ctx.fillStyle = '#0a0d18'; ctx.fillRect(0, 0, c.width, c.height);
      pipes.forEach(p => {
        ctx.fillStyle = 'rgba(0, 243, 255, 0.25)'; ctx.strokeStyle = '#00f3ff'; ctx.lineWidth = 2;
        ctx.fillRect(p.x, 0, 45, p.top); ctx.strokeRect(p.x, 0, 45, p.top);
        ctx.fillRect(p.x, p.bottom, 45, c.height - p.bottom); ctx.strokeRect(p.x, p.bottom, 45, c.height - p.bottom);
      });
      ctx.fillStyle = '#ff0055'; ctx.fillRect(0, c.height - 8, c.width, 8);
      ctx.fillStyle = '#ffe600'; ctx.beginPath(); ctx.arc(bird.x, bird.y, bird.r, 0, Math.PI*2); ctx.fill();
      ctx.fillStyle = '#000'; ctx.beginPath(); ctx.arc(bird.x + 4, bird.y - 2, 3, 0, Math.PI*2); ctx.fill();

      animId = requestAnimationFrame(loop);
    }

    function gameOver() {
      isRunning = false; sound(150, 0.3);
      title.textContent = "CRASH !"; msg.textContent = "Score : " + score;
      btnStart.textContent = "Rejouer ↺"; overlay.classList.remove('hidden');
    }

    window.addEventListener('keydown', e => { if(e.code==='Space'||e.key==='ArrowUp'){ flap(); e.preventDefault(); } });
    c.addEventListener('touchstart', e => { e.preventDefault(); flap(); }, { passive:false });
    c.addEventListener('mousedown', flap);
    btnTap.addEventListener('touchstart', e => { e.preventDefault(); flap(); }, { passive:false });
    btnTap.addEventListener('click', flap);
    btnStart.onclick = init;
  </script>
</body>
</html>
)rawliteral";

#endif
