#ifndef GAME_PACK_PUZZLE_H
#define GAME_PACK_PUZZLE_H

#include <pgmspace.h>

// 19. Lights Out Matrix
const char GAME_LIGHTSOUT_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Lights Out</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:330px;display:flex;justify-content:space-between;margin-bottom:12px;font-weight:bold;}
.grid{display:grid;grid-template-columns:repeat(5,1fr);gap:8px;width:330px;max-width:92vw;}
.btn{aspect-ratio:1;border-radius:10px;border:1px solid #00f3ff;background:#0d1120;cursor:pointer;transition:all 0.15s;}
.btn.on{background:#ffe600;box-shadow:0 0 15px #ffe600;border-color:#ffe600;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Coups: <span id="s">0</span></span></header>
<div style="color:#8b9bb4;font-size:0.85rem;margin-bottom:12px;text-align:center;">Éteins toutes les lampes jaunes !</div>
<div class="grid" id="g"></div>
<button onclick="init()" style="margin-top:16px;width:330px;height:46px;background:#00f3ff;border:none;border-radius:10px;font-weight:bold;cursor:pointer;">RÉINITIALISER 🔄</button>
<script>
const gEl=document.getElementById('g'),sEl=document.getElementById('s');
let g=Array(25).fill(false),moves=0;
function toggle(idx){if(idx>=0&&idx<25)g[idx]=!g[idx];}
function clickCell(r,c){
moves++; sEl.textContent=moves;
toggle(r*5+c); if(r>0)toggle((r-1)*5+c); if(r<4)toggle((r+1)*5+c); if(c>0)toggle(r*5+c-1); if(c<4)toggle(r*5+c+1);
render();
if(g.every(x=>!x))alert('VICTOIRE EN '+moves+' COUPS !');
}
function render(){
gEl.innerHTML='';
for(let r=0;r<5;r++)for(let c=0;c<5;c++){
const d=document.createElement('div');
d.className='btn'+(g[r*5+c]?' on':'');
d.onclick=()=>clickCell(r,c);
gEl.appendChild(d);
}}
function init(){moves=0;sEl.textContent='0';for(let i=0;i<25;i++)g[i]=Math.random()>0.5;render();}
init();
</script></body></html>
)rawliteral";

// 20. Slide 15 Puzzle
const char GAME_SLIDE15_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Taquin 15</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:330px;display:flex;justify-content:space-between;margin-bottom:12px;font-weight:bold;}
.grid{display:grid;grid-template-columns:repeat(4,1fr);gap:8px;width:330px;max-width:92vw;}
.t{aspect-ratio:1;background:#171d33;border:1px solid #00f3ff;border-radius:10px;display:flex;align-items:center;justify-content:center;font-size:1.4rem;font-weight:bold;color:#00f3ff;cursor:pointer;}
.t.empty{background:transparent;border:none;cursor:default;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Coups: <span id="s">0</span></span></header>
<div class="grid" id="g"></div>
<button onclick="init()" style="margin-top:16px;width:330px;height:46px;background:#00f3ff;border:none;border-radius:10px;font-weight:bold;cursor:pointer;">MÉLANGER 🔀</button>
<script>
let b=[],moves=0;
const gEl=document.getElementById('g'),sEl=document.getElementById('s');
function init(){
b=[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0].sort(()=>Math.random()-0.5);
moves=0; sEl.textContent='0'; render();
}
function clickTile(i){
const e=b.indexOf(0);
const r1=Math.floor(i/4),c1=i%4,r2=Math.floor(e/4),c2=e%4;
if(Math.abs(r1-r2)+Math.abs(c1-c2)===1){
b[e]=b[i]; b[i]=0; moves++; sEl.textContent=moves; render();
}
}
function render(){
gEl.innerHTML='';
b.forEach((v,i)=>{
const d=document.createElement('div');
d.className='t'+(v===0?' empty':'');
d.textContent=v||'';
d.onclick=()=>clickTile(i);
gEl.appendChild(d);
});}
init();
</script></body></html>
)rawliteral";

// 21. Color Flood
const char GAME_COLORFLOOD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Color Flood</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:320px;display:flex;justify-content:space-between;margin-bottom:10px;font-weight:bold;}
canvas{border:2px solid #00f3ff;border-radius:10px;width:320px;height:320px;max-width:92vw;}
.pal{display:flex;gap:8px;width:100%;max-width:320px;margin-top:12px;}
.p-btn{flex:1;height:45px;border-radius:8px;border:2px solid #fff;cursor:pointer;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Coups restants: <span id="s">22</span></span></header>
<canvas id="c" width="300" height="300"></canvas>
<div class="pal" id="pal"></div>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
const colors=['#ff0055','#00f3ff','#ffe600','#00ff66','#9d4edd'];
let N=10,g=[],left=22;
function init(){
g=[];left=22;sEl.textContent=left;
for(let r=0;r<N;r++){g[r]=[];for(let col=0;col<N;col++)g[r][col]=Math.floor(Math.random()*5);}
render();
}
function flood(r,col,oldC,newC){
if(r<0||r>=N||col<0||col>=N||g[r][col]!==oldC||oldC===newC)return;
g[r][col]=newC;
flood(r+1,col,oldC,newC);flood(r-1,col,oldC,newC);flood(r,col+1,oldC,newC);flood(r,col-1,oldC,newC);
}
function pick(cIdx){
if(cIdx===g[0][0])return;
flood(0,0,g[0][0],cIdx);
left--;sEl.textContent=left;render();
let won=g.every(row=>row.every(v=>v===cIdx));
if(won)alert('VICTOIRE !');else if(left<=0)alert('PERDU !');
}
function render(){
for(let r=0;r<N;r++)for(let col=0;col<N;col++){ctx.fillStyle=colors[g[r][col]];ctx.fillRect(col*30,r*30,30,30);}
}
const palEl=document.getElementById('pal');
colors.forEach((col,i)=>{
const b=document.createElement('div');b.className='p-btn';b.style.background=col;b.onclick=()=>pick(i);palEl.appendChild(b);
});
init();
</script></body></html>
)rawliteral";

// 22. Match-3 Gem Blitz
const char GAME_MATCH3_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Match-3</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:320px;display:flex;justify-content:space-between;margin-bottom:10px;font-weight:bold;}
.grid{display:grid;grid-template-columns:repeat(6,1fr);gap:4px;width:320px;max-width:92vw;}
.gem{aspect-ratio:1;border-radius:8px;display:flex;align-items:center;justify-content:center;font-size:1.6rem;cursor:pointer;background:rgba(255,255,255,0.08);}
.gem.sel{border:2px solid #fff;box-shadow:0 0 10px #00f3ff;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<div class="grid" id="g"></div>
<script>
const ICONS=['💎','🔮','⭐','🍎','🍀'];
let g=[],sel=null,score=0,gEl=document.getElementById('g'),sEl=document.getElementById('s');
function init(){
score=0;sEl.textContent='0';
for(let r=0;r<6;r++){g[r]=[];for(let c=0;c<6;c++)g[r][c]=Math.floor(Math.random()*5);}
render();
}
function clickGem(r,c){
if(!sel){sel={r,c};render();return;}
if(Math.abs(sel.r-r)+Math.abs(sel.c-c)===1){
let tmp=g[r][c];g[r][c]=g[sel.r][sel.c];g[sel.r][sel.c]=tmp;
checkMatches();
}
sel=null;render();
}
function checkMatches(){
for(let r=0;r<6;r++)for(let c=0;c<4;c++){
if(g[r][c]===g[r][c+1]&&g[r][c]===g[r][c+2]){score+=30;g[r][c]=Math.floor(Math.random()*5);g[r][c+1]=Math.floor(Math.random()*5);g[r][c+2]=Math.floor(Math.random()*5);}
}
sEl.textContent=score;
}
function render(){
gEl.innerHTML='';
for(let r=0;r<6;r++)for(let c=0;c<6;c++){
const d=document.createElement('div');
d.className='gem'+(sel&&sel.r===r&&sel.c===c?' sel':'');
d.textContent=ICONS[g[r][c]];
d.onclick=()=>clickGem(r,c);
gEl.appendChild(d);
}}
init();
</script></body></html>
)rawliteral";

