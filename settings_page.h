#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include <pgmspace.h>

const char SETTINGS_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Paramètres & Mémoire - ESP32 Arcade</title>
  <style>
    :root {
      --bg-color: #0d0f18;
      --card-bg: rgba(26, 31, 51, 0.95);
      --neon-cyan: #00f3ff;
      --neon-pink: #ff0055;
      --neon-yellow: #ffe600;
      --neon-green: #00ff66;
      --text-main: #f0f4fc;
      --text-dim: #8b9bb4;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      background: var(--bg-color); color: var(--text-main); font-family: system-ui, sans-serif;
      min-height: 100vh; display: flex; flex-direction: column; align-items: center; padding: 16px;
    }
    .card {
      background: var(--card-bg); border: 1px solid rgba(0, 243, 255, 0.3); border-radius: 16px;
      padding: 22px; width: 100%; max-width: 440px; box-shadow: 0 10px 30px rgba(0,0,0,0.5);
    }
    h2 { color: var(--neon-cyan); margin-bottom: 4px; font-size: 1.35rem; }
    p.sub { color: var(--text-dim); font-size: 0.85rem; margin-bottom: 16px; }
    .section-title {
      color: var(--neon-yellow); font-size: 0.95rem; font-weight: bold;
      margin: 16px 0 8px; border-bottom: 1px solid rgba(255, 255, 255, 0.1); padding-bottom: 4px;
    }
    
    /* Memory Stats Box */
    .mem-box {
      background: rgba(9, 11, 18, 0.8); border: 1px solid rgba(0, 243, 255, 0.2);
      border-radius: 12px; padding: 14px; margin-bottom: 16px;
    }
    .mem-row { display: flex; justify-content: space-between; font-size: 0.85rem; margin-bottom: 6px; }
    .mem-val { font-weight: bold; color: var(--neon-cyan); }
    .mem-val.free { color: var(--neon-green); font-size: 1rem; }
    .progress-bar-bg { width: 100%; height: 10px; background: rgba(255,255,255,0.1); border-radius: 5px; overflow: hidden; margin: 8px 0; }
    .progress-bar-fill { height: 100%; background: linear-gradient(90deg, var(--neon-cyan), var(--neon-pink)); border-radius: 5px; width: %FLASH_PCT%%; }

    label { display: block; font-size: 0.8rem; color: var(--text-dim); margin-bottom: 4px; }
    input {
      width: 100%; background: #090b12; border: 1px solid rgba(255, 255, 255, 0.2);
      border-radius: 8px; padding: 10px 12px; color: #fff; font-size: 0.95rem; margin-bottom: 12px;
    }
    input:focus { border-color: var(--neon-cyan); outline: none; }
    .btn {
      width: 100%; background: linear-gradient(135deg, var(--neon-cyan), #0077ff);
      color: #000; border: none; padding: 13px; border-radius: 10px; font-weight: 800;
      font-size: 1rem; cursor: pointer; margin-top: 10px;
    }
    .btn-secondary {
      background: rgba(255, 255, 255, 0.08); color: #fff; text-decoration: none;
      display: block; text-align: center; line-height: 40px; margin-top: 10px; border-radius: 10px;
      font-weight: 600; font-size: 0.9rem;
    }
    .hint { font-size: 0.75rem; color: var(--text-dim); margin-top: -6px; margin-bottom: 12px; line-height: 1.3; }
  </style>
</head>
<body>
  <div class="card">
    <h2>⚙️ Paramètres & Mémoire</h2>
    <p class="sub">État de la puce ESP32, pseudo et réglages réseau</p>

    <!-- LIVE MEMORY DIAGNOSTIC -->
    <div class="section-title">📊 1. Espace Mémoire en Direct</div>
    <div class="mem-box">
      <div class="mem-row"><span>Mémoire Flash Totale :</span><span class="mem-val">%FLASH_TOTAL% Ko</span></div>
      <div class="mem-row"><span>Espace Utilisé (50 Jeux) :</span><span class="mem-val">%FLASH_USED% Ko (%FLASH_PCT%%)</span></div>
      <div class="progress-bar-bg"><div class="progress-bar-fill"></div></div>
      <div class="mem-row"><span>✨ Espace Flash Libre :</span><span class="mem-val free">%FLASH_FREE% Ko</span></div>
      <div class="mem-row" style="margin-top:4px;"><span>Mémoire Vive (RAM) Libre :</span><span class="mem-val" style="color:var(--neon-yellow);">%RAM_FREE% Ko</span></div>
    </div>

    <form action="/save_settings" method="POST">
      <!-- Section 2 : Pseudo du Joueur -->
      <div class="section-title">👤 2. Mon Pseudo de Joueur (Multijoueur)</div>
      <label>Nom ou pseudo affiché dans le salon et les matchs 1v1 :</label>
      <div style="display:flex; gap:8px; margin-bottom:4px;">
        <input type="text" id="player_pseudo" name="player_pseudo" maxlength="16" placeholder="Ex: Shadow, Alex, Ninja..." style="margin-bottom:0; flex:1;">
        <button type="button" id="btn-save-pseudo" style="padding:0 14px; background:linear-gradient(135deg,var(--neon-cyan),#0077ff); color:#000; border:none; border-radius:8px; font-weight:800; cursor:pointer; white-space:nowrap; font-size:0.85rem;">Enregistrer</button>
      </div>
      <div id="pseudo-feedback" style="display:none; font-size:0.8rem; color:var(--neon-green); font-weight:bold; margin-bottom:8px;">✅ Pseudo mis à jour avec succès !</div>
      <div class="hint">Ce pseudo est unique à votre smartphone et vous identifie auprès de vos adversaires dans le salon 1v1.</div>

      <!-- Section 3 : Mot de passe d'accès aux jeux -->
      <div class="section-title">🔑 3. Mot de Passe d'Accès aux Jeux</div>
      <label>Mot de passe d'accès :</label>
      <input type="text" name="access_pass" value="%ACCESS_PASS%" required minlength="4">
      <div class="hint">Code à entrer sur le smartphone pour déverrouiller l'arcade.</div>

      <!-- Section 4 : Wi-Fi du Téléphone -->
      <div class="section-title">📱 4. Partage de Connexion Smartphone</div>
      <label>Nom du Wi-Fi du téléphone (SSID) :</label>
      <input type="text" name="sta_ssid" value="%STA_SSID%" placeholder="Ex: Mon-Smartphone">
      
      <label>Mot de passe du hotspot :</label>
      <input type="password" name="sta_pass" value="%STA_PASS%" placeholder="Mot de passe du partage">

      <!-- Section 5 : Point d'accès de l'ESP32 -->
      <div class="section-title">📶 5. Point d'Accès de l'ESP32</div>
      <label>Nom du Wi-Fi ESP32 :</label>
      <input type="text" name="ap_ssid" value="%AP_SSID%" required>

      <label>Clé Wi-Fi ESP32 (Optionnel) :</label>
      <input type="text" name="ap_pass" value="%AP_PASS%" placeholder="Laisser vide pour réseau ouvert">

      <button type="submit" class="btn">💾 Enregistrer Toutes les Modifications</button>
      <a href="/hub" class="btn-secondary">◀ Retour au Salon</a>
    </form>
  </div>

  <script>
    const pseudoInput = document.getElementById('player_pseudo');
    const btnSavePseudo = document.getElementById('btn-save-pseudo');
    const pseudoFeedback = document.getElementById('pseudo-feedback');
    const form = document.querySelector('form');

    // Récupérer le pseudo actuel enregistré dans le téléphone
    const curPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    pseudoInput.value = curPseudo;

    function applyPseudoSave() {
      const val = pseudoInput.value.trim();
      if (val.length > 0) {
        localStorage.setItem('arcade_pseudo', val);
        pseudoFeedback.textContent = "✅ Pseudo enregistré : " + val;
        pseudoFeedback.style.display = "block";
        setTimeout(() => { pseudoFeedback.style.display = "none"; }, 3000);
      }
    }

    btnSavePseudo.addEventListener('click', applyPseudoSave);

    form.addEventListener('submit', function() {
      const val = pseudoInput.value.trim();
      if (val.length > 0) {
        localStorage.setItem('arcade_pseudo', val);
      }
    });
  </script>
</body>
</html>
)rawliteral";

#endif
