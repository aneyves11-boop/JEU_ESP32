#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include <pgmspace.h>

const char LOGIN_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Accès ESP32 Arcade Station</title>
  <style>
    :root {
      --bg: #070913;
      --card-bg: rgba(14, 18, 36, 0.90);
      --cyan: #00f3ff;
      --pink: #ff0055;
      --yellow: #ffe600;
      --green: #00ff66;
      --dim: #8b9bb4;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: var(--bg);
      color: #fff;
      font-family: system-ui, -apple-system, sans-serif;
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      padding: 16px;
      overflow: hidden;
      position: relative;
    }

    /* ANIMATED RETRO PERSPECTIVE GRID */
    .grid-plane {
      position: absolute;
      inset: 0;
      pointer-events: none;
      background: 
        linear-gradient(to bottom, transparent 60%, rgba(0, 243, 255, 0.15) 90%, rgba(255, 0, 85, 0.25) 100%),
        repeating-linear-gradient(0deg, rgba(0, 243, 255, 0.08) 0px, rgba(0, 243, 255, 0.08) 1px, transparent 1px, transparent 40px),
        repeating-linear-gradient(90deg, rgba(0, 243, 255, 0.08) 0px, rgba(0, 243, 255, 0.08) 1px, transparent 1px, transparent 40px);
      transform: perspective(350px) rotateX(62deg) translateY(120px) scale(2);
      transform-origin: bottom center;
      animation: gridMove 20s linear infinite;
    }
    @keyframes gridMove {
      0% { background-position: 0 0, 0 0, 0 0; }
      100% { background-position: 0 400px, 0 400px, 0 400px; }
    }

    /* AMBIENT RADIAL LIGHTS */
    .ambient-glow {
      position: absolute; width: 320px; height: 320px; border-radius: 50%;
      filter: blur(80px); pointer-events: none; opacity: 0.35;
    }
    .glow-cyan { top: -60px; left: -60px; background: var(--cyan); }
    .glow-pink { bottom: -60px; right: -60px; background: var(--pink); }

    /* ARCADE CABINET GLASS CARD */
    .card {
      position: relative;
      z-index: 10;
      background: var(--card-bg);
      backdrop-filter: blur(16px);
      -webkit-backdrop-filter: blur(16px);
      border: 1px solid rgba(0, 243, 255, 0.35);
      border-radius: 24px;
      padding: 34px 26px 26px;
      width: 100%;
      max-width: 380px;
      box-shadow: 
        0 20px 50px rgba(0, 0, 0, 0.8),
        0 0 35px rgba(0, 243, 255, 0.18),
        inset 0 1px 0 rgba(255, 255, 255, 0.2);
      text-align: center;
      overflow: hidden;
    }
    .card::before {
      content: '';
      position: absolute;
      top: 0; left: 0; right: 0; height: 3px;
      background: linear-gradient(90deg, var(--cyan), var(--pink), var(--yellow));
      box-shadow: 0 0 12px var(--cyan);
    }

    /* HEADER & BADGE */
    .arcade-badge {
      display: inline-flex; align-items: center; gap: 8px;
      padding: 5px 12px; border-radius: 20px;
      background: rgba(0, 243, 255, 0.1); border: 1px solid rgba(0, 243, 255, 0.35);
      color: var(--cyan); font-size: 0.75rem; font-weight: 800;
      text-transform: uppercase; letter-spacing: 1px; margin-bottom: 14px;
    }
    .pulse-dot {
      width: 8px; height: 8px; border-radius: 50%;
      background: var(--green); box-shadow: 0 0 8px var(--green);
      animation: pulse 1.5s infinite alternate;
    }
    @keyframes pulse { from { opacity: 0.4; } to { opacity: 1; } }

    .logo-container {
      margin-bottom: 8px;
      display: flex; justify-content: center; align-items: center; gap: 10px;
    }
    .logo-icon {
      font-size: 2.8rem;
      filter: drop-shadow(0 0 15px rgba(0, 243, 255, 0.6));
      animation: float 3s ease-in-out infinite;
    }
    @keyframes float {
      0%, 100% { transform: translateY(0); }
      50% { transform: translateY(-5px); }
    }

    h1 {
      font-size: 1.8rem;
      font-weight: 900;
      letter-spacing: 2px;
      background: linear-gradient(135deg, #fff 20%, var(--cyan) 70%, var(--pink) 100%);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
      margin-bottom: 4px;
      text-transform: uppercase;
    }
    p.subtitle {
      color: var(--dim);
      font-size: 0.86rem;
      line-height: 1.4;
      margin-bottom: 22px;
    }

    /* FORM STYLING */
    .form-group { text-align: left; margin-bottom: 18px; }
    label {
      display: block; font-size: 0.78rem; font-weight: 800;
      color: var(--dim); margin-bottom: 8px;
      text-transform: uppercase; letter-spacing: 1px;
    }
    .input-wrap {
      position: relative;
      display: flex; align-items: center;
    }
    .input-icon {
      position: absolute; left: 14px; font-size: 1.1rem;
      color: var(--dim); pointer-events: none;
    }
    input[type="password"], input[type="text"] {
      width: 100%;
      background: #090c18;
      border: 2px solid rgba(0, 243, 255, 0.25);
      border-radius: 14px;
      padding: 14px 16px 14px 44px;
      color: #fff;
      font-size: 1.15rem;
      letter-spacing: 3px;
      outline: none;
      transition: all 0.2s;
    }
    input:focus {
      border-color: var(--cyan);
      background: #0d1222;
      box-shadow: 0 0 20px rgba(0, 243, 255, 0.35);
    }

    .error-msg {
      background: rgba(255, 0, 85, 0.15);
      border: 1px solid var(--pink);
      color: #ff4d79;
      border-radius: 10px;
      padding: 10px;
      font-size: 0.85rem;
      font-weight: 700;
      margin-bottom: 16px;
      display: %ERROR_DISPLAY%;
    }

    /* TACTILE 3D ARCADE BUTTON */
    .btn-submit {
      width: 100%;
      background: linear-gradient(180deg, #00f3ff 0%, #0088ff 100%);
      color: #000;
      border: none;
      padding: 15px;
      border-radius: 14px;
      font-size: 1.05rem;
      font-weight: 900;
      letter-spacing: 1.5px;
      text-transform: uppercase;
      cursor: pointer;
      box-shadow: 0 6px 0 #0055aa, 0 10px 25px rgba(0, 243, 255, 0.4);
      transition: all 0.1s ease;
      display: flex; align-items: center; justify-content: center; gap: 8px;
    }
    .btn-submit:active {
      transform: translateY(4px);
      box-shadow: 0 2px 0 #0055aa, 0 5px 15px rgba(0, 243, 255, 0.4);
    }

    .footer-features {
      display: flex; justify-content: center; gap: 14px;
      margin-top: 22px; font-size: 0.75rem; color: var(--dim);
    }
    .feat-item { display: flex; align-items: center; gap: 4px; }
  </style>
</head>
<body>
  <div class="grid-plane"></div>
  <div class="ambient-glow glow-cyan"></div>
  <div class="ambient-glow glow-pink"></div>

  <div class="card">
    <div class="arcade-badge">
      <span class="pulse-dot"></span>
      <span>Station Hors-Ligne • 100% Flash</span>
    </div>

    <div class="logo-container">
      <div class="logo-icon">🕹️</div>
    </div>
    <h1>ESP32 Arcade</h1>
    <p class="subtitle">50 Jeux Solo & 7 Matchs Multijoueur 1v1</p>

    <div class="error-msg">
      ⚠️ Code incorrect ! Veuillez réessayer.
    </div>

    <form action="/login" method="POST">
      <div class="form-group">
        <label for="password">Code d'accès arcade</label>
        <div class="input-wrap">
          <span class="input-icon">🔑</span>
          <input type="password" id="password" name="password" placeholder="••••••••" autofocus required>
        </div>
      </div>
      <button type="submit" class="btn-submit">
        <span>DÉVERROUILLER</span>
        <span>🔓</span>
      </button>
    </form>

    <div class="footer-features">
      <span class="feat-item">👾 50 Jeux</span>
      <span>•</span>
      <span class="feat-item">⚔️ 1v1 Wi-Fi</span>
      <span>•</span>
      <span class="feat-item">⚡ 0 Latence</span>
    </div>
  </div>
</body>
</html>
)rawliteral";

#endif