// 23. Code Breaker
const char GAME_CODEBREAKER_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Code Breaker</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:320px;display:flex;justify-content:space-between;margin-bottom:10px;font-weight:bold;}
.slot{display:flex;gap:8px;margin-bottom:8px;}
.peg{width:40px;height:40px;border-radius:50%;background:#222;border:2px solid #444;cursor:pointer;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Essais: <span id="s">10</span></span></header>
<div style="font-size:0.85rem;color:#8b9bb4;margin-bottom:12px;">Devine la combinaison de 4 couleurs secrètes !</div>
<div class="slot" id="guess"></div>
<button onclick="submitGuess()" style="width:320px;height:45px;background:#00f3ff;border:none;border-radius:10px;font-weight:bold;margin-top:10px;">TESTER LE CODE 🔓</button>
<div id="logs" style="margin-top:14px;width:320px;font-size:0.85rem;"></div>
<script>
const COLS=['#ff0055','#00f3ff','#ffe600','#00ff66','#9d4edd','#ff7700'];
let code=[],cur=[0,0,0,0],tries=10;
function init(){
code=[0,1,2,3,4,5].sort(()=>Math.random()-0.5).slice(0,4);
tries=10;cur=[0,0,0,0];document.getElementById('logs').innerHTML='';
renderGuess();
}
function renderGuess(){
const gEl=document.getElementById('guess');gEl.innerHTML='';
cur.forEach((c,i)=>{
const d=document.createElement('div');d.className='peg';d.style.background=COLS[c];
d.onclick=()=>{cur[i]=(cur[i]+1)%6;renderGuess();};gEl.appendChild(d);
});}
function submitGuess(){
let exact=0,present=0;
for(let i=0;i<4;i++){if(cur[i]===code[i])exact++;else if(code.includes(cur[i]))present++;}
const l=document.getElementById('logs');
l.innerHTML=`<div>Essai ${11-tries}: ${exact} Bien placés, ${present} Mauvaise place</div>`+l.innerHTML;
if(exact===4){alert('VICTOIRE ! CODE TROUVÉ !');init();}
tries--;document.getElementById('s').textContent=tries;
if(tries<=0){alert('GAME OVER !');init();}
}
init();
</script></body></html>
)rawliteral";

