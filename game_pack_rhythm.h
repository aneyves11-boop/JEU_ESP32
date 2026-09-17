#ifndef GAME_PACK_RHYTHM_H
#define GAME_PACK_RHYTHM_H

#include <pgmspace.h>

// 37. Beat Tap Reflex
const char GAME_BEATTAP_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Beat Tap</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #00f3ff;border-radius:12px;background:#05070f;width:340px;height:400px;max-width:92vw;}
button{width:340px;height:50px;background:linear-gradient(135deg,#00f3ff,#ff0055);color:#fff;border:none;border-radius:12px;font-weight:bold;margin-top:8px;font-size:1.1rem;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<button onclick="tap()">TAP EN RYTHME 🥁</button>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let notes=[],score=0,timer=0;
function tap(){
let hit=false;
notes.forEach((n,i)=>{if(Math.abs(n.y-340)<30){score+=10;sEl.textContent=score;notes.splice(i,1);hit=true;}});
if(!hit)score=Math.max(0,score-5);
}
function loop(){
timer++;if(timer%50===0)notes.push({y:0,x:170});
notes.forEach((n,i)=>{n.y+=4;if(n.y>400)notes.splice(i,1);});
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
// Hit line
ctx.fillStyle='#ff0055';ctx.fillRect(0,340,c.width,4);
// Notes
ctx.fillStyle='#00f3ff';notes.forEach(n=>{ctx.beginPath();ctx.arc(n.x,n.y,16,0,Math.PI*2);ctx.fill();});
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// 38. Simon Musical 4-Notes
const char GAME_SIMON_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Simon</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:320px;display:flex;justify-content:space-between;margin-bottom:12px;font-weight:bold;}
.grid{display:grid;grid-template-columns:1fr 1fr;gap:12px;width:320px;}
.pad{aspect-ratio:1;border-radius:16px;border:none;cursor:pointer;opacity:0.4;transition:opacity 0.15s;}
.pad.lit{opacity:1;box-shadow:0 0 25px currentColor;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Niveau: <span id="s">1</span></span></header>
<div class="grid">
<div class="pad" id="p0" style="background:#00ff66;color:#00ff66;" onclick="tap(0)"></div>
<div class="pad" id="p1" style="background:#ff0055;color:#ff0055;" onclick="tap(1)"></div>
<div class="pad" id="p2" style="background:#ffe600;color:#ffe600;" onclick="tap(2)"></div>
<div class="pad" id="p3" style="background:#00f3ff;color:#00f3ff;" onclick="tap(3)"></div>
</div>
<script>
let seq=[],user=[],step=0;
function nextRound(){
seq.push(Math.floor(Math.random()*4)); user=[]; document.getElementById('s').textContent=seq.length;
playSeq(0);
}
function playSeq(idx){
if(idx>=seq.length)return;
const p=document.getElementById('p'+seq[idx]);
p.classList.add('lit');
setTimeout(()=>{p.classList.remove('lit');setTimeout(()=>playSeq(idx+1),250);},400);
}
function tap(id){
const p=document.getElementById('p'+id);
p.classList.add('lit');setTimeout(()=>p.classList.remove('lit'),150);
if(id===seq[user.length]){
user.push(id);
if(user.length===seq.length)setTimeout(nextRound,800);
} else {alert('PERDU ! Score: '+(seq.length-1));seq=[];nextRound();}
}
nextRound();
</script></body></html>
)rawliteral";

// 39. Note Drop Rain
const char GAME_NOTEDROP_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Note Drop</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:340px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ffe600;border-radius:12px;background:#060812;width:340px;height:400px;max-width:92vw;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Notes: <span id="s">0</span></span></header>
<canvas id="c" width="340" height="400"></canvas>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let px=170,notes=[],score=0;
function loop(){
if(Math.random()<0.05)notes.push({x:Math.random()*(c.width-20)+10,y:0,s:3});
notes.forEach((n,i)=>{
n.y+=n.s;
if(Math.hypot(n.x-px,n.y-370)<25){score++;sEl.textContent=score;notes.splice(i,1);}
if(n.y>400)notes.splice(i,1);
});
ctx.fillStyle='#060812';ctx.fillRect(0,0,c.width,c.height);
ctx.fillStyle='#00f3ff';ctx.fillRect(px-25,370,50,14);
ctx.fillStyle='#ffe600';notes.forEach(n=>{ctx.beginPath();ctx.arc(n.x,n.y,10,0,Math.PI*2);ctx.fill();});
requestAnimationFrame(loop);
}
loop();
c.ontouchmove=c.onmousemove=e=>{
const rect=c.getBoundingClientRect();px=((e.touches?e.touches[0].clientX:e.clientX)-rect.left)*(c.width/rect.width);
};
</script></body></html>
)rawliteral";

// 40. Tempo Tapper 60 BPM
const char GAME_TEMPO_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Tempo Tapper</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:320px;display:flex;justify-content:space-between;margin-bottom:14px;font-weight:bold;}
.circle{width:180px;height:180px;border-radius:50%;background:rgba(0,243,255,0.1);border:4px solid #00f3ff;display:flex;align-items:center;justify-content:center;font-size:2rem;font-weight:bold;margin:30px 0;cursor:pointer;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>BPM: <span id="s">--</span></span></header>
<div style="color:#8b9bb4;font-size:0.9rem;">Tape en rythme régulier à 60 BPM :</div>
<div class="circle" onclick="tap()">TAP 🎵</div>
<script>
let taps=[],sEl=document.getElementById('s');
function tap(){
let now=Date.now();taps.push(now);if(taps.length>4)taps.shift();
if(taps.length>=2){
let diffs=[];for(let i=1;i<taps.length;i++)diffs.push(taps[i]-taps[i-1]);
let avg=diffs.reduce((a,b)=>a+b,0)/diffs.length;
let bpm=Math.round(60000/avg);
sEl.textContent=bpm;
}}
</script></body></html>
)rawliteral";

