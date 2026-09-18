#ifndef GAME_PACK_RETRO_H
#define GAME_PACK_RETRO_H

#include <pgmspace.h>

// 47. Cyber Tetrominoes (Tetris)
const char GAME_TETRIS_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Tetris</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:300px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #00f3ff;border-radius:10px;background:#05070f;width:240px;height:400px;}
.ctrl{display:grid;grid-template-columns:1fr 1fr 1fr 1fr;gap:6px;width:240px;margin-top:8px;}
button{height:46px;background:rgba(255,255,255,0.1);border:1px solid #00f3ff;color:#fff;border-radius:8px;font-weight:bold;font-size:1.1rem;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Lignes: <span id="s">0</span></span></header>
<canvas id="c" width="200" height="360"></canvas>
<div class="ctrl">
<button onclick="m(-1,0)">◀</button><button onclick="rot()">↻</button><button onclick="m(1,0)">▶</button><button onclick="m(0,1)">▼</button>
</div>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
const SHAPES=[[[1,1,1,1]],[[1,1],[1,1]],[[1,1,1],[0,1,0]],[[1,1,1],[1,0,0]],[[1,1,0],[0,1,1]]];
let g=Array(18).fill(null).map(()=>Array(10).fill(0)),cur={x:4,y:0,s:SHAPES[0]},lines=0,timer=0;
function spawn(){cur={x:4,y:0,s:SHAPES[Math.floor(Math.random()*SHAPES.length)]};if(collide(0,0))alert('GAME OVER !');}
function collide(dx,dy,s=cur.s){
for(let r=0;r<s.length;r++)for(let col=0;col<s[r].length;col++){
if(s[r][col]){let nx=cur.x+col+dx,ny=cur.y+r+dy;if(nx<0||nx>=10||ny>=18||(ny>=0&&g[ny][nx]))return true;}
}return false;}
function m(dx,dy){if(!collide(dx,dy)){cur.x+=dx;cur.y+=dy;}else if(dy>0){
for(let r=0;r<cur.s.length;r++)for(let col=0;col<cur.s[r].length;col++)if(cur.s[r][col])g[cur.y+r][cur.x+col]=1;
for(let r=17;r>=0;r--)if(g[r].every(v=>v===1)){g.splice(r,1);g.unshift(Array(10).fill(0));lines++;sEl.textContent=lines;r++;}
spawn();
}}
function rot(){
let ns=cur.s[0].map((_,i)=>cur.s.map(row=>row[i]).reverse());
if(!collide(0,0,ns))cur.s=ns;
}
function loop(){
timer++;if(timer%30===0)m(0,1);
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
for(let r=0;r<18;r++)for(let col=0;col<10;col++)if(g[r][col]){ctx.fillStyle='#00f3ff';ctx.fillRect(col*20+1,r*20+1,18,18);}
ctx.fillStyle='#ffe600';
for(let r=0;r<cur.s.length;r++)for(let col=0;col<cur.s[r].length;col++)if(cur.s[r][col])ctx.fillRect((cur.x+col)*20+1,(cur.y+r)*20+1,18,18);
requestAnimationFrame(loop);
}
spawn();loop();
</script></body></html>
)rawliteral";

