// rune-fader.js — Canvas reproduction of gravel::RuneFader
// Arrows anchored at rail endpoints. Modifier keys switch drag mode globally.

const GROUND = '#11100E';
const VOID   = '#D5D0B8';
const EMBER  = '#EF8B48';
const GREEN  = '#4CAF50';
const PURPLE = '#AB47BC';

function rgba (hex, a)
{
    const r = parseInt (hex.slice (1, 3), 16);
    const g = parseInt (hex.slice (3, 5), 16);
    const b = parseInt (hex.slice (5, 7), 16);
    return `rgba(${r},${g},${b},${a})`;
}

function mulSat (hex, s)
{
    const r = parseInt (hex.slice (1, 3), 16), g = parseInt (hex.slice (3, 5), 16), b = parseInt (hex.slice (5, 7), 16);
    const max = Math.max (r, g, b) / 255;
    if (max === 0) return hex;
    const rr = Math.round (r * (1 - s + s / max)), gg = Math.round (g * (1 - s + s / max)), bb = Math.round (b * (1 - s + s / max));
    return '#' + [rr, gg, bb].map (v => v.toString (16).padStart (2, '0')).join ('');
}

class RuneFader
{
    /** @param {HTMLCanvasElement} canvas
     *  opts.label, opts.horizontal, opts.value (0..1)
     *  opts.onValue(norm), opts.onModulation(amount), opts.onSlope(amount), opts.onJitter(amount)
     *  opts.onCtrlClick() */
    constructor (canvas, opts = {})
    {
        this.c = canvas;
        this.ctx = canvas.getContext ('2d');
        this.h = opts.horizontal !== false;
        this.label = opts.label || '';
        this.value = opts.value !== undefined ? opts.value : 0.5;
        this.explEnabled = true;

        this._onValue      = opts.onValue      || (() => {});
        this._onModulation = opts.onModulation || (() => {});
        this._onSlope      = opts.onSlope      || (() => {});
        this._onJitter     = opts.onJitter     || (() => {});
        this._onCtrlClick  = opts.onCtrlClick  || (() => {});
        this._onInteraction= opts.onInteraction|| (() => {});

        // Modulation targets
        this.modTarget  = 0;   this.modSmoothed  = 0;
        this.slpTarget  = 0;   this.slpSmoothed  = 0;
        this.jitTarget  = 0;   this.jitSmoothed  = 0;
        this.appTarget  = 0;   this.appSmoothed  = 0;

        // Drag state: 'none' | 'value' | 'mod' | 'slope' | 'jitter'
        this._drag = 'none';
        this._dragStart = 0;
        this._dragOrigin = 0;

        canvas.addEventListener ('mousedown',  this._down = this._down.bind (this));
        canvas.addEventListener ('mousemove',  this._move = this._move.bind (this));
        canvas.addEventListener ('mouseup',    this._up   = this._up.bind (this));
        canvas.addEventListener ('mouseleave', this._up);
        canvas.addEventListener ('contextmenu', e => e.preventDefault());
        canvas.addEventListener ('wheel', this._wheel = this._wheel.bind (this), { passive: false });
        canvas.style.cursor = this.h ? 'ew-resize' : 'ns-resize';

        this._tick();
    }

    // --- Setters ---
    setTargetModulation (v)  { this.modTarget = Math.max (-1, Math.min (1, v)); }
    setAppliedModulation (v) { this.appTarget = Math.max (-1, Math.min (1, v)); }
    setSlopeModulation (v)   { this.slpTarget = Math.max (-1, Math.min (1, v)); }
    setJitterModulation (v)  { this.jitTarget = Math.max (-1, Math.min (1, v)); }
    setExplEnabled (e)       { this.explEnabled = e; }

    // --- Metrics ---
    _rail ()
    {
        const w = this.c.width, hh = this.c.height;
        if (this.h)
        {
            const h = hh / 2 + 7;
            return { left: 18, right: w - 18, y: h, hh: 16 };
        }
        return { top: 30, bot: hh - 16, x: w / 2, hh: 19 };
    }

    _handlePos ()
    {
        const r = this._rail();
        if (this.h) return r.left + this.value * (r.right - r.left);
        return r.bot - this.value * (r.bot - r.top);
    }

    _valFromMouse (mx, my)
    {
        const r = this._rail();
        const v = this.h
            ? (mx - r.left) / (r.right - r.left)
            : 1 - (my - r.top) / (r.bot - r.top);
        return Math.max (0, Math.min (1, v));
    }

