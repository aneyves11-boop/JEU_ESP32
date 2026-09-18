#ifndef GAME_PACK_BOARD_H
#define GAME_PACK_BOARD_H

#include <pgmspace.h>

// 25. Puissance 4 Cyber
const char GAME_CONNECT4_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Puissance 4</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:10px;font-weight:bold;}
.board{background:#0e1428;border:2px solid #00f3ff;border-radius:12px;padding:8px;display:grid;grid-template-columns:repeat(7,1fr);gap:6px;width:330px;}
.slot{aspect-ratio:1;background:#05070f;border-radius:50%;cursor:pointer;}
.slot.r{background:#ff0055;box-shadow:0 0 8px #ff0055;}
.slot.y{background:#ffe600;box-shadow:0 0 8px #ffe600;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span id="st">Ton tour (Rouge)</span></header>
<div class="board" id="b"></div>
<button onclick="init()" style="margin-top:14px;width:330px;height:45px;background:#00f3ff;border:none;border-radius:10px;font-weight:bold;">REJOUER 🔄</button>
<script>
let grid=Array(6).fill(null).map(()=>Array(7).fill(0)),over=false;
const bEl=document.getElementById('b'),stEl=document.getElementById('st');
function init(){grid=Array(6).fill(null).map(()=>Array(7).fill(0));over=false;stEl.textContent="Ton tour (Rouge)";render();}
function drop(col){
if(over)return;
for(let r=5;r>=0;r--){
if(grid[r][col]===0){
grid[r][col]=1; render();
if(checkWin(1)){over=true;stEl.textContent="Gagné ! 🎉";return;}
stEl.textContent="L'IA réfléchit...";
setTimeout(aiDrop,350);return;
}}}
function aiDrop(){
if(over)return;
let validCols=[];for(let c=0;c<7;c++)if(grid[0][c]===0)validCols.push(c);
if(!validCols.length)return;
let c=validCols[Math.floor(Math.random()*validCols.length)];
for(let r=5;r>=0;r--){
if(grid[r][c]===0){grid[r][c]=2;break;}
}
render();
if(checkWin(2)){over=true;stEl.textContent="L'IA a gagné ! 🤖";return;}
stEl.textContent="Ton tour (Rouge)";
}
function checkWin(p){
for(let r=0;r<6;r++)for(let c=0;c<4;c++)if(grid[r][c]===p&&grid[r][c+1]===p&&grid[r][c+2]===p&&grid[r][c+3]===p)return true;
for(let r=0;r<3;r++)for(let c=0;c<7;c++)if(grid[r][c]===p&&grid[r+1][c]===p&&grid[r+2][c]===p&&grid[r+3][c]===p)return true;
return false;
}
function render(){
bEl.innerHTML='';
for(let r=0;r<6;r++)for(let c=0;c<7;c++){
const d=document.createElement('div');
d.className='slot'+(grid[r][c]===1?' r':(grid[r][c]===2?' y':''));
d.onclick=()=>drop(c);
bEl.appendChild(d);
}}
init();
</script></body></html>
)rawliteral";

// 26. Blackjack 21 Pro
const char GAME_BLACKJACK_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Blackjack 21</title>
<style>body{background:#06140b;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:10px;font-weight:bold;}
.tbl{width:330px;background:#0d3319;border:3px solid #ffe600;border-radius:16px;padding:16px;box-sizing:border-box;}
.cards{display:flex;gap:8px;min-height:50px;margin:8px 0;}
.card{width:42px;height:60px;background:#fff;color:#000;border-radius:6px;display:flex;align-items:center;justify-content:center;font-size:1.2rem;font-weight:bold;}
.ctrl{display:flex;gap:10px;margin-top:14px;width:330px;}
button{flex:1;height:46px;border-radius:10px;border:none;font-weight:bold;font-size:1rem;cursor:pointer;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="chips">100</span> $</span></header>
<div class="tbl">
<div style="font-weight:bold;color:#ffe600;">Croupier: <span id="ds">?</span></div>
<div class="cards" id="dc"></div>
<hr style="border-color:rgba(255,255,255,0.1);margin:12px 0;">
<div style="font-weight:bold;color:#00f3ff;">Toi: <span id="ps">0</span></div>
<div class="cards" id="pc"></div>
</div>
<div class="ctrl">
<button onclick="hit()" style="background:#00f3ff;color:#000;">TIRER 🃏</button>
<button onclick="stand()" style="background:#ffe600;color:#000;">RESTER ✋</button>
<button onclick="deal()" style="background:#ff0055;color:#fff;">NOUVEAU 🔄</button>
</div>
<script>
let p=[],d=[],chips=100,over=false;
function card(){const v=Math.floor(Math.random()*13)+1;return v>10?10:(v===1?11:v);}
function sum(h){let s=h.reduce((a,b)=>a+b,0);return s;}
function deal(){
p=[card(),card()]; d=[card()]; over=false; render();
}
function hit(){
if(over)return;
p.push(card()); render();
if(sum(p)>21){alert('DÉPASSÉ ! (Bust)');over=true;}
}
function stand(){
if(over)return;
while(sum(d)<17)d.push(card());
render();
let ps=sum(p),ds=sum(d);
if(ds>21||ps>ds){alert('GAGNÉ ! +20$');chips+=20;}else{alert('PERDU ! -20$');chips-=20;}
document.getElementById('chips').textContent=chips; over=true;
}
function render(){
document.getElementById('pc').innerHTML=p.map(c=>`<div class="card">${c}</div>`).join('');
document.getElementById('dc').innerHTML=d.map(c=>`<div class="card">${c}</div>`).join('');
document.getElementById('ps').textContent=sum(p);
document.getElementById('ds').textContent=sum(d);
}
deal();
</script></body></html>
)rawliteral";

// 27. Memory Flip 16
const char GAME_MEMCARDS_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Memory 16</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:320px;display:flex;justify-content:space-between;margin-bottom:10px;font-weight:bold;}
.grid{display:grid;grid-template-columns:repeat(4,1fr);gap:8px;width:320px;}
.c{aspect-ratio:1;background:#171d33;border:1px solid #00f3ff;border-radius:10px;display:flex;align-items:center;justify-content:center;font-size:1.8rem;cursor:pointer;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Paires: <span id="s">0</span>/8</span></header>
<div class="grid" id="g"></div>
<script>
const ICONS=['🚀','👾','💎','⭐','🍎','🔥','🎾','🤖'];
let deck=[],flipped=[],matched=[],s=0;
function init(){
deck=[...ICONS,...ICONS].sort(()=>Math.random()-0.5);
flipped=[];matched=[];s=0;render();
}
function flip(i){
if(flipped.length===2||flipped.includes(i)||matched.includes(i))return;
flipped.push(i); render();
if(flipped.length===2){
const [a,b]=flipped;
if(deck[a]===deck[b]){matched.push(a,b);s++;document.getElementById('s').textContent=s;flipped=[];if(s===8)alert('BRAVO ! TOUT TROUVÉ !');}
else setTimeout(()=>{flipped=[];render();},600);
}}
function render(){
const gEl=document.getElementById('g');gEl.innerHTML='';
deck.forEach((v,i)=>{
const d=document.createElement('div');d.className='c';
d.textContent=(flipped.includes(i)||matched.includes(i))?v:'❓';
d.onclick=()=>flip(i);gEl.appendChild(d);
});}
init();
</script></body></html>
)rawliteral";

// 28. Pierre-Feuille-Ciseaux Duel
const char GAME_PFC_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>PFC Duel</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:320px;display:flex;justify-content:space-between;margin-bottom:14px;font-weight:bold;}
.arena{width:320px;background:#171d33;border:2px solid #ff0055;border-radius:14px;padding:24px;text-align:center;box-sizing:border-box;}
.btns{display:flex;gap:8px;margin-top:16px;width:320px;}
button{flex:1;height:52px;background:rgba(255,255,255,0.08);border:1px solid #00f3ff;color:#fff;border-radius:10px;font-size:1.4rem;cursor:pointer;}
button:active{background:#00f3ff;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Série: <span id="s">0</span></span></header>
<div class="arena">
<div style="font-size:2rem;margin-bottom:10px;" id="vs">❓ VS ❓</div>
<div id="res" style="font-weight:bold;color:#00f3ff;">Choisis ton coup !</div>
</div>
<div class="btns">
<button onclick="play('🪨')">🪨</button><button onclick="play('📄')">📄</button><button onclick="play('✂️')">✂️</button>
</div>
<script>
let streak=0;
function play(u){
const c=['🪨','📄','✂️'],ai=c[Math.floor(Math.random()*3)];
document.getElementById('vs').textContent=`${u} vs ${ai}`;
if(u===ai){document.getElementById('res').textContent="Égalité ! Rejoue !";}
else if((u==='🪨'&&ai==='✂️')||(u==='📄'&&ai==='🪨')||(u==='✂️'&&ai==='📄')){
streak++; document.getElementById('s').textContent=streak;
document.getElementById('res').textContent="Victoire ! +1";
} else {streak=0;document.getElementById('s').textContent='0';document.getElementById('res').textContent="Défaite !";}
}
</script></body></html>
)rawliteral";

// 29. Plus ou Moins (High-Low)
const char GAME_HIGHLOW_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>High Low</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:320px;display:flex;justify-content:space-between;margin-bottom:14px;font-weight:bold;}
.card{width:120px;height:170px;background:#fff;color:#000;border-radius:12px;display:flex;align-items:center;justify-content:center;font-size:3rem;font-weight:bold;margin:16px auto;}
.ctrl{display:flex;gap:12px;width:320px;}
button{flex:1;height:50px;border-radius:10px;border:none;font-weight:bold;font-size:1.1rem;cursor:pointer;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<div class="card" id="c">7</div>
<div class="ctrl">
<button onclick="guess('high')" style="background:#00ff66;color:#000;">▲ PLUS HAUT</button>
<button onclick="guess('low')" style="background:#ff0055;color:#fff;">▼ PLUS BAS</button>
</div>
<script>
let cur=7,score=0;
function guess(dir){
let next=Math.floor(Math.random()*13)+1;
document.getElementById('c').textContent=next;
if((dir==='high'&&next>=cur)||(dir==='low'&&next<=cur)){
score++;document.getElementById('s').textContent=score;
}else{alert('PERDU ! Score: '+score);score=0;document.getElementById('s').textContent='0';}
cur=next;
}
</script></body></html>
)rawliteral";

// 30. Bataille Navale Mini
const char GAME_BATTLESHIP_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Bataille Navale</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:300px;display:flex;justify-content:space-between;margin-bottom:12px;font-weight:bold;}
.grid{display:grid;grid-template-columns:repeat(5,1fr);gap:6px;width:300px;}
.c{aspect-ratio:1;background:#0d182b;border:1px solid #00f3ff;border-radius:8px;display:flex;align-items:center;justify-content:center;font-size:1.5rem;cursor:pointer;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Navires: <span id="s">3</span>/3</span></header>
<div class="grid" id="g"></div>
<script>
let grid=Array(25).fill(false),hits=0,shots=0;
function init(){
grid=Array(25).fill(false);hits=0;shots=0;
let p=0;while(p<3){let idx=Math.floor(Math.random()*25);if(!grid[idx]){grid[idx]=true;p++;}}
render();
}
function shoot(i,el){
if(el.textContent)return;
shots++;
if(grid[i]){el.textContent='💥';el.style.background='#ff0055';hits++;if(hits===3)alert('VICTOIRE ! Tous les navires coulés en '+shots+' tirs !');}
else{el.textContent='🌊';el.style.background='#080c14';}
document.getElementById('s').textContent=3-hits;
}
function render(){
const gEl=document.getElementById('g');gEl.innerHTML='';
for(let i=0;i<25;i++){
const d=document.createElement('div');d.className='c';
d.onclick=()=>shoot(i,d);gEl.appendChild(d);
}}
init();
</script></body></html>
)rawliteral";

#endif
