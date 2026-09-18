#ifndef GAME_PACK_SPORTS_H
#define GAME_PACK_SPORTS_H

#include <pgmspace.h>

// 31. Tirs au But Penalty
const char GAME_PENALTY_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Penalty</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #00ff66;border-radius:12px;background:#051408;width:340px;height:400px;max-width:92vw;}
.ctrl{display:flex;gap:8px;width:340px;margin-top:8px;}
button{flex:1;height:46px;background:rgba(255,255,255,0.1);border:1px solid #00ff66;color:#fff;border-radius:10px;font-weight:bold;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Buts: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<div class="ctrl"><button onclick="shoot(-1)">◀ GAUCHE</button><button onclick="shoot(0)">▲ CENTRE</button><button onclick="shoot(1)">DROITE ▶</button></div>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let bx=170,by=340,bvx=0,bvy=0,gx=170,gvx=2,score=0,kicked=false;
function shoot(dir){
if(kicked)return;
bvx=dir*5; bvy=-8; kicked=true;
}
function loop(){
gx+=gvx; if(gx<110||gx>230)gvx=-gvx;
if(kicked){
bx+=bvx; by+=bvy;
if(by<120){
if(Math.abs(bx-gx)<35){alert('ARRÊT DU GARDIEN !');}else{score++;sEl.textContent=score;}
bx=170;by=340;bvx=0;bvy=0;kicked=false;
}}
ctx.fillStyle='#051408';ctx.fillRect(0,0,c.width,c.height);
// Cage
ctx.strokeStyle='#fff';ctx.lineWidth=4;ctx.strokeRect(60,60,220,100);
// Gardien
ctx.fillStyle='#ff0055';ctx.fillRect(gx-18,90,36,40);
// Ballon
ctx.fillStyle='#ffe600';ctx.beginPath();ctx.arc(bx,by,10,0,Math.PI*2);ctx.fill();
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// 32. Basket Dunk Shoot
const char GAME_BASKET_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Basket Dunk</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ff7700;border-radius:12px;background:#060810;width:340px;height:400px;max-width:92vw;}
button{width:340px;height:46px;background:#ff7700;border:none;border-radius:10px;font-weight:bold;margin-top:8px;font-size:1rem;color:#fff;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Paniers: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<button onclick="shoot()">LANCER LE BALLON 🏀</button>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let bx=60,by=320,bvx=0,bvy=0,flying=false,power=11,score=0;
function shoot(){if(!flying){bvx=6.2;bvy=-power;flying=true;}}
function loop(){
if(flying){
bx+=bvx; by+=bvy; bvy+=0.35;
if(Math.hypot(bx-260,by-160)<16){score++;sEl.textContent=score;}
if(by>400||bx>340){bx=60;by=320;bvx=0;bvy=0;flying=false;}
}
ctx.fillStyle='#060810';ctx.fillRect(0,0,c.width,c.height);
// Panier
ctx.strokeStyle='#ff0055';ctx.lineWidth=4;ctx.strokeRect(270,120,4,80);
ctx.fillStyle='#ff7700';ctx.fillRect(240,160,30,6);
// Ballon
ctx.fillStyle='#ff7700';ctx.beginPath();ctx.arc(bx,by,12,0,Math.PI*2);ctx.fill();
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// 33. Sprint 100m Dash
const char GAME_DASH_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Sprint 100m</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:10px;font-weight:bold;}
.track{width:330px;background:#2b1408;border:2px solid #ffe600;border-radius:12px;padding:20px 10px;position:relative;height:120px;box-sizing:border-box;}
.runner{position:absolute;bottom:30px;font-size:2.5rem;transition:left 0.05s;}
.ctrl{display:flex;gap:12px;width:330px;margin-top:16px;}
button{flex:1;height:55px;background:rgba(255,255,255,0.1);border:2px solid #00f3ff;color:#fff;border-radius:12px;font-size:1.4rem;}
button:active{background:#00f3ff;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Chrono: <span id="t">0.00</span>s</span></header>
<div class="track"><div class="runner" id="r" style="left:10px;">🏃</div></div>
<div class="ctrl"><button onclick="step('L')">◀ G</button><button onclick="step('R')">D ▶</button></div>
<script>
let pos=10,last='',start=0,timer=null,running=false;
function step(side){
if(!running){running=true;start=Date.now();timer=setInterval(()=>{document.getElementById('t').textContent=((Date.now()-start)/1000).toFixed(2);},30);}
if(side!==last){pos+=7;last=side;document.getElementById('r').style.left=pos+'px';if(pos>=270){clearInterval(timer);alert('ARRIVÉE ! Temps: '+document.getElementById('t').textContent+'s');pos=10;running=false;}}}
</script></body></html>
)rawliteral";

// 34. Ski Slalom
const char GAME_SKI_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Ski Slalom</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #00f3ff;border-radius:12px;background:#eef6ff;width:340px;height:400px;max-width:92vw;}
.ctrl{display:flex;gap:8px;width:340px;margin-top:8px;}
button{flex:1;height:46px;background:rgba(255,255,255,0.1);border:1px solid #00f3ff;color:#fff;border-radius:10px;font-weight:bold;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Portes: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<div class="ctrl"><button onclick="px-=25">◀ GAUCHE</button><button onclick="px+=25">DROITE ▶</button></div>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let px=170,gates=[],score=0,dead=false,timer=0;
function loop(){
if(!dead){
timer++;if(timer%60===0)gates.push({y:400,x:Math.random()*200+50});
gates.forEach((g,gi)=>{g.y-=4;if(Math.abs(g.y-80)<10){if(px>g.x&&px<g.x+60){score++;sEl.textContent=score;}else dead=true;}if(g.y<0)gates.splice(gi,1);});
}
ctx.fillStyle='#dceeff';ctx.fillRect(0,0,c.width,c.height);
ctx.fillStyle='#ff0055';gates.forEach(g=>{ctx.fillRect(g.x,g.y,8,20);ctx.fillRect(g.x+60,g.y,8,20);});
ctx.fillStyle='#0044ff';ctx.fillRect(px-8,70,16,24);
if(dead){ctx.fillStyle='#ff0055';ctx.font='bold 24px sans-serif';ctx.textAlign='center';ctx.fillText('DISQUALIFIÉ !',170,200);}
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// 35. Tir à l'Arc Bullseye
const char GAME_ARCHERY_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Tir à l'Arc</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ffe600;border-radius:12px;background:#05070f;width:340px;height:400px;max-width:92vw;}
button{width:340px;height:48px;background:#ffe600;color:#000;border:none;border-radius:10px;font-weight:bold;margin-top:8px;font-size:1.1rem;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<button onclick="shoot()">DÉCOCHER LA FLÈCHE 🏹</button>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let tx=170,tvx=3,score=0;
function shoot(){
let diff=Math.abs(tx-170);
let pts=Math.max(0,100-Math.floor(diff*2));
score+=pts; sEl.textContent=score; alert(`Tir : ${pts} points !`);
}
function loop(){
tx+=tvx; if(tx<80||tx>260)tvx=-tvx;
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
// Cible
const rings=[{r:80,c:'#fff'},{r:60,c:'#00f3ff'},{r:40,c:'#ff0055'},{r:20,c:'#ffe600'}];
rings.forEach(rg=>{ctx.fillStyle=rg.c;ctx.beginPath();ctx.arc(tx,150,rg.r,0,Math.PI*2);ctx.fill();});
// Viseur
ctx.strokeStyle='#00ff66';ctx.lineWidth=2;ctx.strokeRect(150,130,40,40);
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// 36. Air Hockey Flash
const char GAME_AIRHOCKEY_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Air Hockey</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #00f3ff;border-radius:12px;background:#070b18;width:340px;height:400px;max-width:92vw;}
</style><script src="/pause.js"></script></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Toi: <span id="ps">0</span> | IA: <span id="as">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d');
let px=170,py=350,ax=170,ay=50,bx=170,by=200,bvx=3,bvy=4,ps=0,as=0;
function loop(){
bx+=bvx; by+=bvy;
if(bx<10||bx>330)bvx=-bvx;
if(Math.hypot(bx-px,by-py)<30){bvy=-Math.abs(bvy)*1.05;bvx=(bx-px)*0.2;}
if(Math.hypot(bx-ax,by-ay)<30){bvy=Math.abs(bvy)*1.05;}
ax+=(bx-ax)*0.08;
if(by>400){as++;document.getElementById('as').textContent=as;bx=170;by=200;bvy=-4;}
if(by<0){ps++;document.getElementById('ps').textContent=ps;bx=170;by=200;bvy=4;}
ctx.fillStyle='#070b18';ctx.fillRect(0,0,c.width,c.height);
ctx.fillStyle='#00f3ff';ctx.beginPath();ctx.arc(px,py,22,0,Math.PI*2);ctx.fill();
ctx.fillStyle='#ff0055';ctx.beginPath();ctx.arc(ax,ay,22,0,Math.PI*2);ctx.fill();
ctx.fillStyle='#ffe600';ctx.beginPath();ctx.arc(bx,by,12,0,Math.PI*2);ctx.fill();
requestAnimationFrame(loop);
}
loop();
c.ontouchmove=c.onmousemove=e=>{
const rect=c.getBoundingClientRect();px=((e.touches?e.touches[0].clientX:e.clientX)-rect.left)*(c.width/rect.width);
};
</script></body></html>
)rawliteral";

#endif
