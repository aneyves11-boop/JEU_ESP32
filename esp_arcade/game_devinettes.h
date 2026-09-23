#ifndef GAME_DEVINETTES_H
#define GAME_DEVINETTES_H

#include <pgmspace.h>

const char GAME_DEVINETTES_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Cyber Devinettes 500</title>
<style>
  :root {
    --bg: #090b12;
    --card: #11162a;
    --card-border: rgba(0, 243, 255, 0.25);
    --cyan: #00f3ff;
    --pink: #ff0055;
    --yellow: #ffe600;
    --green: #00ff66;
    --purple: #a855f7;
    --dim: #8b9bb4;
  }
  * { box-sizing: border-box; margin: 0; padding: 0; -webkit-tap-highlight-color: transparent; }
  body {
    background: var(--bg);
    color: #f0f4fc;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
    display: flex;
    flex-direction: column;
    align-items: center;
    min-height: 100vh;
    padding: 10px 12px 30px;
    touch-action: manipulation;
  }
  header {
    width: 100%;
    max-width: 460px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 12px;
    gap: 8px;
  }
  .btn-back {
    color: var(--cyan);
    text-decoration: none;
    font-weight: 700;
    font-size: 0.88rem;
    padding: 6px 12px;
    border-radius: 8px;
    border: 1px solid rgba(0,243,255,0.3);
    background: rgba(0,243,255,0.08);
    white-space: nowrap;
  }
  .score-pill {
    background: rgba(255,230,0,0.1);
    border: 1px solid rgba(255,230,0,0.3);
    color: var(--yellow);
    padding: 6px 10px;
    border-radius: 8px;
    font-family: monospace;
    font-size: 0.92rem;
    font-weight: bold;
    white-space: nowrap;
  }
  .streak-pill {
    background: rgba(255,0,85,0.15);
    border: 1px solid rgba(255,0,85,0.35);
    color: #ff3377;
    padding: 6px 10px;
    border-radius: 8px;
    font-size: 0.85rem;
    font-weight: 800;
    white-space: nowrap;
    display: flex;
    align-items: center;
    gap: 4px;
  }
  
  /* Meta bar */
  .meta-bar {
    width: 100%;
    max-width: 460px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 10px;
  }
  .cat-badge {
    background: rgba(168,85,247,0.15);
    border: 1px solid rgba(168,85,247,0.4);
    color: #c084fc;
    font-size: 0.75rem;
    font-weight: 800;
    text-transform: uppercase;
    letter-spacing: 0.5px;
    padding: 4px 10px;
    border-radius: 20px;
  }
  .prog-badge {
    color: var(--dim);
    font-size: 0.82rem;
    font-weight: bold;
  }

  /* Question Card */
  .q-card {
    width: 100%;
    max-width: 460px;
    background: linear-gradient(145deg, rgba(20,26,48,0.95), rgba(11,15,28,0.95));
    border: 1px solid var(--card-border);
    border-top: 2px solid var(--cyan);
    border-radius: 16px;
    padding: 18px 16px;
    margin-bottom: 14px;
    min-height: 110px;
    display: flex;
    align-items: center;
    justify-content: center;
    text-align: center;
    box-shadow: 0 10px 30px rgba(0,0,0,0.5), inset 0 1px 0 rgba(255,255,255,0.1);
  }
  .q-text {
    font-size: 1.12rem;
    font-weight: 700;
    line-height: 1.4;
    color: #fff;
  }

  /* Answer Options Grid */
  .options-grid {
    width: 100%;
    max-width: 460px;
    display: grid;
    grid-template-columns: 1fr;
    gap: 10px;
    margin-bottom: 16px;
  }
  .opt-btn {
    background: rgba(17, 22, 42, 0.9);
    border: 1px solid rgba(0, 243, 255, 0.2);
    border-radius: 12px;
    padding: 14px 16px;
    color: #e2e8f0;
    font-size: 1rem;
    font-weight: 700;
    text-align: left;
    cursor: pointer;
    display: flex;
    align-items: center;
    gap: 12px;
    transition: all 0.15s ease;
    box-shadow: 0 4px 12px rgba(0,0,0,0.3);
  }
  .opt-btn:active {
    transform: scale(0.98);
  }
  .opt-letter {
    width: 28px;
    height: 28px;
    border-radius: 8px;
    background: rgba(0, 243, 255, 0.1);
    border: 1px solid rgba(0, 243, 255, 0.3);
    color: var(--cyan);
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 0.85rem;
    font-weight: 900;
    flex-shrink: 0;
  }
  .opt-btn.correct {
    background: rgba(0, 255, 102, 0.2) !important;
    border-color: var(--green) !important;
    color: #fff !important;
    box-shadow: 0 0 20px rgba(0,255,102,0.4) !important;
  }
  .opt-btn.correct .opt-letter {
    background: var(--green);
    color: #000;
    border-color: var(--green);
  }
  .opt-btn.wrong {
    background: rgba(255, 0, 85, 0.2) !important;
    border-color: var(--pink) !important;
    color: #fff !important;
    box-shadow: 0 0 20px rgba(255,0,85,0.4) !important;
  }
  .opt-btn.wrong .opt-letter {
    background: var(--pink);
    color: #fff;
    border-color: var(--pink);
  }
  .opt-btn.eliminated {
    opacity: 0.25;
    pointer-events: none;
    transform: scale(0.97);
  }

  /* Lifelines & Actions */
  .actions-bar {
    width: 100%;
    max-width: 460px;
    display: flex;
    gap: 10px;
    margin-bottom: 14px;
  }
  .act-btn {
    flex: 1;
    padding: 10px 12px;
    border-radius: 10px;
    font-size: 0.88rem;
    font-weight: 800;
    border: 1px solid rgba(255,255,255,0.15);
    background: rgba(20, 26, 48, 0.9);
    color: #fff;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 6px;
    transition: all 0.15s;
  }
  .act-btn:active { transform: scale(0.96); }
  .btn-5050 {
    border-color: var(--yellow);
    color: var(--yellow);
    background: rgba(255,230,0,0.08);
  }
  .btn-5050.used {
    opacity: 0.4;
    cursor: default;
    border-color: var(--dim);
    color: var(--dim);
  }
  .btn-skip {
    border-color: var(--cyan);
    color: var(--cyan);
    background: rgba(0,243,255,0.08);
  }

  /* Stats footer */
  .stats-card {
    width: 100%;
    max-width: 460px;
    background: rgba(14, 18, 34, 0.8);
    border: 1px solid rgba(255, 255, 255, 0.08);
    border-radius: 12px;
    padding: 10px 16px;
    display: flex;
    justify-content: space-around;
    align-items: center;
    font-size: 0.8rem;
    color: var(--dim);
  }
  .stat-val {
    font-weight: 900;
    color: #fff;
    font-size: 0.95rem;
  }
</style>
<script src="/pause.js"></script>
</head>
<body>

<header>
  <a href="/hub" class="btn-back">◀ Menu</a>
  <div class="streak-pill" id="streak-pill">🔥 x0</div>
  <div class="score-pill" id="score-pill">0 PTS</div>
</header>

<div class="meta-bar">
  <span class="cat-badge" id="cat-badge">Énigmes</span>
  <span class="prog-badge" id="prog-badge">Question 1 / 500</span>
</div>

<div class="q-card">
  <div class="q-text" id="q-text">Chargement de la devinette...</div>
</div>

<div class="options-grid" id="options-grid"></div>

<div class="actions-bar">
  <button class="act-btn btn-5050" id="btn-5050" onclick="use5050()">💡 Joker 50/50 (<span id="joker-count">3</span>)</button>
  <button class="act-btn btn-skip" onclick="skipQuestion()">⏭️ Suivante</button>
</div>

<div class="stats-card">
  <div>Précision: <span class="stat-val" id="stat-acc">0%</span></div>
  <div>Record Streak: <span class="stat-val" id="stat-best-streak">0</span></div>
  <div>Meilleur Score: <span class="stat-val" id="stat-high-score">0</span></div>
</div>

