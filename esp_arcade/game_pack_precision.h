#ifndef GAME_PACK_PRECISION_H
#define GAME_PACK_PRECISION_H

#include <pgmspace.h>

// 42. Fil Électrique Buzz
const char GAME_WIRE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Fil Électrique</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ffe600;border-radius:12px;background:#05070f;width:340px;height:400px;max-width:92vw;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Progression: <span id="s">0</span>%</span></header>
<canvas id="c" width="340" height="400"></canvas>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let py=20,dead=false;
function wireY(x){return 200+Math.sin(x*0.03)*80+Math.cos(x*0.06)*30;}
function loop(){
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
// Fil
ctx.strokeStyle='#ffe600';ctx.lineWidth=8;
ctx.beginPath();
for(let x=0;x<340;x++){let y=wireY(x);if(x===0)ctx.moveTo(x,y);else ctx.lineTo(x,y);}
ctx.stroke();
// Anneau joueur
ctx.strokeStyle='#00f3ff';ctx.lineWidth=4;
ctx.beginPath();ctx.arc(px,py,18,0,Math.PI*2);ctx.stroke();
if(dead){ctx.fillStyle='#ff0055';ctx.font='bold 24px sans-serif';ctx.textAlign='center';ctx.fillText('⚡ BUZZ ! CHOC ÉLECTRIQUE ⚡',170,100);}
requestAnimationFrame(loop);
}
let px=20;
c.ontouchmove=c.onmousemove=e=>{
if(dead)return;
const rect=c.getBoundingClientRect();
px=((e.touches?e.touches[0].clientX:e.clientX)-rect.left)*(c.width/rect.width);
py=((e.touches?e.touches[0].clientY:e.clientY)-rect.top)*(c.height/rect.height);
sEl.textContent=Math.min(100,Math.floor(px/3.4));
let wy=wireY(px);
if(Math.abs(py-wy)>15){dead=true;}
if(px>=320&&!dead){alert('BRAVO ! PARCOURS RÉUSSI SANS TOUCHER LE FIL !');dead=true;}
};
c.onclick=()=>{if(dead){dead=false;px=20;py=200;}};
loop();
</script></body></html>
)rawliteral";

// 43. Perfect Slice
const char GAME_SLICE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Perfect Slice</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:330px;display:flex;justify-content:space-between;margin-bottom:12px;font-weight:bold;}
canvas{border:2px solid #00f3ff;border-radius:12px;background:#05070f;width:330px;height:380px;max-width:92vw;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<div style="color:#8b9bb4;font-size:0.85rem;margin-bottom:8px;">Coupe le bloc au centre parfait (50% / 50%) :</div>
<canvas id="c" width="330" height="380"></canvas>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let score=0;
function drawBlock(){
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
ctx.fillStyle='#00f3ff';ctx.fillRect(40,140,250,100);
}
c.onclick=e=>{
const rect=c.getBoundingClientRect();
let cutX=((e.clientX-rect.left)*(c.width/rect.width))-40;
if(cutX>0&&cutX<250){
let ratio=Math.round((cutX/250)*100);
let pts=Math.max(0,100-Math.abs(50-ratio)*4);
score+=pts;sEl.textContent=score;
alert(`Coupe : ${ratio}% / ${100-ratio}% (+${pts} pts)`);
drawBlock();
}
};
drawBlock();
</script></body></html>
)rawliteral";

// 44. Coin in the Jar
const char GAME_COINJAR_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Coin Jar</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ffe600;border-radius:12px;background:#05070f;width:340px;height:400px;max-width:92vw;}
button{width:340px;height:48px;background:#ffe600;color:#000;border:none;border-radius:10px;font-weight:bold;margin-top:8px;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Pièces: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<button onclick="drop()">LÂCHER LA PIÈCE 🪙</button>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let cx=170,cvx=3,cy=40,cvy=0,falling=false,score=0;
function drop(){if(!falling){falling=true;cvy=2;}}
function loop(){
if(!falling){cx+=cvx;if(cx<40||cx>300)cvx=-cvx;}
else{
cy+=cvy;cvy+=0.3;
if(cy>=350){
if(Math.abs(cx-170)<30){score++;sEl.textContent=score;}
cy=40;falling=false;
}}
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
// Bocal
ctx.strokeStyle='#00f3ff';ctx.lineWidth=4;ctx.strokeRect(140,330,60,60);
// Pièce
ctx.fillStyle='#ffe600';ctx.beginPath();ctx.arc(cx,cy,12,0,Math.PI*2);ctx.fill();
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// 45. Grappin Harpoon
const char GAME_HOOK_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Grappin</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ff0055;border-radius:12px;background:#05070f;width:340px;height:400px;max-width:92vw;}
button{width:340px;height:48px;background:#ff0055;color:#fff;border:none;border-radius:10px;font-weight:bold;margin-top:8px;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Diamants: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<button onclick="launch()">LANCER LE GRAPPIN 🪝</button>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let a=0,av=0.04,hx=170,hy=30,len=30,firing=false,score=0;
let gems=[{x:80,y:300},{x:170,y:320},{x:260,y:280}];
function launch(){if(!firing)firing=true;}
function loop(){
if(!firing){a+=av;if(a<-1||a>1)av=-av;}
else{
len+=6;
hx=170+Math.sin(a)*len; hy=30+Math.cos(a)*len;
gems.forEach((g,gi)=>{if(Math.hypot(hx-g.x,hy-g.y)<20){score++;sEl.textContent=score;gems.splice(gi,1);len=30;firing=false;}});
if(len>380){len=30;firing=false;}
}
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
// Corde
ctx.strokeStyle='#ffe600';ctx.lineWidth=3;ctx.beginPath();ctx.moveTo(170,30);ctx.lineTo(170+Math.sin(a)*len,30+Math.cos(a)*len);ctx.stroke();
// Diamants
ctx.fillStyle='#00f3ff';gems.forEach(g=>{ctx.beginPath();ctx.arc(g.x,g.y,16,0,Math.PI*2);ctx.fill();});
if(!gems.length){gems=[{x:Math.random()*260+40,y:Math.random()*150+200},{x:Math.random()*260+40,y:Math.random()*150+200}];}
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// 46. Balance Marble
const char GAME_BALANCE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Balance</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #00ff66;border-radius:12px;background:#05070f;width:340px;height:400px;max-width:92vw;}
.ctrl{display:flex;gap:12px;width:340px;margin-top:8px;}
button{flex:1;height:48px;background:rgba(255,255,255,0.1);border:1px solid #00ff66;color:#fff;border-radius:10px;font-weight:bold;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Temps: <span id="s">0</span>s</span></header>
<canvas id="c" width="340" height="400"></canvas>
<div class="ctrl"><button onclick="tilt-=0.05">◀ INCLINER G</button><button onclick="tilt+=0.05">INCLINER D ▶</button></div>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let tilt=0,mx=0,mvx=0,time=0,dead=false;
setInterval(()=>{if(!dead){time++;sEl.textContent=time;}},1000);
function loop(){
if(!dead){
mvx+=Math.sin(tilt)*0.8; mx+=mvx;
if(Math.abs(mx)>120){dead=true;alert('LA BILLE EST TOMBÉE !');}
}
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
ctx.save();ctx.translate(170,200);ctx.rotate(tilt);
// Barre
ctx.fillStyle='#00ff66';ctx.fillRect(-130,-6,260,12);
// Bille
ctx.fillStyle='#ffe600';ctx.beginPath();ctx.arc(mx,-18,12,0,Math.PI*2);ctx.fill();
ctx.restore();
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

#endif
