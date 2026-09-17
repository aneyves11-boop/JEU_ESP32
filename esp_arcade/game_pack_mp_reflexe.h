#ifndef GAME_PACK_MP_REFLEXE_H
#define GAME_PACK_MP_REFLEXE_H

#include <pgmspace.h>

// ==================================================================================
//   ESP32 ARCADE - 5 JEUX MULTIJOUEURS DE REFLEXE & ACTION (1V1 LOCAL WI-FI)
//   1. PONG LASER  |  2. AIR HOCKEY ARCADE  |  3. TANK BATTLE 2D
//   4. DUEL DE REFLEXES  |  5. TAP DUEL (TIR A LA CORDE)
// ==================================================================================

// ----------------------------------------------------------------------------------
//   1. PONG 1V1 LASER REALISTE (SCREEN SHAKE, COMET TRAIL, 3D PADDLES & SPARKS)
// ----------------------------------------------------------------------------------
const char MP_PONG_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Pong 1v1 Laser Pro</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: #050711; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      min-height: 100vh; overflow: hidden; touch-action: none;
    }
    header {
      width: 100%; max-width: 400px; padding: 10px 14px;
      display: flex; justify-content: space-between; align-items: center;
    }
    .btn-hub {
      background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2);
      color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700;
    }
    .status-badge {
      font-size: 0.8rem; font-weight: 800; padding: 4px 12px; border-radius: 20px;
      background: rgba(255,170,0,0.15); color: #ffaa00; border: 1px solid #ffaa00;
    }
    .score-board {
      display: flex; gap: 20px; font-size: 1.25rem; font-weight: 900; margin-bottom: 6px;
      text-shadow: 0 0 10px rgba(0,0,0,0.8);
    }
    .p1-color { color: #00f3ff; text-shadow: 0 0 10px rgba(0,243,255,0.6); }
    .p2-color { color: #ff0055; text-shadow: 0 0 10px rgba(255,0,85,0.6); }
    #canvas-wrap {
      position: relative; width: 340px; height: 500px;
      border: 2px solid rgba(0, 243, 255, 0.4); border-radius: 16px;
      box-shadow: 0 0 35px rgba(0, 243, 255, 0.25), inset 0 0 25px rgba(0,0,0,0.8);
      background: #080b16; overflow: hidden;
    }
    canvas { width: 100%; height: 100%; display: block; }
    #overlay {
      position: absolute; inset: 0; background: rgba(6,8,16,0.92);
      backdrop-filter: blur(8px); -webkit-backdrop-filter: blur(8px);
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      border-radius: 14px; text-align: center; padding: 20px;
    }
    #overlay h2 { font-size: 1.5rem; margin-bottom: 8px; font-weight: 900; }
    #overlay p { font-size: 0.9rem; color: #8b9bb4; line-height: 1.4; margin-bottom: 14px; }
    .act-btn {
      padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem;
      cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%;
      transition: all 0.15s;
    }
    .btn-rematch {
      background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000;
      box-shadow: 0 4px 15px rgba(0,243,255,0.4);
    }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit {
      background: rgba(255,255,255,0.08); color: #8b9bb4;
      border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem;
    }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="status-badge" id="badge">Connexion...</span>
  </header>
  <div class="score-board">
    <span class="p1-color"><span id="p1-name">J1</span> : <span id="s1">0</span></span>
    <span style="color:#555;">-</span>
    <span class="p2-color"><span id="p2-name">J2</span> : <span id="s2">0</span></span>
  </div>
  <div id="canvas-wrap">
    <canvas id="c" width="340" height="500"></canvas>
    <div id="overlay">
      <h2 id="ov-title">🏓 Pong 1v1 Laser</h2>
      <p id="ov-desc">En attente d'un adversaire sur son smartphone...</p>
      <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:240px; margin-top:10px;">
        <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
        <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
      </div>
    </div>
  </div>

  <script>
    const canvas = document.getElementById('c'), ctx = canvas.getContext('2d');
    const badge = document.getElementById('badge'), overlay = document.getElementById('overlay');
    const ovTitle = document.getElementById('ov-title'), ovDesc = document.getElementById('ov-desc');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');
    const s1El = document.getElementById('s1'), s2El = document.getElementById('s2');
    const p1Name = document.getElementById('p1-name'), p2Name = document.getElementById('p2-name');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false;
    let myX = 170, oppX = 170;
    let ball = { x: 170, y: 250, vx: 3.5, vy: 4, r: 8 };
    let score1 = 0, score2 = 0;
    const P_WIDTH = 74, P_HEIGHT = 14;
    let myRematch = false, oppRematch = false;
    let trail = [], sparks = [], screenShake = 0;

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => {
      badge.textContent = "Recherche...";
      ws.send(JSON.stringify({ t: "join", g: "pong", name: myPseudo }));
    };

    ws.onmessage = (e) => {
      try {
        const d = JSON.parse(e.data);
        if (d.t === "joined") {
          role = d.role || d.p;
          badge.textContent = role === 1 ? "🔵 Joueur 1 (Hôte)" : "🔴 Joueur 2";
          ovTitle.textContent = role === 1 ? "🔵 Prêt en Joueur 1" : "🔴 Connecté en Joueur 2";
          ovDesc.textContent = "En attente du second joueur pour débuter...";
        } else if (d.t === "start") {
          overlay.style.display = "none";
          ovActions.style.display = "none";
          playing = true;
          role = d.role || role || 1;
          badge.textContent = "⚡ En Direct";
          badge.style.color = "#00ff66"; badge.style.borderColor = "#00ff66";
          if (d.opp) {
            if (role === 1) { p1Name.textContent = myPseudo; p2Name.textContent = d.opp; }
            else { p1Name.textContent = d.opp; p2Name.textContent = myPseudo; }
          }
        } else if (d.t === "pos") {
          oppX = 340 - d.x;
        } else if (d.t === "ball" && role === 2) {
          ball.x = 340 - d.x; ball.y = 500 - d.y;
          score1 = d.s1; score2 = d.s2; updateScores();
        } else if (d.t === "score") {
          score1 = d.s1; score2 = d.s2; updateScores();
        } else if (d.t === "rematch") {
          oppRematch = true;
          if (myRematch) {
            restartPong();
          } else {
            btnRematch.textContent = "⚡ L'adversaire veut rejouer ! [ACCEPTER]";
            btnRematch.style.background = "linear-gradient(180deg, #00ff66, #00cc44)";
            btnRematch.style.color = "#000";
          }
        } else if (d.t === "left" || d.t === "opp_left") {
          playing = false; overlay.style.display = "flex"; ovActions.style.display = "none";
          ovTitle.textContent = "Adversaire déconnecté";
          ovDesc.textContent = "L'autre joueur a quitté la partie.";
          badge.textContent = "Interrompu";
        }
      } catch(err){}
    };

    function updateScores() {
      s1El.textContent = score1; s2El.textContent = score2;
      if (score1 >= 5 || score2 >= 5) {
        playing = false;
        overlay.style.display = "flex";
        ovActions.style.display = "flex";
        const won = (role === 1 && score1 >= 5) || (role === 2 && score2 >= 5);
        ovTitle.textContent = won ? "🏆 VICTOIRE !" : "💀 DÉFAITE !";
        ovDesc.textContent = `Score final : ${score1} - ${score2}`;
        btnRematch.disabled = false;
        btnRematch.textContent = "🔄 Rejouer (Revanche)";
        btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
        btnRematch.style.color = "#000";
      }
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.disabled = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) restartPong();
    };

    function restartPong() {
      myRematch = false; oppRematch = false;
      score1 = 0; score2 = 0;
      s1El.textContent = 0; s2El.textContent = 0;
      overlay.style.display = "none";
      ovActions.style.display = "none";
      btnRematch.disabled = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
      btnRematch.style.color = "#000";
      playing = true;
      if (role === 1) resetBall(1);
    }

    function handleTouch(e) {
      const rect = canvas.getBoundingClientRect();
      const touch = e.touches ? e.touches[0] : e;
      const x = (touch.clientX - rect.left) * (canvas.width / rect.width);
      myX = Math.max(P_WIDTH/2, Math.min(canvas.width - P_WIDTH/2, x));
      if (ws.readyState === WebSocket.OPEN && playing) {
        ws.send(JSON.stringify({ t: "pos", x: myX }));
      }
    }
    window.addEventListener('touchmove', (e) => { if(e.target === canvas) e.preventDefault(); handleTouch(e); }, { passive: false });
    window.addEventListener('mousemove', (e) => { if (playing) handleTouch(e); });

    function resetBall(servingToRole) {
      ball.x = 170; ball.y = 250;
      ball.vx = (Math.random() > 0.5 ? 3.5 : -3.5);
      ball.vy = servingToRole === 1 ? -4 : 4;
      trail = [];
    }

    function spawnSparks(x, y, color) {
      for (let i = 0; i < 8; i++) {
        const ang = Math.random() * Math.PI * 2;
        const spd = 1.5 + Math.random() * 3.5;
        sparks.push({ x: x, y: y, vx: Math.cos(ang) * spd, vy: Math.sin(ang) * spd, life: 1, color: color });
      }
    }

    function drawPaddle(x, y, isMain) {
      ctx.save();
      const w = P_WIDTH, h = P_HEIGHT;
      // Glow & Shadow
      ctx.shadowColor = isMain ? "#00f3ff" : "#ff0055";
      ctx.shadowBlur = 14;
      // Gradient body
      const g = ctx.createLinearGradient(x - w/2, y, x + w/2, y + h);
      g.addColorStop(0, isMain ? "#00f3ff" : "#ff0055");
      g.addColorStop(0.5, "#ffffff");
      g.addColorStop(1, isMain ? "#0066cc" : "#990033");
      ctx.fillStyle = g;
      ctx.beginPath();
      ctx.roundRect(x - w/2, y, w, h, 6);
      ctx.fill();
      // Metallic reflection line
      ctx.fillStyle = "rgba(255,255,255,0.7)";
      ctx.fillRect(x - w/2 + 6, y + 2, w - 12, 2);
      ctx.restore();
    }

    function loop() {
      ctx.clearRect(0, 0, canvas.width, canvas.height);

      // Screen shake transform
      ctx.save();
      if (screenShake > 0) {
        ctx.translate((Math.random() - 0.5) * screenShake, (Math.random() - 0.5) * screenShake);
        screenShake *= 0.85;
        if (screenShake < 0.2) screenShake = 0;
      }

      // Arena background & center line
      ctx.strokeStyle = "rgba(0, 243, 255, 0.15)";
      ctx.setLineDash([8, 8]);
      ctx.beginPath(); ctx.moveTo(0, 250); ctx.lineTo(340, 250); ctx.stroke();
      ctx.setLineDash([]);

      if (playing && role === 1) {
        ball.x += ball.vx; ball.y += ball.vy;
        trail.push({ x: ball.x, y: ball.y });
        if (trail.length > 8) trail.shift();

        // Wall collisions
        if (ball.x - ball.r <= 0) { ball.x = ball.r; ball.vx = Math.abs(ball.vx); spawnSparks(ball.x, ball.y, "#ffe600"); }
        if (ball.x + ball.r >= canvas.width) { ball.x = canvas.width - ball.r; ball.vx = -Math.abs(ball.vx); spawnSparks(ball.x, ball.y, "#ffe600"); }

        // Collision P1 (bas)
        if (ball.y + ball.r >= canvas.height - 24 && ball.y - ball.r <= canvas.height - 12) {
          if (ball.x >= myX - P_WIDTH/2 && ball.x <= myX + P_WIDTH/2) {
            ball.vy = -Math.abs(ball.vy) * 1.05;
            ball.vx = (ball.x - myX) * 0.16;
            screenShake = 4;
            spawnSparks(ball.x, ball.y + ball.r, "#00f3ff");
          }
        }
        // Collision P2 (haut)
        if (ball.y - ball.r <= 24 && ball.y + ball.r >= 12) {
          if (ball.x >= oppX - P_WIDTH/2 && ball.x <= oppX + P_WIDTH/2) {
            ball.vy = Math.abs(ball.vy) * 1.05;
            ball.vx = (ball.x - oppX) * 0.16;
            screenShake = 4;
            spawnSparks(ball.x, ball.y - ball.r, "#ff0055");
          }
        }
        if (ball.y > canvas.height) {
          score2++;
          ws.send(JSON.stringify({ t: "score", s1: score1, s2: score2 }));
          updateScores(); resetBall(1);
        }
        if (ball.y < 0) {
          score1++;
          ws.send(JSON.stringify({ t: "score", s1: score1, s2: score2 }));
          updateScores(); resetBall(2);
        }
        if (ws.readyState === WebSocket.OPEN) {
          ws.send(JSON.stringify({ t: "ball", x: ball.x, y: ball.y, s1: score1, s2: score2 }));
        }
      }

      // Render Comet Trail
      trail.forEach((t, i) => {
        const ratio = (i + 1) / trail.length;
        ctx.fillStyle = `rgba(255, 230, 0, ${ratio * 0.35})`;
        ctx.beginPath();
        ctx.arc(t.x, t.y, ball.r * ratio, 0, Math.PI * 2);
        ctx.fill();
      });

      // Render Sparks
      for (let i = sparks.length - 1; i >= 0; i--) {
        const sp = sparks[i];
        sp.x += sp.vx; sp.y += sp.vy; sp.life -= 0.05;
        if (sp.life <= 0) { sparks.splice(i, 1); continue; }
        ctx.fillStyle = sp.color;
        ctx.globalAlpha = sp.life;
        ctx.fillRect(sp.x, sp.y, 3, 3);
        ctx.globalAlpha = 1;
      }

      // Draw Paddles
      drawPaddle(myX, canvas.height - 22, role === 1);
      drawPaddle(oppX, 10, role !== 1);

      // Draw Glowing Energy Ball
      ctx.save();
      const ballGrad = ctx.createRadialGradient(ball.x - 2, ball.y - 2, 1, ball.x, ball.y, ball.r);
      ballGrad.addColorStop(0, "#ffffff");
      ballGrad.addColorStop(0.4, "#ffe600");
      ballGrad.addColorStop(1, "#ff5500");
      ctx.fillStyle = ballGrad;
      ctx.shadowColor = "#ffe600";
      ctx.shadowBlur = 15;
      ctx.beginPath();
      ctx.arc(ball.x, ball.y, ball.r, 0, Math.PI * 2);
      ctx.fill();
      ctx.restore();

      ctx.restore(); // restore screen shake
      requestAnimationFrame(loop);
    }
    loop();
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   2. AIR HOCKEY ARCADE NEON (MALLETS 3D, PUCKS GLOW & REBONDS ELASTIQUES)
// ----------------------------------------------------------------------------------
const char MP_HOCKEY_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Air Hockey 1v1 Pro</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: #04060d; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      min-height: 100vh; overflow: hidden; touch-action: none;
    }
    header { width: 100%; max-width: 380px; padding: 10px 14px; display: flex; justify-content: space-between; align-items: center; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .status-badge { font-size: 0.8rem; font-weight: 800; padding: 4px 12px; border-radius: 20px; background: rgba(255,170,0,0.15); color: #ffaa00; border: 1px solid #ffaa00; }
    .score-board { display: flex; gap: 24px; font-size: 1.4rem; font-weight: 900; margin-bottom: 6px; }
    #canvas-wrap {
      position: relative; width: 340px; height: 500px;
      border: 3px solid #00f3ff; border-radius: 20px; background: #080f1d;
      box-shadow: 0 0 35px rgba(0,243,255,0.25), inset 0 0 20px rgba(0,0,0,0.8);
      overflow: hidden;
    }
    canvas { width: 100%; height: 100%; display: block; }
    #overlay { position: absolute; inset: 0; background: rgba(6,9,16,0.92); backdrop-filter: blur(8px); display: flex; flex-direction: column; align-items: center; justify-content: center; border-radius: 18px; text-align: center; padding: 20px; }
    #overlay h2 { font-size: 1.5rem; margin-bottom: 8px; font-weight: 900; }
    #overlay p { font-size: 0.9rem; color: #8b9bb4; line-height: 1.4; margin-bottom: 12px; }
    .act-btn {
      padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem;
      cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%;
      transition: all 0.15s;
    }
    .btn-rematch {
      background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000;
      box-shadow: 0 4px 15px rgba(0,243,255,0.4);
    }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit {
      background: rgba(255,255,255,0.08); color: #8b9bb4;
      border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem;
    }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="status-badge" id="badge">Connexion...</span>
  </header>
  <div class="score-board">
    <span style="color:#00f3ff;">J1 : <span id="s1">0</span></span>
    <span style="color:#555;">-</span>
    <span style="color:#ff0055;">J2 : <span id="s2">0</span></span>
  </div>
  <div id="canvas-wrap">
    <canvas id="c" width="340" height="500"></canvas>
    <div id="overlay">
      <h2 id="ov-title">🏒 Air Hockey 1v1</h2>
      <p id="ov-desc">En attente d'un adversaire...</p>
      <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:240px; margin-top:10px;">
        <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
        <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
      </div>
    </div>
  </div>
  <script>
    const canvas = document.getElementById('c'), ctx = canvas.getContext('2d');
    const badge = document.getElementById('badge'), overlay = document.getElementById('overlay');
    const ovTitle = document.getElementById('ov-title'), ovDesc = document.getElementById('ov-desc');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');
    const s1El = document.getElementById('s1'), s2El = document.getElementById('s2');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false;
    let s1 = 0, s2 = 0;
    let myPos = { x: 170, y: 420 }, oppPos = { x: 170, y: 80 };
    let puck = { x: 170, y: 250, vx: 0, vy: 0, r: 13 };
    const MALLET_R = 25;
    let myRematch = false, oppRematch = false;
    let sparks = [];

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "hockey", name: myPseudo }));
    ws.onmessage = (e) => {
      const d = JSON.parse(e.data);
      if (d.t === "joined") {
        role = d.role || d.p;
        badge.textContent = role === 1 ? "🔵 Joueur 1 (Hôte)" : "🔴 Joueur 2";
        ovDesc.textContent = "En attente du 2ème joueur...";
      } else if (d.t === "start") {
        overlay.style.display = "none";
        ovActions.style.display = "none";
        playing = true;
        role = d.role || role || 1;
        badge.textContent = "⚡ En Direct";
      } else if (d.t === "pos") {
        oppPos = { x: 340 - d.x, y: 500 - d.y };
      } else if (d.t === "puck" && role === 2) {
        puck.x = 340 - d.x; puck.y = 500 - d.y;
        s1 = d.s1; s2 = d.s2;
        checkMatchEnd();
      } else if (d.t === "rematch") {
        oppRematch = true;
        if (myRematch) {
          restartHockey();
        } else {
          btnRematch.textContent = "⚡ L'adversaire veut rejouer ! [ACCEPTER]";
          btnRematch.style.background = "linear-gradient(180deg, #00ff66, #00cc44)";
          btnRematch.style.color = "#000";
        }
      } else if (d.t === "left" || d.t === "opp_left") {
        playing = false; overlay.style.display = "flex"; ovActions.style.display = "none";
        ovTitle.textContent = "Adversaire déconnecté";
        ovDesc.textContent = "L'autre joueur a quitté la partie.";
      }
    };

    function checkMatchEnd() {
      s1El.textContent = s1; s2El.textContent = s2;
      if (s1 >= 5 || s2 >= 5) {
        playing = false;
        overlay.style.display = "flex";
        ovActions.style.display = "flex";
        const won = (role === 1 && s1 >= 5) || (role === 2 && s2 >= 5);
        ovTitle.textContent = won ? "🏆 VICTOIRE !" : "💀 DÉFAITE !";
        ovDesc.textContent = `Score final : ${s1} - ${s2}`;
        btnRematch.disabled = false;
        btnRematch.textContent = "🔄 Rejouer (Revanche)";
        btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
        btnRematch.style.color = "#000";
      }
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.disabled = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) restartHockey();
    };

    function restartHockey() {
      myRematch = false; oppRematch = false;
      s1 = 0; s2 = 0; s1El.textContent = 0; s2El.textContent = 0;
      overlay.style.display = "none";
      ovActions.style.display = "none";
      btnRematch.disabled = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
      btnRematch.style.color = "#000";
      resetPuck();
      playing = true;
    }

    function spawnHitSparks(x, y) {
      for (let i = 0; i < 7; i++) {
        const a = Math.random() * Math.PI * 2, sp = 2 + Math.random() * 3;
        sparks.push({ x: x, y: y, vx: Math.cos(a) * sp, vy: Math.sin(a) * sp, life: 1 });
      }
    }

    function handleTouch(e) {
      const rect = canvas.getBoundingClientRect();
      const t = e.touches ? e.touches[0] : e;
      const x = (t.clientX - rect.left) * (canvas.width / rect.width);
      const y = (t.clientY - rect.top) * (canvas.height / rect.height);
      myPos.x = Math.max(MALLET_R, Math.min(canvas.width - MALLET_R, x));
      myPos.y = Math.max(260, Math.min(canvas.height - MALLET_R, y));
      if (ws.readyState === WebSocket.OPEN && playing) {
        ws.send(JSON.stringify({ t: "pos", x: myPos.x, y: myPos.y }));
      }
    }
    window.addEventListener('touchmove', (e) => { if(e.target===canvas) e.preventDefault(); handleTouch(e); }, {passive:false});
    window.addEventListener('mousemove', (e) => { if(playing) handleTouch(e); });

    function drawMallet(x, y, isP1) {
      ctx.save();
      // Drop Shadow
      ctx.fillStyle = "rgba(0,0,0,0.45)";
      ctx.beginPath(); ctx.arc(x + 3, y + 4, MALLET_R, 0, Math.PI*2); ctx.fill();
      // Base rim
      const baseGrad = ctx.createRadialGradient(x - 5, y - 5, 5, x, y, MALLET_R);
      baseGrad.addColorStop(0, isP1 ? "#00f3ff" : "#ff0055");
      baseGrad.addColorStop(1, isP1 ? "#004b66" : "#660022");
      ctx.fillStyle = baseGrad;
      ctx.beginPath(); ctx.arc(x, y, MALLET_R, 0, Math.PI*2); ctx.fill();
      // Inner metallic knob/dome
      const knobGrad = ctx.createRadialGradient(x - 3, y - 3, 1, x, y, 13);
      knobGrad.addColorStop(0, "#ffffff");
      knobGrad.addColorStop(0.3, isP1 ? "#00f3ff" : "#ff0055");
      knobGrad.addColorStop(1, "#111827");
      ctx.fillStyle = knobGrad;
      ctx.beginPath(); ctx.arc(x, y, 13, 0, Math.PI*2); ctx.fill();
      ctx.restore();
    }

    function drawPuck(x, y) {
      ctx.save();
      ctx.fillStyle = "rgba(0,0,0,0.5)";
      ctx.beginPath(); ctx.arc(x + 2, y + 3, puck.r, 0, Math.PI*2); ctx.fill();
      const pGrad = ctx.createRadialGradient(x - 3, y - 3, 2, x, y, puck.r);
      pGrad.addColorStop(0, "#ffffff");
      pGrad.addColorStop(0.4, "#ffe600");
      pGrad.addColorStop(1, "#ff7700");
      ctx.fillStyle = pGrad;
      ctx.shadowColor = "#ffe600"; ctx.shadowBlur = 14;
      ctx.beginPath(); ctx.arc(x, y, puck.r, 0, Math.PI*2); ctx.fill();
      ctx.strokeStyle = "rgba(0,0,0,0.35)"; ctx.lineWidth = 1.5;
      ctx.beginPath(); ctx.arc(x, y, puck.r * 0.55, 0, Math.PI*2); ctx.stroke();
      ctx.restore();
    }

    function loop() {
      ctx.clearRect(0, 0, canvas.width, canvas.height);

      // Realistic ice rink background
      const iceGrad = ctx.createLinearGradient(0, 0, 340, 500);
      iceGrad.addColorStop(0, "#081020"); iceGrad.addColorStop(0.5, "#0e1b36"); iceGrad.addColorStop(1, "#081020");
      ctx.fillStyle = iceGrad; ctx.fillRect(0, 0, 340, 500);

      // Ice air-holes pattern
      ctx.fillStyle = "rgba(0, 243, 255, 0.04)";
      for (let gx = 15; gx < 340; gx += 22) {
        for (let gy = 15; gy < 500; gy += 22) {
          ctx.fillRect(gx, gy, 2, 2);
        }
      }

      // Rink lines & center circle
      ctx.strokeStyle = "rgba(0, 243, 255, 0.4)"; ctx.lineWidth = 2;
      ctx.beginPath(); ctx.arc(170, 250, 48, 0, Math.PI*2); ctx.stroke();
      ctx.strokeStyle = "rgba(255, 0, 85, 0.4)";
      ctx.beginPath(); ctx.moveTo(0, 250); ctx.lineTo(340, 250); ctx.stroke();

      // Goal zones
      ctx.fillStyle = "rgba(255,0,85,0.3)";
      ctx.fillRect(110, 0, 120, 8); ctx.fillRect(110, 492, 120, 8);

      if (playing && role === 1) {
        puck.x += puck.vx; puck.y += puck.vy;
        puck.vx *= 0.985; puck.vy *= 0.985;
        if (puck.x - puck.r <= 0) { puck.x = puck.r; puck.vx = -puck.vx; spawnHitSparks(puck.x, puck.y); }
        if (puck.x + puck.r >= canvas.width) { puck.x = canvas.width - puck.r; puck.vx = -puck.vx; spawnHitSparks(puck.x, puck.y); }

        let dx = puck.x - myPos.x, dy = puck.y - myPos.y, dist = Math.hypot(dx, dy);
        if (dist < puck.r + MALLET_R) {
          let ang = Math.atan2(dy, dx);
          puck.vx = Math.cos(ang) * 9.5; puck.vy = Math.sin(ang) * 9.5;
          spawnHitSparks(puck.x, puck.y);
        }
        dx = puck.x - oppPos.x; dy = puck.y - oppPos.y; dist = Math.hypot(dx, dy);
        if (dist < puck.r + MALLET_R) {
          let ang = Math.atan2(dy, dx);
          puck.vx = Math.cos(ang) * 9.5; puck.vy = Math.sin(ang) * 9.5;
          spawnHitSparks(puck.x, puck.y);
        }
        if (puck.y < 5 && puck.x > 110 && puck.x < 230) { s1++; checkMatchEnd(); resetPuck(); }
        if (puck.y > 495 && puck.x > 110 && puck.x < 230) { s2++; checkMatchEnd(); resetPuck(); }
        if (puck.y - puck.r <= 0 || puck.y + puck.r >= canvas.height) puck.vy = -puck.vy;

        s1El.textContent = s1; s2El.textContent = s2;
        if (ws.readyState === WebSocket.OPEN) {
          ws.send(JSON.stringify({ t: "puck", x: puck.x, y: puck.y, s1: s1, s2: s2 }));
        }
      }

      // Draw Sparks
      for (let i = sparks.length - 1; i >= 0; i--) {
        const sp = sparks[i]; sp.x += sp.vx; sp.y += sp.vy; sp.life -= 0.05;
        if (sp.life <= 0) { sparks.splice(i, 1); continue; }
        ctx.fillStyle = "#ffe600"; ctx.globalAlpha = sp.life;
        ctx.fillRect(sp.x, sp.y, 3, 3); ctx.globalAlpha = 1;
      }

      // Draw 3D Realistic Mallets & Puck
      drawMallet(myPos.x, myPos.y, role === 1);
      drawMallet(oppPos.x, oppPos.y, role !== 1);
      drawPuck(puck.x, puck.y);

      requestAnimationFrame(loop);
    }
    function resetPuck() { puck.x = 170; puck.y = 250; puck.vx = (Math.random()-0.5)*4; puck.vy = (Math.random()-0.5)*4; }
    loop();
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   3. TANK BATTLE 2D REALISTE (CHENILLES, DEBRIS, PROJECTILES & MURS CAVERNES)
// ----------------------------------------------------------------------------------
const char MP_TANK_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Tank 2D Battle Pro</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: #060810; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      min-height: 100vh; overflow: hidden; touch-action: none;
    }
    header { width: 100%; max-width: 360px; padding: 8px 12px; display: flex; justify-content: space-between; align-items: center; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .status-badge { font-size: 0.8rem; font-weight: 800; padding: 4px 10px; border-radius: 20px; background: rgba(255,170,0,0.15); color: #ffaa00; border: 1px solid #ffaa00; }
    .hp-bar { display: flex; gap: 20px; font-size: 1.1rem; font-weight: 800; margin-bottom: 4px; }
    #canvas-wrap {
      position: relative; width: 340px; height: 360px;
      border: 2px solid #00f3ff; border-radius: 16px; background: #0a0d16;
      box-shadow: 0 0 30px rgba(0,243,255,0.2), inset 0 0 20px rgba(0,0,0,0.8);
      overflow: hidden;
    }
    canvas { width: 100%; height: 100%; display: block; }
    .controls { display: flex; gap: 14px; margin-top: 10px; width: 340px; justify-content: space-between; align-items: center; }
    .dpad { display: grid; grid-template-columns: repeat(3, 46px); grid-template-rows: repeat(2, 46px); gap: 5px; }
    .btn-ctrl { background: linear-gradient(145deg, #182035, #0f1422); border: 1px solid rgba(0,243,255,0.25); color: #fff; border-radius: 10px; font-size: 1.25rem; font-weight: bold; display: flex; align-items: center; justify-content: center; box-shadow: 0 4px 10px rgba(0,0,0,0.4); }
    .btn-ctrl:active { background: #00f3ff; color: #000; transform: scale(0.94); }
    .btn-fire { width: 95px; height: 95px; border-radius: 50%; background: linear-gradient(135deg, #ff0055, #ff5500); border: 2px solid #fff; color: #fff; font-size: 1.35rem; font-weight: 900; box-shadow: 0 0 20px rgba(255,0,85,0.5); cursor: pointer; }
    .btn-fire:active { transform: scale(0.92); }
    #overlay { position: absolute; inset: 0; background: rgba(7,9,16,0.92); backdrop-filter: blur(8px); display: flex; flex-direction: column; align-items: center; justify-content: center; border-radius: 14px; text-align: center; padding: 20px; }
    #overlay h2 { font-size: 1.5rem; margin-bottom: 8px; font-weight: 900; }
    #overlay p { font-size: 0.88rem; color: #8b9bb4; line-height: 1.4; margin-bottom: 12px; }
    .act-btn { padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem; cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; transition: all 0.15s; }
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 15px rgba(0,243,255,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="status-badge" id="badge">Connexion...</span>
  </header>
  <div class="hp-bar">
    <span style="color:#00f3ff;">J1 : <span id="hp1">❤️❤️❤️</span></span>
    <span style="color:#ff0055;">J2 : <span id="hp2">❤️❤️❤️</span></span>
  </div>
  <div id="canvas-wrap">
    <canvas id="c" width="340" height="360"></canvas>
    <div id="overlay">
      <h2 id="ov-title">🛡️ Tank Battle Pro</h2>
      <p id="ov-desc">En attente d'un adversaire...</p>
      <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:240px; margin-top:10px;">
        <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
        <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
      </div>
    </div>
  </div>
  <div class="controls">
    <div class="dpad">
      <div></div><div class="btn-ctrl" id="btn-u">▲</div><div></div>
      <div class="btn-ctrl" id="btn-l">◀</div><div class="btn-ctrl" id="btn-d">▼</div><div class="btn-ctrl" id="btn-r">▶</div>
    </div>
    <button class="btn-fire" id="btn-f">FEU 💥</button>
  </div>

  <script>
    const canvas = document.getElementById('c'), ctx = canvas.getContext('2d');
    const badge = document.getElementById('badge'), overlay = document.getElementById('overlay');
    const ovTitle = document.getElementById('ov-title'), ovDesc = document.getElementById('ov-desc');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');
    const hp1El = document.getElementById('hp1'), hp2El = document.getElementById('hp2');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false;
    let myTank = { x: 50, y: 310, a: 0, hp: 3 };
    let oppTank = { x: 290, y: 50, a: Math.PI, hp: 3 };
    let bullets = [], explosions = [];
    let myRematch = false, oppRematch = false;

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "tank", name: myPseudo }));
    ws.onmessage = (e) => {
      const d = JSON.parse(e.data);
      if (d.t === "joined") {
        role = d.role || d.p;
        badge.textContent = role === 1 ? "🔵 Tank Bleu (J1)" : "🔴 Tank Rouge (J2)";
        if (role === 2) { myTank.x = 290; myTank.y = 50; myTank.a = Math.PI; }
      } else if (d.t === "start") {
        overlay.style.display = "none";
        ovActions.style.display = "none";
        playing = true;
        role = d.role || role || 1;
        badge.textContent = "⚡ Combat En Direct !";
      } else if (d.t === "tank") {
        oppTank.x = d.x; oppTank.y = d.y; oppTank.a = d.a; oppTank.hp = d.hp;
        updateHp();
      } else if (d.t === "bullet") {
        bullets.push({ x: d.x, y: d.y, vx: d.vx, vy: d.vy, fromOpp: true });
      } else if (d.t === "rematch") {
        oppRematch = true;
        if (myRematch) {
          restartTank();
        } else {
          btnRematch.textContent = "⚡ L'adversaire veut rejouer ! [ACCEPTER]";
          btnRematch.style.background = "linear-gradient(180deg, #00ff66, #00cc44)";
          btnRematch.style.color = "#000";
        }
      } else if (d.t === "left" || d.t === "opp_left") {
        playing = false; overlay.style.display = "flex"; ovActions.style.display = "none";
        ovTitle.textContent = "Adversaire déconnecté";
        ovDesc.textContent = "L'autre joueur a quitté le combat.";
      }
    };

    function updateHp() {
      const h1 = role === 1 ? myTank.hp : oppTank.hp;
      const h2 = role === 1 ? oppTank.hp : myTank.hp;
      hp1El.textContent = "❤️".repeat(Math.max(0, h1));
      hp2El.textContent = "❤️".repeat(Math.max(0, h2));
      if (myTank.hp <= 0 || oppTank.hp <= 0) {
        playing = false; overlay.style.display = "flex"; ovActions.style.display = "flex";
        const won = myTank.hp > 0;
        ovTitle.textContent = won ? "🏆 VICTOIRE !" : "💀 DÉFAITE !";
        ovDesc.textContent = won ? "Le char ennemi a été anéanti !" : "Votre char a explosé !";
        btnRematch.disabled = false;
        btnRematch.textContent = "🔄 Rejouer (Revanche)";
        btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
        btnRematch.style.color = "#000";
      }
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.disabled = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) restartTank();
    };

    function restartTank() {
      myRematch = false; oppRematch = false;
      myTank.hp = 3; oppTank.hp = 3;
      bullets = []; explosions = [];
      if (role === 1) { myTank.x = 50; myTank.y = 310; myTank.a = 0; oppTank.x = 290; oppTank.y = 50; oppTank.a = Math.PI; }
      else { myTank.x = 290; myTank.y = 50; myTank.a = Math.PI; oppTank.x = 50; oppTank.y = 310; oppTank.a = 0; }
      updateHp();
      overlay.style.display = "none";
      ovActions.style.display = "none";
      btnRematch.disabled = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
      btnRematch.style.color = "#000";
      playing = true;
    }

    function spawnExplosion(x, y) {
      for (let i = 0; i < 18; i++) {
        const a = Math.random() * Math.PI * 2, sp = 2 + Math.random() * 4;
        explosions.push({ x: x, y: y, vx: Math.cos(a)*sp, vy: Math.sin(a)*sp, life: 1, r: 2 + Math.random()*4, color: i%2===0 ? '#ff0055' : '#ffe600' });
      }
    }

    let keys = {};
    const bindBtn = (id, k) => {
      const el = document.getElementById(id);
      el.addEventListener('touchstart', (e) => { e.preventDefault(); keys[k] = true; });
      el.addEventListener('touchend', (e) => { e.preventDefault(); keys[k] = false; });
      el.addEventListener('mousedown', () => keys[k] = true);
      el.addEventListener('mouseup', () => keys[k] = false);
    };
    bindBtn('btn-u', 'u'); bindBtn('btn-d', 'd'); bindBtn('btn-l', 'l'); bindBtn('btn-r', 'r');
    document.getElementById('btn-f').addEventListener('click', () => {
      if (!playing) return;
      const bx = myTank.x + Math.cos(myTank.a) * 18;
      const by = myTank.y + Math.sin(myTank.a) * 18;
      const bvx = Math.cos(myTank.a) * 6, bvy = Math.sin(myTank.a) * 6;
      bullets.push({ x: bx, y: by, vx: bvx, vy: bvy, fromOpp: false });
      ws.send(JSON.stringify({ t: "bullet", x: bx, y: by, vx: bvx, vy: bvy }));
    });

    function drawRealisticTank(t, isP1) {
      ctx.save();
      ctx.translate(t.x, t.y); ctx.rotate(t.a);

      // Shadow
      ctx.fillStyle = "rgba(0,0,0,0.5)";
      ctx.fillRect(-14, -13, 28, 26);

      // Treads
      ctx.fillStyle = "#1e2230";
      ctx.fillRect(-15, -13, 30, 6);
      ctx.fillRect(-15, 7, 30, 6);
      ctx.fillStyle = "#333b4f";
      for (let tr = -12; tr <= 12; tr += 6) {
        ctx.fillRect(tr, -13, 2, 6);
        ctx.fillRect(tr, 7, 2, 6);
      }

      // Hull
      const hg = ctx.createLinearGradient(-12, -8, 12, 8);
      hg.addColorStop(0, isP1 ? "#0088b3" : "#b3003b");
      hg.addColorStop(0.5, isP1 ? "#00f3ff" : "#ff0055");
      hg.addColorStop(1, isP1 ? "#004466" : "#660022");
      ctx.fillStyle = hg;
      ctx.beginPath(); ctx.roundRect(-12, -8, 24, 16, 4); ctx.fill();

      // Barrel
      ctx.fillStyle = "#1b202e";
      ctx.fillRect(2, -3, 18, 6);
      ctx.fillStyle = "#4a5568";
      ctx.fillRect(17, -4, 4, 8); // Muzzle brake

      // Turret Dome
      const tg = ctx.createRadialGradient(-1, -1, 1, 0, 0, 8);
      tg.addColorStop(0, "#ffffff");
      tg.addColorStop(0.3, isP1 ? "#00f3ff" : "#ff0055");
      tg.addColorStop(1, "#111827");
      ctx.fillStyle = tg;
      ctx.beginPath(); ctx.arc(0, 0, 8, 0, Math.PI*2); ctx.fill();
      ctx.fillStyle = "#111827";
      ctx.beginPath(); ctx.arc(-2, 0, 3, 0, Math.PI*2); ctx.fill();

      ctx.restore();
    }

    function loop() {
      ctx.clearRect(0, 0, canvas.width, canvas.height);

      // Battlefield grid
      ctx.strokeStyle = "rgba(255,255,255,0.03)";
      for (let x = 20; x < canvas.width; x += 30) { ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, canvas.height); ctx.stroke(); }
      for (let y = 20; y < canvas.height; y += 30) { ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(canvas.width, y); ctx.stroke(); }

      if (playing) {
        if (keys['l']) myTank.a -= 0.06;
        if (keys['r']) myTank.a += 0.06;
        if (keys['u']) {
          myTank.x += Math.cos(myTank.a) * 2.5;
          myTank.y += Math.sin(myTank.a) * 2.5;
        }
        if (keys['d']) {
          myTank.x -= Math.cos(myTank.a) * 1.5;
          myTank.y -= Math.sin(myTank.a) * 1.5;
        }
        myTank.x = Math.max(16, Math.min(canvas.width - 16, myTank.x));
        myTank.y = Math.max(16, Math.min(canvas.height - 16, myTank.y));

        if (ws.readyState === WebSocket.OPEN) {
          ws.send(JSON.stringify({ t: "tank", x: myTank.x, y: myTank.y, a: myTank.a, hp: myTank.hp }));
        }

        for (let i = bullets.length - 1; i >= 0; i--) {
          const b = bullets[i];
          b.x += b.vx; b.y += b.vy;
          if (b.x < 0 || b.x > canvas.width || b.y < 0 || b.y > canvas.height) {
            bullets.splice(i, 1); continue;
          }
          if (b.fromOpp) {
            if (Math.hypot(b.x - myTank.x, b.y - myTank.y) < 18) {
              myTank.hp--;
              spawnExplosion(myTank.x, myTank.y);
              bullets.splice(i, 1);
              updateHp();
              continue;
            }
          }
        }
      }

      // Render Bullets (Tracers)
      bullets.forEach(b => {
        ctx.save();
        ctx.fillStyle = "#ffe600";
        ctx.shadowColor = "#ff5500"; ctx.shadowBlur = 10;
        ctx.beginPath(); ctx.arc(b.x, b.y, 4, 0, Math.PI*2); ctx.fill();
        ctx.restore();
      });

      // Render Explosions
      for (let i = explosions.length - 1; i >= 0; i--) {
        const ex = explosions[i];
        ex.x += ex.vx; ex.y += ex.vy; ex.life -= 0.04;
        if (ex.life <= 0) { explosions.splice(i, 1); continue; }
        ctx.fillStyle = ex.color; ctx.globalAlpha = ex.life;
        ctx.beginPath(); ctx.arc(ex.x, ex.y, ex.r * ex.life, 0, Math.PI*2); ctx.fill();
        ctx.globalAlpha = 1;
      }

      // Draw Tanks
      drawRealisticTank(myTank, role === 1);
      drawRealisticTank(oppTank, role !== 1);

      requestAnimationFrame(loop);
    }
    loop();
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   4. DUEL DE REFLEXES CYBER (FEUX TRICOLORES REALISTES, CHRONOMETRE PRECISION)
// ----------------------------------------------------------------------------------
const char MP_REFLEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Duel de Réflexes Pro</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: #060812; color: #fff; font-family: system-ui, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: space-between;
      min-height: 100vh; padding: 14px; text-align: center; overflow: hidden;
    }
    header { width: 100%; max-width: 400px; display: flex; justify-content: space-between; align-items: center; }
    .btn-hub { background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2); color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700; }
    .scores { font-size: 1.6rem; font-weight: 900; }
    #duel-zone {
      width: 100%; max-width: 360px; height: 420px; border-radius: 24px;
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      border: 3px solid rgba(255,255,255,0.15); transition: all 0.15s;
      cursor: pointer; padding: 20px; position: relative;
      box-shadow: 0 10px 35px rgba(0,0,0,0.7);
    }
    .state-wait { background: radial-gradient(circle, #141b30, #080c16); border-color: #00f3ff; box-shadow: 0 0 30px rgba(0,243,255,0.2); }
    .state-ready { background: radial-gradient(circle, #3d0a1b, #150308); border-color: #ff0055; box-shadow: 0 0 45px rgba(255,0,85,0.45); }
    .state-fire { background: radial-gradient(circle, #00662a, #001a0a); border-color: #00ff66; box-shadow: 0 0 60px #00ff66; }
    #msg-big { font-size: 2.2rem; font-weight: 900; text-transform: uppercase; margin-bottom: 10px; letter-spacing: 1px; }
    #msg-sub { font-size: 1rem; color: #8b9bb4; line-height: 1.4; }
    .act-btn {
      padding: 12px 18px; border-radius: 12px; font-weight: 900; font-size: 0.95rem;
      cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%;
      transition: all 0.15s;
    }
    .btn-rematch {
      background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000;
      box-shadow: 0 4px 15px rgba(0,243,255,0.4);
    }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit {
      background: rgba(255,255,255,0.08); color: #8b9bb4;
      border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem;
    }
    .reticle {
      width: 100px; height: 100px; border-radius: 50%; border: 2px dashed rgba(255,255,255,0.3);
      display: flex; align-items: center; justify-content: center; margin-bottom: 16px;
      animation: rotateReticle 10s linear infinite;
    }
    @keyframes rotateReticle { from { transform: rotate(0deg); } to { transform: rotate(360deg); } }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span id="badge" style="color:#00f3ff; font-weight:800; font-size:0.85rem;">Connexion...</span>
  </header>

  <div class="scores">
    <span style="color:#00f3ff;">J1 : <span id="s1">0</span></span>
    <span style="color:#555;"> / </span>
    <span style="color:#ff0055;">J2 : <span id="s2">0</span></span>
  </div>

  <div id="duel-zone" class="state-wait">
    <div class="reticle"><span>🎯</span></div>
    <div id="msg-big">⏳ ATTENTE...</div>
    <div id="msg-sub">En attente d'un adversaire sur son smartphone.</div>
    <div id="ov-actions" style="display:none; flex-direction:column; gap:8px; width:100%; max-width:240px; margin-top:16px;">
      <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
      <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
    </div>
  </div>

  <div style="font-size:0.82rem; color:#889; font-weight:700;">Premier à 3 points remporte le duel !</div>

  <script>
    const zone = document.getElementById('duel-zone');
    const msgBig = document.getElementById('msg-big');
    const msgSub = document.getElementById('msg-sub');
    const ovActions = document.getElementById('ov-actions');
    const btnRematch = document.getElementById('btn-rematch');
    const badge = document.getElementById('badge');
    const s1El = document.getElementById('s1'), s2El = document.getElementById('s2');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false, s1 = 0, s2 = 0;
    let state = "wait";
    let fireTime = 0, timerId = null;
    let myRematch = false, oppRematch = false;

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => ws.send(JSON.stringify({ t: "join", g: "reflex", name: myPseudo }));
    ws.onmessage = (e) => {
      const d = JSON.parse(e.data);
      if (d.t === "joined") {
        role = d.role || d.p;
        badge.textContent = role === 1 ? "🔵 Joueur 1" : "🔴 Joueur 2";
      } else if (d.t === "start") {
        playing = true;
        role = d.role || role || 1;
        badge.textContent = "⚡ Duel en direct";
        ovActions.style.display = "none";
        startNextRound();
      } else if (d.t === "fire_signal") {
        triggerFire();
      } else if (d.t === "tap_win") {
        handleWin(d.winner, d.ms);
      } else if (d.t === "false_start") {
        handleFalseStart(d.who);
      } else if (d.t === "rematch") {
        oppRematch = true;
        if (myRematch) {
          restartReflex();
        } else {
          btnRematch.textContent = "⚡ L'adversaire veut rejouer ! [ACCEPTER]";
          btnRematch.style.background = "linear-gradient(180deg, #00ff66, #00cc44)";
          btnRematch.style.color = "#000";
        }
      } else if (d.t === "left" || d.t === "opp_left") {
        playing = false;
        ovActions.style.display = "none";
        msgBig.textContent = "DÉCONNEXION";
        msgSub.textContent = "L'adversaire a quitté la partie.";
      }
    };

    function startNextRound() {
      if (!playing) return;
      state = "ready";
      zone.className = "state-ready";
      msgBig.textContent = "⚠️ ATTENTION...";
      msgSub.textContent = "Ne touchez PAS ! Attendez le VERT !";
      if (role === 1) {
        const delay = 2000 + Math.random() * 3000;
        clearTimeout(timerId);
        timerId = setTimeout(() => {
          triggerFire();
          ws.send(JSON.stringify({ t: "fire_signal" }));
        }, delay);
      }
    }

    function triggerFire() {
      state = "fire";
      fireTime = performance.now();
      zone.className = "state-fire";
      msgBig.textContent = "⚡ TIREZ ! ⚡";
      msgSub.textContent = "TOUCHEZ L'ÉCRAN VITE !";
      if (navigator.vibrate) navigator.vibrate(100);
    }

    function handleWin(winner, ms) {
      state = "done";
      if (winner === 1) s1++; else s2++;
      s1El.textContent = s1; s2El.textContent = s2;
      const youWon = (winner === role);
      zone.className = youWon ? "state-fire" : "state-ready";
      msgBig.textContent = youWon ? "🏆 GAGNÉ !" : "💀 TROP LENT !";
      msgSub.textContent = `Temps de réaction : ${ms} ms`;

      if (s1 >= 3 || s2 >= 3) {
        playing = false;
        const won = (role === 1 && s1>=3) || (role === 2 && s2>=3);
        msgBig.textContent = won ? "👑 CHAMPION DU DUEL !" : "💀 DÉFAITE FINALE !";
        msgSub.textContent = `Score final : ${s1} - ${s2}`;
        ovActions.style.display = "flex";
        btnRematch.disabled = false;
        btnRematch.textContent = "🔄 Rejouer (Revanche)";
        btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
        btnRematch.style.color = "#000";
      } else {
        setTimeout(startNextRound, 2500);
      }
    }

    function handleFalseStart(who) {
      state = "done";
      if (who === 1) s2++; else s1++;
      s1El.textContent = s1; s2El.textContent = s2;
      const wasYou = (who === role);
      zone.className = "state-ready";
      msgBig.textContent = wasYou ? "❌ FAUX DÉPART !" : "🎉 ADVERSAIRE FAUTIF !";
      msgSub.textContent = wasYou ? "Tu as tiré trop tôt ! Point adverse." : "L'adversaire a tiré avant le vert !";
      setTimeout(startNextRound, 2500);
    }

    btnRematch.onclick = (e) => {
      e.stopPropagation();
      myRematch = true;
      btnRematch.disabled = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) restartReflex();
    };

    function restartReflex() {
      myRematch = false; oppRematch = false;
      s1 = 0; s2 = 0; s1El.textContent = 0; s2El.textContent = 0;
      ovActions.style.display = "none";
      btnRematch.disabled = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "linear-gradient(180deg, #00f3ff, #0077ee)";
      btnRematch.style.color = "#000";
      playing = true;
      startNextRound();
    }

    zone.addEventListener('pointerdown', (e) => {
      if (e.target === btnRematch || e.target.tagName === 'A') return;
      if (!playing || state === "done") return;
      if (state === "ready") {
        ws.send(JSON.stringify({ t: "false_start", who: role }));
        handleFalseStart(role);
      } else if (state === "fire") {
        const ms = Math.round(performance.now() - fireTime);
        ws.send(JSON.stringify({ t: "tap_win", winner: role, ms: ms }));
        handleWin(role, ms);
      }
    });
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------------
//   5. TAP DUEL : TIR A LA CORDE TACTILE (GAUGE DYNAMIQUE, COMBOS & VIBRATIONS)
// ----------------------------------------------------------------------------------
const char MP_TAPDUEL_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Tap Duel : Tir à la Corde</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: radial-gradient(circle at 50% 30%, #0c1229 0%, #040711 100%);
      color: #fff; font-family: system-ui, -apple-system, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: space-between;
      min-height: 100vh; padding: 14px; text-align: center; overflow: hidden; touch-action: manipulation;
    }
    header { width: 100%; max-width: 400px; display: flex; justify-content: space-between; align-items: center; }
    .btn-hub {
      background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2);
      color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700;
    }
    .status-badge {
      font-size: 0.8rem; font-weight: 800; padding: 4px 12px; border-radius: 20px;
      background: rgba(0,243,255,0.15); color: #00f3ff; border: 1px solid #00f3ff;
    }
    .scoreboard {
      display: flex; justify-content: space-between; align-items: center;
      width: 100%; max-width: 380px; padding: 10px 16px; background: rgba(255,255,255,0.04);
      border-radius: 16px; border: 1px solid rgba(255,255,255,0.08); backdrop-filter: blur(10px);
    }
    .player-card { display: flex; flex-direction: column; align-items: center; gap: 4px; }
    .player-card.p1 { color: #00f3ff; text-shadow: 0 0 10px rgba(0,243,255,0.6); }
    .player-card.p2 { color: #ff0055; text-shadow: 0 0 10px rgba(255,0,85,0.6); }
    .p-name { font-size: 0.85rem; font-weight: 800; max-width: 110px; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
    .p-score { font-size: 1.6rem; font-weight: 900; }
    .vs-pill { font-size: 0.85rem; font-weight: 900; color: #ffbb00; padding: 4px 10px; background: rgba(255,187,0,0.1); border-radius: 12px; border: 1px solid rgba(255,187,0,0.3); }

    /* Tug of war bar */
    .arena-box {
      width: 100%; max-width: 380px; display: flex; flex-direction: column; align-items: center; gap: 14px;
      background: rgba(10,16,36,0.6); border: 2px solid rgba(255,255,255,0.1); border-radius: 24px; padding: 20px 16px;
      box-shadow: 0 10px 40px rgba(0,0,0,0.8), inset 0 0 30px rgba(0,0,0,0.5);
    }
    .rope-track {
      width: 100%; height: 26px; background: #080c18; border-radius: 13px;
      position: relative; overflow: hidden; border: 2px solid rgba(255,255,255,0.15);
      box-shadow: inset 0 3px 6px rgba(0,0,0,0.8);
    }
    .rope-fill-p1 {
      position: absolute; left: 0; top: 0; bottom: 0; width: 50%;
      background: linear-gradient(90deg, #00f3ff, #0077ee); transition: width 0.08s ease-out;
      box-shadow: 0 0 15px rgba(0,243,255,0.6);
    }
    .rope-fill-p2 {
      position: absolute; right: 0; top: 0; bottom: 0; width: 50%;
      background: linear-gradient(270deg, #ff0055, #ff5500); transition: width 0.08s ease-out;
      box-shadow: 0 0 15px rgba(255,0,85,0.6);
    }
    .knot-marker {
      position: absolute; top: -3px; bottom: -3px; width: 14px; margin-left: -7px; left: 50%;
      background: #ffffff; border-radius: 6px; box-shadow: 0 0 16px #ffffff, 0 0 8px #ffbb00;
      transition: left 0.08s ease-out; z-index: 5;
    }

    .gauge-labels {
      display: flex; justify-content: space-between; width: 100%; font-size: 0.75rem; font-weight: 800; color: #8b9bb4;
    }

    .cps-badge {
      font-size: 0.85rem; font-weight: 800; color: #ffe600; min-height: 22px;
    }

    /* Giant Tap Button */
    .tap-area {
      width: 100%; max-width: 340px; height: 180px; border-radius: 28px;
      display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 8px;
      background: radial-gradient(circle at 50% 35%, #1a2a50, #0c142b);
      border: 3px solid rgba(0,243,255,0.5);
      box-shadow: 0 12px 35px rgba(0,0,0,0.8), 0 0 25px rgba(0,243,255,0.25), inset 0 2px 4px rgba(255,255,255,0.3);
      cursor: pointer; position: relative; transition: all 0.08s; -webkit-tap-highlight-color: transparent;
    }
    .tap-area:active {
      transform: scale(0.95);
      background: radial-gradient(circle at 50% 50%, #25407d, #0f1c3d);
      box-shadow: 0 4px 15px rgba(0,0,0,0.9), 0 0 35px rgba(0,243,255,0.6);
      border-color: #00f3ff;
    }
    .tap-text {
      font-size: 2.2rem; font-weight: 900; letter-spacing: 2px;
      background: linear-gradient(180deg, #ffffff, #00f3ff);
      -webkit-background-clip: text; -webkit-text-fill-color: transparent;
      pointer-events: none;
    }
    .tap-sub {
      font-size: 0.85rem; font-weight: 800; color: #8b9bb4; text-transform: uppercase; letter-spacing: 1px;
      pointer-events: none;
    }

    /* Overlay modal */
    #overlay {
      position: fixed; inset: 0; background: rgba(4,7,17,0.92);
      backdrop-filter: blur(10px); -webkit-backdrop-filter: blur(10px);
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      padding: 24px; z-index: 99; text-align: center;
    }
    #ov-title { font-size: 1.8rem; font-weight: 900; margin-bottom: 10px; }
    #ov-desc { font-size: 0.95rem; color: #8b9bb4; max-width: 320px; line-height: 1.5; margin-bottom: 20px; }
    .act-btn {
      padding: 13px 20px; border-radius: 14px; font-weight: 900; font-size: 0.95rem;
      cursor: pointer; border: none; text-decoration: none; text-align: center; width: 100%; max-width: 260px;
      transition: all 0.15s; margin-bottom: 10px;
    }
    .btn-rematch {
      background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000;
      box-shadow: 0 4px 20px rgba(0,243,255,0.4);
    }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit {
      background: rgba(255,255,255,0.08); color: #8b9bb4;
      border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem;
    }

    @keyframes pulseHint {
      0%, 100% { transform: scale(1); }
      50% { transform: scale(1.03); }
    }
    .ready-pulse { animation: pulseHint 1.2s infinite ease-in-out; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="status-badge" id="badge">Connexion...</span>
  </header>

  <div class="scoreboard">
    <div class="player-card p1">
      <div class="p-name" id="p1-name">Joueur 1</div>
      <div class="p-score" id="s1">0</div>
    </div>
    <div class="vs-pill">1er à 2 pts</div>
    <div class="player-card p2">
      <div class="p-name" id="p2-name">Joueur 2</div>
      <div class="p-score" id="s2">0</div>
    </div>
  </div>

  <div class="arena-box">
    <div class="gauge-labels">
      <span style="color:#00f3ff">◀ CAMP J1</span>
      <span style="color:#ffe600">CENTRE</span>
      <span style="color:#ff0055">CAMP J2 ▶</span>
    </div>
    <div class="rope-track">
      <div class="rope-fill-p1" id="fill-p1"></div>
      <div class="rope-fill-p2" id="fill-p2"></div>
      <div class="knot-marker" id="knot"></div>
    </div>
    <div class="cps-badge" id="cps-val">Vitesse : 0 taps/sec</div>
  </div>

  <div class="tap-area ready-pulse" id="btn-tap">
    <div class="tap-text">⚡ TAPEZ !</div>
    <div class="tap-sub" id="tap-sub">Cliquez à fond !</div>
  </div>

  <div id="overlay">
    <h2 id="ov-title">⚡ Tir à la Corde</h2>
    <p id="ov-desc">En attente d'un adversaire sur son smartphone...</p>
    <div id="ov-actions" style="display:none; flex-direction:column; align-items:center; width:100%;">
      <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
      <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
    </div>
  </div>

  <script>
    const badge = document.getElementById('badge');
    const p1Name = document.getElementById('p1-name'), p2Name = document.getElementById('p2-name');
    const s1El = document.getElementById('s1'), s2El = document.getElementById('s2');
    const fillP1 = document.getElementById('fill-p1'), fillP2 = document.getElementById('fill-p2'), knot = document.getElementById('knot');
    const cpsVal = document.getElementById('cps-val');
    const btnTap = document.getElementById('btn-tap'), tapSub = document.getElementById('tap-sub');
    const overlay = document.getElementById('overlay'), ovTitle = document.getElementById('ov-title'), ovDesc = document.getElementById('ov-desc');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false;
    let score1 = 0, score2 = 0;
    let ropePos = 0; // -100 (J1 gagne) à +100 (J2 gagne)
    let myTapCount = 0, lastTapCalc = Date.now();
    let myRematch = false, oppRematch = false;

    // Web Audio synthesizer for haptic & click sensation
    const AudioCtx = window.AudioContext || window.webkitAudioContext;
    let actx = null;
    function playTapSfx(freq = 480) {
      try {
        if (!actx) actx = new AudioCtx();
        if (actx.state === 'suspended') actx.resume();
        const osc = actx.createOscillator(), g = actx.createGain();
        osc.type = 'triangle';
        osc.frequency.setValueAtTime(freq, actx.currentTime);
        osc.frequency.exponentialRampToValueAtTime(120, actx.currentTime + 0.08);
        g.gain.setValueAtTime(0.2, actx.currentTime);
        g.gain.exponentialRampToValueAtTime(0.01, actx.currentTime + 0.08);
        osc.connect(g); g.connect(actx.destination);
        osc.start(); osc.stop(actx.currentTime + 0.08);
      } catch(e){}
    }

    function updateRopeUI() {
      // ropePos is from -100 to +100. Percentage is 0% to 100%
      const pct = (ropePos + 100) / 2; // 0 to 100
      fillP1.style.width = pct + '%';
      fillP2.style.width = (100 - pct) + '%';
      knot.style.left = pct + '%';
    }

    function handleTap(e) {
      if (e) { e.preventDefault(); e.stopPropagation(); }
      if (!playing) return;

      myTapCount++;
      playTapSfx(role === 1 ? 520 : 640);
      if (navigator.vibrate) navigator.vibrate(10);

      // Local movement
      const delta = (role === 1) ? -4 : 4;
      ropePos += delta;
      if (ropePos < -100) ropePos = -100;
      if (ropePos > 100) ropePos = 100;
      updateRopeUI();

      // Send tap over ws
      ws.send(JSON.stringify({ t: "tap", p: role, pos: ropePos }));

      checkRoundEnd();
    }

    btnTap.addEventListener('touchstart', handleTap, { passive: false });
    btnTap.addEventListener('mousedown', handleTap);

    // CPS calculator
    setInterval(() => {
      if (!playing) return;
      const now = Date.now();
      const dt = (now - lastTapCalc) / 1000;
      const cps = (myTapCount / dt).toFixed(1);
      cpsVal.textContent = 'Votre rythme : ' + cps + ' taps/sec';
      myTapCount = 0;
      lastTapCalc = now;
    }, 500);

    function checkRoundEnd() {
      if (!playing) return;
      if (ropePos <= -100) {
        // P1 round win
        endRound(1);
      } else if (ropePos >= 100) {
        // P2 round win
        endRound(2);
      }
    }

    function endRound(winnerRole) {
      playing = false;
      if (winnerRole === 1) score1++;
      else score2++;
      s1El.textContent = score1;
      s2El.textContent = score2;

      if (score1 >= 2 || score2 >= 2) {
        // Match over
        const isMeWinner = (role === winnerRole);
        ovTitle.textContent = isMeWinner ? "🏆 VICTOIRE ÉCLATANTE !" : "💀 DÉFAITE...";
        ovTitle.style.color = isMeWinner ? "#00ff66" : "#ff0055";
        ovDesc.textContent = isMeWinner ? "Vous avez écrasé votre adversaire au tir à la corde !" : "L'adversaire a tiré plus vite cette fois !";
        ovActions.style.display = "flex";
        overlay.style.display = "flex";
      } else {
        // Next round after brief pause
        ovTitle.textContent = "💥 MANCHE POUR J" + winnerRole + " !";
        ovTitle.style.color = "#ffe600";
        ovDesc.textContent = "Prochaine manche dans 2 secondes...";
        ovActions.style.display = "none";
        overlay.style.display = "flex";
        setTimeout(() => {
          ropePos = 0;
          updateRopeUI();
          overlay.style.display = "none";
          playing = true;
        }, 2000);
      }
    }

    function resetFullMatch() {
      score1 = 0; score2 = 0; ropePos = 0;
      s1El.textContent = "0"; s2El.textContent = "0";
      updateRopeUI();
      myRematch = false; oppRematch = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "";
      overlay.style.display = "none";
      ovActions.style.display = "none";
      playing = true;
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      btnRematch.style.background = "rgba(255,255,255,0.2)";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) resetFullMatch();
    };

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => {
      badge.textContent = "Recherche...";
      ws.send(JSON.stringify({ t: "join", g: "tapduel", name: myPseudo }));
    };

    ws.onmessage = (e) => {
      try {
        const d = JSON.parse(e.data);
        if (d.t === "joined") {
          role = d.role || d.p;
          badge.textContent = role === 1 ? "🔵 Joueur 1" : "🔴 Joueur 2";
          ovTitle.textContent = role === 1 ? "🔵 Joueur 1 Prêt" : "🔴 Joueur 2 Prêt";
          ovDesc.textContent = "En attente du second joueur...";
          tapSub.textContent = (role === 1) ? "Tirez vers la gauche (Cyan)" : "Tirez vers la droite (Rouge)";
        } else if (d.t === "start") {
          overlay.style.display = "none";
          ovActions.style.display = "none";
          playing = true;
          role = d.role || role || 1;
          badge.textContent = "⚡ En Direct";
          badge.style.color = "#00ff66"; badge.style.borderColor = "#00ff66";
          if (d.opp) {
            if (role === 1) { p1Name.textContent = myPseudo; p2Name.textContent = d.opp; }
            else { p1Name.textContent = d.opp; p2Name.textContent = myPseudo; }
          }
        } else if (d.t === "tap") {
          // Opponent tap
          const delta = (d.p === 1) ? -4 : 4;
          ropePos += delta;
          if (ropePos < -100) ropePos = -100;
          if (ropePos > 100) ropePos = 100;
          updateRopeUI();
          checkRoundEnd();
        } else if (d.t === "rematch") {
          oppRematch = true;
          if (myRematch) {
            resetFullMatch();
          } else {
            ovDesc.textContent = "L'adversaire veut sa revanche ! Cliquez sur Rejouer.";
          }
        } else if (d.t === "opp_left") {
          playing = false;
          badge.textContent = "Adversaire parti";
          badge.style.color = "#ff0055"; badge.style.borderColor = "#ff0055";
          ovTitle.textContent = "Adversaire Déconnecté";
          ovDesc.textContent = "Votre adversaire a quitté la partie.";
          ovActions.style.display = "flex";
          overlay.style.display = "flex";
        }
      } catch(err){}
    };
  </script>
</body>
</html>

)rawliteral";

#endif // GAME_PACK_MP_REFLEXE_H
