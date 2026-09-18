#ifndef GAME_PIANOTILES_H
#define GAME_PIANOTILES_H

#include <pgmspace.h>

const char GAME_PIANOTILES_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Piano Tiles - ESP32 Arcade</title>
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
    .piano-btns { width: 100%; max-width: 340px; margin-top: 8px; display: grid; grid-template-columns: repeat(4, 1fr); gap: 6px; }
    .p-key { height: 50px; background: rgba(0,243,255,0.15); border: 1px solid var(--cyan); border-radius: 10px; color: #fff; font-weight: 900; font-size: 1.1rem; }
    .p-key:active { background: var(--cyan); color: #000; }
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
      <h2 id="title">PIANO TILES</h2>
      <p id="msg">Touche les touches lumineuses en rythme sans en manquer une seule !</p>
      <button class="btn-play" id="btn-start">Jouer du Piano 🎹</button>
    </div>
  </div>

  <div class="piano-btns">
    <button class="p-key" id="k-0">1</button>
    <button class="p-key" id="k-1">2</button>
    <button class="p-key" id="k-2">3</button>
    <button class="p-key" id="k-3">4</button>
  </div>

  <script>
    const c = document.getElementById('c'), ctx = c.getContext('2d');
    const scoreEl = document.getElementById('score'), bestEl = document.getElementById('best');
    const overlay = document.getElementById('overlay'), title = document.getElementById('title'), msg = document.getElementById('msg'), btnStart = document.getElementById('btn-start');

    const colW = 340 / 4;
    const frequencies = [261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25]; // C4 to C5
    let tiles = [], score = 0, speed = 3.8, isRunning = false, loopId = null, timer = 0, noteIdx = 0;
    let bestScore = parseInt(localStorage.getItem('piano_highscore') || '0', 10);
    bestEl.textContent = bestScore;

    function playPianoNote(col) {
      try {
        const AC = window.AudioContext || window.webkitAudioContext;
        if(!AC) return;
        const ac = new AC(), o = ac.createOscillator(), g = ac.createGain();
        o.type = 'triangle';
        o.frequency.value = frequencies[(col + noteIdx) % frequencies.length];
        g.gain.setValueAtTime(0.18, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + 0.35);
        o.connect(g); g.connect(ac.destination);
        o.start(); o.stop(ac.currentTime + 0.35);
        noteIdx++;
      } catch(e){}
    }

    function init() {
      tiles = []; score = 0; speed = 4; timer = 0; noteIdx = 0;
      scoreEl.textContent = '0'; isRunning = true;
      overlay.classList.add('hidden');
      if(loopId) cancelAnimationFrame(loopId);
      // Spawn initial tiles
      for(let i=0; i<4; i++) {
        tiles.push({ col: Math.floor(Math.random()*4), y: -100 - i*110, h: 100, hit: false });
      }
      loop();
    }

    function tapColumn(col) {
      if(!isRunning) return;
      // Find the lowest active tile in this column
      let found = false;
      for(let i=0; i<tiles.length; i++) {
        const t = tiles[i];
        if(!t.hit && t.col === col && t.y + t.h > c.height - 120 && t.y < c.height - 10) {
          t.hit = true;
          found = true;
          score++; scoreEl.textContent = score;
          playPianoNote(col);
          speed = Math.min(9, 4 + score * 0.08);
          if(score > bestScore) {
            bestScore = score; bestEl.textContent = bestScore;
            localStorage.setItem('piano_highscore', bestScore);
          }
          break;
        }
      }
      if(!found) {
        // Tapped empty lane or wrong timing!
        gameOver();
      }
    }

    function loop() {
      if(!isRunning) return;

      // Move tiles down
      for(let i=tiles.length-1; i>=0; i--) {
        const t = tiles[i];
        t.y += speed;

        // Missed tile check
        if(t.y > c.height - 20 && !t.hit) {
          gameOver(); return;
        }

        if(t.y > c.height + 60) {
          tiles.splice(i, 1);
        }
      }

      // Keep spawning continuous tiles
      const topTile = tiles.reduce((min, t) => t.y < min ? t.y : min, c.height);
      if(topTile > -10) {
        tiles.push({
          col: Math.floor(Math.random()*4),
          y: topTile - 110,
          h: 100,
          hit: false
        });
      }

      // --- RENDER ---
      ctx.fillStyle = '#060810'; ctx.fillRect(0, 0, c.width, c.height);

      // 4 Column dividers
      ctx.strokeStyle = 'rgba(255,255,255,0.08)'; ctx.lineWidth = 2;
      for(let i=1; i<4; i++) {
        ctx.beginPath(); ctx.moveTo(i*colW, 0); ctx.lineTo(i*colW, c.height); ctx.stroke();
      }

      // Hit Target Zone
      ctx.fillStyle = 'rgba(0,243,255,0.12)';
      ctx.fillRect(0, c.height - 90, c.width, 80);
      ctx.fillStyle = '#ff0055';
      ctx.fillRect(0, c.height - 90, c.width, 3);

      // Draw Tiles
      tiles.forEach(t => {
        if(t.hit) {
          ctx.fillStyle = 'rgba(0, 255, 102, 0.35)';
        } else {
          ctx.fillStyle = '#00f3ff';
          ctx.shadowColor = '#00f3ff'; ctx.shadowBlur = 8;
        }
        ctx.fillRect(t.col * colW + 3, t.y, colW - 6, t.h);
        ctx.shadowBlur = 0;
      });

      loopId = requestAnimationFrame(loop);
    }

    function gameOver() {
      isRunning = false;
      title.textContent = "FAUSSE NOTE !"; msg.textContent = `Score final : ${score} notes jouées`;
      btnStart.textContent = "Rejouer ↺"; overlay.classList.remove('hidden');
    }

    // Touch & Click on canvas
    c.addEventListener('touchstart', e => {
      e.preventDefault();
      const rect = c.getBoundingClientRect();
      const tx = (e.touches[0].clientX - rect.left) * (c.width / rect.width);
      tapColumn(Math.floor(tx / colW));
    }, { passive: false });

    c.addEventListener('mousedown', e => {
      const rect = c.getBoundingClientRect();
      const tx = (e.clientX - rect.left) * (c.width / rect.width);
      tapColumn(Math.floor(tx / colW));
    });

    document.getElementById('k-0').onclick = () => tapColumn(0);
    document.getElementById('k-1').onclick = () => tapColumn(1);
    document.getElementById('k-2').onclick = () => tapColumn(2);
    document.getElementById('k-3').onclick = () => tapColumn(3);

    window.onkeydown = e => {
      if(e.key==='1'||e.key==='a') tapColumn(0);
      if(e.key==='2'||e.key==='z') tapColumn(1);
      if(e.key==='3'||e.key==='e') tapColumn(2);
      if(e.key==='4'||e.key==='r') tapColumn(3);
    };

    btnStart.onclick = init;
  </script>
</body>
</html>
)rawliteral";

#endif
