#ifndef GAME_SNAKE_H
#define GAME_SNAKE_H

#include <pgmspace.h>

const char GAME_SNAKE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Snake Retro - ESP32 Arcade</title>
  <style>
    :root {
      --bg-color: #0d0f18;
      --card-bg: rgba(26, 31, 51, 0.9);
      --card-border: rgba(0, 243, 255, 0.25);
      --neon-cyan: #00f3ff;
      --neon-pink: #ff0055;
      --neon-yellow: #ffe600;
      --neon-green: #00ff66;
      --text-main: #f0f4fc;
      --text-dim: #8b9bb4;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; touch-action: manipulation; }
    body {
      background: var(--bg-color);
      color: var(--text-main);
      font-family: system-ui, -apple-system, sans-serif;
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 10px;
    }
    header {
      width: 100%; max-width: 400px;
      display: flex; justify-content: space-between; align-items: center;
      margin-bottom: 10px;
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
    canvas { display: block; width: 340px; height: 340px; max-width: 90vw; max-height: 90vw; }
    .overlay {
      position: absolute; inset: 0; background: rgba(13, 15, 24, 0.9);
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      gap: 14px; text-align: center; padding: 20px;
    }
    .overlay.hidden { display: none; }
    .overlay h2 { font-size: 1.8rem; color: var(--neon-pink); text-shadow: 0 0 10px var(--neon-pink); }
    .overlay p { color: var(--text-dim); font-size: 0.85rem; }
    .btn-play {
      background: linear-gradient(135deg, var(--neon-cyan), #0077ff);
      color: #000; font-weight: 800; border: none; padding: 12px 24px; border-radius: 10px;
      font-size: 1rem; cursor: pointer; box-shadow: 0 0 15px rgba(0,243,255,0.4);
    }
    .dpad {
      display: flex; flex-direction: column; align-items: center; gap: 6px; margin-top: 10px;
      width: 100%; max-width: 300px;
    }
    .dpad-row { display: flex; gap: 10px; }
    .dpad-btn {
      width: 58px; height: 50px; background: rgba(255, 255, 255, 0.08);
      border: 1px solid rgba(255, 255, 255, 0.2); border-radius: 10px;
      color: #fff; font-size: 1.3rem; display: flex; align-items: center; justify-content: center;
    }
    .dpad-btn:active { background: rgba(0, 243, 255, 0.3); border-color: var(--neon-cyan); }
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
    <canvas id="c" width="340" height="340"></canvas>
    <div id="overlay" class="overlay">
      <h2 id="title">SNAKE RETRO</h2>
      <p id="msg">Touche ou utilise les flèches du clavier</p>
      <button class="btn-play" id="btn-start">Jouer ▶</button>
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
    const c = document.getElementById('c'), ctx = c.getContext('2d');
    const scoreEl = document.getElementById('score'), bestEl = document.getElementById('best');
    const overlay = document.getElementById('overlay'), title = document.getElementById('title');
    const msg = document.getElementById('msg'), btnStart = document.getElementById('btn-start');

    const G = 17, TC = 20;
    let snake = [], food = {x:15, y:15}, dx=1, dy=0, nextDx=1, nextDy=0, score=0, loop=null, isRunning=false;
    let bestScore = parseInt(localStorage.getItem('snake_highscore') || '0', 10);
    bestEl.textContent = bestScore;

    function playTone(freq, type='square', dur=0.08) {
      try {
        const A = window.AudioContext || window.webkitAudioContext;
        if(!A) return;
        const ac = new A();
        const osc = ac.createOscillator(), g = ac.createGain();
        osc.type = type; osc.frequency.value = freq;
        g.gain.setValueAtTime(0.12, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + dur);
        osc.connect(g); g.connect(ac.destination);
        osc.start(); osc.stop(ac.currentTime + dur);
      } catch(e){}
    }

    function init() {
      snake = [{x:10,y:10},{x:9,y:10},{x:8,y:10}];
      dx=1; dy=0; nextDx=1; nextDy=0; score=0; scoreEl.textContent = '0';
      placeFood(); isRunning = true; overlay.classList.add('hidden');
      if(loop) clearInterval(loop);
      loop = setInterval(tick, 105);
    }

    function placeFood() {
      let v=false;
      while(!v){
        food.x = Math.floor(Math.random()*TC); food.y = Math.floor(Math.random()*TC);
        v = !snake.some(s => s.x===food.x && s.y===food.y);
      }
    }

    function setDir(nx, ny){
      if(!isRunning) return;
      if((nx!==0 && dx===-nx) || (ny!==0 && dy===-ny)) return;
      nextDx = nx; nextDy = ny;
    }

    function tick() {
      dx = nextDx; dy = nextDy;
      const head = { x: snake[0].x + dx, y: snake[0].y + dy };
      if(head.x<0 || head.x>=TC || head.y<0 || head.y>=TC || snake.some(s=>s.x===head.x && s.y===head.y)){
        gameOver(); return;
      }
      snake.unshift(head);
      if(head.x===food.x && head.y===food.y){
        score += 10; scoreEl.textContent = score;
        playTone(650, 'sine', 0.1);
        if(score > bestScore){ bestScore = score; bestEl.textContent = bestScore; localStorage.setItem('snake_highscore', bestScore); }
        placeFood();
      } else { snake.pop(); }
      draw();
    }

    function draw() {
      ctx.fillStyle = '#0a0d14'; ctx.fillRect(0, 0, c.width, c.height);
      ctx.fillStyle = '#ff0055'; ctx.shadowColor = '#ff0055'; ctx.shadowBlur = 8;
      ctx.beginPath(); ctx.arc(food.x*G + G/2, food.y*G + G/2, G/2 - 2, 0, Math.PI*2); ctx.fill();
      snake.forEach((s, i) => {
        ctx.fillStyle = i===0 ? '#00ff66' : '#00f3ff';
        ctx.shadowColor = ctx.fillStyle; ctx.shadowBlur = i===0 ? 6 : 3;
        ctx.fillRect(s.x*G + 1, s.y*G + 1, G - 2, G - 2);
      });
      ctx.shadowBlur = 0;
    }

    function gameOver() {
      isRunning = false; clearInterval(loop); playTone(180, 'sawtooth', 0.25);
      title.textContent = "GAME OVER"; msg.textContent = "Score : " + score;
      btnStart.textContent = "Rejouer ↺"; overlay.classList.remove('hidden');
    }

    window.addEventListener('keydown', e => {
      if(e.key==='ArrowUp'||e.key==='z') { setDir(0,-1); e.preventDefault(); }
      if(e.key==='ArrowDown'||e.key==='s') { setDir(0,1); e.preventDefault(); }
      if(e.key==='ArrowLeft'||e.key==='q') { setDir(-1,0); e.preventDefault(); }
      if(e.key==='ArrowRight'||e.key==='d') { setDir(1,0); e.preventDefault(); }
      if(e.key===' ' && !isRunning) init();
    });

    document.getElementById('u').onclick = () => setDir(0,-1);
    document.getElementById('d').onclick = () => setDir(0,1);
    document.getElementById('l').onclick = () => setDir(-1,0);
    document.getElementById('r').onclick = () => setDir(1,0);
    btnStart.onclick = init;
    draw();
  </script>
</body>
</html>
)rawliteral";

#endif