// 48. Cyber Pac-Maze
const char GAME_PACMAZE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Pac-Maze</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:300px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ffe600;border-radius:10px;background:#05070f;width:280px;height:280px;}
.ctrl{display:grid;grid-template-columns:1fr 1fr 1fr;gap:6px;width:280px;margin-top:8px;}
button{height:46px;background:rgba(255,255,255,0.1);border:1px solid #ffe600;color:#fff;border-radius:8px;font-weight:bold;font-size:1.2rem;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<canvas id="c" width="280" height="280"></canvas>
<div class="ctrl">
<div></div><button onclick="m(0,-1)">▲</button><div></div>
<button onclick="m(-1,0)">◀</button><button onclick="m(0,1)">▼</button><button onclick="m(1,0)">▶</button>
</div>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let px=1,py=1,gx=8,gy=8,dots=[],score=0;
for(let r=0;r<10;r++)for(let col=0;col<10;col++)dots.push({x:col,y:r});
function m(dx,dy){
px=Math.max(0,Math.min(9,px+dx)); py=Math.max(0,Math.min(9,py+dy));
dots.forEach((d,di)=>{if(d.x===px&&d.y===py){score+=10;sEl.textContent=score;dots.splice(di,1);}});
}
function loop(){
if(Math.random()<0.05){gx+=Math.sign(px-gx);gy+=Math.sign(py-gy);}
if(px===gx&&py===gy)alert('ATTRAPÉ PAR LE FANTÔME !');
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
// Dots
ctx.fillStyle='#ffe600';dots.forEach(d=>ctx.fillRect(d.x*28+12,d.y*28+12,4,4));
// Pacman
ctx.fillStyle='#ffe600';ctx.beginPath();ctx.arc(px*28+14,py*28+14,10,0,Math.PI*2);ctx.fill();
// Ghost
ctx.fillStyle='#ff0055';ctx.beginPath();ctx.arc(gx*28+14,gy*28+14,10,0,Math.PI*2);ctx.fill();
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// 49. Lunar Lander
const char GAME_LUNAR_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Lunar Lander</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #fff;border-radius:12px;background:#030408;width:340px;height:400px;max-width:92vw;}
button{width:340px;height:48px;background:linear-gradient(135deg,#ff5500,#ffe600);color:#000;border:none;border-radius:10px;font-weight:bold;margin-top:8px;font-size:1.1rem;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Vitesse: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<button onclick="thrust()">🔥 PROPULSEUR</button>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let y=40,vy=0,fuel=100,dead=false;
function thrust(){if(fuel>0){vy-=1.8;fuel-=5;}}
function loop(){
if(!dead){
vy+=0.08; y+=vy; sEl.textContent=Math.round(vy*10);
if(y>=360){
dead=true;
if(vy<2.2)alert('ALUNISSAGE PARFAIT ! 🌕');else alert('CRASH SUR LA LUNE !');
}}
ctx.fillStyle='#030408';ctx.fillRect(0,0,c.width,c.height);
// Sol lune
ctx.fillStyle='#8b9bb4';ctx.fillRect(0,370,c.width,30);
// Vaisseau
ctx.fillStyle='#00f3ff';ctx.fillRect(160,y,20,16);
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// 50. Cannon Castle Blaster
const char GAME_CANNON_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Cannon Blaster</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ff0055;border-radius:12px;background:#05070f;width:340px;height:400px;max-width:92vw;}
button{width:340px;height:48px;background:#ff0055;color:#fff;border:none;border-radius:10px;font-weight:bold;margin-top:8px;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Châteaux: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<button onclick="fire()">TIRER AU CANON 💣</button>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let bx=30,by=350,bvx=0,bvy=0,flying=false,tx=260,score=0;
function fire(){if(!flying){bvx=7.5;bvy=-7.5;flying=true;}}
function loop(){
if(flying){
bx+=bvx; by+=bvy; bvy+=0.25;
if(Math.hypot(bx-tx,by-330)<30){score++;sEl.textContent=score;tx=Math.random()*150+150;bx=30;by=350;flying=false;}
if(by>370||bx>340){bx=30;by=350;flying=false;}
}
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
// Sol
ctx.fillStyle='#00ff66';ctx.fillRect(0,370,c.width,30);
// Canon
ctx.fillStyle='#8b9bb4';ctx.fillRect(10,340,30,30);
// Cible Château
ctx.fillStyle='#ff0055';ctx.fillRect(tx-15,310,30,60);
// Boulet
ctx.fillStyle='#ffe600';ctx.beginPath();ctx.arc(bx,by,8,0,Math.PI*2);ctx.fill();
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

#endif