// 24. Mini Sudoku 4x4
const char GAME_SUDOKU_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Sudoku 4x4</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:300px;display:flex;justify-content:space-between;margin-bottom:12px;font-weight:bold;}
.grid{display:grid;grid-template-columns:repeat(4,1fr);gap:6px;width:280px;}
.c{aspect-ratio:1;background:#171d33;border:2px solid #00f3ff;border-radius:8px;display:flex;align-items:center;justify-content:center;font-size:1.8rem;font-weight:bold;color:#ffe600;cursor:pointer;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Sudoku 4x4</span></header>
<div class="grid" id="g"></div>
<button onclick="check()" style="margin-top:16px;width:280px;height:45px;background:#00f3ff;border:none;border-radius:10px;font-weight:bold;">VÉRIFIER LA GRILLE ✔</button>
<script>
let b=[1,0,0,4, 0,2,3,0, 0,3,2,0, 4,0,0,1];
function render(){
const gEl=document.getElementById('g');gEl.innerHTML='';
b.forEach((v,i)=>{
const d=document.createElement('div');d.className='c';d.textContent=v||'';
d.onclick=()=>{b[i]=(b[i]%4)+1;render();};gEl.appendChild(d);
});}
function check(){
if(!b.includes(0))alert('BRAVO ! GRILLE COMPLÈTE !');else alert('Il reste des cases vides !');
}
render();
</script></body></html>
)rawliteral";

#endif
