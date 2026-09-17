// Moteur audio 8-bit synthétisé en temps réel (Web Audio API)
// Ne nécessite aucun fichier MP3/WAV externe pour économiser la mémoire Flash de l'ESP32

class SoundFX {
    constructor() {
        this.ctx = null;
        this.enabled = true;
    }

    init() {
        if (!this.ctx) {
            const AudioContext = window.AudioContext || window.webkitAudioContext;
            if (AudioContext) {
                this.ctx = new AudioContext();
            }
        }
        if (this.ctx && this.ctx.state === 'suspended') {
            this.ctx.resume();
        }
    }

    playTone(freq, type = 'sine', duration = 0.1, startVol = 0.15, endVol = 0) {
        if (!this.enabled) return;
        this.init();
        if (!this.ctx) return;

        try {
            const osc = this.ctx.createOscillator();
            const gain = this.ctx.createGain();

            osc.type = type;
            osc.frequency.setValueAtTime(freq, this.ctx.currentTime);

            gain.gain.setValueAtTime(startVol, this.ctx.currentTime);
            gain.gain.exponentialRampToValueAtTime(Math.max(endVol, 0.0001), this.ctx.currentTime + duration);

            osc.connect(gain);
            gain.connect(this.ctx.destination);

            osc.start();
            osc.stop(this.ctx.currentTime + duration);
        } catch (e) {
            console.warn("Audio error:", e);
        }
    }

    click() {
        this.playTone(600, 'triangle', 0.04, 0.1);
    }

    score() {
        if (!this.enabled) return;
        this.init();
        if (!this.ctx) return;
        const now = this.ctx.currentTime;
        this.playToneCustom(587.33, 'square', 0.08, now);     // D5
        this.playToneCustom(880, 'square', 0.15, now + 0.08); // A5
    }

    jump() {
        if (!this.enabled) return;
        this.init();
        if (!this.ctx) return;
        try {
            const osc = this.ctx.createOscillator();
            const gain = this.ctx.createGain();
            osc.type = 'square';
            const now = this.ctx.currentTime;
            osc.frequency.setValueAtTime(150, now);
            osc.frequency.exponentialRampToValueAtTime(450, now + 0.12);
            gain.gain.setValueAtTime(0.12, now);
            gain.gain.exponentialRampToValueAtTime(0.001, now + 0.12);
            osc.connect(gain);
            gain.connect(this.ctx.destination);
            osc.start();
            osc.stop(now + 0.12);
        } catch (e) {}
    }

    hit() {
        if (!this.enabled) return;
        this.init();
        if (!this.ctx) return;
        try {
            const osc = this.ctx.createOscillator();
            const gain = this.ctx.createGain();
            osc.type = 'sawtooth';
            const now = this.ctx.currentTime;
            osc.frequency.setValueAtTime(220, now);
            osc.frequency.exponentialRampToValueAtTime(50, now + 0.15);
            gain.gain.setValueAtTime(0.2, now);
            gain.gain.exponentialRampToValueAtTime(0.001, now + 0.15);
            osc.connect(gain);
            gain.connect(this.ctx.destination);
            osc.start();
            osc.stop(now + 0.15);
        } catch (e) {}
    }

    gameover() {
        if (!this.enabled) return;
        this.init();
        if (!this.ctx) return;
        const now = this.ctx.currentTime;
        this.playToneCustom(300, 'sawtooth', 0.15, now);
        this.playToneCustom(250, 'sawtooth', 0.15, now + 0.15);
        this.playToneCustom(200, 'sawtooth', 0.15, now + 0.3);
        this.playToneCustom(140, 'sawtooth', 0.35, now + 0.45);
    }

    win() {
        if (!this.enabled) return;
        this.init();
        if (!this.ctx) return;
        const now = this.ctx.currentTime;
        this.playToneCustom(523.25, 'triangle', 0.1, now);        // C5
        this.playToneCustom(659.25, 'triangle', 0.1, now + 0.1);  // E5
        this.playToneCustom(783.99, 'triangle', 0.1, now + 0.2);  // G5
        this.playToneCustom(1046.50, 'triangle', 0.25, now + 0.3); // C6
    }

    playToneCustom(freq, type, duration, startTime) {
        try {
            const osc = this.ctx.createOscillator();
            const gain = this.ctx.createGain();
            osc.type = type;
            osc.frequency.setValueAtTime(freq, startTime);
            gain.gain.setValueAtTime(0.15, startTime);
            gain.gain.exponentialRampToValueAtTime(0.001, startTime + duration);
            osc.connect(gain);
            gain.connect(this.ctx.destination);
            osc.start(startTime);
            osc.stop(startTime + duration);
        } catch (e) {}
    }
}

const sfx = new SoundFX();
document.addEventListener('click', () => sfx.init(), { once: true });
document.addEventListener('touchstart', () => sfx.init(), { once: true });