<script>
// Pool de 500 questions aléatoires
const ALL_QUESTIONS = [["Qu'est-ce qui a des dents mais ne mord jamais ?", "Un peigne", "Une scie", "Une fourchette", "Un râteau", "Énigmes"], ["Qu'est-ce qui devient plus mouillé au fur et à mesure qu'il sèche ?", "Une serviette", "Une éponge", "Un savon", "Un parapluie", "Énigmes"], ["Qu'est-ce qui vous appartient, mais que les autres utilisent plus que vous ?", "Votre prénom", "Votre voiture", "Votre téléphone", "Votre voix", "Énigmes"], ["Plus j'ai de gardiens, moins je suis en sécurité. Moins j'en ai, plus je le suis. Que suis-je ?", "Un secret", "Un trésor", "Une prison", "Un coffre-fort", "Énigmes"], ["Je commence la nuit et je termine le matin, qui suis-je ?", "La lettre N", "Le sommeil", "Le soleil", "La lune", "Énigmes"], ["Qu'est-ce qui peut faire le tour du monde en restant toujours dans son coin ?", "Un timbre", "Une boussole", "Un satellite", "Un globe", "Énigmes"], ["Qu'est-ce qui a un œil mais ne peut rien voir ?", "Une aiguille", "Un cyclone", "Une serrure", "Une pomme de terre", "Énigmes"], ["Qu'est-ce qui a des pieds mais ne marche jamais ?", "Une table", "Un arbre", "Une montagne", "Un lit", "Énigmes"], ["Qu'est-ce qui monte et descend sans jamais bouger ?", "Un escalier", "Un ascenseur", "La température", "Le thermomètre", "Énigmes"], ["Je parle toutes les langues du monde sans jamais avoir appris un mot. Qui suis-je ?", "L'écho", "Le perroquet", "Le traducteur", "Le vent", "Énigmes"], ["Plus on m'en retire, plus je deviens grand. Que suis-je ?", "Un trou", "Un tas de sable", "Le silence", "Un gouffre", "Énigmes"], ["Qu'est-ce qui a un cou sans avoir de tête ?", "Une bouteille", "Une chemise", "Une girafe", "Une guitare", "Énigmes"], ["Qu'est-ce qui court toujours sans jamais avoir de jambes ?", "Une rivière", "Le vent", "Le temps", "La montre", "Énigmes"], ["Si vous me donnez à manger je vis, si vous me donnez à boire je meurs. Que suis-je ?", "Le feu", "Une plante", "Une éponge", "La terre", "Énigmes"], ["Qu'est-ce qui a des clés mais aucune serrure à ouvrir ?", "Un piano", "Un concierge", "Un trousseau", "Une carte", "Énigmes"], ["Qu'est-ce qui est toujours devant vous mais que vous ne pouvez jamais voir ?", "L'avenir", "Votre nez", "L'air", "Votre ombre", "Énigmes"], ["Qu'est-ce qui a des villes sans maisons, des forêts sans arbres et de l'eau sans poissons ?", "Une carte géographique", "Un tableau", "Un mirage", "Un écran de cinéma", "Énigmes"], ["Je vous suis partout le jour, mais je disparais quand le soleil s'en va. Qui suis-je ?", "Votre ombre", "Votre reflet", "Votre souffle", "Vos pas", "Énigmes"], ["Qu'est-ce qui se casse dès qu'on prononce son nom ?", "Le silence", "Un secret", "Une promesse", "Le verre", "Énigmes"], ["Je pèse moins lourd qu'une plume, mais l'homme le plus fort ne peut me retenir plus de quelques minutes. Que suis-je ?", "Le souffle", "La fumée", "La pensée", "L'eau", "Énigmes"], ["Combien de mois dans l'année comptent au moins 28 jours ?", "Tous les 12 mois", "Seulement 1 (février)", "7 mois", "6 mois", "Énigmes"], ["Qu'est-ce qui monte quand la pluie tombe ?", "Un parapluie", "L'eau des égouts", "Le brouillard", "La boue", "Énigmes"], ["Qu'est-ce qui a un lit mais ne dort jamais ?", "Une rivière", "Un ruisseau", "Un wagon", "Un hôtel", "Énigmes"], ["Qu'est-ce qui traverse les vitres sans jamais les briser ?", "La lumière", "Le vent", "Le son", "Le froid", "Énigmes"], ["J'ai quatre jambes le matin, deux le midi et trois le soir. Qui suis-je ?", "L'être humain", "Le sphinx", "Le chien", "La grenouille", "Énigmes"], ["Qu'est-ce qui s'ouvre sans clé et se ferme sans bruit ?", "Les yeux", "Une porte", "Une bouche", "Un livre", "Énigmes"], ["Qu'est-ce qui est plein de trous mais retient quand même l'eau ?", "Une éponge", "Une passoire", "Un filet", "Une écumoire", "Énigmes"], ["On me trouve deux fois dans une semaine, une fois dans une année, mais jamais dans un jour. Qui suis-je ?", "La lettre E", "Le repos", "Le chiffre 7", "Le calendrier", "Énigmes"], ["Qu'est-ce qui a des bras mais pas de mains ?", "Un fauteuil", "Une rivière", "Un arbre", "Une chemise", "Énigmes"], ["Qu'est-ce qui s'allume sans feu et s'éteint sans eau ?", "Une idée", "Une ampoule", "Une étoile", "Le soleil", "Énigmes"], ["Je suis noir quand je suis propre et blanc quand je suis sale. Qui suis-je ?", "Le tableau noir", "Un crayon", "Un pneu", "Un chat", "Énigmes"], ["Qu'est-ce qui tombe sans jamais se faire mal ?", "La neige", "La pluie", "La nuit", "La feuille", "Énigmes"], ["Qu'est-ce qui s'achète au mètre et se mange au centimètre ?", "Le boudin", "La corde", "Le ruban", "La réglisse", "Énigmes"], ["Qu'est-ce qui a un chapeau mais pas de tête ?", "Un champignon", "Une cheminée", "Un clou", "Une bouteille", "Énigmes"], ["Qu'est-ce qui vous suit partout sans jamais faire un seul pas ?", "Votre ombre", "Votre reflet", "Votre nom", "Votre passé", "Énigmes"], ["Qu'est-ce qui a des dents, un dos, mais pas de ventre ?", "Une scie", "Un peigne", "Un requin", "Un râteau", "Énigmes"], ["Qu'est-ce qui perd sa tête le matin et la retrouve le soir ?", "Un oreiller", "Une poule", "Un lit", "Une brosse", "Énigmes"], ["Qu'est-ce qui a un cœur qui ne bat pas ?", "Un artichaut", "Une pierre", "Un palmier", "Une laitue", "Énigmes"], ["Qu'est-ce qui est plus grand que la tour Eiffel mais ne pèse rien du tout ?", "Son ombre", "Son reflet", "Le ciel au-dessus", "La nuit", "Énigmes"], ["Qu'est-ce qui court plus vite qu'un cheval sans jamais bouger de place ?", "La pensée", "L'éclair", "Le vent", "L'imagination", "Énigmes"], ["Qu'est-ce qui commence par un 'e', finit par un 'e' et ne contient qu'une seule lettre ?", "Une enveloppe", "Une éponge", "Une épée", "Une étoile", "Énigmes"], ["Qu'est-ce qui a deux cornes mais ne pique pas ?", "Un escargot", "Une chèvre", "Un taureau", "Un croissant", "Énigmes"], ["Qu'est-ce qui ne respire pas mais a pourtant une gorge ?", "Un ruisseau", "Une bouteille", "Une caverne", "Un entonnoir", "Énigmes"], ["Qu'est-ce qui a un dos et un ventre, mais ni bras ni jambes ?", "Un livre", "Un coussin", "Un poisson", "Une armoire", "Énigmes"], ["Qu'est-ce qui se lève sans faire de bruit et réchauffe toute la Terre ?", "Le soleil", "La vapeur", "Le vent", "L'aurore", "Énigmes"], ["Qu'est-ce qui ne fait jamais de bruit en se réveillant mais réveille tout le monde ?", "Le soleil", "Le coq", "Le tonnerre", "Le réveil", "Énigmes"], ["Qu'est-ce qui a des feuilles sans être un arbre ?", "Un cahier", "Une porte", "Une salade", "Une table", "Énigmes"], ["Qu'est-ce qui a une langue mais ne peut ni goûter ni parler ?", "Une chaussure", "Une cloche", "Une balance", "Une serrure", "Énigmes"], ["Qu'est-ce qui peut être rempli de vent sans jamais éclater ?", "Une voile", "Un ballon", "Un pneu", "Un sac plastique", "Énigmes"], ["Qu'est-ce qui entre dans l'eau rouge et en ressort noir ?", "Un fer chauffé à blanc", "Une braise", "Un tissu teint", "Un homard", "Énigmes"], ["Qu'est-ce qui pleure sans yeux et vole sans ailes ?", "Le nuage", "Le vent", "L'avion", "L'écho", "Énigmes"], ["Qu'est-ce qui a 4 roues et des mouches ?", "Un camion poubelle", "Une calèche", "Un tas de fumier", "Un tracteur", "Énigmes"], ["Qu'est-ce qui monte quand on a faim et descend quand on a bien mangé ?", "L'appétit", "La glycémie", "Le poids", "L'énergie", "Énigmes"], ["Qu'est-ce qui a une racine que personne ne voit et est plus haut que les arbres ?", "Une montagne", "Une colline", "Une tour", "Un nuage", "Énigmes"], ["Qu'est-ce qui n'a pas de vie mais peut grandir, n'a pas de poumons mais a besoin d'air ?", "Le feu", "La plante", "L'orage", "Le cristal", "Énigmes"], ["Qu'est-ce qui peut être brisé sans jamais avoir été touché ?", "Une promesse", "Un cœur", "Le silence", "Un miroir", "Énigmes"], ["Si vous m'avez, vous voulez me partager. Si vous me partagez, vous ne m'avez plus. Que suis-je ?", "Un secret", "Une idée", "Une pomme", "De l'argent", "Énigmes"], ["Qu'est-ce qui a une jambe et trois yeux ?", "Un feu de signalisation", "Un pirate", "Un monstre", "Un tabouret", "Énigmes"], ["Je parcours le monde sans jamais quitter mon lit. Qui suis-je ?", "Un fleuve", "Un dormeur", "Un malade", "Un paresseux", "Énigmes"], ["Qu'est-ce qui a des aiguilles mais ne sait pas coudre ?", "Une montre", "Un hérisson", "Un sapin", "Un porc-épic", "Énigmes"], ["Qu'est-ce qui a une tête dure et un corps en bois ?", "Une allumette", "Un marteau", "Un crayon", "Un piquet", "Énigmes"], ["Qu'est-ce qui avance toujours et ne recule jamais ?", "Le temps", "Une horloge", "Une flèche", "Un train", "Énigmes"], ["Je vole sans ailes, je pleure sans yeux, où que j'aille l'obscurité me suit. Qui suis-je ?", "Le nuage d'orage", "La chauve-souris", "La nuit", "La fumée", "Énigmes"], ["Qu'est-ce qui se trouve au milieu de Paris ?", "La lettre R", "La tour Eiffel", "La Seine", "Le Louvre", "Énigmes"], ["Qu'est-ce qui a un œil au milieu du front mais n'a pas de visage ?", "Une aiguille à coudre", "Un cyclope", "Une tempête", "Un phare", "Énigmes"], ["Qu'est-ce qui a 88 touches mais ne peut ouvrir aucune porte ?", "Un piano", "Un clavier d'ordinateur", "Un téléphone", "Une machine à écrire", "Énigmes"], ["Qu'est-ce qui a un talon mais pas de pied ?", "Une chaussure", "Une baguette de pain", "Une chaussette", "Un pneu", "Énigmes"], ["Qu'est-ce qui ne peut être brûlé par le feu ni noyé par l'eau ?", "La glace", "La pierre", "Le vent", "L'ombre", "Énigmes"], ["Qu'est-ce qui devient plus petit quand on le met debout et plus grand quand on le couche ?", "Le chiffre 8", "Un livre", "Une bouteille", "Un crayon", "Énigmes"], ["Qu'est-ce qui a deux branches mais aucune feuille ?", "Une paire de lunettes", "Un compas", "Une fourche", "Un fleuve", "Énigmes"], ["Qu'est-ce qui a un manche mais ne coupe rien ?", "Une poêle", "Un balai", "Une cuillère", "Un parapluie", "Énigmes"], ["Qu'est-ce qui a trois pieds mais ne peut pas marcher ?", "Un trépied", "Une table", "Un tabouret", "Un compas", "Énigmes"], ["Qu'est-ce qui grossit quand on lui enlève de la matière ?", "Un trou", "Une bulle", "Une dette", "Une éponge", "Énigmes"], ["Qu'est-ce qui ne demande rien mais qu'on répond toujours ?", "Le téléphone", "La porte", "La sonnette", "L'écho", "Énigmes"], ["Qu'est-ce qui a un col sans être un vêtement ?", "Une montagne", "Une bouteille", "Une veste", "Un oiseau", "Énigmes"], ["Qu'est-ce qui tourne sans cesse autour de la maison sans jamais y entrer ?", "La clôture", "Le vent", "Le soleil", "Le chien", "Énigmes"], ["Qu'est-ce qui a deux têtes, quatre yeux, six jambes mais seulement une queue ?", "Un cavalier sur son cheval", "Un monstre mythologique", "Deux oiseaux sur une branche", "Un chien à deux têtes", "Énigmes"], ["Qu'est-ce qui a des écailles mais ne nage pas dans l'eau ?", "Une pomme de pin", "Un serpent", "Une armure", "Un toit d'ardoise", "Énigmes"], ["Qu'est-ce qui a un ventre blanc, un dos noir et vole sous l'eau ?", "Le manchot", "L'orque", "La mouette", "La loutre", "Énigmes"], ["Qu'est-ce qui ne fait aucun bruit en tombant dans l'eau mais fait trembler la terre ?", "La foudre", "La pluie", "L'avalanche", "Le météore", "Énigmes"], ["Qu'est-ce qui peut remplir une pièce entière sans occuper le moindre espace ?", "La lumière", "L'air", "Le parfum", "Le son", "Énigmes"], ["Qu'est-ce qui a des cornes mais n'est pas un animal ?", "La lune", "Un croissant de pain", "Une enclume", "Un guidon", "Énigmes"], ["Qu'est-ce qui est toujours dans le coin mais regarde tout le monde passer ?", "Le lampadaire", "Le panneau", "Le mur", "La boîte aux lettres", "Énigmes"], ["Qu'est-ce qui n'a pas de corps mais produit une ombre ?", "Rien du tout", "L'air", "Le fantôme", "La lumière", "Énigmes"], ["Qu'est-ce qui a une crête mais ne chante jamais au lever du soleil ?", "Une montagne", "Une vague", "Un coq en peluche", "Un casque", "Énigmes"], ["Qu'est-ce qui vous quitte à la moindre obscurité ?", "Votre ombre", "Votre courage", "Vos yeux", "Votre chaleur", "Énigmes"], ["Qu'est-ce qui a un chapeau rond et un seul pied pour se tenir droit ?", "Un parasol", "Un champignon", "Une lampe", "Un clou", "Énigmes"], ["Qu'est-ce qui s'allonge quand on le tire et rétrécit quand on le lâche ?", "Un élastique", "Un ressort", "Une pâte", "Un chewing-gum", "Énigmes"], ["Qu'est-ce qui a des dents acérées mais ne mange que du bois ?", "Une tronçonneuse", "Un castor", "Un termite", "Une scie égoïne", "Énigmes"], ["Qu'est-ce qui ne prend pas de place dans la poche mais peut tout acheter ?", "Une carte bancaire", "Une clé", "Un chèque", "Une pièce rare", "Énigmes"], ["Qu'est-ce qui peut être lu d'en haut, d'en bas, de gauche ou de droite sans changer ?", "Un palindrome", "Un acronyme", "Une anagramme", "Une rime", "Énigmes"], ["Qu'est-ce qui traverse les océans sans jamais se mouiller ?", "Un câble sous-marin", "Le soleil", "La lune", "Le vent", "Énigmes"], ["Qu'est-ce qui a des ailes mais ne vole pas dans les airs ?", "Un moulin à vent", "Un avion cloué au sol", "Une raie manta", "Un bâtiment", "Énigmes"], ["Qu'est-ce qui s'use d'autant plus vite qu'on s'en sert pour effacer ?", "Une gomme", "Un chiffon", "Le savon", "La mémoire", "Énigmes"], ["Qu'est-ce qui a des cornes, mange de l'herbe et donne du fromage sans être une chèvre ?", "La vache", "La brebis", "Le buffle", "Le renne", "Énigmes"], ["Qu'est-ce qui a des feuilles vertes en été et d'or en automne ?", "L'arbre", "Le livre ancien", "La vigne", "Le maïs", "Énigmes"], ["Qu'est-ce qui a un fil mais ne tisse jamais de toile ?", "Le téléphone fixe", "L'araignée mécanique", "Le funambule", "La canne à pêche", "Énigmes"], ["Qu'est-ce qui s'ouvre d'un coup quand on saute dans le vide ?", "Un parachute", "Une trappe", "Un parapluie", "Les bras", "Énigmes"], ["Qu'est-ce qui a des marches mais ne mène nulle part si on n'avance pas ?", "Un escalier", "Un tapis roulant", "Une échelle", "Une estrade", "Énigmes"], ["Qu'est-ce qui éclaire sans brûler et guide les marins dans la nuit ?", "Un phare", "La lune", "Une étoile", "Une balise", "Énigmes"], ["Quelle est la planète la plus proche du Soleil ?", "Mercure", "Vénus", "Mars", "Jupiter", "Sciences & Geek"], ["Combien d'octets composent un kilooctet (Ko) standard ?", "1024 octets", "1000 octets", "512 octets", "2048 octets", "Sciences & Geek"], ["Quel est le symbole chimique de l'or ?", "Au", "Ag", "Fe", "Or", "Sciences & Geek"], ["Quel jeu vidéo d'arcade met en scène une boule jaune mangeant des pac-gommes ?", "Pac-Man", "Space Invaders", "Donkey Kong", "Pong", "Sciences & Geek"], ["Combien de cœurs possède une pieuvre ?", "3 cœurs", "1 cœur", "2 cœurs", "4 cœurs", "Sciences & Geek"], ["Quel gaz les plantes absorbent-elles principalement lors de la photosynthèse ?", "Le dioxyde de carbone (CO2)", "L'oxygène", "L'azote", "L'hydrogène", "Sciences & Geek"], ["Quelle est la vitesse approximative de la lumière dans le vide ?", "300 000 km/s", "150 000 km/s", "1 000 000 km/s", "30 000 km/s", "Sciences & Geek"], ["Dans quel jeu vidéo explore-t-on le royaume d'Hyrule avec l'épée de légende ?", "The Legend of Zelda", "Final Fantasy", "Dragon Quest", "Dark Souls", "Sciences & Geek"], ["Quel est l'élément chimique le plus abondant dans l'Univers ?", "L'hydrogène", "L'hélium", "L'oxygène", "Le carbone", "Sciences & Geek"], ["Quelle entreprise a commercialisé la célèbre console Game Boy en 1989 ?", "Nintendo", "Sega", "Sony", "Atari", "Sciences & Geek"], ["Quel est le composant d'un ordinateur chargé d'exécuter les calculs principaux ?", "Le processeur (CPU)", "La mémoire RAM", "Le disque SSD", "La carte mère", "Sciences & Geek"], ["Combien d'os compte le squelette d'un être humain adulte ?", "206 os", "180 os", "250 os", "300 os", "Sciences & Geek"], ["Quelle planète du système solaire possède les anneaux les plus spectaculaires ?", "Saturne", "Jupiter", "Uranus", "Neptune", "Sciences & Geek"], ["Quel animal était la mascotte officielle de Sega face à Mario ?", "Sonic le hérisson", "Crash Bandicoot", "Rayman", "Mega Man", "Sciences & Geek"], ["Que signifie l'acronyme HTML dans le développement web ?", "HyperText Markup Language", "High Tech Multi Language", "Home Tool Modern Link", "Hyperlink Text Maker", "Sciences & Geek"], ["Quelle est l'unité de mesure de la fréquence d'une onde ?", "Le Hertz (Hz)", "Le Volt", "Le Watt", "Le Joule", "Sciences & Geek"], ["Combien de bits forment un octet (byte) ?", "8 bits", "4 bits", "16 bits", "32 bits", "Sciences & Geek"], ["Quel célèbre jeu vidéo de briques empilables a été créé par Alekseï Pajitnov ?", "Tetris", "Arkanoid", "Blockout", "Lumines", "Sciences & Geek"], ["Quelle particule subatomique porte une charge électrique négative ?", "L'électron", "Le proton", "Le neutron", "Le photon", "Sciences & Geek"], ["Quel astre est situé au centre exact de notre système solaire ?", "Le Soleil", "Jupiter", "La Terre", "La Voie Lactée", "Sciences & Geek"], ["En quelle année le premier homme a-t-il marché sur la Lune ?", "1969", "1965", "1972", "1959", "Sciences & Geek"], ["Quel est le système d'exploitation libre au logo de manchot nommé Tux ?", "Linux", "Windows", "macOS", "FreeBSD", "Sciences & Geek"], ["Quel est le nom de la galaxie dans laquelle se trouve notre système solaire ?", "La Voie Lactée", "Andromède", "Le Triangle", "Le Sombrero", "Sciences & Geek"], ["Quel langage de programmation utilise le mot-clé 'def' pour définir une fonction ?", "Python", "C++", "Java", "PHP", "Sciences & Geek"], ["Quel oiseau légendaire renaît de ses propres cendres ?", "Le phénix", "Le griffon", "L'aigle royal", "Le condor", "Sciences & Geek"], ["Quelle planète est surnommée la 'Planète Rouge' ?", "Mars", "Mercure", "Vénus", "Jupiter", "Sciences & Geek"], ["Quel jeu de tir à la première personne de 1993 a révolutionné le genre sur PC ?", "Doom", "Half-Life", "Quake", "Duke Nukem 3D", "Sciences & Geek"], ["Combien de chromosomes possède une cellule humaine normale ?", "46 chromosomes", "23 chromosomes", "48 chromosomes", "44 chromosomes", "Sciences & Geek"], ["Quel est l'organe humain qui consomme le plus d'énergie par rapport à son poids ?", "Le cerveau", "Le cœur", "Le foie", "Les reins", "Sciences & Geek"], ["Quel célèbre rover de la NASA a atterri sur Mars en février 2021 ?", "Perseverance", "Curiosity", "Opportunity", "Spirit", "Sciences & Geek"], ["Quel protocole sécurisé permet de naviguer sur le web avec un cadenas vert ?", "HTTPS", "FTP", "SSH", "SMTP", "Sciences & Geek"], ["Quelle est la formule chimique de l'eau pure ?", "H2O", "CO2", "NaCl", "CH4", "Sciences & Geek"], ["Quelle console de salon 16 bits arborait le slogan 'Mega Drive' ?", "Sega Mega Drive", "Super Nintendo", "Neo Geo", "PC Engine", "Sciences & Geek"], ["Quelle force invisible attire les objets vers le centre de la Terre ?", "La gravité", "Le magnétisme", "La force centrifuge", "L'inertie", "Sciences & Geek"], ["Combien de faces possède un dé cubique standard ?", "6 faces", "4 faces", "8 faces", "12 faces", "Sciences & Geek"], ["Quel scientifique a formulé la théorie de la relativité restreinte et générale ?", "Albert Einstein", "Isaac Newton", "Galilée", "Niels Bohr", "Sciences & Geek"], ["Quel célèbre jeu de sandbox cubique a été créé par 'Notch' Persson ?", "Minecraft", "Terraria", "Roblox", "Cube World", "Sciences & Geek"], ["Quel est le plus grand mammifère vivant sur notre planète ?", "La baleine bleue", "L'éléphant d'Afrique", "Le cachalot", "Le requin-baleine", "Sciences & Geek"], ["Quelle est l'unité de mesure de la résistance électrique ?", "L'Ohm", "L'Ampère", "Le Volt", "Le Tesla", "Sciences & Geek"], ["Combien de minutes la lumière du Soleil met-elle environ pour parvenir à la Terre ?", "8 minutes", "1 minute", "15 minutes", "24 minutes", "Sciences & Geek"], ["Quel est le principal composant de l'atmosphère terrestre (environ 78 %) ?", "L'azote", "L'oxygène", "L'argon", "Le dioxyde de carbone", "Sciences & Geek"], ["Quel était le nom de la première console de jeux vidéo de salon de Sony sortie en 1994 ?", "PlayStation", "Dreamcast", "Nintendo 64", "Saturn", "Sciences & Geek"], ["Quelle est la température minimale absolue en degrés Celsius ?", "-273,15 °C", "-100 °C", "-500 °C", "-200 °C", "Sciences & Geek"], ["Comment appelle-t-on un polygone à huit côtés ?", "Un octogone", "Un hexagone", "Un heptagone", "Un décagone", "Sciences & Geek"], ["Quel microcontrôleur Wi-Fi / Bluetooth équipe la console sur laquelle vous jouez ?", "ESP32", "Arduino Uno", "Raspberry Pi Pico", "STM32", "Sciences & Geek"], ["Quel composant électronique laisse passer le courant dans un seul sens ?", "Une diode", "Une résistance", "Un condensateur", "Une bobine", "Sciences & Geek"], ["Quel jeu de cartes et de monstres de poche a été créé par Satoshi Tajiri ?", "Pokémon", "Yu-Gi-Oh!", "Magic: The Gathering", "Digimon", "Sciences & Geek"], ["Quel organe du corps humain produit l'insuline ?", "Le pancréas", "Le foie", "La rate", "La vésicule biliaire", "Sciences & Geek"], ["Comment appelle-t-on le passage direct de l'état solide à l'état gazeux ?", "La sublimation", "L'évaporation", "La condensation", "La fusion", "Sciences & Geek"], ["Quel animal a été le premier être vivant en orbite terrestre à bord de Spoutnik 2 ?", "La chienne Laïka", "Le singe Albert", "La chatte Félicette", "La souris Youri", "Sciences & Geek"], ["Quelle est la touche de clavier universelle pour annuler la dernière action (Undo) sous Windows ?", "Ctrl + Z", "Ctrl + C", "Ctrl + V", "Ctrl + Y", "Sciences & Geek"], ["Quel astronome polonais a affirmé au XVIe siècle que la Terre tourne autour du Soleil ?", "Nicolas Copernic", "Galilée", "Johannes Kepler", "Tycho Brahe", "Sciences & Geek"], ["Dans l'univers Star Wars, quelle arme emblématique manient les chevaliers Jedi ?", "Le sabre laser", "Le blaster", "Le fusil plasma", "L'arc énergétique", "Sciences & Geek"], ["Quel est le pH d'une solution parfaitement neutre à 25 °C ?", "7", "0", "14", "1", "Sciences & Geek"], ["Quel est le plus grand os du corps humain ?", "Le fémur", "Le tibia", "L'humérus", "Le péroné", "Sciences & Geek"], ["Quel célèbre jeu de simulation de vie a été lancé par Will Wright en 2000 ?", "Les Sims", "SimCity", "Animal Crossing", "Second Life", "Sciences & Geek"], ["Comment appelle-t-on la couche de gaz qui protège la Terre des ultraviolets solaires ?", "La couche d'ozone", "L'ionosphère", "La troposphère", "La magnétosphère", "Sciences & Geek"], ["Quel type de mémoire informatique perd son contenu dès que l'alimentation est coupée ?", "La mémoire vive (RAM)", "Le disque dur (HDD)", "La mémoire Flash", "La mémoire ROM", "Sciences & Geek"], ["Quel est le nom de la sonde spatiale qui a quitté le système solaire pour l'espace interstellaire ?", "Voyager 1", "Curiosity", "Hubble", "Apollo 11", "Sciences & Geek"], ["Quelle particule de lumière se déplace à la vitesse maximale de l'univers ?", "Le photon", "Le gluon", "Le quark", "Le neutrino", "Sciences & Geek"], ["Quel jeu de combat arcade légendaire de Capcom oppose Ryu et Ken ?", "Street Fighter", "Tekken", "Mortal Kombat", "Fatal Fury", "Sciences & Geek"], ["Quelle est la formule chimique du sel de table ordinaire ?", "NaCl", "KCl", "CaCO3", "MgSO4", "Sciences & Geek"], ["Quel est le plus petit élément constitutif de la matière ordinaire ?", "L'atome", "La cellule", "La molécule", "Le cristal", "Sciences & Geek"], ["Quel télescope spatial lancé en décembre 2021 a succédé à Hubble avec un miroir en or ?", "James Webb (JWST)", "Spitzer", "Kepler", "Chandra", "Sciences & Geek"], ["Quelle couleur de lumière visible a la longueur d'onde la plus courte ?", "Le violet", "Le rouge", "Le vert", "Le jaune", "Sciences & Geek"], ["Comment appelle-t-on un nombre qui n'a pour diviseurs que 1 et lui-même ?", "Un nombre premier", "Un nombre pair", "Un nombre parfait", "Un nombre irrationnel", "Sciences & Geek"], ["Dans quel jeu vidéo de plateforme le plombier moustachu saute-t-il sur des Goombas ?", "Super Mario Bros", "Sonic", "Rayman", "Crash Bandicoot", "Sciences & Geek"], ["Quelle vitamine est naturellement synthétisée par notre peau exposée au soleil ?", "La vitamine D", "La vitamine C", "La vitamine B12", "La vitamine A", "Sciences & Geek"], ["Quel oiseau est capable de voler en arrière ?", "Le colibri", "L'hirondelle", "Le martinet", "Le faucon", "Sciences & Geek"], ["Quel est le symbole chimique du fer ?", "Fe", "Ir", "F", "Fr", "Sciences & Geek"], ["Quel était le nom du premier ordinateur entièrement électronique conçu en 1945 ?", "L'ENIAC", "L'Apple I", "L'IBM PC", "L'Altair", "Sciences & Geek"], ["Quelle est la distance approximative de la Terre à la Lune ?", "384 400 km", "150 000 km", "1 000 000 km", "50 000 km", "Sciences & Geek"], ["Dans le jeu Space Invaders, comment tirent les canons ennemis ?", "De haut en bas", "De bas en haut", "En diagonale", "En cercle", "Sciences & Geek"], ["Quel scientifique a découvert la pénicilline en 1928 ?", "Alexander Fleming", "Louis Pasteur", "Marie Curie", "Robert Koch", "Sciences & Geek"], ["Quel est le nom de la constante mathématique égale à environ 3,14159 ?", "Pi (π)", "Euler (e)", "Phi (φ)", "Sigma (σ)", "Sciences & Geek"], ["Quelle partie de la fleur produit le pollen ?", "L'étamine", "Le pistil", "Le pétale", "Le sépale", "Sciences & Geek"], ["Combien de dents comporte la dentition complète d'un adulte humain sans compter les dents de sagesse ?", "28 dents", "32 dents", "24 dents", "30 dents", "Sciences & Geek"], ["Quel célèbre jeu de rôle en ligne massivement multijoueur (MMORPG) a pour cadre Azeroth ?", "World of Warcraft", "Guild Wars", "Final Fantasy XIV", "Lineage", "Sciences & Geek"], ["Quel instrument mesure la pression atmosphérique ?", "Le baromètre", "L'hygromètre", "Le thermomètre", "L'anémomètre", "Sciences & Geek"], ["Quel est le gaz le plus lourd parmi les suivants ?", "Le xénon", "L'hélium", "L'azote", "Le néon", "Sciences & Geek"], ["Comment appelle-t-on le résultat d'une multiplication ?", "Le produit", "La somme", "Le quotient", "La différence", "Sciences & Geek"], ["Quel jeu de combat gore de 1992 s'est fait connaître pour ses 'Fatalities' ?", "Mortal Kombat", "Street Fighter", "Killer Instinct", "Soulcalibur", "Sciences & Geek"], ["Quelle est l'unité de mesure de l'intensité du courant électrique ?", "L'Ampère", "Le Volt", "Le Watt", "Le Coulomb", "Sciences & Geek"], ["Combien d'arêtes possède un cube ?", "12 arêtes", "8 arêtes", "6 arêtes", "16 arêtes", "Sciences & Geek"], ["Quelle est la vitesse du son dans l'air à 20 °C approximativement ?", "343 m/s", "100 m/s", "1 000 m/s", "150 m/s", "Sciences & Geek"], ["Quel célèbre scientifique a énoncé les lois du mouvement et de l'attraction universelle ?", "Isaac Newton", "Albert Einstein", "René Descartes", "Archimède", "Sciences & Geek"], ["Quelle entreprise a créé le système d'exploitation Windows ?", "Microsoft", "Apple", "IBM", "Google", "Sciences & Geek"], ["Quel oiseau nocturne peut tourner sa tête jusqu'à 270 degrés ?", "La chouette", "Le corbeau", "L'aigle", "Le faucon", "Sciences & Geek"], ["Quelle est l'unité de mesure de la puissance d'un appareil électrique ?", "Le Watt (W)", "Le Volt (V)", "L'Ampère (A)", "Le Joule (J)", "Sciences & Geek"], ["Quel gaz inflammable était utilisé dans les premiers ballons dirigeables comme le Hindenburg ?", "L'hydrogène", "L'hélium", "L'azote", "Le méthane", "Sciences & Geek"], ["Dans quel jeu de cartes cyberpunk le hacker cherche-t-il à pirater des serveurs corporatifs ?", "Netrunner", "Magic", "Hearthstone", "Gwent", "Sciences & Geek"], ["Quel est le liquide organique rouge qui circule dans nos veines et artères ?", "Le sang", "La lymphe", "La bile", "Le plasma", "Sciences & Geek"], ["Quelle planète du système solaire tourne sur elle-même dans le sens inverse des autres ?", "Vénus", "Mars", "Jupiter", "Mercure", "Sciences & Geek"], ["Comment appelle-t-on la science qui étudie les fossiles et les espèces disparues ?", "La paléontologie", "L'archéologie", "La géologie", "L'anthropologie", "Sciences & Geek"], ["Quel célèbre jeu de simulation spatiale permet de construire des fusées avec des créatures vertes appelées Kerbals ?", "Kerbal Space Program", "No Man's Sky", "Star Citizen", "Elite Dangerous", "Sciences & Geek"], ["Quelle est la température d'ébullition de l'eau au niveau de la mer ?", "100 °C", "90 °C", "120 °C", "80 °C", "Sciences & Geek"], ["Quel savant grec s'est écrié 'Eurêka !' en découvrant la poussée exercée par les fluides ?", "Archimède", "Pythagore", "Euclide", "Thalès", "Sciences & Geek"], ["Quel réseau sans fil à courte portée permet d'associer un casque audio à un smartphone ?", "Bluetooth", "Wi-Fi", "NFC", "Infrarouge", "Sciences & Geek"], ["Quel phénomène optique crée un cercle coloré dans le ciel quand la pluie et le soleil se croisent ?", "L'arc-en-ciel", "L'aurore boréale", "Le mirage", "Le halo solaire", "Sciences & Geek"], ["Quel était le nom de l'agent secret incarné par James Bond matricule 007 ?", "James Bond", "Ethan Hunt", "Jason Bourne", "John Wick", "Sciences & Geek"], ["Quel oiseau est incapable de voler mais nage avec une agilité remarquable en Antarctique ?", "Le manchot", "Le pingouin", "L'albatros", "Le pélican", "Nature & Animaux"], ["Quel est le plus grand arbre du monde en volume de bois ?", "Le séquoia géant", "Le baobab", "Le chêne centenaire", "L'eucalyptus", "Nature & Animaux"], ["Quel reptile change de couleur pour communiquer et réguler sa température ?", "Le caméléon", "L'iguane", "Le gecko", "Le varan", "Nature & Animaux"], ["Quel félin est réputé pour être l'animal terrestre le plus rapide sur courte distance ?", "Le guépard", "Le lion", "Le léopard", "Le tigre", "Nature & Animaux"], ["Combien de pattes possède une araignée ?", "8 pattes", "6 pattes", "10 pattes", "12 pattes", "Nature & Animaux"], ["Quel mammifère est le seul capable de pratiquer un véritable vol battu ?", "La chauve-souris", "L'écureuil volant", "Le lémurien", "L'ornithorynque", "Nature & Animaux"], ["Quel est le plus grand océan de notre planète ?", "L'océan Pacifique", "L'océan Atlantique", "L'océan Indien", "L'océan Arctique", "Nature & Animaux"], ["Quel grand herbivore africain passe ses journées dans l'eau pour protéger sa peau du soleil ?", "L'hippopotame", "Le rhinocéros", "L'éléphant", "Le buffle", "Nature & Animaux"], ["Comment appelle-t-on le petit du cheval ?", "Le poulain", "Le chevreau", "Le veau", "L'agneau", "Nature & Animaux"], ["Quel insecte produit du miel dans les ruches ?", "L'abeille", "La guêpe", "Le frelon", "Le bourdon", "Nature & Animaux"], ["Quel animal marin possède huit bras munis de ventouses ?", "La pieuvre (poulpe)", "Le calmar", "La seiche", "La méduse", "Nature & Animaux"], ["Quelle plante carnivore referme ses pièges à mâchoires en un éclair sur les insectes ?", "La dionée attrape-mouche", "La népenthès", "Le droséra", "L'utriculaire", "Nature & Animaux"], ["Quel animal australien porte son petit dans une poche ventrale et saute sur ses pattes arrière ?", "Le kangourou", "Le koala", "Le wombat", "Le dingo", "Nature & Animaux"], ["Quel animal emblématique de Chine se nourrit presque exclusivement de bambou ?", "Le panda géant", "Le tigre de Sibérie", "Le yack", "Le pangolin", "Nature & Animaux"], ["Quel oiseau est le symbole de la paix ?", "La colombe", "L'aigle", "Le cygne", "La mouette", "Nature & Animaux"], ["Combien de cœurs possède un ver de terre ?", "5 paires de cœurs", "1 cœur", "Aucun", "2 cœurs", "Nature & Animaux"], ["Quel est le plus grand désert chaud du monde ?", "Le Sahara", "Le désert de Gobi", "Le Kalahari", "L'Atacama", "Nature & Animaux"], ["Quel poisson remonte les rivières contre le courant pour aller pondre ses œufs ?", "Le saumon", "La truite", "Le brochet", "L'esturgeon", "Nature & Animaux"], ["Comment appelle-t-on la femelle du sanglier ?", "La laie", "La truie", "La biche", "La hase", "Nature & Animaux"], ["Quel oiseau pond les plus gros œufs du règne animal actuel ?", "L'autruche", "L'émeu", "Le condor", "L'aigle royal", "Nature & Animaux"], ["Quel prédateur marin possède des rangées de dents renouvelables et un squelette cartilagineux ?", "Le requin", "La baleine", "Le dauphin", "L'orque", "Nature & Animaux"], ["Quel est l'animal le plus grand ayant jamais existé sur Terre ?", "La baleine bleue", "Le tyrannosaure", "Le diplodocus", "Le mégalodon", "Nature & Animaux"], ["Quel oiseau est réputé pour répéter les paroles humaines ?", "Le perroquet", "Le canari", "Le moineau", "La pie", "Nature & Animaux"], ["Comment s'appelle l'état de sommeil profond dans lequel certains animaux passent l'hiver ?", "L'hibernation", "La torpeur", "La somnolence", "L'estivation", "Nature & Animaux"], ["Quel animal produit la laine la plus douce et recherchée originaire des Andes ?", "L'alpaga", "Le mouton", "La chèvre", "Le lama", "Nature & Animaux"], ["Quel champignon souterrain très parfumé est déniché à l'aide de chiens ou de porcs ?", "La truffe", "Le cèpe", "La morille", "La girolle", "Nature & Animaux"], ["Quel grand félin à rayures noires et pelage orange vit dans les forêts d'Asie ?", "Le tigre", "Le lion", "Le léopard", "Le jaguar", "Nature & Animaux"], ["Comment appelle-t-on le petit de la biche ?", "Le faon", "Le veau", "Le marcassin", "Le chevreau", "Nature & Animaux"], ["Quel oiseau nocturne a la réputation d'être le symbole de la sagesse ?", "La chouette", "Le hibou", "La corneille", "Le vautour", "Nature & Animaux"], ["Quel arbre emblématique du Sud produit des olives pour l'huile ?", "L'olivier", "Le cyprès", "Le figuier", "Le platane", "Nature & Animaux"], ["Quel est l'animal terrestre le plus lourd du monde ?", "L'éléphant de savane", "Le rhinocéros blanc", "L'hippopotame", "La girafe", "Nature & Animaux"], ["Quel insecte volant scintille dans la nuit pendant les soirées d'été ?", "La luciole (ver luisant)", "Le papillon de nuit", "Le grillon", "Le frelon", "Nature & Animaux"], ["Quelle créature marine en forme d'étoile peut régénérer ses bras coupés ?", "L'étoile de mer", "L'oursin", "L'anémone", "Le corail", "Nature & Animaux"], ["Quel animal est réputé pour faire le mort lorsqu'il se sent menacé ?", "L'opossum", "Le raton laveur", "Le hérisson", "Le blaireau", "Nature & Animaux"], ["Quel oiseau migrateur revient traditionnellement en Europe au début du printemps ?", "L'hirondelle", "La cigogne", "L'oie sauvage", "Le flamant rose", "Nature & Animaux"], ["Quel crustacé marche de côté sur le sable des plages ?", "Le crabe", "Le homard", "La crevette", "La langouste", "Nature & Animaux"], ["Quelle est la plus haute montagne de la planète Terre au-dessus du niveau de la mer ?", "L'Everest", "Le K2", "Le mont Blanc", "Le Kilimandjaro", "Nature & Animaux"], ["Comment appelle-t-on un bébé chien ?", "Un chiot", "Un chaton", "Un louveteau", "Un renardeau", "Nature & Animaux"], ["Quel mammifère aquatique intelligent communique par cliquetis et sifflements ?", "Le dauphin", "Le phoque", "Le morse", "Le lamantin", "Nature & Animaux"], ["Quel arbre perd ses aiguilles chaque automne contrairement aux autres conifères ?", "Le mélèze", "Le sapin", "L'épicéa", "Le pin sylvestre", "Nature & Animaux"], ["Quel reptile cuirassé vit dans les marécages et possède une mâchoire surpuissante ?", "Le crocodile", "Le serpent", "La tortue géante", "L'iguane", "Nature & Animaux"], ["Quelle petite créature rongeuse fait des provisions de noisettes pour l'hiver ?", "L'écureuil", "Le mulot", "Le hérisson", "Le castor", "Nature & Animaux"], ["Quel est le fleuve le plus long du monde selon la plupart des géographes ?", "Le Nil", "L'Amazone", "Le Yangtsé", "Le Mississippi", "Nature & Animaux"], ["Quel animal porte de grands bois qui tombent et repoussent chaque année ?", "Le cerf", "Le bélier", "La chèvre sauvage", "Le taureau", "Nature & Animaux"], ["Comment appelle-t-on la maison sous-marine construite par les castors ?", "Une hutte", "Un terrier", "Une tanière", "Un nid", "Nature & Animaux"], ["Quel insecte produit un son strident l'été en frottant ses élytres ou ses cymbales ?", "La cigale", "Le moustique", "La mouche", "La coccinelle", "Nature & Animaux"], ["Quel poisson aux dents acérées vit en bancs voraces dans les fleuves d'Amazonie ?", "Le piranha", "Le silure", "Le poisson-chat", "L'anguille électrique", "Nature & Animaux"], ["Quelle fleur jaune se tourne naturellement vers le soleil tout au long de la journée ?", "Le tournesol", "La marguerite", "La tulipe", "Le pissenlit", "Nature & Animaux"], ["Quel animal a de longues oreilles, une queue touffue et vit dans un terrier creusé ?", "Le lapin", "Le lièvre", "Le cobaye", "Le furet", "Nature & Animaux"], ["Quel mammifère marin blanc vit exclusivement dans les eaux glacées de l'Arctique ?", "Le béluga", "Le narval", "L'orque", "Le grand dauphin", "Nature & Animaux"], ["Quel grand mammifère carnivore blanc est le seigneur des glaces du pôle Nord ?", "L'ours polaire", "L'ours brun", "Le loup blanc", "Le phoque barbu", "Nature & Animaux"], ["Comment appelle-t-on le nid d'un aigle situé en haut d'une falaise ?", "Une aire", "Une hutte", "Un terrier", "Un dortoir", "Nature & Animaux"], ["Quel oiseau majestueux plonge à plus de 300 km/h pour attraper ses proies en vol ?", "Le faucon pèlerin", "L'aigle royal", "La buse", "Le vautour", "Nature & Animaux"], ["Quel coquillage produit des perles précieuses en entourant un grain de sable de nacre ?", "L'huître", "La moule", "La coquille Saint-Jacques", "Le bulot", "Nature & Animaux"], ["Quelle plante aromatique méditerranéenne repousse les moustiques par son odeur de lavande ?", "La lavande", "Le romarin", "Le thym", "La menthe", "Nature & Animaux"], ["Quel animal porte une coquille spiralée sur son dos et avance très lentement ?", "L'escargot", "La limace", "La tortue", "Le bernard-l'ermite", "Nature & Animaux"], ["Quel est le plus grand prédateur terrestre d'Europe ?", "L'ours brun", "Le loup gris", "Le lynx boréal", "Le glouton", "Nature & Animaux"], ["Comment appelle-t-on la métamorphose de la chenille avant de devenir papillon ?", "La chrysalide", "La mue", "L'éclosion", "La ponte", "Nature & Animaux"], ["Quel mammifère marin possède une longue défense spiralée comme une corne de licorne ?", "Le narval", "Le morse", "Le béluga", "L'éléphant de mer", "Nature & Animaux"], ["Quel animal sauvage hurle à la lune en meute ?", "Le loup", "Le coyote", "Le chacal", "La hyène", "Nature & Animaux"], ["Quelle fleur rouge éclatante fleurit dans les champs de blé au début de l'été ?", "Le coquelicot", "La rose", "L'œillet", "L'amaryllis", "Nature & Animaux"], ["Quel oiseau rose dort souvent sur une seule patte au bord des lagunes salées ?", "Le flamant rose", "La spatule", "Le héron", "La grue", "Nature & Animaux"], ["Quel amphibien commence sa vie sous forme de têtard nageant dans l'eau ?", "La grenouille", "Le lézard", "La salamandre", "Le triton", "Nature & Animaux"], ["Quel mammifère nocturne se met en boule piquante pour se protéger des prédateurs ?", "Le hérisson", "Le porc-épic", "Le tatou", "Le pangolin", "Nature & Animaux"], ["Quel animal noir et blanc projette un liquide malodorant pour se défendre en Amérique ?", "La mouffette", "Le blaireau", "Le raton laveur", "La belette", "Nature & Animaux"], ["Quel fruit produit le chêne ?", "Le gland", "La châtaigne", "La noisette", "La faine", "Nature & Animaux"], ["Quel oiseau migrateur noir construit son nid sous le toit des maisons en ville ?", "L'hirondelle", "Le moineau", "La mésange", "Le pigeon", "Nature & Animaux"], ["Quel animal ressemble à un cheval mais arbore une robe zébrée noir et blanc ?", "Le zèbre", "L'okapi", "Le quagga", "L'onagre", "Nature & Animaux"], ["Quel oiseau vole sur place comme un hélicoptère pour butiner le nectar des fleurs ?", "Le colibri", "Le martinet", "Le moineau", "Le rossignol", "Nature & Animaux"], ["Comment appelle-t-on une forêt dense et impénétrable des régions tropicales humides ?", "La jungle (ou forêt équatoriale)", "La taïga", "La toundra", "La savane", "Nature & Animaux"], ["Quel reptile sans pattes rampe sur le sol et change de peau par mue ?", "Le serpent", "L'orvet", "Le ver de terre", "L'anguille", "Nature & Animaux"], ["Quel rapace nocturne possède de petites aigrettes de plumes sur la tête ressemblant à des oreilles ?", "Le hibou", "La chouette", "Le faucon", "La buse", "Nature & Animaux"], ["Quel mammifère marin géant possède deux énormes défenses d'ivoire sur la banquise ?", "Le morse", "Le phoque", "L'otarie", "L'éléphant de mer", "Nature & Animaux"], ["Quel arbre emblématique du Canada produit un sirop doré très sucré ?", "L'érable", "Le bouleau", "Le chêne", "Le pin", "Nature & Animaux"], ["Quelle créature sous-marine gélatineuse et translucide peut provoquer des brûlures au contact ?", "La méduse", "L'anémone de mer", "Le corail", "L'éponge", "Nature & Animaux"], ["Quel animal semi-aquatique australien pond des œufs tout en allaitant ses petits ?", "L'ornithorynque", "L'échidné", "Le koala", "Le wombat", "Nature & Animaux"], ["Quel insecte à points noirs sur carapace rouge est surnommé la 'bête à bon Dieu' ?", "La coccinelle", "Le scarabée", "Le gendarme", "Le hanneton", "Nature & Animaux"], ["Comment appelle-t-on le rugissement puissant du cerf en période d'accouplement ?", "Le brame", "Le hurlement", "Le feulement", "Le vagissement", "Nature & Animaux"], ["Quel poisson plat vit enfoui dans le sable au fond de la mer avec ses deux yeux du même côté ?", "La sole", "Le bar", "La daurade", "La truite", "Nature & Animaux"], ["Quel fruit sec est récolté sur le pin parasol et utilisé dans les sauces pesto ?", "Le pignon de pin", "La pistache", "L'amande", "La noix", "Nature & Animaux"], ["Quel félin au pelage tacheté d'ocelles vit principalement dans les forêts d'Amazonie ?", "Le jaguar", "Le guépard", "Le serval", "L'ocelot", "Nature & Animaux"], ["Quel mammifère est le plus proche parent vivant du dauphin ?", "L'hippopotame", "Le cheval", "La vache", "L'éléphant", "Nature & Animaux"], ["Comment appelle-t-on un regroupement de poissons nageant ensemble de façon synchronisée ?", "Un banc", "Une meute", "Un essaim", "Une harde", "Nature & Animaux"], ["Quel rongeur bâtit des barrages de branchages sur les cours d'eau pour créer des étangs ?", "Le castor", "Le ragondin", "La loutre", "Le rat musqué", "Nature & Animaux"], ["Quelle plante piquante des déserts stocke de grandes quantités d'eau dans ses tiges ?", "Le cactus", "L'agave", "L'aloès", "Le yucca", "Nature & Animaux"], ["Quel oiseau voleur à plumage noir et blanc est réputé pour cacher des objets brillants ?", "La pie", "Le corbeau", "Le geai", "L'étourneau", "Nature & Animaux"], ["Quelle créature marine utilise un nuage d'encre noire pour fuir ses assaillants ?", "La seiche", "Le mérou", "L'étoile de mer", "Le homard", "Nature & Animaux"], ["Quel animal terrestre possède le cou le plus long pour brouter le sommet des acacias ?", "La girafe", "L'autruche", "Le lama", "Le dromadaire", "Nature & Animaux"], ["Combien de bosses possède le dromadaire sur son dos ?", "1 bosse", "2 bosses", "3 bosses", "Aucune", "Nature & Animaux"], ["Quel arbre majestueux donne des châtaignes comestibles dans les forêts ?", "Le châtaignier", "Le marronnier", "Le noisetier", "Le hêtre", "Nature & Animaux"], ["Quel animal se déplace très lentement suspendu à l'envers aux branches de la canopée ?", "Le paresseux", "Le koala", "Le lémurien", "Le gibbon", "Nature & Animaux"], ["Quel oiseau marin plonge à pic dans l'eau pour harponner des poissons ?", "Le fou de Bassan", "Le goéland", "La mouette", "Le cormoran", "Nature & Animaux"], ["Quelle fleur aquatique flotte à la surface des étangs avec de larges feuilles rondes ?", "Le nénuphar", "Le lotus", "Le roseau", "L'iris", "Nature & Animaux"], ["Quel animal marin est souvent appelé 'l'orque' ou la 'baleine tueuse' ?", "L'épaulard", "Le cachalot", "Le narval", "Le requin bleu", "Nature & Animaux"], ["Quel grand singe roux vit dans les forêts tropicales de Sumatra et Bornéo ?", "L'orang-outan", "Le chimpanzé", "Le gorille", "Le bonobo", "Nature & Animaux"], ["Quel animal est réputé pour changer de couleur de pelage, devenant tout blanc en hiver ?", "L'hermine", "Le renard roux", "Le blaireau", "La belette", "Nature & Animaux"], ["Quel grand rongeur d'Amérique du Sud est le plus gros rongeur vivant au monde ?", "Le capybara", "Le ragondin", "Le chinchilla", "Le pécari", "Nature & Animaux"], ["Quelle fleur printanière blanche en forme de clochette porte bonheur le 1er mai ?", "Le muguet", "La perce-neige", "La jacinthe", "La jonquille", "Nature & Animaux"], ["Quel est le cri caractéristique poussé par le loup ?", "Le hurlement", "Le jappement", "Le grognement", "Le couinement", "Nature & Animaux"], ["Quel animal produit la soie naturelle grâce à son cocon ?", "Le ver à soie (chenille du bombyx)", "L'araignée d'eau", "La mite", "Le puceron", "Nature & Animaux"], ["Quelle est la capitale de la France ?", "Paris", "Lyon", "Marseille", "Bordeaux", "Culture & Histoire"], ["Quel célèbre monument parisien en fer a été inauguré pour l'Exposition Universelle de 1889 ?", "La Tour Eiffel", "L'Arc de Triomphe", "Le Panthéon", "L'Opéra Garnier", "Culture & Histoire"], ["Qui a peint le célèbre tableau de La Joconde (Mona Lisa) ?", "Léonard de Vinci", "Michel-Ange", "Raphaël", "Pablo Picasso", "Culture & Histoire"], ["En quelle année s'est déroulée la Révolution française avec la prise de la Bastille ?", "1789", "1799", "1776", "1804", "Culture & Histoire"], ["Quel empereur français a été exilé sur l'île de Sainte-Hélène après la bataille de Waterloo ?", "Napoléon Ier", "Napoléon III", "Louis XIV", "Charles de Gaulle", "Culture & Histoire"], ["Quelle civilisation antique a érigé les grandes pyramides de Gizeh ?", "Les Égyptiens", "Les Romains", "Les Grecs", "Les Babyloniens", "Culture & Histoire"], ["Quelle est la monnaie officielle utilisée dans la majorité des pays de l'Union européenne ?", "L'Euro", "Le Franc", "La Livre sterling", "Le Dollar", "Culture & Histoire"], ["Quel dramaturge anglais a écrit 'Roméo et Juliette' et 'Hamlet' ?", "William Shakespeare", "Charles Dickens", "Oscar Wilde", "Arthur Conan Doyle", "Culture & Histoire"], ["Quelle cité antique a été engloutie par les cendres du Vésuve en l'an 79 ?", "Pompéi", "Rome", "Athènes", "Carthage", "Culture & Histoire"], ["Quel est le plus grand pays du monde par sa superficie terrestre ?", "La Russie", "Le Canada", "La Chine", "Les États-Unis", "Culture & Histoire"], ["Quel roi de France était surnommé le 'Roi-Soleil' et a fait construire Versailles ?", "Louis XIV", "François Ier", "Henri IV", "Louis XVI", "Culture & Histoire"], ["Quel explorateur génois a traversé l'Atlantique en 1492 pour atteindre les Amériques ?", "Christophe Colomb", "Vasco de Gama", "Magellan", "Jacques Cartier", "Culture & Histoire"], ["Quelle statue colossale offerte par la France accueille les bateaux à New York ?", "La Statue de la Liberté", "La Victoire de Samothrace", "Le Colosse de Rhodes", "La Vénus de Milo", "Culture & Histoire"], ["Quel célèbre physicien et inventeur serbo-américain a développé le courant alternatif ?", "Nikola Tesla", "Thomas Edison", "Alexander Graham Bell", "Guglielmo Marconi", "Culture & Histoire"], ["Quelle est la capitale de l'Italie réputée pour le Colisée et le Vatican ?", "Rome", "Milan", "Venise", "Florence", "Culture & Histoire"], ["Quel écrivain français a rédigé 'Les Misérables' et 'Notre-Dame de Paris' ?", "Victor Hugo", "Émile Zola", "Gustave Flaubert", "Alexandre Dumas", "Culture & Histoire"], ["Quelle est la plus longue muraille fortifiée construite par l'Homme pour protéger un empire ?", "La Grande Muraille de Chine", "Le mur d'Hadrien", "Le mur de Berlin", "Les remparts de Carcassonne", "Culture & Histoire"], ["Quel compositeur autrichien de génie a composé 'La Flûte enchantée' dès son plus jeune âge ?", "Wolfgang Amadeus Mozart", "Ludwig van Beethoven", "Johann Sebastian Bach", "Frédéric Chopin", "Culture & Histoire"], ["En quelle année s'est achevée la Seconde Guerre mondiale en Europe ?", "1945", "1944", "1940", "1950", "Culture & Histoire"], ["Quel pays d'Europe a la forme caractéristique d'une botte sur la carte ?", "L'Italie", "L'Espagne", "La Grèce", "Le Portugal", "Culture & Histoire"], ["Quel célèbre navigateur portugais a mené la première expédition à faire le tour du globe ?", "Fernand de Magellan", "Christophe Colomb", "Amerigo Vespucci", "James Cook", "Culture & Histoire"], ["Quelle était la capitale de l'Empire romain d'Orient, aujourd'hui nommée Istanbul ?", "Constantinople", "Athènes", "Alexandrie", "Antioche", "Culture & Histoire"], ["Qui a inventé l'imprimerie à caractères mobiles en métal en Europe vers 1440 ?", "Johannes Gutenberg", "Léonard de Vinci", "Galilée", "Erasme", "Culture & Histoire"], ["Quel célèbre festival de cinéma décerne la Palme d'or chaque année sur la Côte d'Azur ?", "Le Festival de Cannes", "La Mostra de Venise", "La Berlinale", "Les Oscars", "Culture & Histoire"], ["Quelle est la plus grande île du monde qui appartient politiquement au Danemark ?", "Le Groenland", "Madagascar", "L'Islande", "La Nouvelle-Guinée", "Culture & Histoire"], ["Quelle reine d'Égypte antique a séduit Jules César et Marc Antoine ?", "Cléopâtre", "Néfertiti", "Hatchepsout", "Néfertari", "Culture & Histoire"], ["Quel célèbre peintre hollandais a peint 'La Nuit étoilée' et s'est coupé l'oreille ?", "Vincent van Gogh", "Rembrandt", "Claude Monet", "Paul Gauguin", "Culture & Histoire"], ["Quelle est la capitale du Japon ?", "Tokyo", "Kyoto", "Osaka", "Hiroshima", "Culture & Histoire"], ["Quel monument romain antique à gradins accueillait les combats de gladiateurs à Rome ?", "Le Colisée", "Le Panthéon", "Le Circus Maximus", "Le Forum", "Culture & Histoire"], ["Quel canal artificiel inauguré en 1869 relie la mer Méditerranée à la mer Rouge ?", "Le canal de Suez", "Le canal de Panama", "Le canal de Corinthe", "Le canal de Kiel", "Culture & Histoire"], ["Quel célèbre détective de fiction habite au 221B Baker Street à Londres ?", "Sherlock Holmes", "Hercule Poirot", "Inspecteur Columbo", "Jules Maigret", "Culture & Histoire"], ["En quelle année le mur de Berlin est-il tombé, réunifiant l'Allemagne ?", "1989", "1991", "1985", "1975", "Culture & Histoire"], ["Quelle ville des États-Unis est surnommée 'La Grosse Pomme' (The Big Apple) ?", "New York", "Los Angeles", "Chicago", "Miami", "Culture & Histoire"], ["Quel savant grec a crié 'Donnez-moi un point d'appui et je soulèverai le monde' ?", "Archimède", "Socrate", "Platon", "Aristote", "Culture & Histoire"], ["Quel roi d'Angleterre a eu six épouses et a fondé l'Église anglicane ?", "Henri VIII", "Richard Cœur de Lion", "Édouard VI", "Guillaume le Conquérant", "Culture & Histoire"], ["Quel monument indien en marbre blanc a été bâti par amour par l'empereur Shâh Jahân ?", "Le Taj Mahal", "Le temple d'or", "Le Qûtb Minâr", "Le Fort Rouge", "Culture & Histoire"], ["Quelle ville italienne est construite sur pilotis et célèbre pour ses canaux et gondoles ?", "Venise", "Florence", "Gênes", "Naples", "Culture & Histoire"], ["Quel compositeur sourd a composé la grandiose 9e Symphonie et l'Hymne à la joie ?", "Ludwig van Beethoven", "Johann Sebastian Bach", "Franz Schubert", "Antonio Vivaldi", "Culture & Histoire"], ["Quel est le plus grand musée d'art du monde situé à Paris ?", "Le musée du Louvre", "Le musée d'Orsay", "Le Centre Pompidou", "Le musée Rodin", "Culture & Histoire"], ["Quel est le plus petit pays souverain du monde enclavé dans la ville de Rome ?", "Le Vatican", "Monaco", "Saint-Marin", "Le Liechtenstein", "Culture & Histoire"], ["Quelle femme scientifique a reçu deux prix Nobel dans deux disciplines scientifiques différentes ?", "Marie Curie", "Rosalind Franklin", "Ada Lovelace", "Lise Meitner", "Culture & Histoire"], ["Quel explorateur français a découvert et exploré le fleuve Saint-Laurent au Canada ?", "Jacques Cartier", "Samuel de Champlain", "La Pérouse", "Bougainville", "Culture & Histoire"], ["Quelle ville antique a résisté au siège des Grecs avant d'être prise grâce à un cheval de bois ?", "Troie", "Sparte", "Mycènes", "Thèbes", "Culture & Histoire"], ["Quel instrument de musique à cordes frottées est le plus petit et le plus aigu de son orchestre ?", "Le violon", "L'alto", "Le violoncelle", "La contrebasse", "Culture & Histoire"], ["Quelle boisson chaude originaire de Chine se prépare en infusant des feuilles séchées ?", "Le thé", "Le café", "Le chocolat", "Le maté", "Culture & Histoire"], ["Quelle guerre entre la France et l'Angleterre a duré en réalité 116 ans au Moyen Âge ?", "La guerre de Cent Ans", "La guerre de Trente Ans", "La guerre de Sept Ans", "La guerre des Deux-Roses", "Culture & Histoire"], ["Quel général carthaginois a traversé les Alpes avec des éléphants de guerre contre Rome ?", "Hannibal Barca", "Scipion l'Africain", "Hamilcar", "César", "Culture & Histoire"], ["Quel héros de la mythologie grecque a accompli douze travaux surhumains ?", "Héraclès (Hercule)", "Achille", "Persée", "Thésée", "Culture & Histoire"], ["Quel fleuve traverse la ville de Londres ?", "La Tamise", "La Seine", "Le Rhin", "Le Danube", "Culture & Histoire"], ["Quel célèbre paquebot a coulé en avril 1912 après avoir heurté un iceberg ?", "Le Titanic", "Le Britannic", "Le Lusitania", "Le Queen Mary", "Culture & Histoire"], ["Quelle déesse grecque de la sagesse et de la guerre stratégique a donné son nom à Athènes ?", "Athéna", "Aphrodite", "Artémis", "Héra", "Culture & Histoire"], ["Qui a peint la fresque mythique du plafond de la chapelle Sixtine à Rome ?", "Michel-Ange", "Léonard de Vinci", "Raphaël", "Botticelli", "Culture & Histoire"], ["Quel animal fabuleux crachant du feu garde traditionnellement les trésors dans les contes ?", "Le dragon", "La licorne", "Le griffon", "La chimère", "Culture & Histoire"], ["Quelle tour penchée célèbre en marbre blanc se trouve en Toscane ?", "La tour de Pise", "La tour Eiffel", "La tour de Londres", "La tour de Belém", "Culture & Histoire"], ["Quel événement marque la date du 14 juillet en France ?", "La Fête nationale", "La Fête du travail", "L'Armistice", "La Toussaint", "Culture & Histoire"], ["Quel fleuve traverse la ville de Paris ?", "La Seine", "La Loire", "Le Rhône", "La Garonne", "Culture & Histoire"], ["Quel pays a offert la statue de la Liberté aux États-Unis pour célébrer leur centenaire ?", "La France", "Le Royaume-Uni", "L'Allemagne", "L'Italie", "Culture & Histoire"], ["Quel aventurier espagnol a mené la conquête de l'Empire aztèque au Mexique ?", "Hernán Cortés", "Francisco Pizarro", "Vasco Núñez de Balboa", "Ponce de León", "Culture & Histoire"], ["Quel héros de fiction créé par Bob Kane combat le crime à Gotham City sans super-pouvoirs ?", "Batman", "Superman", "Spider-Man", "Flash", "Culture & Histoire"], ["Quel célèbre jeu de société consiste à acheter et vendre des terrains et des gares ?", "Le Monopoly", "Le Scrabble", "Le Cluedo", "La Bonne Paye", "Culture & Histoire"], ["Quelle reine de France d'origine autrichienne a péri guillotinée en 1793 ?", "Marie-Antoinette", "Catherine de Médicis", "Anne d'Autriche", "Marguerite de Valois", "Culture & Histoire"], ["Quel est le plus grand monument funéraire antique découvert avec une armée de terre cuite en Chine ?", "Le mausolée de Qin Shi Huang", "La cité interdite", "Le palais d'été", "Les grottes de Mogao", "Culture & Histoire"], ["Quel roi des dieux de la mythologie grecque manie la foudre sur le mont Olympe ?", "Zeus", "Poséidon", "Hadès", "Arès", "Culture & Histoire"], ["Quelle célèbre marque de briques en plastique multicolores vient du Danemark ?", "Lego", "Playmobil", "Meccano", "K'Nex", "Culture & Histoire"], ["Quel pays sud-américain est le plus grand producteur de café au monde ?", "Le Brésil", "La Colombie", "L'Argentine", "Le Pérou", "Culture & Histoire"], ["Quel aviateur français a réussi la première traversée de la Manche en aéroplane en 1909 ?", "Louis Blériot", "Roland Garros", "Jean Mermoz", "Antoine de Saint-Exupéry", "Culture & Histoire"], ["Quelle est la langue officielle la plus parlée en Amérique du Sud en nombre de pays ?", "L'espagnol", "Le portugais", "Le français", "L'anglais", "Culture & Histoire"], ["Quel pays est surnommé le 'Pays du Soleil Levant' ?", "Le Japon", "La Chine", "La Corée du Sud", "La Thaïlande", "Culture & Histoire"], ["Quel héros suisse a fendu une pomme posée sur la tête de son fils avec une arbalète ?", "Guillaume Tell", "Robin des Bois", "D'Artagnan", "Lancelot", "Culture & Histoire"], ["Quel écrivain a créé le monstre créé par le docteur Victor Frankenstein en 1818 ?", "Mary Shelley", "Bram Stoker", "Edgar Allan Poe", "H.G. Wells", "Culture & Histoire"], ["Quel est le plus haut sommet des Alpes et d'Europe occidentale ?", "Le mont Blanc", "Le mont Cervin", "La Jungfrau", "Le mont Rose", "Culture & Histoire"], ["Quelle chanteuse française a interprété la légendaire chanson 'La Vie en rose' ?", "Édith Piaf", "Barbara", "Juliette Gréco", "Dalida", "Culture & Histoire"], ["Quelle fête celtique ancienne est célébrée le soir du 31 octobre avec des citrouilles sculptées ?", "Halloween", "Pâques", "Thanksgiving", "Noël", "Culture & Histoire"], ["Quel écrivain a raconté les aventures d'un jeune prince voyageant d'astéroïde en astéroïde avec son renard et sa rose ?", "Antoine de Saint-Exupéry", "Jules Verne", "Albert Camus", "Jean Giono", "Culture & Histoire"], ["Quelle célèbre cloche logée dans la tour Elizabeth du Parlement britannique sonne les heures à Londres ?", "Big Ben", "Liberty Bell", "Emmanuel", "Great Tom", "Culture & Histoire"], ["Quelle île méditerranéenne française a vu naître Napoléon Bonaparte en 1769 ?", "La Corse", "La Sardaigne", "La Sicile", "L'île d'Elbe", "Culture & Histoire"], ["Quel compositeur italien a créé les célèbres concertos baroques 'Les Quatre Saisons' ?", "Antonio Vivaldi", "Giacomo Puccini", "Giuseppe Verdi", "Gioachino Rossini", "Culture & Histoire"], ["Quel est le plus grand État des États-Unis en superficie ?", "L'Alaska", "Le Texas", "La Californie", "Le Montana", "Culture & Histoire"], ["Quel célèbre traité signé en 1919 a mis fin à la Première Guerre mondiale ?", "Le traité de Versailles", "Le traité de Vienne", "Le traité d'Utrecht", "Le traité de Rome", "Culture & Histoire"], ["Quel géant de la mythologie grecque a été condamné à porter la voûte céleste sur ses épaules ?", "Atlas", "Prométhée", "Chronos", "Sisyphe", "Culture & Histoire"], ["Quel était le nom du premier président des États-Unis d'Amérique ?", "George Washington", "Thomas Jefferson", "Abraham Lincoln", "Benjamin Franklin", "Culture & Histoire"], ["Quelle forteresse médiévale fortifiée surplombe la baie entre la Normandie et la Bretagne ?", "Le Mont-Saint-Michel", "Carcassonne", "Château-Gaillard", "Le château d'If", "Culture & Histoire"], ["Quel sculpteur français a réalisé les chefs-d'œuvre 'Le Penseur' et 'Le Baiser' ?", "Auguste Rodin", "Camille Claudel", "Antoine Bourdelle", "Jean-Baptiste Carpeaux", "Culture & Histoire"], ["Quelle invention attribuée aux frères Lumière en 1895 a donné naissance au 7e art ?", "Le cinématographe", "Le phonographe", "La photographie", "Le télégraphe", "Culture & Histoire"], ["Quel est le seul pays d'Afrique à n'avoir jamais été colonisé par une puissance occidentale ?", "L'Éthiopie", "Le Nigeria", "Le Kenya", "Le Maroc", "Culture & Histoire"], ["Quel philosophe grec antique a été condamné à boire la ciguë pour avoir instruit la jeunesse ?", "Socrate", "Platon", "Aristote", "Épicure", "Culture & Histoire"], ["Quelle boisson pétillante prestigieuse tire son nom de sa région viticole française d'origine ?", "Le champagne", "Le bordeaux", "Le prosecco", "Le cidre", "Culture & Histoire"], ["Quel empereur romain a légalisé le christianisme et fondé une nouvelle capitale à son nom ?", "Constantin Ier", "Néron", "Auguste", "Marc Aurèle", "Culture & Histoire"], ["Quel oiseau légendaire porte les bébés dans un baluchon selon le folklore alsacien ?", "La cigogne", "L'oie", "La grue", "Le pélican", "Culture & Histoire"], ["Quel est le plus long fleuve de France métropolitaine ?", "La Loire", "La Seine", "Le Rhône", "La Garonne", "Culture & Histoire"], ["Quel artiste espagnol a peint l'œuvre monumentale et tragique 'Guernica' en 1937 ?", "Pablo Picasso", "Salvador Dalí", "Joan Miró", "Francisco de Goya", "Culture & Histoire"], ["Quelle reine d'Angleterre a régné pendant 63 ans et donné son nom à une époque faste du XIXe siècle ?", "La reine Victoria", "Élisabeth Ire", "La reine Anne", "Marie Stuart", "Culture & Histoire"], ["Quel roi de France a été assassiné par François Ravaillac en 1610 ?", "Henri IV", "Louis XIII", "Henri III", "Charles IX", "Culture & Histoire"], ["Quelle créature fantastique mi-femme mi-poisson attire les marins par son chant envoûtant ?", "La sirène", "La naïade", "La gorgone", "L'ondine", "Culture & Histoire"], ["Quel château de la Renaissance bâti sur la Loire possède un célèbre escalier à double révolution ?", "Le château de Chambord", "Le château de Chenonceau", "Le château d'Amboise", "Le château de Blois", "Culture & Histoire"], ["Quel sport collectif oppose deux équipes de onze joueurs cherchant à marquer avec un ballon rond ?", "Le football", "Le rugby", "Le basketball", "Le handball", "Culture & Histoire"], ["Quel écrivain a créé le célèbre personnage de d'Artagnan dans 'Les Trois Mousquetaires' ?", "Alexandre Dumas", "Honoré de Balzac", "Jules Verne", "Stendhal", "Culture & Histoire"], ["Quel festival de bande dessinée prestigieux a lieu chaque année dans une ville de Charente ?", "Le Festival d'Angoulême", "Le Festival d'Annecy", "Le Festival d'Avignon", "La Comic-Con", "Culture & Histoire"], ["Quel dieu romain des mers et des tempêtes équivaut au Poséidon grec ?", "Neptune", "Jupiter", "Pluton", "Mars", "Culture & Histoire"], ["Quelle reine mythique d'Angleterre a épousé le roi Arthur selon les légendes arthuriennes ?", "Guenièvre", "Morgane", "Viviane", "Iseult", "Culture & Histoire"], ["Mon premier est un métal précieux (Au), mon second sert à faire du pain (blé), mon tout est précieux et noble. Que suis-je ?", "L'orbe (ou noble)", "L'argent", "Le trésor", "L'anneau", "Jeux de Mots & Logique"], ["Quelle lettre de l'alphabet se boit traditionnellement l'après-midi en Angleterre ?", "Le T (Thé)", "Le O (Eau)", "Le C", "Le L", "Jeux de Mots & Logique"], ["Quelle lettre de l'alphabet permet à un oiseau de voler ?", "L (L'aile)", "V", "P", "O", "Jeux de Mots & Logique"], ["Comment appelle-t-on une phrase ou un mot qui se lit exactement de la même manière dans les deux sens (ex: KAYAK) ?", "Un palindrome", "Un anagramme", "Un acronyme", "Une allitération", "Jeux de Mots & Logique"], ["Quelle lettre trouve-t-on au milieu de la mer ?", "La lettre E", "La lettre M", "La lettre R", "La lettre A", "Jeux de Mots & Logique"], ["Quel mot de 7 lettres commence par 'c', finit par 'e' et contient des millions de lettres ?", "Le courrier", "Le clavier", "Le dictionnaire", "L'encyclopédie", "Jeux de Mots & Logique"], ["Qu'est-ce qui appartient à un pâtissier et à un peintre ?", "La palette", "Le pinceau", "Le four", "Le tablier", "Jeux de Mots & Logique"], ["Quelle est la seule lettre qui n'apparaît dans aucun chiffre de 0 à 9 en toutes lettres françaises ?", "La lettre K", "La lettre W", "La lettre Z", "La lettre X", "Jeux de Mots & Logique"], ["Quel mot français devient plus court lorsqu'on lui ajoute deux lettres ?", "Court (qui devient courte)", "Bref", "Petit", "Fin", "Jeux de Mots & Logique"], ["Mon premier est un félin domestique (chat), mon second est un lieu de repos (lit), mon tout est une punition corporelle ancienne. Que suis-je ?", "Un châtiment", "Un château", "Une chaloupe", "Un chapeau", "Jeux de Mots & Logique"], ["Si un coq pond un œuf sur le sommet d'un toit pointu, de quel côté tombe l'œuf ?", "Un coq ne pond pas d'œuf !", "Du côté droit", "Du côté gauche", "Il reste en équilibre", "Jeux de Mots & Logique"], ["Vous participez à une course à pied et vous dépassez le deuxième coureur. À quelle place êtes-vous ?", "Deuxième", "Premier", "Troisième", "Quatrième", "Jeux de Mots & Logique"], ["Combien de fois peut-on soustraire 5 de 25 ?", "Une seule fois (car après on a 20)", "5 fois", "4 fois", "À l'infini", "Jeux de Mots & Logique"], ["Un père et son fils ont 36 ans à eux deux. Le père a 30 ans de plus que son fils. Quel âge a le fils ?", "3 ans (Père: 33, Fils: 3)", "6 ans", "5 ans", "1 an", "Jeux de Mots & Logique"], ["Quelle lettre de l'alphabet coule doucement dans les ruisseaux ?", "L'O (L'eau)", "Le R (L'air)", "Le C", "Le F", "Jeux de Mots & Logique"], ["Quel mot français contient toutes les cinq voyelles (A, E, I, O, U) une seule fois ?", "Oiseau", "Bateau", "Tableau", "Chapeau", "Jeux de Mots & Logique"], ["Si 3 chats attrapent 3 souris en 3 minutes, combien de temps faut-il à 100 chats pour attraper 100 souris ?", "3 minutes", "100 minutes", "30 minutes", "1 minute", "Jeux de Mots & Logique"], ["Quelle est la fin de tout ?", "La lettre T", "La mort", "L'infini", "Le silence", "Jeux de Mots & Logique"], ["Qu'est-ce qui a 4 lettres, commence par 'p', finit par 't', et permet aux navires d'amarrer ?", "Le port", "Le pont", "Le plot", "Le part", "Jeux de Mots & Logique"], ["Qu'est-ce qui court sur la table pendant le repas sans jamais manger ?", "La salière (ou le poivrier)", "La fourchette", "L'assiette", "Le verre", "Jeux de Mots & Logique"], ["Mon premier est la 1ère note de musique (Do), mon second est un chiffre romain valant 10 (X), mon tout est un endroit pour ranger les bateaux. Que suis-je ?", "Le dock", "La baie", "Le port", "La cale", "Jeux de Mots & Logique"], ["Quel est le contraire d'un mot contenant un sens opposé ?", "Un antonyme", "Un synonyme", "Un homonyme", "Un paronyme", "Jeux de Mots & Logique"], ["Quel animal a trois lettres, commence par 'c' et ronronne près du poêle ?", "Le chat", "Le coq", "Le cerf", "Le carlin", "Jeux de Mots & Logique"], ["Quelle lettre de l'alphabet se trouve entre le 'S' et le 'U' ?", "La lettre T", "La lettre R", "La lettre V", "La lettre P", "Jeux de Mots & Logique"], ["Quelle lettre de l'alphabet se prononce comme une exclamation de surprise ('Oh !') ?", "La lettre O", "La lettre A", "La lettre E", "La lettre U", "Jeux de Mots & Logique"], ["Qu'est-ce qui a 5 lettres, commence par 'R', finit par 'O' et désigne le charme du vintage ?", "Rétro", "Radio", "Robot", "Ronde", "Jeux de Mots & Logique"], ["Quel mot composé désigne un appareil servant à mesurer la température extérieure ?", "Thermomètre", "Baromètre", "Chronomètre", "Anémomètre", "Jeux de Mots & Logique"], ["Quelle est la voyelle la plus fréquente dans la langue française écrite ?", "La lettre E", "La lettre A", "La lettre I", "La lettre O", "Jeux de Mots & Logique"], ["Mon premier est le contraire de chaud (froid), mon second est le son du serpent (sss), mon tout est un légume d'hiver. Que suis-je ?", "Le salsifis", "La mâche", "Le radis", "Le navet", "Jeux de Mots & Logique"], ["Combien de côtés possède un cercle parfait géométrique ?", "2 côtés (l'intérieur et l'extérieur)", "Aucun", "1 côté", "Une infinité de segments", "Jeux de Mots & Logique"], ["Quel nombre romain s'écrit avec la lettre 'X' ?", "10", "5", "50", "100", "Jeux de Mots & Logique"], ["Quel mot français est une anagramme exacte du mot 'GARE' ?", "Rage", "Gras", "Gros", "Rame", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois un fruit délicieux et une pièce de monnaie ?", "Aucun (piège : la pêche est un fruit, la prune est une amende !)", "L'orange", "La cerise", "La poire", "Jeux de Mots & Logique"], ["Qu'est-ce qui commence au 1er janvier et se termine au 31 décembre ?", "Une année", "Un mois", "Une saison", "Un siècle", "Jeux de Mots & Logique"], ["Si vous avez un panier avec 6 pommes et que vous en prenez 4, combien de pommes avez-vous ?", "4 pommes (celles que vous avez prises !)", "2 pommes", "6 pommes", "Aucune", "Jeux de Mots & Logique"], ["Quel animal porte son nom composé de deux fois la même syllabe 'cri-cri' ?", "Le criquet", "Le grillon", "Le crapaud", "Le canari", "Jeux de Mots & Logique"], ["Mon premier est un récipient pour boire (verre), mon second est une note de musique (mi), mon tout est un animal qui creuse la terre. Que suis-je ?", "Le lombric (ver)", "La taupe", "Le furet", "La souris", "Jeux de Mots & Logique"], ["Quelle lettre de l'alphabet permet d'indiquer la direction du Nord sur une boussole ?", "La lettre N", "La lettre S", "La lettre E", "La lettre O", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois une pièce d'échecs et une personne qui règne sur un royaume ?", "Le roi", "La tour", "Le fou", "Le cavalier", "Jeux de Mots & Logique"], ["Quel est le seul chiffre dont le nom français a le même nombre de lettres que sa valeur ?", "Quatre (4 lettres)", "Deux", "Trois", "Cinq", "Jeux de Mots & Logique"], ["Quelle lettre se trouve au début de l'éternité et à la fin de l'espace ?", "La lettre E", "La lettre T", "La lettre R", "La lettre A", "Jeux de Mots & Logique"], ["Quel est le féminin du mot 'héros' ?", "Héroïne", "Héroïste", "Hérose", "Hérote", "Jeux de Mots & Logique"], ["Quel mot de 4 lettres est le nom d'un célèbre jeu de tennis d'arcade vintage à deux raquettes ?", "Pong", "Game", "Play", "Ball", "Jeux de Mots & Logique"], ["Quel nombre multiplié par n'importe quel autre nombre donne toujours zéro ?", "0 (Zéro)", "1", "-1", "10", "Jeux de Mots & Logique"], ["Quelle lettre de l'alphabet a le son du mot 'nœud' en français ?", "La lettre E (son 'eu')", "La lettre O", "La lettre N", "La lettre U", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois un éclair de génie et la décharge d'un appareil photo ?", "Le flash", "Le rayon", "L'étincelle", "Le zoom", "Jeux de Mots & Logique"], ["Mon premier est un rongeur (rat), mon second est une étendue d'eau (mer), mon tout est un instrument de cuisine pour le fromage. Que suis-je ?", "Une râpe", "Un couteau", "Une planche", "Un poêlon", "Jeux de Mots & Logique"], ["Combien y a-t-il de secondes dans une heure entière ?", "3600 secondes", "60 secondes", "600 secondes", "1800 secondes", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois une partie de tennis et une boîte d'allumettes en anglais ?", "Match", "Set", "Game", "Court", "Jeux de Mots & Logique"], ["Quelle consonne double trouve-t-on au milieu du mot 'ballon' ?", "LL", "BB", "NN", "OO", "Jeux de Mots & Logique"], ["Quel mot français est l'anagramme exacte de 'CHIEN' ?", "Niche", "Chine", "Chine (les deux sont valides !)", "Échine", "Jeux de Mots & Logique"], ["Quel est le résultat de l'opération 7 x 8 ?", "56", "54", "64", "48", "Jeux de Mots & Logique"], ["Combien de zéros contient le nombre un million en chiffres (1 000 000) ?", "6 zéros", "5 zéros", "7 zéros", "8 zéros", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois une pièce de monnaie ancienne et une couronne royale ?", "L'écu (ou le sou)", "La livre", "Le florin", "Le ducat", "Jeux de Mots & Logique"], ["Mon premier est une boisson chaude (thé), mon second est un pronom personnel (toi), mon tout est la partie supérieure d'une maison. Que suis-je ?", "Le toit", "La cave", "La cheminée", "Le grenier", "Jeux de Mots & Logique"], ["Combien de côtés comporte un triangle équilatéral ?", "3 côtés", "4 côtés", "2 côtés", "5 côtés", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois une arme tranchante et la partie métallique d'un patin à glace ?", "La lame", "Le fer", "L'acier", "La pointe", "Jeux de Mots & Logique"], ["Quel est le nom de la touche du clavier servant à faire un espace entre les mots ?", "La barre d'espace", "La touche Entrée", "La touche Tab", "La touche Maj", "Jeux de Mots & Logique"], ["Quel adjectif qualifie une énigme difficile à résoudre ?", "Mystérieuse (ou complexe)", "Simple", "Transparente", "Évidente", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois le bruit d'un coup sec et un pointeur de souris informatique ?", "Le clic", "Le bip", "Le flash", "Le tap", "Jeux de Mots & Logique"], ["Quelle lettre romaine correspond au nombre 50 ?", "La lettre L", "La lettre C", "La lettre D", "La lettre M", "Jeux de Mots & Logique"], ["Mon premier est la moitié de deux (un), mon second est une céréale (riz), mon tout est un fruit de mer prisé. Que suis-je ?", "L'oursin", "La crevette", "La moule", "L'huître", "Jeux de Mots & Logique"], ["Quel mot de 5 lettres désigne le plus petit point lumineux d'un écran d'ordinateur ?", "Pixel", "Voxel", "Point", "Trace", "Jeux de Mots & Logique"], ["Quel est le mot opposé à 'victoire' ?", "Défaite", "Match nul", "Triomphe", "Abandon", "Jeux de Mots & Logique"], ["Combien de joueurs composent une équipe de basket-ball sur le terrain ?", "5 joueurs", "11 joueurs", "7 joueurs", "6 joueurs", "Jeux de Mots & Logique"], ["Quelle lettre ressemble à un serpent ondulant ?", "La lettre S", "La lettre Z", "La lettre C", "La lettre J", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois un récipient à anse et un ensemble de clés rassemblées ?", "Le trousseau", "Le panier", "Le casier", "Le coffre", "Jeux de Mots & Logique"], ["Quelle est la moitié de 100 ?", "50", "25", "75", "40", "Jeux de Mots & Logique"], ["Quelle est la troisième note de la gamme musicale occidentale (Do, Ré, ...) ?", "Mi", "Fa", "Sol", "La", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois un appareil auditif et une coiffure d'écouteurs ?", "Le casque", "L'oreillette", "Le micro", "Le bandeau", "Jeux de Mots & Logique"], ["Mon premier est le contraire de haut (bas), mon second est un lieu de danse (bal), mon tout est un jouet rebondissant d'enfant. Que suis-je ?", "La balle", "Le ballon", "La toupie", "Le cerceau", "Jeux de Mots & Logique"], ["Quel mot de 4 lettres désigne un code secret servant à sécuriser un accès ?", "Pass (ou Code)", "Clef", "Mot", "Bail", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois un poisson d'eau douce et un petit avion sans pilote téléguidé ?", "Le drone (ou le vairon)", "L'éperlan", "La perche", "Le goujon", "Jeux de Mots & Logique"], ["Quelle lettre minuscule comporte un point au-dessus d'elle ?", "Le 'i' (et le 'j')", "Le 'l'", "Le 't'", "Le 'e'", "Jeux de Mots & Logique"], ["Combien font 9 multiplié par 9 ?", "81", "72", "99", "90", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois une pièce de monnaie américaine et le chiffre 100 en argot ?", "Le cent", "Le dime", "Le dollar", "Le nickel", "Jeux de Mots & Logique"], ["Quel est le prénom de l'agent 007 ?", "James", "John", "Jack", "Jason", "Jeux de Mots & Logique"], ["Quelle lettre de l'alphabet a la même forme qu'un fer à cheval tourné vers le bas ?", "La lettre U inversée (ou Omega)", "La lettre C", "La lettre O", "La lettre D", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois une rangée de sièges et une place de rang militaire ?", "Le rang", "La file", "L'allée", "La travée", "Jeux de Mots & Logique"], ["Quel chiffre s'obtient quand on additionne tous les doigts des deux mains ?", "10", "5", "20", "12", "Jeux de Mots & Logique"], ["Mon premier est un métal précieux (or), mon second est un oiseau bavard (pie), mon tout est le bord d'un champ ou d'un bois. Que suis-je ?", "L'orée", "La haie", "La rive", "Le fossé", "Jeux de Mots & Logique"], ["Combien de saisons compte une année civile ?", "4 saisons", "2 saisons", "12 saisons", "6 saisons", "Jeux de Mots & Logique"], ["Quel est le contraire exact du mot 'lourd' ?", "Léger", "Mince", "Fin", "Petit", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois une note musicale et un astre lumineux ?", "Sol (le Soleil)", "La", "Fa", "Si", "Jeux de Mots & Logique"], ["Quel est le carré du chiffre 5 (5 x 5) ?", "25", "10", "20", "30", "Jeux de Mots & Logique"], ["Quelle lettre de l'alphabet est la première consonne de l'alphabet latin ?", "La lettre B", "La lettre A", "La lettre C", "La lettre D", "Jeux de Mots & Logique"], ["Quelle lettre de l'alphabet termine toujours le mot 'FIN' ?", "La lettre N", "La lettre F", "La lettre I", "La lettre S", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois un instrument de musique à vent et un conduit d'évacuation ?", "Le tuyau", "Le cor", "La flûte", "Le tube", "Jeux de Mots & Logique"], ["Quel adjectif qualifie une personne qui a de la chance au jeu ?", "Chanceux (ou veinard)", "Habile", "Riche", "Prévoyant", "Jeux de Mots & Logique"], ["Mon premier est la première lettre de l'alphabet (A), mon second est un rongeur (rat), mon tout est une créature légendaire ailée et mystérieuse. Que suis-je ?", "L'ara (perroquet)", "L'aigle", "L'albatros", "Le harfang", "Jeux de Mots & Logique"], ["Combien d'angles droits possède un carré parfait ?", "4 angles droits", "2 angles droits", "8 angles droits", "Aucun", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois une partie de tennis et un jeu de 52 pièces ?", "Le paquet (ou cartes)", "La table", "Le tour", "La manche", "Jeux de Mots & Logique"], ["Quel mot désigne un habitant de la planète Mars dans la science-fiction ?", "Un Martien", "Un Lunaire", "Un Jovien", "Un Terrien", "Jeux de Mots & Logique"], ["Quel est le pluriel du mot 'cheval' ?", "Chevaux", "Chevals", "Chevauxs", "Chevales", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois un dispositif de stockage informatique et un rond de musique vinyle ?", "Le disque", "La cassette", "La disquette", "Le lecteur", "Jeux de Mots & Logique"], ["Quelle consonne se répète trois fois dans le mot 'statistiques' ?", "La lettre T (et S)", "La lettre S", "La lettre I", "La lettre A", "Jeux de Mots & Logique"], ["Mon premier est un métal précieux (or), mon second est le ciel sans nuage (azur), mon tout est un orage qui approche. Que suis-je ?", "L'ouragan", "La tempête", "L'éclair", "Le tonnerre", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois une arme à feu portative et une fleur d'eau printanière ?", "Le pistolet (ou pétale)", "Le bouton", "La rose", "L'œillet", "Jeux de Mots & Logique"], ["Quel mot désigne à la fois une boîte pour dormir pour un bébé et une mangeoire pour animaux ?", "La crèche (ou berceau)", "Le lit", "Le couffin", "La nacelle", "Jeux de Mots & Logique"], ["Quelle lettre termine l'alphabet latin ?", "La lettre Z", "La lettre Y", "La lettre X", "La lettre W", "Jeux de Mots & Logique"]];

let deck = [];
let curQ = null;
let curOptions = [];
let answered = false;
let score = 0;
let streak = 0;
let jokers = 3;
let totalAnswered = 0;
let totalCorrect = 0;
let bestStreak = parseInt(localStorage.getItem('devinettes_best_streak') || '0');
let highScore = parseInt(localStorage.getItem('devinettes_high_score') || '0');

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

function playCorrectSound() {
  playTone(523.25, 0.1, 'sine');
  setTimeout(() => playTone(659.25, 0.12, 'sine'), 100);
  setTimeout(() => playTone(783.99, 0.2, 'triangle'), 200);
}

function playWrongSound() {
  playTone(220, 0.15, 'sawtooth');
  setTimeout(() => playTone(165, 0.25, 'sawtooth'), 120);
}

function playJokerSound() {
  playTone(880, 0.08, 'sine');
  setTimeout(() => playTone(1174, 0.12, 'triangle'), 80);
}

function shuffle(arr) {
  const a = [...arr];
  for (let i = a.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [a[i], a[j]] = [a[j], a[i]];
  }
  return a;
}

function resetDeck() {
  deck = shuffle([...Array(ALL_QUESTIONS.length).keys()]);
}

function nextQuestion() {
  answered = false;
  if (deck.length === 0) resetDeck();
  const qIdx = deck.pop();
  curQ = ALL_QUESTIONS[qIdx];

  // Options: 1 correct + 3 wrong
  curOptions = shuffle([
    { text: curQ[1], correct: true },
    { text: curQ[2], correct: false },
    { text: curQ[3], correct: false },
    { text: curQ[4], correct: false }
  ]);

  document.getElementById('cat-badge').textContent = curQ[5];
  document.getElementById('prog-badge').textContent = `Question ${500 - deck.length} / 500`;
  document.getElementById('q-text').textContent = curQ[0];

  const grid = document.getElementById('options-grid');
  grid.innerHTML = '';
  const letters = ['A', 'B', 'C', 'D'];

  curOptions.forEach((opt, idx) => {
    const btn = document.createElement('button');
    btn.className = 'opt-btn';
    btn.id = `opt-${idx}`;
    btn.innerHTML = `<span class="opt-letter">${letters[idx]}</span> <span>${opt.text}</span>`;
    btn.onclick = () => selectOption(idx);
    grid.appendChild(btn);
  });
}

function selectOption(selectedIdx) {
  if (answered) return;
  answered = true;
  totalAnswered++;

  const opt = curOptions[selectedIdx];
  const btn = document.getElementById(`opt-${selectedIdx}`);

  if (opt.correct) {
    btn.classList.add('correct');
    streak++;
    const mult = Math.min(streak, 5);
    const earned = 100 * mult;
    score += earned;
    totalCorrect++;
    playCorrectSound();

    if (streak > bestStreak) {
      bestStreak = streak;
      localStorage.setItem('devinettes_best_streak', bestStreak);
    }
    if (score > highScore) {
      highScore = score;
      localStorage.setItem('devinettes_high_score', highScore);
    }
  } else {
    btn.classList.add('wrong');
    streak = 0;
    playWrongSound();

    // Reveal correct
    curOptions.forEach((o, i) => {
      if (o.correct) {
        document.getElementById(`opt-${i}`).classList.add('correct');
      }
    });
  }

  updateHeaderStats();

  // Auto-advance after 1.5s
  setTimeout(() => {
    nextQuestion();
  }, 1400);
}

function use5050() {
  if (answered || jokers <= 0) return;
  jokers--;
  document.getElementById('joker-count').textContent = jokers;
  if (jokers === 0) {
    document.getElementById('btn-5050').classList.add('used');
  }
  playJokerSound();

  // Find 2 wrong options to eliminate
  let wrongIndices = [];
  curOptions.forEach((o, i) => {
    if (!o.correct) wrongIndices.push(i);
  });
  wrongIndices = shuffle(wrongIndices).slice(0, 2);

  wrongIndices.forEach(idx => {
    const btn = document.getElementById(`opt-${idx}`);
    if (btn) btn.classList.add('eliminated');
  });
}

function skipQuestion() {
  if (answered) return;
  streak = 0;
  updateHeaderStats();
  nextQuestion();
}

function updateHeaderStats() {
  document.getElementById('score-pill').textContent = `${score} PTS`;
  document.getElementById('streak-pill').textContent = `🔥 x${streak}`;
  
  const acc = totalAnswered > 0 ? Math.round((totalCorrect / totalAnswered) * 100) : 0;
  document.getElementById('stat-acc').textContent = `${acc}%`;
  document.getElementById('stat-best-streak').textContent = bestStreak;
  document.getElementById('stat-high-score').textContent = highScore;
}

// Initialize
updateHeaderStats();
resetDeck();
nextQuestion();
</script>
</body>
</html>
)rawliteral";

#endif
