#ifndef GAME_RACER_H
#define GAME_RACER_H

#include <pgmspace.h>

const char GAME_RACER_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Cyber Racer - ESP32 Arcade</title>
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
    .stage { position: relative; border: 2px solid var(--card-border); border-radius: 14px; overflow: hidden; background: #05070d; box-shadow: 0 0 20px rgba(0,243,255,0.2); }
    canvas { display: block; width: 340px; height: 420px; max-width: 92vw; max-height: 62vh; }
    .overlay { position: absolute; inset: 0; background: rgba(9, 11, 18, 0.9); display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 14px; text-align: center; padding: 20px; }
    .overlay.hidden { display: none; }
    .overlay h2 { font-size: 1.8rem; color: var(--pink); }
    .btn-play { background: linear-gradient(135deg, var(--cyan), #0077ff); color: #000; font-weight: 800; border: none; padding: 12px 24px; border-radius: 10px; font-size: 1rem; cursor: pointer; }
    .controls { width: 100%; max-width: 340px; margin-top: 8px; display: flex; gap: 12px; }
    .btn-steer { flex: 1; height: 50px; background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); border-radius: 12px; color: #fff; font-weight: 900; font-size: 1.2rem; }
    .btn-steer:active { background: rgba(0,243,255,0.3); border-color: var(--cyan); transform: scale(0.96); }
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
      <h2 id="title">CYBER RACER</h2>
      <p id="msg">Change de voie pour doubler les bolides à plus de 200 km/h !</p>
      <button class="btn-play" id="btn-start">Piloter 🏎️</button>
    </div>
  </div>

  <div class="controls">
    <button class="btn-steer" id="b-l">◀ VOIE GAUCHE</button>
    <button class="btn-steer" id="b-r">VOIE DROITE ▶</button>
  </div>

  <script>
    const c = document.getElementById('c'), ctx = c.getContext('2d');
    const scoreEl = document.getElementById('score'), bestEl = document.getElementById('best');
    const overlay = document.getElementById('overlay'), title = document.getElementById('title'), msg = document.getElementById('msg'), btnStart = document.getElementById('btn-start');

    let lane = 1; // 0, 1, 2
    let playerX = 170, targetX = 170;
    let cars = [], lineOffset = 0, score = 0, speed = 4, isRunning = false, loopId = null, timer = 0;
    let bestScore = parseInt(localStorage.getItem('racer_highscore') || '0', 10);
    bestEl.textContent = bestScore;

    const laneWidth = 340 / 3;
    const laneCenters = [laneWidth * 0.5, laneWidth * 1.5, laneWidth * 2.5];

    function sound(f, dur=0.08) {
      try {
        const AC = window.AudioContext || window.webkitAudioContext;
        if(!AC) return;
        const ac = new AC(), o = ac.createOscillator(), g = ac.createGain();
        o.type = 'sawtooth'; o.frequency.value = f;
        g.gain.setValueAtTime(0.08, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + dur);
        o.connect(g); g.connect(ac.destination);
        o.start(); o.stop(ac.currentTime + dur);
      } catch(e){}
    }

    function init() {
      lane = 1; targetX = laneCenters[1]; playerX = targetX;
      cars = []; score = 0; speed = 4.5; timer = 0;
      scoreEl.textContent = '0'; isRunning = true;
      overlay.classList.add('hidden');
      if(loopId) cancelAnimationFrame(loopId);
      loop();
    }

    function moveLeft() {
      if(lane > 0) { lane--; targetX = laneCenters[lane]; sound(280); }
    }
    function moveRight() {
      if(lane < 2) { lane++; targetX = laneCenters[lane]; sound(340); }
    }

    function loop() {
      if(!isRunning) return;

      // Smooth steering
      playerX += (targetX - playerX) * 0.25;

      // Road scroll
      lineOffset = (lineOffset + speed * 1.5) % 40;

      // Spawn traffic cars
      timer++;
      if(timer % Math.max(25, 55 - Math.floor(score/30)) === 0) {
        const clane = Math.floor(Math.random() * 3);
        const colors = ['#ff0055', '#ffe600', '#9d4edd', '#00ff66'];
        cars.push({
          lane: clane,
          x: laneCenters[clane],
          y: -60,
          color: colors[Math.floor(Math.random() * colors.length)],
          speed: speed * 0.4
        });
      }

      // Update traffic
      for(let i=cars.length-1; i>=0; i--) {
        const car = cars[i];
        car.y += (speed - car.speed) + 2;

        // Collision detection
        if(Math.abs(car.x - playerX) < 32 && Math.abs(car.y - (c.height - 60)) < 48) {
          gameOver(); return;
        }

        // Passed car
        if(car.y > c.height + 60) {
          cars.splice(i, 1);
          score += 10; scoreEl.textContent = score;
          speed = Math.min(10, 4.5 + (score / 100));
          if(score > bestScore) {
            bestScore = score; bestEl.textContent = bestScore;
            localStorage.setItem('racer_highscore', bestScore);
          }
        }
      }

      // --- RENDER ---
      // Road surface
      ctx.fillStyle = '#080a14'; ctx.fillRect(0, 0, c.width, c.height);

      // Road shoulders
      ctx.fillStyle = '#ff0055';
      ctx.fillRect(0, 0, 8, c.height);
      ctx.fillRect(c.width - 8, 0, 8, c.height);

      // Lane dividers
      ctx.strokeStyle = 'rgba(0,243,255,0.4)';
      ctx.lineWidth = 3;
      ctx.setLineDash([20, 20]);
      ctx.lineDashOffset = -lineOffset;

      ctx.beginPath(); ctx.moveTo(laneWidth, 0); ctx.lineTo(laneWidth, c.height); ctx.stroke();
      ctx.beginPath(); ctx.moveTo(laneWidth*2, 0); ctx.lineTo(laneWidth*2, c.height); ctx.stroke();
      ctx.setLineDash([]);

      // Draw Enemy Cars
      cars.forEach(car => {
        ctx.fillStyle = car.color;
        ctx.fillRect(car.x - 14, car.y - 24, 28, 48);
        // Roof & windshield
        ctx.fillStyle = '#090b12';
        ctx.fillRect(car.x - 10, car.y - 12, 20, 16);
        // Tail lights
        ctx.fillStyle = '#ff0000';
        ctx.fillRect(car.x - 12, car.y + 20, 6, 4);
        ctx.fillRect(car.x + 6, car.y + 20, 6, 4);
      });

      // Draw Player Car (Neon Cyber Formula)
      const py = c.height - 60;
      ctx.fillStyle = '#00f3ff';
      ctx.shadowColor = '#00f3ff'; ctx.shadowBlur = 10;
      ctx.fillRect(playerX - 15, py - 26, 30, 52);
      ctx.shadowBlur = 0;

      // Cockpit & Headlights
      ctx.fillStyle = '#090b12';
      ctx.fillRect(playerX - 10, py - 14, 20, 18);
      ctx.fillStyle = '#ffe600';
      ctx.fillRect(playerX - 12, py - 26, 6, 4);
      ctx.fillRect(playerX + 6, py - 26, 6, 4);

      // Speed gauge
      ctx.fillStyle = '#ffe600'; ctx.font = 'bold 13px sans-serif'; ctx.textAlign = 'left';
      ctx.fillText(`${Math.floor(speed * 30)} KM/H`, 16, 25);

      loopId = requestAnimationFrame(loop);
    }

    function gameOver() {
      isRunning = false; sound(120, 0.4);
      title.textContent = "ACCIDENT !"; msg.textContent = `Score : ${score} • Vitesse max : ${Math.floor(speed*30)} km/h`;
      btnStart.textContent = "Rejouer ↺"; overlay.classList.remove('hidden');
    }

    window.onkeydown = e => {
      if(e.key==='ArrowLeft'||e.key==='q') moveLeft();
      if(e.key==='ArrowRight'||e.key==='d') moveRight();
    };

    document.getElementById('b-l').onclick = moveLeft;
    document.getElementById('b-r').onclick = moveRight;
    btnStart.onclick = init;
  </script>
</body>
</html>
)rawliteral";

#endif