    // --- Mouse: modifier keys switch drag mode (no arrow hit-testing) ---
    // Priority: Ctrl > Shift > Alt > Cmd > plain value drag
    _down (e)
    {
        this._onInteraction();
        if (e.ctrlKey) { this._onCtrlClick(); return; }

        if (e.shiftKey)               { this._drag = 'slope';  this._dragOrigin = this.slpTarget; }
        else if (e.altKey)            { this._drag = 'jitter'; this._dragOrigin = this.jitTarget; }
        else if (e.metaKey)           { this._drag = 'mod';    this._dragOrigin = this.modTarget; }
        else
        {
            const v = this._valFromMouse (e.offsetX, e.offsetY);
            this.value = v;
            this._onValue (v);
            this._drag = 'value';
        }
        this._dragStart = this.h ? e.clientX : e.clientY;
        if (this._drag !== 'value') this._applyDrag (e);
        e.preventDefault();
    }

    _applyDrag (e)
    {
        const rangeRaw = this.h ? (this.c.width - 36) : (this.c.height - 46);
        const range = rangeRaw / 2; // kCmdSensitivity = 2.0
        const delta = this.h
            ? (e.clientX - this._dragStart) / range
            : -(e.clientY - this._dragStart) / range;

        const mods = ['slope', 'jitter', 'mod'];
        const idx = mods.indexOf (this._drag);
        if (idx >= 0)
        {
            const getter = [()=>this.slpTarget, ()=>this.jitTarget, ()=>this.modTarget][idx];
            const setter = [(v)=>{ this.slpTarget=v; this._onSlope(v); },
                            (v)=>{ this.jitTarget=v; this._onJitter(v); },
                            (v)=>{ this.modTarget=v; this._onModulation(v); }][idx];
            const val = Math.max (-1, Math.min (1, this._dragOrigin + delta));
            setter (val);
        }
        else
        {
            this.value = this._valFromMouse (e.offsetX, e.offsetY);
            this._onValue (this.value);
        }
    }

    _move (e)
    {
        if (this._drag === 'none') return;
        this._applyDrag (e);
    }

    _up () { this._drag = 'none'; }

    _wheel (e)
    {
        if (e.shiftKey)
        {
            e.preventDefault();
            const step = e.deltaY < 0 ? 0.001 : -0.001;
            this.value = Math.max (0, Math.min (1, this.value + step));
            this._onValue (this.value);
        }
    }

    // --- Animation ---
    _tick ()
    {
        const c = 0.18;
        this.modSmoothed += (this.modTarget - this.modSmoothed) * c;
        this.slpSmoothed += (this.slpTarget - this.slpSmoothed) * c;
        this.jitSmoothed += (this.jitTarget - this.jitSmoothed) * c;
        this.appSmoothed += (this.appTarget - this.appSmoothed) * c;
        this._paint();
        requestAnimationFrame (() => this._tick());
    }

