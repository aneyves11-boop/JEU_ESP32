#ifndef GAME_PONG_H
#define GAME_PONG_H

#include <pgmspace.h>

const char GAME_PONG_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Pong vs IA Pro - ESP32 Arcade</title>
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
    .stage { position: relative; border: 2px solid var(--card-border); border-radius: 14px; overflow: hidden; background: #05070f; box-shadow: 0 0 20px rgba(0,243,255,0.2); }
    canvas { display: block; width: 340px; height: 420px; max-width: 92vw; max-height: 62vh; cursor: pointer; }
    .overlay { position: absolute; inset: 0; background: rgba(9, 11, 18, 0.9); display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 14px; text-align: center; padding: 20px; }
    .overlay.hidden { display: none; }
    .overlay h2 { font-size: 1.8rem; color: var(--pink); }
    .btn-play { background: linear-gradient(135deg, var(--cyan), #0077ff); color: #000; font-weight: 800; border: none; padding: 12px 24px; border-radius: 10px; font-size: 1rem; cursor: pointer; }
    .controls { width: 100%; max-width: 340px; margin-top: 8px; display: flex; gap: 12px; }
    .btn-paddle { flex: 1; height: 48px; background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); border-radius: 12px; color: #fff; font-weight: 900; font-size: 1.2rem; }
    .btn-paddle:active { background: rgba(0,243,255,0.3); border-color: var(--cyan); }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-back">◀ Menu</a>
    <div class="score-panel">
      <div class="score-box"><div class="lbl">Toi</div><div class="val" id="player-score">0</div></div>
      <div class="score-box"><div class="lbl">IA</div><div class="val" id="ai-score" style="color:var(--pink);">0</div></div>
    </div>
  </header>

  <div class="stage">
    <canvas id="c" width="340" height="420"></canvas>
    <div id="overlay" class="overlay">
      <h2 id="title">PONG VS IA PRO</h2>
      <p id="msg">Glisse ton doigt ou utilise les boutons pour renvoyer la balle à l'IA !</p>
      <button class="btn-play" id="btn-start">Commencer le Duel 🏓</button>
    </div>
  </div>

  <div class="controls">
    <button class="btn-paddle" id="b-l">◀ GAUCHE</button>
    <button class="btn-paddle" id="b-r">DROITE ▶</button>
  </div>

  <script>
    const c = document.getElementById('c'), ctx = c.getContext('2d');
    const pScoreEl = document.getElementById('player-score'), aiScoreEl = document.getElementById('ai-score');
    const overlay = document.getElementById('overlay'), title = document.getElementById('title'), msg = document.getElementById('msg'), btnStart = document.getElementById('btn-start');

    const PW = 75, PH = 10;
    let px = (c.width - PW)/2, aiX = (c.width - PW)/2;
    let bx = c.width/2, by = c.height/2, bdx = 3.5, bdy = -4;
    let pScore = 0, aiScore = 0, isRunning = false, loopId = null, leftP = false, rightP = false;

    function sound(f, dur=0.06) {
      try {
        const AC = window.AudioContext || window.webkitAudioContext;
        if(!AC) return;
        const ac = new AC(), o = ac.createOscillator(), g = ac.createGain();
        o.type = 'sine'; o.frequency.value = f;
        g.gain.setValueAtTime(0.1, ac.currentTime);
        g.gain.exponentialRampToValueAtTime(0.001, ac.currentTime + dur);
        o.connect(g); g.connect(ac.destination);
        o.start(); o.stop(ac.currentTime + dur);
      } catch(e){}
    }

    function init() {
      px = (c.width - PW)/2; aiX = (c.width - PW)/2;
      pScore = 0; aiScore = 0;
      pScoreEl.textContent = '0'; aiScoreEl.textContent = '0';
      resetBall(); isRunning = true;
      overlay.classList.add('hidden');
      if(loopId) cancelAnimationFrame(loopId);
      loop();
    }

    function resetBall() {
      bx = c.width / 2; by = c.height / 2;
      bdx = (Math.random() > 0.5 ? 1 : -1) * (3 + Math.random()*1.5);
      bdy = (Math.random() > 0.5 ? 1 : -1) * 4;
    }

    function loop() {
      if(!isRunning) return;

      // Player Movement
      if(leftP && px > 0) px -= 6;
      if(rightP && px < c.width - PW) px += 6;

      // AI Movement (Smart tracking with slight human error margin)
      const aiCenter = aiX + PW/2;
      if(aiCenter < bx - 6) aiX += Math.min(4.5, bx - aiCenter);
      else if(aiCenter > bx + 6) aiX -= Math.min(4.5, aiCenter - bx);
      aiX = Math.max(0, Math.min(c.width - PW, aiX));

      // Ball Movement
      bx += bdx; by += bdy;

      // Wall bounce
      if(bx < 8 || bx > c.width - 8) {
        bdx = -bdx; sound(350);
      }

      // Player Paddle Bounce
      const py = c.height - PH - 10;
      if(by + 6 >= py && by - 6 <= py + PH && bx >= px - 4 && bx <= px + PW + 4) {
        const hit = (bx - (px + PW/2)) / (PW/2);
        bdx = hit * 5; bdy = -Math.abs(bdy) * 1.05;
        sound(600);
      }

      // AI Paddle Bounce
      const aiy = 10;
      if(by - 6 <= aiy + PH && by + 6 >= aiy && bx >= aiX - 4 && bx <= aiX + PW + 4) {
        const hit = (bx - (aiX + PW/2)) / (PW/2);
        bdx = hit * 5; bdy = Math.abs(bdy) * 1.05;
        sound(450);
      }

      // Point Scored
      if(by > c.height) {
        aiScore++; aiScoreEl.textContent = aiScore; sound(180, 0.2);
        if(aiScore >= 5) { endMatch(false); return; }
        resetBall();
      } else if(by < 0) {
        pScore++; pScoreEl.textContent = pScore; sound(850, 0.2);
        if(pScore >= 5) { endMatch(true); return; }
        resetBall();
      }

      // --- RENDER ---
      ctx.fillStyle = '#05070f'; ctx.fillRect(0, 0, c.width, c.height);

      // Center court line
      ctx.strokeStyle = 'rgba(255,255,255,0.1)'; ctx.lineWidth = 2; ctx.setLineDash([10, 10]);
      ctx.beginPath(); ctx.moveTo(0, c.height/2); ctx.lineTo(c.width, c.height/2); ctx.stroke();
      ctx.setLineDash([]);

      // Draw AI Paddle
      ctx.fillStyle = '#ff0055'; ctx.fillRect(aiX, 10, PW, PH);

      // Draw Player Paddle
      ctx.fillStyle = '#00f3ff'; ctx.fillRect(px, c.height - PH - 10, PW, PH);

      // Draw Ball
      ctx.fillStyle = '#ffe600'; ctx.shadowColor = '#ffe600'; ctx.shadowBlur = 10;
      ctx.beginPath(); ctx.arc(bx, by, 7, 0, Math.PI*2); ctx.fill();
      ctx.shadowBlur = 0;

      loopId = requestAnimationFrame(loop);
    }

    function endMatch(won) {
      isRunning = false;
      title.textContent = won ? "VICTOIRE ! 🎉" : "DÉFAITE ! 🤖";
      title.style.color = won ? "var(--green)" : "var(--pink)";
      msg.textContent = `Score final : ${pScore} - ${aiScore}`;
      btnStart.textContent = "Rejouer ↺"; overlay.classList.remove('hidden');
      if(won) localStorage.setItem('pong_highscore', (parseInt(localStorage.getItem('pong_highscore')||'0',10)+1));
    }

    // Touch dragging
    c.addEventListener('touchmove', e => {
      if(!isRunning || !e.touches.length) return;
      e.preventDefault();
      const rect = c.getBoundingClientRect();
      const tx = (e.touches[0].clientX - rect.left) * (c.width / rect.width);
      px = Math.max(0, Math.min(c.width - PW, tx - PW/2));
    }, { passive: false });

    // Buttons
    document.getElementById('b-l').ontouchstart = (e)=>{ e.preventDefault(); leftP = true; };
    document.getElementById('b-l').ontouchend = ()=> leftP = false;
    document.getElementById('b-l').onmousedown = ()=> leftP = true;
    document.getElementById('b-l').onmouseup = ()=> leftP = false;

    document.getElementById('b-r').ontouchstart = (e)=>{ e.preventDefault(); rightP = true; };
    document.getElementById('b-r').ontouchend = ()=> rightP = false;
    document.getElementById('b-r').onmousedown = ()=> rightP = true;
    document.getElementById('b-r').onmouseup = ()=> rightP = false;

    btnStart.onclick = init;
  </script>
</body>
</html>
)rawliteral";

#endif