// 41. Guitar Hero Mini
const char GAME_GUITAR_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Guitar Mini</title>
<style>body{background:#090b12;color:#f0f4fc;font-family:sans-serif;margin:0;display:flex;flex-direction:column;align-items:center;padding:10px;}
header{width:100%;max-width:330px;display:flex;justify-content:space-between;margin-bottom:8px;font-weight:bold;}
canvas{border:2px solid #ff0055;border-radius:12px;background:#05070f;width:330px;height:380px;max-width:92vw;}
.btns{display:flex;gap:8px;width:330px;margin-top:8px;}
button{flex:1;height:50px;border-radius:10px;border:none;font-weight:bold;font-size:1.2rem;cursor:pointer;}
</style></head><body>
<header><a href="/hub" style="color:#00f3ff;text-decoration:none;">◀ Menu</a><span>Score: <span id="s">0</span></span></header>
<canvas id="c" width="330" height="380"></canvas>
<div class="btns">
<button style="background:#00ff66;" onclick="fret(0)">1</button>
<button style="background:#ff0055;" onclick="fret(1)">2</button>
<button style="background:#00f3ff;" onclick="fret(2)">3</button>
</div>
<script>
const c=document.getElementById('c'),ctx=c.getContext('2d'),sEl=document.getElementById('s');
let notes=[],score=0,timer=0;
const cols=['#00ff66','#ff0055','#00f3ff'];
function fret(lane){
notes.forEach((n,i)=>{if(n.lane===lane&&Math.abs(n.y-330)<35){score+=20;sEl.textContent=score;notes.splice(i,1);}});
}
function loop(){
timer++;if(timer%40===0)notes.push({lane:Math.floor(Math.random()*3),y:0});
notes.forEach((n,i)=>{n.y+=4.5;if(n.y>380)notes.splice(i,1);});
ctx.fillStyle='#05070f';ctx.fillRect(0,0,c.width,c.height);
ctx.fillStyle='#ff0055';ctx.fillRect(0,330,c.width,3);
notes.forEach(n=>{ctx.fillStyle=cols[n.lane];ctx.fillRect(n.lane*110+25,n.y,60,16);});
requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

#endif