    // --- Paint ---
    _paint ()
    {
        const ctx = this.ctx;
        const w = this.c.width, hh = this.c.height;
        ctx.clearRect (0, 0, w, hh);

        const railA = this.explEnabled ? (this.h ? 0.80 : 0.54) : 0.30;
        const railC = rgba (VOID, railA);
        const hndC  = this.explEnabled ? EMBER : mulSat (EMBER, 0.3);
        const r = this._rail();
        const hp  = this._handlePos();
        const lw = this.h ? 2 : 1.4;
        const rhh = r.hh;
        const spread = this.h ? 10.667 : rhh * 2 / 3;

        // Label
        ctx.textBaseline = 'top';
        ctx.textAlign = 'center';
        ctx.fillStyle = rgba (VOID, 0.76);
        ctx.font = (this.h ? '13px' : '11px') + ' sans-serif';
        ctx.fillText (this.label.toUpperCase(), w / 2, this.h ? 0 : hh - 16);

        if (this.h)
        {
            // Rail
            ctx.strokeStyle = railC;
            ctx.lineWidth = lw;
            ctx.beginPath();
            ctx.moveTo (r.left, r.y); ctx.lineTo (r.right, r.y);
            ctx.stroke();
            // End caps
            ctx.beginPath();
            ctx.moveTo (r.left, r.y - rhh); ctx.lineTo (r.left, r.y + rhh);
            ctx.moveTo (r.right, r.y - rhh); ctx.lineTo (r.right, r.y + rhh);
            ctx.stroke();
            // Handle
            ctx.strokeStyle = hndC;
            ctx.beginPath();
            ctx.moveTo (hp, r.y - rhh); ctx.lineTo (hp, r.y + rhh);
            ctx.stroke();
            ctx.fillStyle = hndC;
            ctx.beginPath();
            ctx.ellipse (hp, r.y, lw, lw, 0, 0, Math.PI * 2); ctx.fill();
            // Arrows — anchored at rail endpoints
            this._drawHArrow (ctx, r.left, r.right, r.y, spread, this.modSmoothed, EMBER, 1.8);
            this._drawHArrow (ctx, r.left, r.right, r.y, spread, this.appSmoothed, VOID, 1.2);
            this._drawHArrow (ctx, r.left, r.right, r.y, spread, this.slpSmoothed, GREEN, 1.8);
            this._drawHArrow (ctx, r.left, r.right, r.y, spread, this.jitSmoothed, PURPLE, 1.8);
        }
        else
        {
            // Rail
            ctx.strokeStyle = railC;
            ctx.lineWidth = lw;
            ctx.beginPath();
            ctx.moveTo (r.x, r.top); ctx.lineTo (r.x, r.bot);
            ctx.stroke();
            // End caps
            ctx.beginPath();
            ctx.moveTo (r.x - rhh, r.top); ctx.lineTo (r.x + rhh, r.top);
            ctx.moveTo (r.x - rhh, r.bot); ctx.lineTo (r.x + rhh, r.bot);
            ctx.stroke();
            // Handle
            const vhndC = this.explEnabled ? rgba (VOID, 1.0) : rgba (VOID, 0.3);
            ctx.strokeStyle = vhndC;
            ctx.beginPath();
            ctx.moveTo (r.x - rhh, hp); ctx.lineTo (r.x + rhh, hp);
            ctx.stroke();
            ctx.fillStyle = vhndC;
            ctx.beginPath();
            ctx.ellipse (r.x, hp, lw, lw, 0, 0, Math.PI * 2); ctx.fill();
            // Arrows — anchored at rail endpoints
            this._drawVArrow (ctx, r.x, r.top, r.bot, spread, this.modSmoothed, EMBER, 1.8);
            this._drawVArrow (ctx, r.x, r.top, r.bot, spread, this.appSmoothed, VOID, 1.2);
            this._drawVArrow (ctx, r.x, r.top, r.bot, spread, this.slpSmoothed, GREEN, 1.8);
            this._drawVArrow (ctx, r.x, r.top, r.bot, spread, this.jitSmoothed, PURPLE, 1.8);
        }
    }

    /** Horizontal arrow: base at rail endpoint, tip extends along rail */
    _drawHArrow (ctx, rLeft, rRight, y, spread, value, colour, strokeW)
    {
        if (Math.abs (value) <= 0.001) return;
        const alpha = 0.25 + Math.abs (value) * 0.40;
        ctx.strokeStyle = rgba (colour, alpha);
        ctx.lineWidth = strokeW;
        ctx.lineJoin = 'miter';
        ctx.beginPath();
        if (value > 0)
        {
            const tipX = rLeft + value * (rRight - rLeft);
            ctx.moveTo (rLeft, y - spread);
            ctx.lineTo (tipX, y);
            ctx.lineTo (rLeft, y + spread);
        }
        else
        {
            const tipX = rRight + value * (rRight - rLeft);
            ctx.moveTo (rRight, y - spread);
            ctx.lineTo (tipX, y);
            ctx.lineTo (rRight, y + spread);
        }
        ctx.closePath();
        ctx.stroke();
    }

    /** Vertical arrow: base at rail endpoint, tip extends along rail */
    _drawVArrow (ctx, x, rTop, rBot, spread, value, colour, strokeW)
    {
        if (Math.abs (value) <= 0.001) return;
        const alpha = 0.25 + Math.abs (value) * 0.40;
        ctx.strokeStyle = rgba (colour, alpha);
        ctx.lineWidth = strokeW;
        ctx.beginPath();
        if (value < 0)
        {
            const tipY = rTop + Math.abs (value) * (rBot - rTop);
            ctx.moveTo (x - spread, rTop);
            ctx.lineTo (x, tipY);
            ctx.lineTo (x + spread, rTop);
        }
        else
        {
            const tipY = rBot - value * (rBot - rTop);
            ctx.moveTo (x - spread, rBot);
            ctx.lineTo (x, tipY);
            ctx.lineTo (x + spread, rBot);
        }
        ctx.closePath();
        ctx.stroke();
    }

    destroy ()
    {
        this.c.removeEventListener ('mousedown',  this._down);
        this.c.removeEventListener ('mousemove',  this._move);
        this.c.removeEventListener ('mouseup',    this._up);
        this.c.removeEventListener ('mouseleave', this._up);
        this.c.removeEventListener ('wheel',      this._wheel);
        this.c.removeEventListener ('contextmenu', () => {});
    }
}
