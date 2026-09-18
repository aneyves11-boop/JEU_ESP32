#ifndef GAME_INVADERS_H
#define GAME_INVADERS_H

#include <pgmspace.h>

const char GAME_INVADERS_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Space Invaders - ESP32 Arcade</title>
  <style>
    :root {
      --bg: #090b14; --card: rgba(22, 27, 46, 0.9); --card-border: rgba(0, 243, 255, 0.25);
      --cyan: #00f3ff; --pink: #ff0055; --yellow: #ffe600; --green: #00ff66; --text: #f0f4fc; --dim: #8b9bb4;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; touch-action: manipulation; }
    body { background: var(--bg); color: var(--text); font-family: system-ui, sans-serif; min-height: 100vh; display: flex; flex-direction: column; align-items: center; padding: 10px; }
    header { width: 100%; max-width: 360px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
    .btn-back { background: var(--card); border: 1px solid var(--card-border); color: #fff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.85rem; font-weight: bold; }
    .score-panel { display: flex; gap: 8px; }
    .score-box { background: var(--card); border: 1px solid rgba(255,255,255,0.1); padding: 4px 10px; border-radius: 8px; text-align: center; min-width: 65px; }
    .score-box .lbl { font-size: 0.65rem; color: var(--dim); text-transform: uppercase; }
    .score-box .val { font-size: 1.1rem; font-weight: 800; color: var(--cyan); }
    .score-box.best .val { color: var(--yellow); }
    .stage { position: relative; border: 2px solid var(--card-border); border-radius: 14px; overflow: hidden; background: #04060a; box-shadow: 0 0 20px rgba(0,243,255,0.2); }
    canvas { display: block; width: 340px; height: 420px; max-width: 92vw; max-height: 62vh; }
    .overlay { position: absolute; inset: 0; background: rgba(9, 11, 18, 0.9); display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 14px; text-align: center; padding: 20px; }
    .overlay.hidden { display: none; }
    .overlay h2 { font-size: 1.8rem; color: var(--pink); }
    .btn-play { background: linear-gradient(135deg, var(--cyan), #0077ff); color: #000; font-weight: 800; border: none; padding: 12px 24px; border-radius: 10px; font-size: 1rem; cursor: pointer; }
    .controls { width: 100%; max-width: 340px; margin-top: 8px; display: flex; gap: 8px; }
    .btn-lr { flex: 1; height: 48px; background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); border-radius: 10px; color: #fff; font-weight: bold; font-size: 1.2rem; }
    .btn-fire { flex: 1.5; height: 48px; background: linear-gradient(135deg, #ff0055, #ff5500); border: none; border-radius: 10px; color: #fff; font-weight: 900; font-size: 1rem; }
    .btn-lr:active, .btn-fire:active { transform: scale(0.96); }
  </style>
<script src="/pause.js"></script></head>
<body>
  <header>
    <a href="/hub" class="btn-back">◀ Menu</a>
    <div class="score-panel">
      <div class="score-box"><div class="lbl">Score</div><div class="val" id="score">0</div></div>
      <div class="score-box best"><div class="lbl">Record</div><div class="val" id="best">0</div></div>
    </div>
  </header>

  <div class="stage">
    <canvas id="c" width="340" height="420"></canvas>
    <div id="overlay" class="overlay">
      <h2 id="title">SPACE INVADERS</h2>
      <p id="msg">Déplace ton vaisseau et tire sur les envahisseurs extraterrestres !</p>
      <button class="btn-play" id="btn-start">Jouer ▶</button>
    </div>
  </div>

  <div class="controls">
    <button class="btn-lr" id="b-l">◀</button>
    <button class="btn-lr" id="b-r">▶</button>
    <button class="btn-fire" id="b-f">🔥 TIRER</button>
  </div>

  <script>
    const c = document.getElementById('c'), ctx = c.getContext('2d');
    const scoreEl = document.getElementById('score'), bestEl = document.getElementById('best');
    const overlay = document.getElementById('overlay'), title = document.getElementById('title'), msg = document.getElementById('msg'), btnStart = document.getElementById('btn-start');

    let px = c.width / 2, bullets = [], alienBullets = [], aliens = [], ufo = null;
    let alienVx = 1.4, alienDir = 1, isRunning = false, loopId = null, score = 0, level = 1;
    let bestScore = parseInt(localStorage.getItem('invaders_highscore') || '0', 10);
    bestEl.textContent = bestScore;

    let leftP = false, rightP = false;

    function sound(f, type='square', dur=0.08) {
      try {
        const AC = window.AudioContext || window.webkitAudioContext;
        if(!AC) return;
        const ac = new AC(), o = ac.createOscillator(), g = ac.createGain();
        o.type = type; o.frequency.value = f;
        g.gain.setValueAtTime(0.1, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + dur);
        o.connect(g); g.connect(ac.destination);
        o.start(); o.stop(ac.currentTime + dur);
      } catch(e){}
    }

    function spawnAliens() {
      aliens = [];
      const rows = 4, cols = 6;
      for(let r=0; r<rows; r++) {
        for(let col=0; col<cols; col++) {
          aliens.push({
            x: 35 + col * 46,
            y: 35 + r * 28,
            type: r === 0 ? 3 : (r < 2 ? 2 : 1),
            alive: true
          });
        }
      }
      alienVx = 1.2 + level * 0.3;
      alienDir = 1;
    }

    function init() {
      px = c.width / 2; bullets = []; alienBullets = []; score = 0; level = 1;
      scoreEl.textContent = '0'; spawnAliens();
      isRunning = true; overlay.classList.add('hidden');
      if(loopId) cancelAnimationFrame(loopId);
      loop();
    }

    function fire() {
      if(!isRunning) return;
      if(bullets.length < 3) {
        bullets.push({ x: px, y: c.height - 35 });
        sound(880, 'sine', 0.05);
      }
    }

    function loop() {
      if(!isRunning) return;

      if(leftP && px > 20) px -= 5;
      if(rightP && px < c.width - 20) px += 5;

      // Move player bullets
      for(let i=bullets.length-1; i>=0; i--) {
        const b = bullets[i]; b.y -= 7;
        if(b.y < 0) bullets.splice(i, 1);
      }

      // Move alien bullets
      for(let i=alienBullets.length-1; i>=0; i--) {
        const ab = alienBullets[i]; ab.y += 4;
        if(Math.abs(ab.x - px) < 14 && Math.abs(ab.y - (c.height - 25)) < 12) {
          gameOver(); return;
        }
        if(ab.y > c.height) alienBullets.splice(i, 1);
      }

      // Alien movement & descent
      let hitEdge = false;
      let livingAliens = aliens.filter(a => a.alive);

      if(livingAliens.length === 0) {
        level++; score += 100; scoreEl.textContent = score; sound(900, 'triangle', 0.3);
        spawnAliens();
      }

      livingAliens.forEach(a => {
        a.x += alienVx * alienDir;
        if(a.x < 15 || a.x > c.width - 35) hitEdge = true;

        // Alien random shooting
        if(Math.random() < 0.003 + level * 0.001 && alienBullets.length < 4) {
          alienBullets.push({ x: a.x + 11, y: a.y + 16 });
        }

        // Alien bullet collision
        bullets.forEach((b, bi) => {
          if(b.x > a.x && b.x < a.x + 22 && b.y > a.y && b.y < a.y + 18) {
            a.alive = false; bullets.splice(bi, 1);
            score += a.type * 10; scoreEl.textContent = score; sound(400 + a.type*100);
            if(score > bestScore) {
              bestScore = score; bestEl.textContent = bestScore;
              localStorage.setItem('invaders_highscore', bestScore);
            }
          }
        });

        // Invasion reached bottom
        if(a.y + 18 >= c.height - 40) {
          gameOver(); return;
        }
      });

      if(hitEdge) {
        alienDir = -alienDir;
        aliens.forEach(a => a.y += 12);
      }

      // Draw Screen
      ctx.fillStyle = '#04060a'; ctx.fillRect(0, 0, c.width, c.height);

      // Stars
      ctx.fillStyle = 'rgba(255,255,255,0.2)';
      for(let i=0; i<15; i++) {
        ctx.fillRect((i*37)%c.width, (i*53)%c.height, 2, 2);
      }

      // Draw Player Cannon
      ctx.fillStyle = '#00f3ff'; ctx.fillRect(px - 14, c.height - 24, 28, 12);
      ctx.fillRect(px - 4, c.height - 32, 8, 8);

      // Draw Bullets
      ctx.fillStyle = '#00ff66';
      bullets.forEach(b => ctx.fillRect(b.x - 2, b.y, 4, 8));

      ctx.fillStyle = '#ff0055';
      alienBullets.forEach(ab => ctx.fillRect(ab.x - 2, ab.y, 4, 8));

      // Draw Aliens
      aliens.forEach(a => {
        if(!a.alive) return;
        ctx.fillStyle = a.type === 3 ? '#ff0055' : (a.type === 2 ? '#ffe600' : '#00f3ff');
        ctx.fillRect(a.x + 2, a.y + 2, 18, 14);
        ctx.fillStyle = '#000';
        ctx.fillRect(a.x + 5, a.y + 6, 3, 3);
        ctx.fillRect(a.x + 14, a.y + 6, 3, 3);
      });

      loopId = requestAnimationFrame(loop);
    }

    function gameOver() {
      isRunning = false; sound(130, 'sawtooth', 0.4);
      title.textContent = "BASE DÉTRUITE !"; msg.textContent = "Score final : " + score;
      btnStart.textContent = "Rejouer ↺"; overlay.classList.remove('hidden');
    }

    // Controls
    window.onkeydown = e => {
      if(e.key==='ArrowLeft'||e.key==='q') leftP = true;
      if(e.key==='ArrowRight'||e.key==='d') rightP = true;
      if(e.code==='Space'||e.key===' ') { fire(); e.preventDefault(); }
    };
    window.onkeyup = e => {
      if(e.key==='ArrowLeft'||e.key==='q') leftP = false;
      if(e.key==='ArrowRight'||e.key==='d') rightP = false;
    };

    document.getElementById('b-l').ontouchstart = (e)=>{ e.preventDefault(); leftP = true; };
    document.getElementById('b-l').ontouchend = ()=> leftP = false;
    document.getElementById('b-l').onmousedown = ()=> leftP = true;
    document.getElementById('b-l').onmouseup = ()=> leftP = false;

    document.getElementById('b-r').ontouchstart = (e)=>{ e.preventDefault(); rightP = true; };
    document.getElementById('b-r').ontouchend = ()=> rightP = false;
    document.getElementById('b-r').onmousedown = ()=> rightP = true;
    document.getElementById('b-r').onmouseup = ()=> rightP = false;

    document.getElementById('b-f').ontouchstart = (e)=>{ e.preventDefault(); fire(); };
    document.getElementById('b-f').onclick = fire;
    btnStart.onclick = init;
  </script>
</body>
</html>
)rawliteral";

#endif
