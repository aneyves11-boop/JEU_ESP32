#ifndef GAME_KNIFE_H
#define GAME_KNIFE_H

#include <pgmspace.h>

const char GAME_KNIFE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Knife Master - ESP32 Arcade</title>
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
    .btn-throw { width: 100%; max-width: 340px; height: 50px; margin-top: 8px; background: linear-gradient(135deg, #ff0055, #ff7700); border: none; border-radius: 12px; color: #fff; font-weight: 900; font-size: 1.1rem; cursor: pointer; }
    .btn-throw:active { transform: scale(0.97); }
  </style>
</head>
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
      <h2 id="title">KNIFE MASTER</h2>
      <p id="msg">Plante tous tes couteaux dans la cible tournante sans toucher les autres lames !</p>
      <button class="btn-play" id="btn-start">Lancer les Couteaux 🔪</button>
    </div>
  </div>

  <button class="btn-throw" id="btn-throw">LANCER LE COUTEAU 🗡️</button>

  <script>
    const c = document.getElementById('c'), ctx = c.getContext('2d');
    const scoreEl = document.getElementById('score'), bestEl = document.getElementById('best');
    const overlay = document.getElementById('overlay'), title = document.getElementById('title'), msg = document.getElementById('msg'), btnStart = document.getElementById('btn-start');
    const btnThrow = document.getElementById('btn-throw');

    const R = 65, CX = 170, CY = 140;
    let angle = 0, rotSpeed = 0.035, knifes = [], flyingKnife = null;
    let score = 0, knifesToPlant = 7, isRunning = false, loopId = null;
    let bestScore = parseInt(localStorage.getItem('knife_highscore') || '0', 10);
    bestEl.textContent = bestScore;

    function sound(f, dur=0.08) {
      try {
        const AC = window.AudioContext || window.webkitAudioContext;
        if(!AC) return;
        const ac = new AC(), o = ac.createOscillator(), g = ac.createGain();
        o.type = 'triangle'; o.frequency.value = f;
        g.gain.setValueAtTime(0.12, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + dur);
        o.connect(g); g.connect(ac.destination);
        o.start(); o.stop(ac.currentTime + dur);
      } catch(e){}
    }

    function init() {
      angle = 0; rotSpeed = 0.035; knifes = []; flyingKnife = null;
      score = 0; knifesToPlant = 7; scoreEl.textContent = '0';
      isRunning = true; overlay.classList.add('hidden');
      if(loopId) cancelAnimationFrame(loopId);
      loop();
    }

    function throwKnife() {
      if(!isRunning || flyingKnife) return;
      flyingKnife = { y: c.height - 60 };
      sound(520, 0.06);
    }

    function loop() {
      if(!isRunning) return;

      angle += rotSpeed;

      // Update Flying Knife
      if(flyingKnife) {
        flyingKnife.y -= 18;

        if(flyingKnife.y <= CY + R) {
          // Impact angle calculation
          const hitAngle = (-angle + Math.PI/2) % (Math.PI*2);
          const normA = (hitAngle + Math.PI*2) % (Math.PI*2);

          // Collision detection with embedded knives
          const hitOther = knifes.some(k => {
            const normK = (k + Math.PI*2) % (Math.PI*2);
            let diff = Math.abs(normK - normA);
            if(diff > Math.PI) diff = Math.PI*2 - diff;
            return diff < 0.28; // ~16 degrees clearance required
          });

          if(hitOther) {
            gameOver(); return;
          }

          // Successful hit!
          knifes.push(normA);
          flyingKnife = null;
          score++; scoreEl.textContent = score;
          sound(780, 0.1);

          if(score > bestScore) {
            bestScore = score; bestEl.textContent = bestScore;
            localStorage.setItem('knife_highscore', bestScore);
          }

          knifesToPlant--;
          if(knifesToPlant <= 0) {
            // Stage Cleared!
            sound(980, 0.25);
            knifes = [];
            knifesToPlant = 7 + Math.min(6, Math.floor(score/10));
            rotSpeed = (Math.random() > 0.5 ? 1 : -1) * (0.04 + Math.min(0.04, score * 0.003));
          }
        }
      }

      // --- RENDER ---
      ctx.fillStyle = '#060810'; ctx.fillRect(0, 0, c.width, c.height);

      // Draw Rotating Target Wheel
      ctx.save();
      ctx.translate(CX, CY);
      ctx.rotate(angle);

      // Wood log
      ctx.fillStyle = '#2d1e12';
      ctx.strokeStyle = '#00f3ff'; ctx.lineWidth = 4;
      ctx.beginPath(); ctx.arc(0, 0, R, 0, Math.PI*2); ctx.fill(); ctx.stroke();

      // Target core
      ctx.fillStyle = '#ff0055';
      ctx.beginPath(); ctx.arc(0, 0, 16, 0, Math.PI*2); ctx.fill();

      // Embedded Knives
      knifes.forEach(a => {
        ctx.save();
        ctx.rotate(a);
        ctx.fillStyle = '#00f3ff';
        ctx.fillRect(-3, R - 8, 6, 38);
        ctx.fillStyle = '#ffe600';
        ctx.fillRect(-6, R + 26, 12, 6);
        ctx.restore();
      });

      ctx.restore();

      // Draw Flying Knife or Ready Knife
      const ky = flyingKnife ? flyingKnife.y : (c.height - 60);
      ctx.fillStyle = '#ff0055';
      ctx.fillRect(CX - 3, ky, 6, 40);
      ctx.fillStyle = '#ffe600';
      ctx.fillRect(CX - 6, ky + 28, 12, 6);

      // Knives remaining icons on left
      for(let i=0; i<knifesToPlant; i++) {
        ctx.fillStyle = '#ff0055';
        ctx.fillRect(16, c.height - 30 - i*14, 4, 10);
      }

      loopId = requestAnimationFrame(loop);
    }

    function gameOver() {
      isRunning = false; sound(130, 0.4);
      title.textContent = "LAME BRISÉE !"; msg.textContent = `Score final : ${score} couteaux plantés`;
      btnStart.textContent = "Rejouer ↺"; overlay.classList.remove('hidden');
    }

    c.onclick = throwKnife;
    btnThrow.onclick = throwKnife;
    window.onkeydown = e => { if(e.code==='Space'||e.key===' ') { throwKnife(); e.preventDefault(); } };
    btnStart.onclick = init;
  </script>
</body>
</html>
)rawliteral";

#endif
