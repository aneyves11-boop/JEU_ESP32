#ifndef GAME_PACK_ACTION_H
#define GAME_PACK_ACTION_H

#include <pgmspace.h>

// 13. Asteroids Blaster
const char GAME_ASTEROIDS_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Asteroids</title>
<style>body{background:#060810;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #00f3ff;border-radius:12px;background:#030408;width:340px;height:400px;max-width:92vw;}
.ctrl{display:flex;gap:8px;width:100%;max-width:340px;margin-top:8px;}
button{flex:1;height:48px;background:rgba(255,255,255,0.1);border:1px solid #00f3ff;color:#fff;border-radius:10px;font-weight:bold;font-size:1.1rem;}
button:active{background:#00f3ff;color:#000;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<div class="ctrl"><button id="bl">↺</button><button id="bt">▲ GAZ</button><button id="br">↻</button><button id="bf" style="background:#ff0055;border-color:#ff0055;">🔥</button></div>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let ship={x:170,y:200,a:-Math.PI/2,vx:0,vy:0},rocks=[],lasers=[],score=0,dead=false;
function spawn(){for(let i=0;i<4;i++)rocks.push({x:Math.random()*c.width,y:Math.random()*100,r:25,vx:(Math.random()-0.5)*2,vy:Math.random()*1.5});}
spawn();
function fire(){lasers.push({x:ship.x,y:ship.y,vx:Math.cos(ship.a)*7,vy:Math.sin(ship.a)*7,life:40});}
function loop(){
if(!dead){
ship.x=(ship.x+ship.vx+c.width)%c.width; ship.y=(ship.y+ship.vy+c.height)%c.height;
ship.vx*=0.98; ship.vy*=0.98;
lasers.forEach((l,li)=>{l.x=(l.x+l.vx+c.width)%c.width;l.y=(l.y+l.vy+c.height)%c.height;l.life--;if(l.life<=0)lasers.splice(li,1);});
rocks.forEach((r,ri)=>{
r.x=(r.x+r.vx+c.width)%c.width; r.y=(r.y+r.vy+c.height)%c.height;
if(Math.hypot(r.x-ship.x,r.y-ship.y)<r.r+10){dead=true;}
lasers.forEach((l,li)=>{
if(Math.hypot(r.x-l.x,r.y-l.y)<r.r){
lasers.splice(li,1); score+=20; sEl.textContent=score;
if(r.r>14){rocks.push({x:r.x,y:r.y,r:r.r/2,vx:(Math.random()-0.5)*3,vy:(Math.random()-0.5)*3});rocks.push({x:r.x,y:r.y,r:r.r/2,vx:(Math.random()-0.5)*3,vy:(Math.random()-0.5)*3});}
rocks.splice(ri,1);
}});
});
if(rocks.length===0)spawn();
}
ctx.fillStyle='#030408';ctx.fillRect(0,0,c.width,c.height);
ctx.strokeStyle='#00f3ff';ctx.lineWidth=2;
lasers.forEach(l=>{ctx.fillStyle='#ff0055';ctx.fillRect(l.x-2,l.y-2,4,4);});
ctx.strokeStyle='#ffe600';rocks.forEach(r=>{ctx.beginPath();ctx.arc(r.x,r.y,r.r,0,Math.PI*2);ctx.stroke();});
if(!dead){
ctx.save();ctx.translate(ship.x,ship.y);ctx.rotate(ship.a);ctx.strokeStyle='#00ff66';
ctx.beginPath();ctx.moveTo(14,0);ctx.lineTo(-10,-8);ctx.lineTo(-6,0);ctx.lineTo(-10,8);ctx.closePath();ctx.stroke();ctx.restore();
} else {ctx.fillStyle='#ff0055';ctx.font='bold 24px sans-serif';ctx.textAlign='center';ctx.fillText('CRASH ! REJOUE',170,200);}
requestAnimationFrame(loop);
}
loop();
document.getElementById('bl').onclick=()=>ship.a-=0.3; document.getElementById('br').onclick=()=>ship.a+=0.3;
document.getElementById('bt').onclick=()=>{ship.vx+=Math.cos(ship.a)*1.8;ship.vy+=Math.sin(ship.a)*1.8;};
document.getElementById('bf').onclick=()=>{if(dead){dead=false;score=0;sEl.textContent='0';rocks=[];spawn();}else fire();};
</script></body></html>
)rawliteral";

// 14. Missile Command Defense
const char GAME_MISSILE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Missile Defense</title>
<style>body{background:#060810;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ff0055;border-radius:12px;background:#030408;width:340px;height:420px;max-width:92vw;cursor:crosshair;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="420"></canvas>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let missiles=[],blasts=[],score=0,timer=0,dead=false;
function spawnM(){missiles.push({x:Math.random()*c.width,y:0,tx:Math.random()*c.width,ty:c.height-20,vx:(Math.random()-0.5)*1.5,vy:1.8});}
function loop(){
if(!dead){
timer++; if(timer%40===0)spawnM();
missiles.forEach((m,mi)=>{
m.x+=m.vx; m.y+=m.vy;
if(m.y>=c.height-20){dead=true;}
blasts.forEach(b=>{if(Math.hypot(m.x-b.x,m.y-b.y)<b.r){missiles.splice(mi,1);score+=15;sEl.textContent=score;}});
});
blasts.forEach((b,bi)=>{b.r+=1.5;if(b.r>30)blasts.splice(bi,1);});
}
ctx.fillStyle='#030408';ctx.fillRect(0,0,c.width,c.height);
ctx.fillStyle='#00f3ff';ctx.fillRect(0,c.height-16,c.width,16);
ctx.strokeStyle='#ff0055';ctx.lineWidth=2;
missiles.forEach(m=>{ctx.beginPath();ctx.moveTo(m.x,0);ctx.lineTo(m.x,m.y);ctx.stroke();});
blasts.forEach(b=>{ctx.fillStyle='rgba(255,230,0,0.6)';ctx.beginPath();ctx.arc(b.x,b.y,b.r,0,Math.PI*2);ctx.fill();});
if(dead){ctx.fillStyle='#ff0055';ctx.font='bold 24px sans-serif';ctx.textAlign='center';ctx.fillText('BASE DÉTRUITE !',170,200);ctx.font='14px sans-serif';ctx.fillText('Clique pour rejouer',170,230);}
requestAnimationFrame(loop);
}
loop();
c.onclick=e=>{
if(dead){dead=false;score=0;sEl.textContent='0';missiles=[];blasts=[];return;}
const rect=c.getBoundingClientRect(),x=(e.clientX-rect.left)*(c.width/rect.width),y=(e.clientY-rect.top)*(c.height/rect.height);
blasts.push({x:x,y:y,r:4});
};
</script></body></html>
)rawliteral";

// 15. Cyber Turret 360
const char GAME_TURRET_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Cyber Turret</title>
<style>body{background:#060810;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #00f3ff;border-radius:12px;background:#030408;width:340px;height:420px;max-width:92vw;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="420"></canvas>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let angle=0,bullets=[],enemies=[],score=0,dead=false;
function spawnE(){
const a=Math.random()*Math.PI*2,dist=220;
enemies.push({x:c.width/2+Math.cos(a)*dist,y:c.height/2+Math.sin(a)*dist,r:10,s:1.2});
}
function loop(){
if(!dead){
if(Math.random()<0.04)spawnE();
bullets.forEach((b,bi)=>{b.x+=b.vx;b.y+=b.vy;if(b.x<0||b.x>c.width||b.y<0||b.y>c.height)bullets.splice(bi,1);});
enemies.forEach((e,ei)=>{
const a=Math.atan2(c.height/2-e.y,c.width/2-e.x);
e.x+=Math.cos(a)*e.s; e.y+=Math.sin(a)*e.s;
if(Math.hypot(e.x-c.width/2,e.y-c.height/2)<24)dead=true;
bullets.forEach((b,bi)=>{
if(Math.hypot(e.x-b.x,e.y-b.y)<e.r+4){enemies.splice(ei,1);bullets.splice(bi,1);score+=10;sEl.textContent=score;}
});
});
}
ctx.fillStyle='#030408';ctx.fillRect(0,0,c.width,c.height);
// Turret
ctx.fillStyle='#00f3ff';ctx.beginPath();ctx.arc(c.width/2,c.height/2,16,0,Math.PI*2);ctx.fill();
ctx.strokeStyle='#ffe600';ctx.lineWidth=4;ctx.beginPath();ctx.moveTo(c.width/2,c.height/2);ctx.lineTo(c.width/2+Math.cos(angle)*26,c.height/2+Math.sin(angle)*26);ctx.stroke();
// Bullets
ctx.fillStyle='#00ff66';bullets.forEach(b=>{ctx.fillRect(b.x-2,b.y-2,5,5);});
// Enemies
ctx.fillStyle='#ff0055';enemies.forEach(e=>{ctx.beginPath();ctx.arc(e.x,e.y,e.r,0,Math.PI*2);ctx.fill();});
if(dead){ctx.fillStyle='#ff0055';ctx.font='bold 24px sans-serif';ctx.textAlign='center';ctx.fillText('TOURELLE DÉTRUITE',170,210);}
requestAnimationFrame(loop);
}
loop();
c.onmousemove=c.ontouchmove=e=>{
const rect=c.getBoundingClientRect(),tx=((e.touches?e.touches[0].clientX:e.clientX)-rect.left)*(c.width/rect.width),ty=((e.touches?e.touches[0].clientY:e.clientY)-rect.top)*(c.height/rect.height);
angle=Math.atan2(ty-c.height/2,tx-c.width/2);
};
c.onclick=()=>{
if(dead){dead=false;score=0;sEl.textContent='0';enemies=[];bullets=[];return;}
bullets.push({x:c.width/2,y:c.height/2,vx:Math.cos(angle)*8,vy:Math.sin(angle)*8});
};
</script></body></html>
)rawliteral";

// 16. Galaxian Dive
const char GAME_GALAXIAN_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Galaxian Dive</title>
<style>body{background:#060810;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ffe600;border-radius:12px;background:#030408;width:340px;height:420px;max-width:92vw;}
.ctrl{display:flex;gap:8px;width:100%;max-width:340px;margin-top:8px;}
button{flex:1;height:48px;background:rgba(255,255,255,0.1);border:1px solid #ffe600;color:#fff;border-radius:10px;font-weight:bold;font-size:1.1rem;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="420"></canvas>
<div class="ctrl"><button id="bl">◀ GAUCHE</button><button id="br">DROITE ▶</button><button id="bf" style="background:#ff0055;">🔥 TIR</button></div>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let px=170,bullets=[],aliens=[],score=0,dead=false,timer=0;
function spawnRow(){for(let i=0;i<6;i++)aliens.push({x:30+i*48,y:30,vx:2,diving:false,angle:0});}
spawnRow();
function loop(){
if(!dead){
timer++; if(timer%120===0&&aliens.length){const a=aliens[Math.floor(Math.random()*aliens.length)];a.diving=true;}
bullets.forEach((b,bi)=>{b.y-=7;if(b.y<0)bullets.splice(bi,1);});
aliens.forEach((a,ai)=>{
if(a.diving){a.y+=3.5;a.x+=Math.sin(a.y*0.05)*3;if(Math.hypot(a.x-px,a.y-(c.height-30))<20)dead=true;if(a.y>c.height){a.y=30;a.diving=false;}}
else{a.x+=a.vx;if(a.x<15||a.x>c.width-35)a.vx=-a.vx;}
bullets.forEach((b,bi)=>{if(Math.hypot(a.x-b.x,a.y-b.y)<18){aliens.splice(ai,1);bullets.splice(bi,1);score+=25;sEl.textContent=score;}});
});
if(!aliens.length)spawnRow();
}
ctx.fillStyle='#030408';ctx.fillRect(0,0,c.width,c.height);
ctx.fillStyle='#00f3ff';ctx.fillRect(px-14,c.height-25,28,12);ctx.fillRect(px-4,c.height-32,8,8);
ctx.fillStyle='#00ff66';bullets.forEach(b=>ctx.fillRect(b.x-2,b.y,4,8));
ctx.fillStyle='#ff0055';aliens.forEach(a=>{ctx.beginPath();ctx.arc(a.x,a.y,12,0,Math.PI*2);ctx.fill();});
if(dead){ctx.fillStyle='#ff0055';ctx.font='bold 24px sans-serif';ctx.textAlign='center';ctx.fillText('VAISSEAU DÉTRUIT',170,210);}
requestAnimationFrame(loop);
}
loop();
document.getElementById('bl').onclick=()=>px=Math.max(20,px-25);
document.getElementById('br').onclick=()=>px=Math.min(c.width-20,px+25);
document.getElementById('bf').onclick=()=>{if(dead){dead=false;score=0;aliens=[];spawnRow();return;}bullets.push({x:px,y:c.height-35});};
</script></body></html>
)rawliteral";

// 17. Tank 2D Battle
const char GAME_TANK_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Tank 2D</title>
<style>body{background:#060810;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #00ff66;border-radius:12px;background:#030408;width:340px;height:420px;max-width:92vw;}
.ctrl{display:grid;grid-template-columns:1fr 1fr 1fr;gap:6px;width:100%;max-width:340px;margin-top:8px;}
button{height:46px;background:rgba(255,255,255,0.1);border:1px solid #00ff66;color:#fff;border-radius:8px;font-weight:bold;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="420"></canvas>
<div class="ctrl">
<button onclick="m(0,-1)">▲</button><button onclick="fire()" style="background:#ff0055;">🔥</button><button onclick="m(0,1)">▼</button>
<button onclick="m(-1,0)">◀</button><button onclick="m(0,0)">⏹</button><button onclick="m(1,0)">▶</button>
</div>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let p={x:170,y:360,dx:0,dy:-1},enemy={x:170,y:50,dx:1,dy:0},bullets=[],ebullets=[],score=0,dead=false;
function m(dx,dy){p.dx=dx;p.dy=dy;}
function fire(){bullets.push({x:p.x,y:p.y,dx:p.dx||0,dy:p.dy||-1});}
function loop(){
if(!dead){
p.x=Math.max(15,Math.min(c.width-15,p.x+p.dx*2)); p.y=Math.max(15,Math.min(c.height-15,p.y+p.dy*2));
enemy.x+=enemy.dx*2; if(enemy.x<20||enemy.x>c.width-20)enemy.dx=-enemy.dx;
if(Math.random()<0.03)ebullets.push({x:enemy.x,y:enemy.y,dy:3});
bullets.forEach((b,bi)=>{b.x+=b.dx*5;b.y+=b.dy*5;if(Math.hypot(b.x-enemy.x,b.y-enemy.y)<20){score+=50;sEl.textContent=score;enemy.x=Math.random()*(c.width-40)+20;bullets.splice(bi,1);}});
ebullets.forEach(eb=>{eb.y+=eb.dy;if(Math.hypot(eb.x-p.x,eb.y-p.y)<16)dead=true;});
}
ctx.fillStyle='#030408';ctx.fillRect(0,0,c.width,c.height);
ctx.fillStyle='#00ff66';ctx.fillRect(p.x-10,p.y-10,20,20);
ctx.fillStyle='#ff0055';ctx.fillRect(enemy.x-12,enemy.y-12,24,24);
ctx.fillStyle='#ffe600';bullets.forEach(b=>ctx.fillRect(b.x-2,b.y-2,4,4));
ctx.fillStyle='#ff0055';ebullets.forEach(eb=>ctx.fillRect(eb.x-2,eb.y-2,4,4));
if(dead){ctx.fillStyle='#ff0055';ctx.font='bold 24px sans-serif';ctx.textAlign='center';ctx.fillText('TANK DÉTRUIT !',170,210);}
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// 18. Alien Swarm Strike
const char GAME_SWARM_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Alien Swarm</title>
<style>body{background:#060810;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #9d4edd;border-radius:12px;background:#030408;width:340px;height:420px;max-width:92vw;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="420"></canvas>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let px=170,bugs=[],score=0,dead=false;
function loop(){
if(!dead){
if(Math.random()<0.08)bugs.push({x:Math.random()*(c.width-20)+10,y:-10,s:2+Math.random()*2});
bugs.forEach((b,bi)=>{
b.y+=b.s; if(Math.hypot(b.x-px,b.y-(c.height-30))<20)dead=true;
if(b.y>c.height){bugs.splice(bi,1);score+=5;sEl.textContent=score;}
});
}
ctx.fillStyle='#030408';ctx.fillRect(0,0,c.width,c.height);
ctx.fillStyle='#00f3ff';ctx.fillRect(px-15,c.height-25,30,15);
ctx.fillStyle='#9d4edd';bugs.forEach(b=>{ctx.beginPath();ctx.arc(b.x,b.y,9,0,Math.PI*2);ctx.fill();});
if(dead){ctx.fillStyle='#ff0055';ctx.font='bold 24px sans-serif';ctx.textAlign='center';ctx.fillText('ESSAIM MORTEL !',170,210);ctx.font='14px sans-serif';ctx.fillText('Clique pour rejouer',170,240);}
requestAnimationFrame(loop);
}
loop();
c.ontouchmove=c.onmousemove=e=>{
const rect=c.getBoundingClientRect();px=Math.max(15,Math.min(c.width-15,((e.touches?e.touches[0].clientX:e.clientX)-rect.left)*(c.width/rect.width)));
};
c.onclick=()=>{if(dead){dead=false;score=0;sEl.textContent='0';bugs=[];}};
</script></body></html>
)rawliteral";

#endif
