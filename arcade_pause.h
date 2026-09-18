#ifndef ARCADE_PAUSE_H
#define ARCADE_PAUSE_H

#include <pgmspace.h>

const char ARCADE_PAUSE_JS[] PROGMEM = R"rawliteral(
// ====================================================================
// ESP32 ARCADE STATION - CONTRÔLEUR UNIVERSEL DE PAUSE POUR JEUX SOLO
// ====================================================================
(function() {
  let isPaused = false;
  const origRAF = window.requestAnimationFrame;

  // 1. Interception transparente de requestAnimationFrame
  window.requestAnimationFrame = function(cb) {
    return origRAF(function(time) {
      if (!isPaused) {
        cb(time);
      } else {
        origRAF(function waitResume(t) {
          if (!isPaused) cb(t);
          else origRAF(waitResume);
        });
      }
    });
  };

  // 2. Interception transparente de setInterval
  const origSetInterval = window.setInterval;
  window.setInterval = function(fn, delay, ...args) {
    return origSetInterval(function() {
      if (!isPaused) fn(...args);
    }, delay);
  };

  // 3. Construction de l'interface graphique du bouton et de l'overlay
  window.addEventListener('DOMContentLoaded', () => {
    // Bouton de pause stylisé
    const btn = document.createElement('button');
    btn.id = 'arcade-global-pause-btn';
    btn.innerHTML = '⏸️';
    btn.title = 'Mettre en pause (P / Échap)';
    btn.style.cssText = `
      width: 40px; height: 40px; border-radius: 10px;
      background: rgba(18, 22, 38, 0.9); border: 2px solid #00f3ff;
      color: #fff; font-size: 1.25rem; cursor: pointer;
      display: inline-flex; align-items: center; justify-content: center;
      box-shadow: 0 0 12px rgba(0, 243, 255, 0.4);
      transition: all 0.2s; z-index: 99999;
    `;

    // Modal Overlay de Pause
    const overlay = document.createElement('div');
    overlay.id = 'arcade-global-pause-overlay';
    overlay.style.cssText = `
      position: fixed; inset: 0; z-index: 100000;
      background: rgba(5, 7, 15, 0.88); backdrop-filter: blur(8px);
      display: none; flex-direction: column; align-items: center;
      justify-content: center; padding: 20px; font-family: system-ui, sans-serif;
    `;
    overlay.innerHTML = `
      <div style="
        background: rgba(22, 27, 46, 0.98); border: 2px solid #00f3ff;
        border-radius: 20px; padding: 26px 22px; width: 100%; max-width: 330px;
        text-align: center; box-shadow: 0 0 35px rgba(0, 243, 255, 0.35);
      ">
        <h2 style="font-size: 2.1rem; color: #00f3ff; margin-bottom: 6px; text-shadow: 0 0 12px #00f3ff;">⏸️ PAUSE</h2>
        <p style="font-size: 0.9rem; color: #8b9bb4; margin-bottom: 22px;">Partie momentanément suspendue</p>
        <button id="arcade-pause-resume" style="
          width: 100%; background: linear-gradient(135deg, #00f3ff, #0077ff);
          color: #000; font-weight: 800; border: none; padding: 14px;
          border-radius: 12px; font-size: 1.05rem; cursor: pointer; margin-bottom: 12px;
          box-shadow: 0 0 15px rgba(0,243,255,0.4);
        ">▶ REPRENDRE</button>
        <button id="arcade-pause-restart" style="
          width: 100%; background: rgba(255, 255, 255, 0.1);
          color: #fff; font-weight: 700; border: 1px solid rgba(255,255,255,0.25);
          padding: 12px; border-radius: 12px; font-size: 0.95rem; cursor: pointer; margin-bottom: 12px;
        ">🔄 RECOMMENCER</button>
        <a href="/hub" style="
          display: block; width: 100%; background: rgba(255, 0, 85, 0.15);
          color: #ff0055; font-weight: 700; border: 1px solid rgba(255,0,85,0.3);
          padding: 12px; border-radius: 12px; font-size: 0.95rem; text-decoration: none; box-sizing: border-box;
        ">🏠 QUITTER VERS LE SALON</a>
      </div>
    `;

    document.body.appendChild(overlay);

    // Positionnement intelligent dans le header du jeu à côté du bouton retour / menu
    const backBtn = document.querySelector('header a, .back-btn, .btn-exit, .btn-back');
    if (backBtn && backBtn.parentNode) {
      backBtn.parentNode.insertBefore(btn, backBtn.nextSibling);
      btn.style.marginLeft = '8px';
    } else {
      btn.style.position = 'fixed';
      btn.style.top = '10px';
      btn.style.right = '12px';
      document.body.appendChild(btn);
    }

    function togglePause() {
      isPaused = !isPaused;
      overlay.style.display = isPaused ? 'flex' : 'none';
      btn.innerHTML = isPaused ? '▶️' : '⏸️';
      btn.style.borderColor = isPaused ? '#00ff66' : '#00f3ff';
      if (window.audioCtx && window.audioCtx.state === 'running' && isPaused) {
        try { window.audioCtx.suspend(); } catch(e){}
      } else if (window.audioCtx && window.audioCtx.state === 'suspended' && !isPaused) {
        try { window.audioCtx.resume(); } catch(e){}
      }
    }

    btn.onclick = (e) => { e.preventDefault(); e.stopPropagation(); togglePause(); };
    document.getElementById('arcade-pause-resume').onclick = togglePause;
    document.getElementById('arcade-pause-restart').onclick = () => location.reload();

    // Raccourci clavier P ou Échap
    window.addEventListener('keydown', (e) => {
      if (e.key === 'p' || e.key === 'P' || e.key === 'Escape') {
        togglePause();
      }
    });
  });
})();
)rawliteral";

#endif
