#ifndef GAME_FPS3D_H
#define GAME_FPS3D_H

#include <pgmspace.h>

const char GAME_FPS3D_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Cyber-FPS 3D Retro WebGL</title>
  <style>
    :root {
      --cyan: #00f3ff;
      --pink: #ff0055;
      --yellow: #ffe600;
      --green: #00ff66;
      --bg: #05070d;
      --panel: rgba(14, 18, 30, 0.85);
    }
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    html, body {
      width: 100%; height: 100%; overflow: hidden; background: #000;
      font-family: system-ui, -apple-system, sans-serif; color: #fff;
    }
    #gameContainer {
      position: relative; width: 100%; height: 100%; display: flex; align-items: center; justify-content: center;
    }
    canvas#view3D {
      width: 100%; height: 100%; object-fit: cover; display: block;
    }
    /* HUD Overlays */
    .hud-top {
      position: absolute; top: 12px; left: 14px; right: 14px;
      display: flex; justify-content: space-between; align-items: center;
      pointer-events: none; z-index: 10;
    }
    .hud-stat-box {
      background: var(--panel); border: 1px solid rgba(0, 243, 255, 0.3);
      border-radius: 12px; padding: 8px 14px; backdrop-filter: blur(8px);
      display: flex; align-items: center; gap: 12px; font-weight: 800; font-size: 0.85rem;
    }
    .btn-exit {
      pointer-events: auto; background: rgba(255,255,255,0.1); border: 1px solid rgba(255,255,255,0.25);
      color: #fff; text-decoration: none; padding: 6px 12px; border-radius: 8px; font-weight: bold; font-size: 0.8rem;
    }
    /* Minimap */
    #minimap {
      position: absolute; top: 60px; right: 14px; width: 90px; height: 90px;
      background: rgba(5, 8, 16, 0.75); border: 1px solid var(--cyan);
      border-radius: 10px; z-index: 10; pointer-events: none;
    }
    /* Crosshair */
    .crosshair {
      position: absolute; top: 50%; left: 50%; width: 14px; height: 14px;
      transform: translate(-50%, -50%); pointer-events: none; z-index: 10;
    }
    .crosshair::before, .crosshair::after {
      content: ''; position: absolute; background: var(--cyan); box-shadow: 0 0 6px var(--cyan);
    }
    .crosshair::before { top: 6px; left: 0; width: 14px; height: 2px; }
    .crosshair::after { top: 0; left: 6px; width: 2px; height: 14px; }

    /* HUD Bottom */
    .hud-bottom {
      position: absolute; bottom: 14px; left: 14px; right: 14px;
      display: flex; justify-content: space-between; align-items: flex-end;
      pointer-events: none; z-index: 10;
    }
    .stat-pill {
      display: flex; flex-direction: column; gap: 4px;
    }
    .bar-wrap {
      width: 130px; height: 10px; background: rgba(0,0,0,0.6); border: 1px solid rgba(255,255,255,0.2);
      border-radius: 5px; overflow: hidden;
    }
    .bar-fill { height: 100%; transition: width 0.15s; }
    .fill-hp { background: linear-gradient(90deg, #ff0055, #ff5500); }
    .fill-shield { background: linear-gradient(90deg, #0077ff, var(--cyan)); }

    .ammo-pill {
      font-size: 1.4rem; font-weight: 900; color: var(--yellow); text-shadow: 0 0 10px rgba(255,230,0,0.5);
      background: var(--panel); border: 1px solid rgba(255,230,0,0.3); padding: 8px 16px; border-radius: 12px;
    }

    /* Mobile Controls */
    .mobile-touch-zone {
      position: absolute; inset: 0; z-index: 5; pointer-events: auto; display: flex;
    }
    .touch-left { width: 50%; height: 100%; position: relative; }
    .touch-right { width: 50%; height: 100%; position: relative; }

    .virtual-stick {
      position: absolute; width: 110px; height: 110px; border-radius: 50%;
      border: 2px solid rgba(0, 243, 255, 0.4); background: rgba(0, 243, 255, 0.1);
      display: none; transform: translate(-50%, -50%); pointer-events: none;
    }
    .stick-knob {
      position: absolute; width: 44px; height: 44px; border-radius: 50%;
      background: var(--cyan); box-shadow: 0 0 14px var(--cyan);
      top: 50%; left: 50%; transform: translate(-50%, -50%);
    }

    .btn-fire-touch {
      position: absolute; bottom: 30px; right: 30px; width: 80px; height: 80px; border-radius: 50%;
      background: radial-gradient(circle, #ff0055, #990033); border: 3px solid #ff3377;
      color: #fff; font-weight: 900; font-size: 1rem; text-transform: uppercase;
      box-shadow: 0 0 20px rgba(255, 0, 85, 0.6); pointer-events: auto; z-index: 20;
      display: flex; align-items: center; justify-content: center; cursor: pointer;
    }
    .btn-fire-touch:active { transform: scale(0.92); box-shadow: 0 0 35px rgba(255, 0, 85, 0.9); }

    .btn-reload-touch {
      position: absolute; bottom: 125px; right: 40px; width: 50px; height: 50px; border-radius: 50%;
      background: rgba(18, 24, 42, 0.9); border: 1px solid var(--yellow);
      color: var(--yellow); font-weight: 900; font-size: 0.8rem; pointer-events: auto; z-index: 20;
      display: flex; align-items: center; justify-content: center; cursor: pointer;
    }

    /* Weapon Overlay (Canvas drawn or CSS animated) */
    #weaponCanvas {
      position: absolute; bottom: 0; left: 50%; transform: translateX(-50%);
      width: 280px; height: 220px; pointer-events: none; z-index: 8;
    }

    /* Hit / Flash Effects */
    #damageFlash {
      position: absolute; inset: 0; background: rgba(255,0,0,0.4); pointer-events: none; z-index: 15;
      opacity: 0; transition: opacity 0.1s;
    }

    /* Screen Modals */
    .screen-modal {
      position: absolute; inset: 0; background: rgba(5, 7, 13, 0.92); backdrop-filter: blur(10px);
      display: flex; flex-direction: column; align-items: center; justify-content: center;
      padding: 24px; text-align: center; z-index: 50; gap: 16px;
    }
    .screen-modal h1 {
      font-size: 2rem; text-transform: uppercase; letter-spacing: 2px;
      background: linear-gradient(135deg, #fff, var(--cyan), var(--pink));
      -webkit-background-clip: text; -webkit-text-fill-color: transparent;
    }
    .btn-play {
      background: linear-gradient(135deg, var(--cyan), #0077ff); color: #000;
      border: none; padding: 14px 34px; border-radius: 30px; font-weight: 900; font-size: 1.1rem;
      text-transform: uppercase; cursor: pointer; box-shadow: 0 0 25px rgba(0, 243, 255, 0.5);
      transition: all 0.2s;
    }
    .btn-play:active { transform: scale(0.95); }
  </style>
</head>
<body>

<div id="gameContainer">
  <canvas id="view3D"></canvas>
  <canvas id="minimap"></canvas>
  <canvas id="weaponCanvas"></canvas>

  <div class="crosshair"></div>
  <div id="damageFlash"></div>

  <!-- HUD TOP -->
  <div class="hud-top">
    <div class="hud-stat-box">
      <a href="/hub" class="btn-exit">⬅ Quitter</a>
      <span>SCORE : <span id="txtScore" style="color:var(--cyan);">0</span></span>
      <span>ENNEMIS : <span id="txtEnemies" style="color:var(--pink);">0</span></span>
    </div>
  </div>

  <!-- HUD BOTTOM -->
  <div class="hud-bottom">
    <div class="hud-stat-box" style="flex-direction:column; align-items:flex-start; gap:6px;">
      <div class="stat-pill">
        <span style="font-size:0.75rem; color:#ff5588;">❤️ SANTÉ (<span id="txtHp">100</span>%)</span>
        <div class="bar-wrap"><div id="barHp" class="bar-fill fill-hp" style="width:100%;"></div></div>
      </div>
      <div class="stat-pill">
        <span style="font-size:0.75rem; color:var(--cyan);">🛡️ BOUCLIER (<span id="txtShield">100</span>%)</span>
        <div class="bar-wrap"><div id="barShield" class="bar-fill fill-shield" style="width:100%;"></div></div>
      </div>
    </div>

    <div class="ammo-pill">
      ⚡ <span id="txtAmmo">30</span> / <span id="txtAmmoMax" style="font-size:0.9rem; color:var(--dim);">90</span>
    </div>
  </div>

  <!-- Touch Controls for Mobile -->
  <div class="mobile-touch-zone" id="touchZone">
    <div class="touch-left" id="touchLeft">
      <div class="virtual-stick" id="stickLeft"><div class="stick-knob" id="knobLeft"></div></div>
    </div>
    <div class="touch-right" id="touchRight">
      <div class="virtual-stick" id="stickRight"><div class="stick-knob" id="knobRight"></div></div>
    </div>
  </div>

  <div class="btn-fire-touch" id="btnFireTouch" ontouchstart="triggerShoot(event)" onmousedown="triggerShoot(event)">🔥</div>
  <div class="btn-reload-touch" id="btnReloadTouch" ontouchstart="triggerReload(event)" onmousedown="triggerReload(event)">🔄 R</div>

  <!-- START SCREEN -->
  <div class="screen-modal" id="startScreen">
    <h1>CYBER-FPS 3D</h1>
    <p style="color:var(--cyan); font-weight:800; font-size:0.95rem;">Moteur 3D Rétro Raycasting 60 FPS</p>
    <p style="color:#8b9bb4; max-width:400px; font-size:0.85rem; line-height:1.5;">
      Infiltrez la station cybernétique, éliminez les drones de combat et sécurisez le secteur !
    </p>
    <div style="background:rgba(255,255,255,0.05); padding:12px; border-radius:12px; font-size:0.8rem; color:#aaa; max-width:380px;">
      <b>📱 Mobile :</b> Joystick gauche (Déplacement), Joystick droit (Viser), Bouton 🔥 (Tir)<br>
      <b>💻 PC :</b> ZQSD/WASD pour bouger, Souris/Espace pour tirer, R pour recharger.
    </div>
    <button class="btn-play" onclick="startGame()">🚀 Démarrer la Mission</button>
  </div>

  <!-- GAME OVER SCREEN -->
  <div class="screen-modal" id="endScreen" style="display:none;">
    <h1 id="endTitle">MISSION ÉCHOUÉE</h1>
    <p id="endDesc" style="color:var(--pink); font-size:1.1rem; font-weight:800;">Votre signal vital s'est éteint.</p>
    <p style="color:#fff; font-size:1rem;">Score Final : <b id="endScoreText" style="color:var(--yellow);">0</b></p>
    <button class="btn-play" onclick="startGame()">🔄 Recommencer</button>
  </div>
</div>

<script>
  // Web Audio Synthesizer (Zero asset loading)
  const audioCtx = new (window.AudioContext || window.webkitAudioContext)();
  function playSound(type) {
    if (!audioCtx) return;
    if (audioCtx.state === 'suspended') audioCtx.resume();
    const now = audioCtx.currentTime;

    if (type === 'shoot') {
      const osc = audioCtx.createOscillator();
      const gain = audioCtx.createGain();
      osc.type = 'sawtooth';
      osc.frequency.setValueAtTime(800, now);
      osc.frequency.exponentialRampToValueAtTime(100, now + 0.15);
      gain.gain.setValueAtTime(0.3, now);
      gain.gain.linearRampToValueAtTime(0.01, now + 0.15);
      osc.connect(gain); gain.connect(audioCtx.destination);
      osc.start(now); osc.stop(now + 0.15);
    } else if (type === 'hit') {
      const osc = audioCtx.createOscillator();
      const gain = audioCtx.createGain();
      osc.type = 'triangle';
      osc.frequency.setValueAtTime(150, now);
      osc.frequency.linearRampToValueAtTime(40, now + 0.1);
      gain.gain.setValueAtTime(0.4, now);
      gain.gain.linearRampToValueAtTime(0.01, now + 0.1);
      osc.connect(gain); gain.connect(audioCtx.destination);
      osc.start(now); osc.stop(now + 0.1);
    } else if (type === 'explode') {
      const osc = audioCtx.createOscillator();
      const gain = audioCtx.createGain();
      osc.type = 'square';
      osc.frequency.setValueAtTime(120, now);
      osc.frequency.exponentialRampToValueAtTime(20, now + 0.35);
      gain.gain.setValueAtTime(0.5, now);
      gain.gain.linearRampToValueAtTime(0.01, now + 0.35);
      osc.connect(gain); gain.connect(audioCtx.destination);
      osc.start(now); osc.stop(now + 0.35);
    } else if (type === 'reload') {
      const osc = audioCtx.createOscillator();
      const gain = audioCtx.createGain();
      osc.type = 'sine';
      osc.frequency.setValueAtTime(400, now);
      osc.frequency.setValueAtTime(800, now + 0.1);
      gain.gain.setValueAtTime(0.2, now);
      gain.gain.linearRampToValueAtTime(0.01, now + 0.25);
      osc.connect(gain); gain.connect(audioCtx.destination);
      osc.start(now); osc.stop(now + 0.25);
    }
  }

  // 3D Raycasting Engine Constants
  const MAP_W = 16;
  const MAP_H = 16;
  const MAP = [
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
    1,0,2,2,0,0,1,0,3,3,3,0,2,2,0,1,
    1,0,2,0,0,0,0,0,0,0,3,0,0,2,0,1,
    1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,
    1,0,0,0,1,0,0,1,0,2,2,2,2,0,0,1,
    1,1,0,1,1,0,0,1,0,0,0,0,2,0,0,1,
    1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
    1,0,3,3,0,1,1,0,0,0,1,0,3,3,0,1,
    1,0,3,0,0,1,1,0,2,0,0,0,0,3,0,1,
    1,0,0,0,0,0,0,0,2,0,0,0,0,0,0,1,
    1,0,2,2,0,0,3,0,2,2,0,1,1,0,0,1,
    1,0,0,2,0,0,3,0,0,0,0,1,1,0,0,1,
    1,0,0,0,0,0,3,3,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
  ];

  const canvas3D = document.getElementById('view3D');
  const ctx3D = canvas3D.getContext('2d');
  const minimapCanvas = document.getElementById('minimap');
  const mmCtx = minimapCanvas.getContext('2d');
  const wpCanvas = document.getElementById('weaponCanvas');
  const wpCtx = wpCanvas.getContext('2d');

  let player = {
    x: 2.5, y: 2.5, dir: 0,
    fov: Math.PI / 3,
    hp: 100, shield: 100,
    ammo: 30, maxAmmo: 90,
    reloading: false, score: 0
  };

  let enemies = [];
  let particles = [];
  let isRunning = false;
  let lastTime = 0;
  let weaponRecoil = 0;
  let weaponBob = 0;

  function resizeCanvases() {
    canvas3D.width = Math.min(window.innerWidth, 640);
    canvas3D.height = Math.min(window.innerHeight, 480);
    minimapCanvas.width = 90;
    minimapCanvas.height = 90;
    wpCanvas.width = 280;
    wpCanvas.height = 220;
  }
  window.addEventListener('resize', resizeCanvases);
  resizeCanvases();

  function spawnEnemies() {
    enemies = [
      { x: 5.5, y: 2.5, hp: 50, alive: true, shootTimer: 0, alert: false },
      { x: 13.5, y: 3.5, hp: 50, alive: true, shootTimer: 0, alert: false },
      { x: 10.5, y: 9.5, hp: 50, alive: true, shootTimer: 0, alert: false },
      { x: 3.5, y: 13.5, hp: 50, alive: true, shootTimer: 0, alert: false },
      { x: 13.5, y: 13.5, hp: 75, alive: true, shootTimer: 0, alert: false }
    ];
  }

  function startGame() {
    document.getElementById('startScreen').style.display = 'none';
    document.getElementById('endScreen').style.display = 'none';
    player.x = 2.5; player.y = 2.5; player.dir = 0;
    player.hp = 100; player.shield = 100;
    player.ammo = 30; player.maxAmmo = 90;
    player.score = 0; player.reloading = false;
    spawnEnemies();
    particles = [];
    isRunning = true;
    lastTime = performance.now();
    updateHUD();
    requestAnimationFrame(gameLoop);
  }

  // Input Handling (Keyboard + Joysticks)
  let keys = {};
  window.addEventListener('keydown', e => {
    keys[e.code] = true;
    if (e.code === 'Space') triggerShoot(e);
    if (e.code === 'KeyR') triggerReload(e);
  });
  window.addEventListener('keyup', e => { keys[e.code] = false; });

  // Mobile Virtual Joystick Logic
  let stickL = { active: false, startX: 0, startY: 0, curX: 0, curY: 0, touchId: null };
  let stickR = { active: false, startX: 0, startY: 0, curX: 0, curY: 0, touchId: null };

  const touchLeftZone = document.getElementById('touchLeft');
  const touchRightZone = document.getElementById('touchRight');
  const stickElemL = document.getElementById('stickLeft');
  const knobElemL = document.getElementById('knobLeft');
  const stickElemR = document.getElementById('stickRight');
  const knobElemR = document.getElementById('knobRight');

  touchLeftZone.addEventListener('touchstart', e => {
    e.preventDefault();
    const t = e.changedTouches[0];
    stickL.active = true; stickL.touchId = t.identifier;
    stickL.startX = t.clientX; stickL.startY = t.clientY;
    stickL.curX = t.clientX; stickL.curY = t.clientY;
    stickElemL.style.display = 'block';
    stickElemL.style.left = t.clientX + 'px';
    stickElemL.style.top = t.clientY + 'px';
    knobElemL.style.left = '50%'; knobElemL.style.top = '50%';
  }, { passive: false });

  touchRightZone.addEventListener('touchstart', e => {
    e.preventDefault();
    const t = e.changedTouches[0];
    stickR.active = true; stickR.touchId = t.identifier;
    stickR.startX = t.clientX; stickR.startY = t.clientY;
    stickR.curX = t.clientX; stickR.curY = t.clientY;
    stickElemR.style.display = 'block';
    stickElemR.style.left = t.clientX + 'px';
    stickElemR.style.top = t.clientY + 'px';
    knobElemR.style.left = '50%'; knobElemR.style.top = '50%';
  }, { passive: false });

  window.addEventListener('touchmove', e => {
    for (let i = 0; i < e.changedTouches.length; i++) {
      const t = e.changedTouches[i];
      if (stickL.active && t.identifier === stickL.touchId) {
        let dx = t.clientX - stickL.startX;
        let dy = t.clientY - stickL.startY;
        let dist = Math.sqrt(dx*dx + dy*dy);
        if (dist > 45) { dx = (dx/dist)*45; dy = (dy/dist)*45; }
        knobElemL.style.left = (50 + dx) + '%';
        knobElemL.style.top = (50 + dy) + '%';
        stickL.curX = stickL.startX + dx;
        stickL.curY = stickL.startY + dy;
      }
      if (stickR.active && t.identifier === stickR.touchId) {
        let dx = t.clientX - stickR.startX;
        let dy = t.clientY - stickR.startY;
        let dist = Math.sqrt(dx*dx + dy*dy);
        if (dist > 45) { dx = (dx/dist)*45; dy = (dy/dist)*45; }
        knobElemR.style.left = (50 + dx) + '%';
        knobElemR.style.top = (50 + dy) + '%';
        stickR.curX = stickR.startX + dx;
        stickR.curY = stickR.startY + dy;
      }
    }
  });

  window.addEventListener('touchend', e => {
    for (let i = 0; i < e.changedTouches.length; i++) {
      const t = e.changedTouches[i];
      if (stickL.active && t.identifier === stickL.touchId) {
        stickL.active = false; stickElemL.style.display = 'none';
      }
      if (stickR.active && t.identifier === stickR.touchId) {
        stickR.active = false; stickElemR.style.display = 'none';
      }
    }
  });

  function triggerShoot(e) {
    if (e) e.preventDefault();
    if (!isRunning || player.reloading) return;
    if (player.ammo <= 0) { triggerReload(); return; }

    player.ammo--;
    playSound('shoot');
    weaponRecoil = 25;
    updateHUD();

    // Raycast center shot to hit enemies
    let hitEnemy = null;
    let minDist = 8;
    enemies.forEach(en => {
      if (!en.alive) return;
      let dx = en.x - player.x;
      let dy = en.y - player.y;
      let dist = Math.sqrt(dx*dx + dy*dy);
      let angleTo = Math.atan2(dy, dx);
      let diff = angleTo - player.dir;
      while (diff > Math.PI) diff -= Math.PI*2;
      while (diff < -Math.PI) diff += Math.PI*2;

      if (Math.abs(diff) < 0.15 && dist < minDist) {
        // Check wall occlusion
        if (!isWallBetween(player.x, player.y, en.x, en.y)) {
          minDist = dist;
          hitEnemy = en;
        }
      }
    });

    if (hitEnemy) {
      hitEnemy.hp -= 25;
      hitEnemy.alert = true;
      playSound('hit');
      spawnParticles(hitEnemy.x, hitEnemy.y, '#00f3ff');
      if (hitEnemy.hp <= 0) {
        hitEnemy.alive = false;
        player.score += 200;
        playSound('explode');
        spawnParticles(hitEnemy.x, hitEnemy.y, '#ff0055', 20);
        checkVictory();
      }
      updateHUD();
    }
  }

  function triggerReload(e) {
    if (e) e.preventDefault();
    if (player.reloading || player.ammo === 30 || player.maxAmmo <= 0) return;
    player.reloading = true;
    playSound('reload');
    setTimeout(() => {
      let needed = 30 - player.ammo;
      let take = Math.min(needed, player.maxAmmo);
      player.ammo += take;
      player.maxAmmo -= take;
      player.reloading = false;
      updateHUD();
    }, 1200);
  }

  function isWallBetween(x1, y1, x2, y2) {
    let steps = 15;
    for (let i = 1; i < steps; i++) {
      let t = i / steps;
      let cx = Math.floor(x1 + (x2 - x1) * t);
      let cy = Math.floor(y1 + (y2 - y1) * t);
      if (MAP[cy * MAP_W + cx] > 0) return true;
    }
    return false;
  }

  function spawnParticles(x, y, color, count = 8) {
    for (let i = 0; i < count; i++) {
      particles.push({
        x: x, y: y,
        vx: (Math.random() - 0.5) * 2,
        vy: (Math.random() - 0.5) * 2,
        life: 1.0, color: color
      });
    }
  }

  function updateHUD() {
    document.getElementById('txtScore').innerText = player.score;
    document.getElementById('txtEnemies').innerText = enemies.filter(e => e.alive).length;
    document.getElementById('txtHp').innerText = player.hp;
    document.getElementById('barHp').style.width = Math.max(0, player.hp) + '%';
    document.getElementById('txtShield').innerText = player.shield;
    document.getElementById('barShield').style.width = Math.max(0, player.shield) + '%';
    document.getElementById('txtAmmo').innerText = player.reloading ? "RELOAD" : player.ammo;
    document.getElementById('txtAmmoMax').innerText = player.maxAmmo;
  }

  function checkVictory() {
    if (enemies.every(e => !e.alive)) {
      isRunning = false;
      document.getElementById('endTitle').innerText = "MISSION ACCOMPLIE !";
      document.getElementById('endTitle').style.color = "var(--green)";
      document.getElementById('endDesc').innerText = "Secteur sécurisé. Tous les drones sont éliminés.";
      document.getElementById('endScoreText').innerText = player.score;
      document.getElementById('endScreen').style.display = 'flex';
    }
  }

  function playerDamage(amount) {
    const flash = document.getElementById('damageFlash');
    flash.style.opacity = '1';
    setTimeout(() => { flash.style.opacity = '0'; }, 100);

    if (player.shield > 0) {
      player.shield -= amount;
      if (player.shield < 0) { player.hp += player.shield; player.shield = 0; }
    } else {
      player.hp -= amount;
    }
    playSound('hit');
    updateHUD();

    if (player.hp <= 0) {
      player.hp = 0;
      isRunning = false;
      document.getElementById('endTitle').innerText = "MISSION ÉCHOUÉE";
      document.getElementById('endTitle').style.color = "var(--pink)";
      document.getElementById('endDesc').innerText = "Votre signal vital s'est éteint.";
      document.getElementById('endScoreText').innerText = player.score;
      document.getElementById('endScreen').style.display = 'flex';
    }
  }

  function gameLoop(time) {
    if (!isRunning) return;
    const dt = Math.min((time - lastTime) / 1000, 0.1);
    lastTime = time;

    // Movement & Turning
    let moveSpeed = 3.2 * dt;
    let rotSpeed = 2.4 * dt;

    // Rotation from stick or keys
    if (keys['ArrowLeft'] || keys['KeyA']) player.dir -= rotSpeed;
    if (keys['ArrowRight'] || keys['KeyD']) player.dir += rotSpeed;
    if (stickR.active) {
      let rdx = (stickR.curX - stickR.startX) / 45;
      player.dir += rdx * rotSpeed * 1.5;
    }

    // Forward/Backward & Strafe
    let dx = 0, dy = 0;
    if (keys['KeyW'] || keys['ArrowUp']) { dx += Math.cos(player.dir); dy += Math.sin(player.dir); }
    if (keys['KeyS'] || keys['ArrowDown']) { dx -= Math.cos(player.dir); dy -= Math.sin(player.dir); }

    if (stickL.active) {
      let ldx = (stickL.curX - stickL.startX) / 45;
      let ldy = (stickL.curY - stickL.startY) / 45;
      // Forward vector + Strafe vector
      dx += Math.cos(player.dir) * (-ldy) + Math.cos(player.dir + Math.PI/2) * ldx;
      dy += Math.sin(player.dir) * (-ldy) + Math.sin(player.dir + Math.PI/2) * ldx;
    }

    if (dx !== 0 || dy !== 0) {
      let len = Math.sqrt(dx*dx + dy*dy);
      dx = (dx/len) * moveSpeed;
      dy = (dy/len) * moveSpeed;
      let newX = player.x + dx;
      let newY = player.y + dy;
      if (MAP[Math.floor(player.y) * MAP_W + Math.floor(newX)] === 0) player.x = newX;
      if (MAP[Math.floor(newY) * MAP_W + Math.floor(player.x)] === 0) player.y = newY;
      weaponBob += dt * 10;
    }

    // Enemies AI & Drone behaviors
    enemies.forEach(en => {
      if (!en.alive) return;
      let dist = Math.hypot(player.x - en.x, player.y - en.y);
      if (dist < 7 && !isWallBetween(player.x, player.y, en.x, en.y)) {
        en.alert = true;
      }
      if (en.alert) {
        let angle = Math.atan2(player.y - en.y, player.x - en.x);
        let ex = Math.cos(angle) * dt * 1.2;
        let ey = Math.sin(angle) * dt * 1.2;
        if (dist > 2.5) {
          if (MAP[Math.floor(en.y) * MAP_W + Math.floor(en.x + ex)] === 0) en.x += ex;
          if (MAP[Math.floor(en.y + ey) * MAP_W + Math.floor(en.x)] === 0) en.y += ey;
        }
        en.shootTimer += dt;
        if (en.shootTimer > 2.0 && dist < 6) {
          en.shootTimer = 0;
          playerDamage(15);
        }
      }
    });

    // Raycast Rendering 3D
    render3D();
    renderMinimap();
    renderWeapon(dt);

    requestAnimationFrame(gameLoop);
  }

  function render3D() {
    const w = canvas3D.width;
    const h = canvas3D.height;
    const halfH = h / 2;

    // Gradient Ceiling & Floor
    const skyGrad = ctx3D.createLinearGradient(0, 0, 0, halfH);
    skyGrad.addColorStop(0, '#020308');
    skyGrad.addColorStop(1, '#0e1526');
    ctx3D.fillStyle = skyGrad;
    ctx3D.fillRect(0, 0, w, halfH);

    const floorGrad = ctx3D.createLinearGradient(0, halfH, 0, h);
    floorGrad.addColorStop(0, '#0a0d18');
    floorGrad.addColorStop(1, '#03050a');
    ctx3D.fillStyle = floorGrad;
    ctx3D.fillRect(0, halfH, w, halfH);

    const numRays = w / 2;
    const stepAngle = player.fov / numRays;
    let startAngle = player.dir - player.fov / 2;
    let zBuffer = [];

    for (let i = 0; i < numRays; i++) {
      let rayAngle = startAngle + i * stepAngle;
      let cosA = Math.cos(rayAngle);
      let sinA = Math.sin(rayAngle);
      let dist = 0;
      let hitWall = 0;
      let side = 0;

      while (dist < 16) {
        dist += 0.05;
        let testX = Math.floor(player.x + cosA * dist);
        let testY = Math.floor(player.y + sinA * dist);
        if (testX < 0 || testX >= MAP_W || testY < 0 || testY >= MAP_H) {
          hitWall = 1; break;
        }
        let tile = MAP[testY * MAP_W + testX];
        if (tile > 0) { hitWall = tile; break; }
      }

      // Fisheye correction
      let correctDist = dist * Math.cos(rayAngle - player.dir);
      zBuffer.push(correctDist);

      let wallHeight = (h / correctDist);
      let wallTop = halfH - wallHeight / 2;

      // Color shading based on distance and tile type
      let shade = Math.max(0.1, 1 - (correctDist / 12));
      let wallColor = '#00f3ff';
      if (hitWall === 2) wallColor = '#ff0055';
      if (hitWall === 3) wallColor = '#a855f7';

      ctx3D.fillStyle = shadeColor(wallColor, shade);
      ctx3D.fillRect(i * 2, wallTop, 2, wallHeight);
    }

    // Render Enemies (Sprites sorted by depth)
    let sortedEnemies = enemies.map((en, idx) => {
      let dx = en.x - player.x;
      let dy = en.y - player.y;
      let dist = Math.hypot(dx, dy);
      let angle = Math.atan2(dy, dx) - player.dir;
      while (angle > Math.PI) angle -= Math.PI*2;
      while (angle < -Math.PI) angle += Math.PI*2;
      return { en, dist, angle };
    }).filter(s => s.en.alive && s.dist > 0.5 && Math.abs(s.angle) < player.fov)
      .sort((a, b) => b.dist - a.dist);

    sortedEnemies.forEach(s => {
      let screenX = (halfH * Math.tan(s.angle) / Math.tan(player.fov/2)) + (w / 2);
      let spriteHeight = (h / s.dist) * 0.7;
      let spriteWidth = spriteHeight * 0.8;
      let topY = halfH - spriteHeight / 2;

      let rayIdx = Math.floor((screenX / w) * numRays);
      if (rayIdx >= 0 && rayIdx < zBuffer.length && s.dist < zBuffer[rayIdx]) {
        // Draw Cyber Drone
        ctx3D.save();
        ctx3D.translate(screenX, halfH);
        let sShade = Math.max(0.2, 1 - (s.dist / 10));

        // Glow ring
        ctx3D.strokeStyle = `rgba(255, 0, 85, ${sShade * 0.8})`;
        ctx3D.lineWidth = 3;
        ctx3D.beginPath();
        ctx3D.arc(0, 0, spriteWidth * 0.45, 0, Math.PI * 2);
        ctx3D.stroke();

        // Core eye
        ctx3D.fillStyle = `rgba(0, 243, 255, ${sShade})`;
        ctx3D.beginPath();
        ctx3D.arc(0, 0, spriteWidth * 0.22, 0, Math.PI * 2);
        ctx3D.fill();

        // Health bar above drone
        ctx3D.fillStyle = 'rgba(0,0,0,0.6)';
        ctx3D.fillRect(-spriteWidth * 0.4, -spriteHeight * 0.6, spriteWidth * 0.8, 6);
        ctx3D.fillStyle = '#ff0055';
        ctx3D.fillRect(-spriteWidth * 0.4, -spriteHeight * 0.6, (spriteWidth * 0.8) * (s.en.hp / 50), 6);

        ctx3D.restore();
      }
    });
  }

  function renderMinimap() {
    mmCtx.clearRect(0, 0, 90, 90);
    const cellW = 90 / MAP_W;
    const cellH = 90 / MAP_H;

    for (let y = 0; y < MAP_H; y++) {
      for (let x = 0; x < MAP_W; x++) {
        if (MAP[y * MAP_W + x] > 0) {
          mmCtx.fillStyle = 'rgba(0, 243, 255, 0.4)';
          mmCtx.fillRect(x * cellW, y * cellH, cellW, cellH);
        }
      }
    }

    // Enemies
    enemies.forEach(en => {
      if (!en.alive) return;
      mmCtx.fillStyle = '#ff0055';
      mmCtx.beginPath();
      mmCtx.arc(en.x * cellW, en.y * cellH, 2.5, 0, Math.PI*2);
      mmCtx.fill();
    });

    // Player
    mmCtx.fillStyle = '#ffe600';
    mmCtx.beginPath();
    mmCtx.arc(player.x * cellW, player.y * cellH, 3, 0, Math.PI*2);
    mmCtx.fill();

    mmCtx.strokeStyle = '#ffe600';
    mmCtx.lineWidth = 1.5;
    mmCtx.beginPath();
    mmCtx.moveTo(player.x * cellW, player.y * cellH);
    mmCtx.lineTo((player.x + Math.cos(player.dir)*1.8) * cellW, (player.y + Math.sin(player.dir)*1.8) * cellH);
    mmCtx.stroke();
  }

  function renderWeapon(dt) {
    wpCtx.clearRect(0, 0, 280, 220);
    weaponRecoil = Math.max(0, weaponRecoil - dt * 80);
    let bobX = Math.cos(weaponBob) * 6;
    let bobY = Math.abs(Math.sin(weaponBob)) * 8 + weaponRecoil;

    let cx = 140 + bobX;
    let cy = 180 + bobY;

    // Cyber Blaster Body
    wpCtx.save();
    wpCtx.translate(cx, cy);

    // Muzzle Flash
    if (weaponRecoil > 12) {
      wpCtx.fillStyle = '#00f3ff';
      wpCtx.shadowColor = '#00f3ff';
      wpCtx.shadowBlur = 30;
      wpCtx.beginPath();
      wpCtx.arc(0, -90, 28, 0, Math.PI*2);
      wpCtx.fill();
      wpCtx.shadowBlur = 0;
    }

    // Gun Barrel & Chassis
    wpCtx.fillStyle = '#161c2e';
    wpCtx.fillRect(-22, -80, 44, 100);

    wpCtx.fillStyle = '#0e1220';
    wpCtx.fillRect(-28, -20, 56, 70);

    // Cyan Glow Trim
    wpCtx.fillStyle = '#00f3ff';
    wpCtx.fillRect(-4, -75, 8, 60);

    // Energy Core
    wpCtx.fillStyle = '#ff0055';
    wpCtx.beginPath();
    wpCtx.arc(0, 10, 10, 0, Math.PI*2);
    wpCtx.fill();

    wpCtx.restore();
  }

  function shadeColor(color, percent) {
    let num = parseInt(color.replace("#",""), 16),
    amt = Math.round(2.55 * (percent * 100 - 100)),
    R = (num >> 16) + amt,
    G = (num >> 8 & 0x00FF) + amt,
    B = (num & 0x0000FF) + amt;
    return "#" + (0x1000000 + (R<255?R<1?0:R:255)*0x10000 + (G<255?G<1?0:G:255)*0x100 + (B<255?B<1?0:B:255)).toString(16).slice(1);
  }
</script>
</body>
</html>
)rawliteral";

#endif
