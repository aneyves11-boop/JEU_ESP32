#ifndef GAME_PACK_MP_MOTS_H
#define GAME_PACK_MP_MOTS_H

#include <pgmspace.h>

// ==================================================================================
//   ESP32 ARCADE - 5 JEUX MULTIJOUEURS DE BACCALAUREAT, MOTS & LETTRES (1V1 LOCAL WI-FI)
//   1. PETIT BAC CLASSIC  |  2. ANAGRAMME FLASH  |  3. BOMBE A MOTS
//   4. CHAINE DE MOTS     |  5. PENDU DUEL
// ==================================================================================

// ----------------------------------------------------------------------------------
//   1. LE PETIT BAC CLASSIC 1V1 (LETTRE SYNCHRO, 4 CATEGORIES & REVISION COMPARATIVE)
// ----------------------------------------------------------------------------------
const char MP_PETITBAC_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Le Petit Bac 1v1</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: radial-gradient(circle at 50% 20%, #0d1633 0%, #050813 100%);
      color: #fff; font-family: system-ui, -apple-system, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: flex-start;
      min-height: 100vh; padding: 12px; overflow-x: hidden; touch-action: manipulation;
    }
    header { width: 100%; max-width: 420px; display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
    .btn-hub {
      background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2);
      color: #00f3ff; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700;
    }
    .status-badge {
      font-size: 0.8rem; font-weight: 800; padding: 4px 12px; border-radius: 20px;
      background: rgba(0,243,255,0.15); color: #00f3ff; border: 1px solid #00f3ff;
    }
    .letter-banner {
      width: 100%; max-width: 420px; background: rgba(255,255,255,0.04);
      border: 2px solid rgba(0,243,255,0.3); border-radius: 18px; padding: 12px 16px;
      display: flex; justify-content: space-between; align-items: center; margin-bottom: 12px;
      box-shadow: 0 8px 25px rgba(0,0,0,0.6);
    }
    .letter-box {
      width: 56px; height: 56px; border-radius: 14px;
      background: radial-gradient(circle, #00f3ff, #0066cc); color: #000;
      font-size: 2.2rem; font-weight: 900; display: flex; align-items: center; justify-content: center;
      box-shadow: 0 0 20px rgba(0,243,255,0.6);
    }
    .info-box { display: flex; flex-direction: column; text-align: left; gap: 2px; }
    .info-title { font-size: 0.8rem; color: #8b9bb4; font-weight: 700; text-transform: uppercase; }
    .timer-val { font-size: 1.4rem; font-weight: 900; color: #ffe600; }
    .timer-alert { color: #ff0055 !important; animation: blink 0.5s infinite; }
    @keyframes blink { 50% { opacity: 0.3; } }

    /* Forms */
    .form-wrap { width: 100%; max-width: 420px; display: flex; flex-direction: column; gap: 10px; }
    .cat-row {
      background: rgba(14,22,48,0.7); border: 1.5px solid rgba(255,255,255,0.1); border-radius: 14px;
      padding: 10px 14px; display: flex; flex-direction: column; gap: 4px;
      box-shadow: inset 0 2px 5px rgba(0,0,0,0.5);
    }
    .cat-label { font-size: 0.8rem; font-weight: 800; color: #00f3ff; display: flex; align-items: center; gap: 6px; }
    .cat-input {
      background: rgba(0,0,0,0.4); border: 1px solid rgba(255,255,255,0.2); border-radius: 10px;
      color: #fff; font-size: 1rem; font-weight: 700; padding: 8px 12px; outline: none;
      user-select: text; -webkit-user-select: text;
    }
    .cat-input:focus { border-color: #00f3ff; box-shadow: 0 0 10px rgba(0,243,255,0.3); }

    .btn-stop {
      margin-top: 12px; width: 100%; max-width: 420px; padding: 14px; border-radius: 16px;
      background: linear-gradient(180deg, #ff0055, #bb0033); color: #fff; font-size: 1.2rem; font-weight: 900;
      border: none; cursor: pointer; box-shadow: 0 6px 20px rgba(255,0,85,0.5); transition: transform 0.1s;
    }
    .btn-stop:active { transform: scale(0.96); }

    /* Results overlay */
    #review-wrap {
      display: none; width: 100%; max-width: 420px; flex-direction: column; gap: 10px; margin-top: 10px;
    }
    .review-card {
      background: rgba(14,22,48,0.9); border: 1px solid rgba(0,243,255,0.3); border-radius: 14px;
      padding: 10px 14px; text-align: left;
    }
    .review-card h4 { color: #ffe600; font-size: 0.85rem; margin-bottom: 6px; }
    .review-answers { display: grid; grid-template-columns: 1fr 1fr; gap: 8px; font-size: 0.85rem; }
    .ans-box { padding: 6px; border-radius: 8px; background: rgba(0,0,0,0.4); word-break: break-word; }
    .ans-p1 { border-left: 3px solid #00f3ff; }
    .ans-p2 { border-left: 3px solid #ff0055; }

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
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 20px rgba(0,243,255,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="status-badge" id="badge">Connexion...</span>
  </header>

  <div class="letter-banner">
    <div class="letter-box" id="cur-letter">?</div>
    <div class="info-box">
      <div class="info-title">LETTRE IMPOSÉE</div>
      <div style="font-size:0.9rem; font-weight:800; color:#fff;" id="opp-status">Attente joueur...</div>
    </div>
    <div class="info-box" style="text-align:right;">
      <div class="info-title">CHRONO</div>
      <div class="timer-val" id="timer-val">--</div>
    </div>
  </div>

  <div class="form-wrap" id="inputs-wrap">
    <div class="cat-row">
      <div class="cat-label">👤 PRÉNOM</div>
      <input type="text" class="cat-input" id="in-prenom" placeholder="Ex: Lucas, Marie..." autocomplete="off">
    </div>
    <div class="cat-row">
      <div class="cat-label">🌍 VILLE OU PAYS</div>
      <input type="text" class="cat-input" id="in-ville" placeholder="Ex: Lyon, Canada..." autocomplete="off">
    </div>
    <div class="cat-row">
      <div class="cat-label">🦁 ANIMAL</div>
      <input type="text" class="cat-input" id="in-animal" placeholder="Ex: Lion, Panda..." autocomplete="off">
    </div>
    <div class="cat-row">
      <div class="cat-label">📦 OBJET OU MÉTIER</div>
      <input type="text" class="cat-input" id="in-objet" placeholder="Ex: Lampe, Pompier..." autocomplete="off">
    </div>
    <button class="btn-stop" id="btn-stop">🛑 STOP ! (J'AI FINI)</button>
  </div>

  <div id="review-wrap">
    <h3 style="color:#00f3ff; margin-bottom:8px; font-weight:900;">📋 Comparatif des Réponses</h3>
    <div id="review-cards"></div>
    <div style="display:flex; gap:10px; margin-top:14px; justify-content:center;">
      <button class="act-btn btn-rematch" id="btn-rematch" style="max-width:200px;">🔄 Manche Suivante</button>
      <a href="/hub" class="act-btn btn-quit" style="max-width:140px;">◀ Salon</a>
    </div>
  </div>

  <div id="overlay">
    <h2 id="ov-title">📝 Le Petit Bac 1v1</h2>
    <p id="ov-desc">En attente d'un adversaire sur son smartphone...</p>
    <div id="ov-actions" style="display:none; flex-direction:column; align-items:center; width:100%;">
      <button id="btn-ov-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
      <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
    </div>
  </div>

  <script>
    const badge = document.getElementById('badge');
    const curLetter = document.getElementById('cur-letter');
    const timerVal = document.getElementById('timer-val');
    const oppStatus = document.getElementById('opp-status');
    const btnStop = document.getElementById('btn-stop');
    const inputsWrap = document.getElementById('inputs-wrap');
    const reviewWrap = document.getElementById('review-wrap');
    const reviewCards = document.getElementById('review-cards');
    const btnRematch = document.getElementById('btn-rematch');
    const overlay = document.getElementById('overlay'), ovTitle = document.getElementById('ov-title'), ovDesc = document.getElementById('ov-desc');
    const ovActions = document.getElementById('ov-actions'), btnOvRematch = document.getElementById('btn-ov-rematch');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false, currentLetter = '';
    let oppPseudo = 'Adversaire';
    let timerInterval = null, timeLeft = 0;
    let myAnswers = {}, oppAnswers = {};
    let myRematch = false, oppRematch = false;

    const LETTERS = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'L', 'M', 'P', 'R', 'S', 'T', 'V'];

    function startNewRound(letter) {
      currentLetter = letter;
      curLetter.textContent = letter;
      document.getElementById('in-prenom').value = '';
      document.getElementById('in-ville').value = '';
      document.getElementById('in-animal').value = '';
      document.getElementById('in-objet').value = '';
      timerVal.textContent = 'En cours';
      timerVal.classList.remove('timer-alert');
      oppStatus.textContent = oppPseudo + ' réfléchit...';
      inputsWrap.style.display = 'flex';
      reviewWrap.style.display = 'none';
      overlay.style.display = 'none';
      ovActions.style.display = 'none';
      btnStop.disabled = false;
      btnStop.style.opacity = '1';
      myRematch = false; oppRematch = false;
      playing = true;
    }

    function triggerStopCountdown(starterPseudo) {
      btnStop.disabled = true;
      btnStop.style.opacity = '0.5';
      oppStatus.textContent = starterPseudo + ' a crié STOP !';
      timeLeft = 10;
      timerVal.textContent = timeLeft + 's';
      timerVal.classList.add('timer-alert');
      clearInterval(timerInterval);
      timerInterval = setInterval(() => {
        timeLeft--;
        if (timeLeft > 0) {
          timerVal.textContent = timeLeft + 's';
        } else {
          clearInterval(timerInterval);
          finishRound();
        }
      }, 1000);
    }

    btnStop.onclick = () => {
      if (!playing) return;
      collectAnswers();
      ws.send(JSON.stringify({ t: "bac_stop", name: myPseudo }));
      triggerStopCountdown("Vous");
    };

    function collectAnswers() {
      myAnswers = {
        prenom: document.getElementById('in-prenom').value.trim(),
        ville: document.getElementById('in-ville').value.trim(),
        animal: document.getElementById('animal' ? 'in-animal' : 'in-animal').value.trim(),
        objet: document.getElementById('in-objet').value.trim()
      };
    }

    function finishRound() {
      playing = false;
      collectAnswers();
      ws.send(JSON.stringify({ t: "bac_answers", answers: myAnswers }));
      inputsWrap.style.display = 'none';
      showReviewUI();
    }

    function showReviewUI() {
      const cats = [
        { id: 'prenom', label: '👤 Prénom' },
        { id: 'ville', label: '🌍 Ville / Pays' },
        { id: 'animal', label: '🦁 Animal' },
        { id: 'objet', label: '📦 Objet / Métier' }
      ];

      reviewCards.innerHTML = '';
      cats.forEach(c => {
        const card = document.createElement('div');
        card.className = 'review-card';
        const myA = myAnswers[c.id] || '(Vide)';
        const oppA = oppAnswers[c.id] || '(En attente...)';
        card.innerHTML = `
          <h4>${c.label} (Lettre ${currentLetter})</h4>
          <div class="review-answers">
            <div class="ans-box ans-p1"><b>${myPseudo} :</b><br>${escapeHtml(myA)}</div>
            <div class="ans-box ans-p2"><b>${oppPseudo} :</b><br>${escapeHtml(oppA)}</div>
          </div>
        `;
        reviewCards.appendChild(card);
      });
      reviewWrap.style.display = 'flex';
    }

    function escapeHtml(text) {
      const div = document.createElement('div');
      div.textContent = text;
      return div.innerHTML;
    }

    function requestRematch() {
      myRematch = true;
      btnRematch.textContent = "⏳ En attente...";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) {
        // Host picks new letter
        if (role === 1) {
          const l = LETTERS[Math.floor(Math.random() * LETTERS.length)];
          ws.send(JSON.stringify({ t: "bac_new_letter", letter: l }));
          startNewRound(l);
        }
      }
    }

    btnRematch.onclick = requestRematch;
    btnOvRematch.onclick = requestRematch;

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => {
      badge.textContent = "Recherche...";
      ws.send(JSON.stringify({ t: "join", g: "petitbac", name: myPseudo }));
    };

    ws.onmessage = (e) => {
      try {
        const d = JSON.parse(e.data);
        if (d.t === "joined") {
          role = d.role || d.p;
          badge.textContent = role === 1 ? "🔵 Joueur 1" : "🔴 Joueur 2";
          ovTitle.textContent = role === 1 ? "🔵 Joueur 1 Prêt" : "🔴 Joueur 2 Prêt";
          ovDesc.textContent = "En attente du second joueur...";
        } else if (d.t === "start") {
          role = d.role || role || 1;
          oppPseudo = d.opp || 'Adversaire';
          badge.textContent = "⚡ En Direct";
          badge.style.color = "#00ff66"; badge.style.borderColor = "#00ff66";
          if (role === 1) {
            // Pick random letter and broadcast
            const l = LETTERS[Math.floor(Math.random() * LETTERS.length)];
            setTimeout(() => {
              ws.send(JSON.stringify({ t: "bac_new_letter", letter: l }));
              startNewRound(l);
            }, 300);
          }
        } else if (d.t === "bac_new_letter") {
          startNewRound(d.letter);
        } else if (d.t === "bac_stop") {
          triggerStopCountdown(d.name || oppPseudo);
        } else if (d.t === "bac_answers") {
          oppAnswers = d.answers || {};
          showReviewUI();
        } else if (d.t === "rematch") {
          oppRematch = true;
          if (myRematch && role === 1) {
            const l = LETTERS[Math.floor(Math.random() * LETTERS.length)];
            ws.send(JSON.stringify({ t: "bac_new_letter", letter: l }));
            startNewRound(l);
          }
        } else if (d.t === "opp_left") {
          playing = false;
          badge.textContent = "Adversaire parti";
          badge.style.color = "#ff0055";
        }
      } catch(err){}
    };
  </script>
</body>
</html>

)rawliteral";

// ----------------------------------------------------------------------------------
//   2. ANAGRAMME FLASH 1V1 (TUILES 3D, DICTIONNAIRE FRANCAIS & COURSE AU SCORE)
// ----------------------------------------------------------------------------------
const char MP_ANAGRAM_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Course aux Anagrammes 1v1</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: radial-gradient(circle at 50% 30%, #0c1530 0%, #030611 100%);
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
    .player-card.p1 { color: #00f3ff; }
    .player-card.p2 { color: #ff0055; }
    .p-name { font-size: 0.85rem; font-weight: 800; max-width: 110px; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
    .p-score { font-size: 1.6rem; font-weight: 900; }
    .vs-pill { font-size: 0.85rem; font-weight: 900; color: #ffe600; padding: 4px 10px; background: rgba(255,230,0,0.1); border-radius: 12px; border: 1px solid rgba(255,230,0,0.3); }

    .tile-container {
      display: flex; flex-wrap: wrap; gap: 8px; justify-content: center; max-width: 360px; margin: 20px 0;
    }
    .letter-tile {
      width: 46px; height: 50px; background: linear-gradient(180deg, #1f2f58, #0e172e);
      border: 2px solid #00f3ff; border-radius: 12px; font-size: 1.8rem; font-weight: 900;
      display: flex; align-items: center; justify-content: center; color: #00f3ff;
      box-shadow: 0 6px 15px rgba(0,0,0,0.6), 0 0 10px rgba(0,243,255,0.2);
      animation: popTile 0.3s ease-out;
    }
    @keyframes popTile { from { transform: scale(0.5); opacity: 0; } to { transform: scale(1); opacity: 1; } }

    .input-zone { width: 100%; max-width: 360px; display: flex; gap: 8px; }
    .word-in {
      flex: 1; background: rgba(0,0,0,0.5); border: 2px solid rgba(255,255,255,0.2); border-radius: 14px;
      color: #fff; font-size: 1.2rem; font-weight: 900; padding: 10px 14px; text-transform: uppercase;
      outline: none; text-align: center; letter-spacing: 2px;
      user-select: text; -webkit-user-select: text;
    }
    .word-in:focus { border-color: #00f3ff; box-shadow: 0 0 15px rgba(0,243,255,0.4); }
    .btn-send {
      padding: 0 20px; background: linear-gradient(180deg, #00f3ff, #0066cc); border: none;
      border-radius: 14px; color: #000; font-size: 1.1rem; font-weight: 900; cursor: pointer;
    }

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
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 20px rgba(0,243,255,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
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
    <div class="vs-pill">1er à 3 pts</div>
    <div class="player-card p2">
      <div class="p-name" id="p2-name">Joueur 2</div>
      <div class="p-score" id="s2">0</div>
    </div>
  </div>

  <div>
    <div style="font-size:0.85rem; color:#8b9bb4; font-weight:800; text-transform:uppercase;">Reconstituez le mot :</div>
    <div class="tile-container" id="tile-container"></div>
  </div>

  <div class="input-zone">
    <input type="text" class="word-in" id="word-in" placeholder="VOTRE MOT..." autocomplete="off">
    <button class="btn-send" id="btn-send">OK</button>
  </div>

  <div id="overlay">
    <h2 id="ov-title">🔤 Anagrammes 1v1</h2>
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
    const tileContainer = document.getElementById('tile-container');
    const wordIn = document.getElementById('word-in'), btnSend = document.getElementById('btn-send');
    const overlay = document.getElementById('overlay'), ovTitle = document.getElementById('ov-title'), ovDesc = document.getElementById('ov-desc');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false, targetWord = '', scrambled = '';
    let score1 = 0, score2 = 0;
    let myRematch = false, oppRematch = false;

    const WORDS = [
      "ARCADE", "CANARD", "CASTEL", "CHEVAL", "SOLEIL", "PLANETE", "MYSTERE",
      "GAUFRE", "BATEAU", "DRAGON", "TRESOR", "COUTEAU", "JARDIN", "LUMIERE",
      "NAVIGUER", "GUITARE", "POISSON", "VALISE", "BOMBARD", "CHAMPION",
      "PAPILLON", "BOUCLIER", "HORLOGE", "POMPIER", "ROBOTIQUE"
    ];

    function scramble(w) {
      let arr = w.split('');
      for (let i = arr.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [arr[i], arr[j]] = [arr[j], arr[i]];
      }
      if (arr.join('') === w) return scramble(w);
      return arr.join('');
    }

    function setWord(w, sc) {
      targetWord = w;
      scrambled = sc;
      tileContainer.innerHTML = '';
      sc.split('').forEach(ch => {
        const t = document.createElement('div');
        t.className = 'letter-tile';
        t.textContent = ch;
        tileContainer.appendChild(t);
      });
      wordIn.value = '';
      wordIn.focus();
    }

    function submitWord() {
      if (!playing) return;
      const guess = wordIn.value.trim().toUpperCase();
      if (guess === targetWord) {
        // Point scored!
        ws.send(JSON.stringify({ t: "anag_win", p: role, word: targetWord }));
        handleRoundWin(role);
      } else {
        wordIn.style.borderColor = '#ff0055';
        setTimeout(() => wordIn.style.borderColor = '', 400);
      }
    }

    btnSend.onclick = submitWord;
    wordIn.onkeydown = (e) => { if (e.key === 'Enter') submitWord(); };

    function handleRoundWin(winnerRole) {
      if (winnerRole === 1) score1++; else score2++;
      s1El.textContent = score1; s2El.textContent = score2;

      if (score1 >= 3 || score2 >= 3) {
        playing = false;
        const isMe = (role === winnerRole);
        ovTitle.textContent = isMe ? "🏆 VICTOIRE !" : "💀 DÉFAITE...";
        ovTitle.style.color = isMe ? "#00ff66" : "#ff0055";
        ovDesc.textContent = isMe ? "Vous avez trouvé les anagrammes le plus rapidement !" : "L'adversaire a été plus vif !";
        ovActions.style.display = "flex";
        overlay.style.display = "flex";
      } else {
        ovTitle.textContent = (role === winnerRole) ? "⚡ BRAVO ! (+1 PT)" : "❌ TROUVÉ PAR L'AUTRE";
        ovTitle.style.color = (role === winnerRole) ? "#00ff66" : "#ff0055";
        ovDesc.textContent = "Le mot était : " + targetWord;
        ovActions.style.display = "none";
        overlay.style.display = "flex";
        if (role === 1) {
          setTimeout(() => {
            const nextW = WORDS[Math.floor(Math.random() * WORDS.length)];
            const nextSc = scramble(nextW);
            ws.send(JSON.stringify({ t: "anag_next", w: nextW, sc: nextSc }));
            setWord(nextW, nextSc);
            overlay.style.display = "none";
          }, 2000);
        }
      }
    }

    function resetGame() {
      score1 = 0; score2 = 0;
      s1El.textContent = "0"; s2El.textContent = "0";
      myRematch = false; oppRematch = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "";
      if (role === 1) {
        const nextW = WORDS[Math.floor(Math.random() * WORDS.length)];
        const nextSc = scramble(nextW);
        ws.send(JSON.stringify({ t: "anag_next", w: nextW, sc: nextSc }));
        setWord(nextW, nextSc);
      }
      overlay.style.display = "none";
      ovActions.style.display = "none";
      playing = true;
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      btnRematch.style.background = "rgba(255,255,255,0.2)";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) resetGame();
    };

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => {
      badge.textContent = "Recherche...";
      ws.send(JSON.stringify({ t: "join", g: "anagram", name: myPseudo }));
    };

    ws.onmessage = (e) => {
      try {
        const d = JSON.parse(e.data);
        if (d.t === "joined") {
          role = d.role || d.p;
          badge.textContent = role === 1 ? "🔵 Joueur 1" : "🔴 Joueur 2";
          ovTitle.textContent = role === 1 ? "🔵 Joueur 1 Prêt" : "🔴 Joueur 2 Prêt";
          ovDesc.textContent = "En attente du second joueur...";
        } else if (d.t === "start") {
          role = d.role || role || 1;
          badge.textContent = "⚡ En Direct";
          badge.style.color = "#00ff66"; badge.style.borderColor = "#00ff66";
          if (d.opp) {
            if (role === 1) { p1Name.textContent = myPseudo; p2Name.textContent = d.opp; }
            else { p1Name.textContent = d.opp; p2Name.textContent = myPseudo; }
          }
          if (role === 1) {
            const w = WORDS[Math.floor(Math.random() * WORDS.length)];
            const sc = scramble(w);
            setTimeout(() => {
              ws.send(JSON.stringify({ t: "anag_next", w: w, sc: sc }));
              setWord(w, sc);
              overlay.style.display = "none";
              playing = true;
            }, 300);
          }
        } else if (d.t === "anag_next") {
          setWord(d.w, d.sc);
          overlay.style.display = "none";
          playing = true;
        } else if (d.t === "anag_win") {
          handleRoundWin(d.p);
        } else if (d.t === "rematch") {
          oppRematch = true;
          if (myRematch) resetGame();
        } else if (d.t === "opp_left") {
          playing = false;
          badge.textContent = "Adversaire parti";
          badge.style.color = "#ff0055";
        }
      } catch(err){}
    };
  </script>
</body>
</html>

)rawliteral";

// ----------------------------------------------------------------------------------
//   3. LA BOMBE A MOTS 1V1 (MECHE ANIMEE, SYLLABES CYBER & TIC-TAC SOUS PRESSION)
// ----------------------------------------------------------------------------------
const char MP_WORDBOMB_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Bombe à Mots 1v1</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: radial-gradient(circle at 50% 30%, #1a0808 0%, #050304 100%);
      color: #fff; font-family: system-ui, -apple-system, sans-serif;
      display: flex; flex-direction: column; align-items: center; justify-content: space-between;
      min-height: 100vh; padding: 14px; text-align: center; overflow: hidden; touch-action: manipulation;
    }
    header { width: 100%; max-width: 400px; display: flex; justify-content: space-between; align-items: center; }
    .btn-hub {
      background: rgba(255,255,255,0.08); border: 1px solid rgba(255,255,255,0.2);
      color: #ffaa00; padding: 6px 12px; border-radius: 8px; text-decoration: none; font-size: 0.82rem; font-weight: 700;
    }
    .status-badge {
      font-size: 0.8rem; font-weight: 800; padding: 4px 12px; border-radius: 20px;
      background: rgba(255,0,85,0.15); color: #ff0055; border: 1px solid #ff0055;
    }
    .bomb-arena {
      position: relative; width: 220px; height: 220px; display: flex; flex-direction: column;
      align-items: center; justify-content: center; margin: 10px 0;
    }
    .bomb-icon {
      font-size: 6.5rem; filter: drop-shadow(0 0 25px rgba(255,80,0,0.7));
      transition: transform 0.1s;
    }
    .fuse-spark {
      position: absolute; top: 12px; right: 54px; width: 14px; height: 14px;
      background: #ffe600; border-radius: 50%; box-shadow: 0 0 15px #ff5500, 0 0 25px #ffe600;
      animation: spark 0.15s infinite alternate;
    }
    @keyframes spark { from { transform: scale(0.8); opacity: 0.7; } to { transform: scale(1.4); opacity: 1; } }

    .syllable-badge {
      font-size: 2.2rem; font-weight: 900; color: #ffe600; text-shadow: 0 0 20px rgba(255,230,0,0.6);
      padding: 6px 20px; background: rgba(0,0,0,0.6); border: 2px solid #ff5500; border-radius: 16px;
      margin-top: -15px; z-index: 5;
    }
    .turn-label { font-size: 1.1rem; font-weight: 900; margin-top: 10px; }

    .timer-ring {
      font-size: 1.6rem; font-weight: 900; color: #ff0055; margin-top: 4px;
    }

    .input-zone { width: 100%; max-width: 360px; display: flex; gap: 8px; }
    .word-in {
      flex: 1; background: rgba(0,0,0,0.5); border: 2px solid rgba(255,255,255,0.2); border-radius: 14px;
      color: #fff; font-size: 1.2rem; font-weight: 900; padding: 10px 14px; text-transform: uppercase;
      outline: none; text-align: center; letter-spacing: 2px;
      user-select: text; -webkit-user-select: text;
    }
    .word-in:focus { border-color: #ff5500; box-shadow: 0 0 15px rgba(255,85,0,0.4); }
    .btn-send {
      padding: 0 20px; background: linear-gradient(180deg, #ff5500, #bb0000); border: none;
      border-radius: 14px; color: #fff; font-size: 1.1rem; font-weight: 900; cursor: pointer;
    }

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
    .btn-rematch { background: linear-gradient(180deg, #ff5500, #ff0055); color: #fff; box-shadow: 0 4px 20px rgba(255,0,85,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="status-badge" id="badge">Connexion...</span>
  </header>

  <div class="bomb-arena">
    <div class="fuse-spark"></div>
    <div class="bomb-icon" id="bomb-icon">💣</div>
    <div class="syllable-badge" id="syl-badge">CH</div>
  </div>

  <div>
    <div class="turn-label" id="turn-label">À vous de jouer !</div>
    <div class="timer-ring" id="timer-val">10s</div>
  </div>

  <div class="input-zone">
    <input type="text" class="word-in" id="word-in" placeholder="Contient cette syllabe..." autocomplete="off">
    <button class="btn-send" id="btn-send">PASSER</button>
  </div>

  <div id="overlay">
    <h2 id="ov-title">💣 Bombe à Mots 1v1</h2>
    <p id="ov-desc">En attente d'un adversaire sur son smartphone...</p>
    <div id="ov-actions" style="display:none; flex-direction:column; align-items:center; width:100%;">
      <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
      <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
    </div>
  </div>

  <script>
    const badge = document.getElementById('badge');
    const bombIcon = document.getElementById('bomb-icon');
    const sylBadge = document.getElementById('syl-badge');
    const turnLabel = document.getElementById('turn-label');
    const timerVal = document.getElementById('timer-val');
    const wordIn = document.getElementById('word-in'), btnSend = document.getElementById('btn-send');
    const overlay = document.getElementById('overlay'), ovTitle = document.getElementById('ov-title'), ovDesc = document.getElementById('ov-desc');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false, activeRole = 1;
    let currentSyllable = 'CH';
    let timeLeft = 10, maxTurnTime = 12;
    let timerInt = null;
    let myRematch = false, oppRematch = false;

    const SYLLABLES = ["CH", "ON", "AR", "TE", "PO", "MA", "IN", "QU", "BL", "TR", "VI", "ST", "RA", "LO", "DI", "OU", "AN"];

    function setTurn(r, syl) {
      activeRole = r;
      currentSyllable = syl;
      sylBadge.textContent = syl;
      timeLeft = maxTurnTime;
      timerVal.textContent = timeLeft + 's';
      wordIn.value = '';

      if (activeRole === role) {
        turnLabel.textContent = "⚡ C'EST VOTRE TOUR ! TROUVEZ UN MOT !";
        turnLabel.style.color = "#00ff66";
        wordIn.disabled = false;
        btnSend.disabled = false;
        wordIn.focus();
      } else {
        turnLabel.textContent = "⏳ Tour de l'adversaire...";
        turnLabel.style.color = "#8b9bb4";
        wordIn.disabled = true;
        btnSend.disabled = true;
      }

      clearInterval(timerInt);
      timerInt = setInterval(() => {
        if (!playing) return;
        timeLeft--;
        timerVal.textContent = timeLeft + 's';
        if (timeLeft <= 0) {
          clearInterval(timerInt);
          handleExplosion(activeRole);
        }
      }, 1000);
    }

    function handleExplosion(loserRole) {
      playing = false;
      const isMeLoser = (role === loserRole);
      bombIcon.textContent = "💥";
      ovTitle.textContent = isMeLoser ? "💥 BOOM ! TU AS EXPLOSÉ !" : "🏆 VICTOIRE !";
      ovTitle.style.color = isMeLoser ? "#ff0055" : "#00ff66";
      ovDesc.textContent = isMeLoser ? "Le temps est écoulé avec la bombe entre tes mains !" : "L'adversaire n'a pas trouvé de mot à temps !";
      ovActions.style.display = "flex";
      overlay.style.display = "flex";
    }

    function submitWord() {
      if (!playing || activeRole !== role) return;
      const w = wordIn.value.trim().toUpperCase();
      if (w.length >= 3 && w.includes(currentSyllable)) {
        // Word valid! Pass bomb
        maxTurnTime = Math.max(5, maxTurnTime - 1);
        const nextRole = (role === 1) ? 2 : 1;
        const nextSyl = SYLLABLES[Math.floor(Math.random() * SYLLABLES.length)];
        ws.send(JSON.stringify({ t: "bomb_pass", next: nextRole, syl: nextSyl }));
        setTurn(nextRole, nextSyl);
      } else {
        wordIn.style.borderColor = '#ff0055';
        setTimeout(() => wordIn.style.borderColor = '', 400);
      }
    }

    btnSend.onclick = submitWord;
    wordIn.onkeydown = (e) => { if (e.key === 'Enter') submitWord(); };

    function resetGame() {
      maxTurnTime = 12;
      bombIcon.textContent = "💣";
      myRematch = false; oppRematch = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "";
      overlay.style.display = "none";
      ovActions.style.display = "none";
      playing = true;
      if (role === 1) {
        const syl = SYLLABLES[Math.floor(Math.random() * SYLLABLES.length)];
        ws.send(JSON.stringify({ t: "bomb_start", syl: syl }));
        setTurn(1, syl);
      }
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      btnRematch.style.background = "rgba(255,255,255,0.2)";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) resetGame();
    };

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => {
      badge.textContent = "Recherche...";
      ws.send(JSON.stringify({ t: "join", g: "wordbomb", name: myPseudo }));
    };

    ws.onmessage = (e) => {
      try {
        const d = JSON.parse(e.data);
        if (d.t === "joined") {
          role = d.role || d.p;
          badge.textContent = role === 1 ? "🔵 Joueur 1" : "🔴 Joueur 2";
          ovTitle.textContent = role === 1 ? "🔵 Joueur 1 Prêt" : "🔴 Joueur 2 Prêt";
          ovDesc.textContent = "En attente du second joueur...";
        } else if (d.t === "start") {
          role = d.role || role || 1;
          badge.textContent = "⚡ En Direct";
          badge.style.color = "#00ff66"; badge.style.borderColor = "#00ff66";
          overlay.style.display = "none";
          playing = true;
          if (role === 1) {
            const syl = SYLLABLES[Math.floor(Math.random() * SYLLABLES.length)];
            setTimeout(() => {
              ws.send(JSON.stringify({ t: "bomb_start", syl: syl }));
              setTurn(1, syl);
            }, 300);
          }
        } else if (d.t === "bomb_start") {
          overlay.style.display = "none";
          playing = true;
          setTurn(1, d.syl);
        } else if (d.t === "bomb_pass") {
          setTurn(d.next, d.syl);
        } else if (d.t === "rematch") {
          oppRematch = true;
          if (myRematch) resetGame();
        } else if (d.t === "opp_left") {
          playing = false;
          badge.textContent = "Adversaire parti";
          badge.style.color = "#ff0055";
        }
      } catch(err){}
    };
  </script>
</body>
</html>

)rawliteral";

// ----------------------------------------------------------------------------------
//   4. CHAINE DE MOTS 1V1 (SHIRITORI FRANCAIS, FIL HISTORIQUE & CHRONO EN TENSION)
// ----------------------------------------------------------------------------------
const char MP_WORDCHAIN_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Chaîne de Mots 1v1</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: radial-gradient(circle at 50% 30%, #0d1b2a 0%, #050b12 100%);
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

    .chain-target-box {
      width: 100%; max-width: 360px; background: rgba(255,255,255,0.05); border: 2px solid rgba(0,243,255,0.3);
      border-radius: 18px; padding: 14px; margin: 10px 0;
    }
    .must-start { font-size: 0.8rem; color: #8b9bb4; font-weight: 800; text-transform: uppercase; margin-bottom: 4px; }
    .last-letter { font-size: 2.4rem; font-weight: 900; color: #ffe600; text-shadow: 0 0 15px rgba(255,230,0,0.6); }

    .history-feed {
      width: 100%; max-width: 360px; height: 160px; overflow-y: auto; background: rgba(0,0,0,0.4);
      border-radius: 14px; padding: 10px; display: flex; flex-direction: column; gap: 6px;
      border: 1px solid rgba(255,255,255,0.1);
    }
    .feed-item { font-size: 0.9rem; font-weight: 800; text-align: left; padding: 4px 8px; border-radius: 6px; }
    .feed-p1 { color: #00f3ff; background: rgba(0,243,255,0.1); }
    .feed-p2 { color: #ff0055; background: rgba(255,0,85,0.1); }

    .turn-status { font-size: 1rem; font-weight: 900; margin: 6px 0; }
    .timer-badge { font-size: 1.3rem; font-weight: 900; color: #ffe600; }

    .input-zone { width: 100%; max-width: 360px; display: flex; gap: 8px; }
    .word-in {
      flex: 1; background: rgba(0,0,0,0.5); border: 2px solid rgba(255,255,255,0.2); border-radius: 14px;
      color: #fff; font-size: 1.1rem; font-weight: 900; padding: 10px 14px; text-transform: uppercase;
      outline: none; text-align: center; letter-spacing: 2px;
      user-select: text; -webkit-user-select: text;
    }
    .word-in:focus { border-color: #00f3ff; box-shadow: 0 0 15px rgba(0,243,255,0.4); }
    .btn-send {
      padding: 0 20px; background: linear-gradient(180deg, #00f3ff, #0066cc); border: none;
      border-radius: 14px; color: #000; font-size: 1.1rem; font-weight: 900; cursor: pointer;
    }

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
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 20px rgba(0,243,255,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="status-badge" id="badge">Connexion...</span>
  </header>

  <div class="chain-target-box">
    <div class="must-start">LE PROCHAIN MOT DOIT COMMENCER PAR :</div>
    <div class="last-letter" id="target-letter">?</div>
  </div>

  <div class="history-feed" id="history-feed"></div>

  <div>
    <div class="turn-status" id="turn-status">Attente...</div>
    <div class="timer-badge" id="timer-val">15s</div>
  </div>

  <div class="input-zone">
    <input type="text" class="word-in" id="word-in" placeholder="Votre mot..." autocomplete="off">
    <button class="btn-send" id="btn-send">ENVOYER</button>
  </div>

  <div id="overlay">
    <h2 id="ov-title">🔗 Chaîne de Mots 1v1</h2>
    <p id="ov-desc">En attente d'un adversaire sur son smartphone...</p>
    <div id="ov-actions" style="display:none; flex-direction:column; align-items:center; width:100%;">
      <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
      <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
    </div>
  </div>

  <script>
    const badge = document.getElementById('badge');
    const targetLetterEl = document.getElementById('target-letter');
    const historyFeed = document.getElementById('history-feed');
    const turnStatus = document.getElementById('turn-status');
    const timerVal = document.getElementById('timer-val');
    const wordIn = document.getElementById('word-in'), btnSend = document.getElementById('btn-send');
    const overlay = document.getElementById('overlay'), ovTitle = document.getElementById('ov-title'), ovDesc = document.getElementById('ov-desc');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false, activeRole = 1;
    let requiredLetter = 'A';
    let usedWords = new Set();
    let timeLeft = 15, timerInt = null;
    let myRematch = false, oppRematch = false;

    function setTurn(r, reqLetter) {
      activeRole = r;
      requiredLetter = reqLetter.toUpperCase();
      targetLetterEl.textContent = requiredLetter;
      timeLeft = 15;
      timerVal.textContent = timeLeft + 's';
      wordIn.value = '';

      if (activeRole === role) {
        turnStatus.textContent = "⚡ À VOTRE TOUR !";
        turnStatus.style.color = "#00ff66";
        wordIn.disabled = false;
        btnSend.disabled = false;
        wordIn.focus();
      } else {
        turnStatus.textContent = "⏳ Tour de l'adversaire...";
        turnStatus.style.color = "#8b9bb4";
        wordIn.disabled = true;
        btnSend.disabled = true;
      }

      clearInterval(timerInt);
      timerInt = setInterval(() => {
        if (!playing) return;
        timeLeft--;
        timerVal.textContent = timeLeft + 's';
        if (timeLeft <= 0) {
          clearInterval(timerInt);
          handleTimeout(activeRole);
        }
      }, 1000);
    }

    function handleTimeout(loserRole) {
      playing = false;
      const isMeLoser = (role === loserRole);
      ovTitle.textContent = isMeLoser ? "💀 TEMPS ÉCOULÉ !" : "🏆 VICTOIRE !";
      ovTitle.style.color = isMeLoser ? "#ff0055" : "#00ff66";
      ovDesc.textContent = isMeLoser ? "Vous n'avez pas trouvé de mot à temps !" : "L'adversaire a manqué de temps !";
      ovActions.style.display = "flex";
      overlay.style.display = "flex";
    }

    function addFeedItem(p, word) {
      const item = document.createElement('div');
      item.className = 'feed-item ' + (p === 1 ? 'feed-p1' : 'feed-p2');
      item.textContent = (p === 1 ? '🔵 ' : '🔴 ') + word;
      historyFeed.appendChild(item);
      historyFeed.scrollTop = historyFeed.scrollHeight;
    }

    function submitWord() {
      if (!playing || activeRole !== role) return;
      const w = wordIn.value.trim().toUpperCase();
      if (w.length >= 3 && w.startsWith(requiredLetter) && !usedWords.has(w)) {
        usedWords.add(w);
        addFeedItem(role, w);
        const lastCh = w.slice(-1);
        const nextRole = (role === 1) ? 2 : 1;
        ws.send(JSON.stringify({ t: "chain_word", word: w, next: nextRole, ch: lastCh }));
        setTurn(nextRole, lastCh);
      } else {
        wordIn.style.borderColor = '#ff0055';
        setTimeout(() => wordIn.style.borderColor = '', 400);
      }
    }

    btnSend.onclick = submitWord;
    wordIn.onkeydown = (e) => { if (e.key === 'Enter') submitWord(); };

    function resetGame() {
      usedWords.clear();
      historyFeed.innerHTML = '';
      myRematch = false; oppRematch = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "";
      overlay.style.display = "none";
      ovActions.style.display = "none";
      playing = true;
      if (role === 1) {
        const letters = ['A', 'B', 'C', 'D', 'E', 'L', 'M', 'P', 'R', 'S', 'T'];
        const startL = letters[Math.floor(Math.random() * letters.length)];
        ws.send(JSON.stringify({ t: "chain_start", ch: startL }));
        setTurn(1, startL);
      }
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      btnRematch.style.background = "rgba(255,255,255,0.2)";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) resetGame();
    };

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => {
      badge.textContent = "Recherche...";
      ws.send(JSON.stringify({ t: "join", g: "wordchain", name: myPseudo }));
    };

    ws.onmessage = (e) => {
      try {
        const d = JSON.parse(e.data);
        if (d.t === "joined") {
          role = d.role || d.p;
          badge.textContent = role === 1 ? "🔵 Joueur 1" : "🔴 Joueur 2";
          ovTitle.textContent = role === 1 ? "🔵 Joueur 1 Prêt" : "🔴 Joueur 2 Prêt";
          ovDesc.textContent = "En attente du second joueur...";
        } else if (d.t === "start") {
          role = d.role || role || 1;
          badge.textContent = "⚡ En Direct";
          badge.style.color = "#00ff66"; badge.style.borderColor = "#00ff66";
          overlay.style.display = "none";
          playing = true;
          if (role === 1) {
            const startL = 'P';
            setTimeout(() => {
              ws.send(JSON.stringify({ t: "chain_start", ch: startL }));
              setTurn(1, startL);
            }, 300);
          }
        } else if (d.t === "chain_start") {
          overlay.style.display = "none";
          playing = true;
          setTurn(1, d.ch);
        } else if (d.t === "chain_word") {
          usedWords.add(d.word);
          const senderRole = (d.next === 1) ? 2 : 1;
          addFeedItem(senderRole, d.word);
          setTurn(d.next, d.ch);
        } else if (d.t === "rematch") {
          oppRematch = true;
          if (myRematch) resetGame();
        } else if (d.t === "opp_left") {
          playing = false;
          badge.textContent = "Adversaire parti";
          badge.style.color = "#ff0055";
        }
      } catch(err){}
    };
  </script>
</body>
</html>

)rawliteral";

// ----------------------------------------------------------------------------------
//   5. LE PENDU DUEL 1V1 (MOT SECRET, GIBET CYBER 6 CHANCES & CLAVIER TACTILE)
// ----------------------------------------------------------------------------------
const char MP_HANGMAN_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Pendu Duel 1v1</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: radial-gradient(circle at 50% 30%, #0d172e 0%, #030611 100%);
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

    /* Gallows Canvas */
    .canvas-box {
      width: 180px; height: 160px; background: rgba(0,0,0,0.4); border-radius: 16px;
      border: 1px solid rgba(255,255,255,0.1); margin: 6px 0;
    }

    .word-display {
      display: flex; gap: 8px; justify-content: center; font-size: 2rem; font-weight: 900;
      letter-spacing: 4px; color: #00f3ff; margin: 10px 0; min-height: 48px;
    }
    .word-letter {
      border-bottom: 3px solid #00f3ff; min-width: 28px; display: inline-block;
    }

    /* Virtual Keyboard */
    .keyboard {
      display: flex; flex-wrap: wrap; gap: 4px; justify-content: center; max-width: 360px;
    }
    .kb-key {
      width: 32px; height: 40px; border-radius: 8px; background: rgba(255,255,255,0.08);
      border: 1px solid rgba(255,255,255,0.15); font-size: 1rem; font-weight: 900; color: #fff;
      display: flex; align-items: center; justify-content: center; cursor: pointer;
    }
    .kb-key:active { transform: scale(0.92); background: rgba(0,243,255,0.3); }
    .kb-key.used { opacity: 0.25; pointer-events: none; }

    /* Role info */
    .role-info { font-size: 0.85rem; font-weight: 800; color: #ffe600; margin-bottom: 4px; }

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
    .btn-rematch { background: linear-gradient(180deg, #00f3ff, #0077ee); color: #000; box-shadow: 0 4px 20px rgba(0,243,255,0.4); }
    .btn-rematch:active { transform: scale(0.96); }
    .btn-quit { background: rgba(255,255,255,0.08); color: #8b9bb4; border: 1px solid rgba(255,255,255,0.2); font-size: 0.85rem; }

    .word-input-box {
      display: none; flex-direction: column; gap: 8px; width: 100%; max-width: 320px;
    }
    .sec-word-in {
      background: rgba(0,0,0,0.5); border: 2px solid rgba(0,243,255,0.5); border-radius: 12px;
      padding: 10px; font-size: 1.2rem; font-weight: 900; color: #fff; text-align: center; text-transform: uppercase;
      outline: none; user-select: text; -webkit-user-select: text;
    }
  </style>
</head>
<body>
  <header>
    <a href="/hub" class="btn-hub">◀ Salon</a>
    <span class="status-badge" id="badge">Connexion...</span>
  </header>

  <div class="role-info" id="role-info">Chargement...</div>

  <canvas class="canvas-box" id="c" width="180" height="160"></canvas>

  <div class="word-display" id="word-display"></div>

  <!-- Setter Input -->
  <div class="word-input-box" id="setter-box">
    <input type="text" class="sec-word-in" id="sec-word-in" placeholder="MOT SECRET (4-9 LETTRES)" autocomplete="off">
    <button class="act-btn btn-rematch" id="btn-submit-word">Valider le Mot</button>
  </div>

  <!-- Guesser Keyboard -->
  <div class="keyboard" id="keyboard"></div>

  <div id="overlay">
    <h2 id="ov-title">🪢 Pendu Duel 1v1</h2>
    <p id="ov-desc">En attente d'un adversaire sur son smartphone...</p>
    <div id="ov-actions" style="display:none; flex-direction:column; align-items:center; width:100%;">
      <button id="btn-rematch" class="act-btn btn-rematch">🔄 Rejouer (Revanche)</button>
      <a href="/hub" class="act-btn btn-quit">◀ Quitter au Salon</a>
    </div>
  </div>

  <script>
    const canvas = document.getElementById('c'), ctx = canvas.getContext('2d');
    const badge = document.getElementById('badge');
    const roleInfo = document.getElementById('role-info');
    const wordDisplay = document.getElementById('word-display');
    const setterBox = document.getElementById('setter-box');
    const secWordIn = document.getElementById('sec-word-in'), btnSubmitWord = document.getElementById('btn-submit-word');
    const keyboard = document.getElementById('keyboard');
    const overlay = document.getElementById('overlay'), ovTitle = document.getElementById('ov-title'), ovDesc = document.getElementById('ov-desc');
    const ovActions = document.getElementById('ov-actions'), btnRematch = document.getElementById('btn-rematch');

    const myPseudo = localStorage.getItem('arcade_pseudo') || 'Joueur';
    let role = 0, playing = false;
    let setterRole = 1; // 1 creates word, 2 guesses (then inverts)
    let secretWord = '', revealed = [];
    let mistakes = 0;
    const MAX_MISTAKES = 6;
    let myRematch = false, oppRematch = false;

    // Draw gallows
    function drawGallows(m) {
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      ctx.strokeStyle = '#00f3ff'; ctx.lineWidth = 3; ctx.lineCap = 'round';
      ctx.beginPath();
      // Base
      ctx.moveTo(20, 145); ctx.lineTo(160, 145);
      // Pole
      ctx.moveTo(50, 145); ctx.lineTo(50, 20);
      // Beam
      ctx.lineTo(120, 20);
      // Rope
      ctx.lineTo(120, 40);
      ctx.stroke();

      ctx.strokeStyle = '#ff0055';
      // Head
      if (m >= 1) { ctx.beginPath(); ctx.arc(120, 52, 12, 0, Math.PI * 2); ctx.stroke(); }
      // Body
      if (m >= 2) { ctx.beginPath(); ctx.moveTo(120, 64); ctx.lineTo(120, 95); ctx.stroke(); }
      // Left arm
      if (m >= 3) { ctx.beginPath(); ctx.moveTo(120, 72); ctx.lineTo(100, 85); ctx.stroke(); }
      // Right arm
      if (m >= 4) { ctx.beginPath(); ctx.moveTo(120, 72); ctx.lineTo(140, 85); ctx.stroke(); }
      // Left leg
      if (m >= 5) { ctx.beginPath(); ctx.moveTo(120, 95); ctx.lineTo(105, 125); ctx.stroke(); }
      // Right leg
      if (m >= 6) { ctx.beginPath(); ctx.moveTo(120, 95); ctx.lineTo(135, 125); ctx.stroke(); }
    }

    function createKeyboard() {
      keyboard.innerHTML = '';
      const letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ".split('');
      letters.forEach(l => {
        const k = document.createElement('div');
        k.className = 'kb-key'; k.textContent = l;
        k.onclick = () => guessLetter(l, k);
        keyboard.appendChild(k);
      });
    }

    function guessLetter(letter, keyEl) {
      if (!playing || role === setterRole) return;
      keyEl.classList.add('used');
      ws.send(JSON.stringify({ t: "h_guess", letter: letter }));
      processGuess(letter);
    }

    function processGuess(letter) {
      if (secretWord.includes(letter)) {
        for (let i = 0; i < secretWord.length; i++) {
          if (secretWord[i] === letter) revealed[i] = letter;
        }
        updateWordUI();
        if (!revealed.includes('_')) {
          endGame(true);
        }
      } else {
        mistakes++;
        drawGallows(mistakes);
        if (mistakes >= MAX_MISTAKES) {
          endGame(false);
        }
      }
    }

    function updateWordUI() {
      wordDisplay.innerHTML = '';
      revealed.forEach(ch => {
        const span = document.createElement('span');
        span.className = 'word-letter';
        span.textContent = ch;
        wordDisplay.appendChild(span);
      });
    }

    function endGame(guesserWon) {
      playing = false;
      const isMeGuesser = (role !== setterRole);
      let iWon = isMeGuesser ? guesserWon : !guesserWon;

      ovTitle.textContent = iWon ? "🏆 VICTOIRE !" : "💀 DÉFAITE...";
      ovTitle.style.color = iWon ? "#00ff66" : "#ff0055";
      ovDesc.textContent = "Le mot secret était : " + secretWord;
      ovActions.style.display = "flex";
      overlay.style.display = "flex";
    }

    btnSubmitWord.onclick = () => {
      const w = secWordIn.value.trim().toUpperCase();
      if (w.length >= 4 && /^[A-Z]+$/.test(w)) {
        secretWord = w;
        revealed = Array(w.length).fill('_');
        setterBox.style.display = 'none';
        roleInfo.textContent = "L'adversaire devine votre mot...";
        ws.send(JSON.stringify({ t: "h_word", len: w.length, word: w }));
        drawGallows(0);
        updateWordUI();
        playing = true;
      } else {
        secWordIn.style.borderColor = '#ff0055';
        setTimeout(() => secWordIn.style.borderColor = '', 400);
      }
    };

    function startSetup() {
      mistakes = 0;
      drawGallows(0);
      wordDisplay.innerHTML = '';
      createKeyboard();
      overlay.style.display = 'none';
      ovActions.style.display = 'none';

      if (role === setterRole) {
        roleInfo.textContent = "VOUS CHOISISSEZ LE MOT SECRET";
        setterBox.style.display = 'flex';
        keyboard.style.display = 'none';
        secWordIn.value = '';
        secWordIn.focus();
      } else {
        roleInfo.textContent = "L'ADVERSAIRE CHOISIT LE MOT...";
        setterBox.style.display = 'none';
        keyboard.style.display = 'flex';
      }
    }

    function resetGame() {
      setterRole = (setterRole === 1) ? 2 : 1; // Alternate roles
      myRematch = false; oppRematch = false;
      btnRematch.textContent = "🔄 Rejouer (Revanche)";
      btnRematch.style.background = "";
      startSetup();
    }

    btnRematch.onclick = () => {
      myRematch = true;
      btnRematch.textContent = "⏳ En attente de l'adversaire...";
      btnRematch.style.background = "rgba(255,255,255,0.2)";
      ws.send(JSON.stringify({ t: "rematch" }));
      if (oppRematch) resetGame();
    };

    const ws = new WebSocket('ws://' + location.hostname + ':81/');
    ws.onopen = () => {
      badge.textContent = "Recherche...";
      ws.send(JSON.stringify({ t: "join", g: "hangman", name: myPseudo }));
    };

    ws.onmessage = (e) => {
      try {
        const d = JSON.parse(e.data);
        if (d.t === "joined") {
          role = d.role || d.p;
          badge.textContent = role === 1 ? "🔵 Joueur 1" : "🔴 Joueur 2";
          ovTitle.textContent = role === 1 ? "🔵 Joueur 1 Prêt" : "🔴 Joueur 2 Prêt";
          ovDesc.textContent = "En attente du second joueur...";
        } else if (d.t === "start") {
          role = d.role || role || 1;
          badge.textContent = "⚡ En Direct";
          badge.style.color = "#00ff66"; badge.style.borderColor = "#00ff66";
          startSetup();
        } else if (d.t === "h_word") {
          secretWord = d.word;
          revealed = Array(d.len).fill('_');
          updateWordUI();
          roleInfo.textContent = "À VOUS DE DEVINER LE MOT !";
          playing = true;
        } else if (d.t === "h_guess") {
          processGuess(d.letter);
        } else if (d.t === "rematch") {
          oppRematch = true;
          if (myRematch) resetGame();
        } else if (d.t === "opp_left") {
          playing = false;
          badge.textContent = "Adversaire parti";
          badge.style.color = "#ff0055";
        }
      } catch(err){}
    };
  </script>
</body>
</html>

)rawliteral";

#endif // GAME_PACK_MP_MOTS_H
