#ifndef GAME_MP_UNDERCOVER_H
#define GAME_MP_UNDERCOVER_H

#include <pgmspace.h>

const char MP_UNDERCOVER_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Undercover - Multijoueur 3-8 Joueurs</title>
  <style>
    :root {
      --bg: #0b0f19;
      --card: rgba(22, 28, 48, 0.92);
      --card-border: rgba(0, 243, 255, 0.25);
      --cyan: #00f3ff;
      --pink: #ff0055;
      --yellow: #ffe600;
      --green: #00ff66;
      --purple: #a855f7;
      --text: #f0f4fc;
      --dim: #8b9bb4;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; user-select: none; -webkit-user-select: none; }
    body {
      background: var(--bg);
      background-image: 
        radial-gradient(circle at 10% 20%, rgba(168, 85, 247, 0.15) 0%, transparent 40%),
        radial-gradient(circle at 90% 80%, rgba(0, 243, 255, 0.12) 0%, transparent 40%),
        repeating-linear-gradient(0deg, rgba(255, 255, 255, 0.015) 0px, rgba(255, 255, 255, 0.015) 1px, transparent 1px, transparent 30px);
      color: var(--text);
      font-family: system-ui, -apple-system, sans-serif;
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 12px;
    }
    header {
      width: 100%; max-width: 600px;
      display: flex; justify-content: space-between; align-items: center;
      padding: 10px 14px; background: rgba(14, 18, 32, 0.85);
      border-radius: 14px; border: 1px solid var(--card-border);
      margin-bottom: 14px; backdrop-filter: blur(10px);
    }
    .btn-back {
      color: #fff; text-decoration: none; font-size: 0.85rem; font-weight: 700;
      background: rgba(255,255,255,0.08); padding: 6px 12px; border-radius: 8px;
      border: 1px solid rgba(255,255,255,0.2);
    }
    .status-badge {
      font-size: 0.8rem; font-weight: 800; padding: 4px 10px; border-radius: 12px;
      display: flex; align-items: center; gap: 6px;
    }
    .status-online { background: rgba(0, 255, 102, 0.15); color: var(--green); border: 1px solid var(--green); }
    .status-offline { background: rgba(255, 0, 85, 0.15); color: var(--pink); border: 1px solid var(--pink); }

    .main-box {
      width: 100%; max-width: 600px;
      background: var(--card); border: 1px solid var(--card-border);
      border-radius: 18px; padding: 18px;
      box-shadow: 0 10px 30px rgba(0,0,0,0.5);
      display: flex; flex-direction: column; gap: 16px;
    }
    h1 {
      font-size: 1.5rem; text-align: center; text-transform: uppercase; letter-spacing: 1px;
      background: linear-gradient(135deg, #fff, var(--purple), var(--cyan));
      -webkit-background-clip: text; -webkit-text-fill-color: transparent;
    }
    .subtitle { text-align: center; color: var(--dim); font-size: 0.85rem; }

    /* Player Chips */
    .player-grid {
      display: grid; grid-template-columns: repeat(auto-fill, minmax(130px, 1fr)); gap: 10px;
      margin: 10px 0;
    }
    .player-card {
      background: rgba(11, 15, 28, 0.8); border: 1px solid rgba(255,255,255,0.1);
      border-radius: 12px; padding: 10px; text-align: center;
      transition: all 0.2s; position: relative;
    }
    .player-card.is-me { border-color: var(--cyan); box-shadow: 0 0 10px rgba(0,243,255,0.2); }
    .player-card.is-host::after {
      content: '👑 HÔTE'; position: absolute; top: -6px; right: 6px;
      font-size: 0.6rem; font-weight: 900; background: var(--yellow); color: #000;
      padding: 1px 5px; border-radius: 4px;
    }
    .player-card.is-dead { opacity: 0.4; filter: grayscale(1); border-color: #555; text-decoration: line-through; }
    .player-card.is-active-speaker { border-color: var(--yellow); box-shadow: 0 0 15px rgba(255,230,0,0.4); }
    .p-avatar { font-size: 1.6rem; margin-bottom: 4px; }
    .p-name { font-weight: 800; font-size: 0.9rem; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
    .p-status { font-size: 0.75rem; color: var(--dim); }

    /* Controls & Buttons */
    .btn {
      width: 100%; padding: 14px; border: none; border-radius: 12px;
      font-size: 1rem; font-weight: 900; text-transform: uppercase; letter-spacing: 1px;
      cursor: pointer; transition: all 0.15s; display: flex; align-items: center; justify-content: center; gap: 8px;
    }
    .btn-primary {
      background: linear-gradient(135deg, #a855f7, #6366f1); color: #fff;
      box-shadow: 0 4px 15px rgba(168, 85, 247, 0.4);
    }
    .btn-action {
      background: linear-gradient(135deg, var(--cyan), #0077ff); color: #000;
      box-shadow: 0 4px 15px rgba(0, 243, 255, 0.4);
    }
    .btn-danger {
      background: linear-gradient(135deg, #ff0055, #c026d3); color: #fff;
    }
    .btn:active { transform: scale(0.97); }
    .btn:disabled { opacity: 0.4; cursor: not-allowed; box-shadow: none; }

    .role-settings {
      background: rgba(0,0,0,0.3); border-radius: 12px; padding: 12px;
      display: flex; justify-content: space-around; font-size: 0.85rem;
    }
    .role-opt { display: flex; flex-direction: column; align-items: center; gap: 4px; }
    .role-opt select {
      background: #111628; color: #fff; border: 1px solid var(--card-border);
      padding: 6px 10px; border-radius: 8px; font-weight: bold;
    }

    /* Secret Card Box */
    .secret-card-container {
      background: linear-gradient(145deg, #182038, #0e1424);
      border: 2px dashed var(--purple); border-radius: 16px;
      padding: 24px 16px; text-align: center; cursor: pointer;
      position: relative; overflow: hidden; transition: all 0.3s;
    }
    .secret-card-container:active, .secret-card-container.revealed {
      border-style: solid; border-color: var(--cyan);
      background: linear-gradient(145deg, #202b4c, #131c33);
      box-shadow: 0 0 25px rgba(0, 243, 255, 0.25);
    }
    .card-secret-content { display: none; }
    .revealed .card-secret-content { display: block; }
    .revealed .card-hidden-hint { display: none; }

    .secret-word-display {
      font-size: 2rem; font-weight: 900; letter-spacing: 2px;
      color: var(--yellow); text-shadow: 0 0 20px rgba(255, 230, 0, 0.6);
      margin: 12px 0; text-transform: uppercase;
    }
    .role-badge-tag {
      display: inline-block; padding: 4px 12px; border-radius: 20px; font-weight: 900; font-size: 0.85rem;
      margin-bottom: 8px;
    }
    .tag-civil { background: rgba(0, 255, 102, 0.2); color: var(--green); border: 1px solid var(--green); }
    .tag-undercover { background: rgba(255, 0, 85, 0.2); color: var(--pink); border: 1px solid var(--pink); }
    .tag-mrwhite { background: rgba(255, 255, 255, 0.2); color: #fff; border: 1px solid #fff; }

    /* Voting Options */
    .vote-list { display: flex; flex-direction: column; gap: 8px; margin: 10px 0; }
    .vote-item {
      background: rgba(14, 19, 36, 0.9); border: 1px solid rgba(255,255,255,0.15);
      border-radius: 12px; padding: 12px 16px; display: flex; justify-content: space-between;
      align-items: center; cursor: pointer; transition: all 0.2s; font-weight: 700;
    }
    .vote-item:active, .vote-item.selected {
      border-color: var(--pink); background: rgba(255, 0, 85, 0.15);
      box-shadow: 0 0 12px rgba(255, 0, 85, 0.3);
    }

    /* Modal / Popups */
    .overlay {
      position: fixed; inset: 0; background: rgba(0,0,0,0.85); backdrop-filter: blur(8px);
      display: flex; align-items: center; justify-content: center; padding: 16px; z-index: 100;
    }
    .modal {
      background: var(--card); border: 2px solid var(--cyan); border-radius: 20px;
      padding: 22px; width: 100%; max-width: 480px; text-align: center;
      box-shadow: 0 10px 40px rgba(0, 243, 255, 0.3); display: flex; flex-direction: column; gap: 14px;
    }
    input[type="text"] {
      width: 100%; padding: 12px; background: #0c1020; border: 1px solid var(--cyan);
      border-radius: 10px; color: #fff; font-size: 1.1rem; text-align: center;
    }
  </style>
</head>
<body>

  <header>
    <a href="/hub" class="btn-back">⬅ Salon</a>
    <div id="statusTag" class="status-badge status-offline">● Déconnecté</div>
  </header>

  <div class="main-box">
    <h1>🕵️ UNDERCOVER</h1>
    <p class="subtitle">Jeu de déduction & bluff multijoueur (3 à 8 Joueurs)</p>

    <!-- SECTION 1: LOBBY ATTENTE -->
    <div id="viewLobby" style="display: block;">
      <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:8px;">
        <span style="font-weight:bold; font-size:0.9rem;">Joueurs dans la salle (<span id="playerCount">0</span>/8) :</span>
        <span style="font-size:0.78rem; color:var(--dim);">Min. 3 joueurs</span>
      </div>

      <div class="player-grid" id="lobbyPlayerGrid">
        <!-- Rempli en JS -->
      </div>

      <!-- Réglages Réservés à l'Hôte -->
      <div id="hostSettingsBox" style="display:none; flex-direction:column; gap:10px; margin-top:10px;">
        <div class="role-settings">
          <div class="role-opt">
            <span>🕵️ Undercover</span>
            <select id="selUndercover">
              <option value="1" selected>1</option>
              <option value="2">2</option>
            </select>
          </div>
          <div class="role-opt">
            <span>🎭 Mr. White</span>
            <select id="selMrWhite">
              <option value="0">0</option>
              <option value="1" selected>1</option>
            </select>
          </div>
        </div>
        <button id="btnStartGame" class="btn btn-primary" onclick="hostStartGame()">🎮 Lancer la Partie</button>
      </div>

      <div id="guestWaitMsg" style="display:none; text-align:center; padding:12px; color:var(--dim); font-size:0.9rem;">
        ⏳ En attente que l'hôte lance la partie...
      </div>
    </div>

    <!-- SECTION 2: RÉVÉLATION SECRÈTE DU MOT -->
    <div id="viewSecret" style="display: none;">
      <p style="text-align:center; font-size:0.88rem; color:var(--dim); margin-bottom:10px;">
        Gardez votre écran discret ! Maintenez la carte pour voir votre rôle.
      </p>

      <div class="secret-card-container" id="secretCard" onmousedown="showSecretCard(true)" onmouseup="showSecretCard(false)" ontouchstart="showSecretCard(true)" ontouchend="showSecretCard(false)">
        <div class="card-hidden-hint">
          <div style="font-size:3rem; margin-bottom:10px;">🔒</div>
          <div style="font-size:1.1rem; font-weight:800; color:var(--cyan);">TOUCHEZ & MAINTENEZ</div>
          <div style="font-size:0.8rem; color:var(--dim); margin-top:4px;">pour révéler votre identité secrète</div>
        </div>

        <div class="card-secret-content">
          <div id="secretRoleBadge" class="role-badge-tag tag-civil">CIVIL</div>
          <div class="secret-word-display" id="secretWordText">POMME</div>
          <p id="secretRoleDesc" style="font-size:0.82rem; color:var(--dim); line-height:1.4;">
            Donnez un indice subtil sans vous faire démasquer !
          </p>
        </div>
      </div>

      <button class="btn btn-action" style="margin-top:14px;" onclick="confirmSecretReady()">✅ J'ai mémorisé mon mot</button>
    </div>

    <!-- SECTION 3: TOUR DE PAROLE / DISCUSSION -->
    <div id="viewDiscussion" style="display: none;">
      <div style="background:rgba(255,230,0,0.1); border:1px solid var(--yellow); border-radius:12px; padding:12px; text-align:center;">
        <div style="font-size:0.8rem; color:var(--yellow); font-weight:bold; text-transform:uppercase;">🎤 Tour de parole</div>
        <div style="font-size:1.2rem; font-weight:900; margin:4px 0;" id="currentSpeakerName">Joueur...</div>
        <div style="font-size:0.8rem; color:var(--dim);">Donnez à voix haute <b>1 seul mot/indice</b> pour décrire votre mot.</div>
      </div>

      <div class="player-grid" id="discussPlayerGrid" style="margin-top:14px;"></div>

      <div id="hostDiscussControls" style="display:none; flex-direction:column; gap:8px; margin-top:10px;">
        <button class="btn btn-action" onclick="nextSpeaker()">⏭ Joueur Suivant</button>
        <button class="btn btn-danger" onclick="hostStartVote()">🗳 Passer au Vote</button>
      </div>
    </div>

    <!-- SECTION 4: VOTE DE L'IMPOSTEUR -->
    <div id="viewVote" style="display: none;">
      <h2 style="font-size:1.1rem; text-align:center; color:var(--pink);">🗳 QUI EST L'IMPOSTEUR ?</h2>
      <p style="text-align:center; font-size:0.82rem; color:var(--dim);">Votez pour la personne que vous suspectez.</p>

      <div class="vote-list" id="voteOptionsList"></div>

      <button id="btnConfirmVote" class="btn btn-danger" disabled onclick="submitMyVote()">Confirmer mon vote</button>
      <div id="voteStatusMsg" style="text-align:center; font-size:0.82rem; color:var(--dim); margin-top:8px;"></div>
    </div>

    <!-- SECTION 5: ÉCRAN DE FIN & RÉCAPITULATIF -->
    <div id="viewEnd" style="display: none;">
      <div style="text-align:center; padding:14px 0;">
        <div id="endVictoryIcon" style="font-size:3.5rem; margin-bottom:6px;">🏆</div>
        <h2 id="endVictoryTitle" style="font-size:1.5rem; color:var(--green);">VICTOIRE DES CIVILS !</h2>
        <p id="endVictorySub" style="color:var(--dim); font-size:0.85rem; margin-top:4px;">Tous les imposteurs ont été éliminés.</p>
      </div>

      <div style="background:rgba(0,0,0,0.3); border-radius:12px; padding:12px; margin:10px 0;">
        <div style="font-size:0.85rem; font-weight:bold; margin-bottom:8px;">Mots de la partie :</div>
        <div style="display:flex; justify-content:space-between; font-size:0.85rem; margin-bottom:4px;">
          <span>👤 Mot des Civils :</span>
          <b id="endWordCivil" style="color:var(--green);">-</b>
        </div>
        <div style="display:flex; justify-content:space-between; font-size:0.85rem;">
          <span>🕵️ Mot Undercover :</span>
          <b id="endWordUndercover" style="color:var(--pink);">-</b>
        </div>
      </div>

      <div class="player-grid" id="endPlayerGrid"></div>

      <button id="btnRestartGame" class="btn btn-primary" style="margin-top:12px; display:none;" onclick="hostRestartLobby()">🔄 Rejouer une Partie</button>
    </div>

  </div>

  <!-- MODAL MR WHITE DEVINER -->
  <div id="modalMrWhite" class="overlay" style="display:none;">
    <div class="modal">
      <div style="font-size:2.5rem;">🎭</div>
      <h2 style="color:var(--yellow); font-size:1.3rem;">MR. WHITE DÉMASQUÉ !</h2>
      <p style="font-size:0.85rem; color:var(--dim);">
        Mr. White a été éliminé mais peut encore voler la victoire s'il devine le mot exact des civils !
      </p>
      <div id="mrWhiteInputBox" style="display:none; flex-direction:column; gap:10px;">
        <input type="text" id="mrWhiteGuessInput" placeholder="Entrez le mot secret..." maxlength="30">
        <button class="btn btn-action" onclick="submitMrWhiteGuess()">Deviner le Mot</button>
      </div>
      <div id="mrWhiteWaitMsg" style="display:none; color:var(--dim); font-size:0.9rem;">
        ⏳ Mr. White tente de deviner le mot...
      </div>
    </div>
  </div>

<script>
  // Dictionnaire de 50 paires de mots riches en français
  const WORD_PAIRS = [
    ["Guitare", "Violon"], ["Pizza", "Burger"], ["Chien", "Chat"], ["Café", "Thé"],
    ["Plage", "Piscine"], ["Avion", "Hélicoptère"], ["Football", "Rugby"], ["Lion", "Tigre"],
    ["Soleil", "Lune"], ["Pomme", "Poire"], ["Cinéma", "Théâtre"], ["Bateau", "Sous-marin"],
    ["Chocolat", "Bonbon"], ["Stylo", "Crayon"], ["Montagne", "Colline"], ["Voiture", "Moto"],
    ["Neige", "Glace"], ["Docteur", "Infirmier"], ["Roi", "Président"], ["Livre", "Journal"],
    ["Forêt", "Jungle"], ["Bière", "Vin"], ["Pain", "Baguette"], ["Miroir", "Vitre"],
    ["Téléphone", "Tablette"], ["Montre", "Horloge"], ["Lampe", "Bougie"], ["Couteau", "Fourchette"],
    ["Poule", "Canard"], ["Veste", "Manteau"], ["Chapeau", "Casquette"], ["Bague", "Bracelet"],
    ["Rivière", "Fleuve"], ["Gâteau", "Tarte"], ["Chaise", "Fauteuil"], ["Lit", "Canapé"],
    ["Avocat", "Juge"], ["Épée", "Sabre"], ["Police", "Gendarme"], ["Danse", "Chant"]
  ];

  const AVATARS = ["🐱","🦊","🐻","🐼","🦁","🐨","🐯","🐰"];

  let ws;
  let myId = -1;
  let myPseudo = localStorage.getItem('arcade_pseudo') || ("Joueur_" + Math.floor(Math.random()*900 + 100));
  let roomPlayers = [];
  let isHost = false;
  let mySecretRole = "";
  let mySecretWord = "";
  let selectedVoteId = -1;
  let gameState = null;

  function initWs() {
    const host = window.location.hostname || "192.168.4.1";
    ws = new WebSocket("ws://" + host + ":81");

    ws.onopen = () => {
      document.getElementById("statusTag").className = "status-badge status-online";
      document.getElementById("statusTag").innerText = "● Connecté";
      ws.send(JSON.stringify({ t: "room_join", room: "undercover", name: myPseudo }));
    };

    ws.onclose = () => {
      document.getElementById("statusTag").className = "status-badge status-offline";
      document.getElementById("statusTag").innerText = "● Reconnexion...";
      setTimeout(initWs, 2000);
    };

    ws.onmessage = (e) => {
      try {
        const d = JSON.parse(e.data);
        handleMessage(d);
      } catch(err) { console.error(err); }
    };
  }

  function handleMessage(d) {
    if (d.t === "room_joined") {
      myId = d.my_id;
    } else if (d.t === "room_state") {
      updateRoomState(d.players);
    } else if (d.t === "game_started") {
      onGameStarted(d);
    } else if (d.t === "next_turn") {
      onNextTurn(d);
    } else if (d.t === "start_vote_phase") {
      onStartVotePhase(d);
    } else if (d.t === "vote_cast_update") {
      onVoteCastUpdate(d);
    } else if (d.t === "vote_result") {
      onVoteResult(d);
    } else if (d.t === "mrwhite_guess_prompt") {
      onMrWhiteGuessPrompt(d);
    } else if (d.t === "game_over") {
      onGameOver(d);
    } else if (d.t === "restart_lobby") {
      onRestartLobby();
    }
  }

  function updateRoomState(players) {
    roomPlayers = players || [];
    document.getElementById("playerCount").innerText = roomPlayers.length;

    const me = roomPlayers.find(p => p.id === myId);
    isHost = me && me.host;

    document.getElementById("hostSettingsBox").style.display = isHost ? "flex" : "none";
    document.getElementById("guestWaitMsg").style.display = (!isHost && (!gameState || gameState.phase === "lobby")) ? "block" : "none";
    document.getElementById("btnStartGame").disabled = roomPlayers.length < 3;

    renderLobbyPlayers();
  }

  function renderLobbyPlayers() {
    const grid = document.getElementById("lobbyPlayerGrid");
    grid.innerHTML = "";
    roomPlayers.forEach((p, idx) => {
      const card = document.createElement("div");
      card.className = "player-card" + (p.id === myId ? " is-me" : "") + (p.host ? " is-host" : "");
      card.innerHTML = `
        <div class="p-avatar">${AVATARS[idx % AVATARS.length]}</div>
        <div class="p-name">${escapeHtml(p.name)}</div>
        <div class="p-status">${p.id === myId ? "Vous" : "Prêt"}</div>
      `;
      grid.appendChild(card);
    });
  }

  function hostStartGame() {
    if (!isHost || roomPlayers.length < 3) return;

    const numUndercover = parseInt(document.getElementById("selUndercover").value) || 1;
    const numMrWhite = parseInt(document.getElementById("selMrWhite").value) || 0;

    const pair = WORD_PAIRS[Math.floor(Math.random() * WORD_PAIRS.length)];
    const civilWord = pair[0];
    const undercoverWord = pair[1];

    let pList = [...roomPlayers];
    pList.sort(() => Math.random() - 0.5);

    let assignments = {};
    let idx = 0;

    for (let u = 0; u < numUndercover; u++) {
      if (idx < pList.length) assignments[pList[idx++].id] = { role: "undercover", word: undercoverWord };
    }
    for (let w = 0; w < numMrWhite; w++) {
      if (idx < pList.length) assignments[pList[idx++].id] = { role: "mrwhite", word: "" };
    }
    while (idx < pList.length) {
      assignments[pList[idx++].id] = { role: "civil", word: civilWord };
    }

    let speakerOrder = roomPlayers.map(p => p.id).sort(() => Math.random() - 0.5);

    const payload = {
      t: "game_started",
      assignments: assignments,
      speakerOrder: speakerOrder,
      civilWord: civilWord,
      undercoverWord: undercoverWord
    };

    ws.send(JSON.stringify(payload));
    onGameStarted(payload);
  }

  function onGameStarted(d) {
    gameState = {
      phase: "secret",
      assignments: d.assignments,
      speakerOrder: d.speakerOrder,
      speakerIdx: 0,
      civilWord: d.civilWord,
      undercoverWord: d.undercoverWord,
      aliveIds: roomPlayers.map(p => p.id),
      votes: {}
    };

    const myAssign = d.assignments[myId];
    if (myAssign) {
      mySecretRole = myAssign.role;
      mySecretWord = myAssign.word;
    }

    const badge = document.getElementById("secretRoleBadge");
    const wText = document.getElementById("secretWordText");
    const desc = document.getElementById("secretRoleDesc");

    if (mySecretRole === "civil") {
      badge.className = "role-badge-tag tag-civil";
      badge.innerText = "👤 CIVIL";
      wText.innerText = mySecretWord;
      desc.innerText = "Donnez un indice subtil. Démasquez l'Undercover et Mr. White !";
    } else if (mySecretRole === "undercover") {
      badge.className = "role-badge-tag tag-undercover";
      badge.innerText = "🕵️ UNDERCOVER";
      wText.innerText = mySecretWord;
      desc.innerText = "Votre mot est proche de celui des civils. Fondez-vous dans la masse !";
    } else {
      badge.className = "role-badge-tag tag-mrwhite";
      badge.innerText = "🎭 MR. WHITE";
      wText.innerText = "AUCUN MOT";
      desc.innerText = "Vous n'avez aucun mot ! Écoutez attentivement et bluffez sans vous faire attraper.";
    }

    switchView("viewSecret");
  }

  function showSecretCard(show) {
    const card = document.getElementById("secretCard");
    if (show) card.classList.add("revealed");
    else card.classList.remove("revealed");
  }

  function confirmSecretReady() {
    switchView("viewDiscussion");
    renderDiscussionPhase();
  }

  function renderDiscussionPhase() {
    if (!gameState) return;
    const currentSpkId = gameState.speakerOrder[gameState.speakerIdx % gameState.speakerOrder.length];
    const spkPlayer = roomPlayers.find(p => p.id === currentSpkId);

    document.getElementById("currentSpeakerName").innerText = spkPlayer ? spkPlayer.name : "Joueur";
    document.getElementById("hostDiscussControls").style.display = isHost ? "flex" : "none";

    const grid = document.getElementById("discussPlayerGrid");
    grid.innerHTML = "";
    roomPlayers.forEach((p, idx) => {
      const isDead = !gameState.aliveIds.includes(p.id);
      const isSpeaker = p.id === currentSpkId;
      const card = document.createElement("div");
      card.className = "player-card" + (p.id === myId ? " is-me" : "") + (isSpeaker ? " is-active-speaker" : "") + (isDead ? " is-dead" : "");
      card.innerHTML = `
        <div class="p-avatar">${AVATARS[idx % AVATARS.length]}</div>
        <div class="p-name">${escapeHtml(p.name)}</div>
        <div class="p-status">${isDead ? "Éliminé" : (isSpeaker ? "🎤 Parle" : "En écoute")}</div>
      `;
      grid.appendChild(card);
    });
  }

  function nextSpeaker() {
    if (!isHost || !gameState) return;
    let nextIdx = (gameState.speakerIdx + 1) % gameState.speakerOrder.length;
    while (!gameState.aliveIds.includes(gameState.speakerOrder[nextIdx])) {
      nextIdx = (nextIdx + 1) % gameState.speakerOrder.length;
    }
    const payload = { t: "next_turn", speakerIdx: nextIdx };
    ws.send(JSON.stringify(payload));
    onNextTurn(payload);
  }

  function onNextTurn(d) {
    if (!gameState) return;
    gameState.speakerIdx = d.speakerIdx;
    renderDiscussionPhase();
  }

  function hostStartVote() {
    if (!isHost) return;
    const payload = { t: "start_vote_phase" };
    ws.send(JSON.stringify(payload));
    onStartVotePhase(payload);
  }

  function onStartVotePhase(d) {
    if (!gameState) return;
    gameState.phase = "vote";
    gameState.votes = {};
    selectedVoteId = -1;
    document.getElementById("btnConfirmVote").disabled = true;
    document.getElementById("voteStatusMsg").innerText = "";

    switchView("viewVote");
    renderVoteOptions();
  }

  function renderVoteOptions() {
    const list = document.getElementById("voteOptionsList");
    list.innerHTML = "";

    const isAlive = gameState.aliveIds.includes(myId);
    if (!isAlive) {
      list.innerHTML = "<p style='text-align:center; color:var(--dim);'>Vous êtes éliminé. Vous observez le vote.</p>";
      document.getElementById("btnConfirmVote").style.display = "none";
      return;
    }
    document.getElementById("btnConfirmVote").style.display = "block";

    gameState.aliveIds.forEach(id => {
      if (id === myId) return;
      const p = roomPlayers.find(pl => pl.id === id);
      if (!p) return;

      const item = document.createElement("div");
      item.className = "vote-item" + (selectedVoteId === id ? " selected" : "");
      item.innerHTML = `<span>👤 ${escapeHtml(p.name)}</span> <span style="color:var(--dim);">👉 Suspect</span>`;
      item.onclick = () => {
        selectedVoteId = id;
        document.getElementById("btnConfirmVote").disabled = false;
        renderVoteOptions();
      };
      list.appendChild(item);
    });
  }

  function submitMyVote() {
    if (selectedVoteId < 0 || !gameState) return;
    document.getElementById("btnConfirmVote").disabled = true;
    document.getElementById("voteStatusMsg").innerText = "✅ Vote enregistré ! En attente des autres...";

    const payload = { t: "vote_cast_update", voterId: myId, targetId: selectedVoteId };
    ws.send(JSON.stringify(payload));
    onVoteCastUpdate(payload);
  }

  function onVoteCastUpdate(d) {
    if (!gameState) return;
    gameState.votes[d.voterId] = d.targetId;

    const aliveVoters = gameState.aliveIds;
    const totalVotes = Object.keys(gameState.votes).length;
    document.getElementById("voteStatusMsg").innerText = `Votes reçus : ${totalVotes} / ${aliveVoters.length}`;

    if (isHost && totalVotes >= aliveVoters.length) {
      tallyVotes();
    }
  }

  function tallyVotes() {
    let counts = {};
    for (let voter in gameState.votes) {
      let target = gameState.votes[voter];
      counts[target] = (counts[target] || 0) + 1;
    }

    let maxVotes = 0;
    let eliminatedId = -1;
    for (let target in counts) {
      if (counts[target] > maxVotes) {
        maxVotes = counts[target];
        eliminatedId = parseInt(target);
      }
    }

    const payload = { t: "vote_result", eliminatedId: eliminatedId };
    ws.send(JSON.stringify(payload));
    onVoteResult(payload);
  }

  function onVoteResult(d) {
    if (!gameState) return;
    const elimId = d.eliminatedId;
    const elimAssign = gameState.assignments[elimId];

    gameState.aliveIds = gameState.aliveIds.filter(id => id !== elimId);

    if (elimAssign && elimAssign.role === "mrwhite") {
      const promptPayload = { t: "mrwhite_guess_prompt", mrWhiteId: elimId };
      if (isHost) ws.send(JSON.stringify(promptPayload));
      onMrWhiteGuessPrompt(promptPayload);
      return;
    }

    checkGameConditions();
  }

  function onMrWhiteGuessPrompt(d) {
    document.getElementById("modalMrWhite").style.display = "flex";
    if (myId === d.mrWhiteId) {
      document.getElementById("mrWhiteInputBox").style.display = "flex";
      document.getElementById("mrWhiteWaitMsg").style.display = "none";
    } else {
      document.getElementById("mrWhiteInputBox").style.display = "none";
      document.getElementById("mrWhiteWaitMsg").style.display = "block";
    }
  }

  function submitMrWhiteGuess() {
    const guess = document.getElementById("mrWhiteGuessInput").value.trim().toLowerCase();
    document.getElementById("modalMrWhite").style.display = "none";

    const civilWord = gameState.civilWord.toLowerCase();
    const isCorrect = guess.length > 0 && civilWord.includes(guess);

    if (isCorrect) {
      triggerGameOver("mrwhite_guessed");
    } else {
      alert("❌ Mauvaise réponse ! Mr. White est définitivement éliminé.");
      checkGameConditions();
    }
  }

  function checkGameConditions() {
    if (!isHost || !gameState) return;

    let livingRoles = gameState.aliveIds.map(id => gameState.assignments[id].role);
    let impostors = livingRoles.filter(r => r === "undercover" || r === "mrwhite").length;
    let civils = livingRoles.filter(r => r === "civil").length;

    if (impostors === 0) {
      triggerGameOver("civils_win");
    } else if (impostors >= civils) {
      triggerGameOver("impostors_win");
    } else {
      let nextSpk = 0;
      while (!gameState.aliveIds.includes(gameState.speakerOrder[nextSpk])) {
        nextSpk = (nextSpk + 1) % gameState.speakerOrder.length;
      }
      const payload = { t: "next_turn", speakerIdx: nextSpk };
      ws.send(JSON.stringify(payload));
      onNextTurn(payload);
      switchView("viewDiscussion");
    }
  }

  function triggerGameOver(reason) {
    const payload = { t: "game_over", reason: reason };
    ws.send(JSON.stringify(payload));
    onGameOver(payload);
  }

  function onGameOver(d) {
    document.getElementById("modalMrWhite").style.display = "none";
    switchView("viewEnd");

    const title = document.getElementById("endVictoryTitle");
    const sub = document.getElementById("endVictorySub");
    const icon = document.getElementById("endVictoryIcon");

    if (d.reason === "civils_win") {
      icon.innerText = "🏆";
      title.innerText = "VICTOIRE DES CIVILS !";
      title.style.color = "var(--green)";
      sub.innerText = "Tous les imposteurs ont été démasqués.";
    } else if (d.reason === "mrwhite_guessed") {
      icon.innerText = "🎭";
      title.innerText = "VICTOIRE DE MR. WHITE !";
      title.style.color = "var(--yellow)";
      sub.innerText = "Mr. White a deviné le mot secret des civils !";
    } else {
      icon.innerText = "🕵️";
      title.innerText = "VICTOIRE DES UNDERCOVERS !";
      title.style.color = "var(--pink)";
      sub.innerText = "Les imposteurs sont restés indétectables jusqu'au bout.";
    }

    document.getElementById("endWordCivil").innerText = gameState ? gameState.civilWord : "-";
    document.getElementById("endWordUndercover").innerText = gameState ? gameState.undercoverWord : "-";

    const grid = document.getElementById("endPlayerGrid");
    grid.innerHTML = "";
    roomPlayers.forEach((p, idx) => {
      const assign = gameState ? gameState.assignments[p.id] : null;
      let roleTag = "👤 Civil";
      if (assign && assign.role === "undercover") roleTag = "🕵️ Undercover (" + assign.word + ")";
      else if (assign && assign.role === "mrwhite") roleTag = "🎭 Mr. White";
      else if (assign) roleTag = "👤 Civil (" + assign.word + ")";

      const card = document.createElement("div");
      card.className = "player-card" + (p.id === myId ? " is-me" : "");
      card.innerHTML = `
        <div class="p-avatar">${AVATARS[idx % AVATARS.length]}</div>
        <div class="p-name">${escapeHtml(p.name)}</div>
        <div class="p-status" style="font-size:0.7rem; color:var(--cyan);">${roleTag}</div>
      `;
      grid.appendChild(card);
    });

    document.getElementById("btnRestartGame").style.display = isHost ? "block" : "none";
  }

  function hostRestartLobby() {
    if (!isHost) return;
    const payload = { t: "restart_lobby" };
    ws.send(JSON.stringify(payload));
    onRestartLobby();
  }

  function onRestartLobby() {
    gameState = null;
    switchView("viewLobby");
    renderLobbyPlayers();
  }

  function switchView(viewId) {
    const views = ["viewLobby", "viewSecret", "viewDiscussion", "viewVote", "viewEnd"];
    views.forEach(v => {
      document.getElementById(v).style.display = (v === viewId) ? "block" : "none";
    });
  }

  function escapeHtml(str) {
    return (str || "").replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
  }

  window.addEventListener("DOMContentLoaded", initWs);
</script>
</body>
</html>
)rawliteral";

#endif
