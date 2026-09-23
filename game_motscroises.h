#ifndef GAME_MOTSCROISES_H
#define GAME_MOTSCROISES_H

#include <pgmspace.h>

const char GAME_MOTSCROISES_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Mots Croisés Cyber - 100 Niveaux</title>
<style>
  :root {
    --bg: #090b12;
    --card: #11162a;
    --card-border: rgba(0, 243, 255, 0.25);
    --cyan: #00f3ff;
    --pink: #ff0055;
    --yellow: #ffe600;
    --green: #00ff66;
    --dim: #8b9bb4;
  }
  * { box-sizing: border-box; margin: 0; padding: 0; -webkit-tap-highlight-color: transparent; }
  body {
    background: var(--bg);
    color: #f0f4fc;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
    display: flex;
    flex-direction: column;
    align-items: center;
    min-height: 100vh;
    padding: 8px 10px 24px;
    touch-action: manipulation;
  }
  header {
    width: 100%;
    max-width: 440px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 8px;
    gap: 6px;
  }
  .btn-back {
    color: var(--cyan);
    text-decoration: none;
    font-weight: 700;
    font-size: 0.85rem;
    padding: 6px 10px;
    border-radius: 8px;
    border: 1px solid rgba(0,243,255,0.3);
    background: rgba(0,243,255,0.08);
    white-space: nowrap;
  }
  .lvl-nav-group {
    display: flex;
    align-items: center;
    gap: 4px;
  }
  .btn-lvl-step {
    background: var(--card);
    border: 1px solid var(--card-border);
    color: var(--cyan);
    font-weight: bold;
    font-size: 0.85rem;
    width: 30px;
    height: 30px;
    border-radius: 8px;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
  }
  .btn-lvl-step:active { transform: scale(0.92); }
  .btn-lvl-modal {
    background: linear-gradient(135deg, rgba(20,26,48,0.9), rgba(11,15,28,0.9));
    border: 1px solid var(--cyan);
    color: #fff;
    padding: 5px 10px;
    border-radius: 8px;
    font-size: 0.82rem;
    font-weight: 800;
    cursor: pointer;
    display: flex;
    align-items: center;
    gap: 4px;
    box-shadow: 0 0 10px rgba(0,243,255,0.25);
  }
  .timer-pill {
    background: rgba(255,230,0,0.1);
    border: 1px solid rgba(255,230,0,0.3);
    color: var(--yellow);
    padding: 5px 8px;
    border-radius: 8px;
    font-family: monospace;
    font-size: 0.88rem;
    font-weight: bold;
    white-space: nowrap;
  }
  .clue-banner {
    width: 100%;
    max-width: 440px;
    background: linear-gradient(135deg, rgba(20,26,48,0.95), rgba(11,15,28,0.95));
    border: 1px solid var(--card-border);
    border-left: 4px solid var(--cyan);
    border-radius: 10px;
    padding: 8px 12px;
    margin-bottom: 8px;
    min-height: 48px;
    display: flex;
    flex-direction: column;
    justify-content: center;
    cursor: pointer;
    box-shadow: 0 4px 15px rgba(0,0,0,0.4);
  }
  .clue-meta {
    font-size: 0.7rem;
    color: var(--cyan);
    text-transform: uppercase;
    font-weight: 800;
    letter-spacing: 0.5px;
    margin-bottom: 2px;
  }
  .clue-text {
    font-size: 0.88rem;
    color: #fff;
    font-weight: 600;
    line-height: 1.25;
  }
  .board-container {
    width: 100%;
    max-width: 340px;
    background: rgba(14, 18, 34, 0.8);
    border: 1px solid rgba(0, 243, 255, 0.2);
    border-radius: 14px;
    padding: 8px;
    box-shadow: 0 8px 30px rgba(0,0,0,0.6);
    margin-bottom: 8px;
  }
  .cw-grid {
    display: grid;
    grid-template-columns: repeat(5, 1fr);
    gap: 4px;
    width: 100%;
  }
  .cw-cell {
    aspect-ratio: 1;
    background: #151b30;
    border: 1px solid rgba(0, 243, 255, 0.25);
    border-radius: 6px;
    position: relative;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 1.25rem;
    font-weight: 900;
    color: #fff;
    cursor: pointer;
    user-select: none;
    transition: all 0.1s ease;
  }
  .cw-cell.black {
    background: #080a12;
    border-color: rgba(255, 255, 255, 0.03);
    cursor: default;
  }
  .cw-cell .cell-num {
    position: absolute;
    top: 2px;
    left: 3px;
    font-size: 0.6rem;
    font-weight: 700;
    color: var(--dim);
    line-height: 1;
    pointer-events: none;
  }
  .cw-cell.in-word {
    background: rgba(0, 243, 255, 0.15);
    border-color: rgba(0, 243, 255, 0.6);
  }
  .cw-cell.active-focus {
    background: rgba(255, 230, 0, 0.25) !important;
    border-color: var(--yellow) !important;
    box-shadow: 0 0 12px rgba(255, 230, 0, 0.5);
    transform: scale(1.02);
    z-index: 2;
  }
  .cw-cell.cell-correct {
    border-color: var(--green) !important;
    color: var(--green);
  }
  .cw-cell.cell-wrong {
    border-color: var(--pink) !important;
    color: var(--pink);
  }

  .action-bar {
    width: 100%;
    max-width: 440px;
    display: flex;
    gap: 6px;
    margin-bottom: 8px;
  }
  .btn-act {
    flex: 1;
    padding: 7px 4px;
    border-radius: 8px;
    font-size: 0.8rem;
    font-weight: 700;
    border: 1px solid rgba(255,255,255,0.15);
    background: rgba(20, 26, 48, 0.9);
    color: #fff;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 4px;
  }
  .btn-act:active { transform: scale(0.96); }
  .btn-check { border-color: var(--cyan); color: var(--cyan); }
  .btn-hint { border-color: var(--yellow); color: var(--yellow); }
  .btn-next { border-color: #a855f7; color: #c084fc; }

  /* Virtual Keyboard */
  .v-kb {
    width: 100%;
    max-width: 440px;
    display: flex;
    flex-direction: column;
    gap: 4px;
    margin-bottom: 8px;
    user-select: none;
  }
  .kb-row {
    display: flex;
    gap: 3px;
    justify-content: center;
    width: 100%;
  }
  .kb-key {
    flex: 1;
    max-width: 36px;
    height: 36px;
    background: #171d33;
    border: 1px solid rgba(255,255,255,0.12);
    border-radius: 6px;
    color: #fff;
    font-size: 0.95rem;
    font-weight: 800;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    box-shadow: 0 2px 4px rgba(0,0,0,0.3);
  }
  .kb-key:active {
    background: var(--cyan);
    color: #000;
    transform: scale(0.95);
  }
  .kb-key.wide {
    flex: 1.5;
    max-width: 52px;
    font-size: 0.78rem;
    background: #202742;
  }

  /* Clues Container */
  .clues-wrapper {
    width: 100%;
    max-width: 440px;
    background: var(--card);
    border: 1px solid var(--card-border);
    border-radius: 10px;
    padding: 8px 10px;
  }
  .clues-nav {
    display: flex;
    gap: 8px;
    margin-bottom: 6px;
    border-bottom: 1px solid rgba(255,255,255,0.1);
    padding-bottom: 4px;
  }
  .clue-tab-btn {
    background: none;
    border: none;
    color: var(--dim);
    font-weight: 800;
    font-size: 0.8rem;
    padding: 3px 6px;
    cursor: pointer;
  }
  .clue-tab-btn.active {
    color: var(--cyan);
    border-bottom: 2px solid var(--cyan);
  }
  .clue-item {
    font-size: 0.78rem;
    padding: 4px 6px;
    border-radius: 6px;
    cursor: pointer;
    margin-bottom: 3px;
    display: flex;
    gap: 5px;
    color: #c5d1e8;
  }
  .clue-item.active {
    background: rgba(0, 243, 255, 0.15);
    color: #fff;
    font-weight: bold;
    border-left: 3px solid var(--cyan);
  }
  .clue-item.solved {
    text-decoration: line-through;
    opacity: 0.6;
  }

  /* Modal 100 Niveaux */
  .modal-overlay {
    display: none;
    position: fixed;
    top: 0; left: 0; right: 0; bottom: 0;
    background: rgba(4, 6, 12, 0.88);
    backdrop-filter: blur(6px);
    z-index: 100;
    align-items: center;
    justify-content: center;
    padding: 16px;
  }
  .modal-box {
    background: linear-gradient(145deg, #131a33, #0d1224);
    border: 2px solid var(--cyan);
    border-radius: 18px;
    padding: 16px 14px;
    max-width: 420px;
    width: 100%;
    max-height: 85vh;
    display: flex;
    flex-direction: column;
    box-shadow: 0 0 35px rgba(0,243,255,0.35);
  }
  .modal-head {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 10px;
    border-bottom: 1px solid rgba(255,255,255,0.1);
    padding-bottom: 6px;
  }
  .modal-head h2 {
    font-size: 1.05rem;
    color: var(--cyan);
    font-weight: 900;
  }
  .btn-close-modal {
    background: none;
    border: none;
    color: var(--dim);
    font-size: 1.4rem;
    cursor: pointer;
  }
  .levels-grid {
    display: grid;
    grid-template-columns: repeat(5, 1fr);
    gap: 6px;
    overflow-y: auto;
    padding-right: 4px;
    max-height: 60vh;
  }
  .lvl-btn {
    aspect-ratio: 1;
    background: #171d33;
    border: 1px solid rgba(255,255,255,0.15);
    border-radius: 8px;
    color: #fff;
    font-size: 0.82rem;
    font-weight: 800;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    transition: all 0.1s;
  }
  .lvl-btn:active { transform: scale(0.95); }
  .lvl-btn.active {
    border-color: var(--yellow) !important;
    background: rgba(255, 230, 0, 0.2) !important;
    color: var(--yellow);
  }
  .lvl-btn.completed {
    border-color: var(--green);
    background: rgba(0, 255, 102, 0.12);
  }

  /* Victory Modal */
  .victory-box {
    background: linear-gradient(145deg, #131a33, #0d1224);
    border: 2px solid var(--green);
    border-radius: 18px;
    padding: 22px 18px;
    max-width: 340px;
    width: 100%;
    text-align: center;
    box-shadow: 0 0 35px rgba(0,255,102,0.4);
  }
  .victory-title {
    font-size: 1.35rem;
    color: var(--yellow);
    font-weight: 900;
    margin-bottom: 6px;
  }
  .victory-text {
    color: var(--dim);
    font-size: 0.85rem;
    margin-bottom: 16px;
  }
  .victory-btn {
    width: 100%;
    padding: 12px;
    background: linear-gradient(135deg, var(--green), #00bbff);
    border: none;
    border-radius: 10px;
    color: #000;
    font-weight: 900;
    font-size: 0.95rem;
    cursor: pointer;
    box-shadow: 0 4px 15px rgba(0,255,102,0.4);
  }
</style>
<script src="/pause.js"></script>
</head>
<body>

<header>
  <a href="/hub" class="btn-back">◀ Menu</a>
  
  <div class="lvl-nav-group">
    <button class="btn-lvl-step" onclick="prevGrid()">◀</button>
    <button class="btn-lvl-modal" onclick="openLevelModal()">
      🏆 <span id="cur-lvl-num">1</span>/100 ▾
    </button>
    <button class="btn-lvl-step" onclick="nextGrid()">▶</button>
  </div>

  <div class="timer-pill" id="timer">00:00</div>
</header>

<div class="clue-banner" id="clue-banner" onclick="toggleDirection()">
  <div class="clue-meta" id="clue-meta">1 HORIZONTAL (5 LETTRES)</div>
  <div class="clue-text" id="clue-text">Définition du mot...</div>
</div>

<div class="board-container">
  <div class="cw-grid" id="cw-grid"></div>
</div>

<div class="action-bar">
  <button class="btn-act btn-check" onclick="checkAnswers()">🔍 Vérifier</button>
  <button class="btn-act btn-hint" onclick="revealCurrentCell()">💡 Indice</button>
  <button class="btn-act btn-next" onclick="nextWord()">⏭️ Suivant</button>
</div>

<div class="v-kb">
  <div class="kb-row">
    <div class="kb-key" onclick="handleVirtualKey('A')">A</div>
    <div class="kb-key" onclick="handleVirtualKey('Z')">Z</div>
    <div class="kb-key" onclick="handleVirtualKey('E')">E</div>
    <div class="kb-key" onclick="handleVirtualKey('R')">R</div>
    <div class="kb-key" onclick="handleVirtualKey('T')">T</div>
    <div class="kb-key" onclick="handleVirtualKey('Y')">Y</div>
    <div class="kb-key" onclick="handleVirtualKey('U')">U</div>
    <div class="kb-key" onclick="handleVirtualKey('I')">I</div>
    <div class="kb-key" onclick="handleVirtualKey('O')">O</div>
    <div class="kb-key" onclick="handleVirtualKey('P')">P</div>
  </div>
  <div class="kb-row">
    <div class="kb-key" onclick="handleVirtualKey('Q')">Q</div>
    <div class="kb-key" onclick="handleVirtualKey('S')">S</div>
    <div class="kb-key" onclick="handleVirtualKey('D')">D</div>
    <div class="kb-key" onclick="handleVirtualKey('F')">F</div>
    <div class="kb-key" onclick="handleVirtualKey('G')">G</div>
    <div class="kb-key" onclick="handleVirtualKey('H')">H</div>
    <div class="kb-key" onclick="handleVirtualKey('J')">J</div>
    <div class="kb-key" onclick="handleVirtualKey('K')">K</div>
    <div class="kb-key" onclick="handleVirtualKey('L')">L</div>
    <div class="kb-key" onclick="handleVirtualKey('M')">M</div>
  </div>
  <div class="kb-row">
    <div class="kb-key wide" onclick="handleVirtualKey('DEL')">⌫</div>
    <div class="kb-key" onclick="handleVirtualKey('W')">W</div>
    <div class="kb-key" onclick="handleVirtualKey('X')">X</div>
    <div class="kb-key" onclick="handleVirtualKey('C')">C</div>
    <div class="kb-key" onclick="handleVirtualKey('V')">V</div>
    <div class="kb-key" onclick="handleVirtualKey('B')">B</div>
    <div class="kb-key" onclick="handleVirtualKey('N')">N</div>
    <div class="kb-key wide" onclick="toggleDirection()">🔄 H/V</div>
  </div>
</div>

<div class="clues-wrapper">
  <div class="clues-nav">
    <button class="clue-tab-btn active" id="tab-across-btn" onclick="setClueTab('across')">➡️ Horizontales</button>
    <button class="clue-tab-btn" id="tab-down-btn" onclick="setClueTab('down')">⬇️ Verticales</button>
  </div>
  <div id="clues-list"></div>
</div>

<!-- Modal 100 Niveaux -->
<div class="modal-overlay" id="levels-modal">
  <div class="modal-box">
    <div class="modal-head">
      <h2>🏆 100 Niveaux de Mots Croisés</h2>
      <button class="btn-close-modal" onclick="closeLevelModal()">✕</button>
    </div>
    <div class="levels-grid" id="levels-grid"></div>
  </div>
</div>

<!-- Victory Modal -->
<div class="modal-overlay" id="victory-modal">
  <div class="victory-box">
    <div style="font-size:2.8rem;margin-bottom:6px;">🎉</div>
    <div class="victory-title">Niveau Complété !</div>
    <div class="victory-text" id="victory-stats">Bravo ! Niveau résolu avec succès.</div>
    <button class="victory-btn" onclick="nextGridAfterWin()">Niveau Suivant ⏭️</button>
  </div>
</div>

<script>
const DICT = {"ACTES": "Actions réelles ou parties d'une pièce de théâtre", "BALLE": "Sphère de jeu rebondissante", "BETES": "Animaux vivants ou personnes peu avisées", "BLOCS": "Gros morceaux solides de pierre ou de bois", "BOITE": "Récipient fermé pour ranger des objets", "BOMBE": "Engin explosif ou arme de jeu rétro", "BONUS": "Récompense ou points supplémentaires dans un jeu", "CABLE": "Faisceau de fils conducteurs sous gaine", "CANAL": "Bande de fréquence de transmission radio", "CARGO": "Grand navire de transport de fret", "CHANT": "Émission musicale de la voix", "COURS": "Leçon d'un professeur ou flux d'eau", "CYBER": "Relatif à l'informatique et aux réseaux", "DAMES": "Jeu de pions sur damier noir et blanc", "DELTA": "Embouchure de fleuve ou lettre grecque", "ECRAN": "Surface vitrée diffusant images et jeux", "EPICE": "Substance aromatique pour la cuisine", "ETAPE": "Phase d'un voyage ou palier d'un tour", "FARCE": "Tour comique ou petite plaisanterie", "FORCE": "Puissance musculaire ou physique", "FORET": "Vaste espace boisé peuplé d'arbres", "FORME": "Contour géométrique d'un corps", "HERBE": "Végétation verte des prairies", "HOTEL": "Établissement offrant des chambres aux voyageurs", "MACRO": "De grande dimension ou commande groupée", "MANGA": "Bande dessinée japonaise", "MATCH": "Rencontre sportive ou duel de jeu", "MICRO": "Microprocesseur ou capteur de son", "MONDE": "La planète Terre et l'humanité", "MOTIF": "Dessin répété ou raison d'agir", "MOTOS": "Véhicules rapides à deux roues", "NOBLE": "De sang aristocratique ou d'esprit élevé", "OCEAN": "Immense étendue d'eau salée", "OMBRE": "Zone obscure protégée de la lumière", "OPERA": "Spectacle musical grandiose théâtral", "ORAGE": "Perturbation météo avec éclairs et tonnerre", "ORDRE": "Organisation méthodique ou commandement", "OREES": "Lisières d'une forêt", "OUEST": "Point cardinal où le soleil se couche", "OUTIL": "Ustensile manuel servant à travailler", "OVALE": "En forme d'œuf allongé", "PAGES": "Feuilles reliées d'un livre", "PIONS": "Pièces déplacées sur un damier", "PLOMB": "Métal gris très lourd", "POIDS": "Force de pesanteur mesurée en kilogrammes", "POINT": "Marque minuscule ou unité gagnée dans un jeu", "RADIO": "Diffusion d'émissions sonores par ondes hertziennes", "RADIS": "Petit légume rouge croquant et piquant", "RAMPE": "Plan incliné facilitant la montée", "REPAS": "Nourriture prise à des heures régulières", "RESET": "Réinitialisation d'un système à son départ", "RESTO": "Lieu convivial où l'on partage un repas", "ROBOT": "Machine autonome programmable", "ROCHE": "Masse minérale solide formant les montagnes", "RUCHE": "Maison des abeilles et du miel", "SALLE": "Pièce fermée d'un bâtiment", "SCENE": "Plateau de théâtre ou espace de jeu", "SCORE": "Total de points enregistrés dans un jeu", "SCOUT": "Éclaireur dans la nature", "SERIE": "Suite ordonnée d'éléments ou d'épisodes", "SIEGE": "Meuble fait pour s'asseoir", "SIGNE": "Geste ou symbole porteur de signification", "SINGE": "Primate agile grimpant dans les arbres", "SMART": "Intelligent, élégant ou connecté", "SOEUR": "Fille née des mêmes parents", "SOLAR": "Relatif au soleil et à son rayonnement", "SOLDE": "Prix réduit ou montant restant", "SONAR": "Appareil acoustique de localisation sous-marine", "SOURI": "Petit rongeur agile", "SPEED": "Vitesse vive et soutenue dans l'action", "SPORT": "Activité physique ou compétition comme l'e-sport", "STAGE": "Niveau de progression dans un jeu vidéo", "START": "Point de départ d'une course", "STEAM": "Vapeur d'eau sous pression ou boutique de jeux", "SUITE": "Succession logique d'éléments coordonnés", "TABLE": "Meuble à plateau plat reposant sur des pieds", "TALON": "Partie arrière de la chaussure ou du pied", "TASSE": "Récipient à anse pour boisson chaude", "TEMPS": "Durée mesurée par les secondes et les heures", "TERRE": "Notre planète bleue d'origine", "TESTE": "Met à l'épreuve pour vérifier", "TIBIA": "Grand os de la jambe", "TISON": "Morceau de bois qui brûle dans l'âtre", "TITRE": "Nom d'une œuvre ou trophée de vainqueur", "TOTAL": "Montant cumulé de tous les éléments réunis", "TRACE": "Empreinte laissée par le passage de quelqu'un", "TRAIN": "Suite de wagons tractés par une locomotive", "TRAIT": "Ligne tracée au crayon ou caractéristique"};
const RAW_LEVELS = [[["ROBOT", "SONAR", "TASSE"], ["RESET", "BONUS", "TERRE"]], [["ROBOT", "SONAR", "TESTE"], ["RESET", "BONUS", "TERRE"]], [["ROBOT", "SPORT", "TASSE"], ["RESET", "BLOCS", "TITRE"]], [["ROBOT", "SPORT", "TESTE"], ["RESET", "BLOCS", "TITRE"]], [["ROBOT", "DELTA", "OUEST"], ["RADIO", "BALLE", "TRAIT"]], [["ROBOT", "DELTA", "OCEAN"], ["RADIO", "BALLE", "TRAIN"]], [["ROBOT", "DELTA", "SCENE"], ["RADIS", "BALLE", "TRACE"]], [["ROBOT", "DELTA", "SIEGE"], ["RADIS", "BALLE", "TRACE"]], [["ROBOT", "SCOUT", "TASSE"], ["RESET", "BLOCS", "TITRE"]], [["ROBOT", "SCOUT", "TESTE"], ["RESET", "BLOCS", "TITRE"]], [["ROBOT", "DAMES", "OCEAN"], ["RADIO", "BOMBE", "TISON"]], [["ROBOT", "DAMES", "SCENE"], ["RADIS", "BOMBE", "TASSE"]], [["ROBOT", "DAMES", "SCENE"], ["RADIS", "BOMBE", "TESTE"]], [["ROBOT", "DAMES", "SIEGE"], ["RADIS", "BOMBE", "TASSE"]], [["ROBOT", "DAMES", "SIEGE"], ["RADIS", "BOMBE", "TESTE"]], [["ROBOT", "POIDS", "SCENE"], ["REPAS", "BOITE", "TASSE"]], [["ROBOT", "POIDS", "SCENE"], ["REPAS", "BOITE", "TESTE"]], [["ROBOT", "POIDS", "SIEGE"], ["REPAS", "BOITE", "TASSE"]], [["ROBOT", "POIDS", "SIEGE"], ["REPAS", "BOITE", "TESTE"]], [["ROBOT", "POINT", "SCENE"], ["REPAS", "BOITE", "TITRE"]], [["ROBOT", "POINT", "SIEGE"], ["REPAS", "BOITE", "TITRE"]], [["TASSE", "MANGA", "SCENE"], ["TEMPS", "SINGE", "ETAPE"]], [["TASSE", "MANGA", "SERIE"], ["TEMPS", "SONAR", "ETAPE"]], [["TASSE", "MANGA", "SIEGE"], ["TEMPS", "SINGE", "ETAPE"]], [["RESET", "SPORT", "OUTIL"], ["RESTO", "SCOUT", "TOTAL"]], [["RESET", "CANAL", "ECRAN"], ["RUCHE", "SONAR", "TALON"]], [["RESET", "CANAL", "ECRAN"], ["ROCHE", "SONAR", "TALON"]], [["RESET", "PIONS", "SCENE"], ["REPAS", "SCORE", "TASSE"]], [["RESET", "PIONS", "SCENE"], ["REPAS", "SCORE", "TESTE"]], [["RESET", "PIONS", "SIEGE"], ["REPAS", "SCORE", "TASSE"]], [["RESET", "PIONS", "SIEGE"], ["REPAS", "SCORE", "TESTE"]], [["RESET", "DELTA", "OUEST"], ["RADIO", "SALLE", "TRAIT"]], [["RESET", "DELTA", "OUEST"], ["RADIO", "SOLDE", "TRAIT"]], [["RESET", "DELTA", "OCEAN"], ["RADIO", "SALLE", "TRAIN"]], [["RESET", "DELTA", "OCEAN"], ["RADIO", "SOLDE", "TRAIN"]], [["RESET", "DELTA", "SCENE"], ["RADIS", "SALLE", "TRACE"]], [["RESET", "DELTA", "SCENE"], ["RADIS", "SOLDE", "TRACE"]], [["RESET", "DELTA", "SERIE"], ["RADIS", "SOLAR", "TRACE"]], [["RESET", "DELTA", "SIEGE"], ["RADIS", "SALLE", "TRACE"]], [["RESET", "DELTA", "SIEGE"], ["RADIS", "SOLDE", "TRACE"]], [["RESET", "COURS", "EPICE"], ["RUCHE", "SOURI", "TASSE"]], [["RESET", "COURS", "EPICE"], ["RUCHE", "SOURI", "TESTE"]], [["RESET", "COURS", "EPICE"], ["ROCHE", "SOURI", "TASSE"]], [["RESET", "COURS", "EPICE"], ["ROCHE", "SOURI", "TESTE"]], [["RESET", "SMART", "OUTIL"], ["RESTO", "START", "TOTAL"]], [["RESET", "PLOMB", "SCENE"], ["REPAS", "SCORE", "TABLE"]], [["RESET", "PLOMB", "SIEGE"], ["REPAS", "SCORE", "TABLE"]], [["RESET", "SOEUR", "ORDRE"], ["RESTO", "SPEED", "TERRE"]], [["RESET", "PAGES", "SCENE"], ["REPAS", "SIGNE", "TASSE"]], [["RESET", "PAGES", "SCENE"], ["REPAS", "SIGNE", "TESTE"]], [["RESET", "PAGES", "SIEGE"], ["REPAS", "SIGNE", "TASSE"]], [["RESET", "PAGES", "SIEGE"], ["REPAS", "SIGNE", "TESTE"]], [["RESET", "MANGA", "ECRAN"], ["RAMPE", "SONAR", "TRAIN"]], [["RESET", "POIDS", "SCENE"], ["REPAS", "SUITE", "TASSE"]], [["RESET", "POIDS", "SCENE"], ["REPAS", "SUITE", "TESTE"]], [["RESET", "POIDS", "SIEGE"], ["REPAS", "SUITE", "TASSE"]], [["RESET", "POIDS", "SIEGE"], ["REPAS", "SUITE", "TESTE"]], [["RESET", "POINT", "SCENE"], ["REPAS", "SUITE", "TITRE"]], [["RESET", "POINT", "SIEGE"], ["REPAS", "SUITE", "TITRE"]], [["RESET", "STEAM", "OREES"], ["RESTO", "SCENE", "TEMPS"]], [["RESET", "STEAM", "OREES"], ["RESTO", "SIEGE", "TEMPS"]], [["BONUS", "OMBRE", "SCENE"], ["BLOCS", "NOBLE", "SIEGE"]], [["BONUS", "TIBIA", "SCENE"], ["BETES", "NOBLE", "STAGE"]], [["BONUS", "TIBIA", "SIEGE"], ["BETES", "NOBLE", "STAGE"]], [["BONUS", "TABLE", "SCENE"], ["BETES", "NOBLE", "SIEGE"]], [["MICRO", "TERRE", "SPORT"], ["MOTOS", "CARGO", "OUEST"]], [["MICRO", "TERRE", "SCOUT"], ["MOTOS", "CARGO", "OUEST"]], [["MICRO", "TIBIA", "FARCE"], ["MOTIF", "CYBER", "ORAGE"]], [["MICRO", "TIBIA", "FARCE"], ["MOTIF", "CYBER", "OVALE"]], [["MICRO", "TIBIA", "SCENE"], ["MOTOS", "CABLE", "ORAGE"]], [["MICRO", "TIBIA", "SCENE"], ["MOTOS", "CABLE", "OVALE"]], [["MICRO", "TIBIA", "SERIE"], ["MOTOS", "CYBER", "ORAGE"]], [["MICRO", "TIBIA", "SERIE"], ["MOTOS", "CYBER", "OVALE"]], [["MICRO", "TIBIA", "SIEGE"], ["MOTOS", "CABLE", "ORAGE"]], [["MICRO", "TIBIA", "SIEGE"], ["MOTOS", "CABLE", "OVALE"]], [["MICRO", "TIBIA", "HERBE"], ["MATCH", "CYBER", "ORAGE"]], [["MICRO", "TIBIA", "HERBE"], ["MATCH", "CYBER", "OVALE"]], [["MICRO", "TIBIA", "FORCE"], ["MOTIF", "CYBER", "ORAGE"]], [["MICRO", "TIBIA", "FORCE"], ["MOTIF", "CYBER", "OVALE"]], [["MICRO", "TIBIA", "FORME"], ["MOTIF", "CYBER", "ORAGE"]], [["MICRO", "TIBIA", "FORME"], ["MOTIF", "CYBER", "OVALE"]], [["MICRO", "TRAIT", "HOTEL"], ["MATCH", "CHANT", "OUTIL"]], [["MICRO", "NOBLE", "ECRAN"], ["MONDE", "CYBER", "OCEAN"]], [["MICRO", "TABLE", "FORET"], ["MOTIF", "CYBER", "OUEST"]], [["SPORT", "OUEST", "TASSE"], ["SCOUT", "OREES", "TITRE"]], [["SPORT", "OUEST", "TESTE"], ["SCOUT", "OREES", "TITRE"]], [["SPORT", "OUEST", "ETAPE"], ["SCORE", "OPERA", "TITRE"]], [["SPORT", "OUEST", "TRACE"], ["SCOUT", "OPERA", "TITRE"]], [["SPORT", "OPERA", "TASSE"], ["SCOUT", "OREES", "TRACE"]], [["SPORT", "OPERA", "TISON"], ["SCOUT", "OREES", "TRAIN"]], [["SPORT", "OPERA", "TESTE"], ["SCOUT", "OREES", "TRACE"]], [["SPORT", "OREES", "ETAPE"], ["SCORE", "OPERA", "TASSE"]], [["SPORT", "OREES", "ETAPE"], ["SCORE", "OPERA", "TESTE"]], [["SPORT", "ACTES", "TALON"], ["SMART", "OUTIL", "TISON"]], [["SPORT", "ACTES", "TALON"], ["START", "OUTIL", "TISON"]], [["MOTOS", "CARGO", "OUEST"], ["MACRO", "TERRE", "SPORT"]], [["MOTOS", "CARGO", "OUEST"], ["MACRO", "TERRE", "SCOUT"]], [["MOTOS", "CYBER", "ORAGE"], ["MACRO", "TIBIA", "SERIE"]], [["MOTOS", "CYBER", "OVALE"], ["MACRO", "TIBIA", "SERIE"]], [["MOTOS", "CABLE", "ORAGE"], ["MACRO", "TIBIA", "SCENE"]]];
const THEMES = ["Rétro Arcade", "Cyber & Tech", "Espace & Énergie", "Océan & Mystère", "Forêt & Aventure", "Damier Électronique", "Vitesse & Circuit", "Laboratoire Secret", "Forteresse Médiévale", "Galaxie Lointaine", "Pixel Art", "Exploration Lunaire", "Alchimie Ancienne", "Mission Spatiale", "Profondeurs Marines", "Île Mystérieuse", "Code Source", "Réseau Cybernétique", "Robotique Avancée", "Constellation", "Temple Oublié", "Sanctuaire Zen", "Chemin des Étoiles", "Jardin Botanique", "Base Polaire", "Vaisseau Mère", "Planète Sauvage", "Cité Futuriste", "Royaume des Brumes", "Énigme d'Orient", "Puits Quantique", "Oasis Enchantée", "Monde Virtuel", "Dôme de Cristal", "Machine à Vapeur", "Soleil de Minuit", "Canyon Écarlate", "Signal Radio", "Horizon Infini", "Aurore Polaire", "Hangar Spatial", "Vallée Silencieuse", "Labyrinthe d'Or", "Récif de Corail", "Station Orbitale", "Mine de Diamant", "Forêt d'Émeraude", "Bouclier Plasma", "Vortex Temporel", "Sommet Enneigé", "Cœur d'Énergie", "Ruines Antiques", "Rivage Bleu", "Sphère Céleste", "Sentier Perdu", "Satellite Secret", "Terre d'Aventure", "Passage Secret", "Désert Doré", "Cratère Lunaire", "Flamme Éternelle", "Jungle Tropicale", "Porte des Mondes", "Titan d'Acier", "Comète Brillante", "Boussole Magique", "Royaume Aquatique", "Volcan Actif", "Ciel Étoilé", "Îlot Isolé", "Sentinelle Cyber", "Archipel Éloigné", "Canyon Profond", "Cascade Cachée", "Matrice Neuronale", "Station Sous-Marine", "Cité Suspendue", "Brume Mystique", "Grotte de Cristal", "Étoile Filante", "Forêt de Séquoias", "Château Fort", "Phare de la Nuit", "Ciel Boréal", "Monde Sous-Marin", "Dune Infinie", "Miroir d'Eau", "Chemin de Fer", "Tour d'Ivoire", "Horloge Céleste", "Sphère d'Énergie", "Cœur de Lion", "Fleur de Lotus", "Vapeur Cyber", "Sanctuaire Sacré", "Boussole Dorée", "Vague Géante", "Couronne Royale", "Éclipse Solaire", "Maître des Mots"];

let curLvlIdx = parseInt(localStorage.getItem('cw_last_lvl') || '0');
if (curLvlIdx >= 100) curLvlIdx = 0;

let curGrid = null;
let boardLetters = [];
let solutionLetters = [];
let cellNumbers = [];
let activeRow = 0, activeCol = 0;
let activeDir = 'across';
let curClueTab = 'across';
let timerSec = 0, timerTimer = null;
let gameWon = false;

// Audio Synth Procedural
const AudioCtx = window.AudioContext || window.webkitAudioContext;
let audioCtx = null;
function playTone(freq, dur, type='sine') {
  try {
    if (!audioCtx) audioCtx = new AudioCtx();
    const osc = audioCtx.createOscillator();
    const gain = audioCtx.createGain();
    osc.type = type;
    osc.frequency.setValueAtTime(freq, audioCtx.currentTime);
    gain.gain.setValueAtTime(0.08, audioCtx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, audioCtx.currentTime + dur);
    osc.connect(gain);
    gain.connect(audioCtx.destination);
    osc.start();
    osc.stop(audioCtx.currentTime + dur);
  } catch(e){}
}

function playWinSound() {
  [523.25, 659.25, 783.99, 1046.50].forEach((freq, i) => {
    setTimeout(() => playTone(freq, 0.25, 'triangle'), i * 110);
  });
}

function buildGrid(idx) {
  const pair = RAW_LEVELS[idx];
  const aWords = pair[0];
  const dWords = pair[1];
  return {
    id: idx + 1,
    title: (idx + 1) + '. ' + THEMES[idx],
    rows: 5,
    cols: 5,
    across: [
      { num: 1, r: 0, c: 0, word: aWords[0], clue: DICT[aWords[0]] },
      { num: 4, r: 2, c: 0, word: aWords[1], clue: DICT[aWords[1]] },
      { num: 5, r: 4, c: 0, word: aWords[2], clue: DICT[aWords[2]] }
    ],
    down: [
      { num: 1, r: 0, c: 0, word: dWords[0], clue: DICT[dWords[0]] },
      { num: 2, r: 0, c: 2, word: dWords[1], clue: DICT[dWords[1]] },
      { num: 3, r: 0, c: 4, word: dWords[2], clue: DICT[dWords[2]] }
    ]
  };
}

function initGrid() {
  curGrid = buildGrid(curLvlIdx);
  localStorage.setItem('cw_last_lvl', curLvlIdx);
  document.getElementById('cur-lvl-num').textContent = curGrid.id;

  boardLetters = Array(5).fill(null).map(() => Array(5).fill(null));
  solutionLetters = Array(5).fill(null).map(() => Array(5).fill(null));
  cellNumbers = Array(5).fill(null).map(() => Array(5).fill(null));
  gameWon = false;

  curGrid.across.forEach(item => {
    cellNumbers[item.r][item.c] = item.num;
    for (let i = 0; i < item.word.length; i++) {
      solutionLetters[item.r][item.c + i] = item.word[i];
      boardLetters[item.r][item.c + i] = '';
    }
  });

  curGrid.down.forEach(item => {
    if (!cellNumbers[item.r][item.c]) cellNumbers[item.r][item.c] = item.num;
    for (let i = 0; i < item.word.length; i++) {
      solutionLetters[item.r + i][item.c] = item.word[i];
      boardLetters[item.r + i][item.c] = '';
    }
  });

  activeRow = 0;
  activeCol = 0;
  activeDir = 'across';
  renderBoard();
  renderClues();
  updateClueBanner();

  clearInterval(timerTimer);
  timerSec = 0;
  updateTimerDisplay();
  timerTimer = setInterval(() => {
    timerSec++;
    updateTimerDisplay();
  }, 1000);
}

function updateTimerDisplay() {
  const m = String(Math.floor(timerSec / 60)).padStart(2, '0');
  const s = String(timerSec % 60).padStart(2, '0');
  document.getElementById('timer').textContent = `${m}:${s}`;
}

function prevGrid() {
  curLvlIdx = (curLvlIdx - 1 + 100) % 100;
  initGrid();
}

function nextGrid() {
  curLvlIdx = (curLvlIdx + 1) % 100;
  initGrid();
}

function nextGridAfterWin() {
  document.getElementById('victory-modal').style.display = 'none';
  nextGrid();
}

function selectGrid(idx) {
  curLvlIdx = idx;
  closeLevelModal();
  initGrid();
}

function openLevelModal() {
  const grid = document.getElementById('levels-grid');
  grid.innerHTML = '';
  const completed = JSON.parse(localStorage.getItem('cw_done') || '[]');

  for (let i = 0; i < 100; i++) {
    const btn = document.createElement('button');
    btn.className = 'lvl-btn';
    if (i === curLvlIdx) btn.classList.add('active');
    const isDone = completed.includes(i + 1);
    if (isDone) btn.classList.add('completed');
    btn.innerHTML = `<span>${i + 1}</span>${isDone ? '<span style="color:#00ff66;font-size:0.6rem;">✓</span>' : ''}`;
    btn.onclick = () => selectGrid(i);
    grid.appendChild(btn);
  }

  document.getElementById('levels-modal').style.display = 'flex';
}

function closeLevelModal() {
  document.getElementById('levels-modal').style.display = 'none';
}

function getWordAt(r, c, dir) {
  const list = (dir === 'across') ? curGrid.across : curGrid.down;
  for (const item of list) {
    if (dir === 'across') {
      if (r === item.r && c >= item.c && c < item.c + item.word.length) return item;
    } else {
      if (c === item.c && r >= item.r && r < item.r + item.word.length) return item;
    }
  }
  return null;
}

function getWordCells(wordItem, dir) {
  const cells = [];
  for (let i = 0; i < wordItem.word.length; i++) {
    if (dir === 'across') cells.push({ r: wordItem.r, c: wordItem.c + i });
    else cells.push({ r: wordItem.r + i, c: wordItem.c });
  }
  return cells;
}

function handleCellClick(r, c) {
  if (solutionLetters[r][c] === null) return;

  if (activeRow === r && activeCol === c) {
    const hasAcross = getWordAt(r, c, 'across') !== null;
    const hasDown = getWordAt(r, c, 'down') !== null;
    if (hasAcross && hasDown) {
      activeDir = (activeDir === 'across') ? 'down' : 'across';
    }
  } else {
    activeRow = r;
    activeCol = c;
    if (!getWordAt(r, c, activeDir)) {
      activeDir = (activeDir === 'across') ? 'down' : 'across';
    }
  }

  playTone(400, 0.04, 'triangle');
  renderBoard();
  updateClueBanner();
  renderClues();
}

function toggleDirection() {
  activeDir = (activeDir === 'across') ? 'down' : 'across';
  playTone(440, 0.04, 'sine');
  renderBoard();
  updateClueBanner();
  renderClues();
}

function updateClueBanner() {
  const curWord = getWordAt(activeRow, activeCol, activeDir);
  if (!curWord) return;
  const dirName = (activeDir === 'across') ? 'HORIZONTAL' : 'VERTICAL';
  document.getElementById('clue-meta').textContent = `NIV. ${curGrid.id} • ${curWord.num} ${dirName} (${curWord.word.length} LETTRES)`;
  document.getElementById('clue-text').textContent = curWord.clue;
}

function renderBoard() {
  const gridEl = document.getElementById('cw-grid');
  gridEl.innerHTML = '';

  const curWord = getWordAt(activeRow, activeCol, activeDir);
  const wordCells = curWord ? getWordCells(curWord, activeDir) : [];

  for (let r = 0; r < 5; r++) {
    for (let c = 0; c < 5; c++) {
      const cell = document.createElement('div');
      cell.className = 'cw-cell';

      if (solutionLetters[r][c] === null) {
        cell.classList.add('black');
      } else {
        if (cellNumbers[r][c]) {
          const numSpan = document.createElement('span');
          numSpan.className = 'cell-num';
          numSpan.textContent = cellNumbers[r][c];
          cell.appendChild(numSpan);
        }

        const isFocus = (r === activeRow && c === activeCol);
        const isInWord = wordCells.some(cellCoord => cellCoord.r === r && cellCoord.c === c);

        if (isFocus) cell.classList.add('active-focus');
        else if (isInWord) cell.classList.add('in-word');

        cell.appendChild(document.createTextNode(boardLetters[r][c] || ''));
        cell.onclick = () => handleCellClick(r, c);
      }
      gridEl.appendChild(cell);
    }
  }
}

function renderClues() {
  const listEl = document.getElementById('clues-list');
  listEl.innerHTML = '';

  const curWord = getWordAt(activeRow, activeCol, activeDir);
  const items = (curClueTab === 'across') ? curGrid.across : curGrid.down;

  items.forEach(item => {
    const div = document.createElement('div');
    div.className = 'clue-item';
    const isActive = (curWord && curWord.num === item.num && activeDir === curClueTab);
    if (isActive) div.classList.add('active');

    const cells = getWordCells(item, curClueTab);
    const userStr = cells.map(pt => boardLetters[pt.r][pt.c]).join('');
    if (userStr === item.word) div.classList.add('solved');

    div.innerHTML = `<strong>${item.num}.</strong> <span>${item.clue}</span> <span style="margin-left:auto;color:var(--dim);font-size:0.75rem;">(5)</span>`;
    div.onclick = () => {
      activeRow = item.r;
      activeCol = item.c;
      activeDir = curClueTab;
      playTone(420, 0.04, 'triangle');
      renderBoard();
      updateClueBanner();
      renderClues();
    };
    listEl.appendChild(div);
  });
}

function setClueTab(tab) {
  curClueTab = tab;
  document.getElementById('tab-across-btn').classList.toggle('active', tab === 'across');
  document.getElementById('tab-down-btn').classList.toggle('active', tab === 'down');
  renderClues();
}

function handleVirtualKey(key) {
  if (gameWon) return;
  if (solutionLetters[activeRow][activeCol] === null) return;

  if (key === 'DEL') {
    if (boardLetters[activeRow][activeCol] === '') {
      stepCell(-1);
    }
    boardLetters[activeRow][activeCol] = '';
    playTone(250, 0.05, 'square');
    renderBoard();
    return;
  }

  const char = key.toUpperCase();
  boardLetters[activeRow][activeCol] = char;
  playTone(550, 0.04, 'sine');
  renderBoard();

  stepCell(1);
  checkFullWin();
}

function stepCell(delta) {
  const curWord = getWordAt(activeRow, activeCol, activeDir);
  if (!curWord) return;
  const cells = getWordCells(curWord, activeDir);
  const curIdx = cells.findIndex(pt => pt.r === activeRow && pt.c === activeCol);

  if (curIdx !== -1) {
    const nextIdx = curIdx + delta;
    if (nextIdx >= 0 && nextIdx < cells.length) {
      activeRow = cells[nextIdx].r;
      activeCol = cells[nextIdx].c;
      renderBoard();
      updateClueBanner();
      renderClues();
    }
  }
}

function nextWord() {
  const list = (activeDir === 'across') ? curGrid.across : curGrid.down;
  const curWord = getWordAt(activeRow, activeCol, activeDir);
  let nextIdx = 0;
  if (curWord) {
    const idx = list.findIndex(w => w.num === curWord.num);
    if (idx !== -1 && idx < list.length - 1) nextIdx = idx + 1;
    else {
      activeDir = (activeDir === 'across') ? 'down' : 'across';
      const otherList = (activeDir === 'across') ? curGrid.across : curGrid.down;
      activeRow = otherList[0].r;
      activeCol = otherList[0].c;
      renderBoard();
      updateClueBanner();
      renderClues();
      return;
    }
  }
  activeRow = list[nextIdx].r;
  activeCol = list[nextIdx].c;
  renderBoard();
  updateClueBanner();
  renderClues();
}

function revealCurrentCell() {
  if (gameWon) return;
  if (solutionLetters[activeRow][activeCol] === null) return;
  boardLetters[activeRow][activeCol] = solutionLetters[activeRow][activeCol];
  playTone(600, 0.08, 'sine');
  renderBoard();
  stepCell(1);
  checkFullWin();
}

function checkAnswers() {
  const gridCells = document.querySelectorAll('.cw-cell:not(.black)');
  let idx = 0;
  let allCorrect = true;

  for (let r = 0; r < 5; r++) {
    for (let c = 0; c < 5; c++) {
      if (solutionLetters[r][c] !== null) {
        const domCell = gridCells[idx];
        const val = boardLetters[r][c];
        if (val) {
          if (val === solutionLetters[r][c]) {
            domCell.classList.add('cell-correct');
          } else {
            domCell.classList.add('cell-wrong');
            allCorrect = false;
          }
        } else {
          allCorrect = false;
        }
        idx++;
      }
    }
  }

  if (allCorrect) {
    triggerWin();
  } else {
    playTone(200, 0.15, 'sawtooth');
    setTimeout(() => {
      document.querySelectorAll('.cell-correct, .cell-wrong').forEach(el => {
        el.classList.remove('cell-correct', 'cell-wrong');
      });
    }, 1600);
  }
}

function checkFullWin() {
  for (let r = 0; r < 5; r++) {
    for (let c = 0; c < 5; c++) {
      if (solutionLetters[r][c] !== null) {
        if (boardLetters[r][c] !== solutionLetters[r][c]) return;
      }
    }
  }
  triggerWin();
}

function triggerWin() {
  if (gameWon) return;
  gameWon = true;
  clearInterval(timerTimer);
  playWinSound();

  const completed = JSON.parse(localStorage.getItem('cw_done') || '[]');
  if (!completed.includes(curGrid.id)) {
    completed.push(curGrid.id);
    localStorage.setItem('cw_done', JSON.stringify(completed));
  }

  const m = String(Math.floor(timerSec / 60)).padStart(2, '0');
  const s = String(timerSec % 60).padStart(2, '0');
  document.getElementById('victory-stats').textContent = `Félicitations ! Vous avez résolu « ${curGrid.title} » en ${m}:${s}.`;
  document.getElementById('victory-modal').style.display = 'flex';
}

window.addEventListener('keydown', (e) => {
  if (gameWon) return;
  const k = e.key;
  if (/^[a-zA-Z]$/.test(k)) {
    handleVirtualKey(k.toUpperCase());
    e.preventDefault();
  } else if (k === 'Backspace') {
    handleVirtualKey('DEL');
    e.preventDefault();
  } else if (k === ' ' || k === 'Enter') {
    toggleDirection();
    e.preventDefault();
  } else if (k === 'Tab') {
    nextWord();
    e.preventDefault();
  } else if (k === 'ArrowLeft') {
    if (activeCol > 0 && solutionLetters[activeRow][activeCol - 1] !== null) {
      activeCol--;
      renderBoard(); updateClueBanner(); renderClues();
    }
    e.preventDefault();
  } else if (k === 'ArrowRight') {
    if (activeCol < 4 && solutionLetters[activeRow][activeCol + 1] !== null) {
      activeCol++;
      renderBoard(); updateClueBanner(); renderClues();
    }
    e.preventDefault();
  } else if (k === 'ArrowUp') {
    if (activeRow > 0 && solutionLetters[activeRow - 1][activeCol] !== null) {
      activeRow--;
      renderBoard(); updateClueBanner(); renderClues();
    }
    e.preventDefault();
  } else if (k === 'ArrowDown') {
    if (activeRow < 4 && solutionLetters[activeRow + 1][activeCol] !== null) {
      activeRow++;
      renderBoard(); updateClueBanner(); renderClues();
    }
    e.preventDefault();
  }
});

initGrid();
</script>
</body>
</html>
)rawliteral";

#endif
