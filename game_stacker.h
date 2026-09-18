#ifndef GAME_STACKER_H
#define GAME_STACKER_H

#include <pgmspace.h>

const char GAME_STACKER_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Tower Stacker - ESP32 Arcade</title>
  <style>
    :root {
      --bg: #090b12; --card: rgba(22, 27, 46, 0.9); --card-border: rgba(0, 243, 255, 0.25);
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
    .stage { position: relative; border: 2px solid var(--card-border); border-radius: 14px; overflow: hidden; background: #060810; box-shadow: 0 0 20px rgba(0,243,255,0.2); }
    canvas { display: block; width: 340px; height: 420px; max-width: 92vw; max-height: 62vh; cursor: pointer; }
    .overlay { position: absolute; inset: 0; background: rgba(9, 11, 18, 0.9); display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 14px; text-align: center; padding: 20px; }
    .overlay.hidden { display: none; }
    .overlay h2 { font-size: 1.8rem; color: var(--pink); }
    .btn-play { background: linear-gradient(135deg, var(--cyan), #0077ff); color: #000; font-weight: 800; border: none; padding: 12px 24px; border-radius: 10px; font-size: 1rem; cursor: pointer; }
    .btn-drop { width: 100%; max-width: 340px; height: 50px; margin-top: 8px; background: linear-gradient(135deg, var(--cyan), #0077ff); border: none; border-radius: 12px; color: #000; font-weight: 900; font-size: 1.1rem; cursor: pointer; }
    .btn-drop:active { transform: scale(0.97); }
  </style>
<script src="/pause.js"></script></head>
<body>
  <header>
    <a href="/hub" class="btn-back">◀ Menu</a>
    <div class="score-panel">
      <div class="score-box"><div class="lbl">Étages</div><div class="val" id="score">0</div></div>
      <div class="score-box best"><div class="lbl">Record</div><div class="val" id="best">0</div></div>
    </div>
  </header>

  <div class="stage">
    <canvas id="c" width="340" height="420"></canvas>
    <div id="overlay" class="overlay">
      <h2 id="title">TOWER STACKER</h2>
      <p id="msg">Touche l'écran pour empiler les étages au millimètre près !</p>
      <button class="btn-play" id="btn-start">Construire la Tour 🏢</button>
    </div>
  </div>

  <button class="btn-drop" id="btn-drop">EMPILER L'ÉTAGE 🏗️</button>

  <script>
    const c = document.getElementById('c'), ctx = c.getContext('2d');
    const scoreEl = document.getElementById('score'), bestEl = document.getElementById('best');
    const overlay = document.getElementById('overlay'), title = document.getElementById('title'), msg = document.getElementById('msg'), btnStart = document.getElementById('btn-start');
    const btnDrop = document.getElementById('btn-drop');

    const BLOCK_H = 18;
    let stack = [], cur = { x: 0, y: 0, w: 200, vx: 3.8 };
    let score = 0, isRunning = false, loopId = null, scrollY = 0;
    let bestScore = parseInt(localStorage.getItem('stacker_highscore') || '0', 10);
    bestEl.textContent = bestScore;

    const colors = ['#00f3ff', '#00ff66', '#ffe600', '#ff0055', '#9d4edd', '#ff7700'];

    function sound(f, dur=0.08) {
      try {
        const AC = window.AudioContext || window.webkitAudioContext;
        if(!AC) return;
        const ac = new AC(), o = ac.createOscillator(), g = ac.createGain();
        o.type = 'sine'; o.frequency.value = f;
        g.gain.setValueAtTime(0.12, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + dur);
        o.connect(g); g.connect(ac.destination);
        o.start(); o.stop(ac.currentTime + dur);
      } catch(e){}
    }

    function init() {
      stack = [
        { x: (c.width - 200)/2, y: c.height - BLOCK_H - 10, w: 200, color: colors[0] }
      ];
      score = 0; scrollY = 0; scoreEl.textContent = '0';
      spawnNext();
      isRunning = true; overlay.classList.add('hidden');
      if(loopId) cancelAnimationFrame(loopId);
      loop();
    }

    function spawnNext() {
      const prev = stack[stack.length - 1];
      const speed = 3.6 + Math.min(6, score * 0.25);
      cur = {
        x: 0,
        y: prev.y - BLOCK_H,
        w: prev.w,
        vx: (Math.random() > 0.5 ? 1 : -1) * speed,
        color: colors[(stack.length) % colors.length]
      };
    }

    function dropBlock() {
      if(!isRunning) return;
      const prev = stack[stack.length - 1];
      const diff = cur.x - prev.x;

      if(Math.abs(diff) >= cur.w) {
        // Total miss
        gameOver(); return;
      }

      // Slicing logic
      let newW = cur.w - Math.abs(diff);
      let newX = diff > 0 ? cur.x : prev.x;

      // Perfect combo check
      if(Math.abs(diff) < 3) {
        newW = prev.w; newX = prev.x; // Perfect snap
        sound(880, 0.15);
      } else {
        sound(400 + stack.length * 20);
      }

      stack.push({ x: newX, y: cur.y, w: newW, color: cur.color });
      score++; scoreEl.textContent = score;

      if(score > bestScore) {
        bestScore = score; bestEl.textContent = bestScore;
        localStorage.setItem('stacker_highscore', bestScore);
      }

      // Smooth camera scroll down if stack gets high
      if(cur.y < c.height / 2) {
        scrollY += BLOCK_H;
      }

      spawnNext();
    }

    function loop() {
      if(!isRunning) return;

      cur.x += cur.vx;
      if(cur.x < 0 || cur.x + cur.w > c.width) {
        cur.vx = -cur.vx;
      }

      // --- RENDER ---
      ctx.fillStyle = '#060810'; ctx.fillRect(0, 0, c.width, c.height);

      ctx.save();
      ctx.translate(0, scrollY);

      // Draw Stacked Blocks
      stack.forEach((b, i) => {
        ctx.fillStyle = b.color;
        ctx.shadowColor = b.color; ctx.shadowBlur = 6;
        ctx.fillRect(b.x, b.y, b.w, BLOCK_H - 1);
      });

      // Draw Current Moving Block
      ctx.fillStyle = cur.color;
      ctx.shadowColor = '#fff'; ctx.shadowBlur = 10;
      ctx.fillRect(cur.x, cur.y, cur.w, BLOCK_H - 1);

      ctx.restore();
      ctx.shadowBlur = 0;

      loopId = requestAnimationFrame(loop);
    }

    function gameOver() {
      isRunning = false; sound(130, 0.4);
      title.textContent = "TOUR ÉCROULÉE !"; msg.textContent = `Hauteur atteinte : ${score} étages`;
      btnStart.textContent = "Recommencer ↺"; overlay.classList.remove('hidden');
    }

    c.onclick = dropBlock;
    btnDrop.onclick = dropBlock;
    window.onkeydown = e => { if(e.code==='Space'||e.key===' ') { dropBlock(); e.preventDefault(); } };
    btnStart.onclick = init;
  </script>
</body>
</html>
)rawliteral";

#endif
