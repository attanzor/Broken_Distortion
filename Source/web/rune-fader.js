// rune-fader.js — Canvas-based reproduction of gravel::RuneFader
// Single file, zero dependencies. Supports horizontal + vertical orientations,
// 4 modulation arrow types, modifier-key interactions, smooth animation.

const GROUND  = '#11100E';
const VOID    = '#D5D0B8';
const EMBER   = '#EF8B48';
const GREEN   = '#4CAF50';
const PURPLE  = '#AB47BC';

function hexToRgba (hex, alpha)
{
    const r = parseInt (hex.slice (1, 3), 16);
    const g = parseInt (hex.slice (3, 5), 16);
    const b = parseInt (hex.slice (5, 7), 16);
    return `rgba(${r},${g},${b},${alpha})`;
}

function mulSat (hex, s)
{
    const r = parseInt (hex.slice (1, 3), 16);
    const g = parseInt (hex.slice (3, 5), 16);
    const b = parseInt (hex.slice (5, 7), 16);
    const max = Math.max (r, g, b) / 255;
    if (max === 0) return hex;
    const rr = Math.round (r * (1 - s + s / max));
    const gg = Math.round (g * (1 - s + s / max));
    const bb = Math.round (b * (1 - s + s / max));
    return '#' + [rr, gg, bb].map (v => v.toString (16).padStart (2, '0')).join ('');
}

class RuneFader
{
    /** @param {HTMLCanvasElement} canvas
     *  @param {object} opts
     *  opts.label       — uppercase label string
     *  opts.horizontal  — true for horizontal, false for vertical
     *  opts.interaction — () => void, called on any mousedown
     *  opts.onCtrlClick — () => void, called on Ctrl+click
     *  opts.onValue     — (normalized: number) => void, 0..1
     *  opts.onModulation — (amount: number) => void, Cmd-drag on orange arrow
     *  opts.onSlope     — (amount: number) => void, Shift-drag on green arrow
     *  opts.onJitter    — (amount: number) => void, Alt-drag on purple arrow
     *  opts.value       — initial normalized value (0..1), default 0.5
     */
    constructor (canvas, opts = {})
    {
        this.canvas = canvas;
        this.ctx = canvas.getContext ('2d');
        this.horizontal = opts.horizontal !== false;
        this.label = opts.label || '';

        this.interaction = opts.interaction || (() => {});
        this.onCtrlClick  = opts.onCtrlClick  || (() => {});
        this.onValue      = opts.onValue      || (() => {});
        this.onModulation = opts.onModulation || (() => {});
        this.onSlope      = opts.onSlope      || (() => {});
        this.onJitter     = opts.onJitter     || (() => {});

        this.value = opts.value !== undefined ? opts.value : 0.5;
        this.explEnabled = true;

        this.modTarget  = 0.0;
        this.modApplied = 0.0;
        this.slpAmount  = 0.0;
        this.jitAmount  = 0.0;

        this.modSmoothed  = 0.0;
        this.slpSmoothed  = 0.0;
        this.jitSmoothed  = 0.0;
        this.appSmoothed  = 0.0;

        this.dragTarget = 'none'; // 'value' | 'mod' | 'slope' | 'jitter'
        this.isDragging = false;

        this._onMouseDown = this._onMouseDown.bind (this);
        this._onMouseMove = this._onMouseMove.bind (this);
        this._onMouseUp   = this._onMouseUp.bind (this);
        this._onWheel     = this._onWheel.bind (this);

        canvas.addEventListener ('mousedown', this._onMouseDown);
        canvas.addEventListener ('mousemove', this._onMouseMove);
        canvas.addEventListener ('mouseup',   this._onMouseUp);
        canvas.addEventListener ('mouseleave',this._onMouseUp);
        canvas.addEventListener ('wheel',     this._onWheel, { passive: false });
        canvas.addEventListener ('contextmenu', e => e.preventDefault());

        this._tick();
    }

    // --- Setters for external modulation updates ---
    setTargetModulation (v)      { this.modTarget = v; }
    setAppliedModulation (v)     { this.modApplied = v; }
    setSlopeModulation (v)       { this.slpAmount = v; }
    setJitterModulation (v)      { this.jitAmount = v; }
    setExplEnabled (e)           { this.explEnabled = e; }

    // --- Hit testing ---
    _railY () { const r = this.canvas.getBoundingClientRect(); return r.height / 2 + 7; }
    _railX () { const r = this.canvas.getBoundingClientRect(); return r.width / 2; }
    _handleHalf () { return this.horizontal ? 16 : 19; }
    _spread () { return this._handleHalf() * 2 / 3; }

    _handlePos ()
    {
        const r = this.canvas.getBoundingClientRect();
        const hh = this._handleHalf();
        const realRange = r.width - hh * 2;
        return this.horizontal
            ? hh + this.value * realRange
            : hh + this.value * (r.height - hh * 2);
    }

    _setValueFromClient (clientX, clientY)
    {
        const r = this.canvas.getBoundingClientRect();
        const hh = this._handleHalf();
        const v = this.horizontal
            ? (clientX - r.left - hh) / (r.width  - hh * 2)
            : (clientY - r.top  - hh) / (r.height - hh * 2);
        this.value = Math.max (0, Math.min (1, v));
        this.onValue (this.value);
    }

    _setModFromClient (clientX, clientY)
    {
        const r = this.canvas.getBoundingClientRect();
        const range = this.horizontal ? r.height : r.width;
        const raw = (this.horizontal
            ? -(clientY - r.top - r.height / 2) / range
            :  (clientX - r.left - r.width / 2)  / range) * 2;
        this.modTarget = Math.max (-1, Math.min (1, raw));
        this.onModulation (this.modTarget);
    }

    _setSlopeFromClient (clientX, clientY)
    {
        const r = this.canvas.getBoundingClientRect();
        const range = this.horizontal ? r.height : r.width;
        const raw = (this.horizontal
            ? -(clientY - r.top - r.height / 2) / range
            :  (clientX - r.left - r.width / 2)  / range) * 2;
        this.slpAmount = Math.max (-1, Math.min (1, raw));
        this.onSlope (this.slpAmount);
    }

    _setJitterFromClient (clientX, clientY)
    {
        const r = this.canvas.getBoundingClientRect();
        const range = this.horizontal ? r.height : r.width;
        const raw = (this.horizontal
            ? -(clientY - r.top - r.height / 2) / range
            :  (clientX - r.left - r.width / 2)  / range) * 2;
        this.jitAmount = Math.max (-1, Math.min (1, raw));
        this.onJitter (this.jitAmount);
    }

    // --- Mouse events ---
    _onMouseDown (e)
    {
        this.interaction();

        if (e.ctrlKey || e.metaKey)
        {
            this.onCtrlClick();
            return;
        }

        const r = this.canvas.getBoundingClientRect();
        const mx = e.clientX, my = e.clientY;
        const hp = this._handlePos();
        const hh = this._handleHalf();
        const spread = this._spread();

        // Check arrow hit zones first
        const hx = this.horizontal ? hp : r.width / 2;
        const hy = this.horizontal ? this._railY() : hp;
        const tipDist = this.horizontal
            ? my - hy
            : mx - hx;

        const arrowHitTol = spread + 6;
        if (Math.abs (tipDist) < arrowHitTol && Math.abs (tipDist) > 2)
        {
            if (e.shiftKey && Math.abs (this.slpAmount) > 0.001)
            {
                this.dragTarget = 'slope';
                this.isDragging = true;
                this._setSlopeFromClient (mx, my);
                e.preventDefault();
                return;
            }
            if (e.altKey && Math.abs (this.jitAmount) > 0.001)
            {
                this.dragTarget = 'jitter';
                this.isDragging = true;
                this._setJitterFromClient (mx, my);
                e.preventDefault();
                return;
            }
            if ((e.metaKey || e.shiftKey) && Math.abs (this.modTarget) > 0.001)
            {
                this.dragTarget = 'mod';
                this.isDragging = true;
                this._setModFromClient (mx, my);
                e.preventDefault();
                return;
            }
        }

        // Default: drag value
        this.dragTarget = 'value';
        this.isDragging = true;
        if (e.shiftKey && this.dragTarget === 'value')
        {
            // fine-tune
            this._dragOrigin = this.value;
            this._dragClient = this.horizontal ? mx : my;
        }
        else
        {
            this._setValueFromClient (mx, my);
        }
        e.preventDefault();
    }

    _onMouseMove (e)
    {
        if (!this.isDragging) return;
        const mx = e.clientX, my = e.clientY;

        if (this.dragTarget === 'value')
        {
            if (this._dragOrigin !== undefined)
            {
                const r = this.canvas.getBoundingClientRect();
                const range = this.horizontal ? r.width : r.height;
                const delta = this.horizontal
                    ? (mx - this._dragClient) / range
                    : (my - this._dragClient) / range;
                this.value = Math.max (0, Math.min (1, this._dragOrigin + delta * 0.1));
                this.onValue (this.value);
            }
            else
            {
                this._setValueFromClient (mx, my);
            }
        }
        else if (this.dragTarget === 'mod')   this._setModFromClient (mx, my);
        else if (this.dragTarget === 'slope') this._setSlopeFromClient (mx, my);
        else if (this.dragTarget === 'jitter')this._setJitterFromClient (mx, my);
    }

    _onMouseUp ()
    {
        this.isDragging = false;
        this.dragTarget = 'none';
        this._dragOrigin = undefined;
    }

    _onWheel (e)
    {
        if (e.shiftKey)
        {
            e.preventDefault();
            this.value = Math.max (0, Math.min (1, this.value + (e.deltaY < 0 ? 0.001 : -0.001)));
            this.onValue (this.value);
        }
    }

    // --- Animation ---
    _tick ()
    {
        const c = 0.18;
        this.modSmoothed += (this.modTarget - this.modSmoothed) * c;
        this.slpSmoothed += (this.slpAmount - this.slpSmoothed) * c;
        this.jitSmoothed += (this.jitAmount - this.jitSmoothed) * c;
        this.appSmoothed += (this.modApplied - this.appSmoothed) * c;
        this._paint();
        requestAnimationFrame (() => this._tick());
    }

    // --- Paint ---
    _paint ()
    {
        const ctx = this.ctx;
        const w = this.canvas.width, h = this.canvas.height;
        ctx.clearRect (0, 0, w, h);

        const railA = this.explEnabled ? (this.horizontal ? 0.80 : 0.54) : 0.30;
        const railC = hexToRgba (VOID, railA);
        const hndC  = this.explEnabled ? EMBER : mulSat (EMBER, 0.3);
        const hh = this._handleHalf();
        const hp = this._handlePos();

        ctx.textBaseline = 'top';
        ctx.textAlign = 'center';
        ctx.fillStyle = hexToRgba (VOID, 0.76);
        ctx.font = this.horizontal ? '13px sans-serif' : '11px sans-serif';
        ctx.fillText (this.label.toUpperCase(),
                       w / 2, this.horizontal ? 0 : h - 16);

        if (this.horizontal)
        {
            const ry = h / 2 + 7;
            // Rail
            ctx.strokeStyle = railC;
            ctx.lineWidth = 2;
            ctx.beginPath();
            ctx.moveTo (18, ry);
            ctx.lineTo (w - 18, ry);
            ctx.stroke();
            // End caps
            ctx.beginPath();
            ctx.moveTo (18, ry - hh);
            ctx.lineTo (18, ry + hh);
            ctx.moveTo (w - 18, ry - hh);
            ctx.lineTo (w - 18, ry + hh);
            ctx.stroke();
            // Handle
            ctx.strokeStyle = hndC;
            ctx.beginPath();
            ctx.moveTo (hp, ry - hh);
            ctx.lineTo (hp, ry + hh);
            ctx.stroke();
            ctx.fillStyle = hndC;
            ctx.beginPath();
            ctx.ellipse (hp, ry, 4, 4, 0, 0, Math.PI * 2);
            ctx.fill();
            // Arrows
            this._drawArrows (hp, ry, true);
        }
        else
        {
            const rx = w / 2;
            const ry1 = 30;
            const ry2 = h - 16;
            // Rail
            ctx.strokeStyle = railC;
            ctx.lineWidth = 1.4;
            ctx.beginPath();
            ctx.moveTo (rx, ry1);
            ctx.lineTo (rx, ry2);
            ctx.stroke();
            // End caps
            ctx.beginPath();
            ctx.moveTo (rx - hh, ry1);
            ctx.lineTo (rx + hh, ry1);
            ctx.moveTo (rx - hh, ry2);
            ctx.lineTo (rx + hh, ry2);
            ctx.stroke();
            // Handle
            ctx.strokeStyle = hexToRgba (VOID, this.explEnabled ? 1.0 : 0.3);
            ctx.beginPath();
            ctx.moveTo (rx - hh, hp);
            ctx.lineTo (rx + hh, hp);
            ctx.stroke();
            ctx.fillStyle = hexToRgba (VOID, this.explEnabled ? 1.0 : 0.3);
            ctx.beginPath();
            ctx.ellipse (rx, hp, 4, 4, 0, 0, Math.PI * 2);
            ctx.fill();
            // Arrows
            this._drawArrows (rx, hp, false);
        }
    }

    _drawArrow (ctx, cx, cy, value, colour, horizontal)
    {
        if (Math.abs (value) < 0.001) return;
        const spread = this._spread();
        const alpha = 0.25 + Math.abs (value) * 0.40;
        const len = spread * 1.8;
        const dir = value > 0 ? -1 : 1;

        ctx.fillStyle = hexToRgba (colour, alpha);
        ctx.beginPath();
        if (horizontal)
        {
            const tipY = cy + dir * len;
            ctx.moveTo (cx, tipY);
            ctx.lineTo (cx - spread, cy + dir * 4);
            ctx.lineTo (cx + spread, cy + dir * 4);
        }
        else
        {
            const tipX = cx + dir * len;
            ctx.moveTo (tipX, cy);
            ctx.lineTo (cx + dir * 4, cy - spread);
            ctx.lineTo (cx + dir * 4, cy + spread);
        }
        ctx.closePath();
        ctx.fill();
    }

    _drawArrows (cx, cy, isH)
    {
        this._drawArrow (this.ctx, cx, cy, this.modSmoothed, EMBER, isH);
        this._drawArrow (this.ctx, cx, cy, this.appSmoothed, VOID, isH);
        this._drawArrow (this.ctx, cx, cy, this.slpSmoothed, GREEN, isH);
        this._drawArrow (this.ctx, cx, cy, this.jitSmoothed, PURPLE, isH);
    }

    // --- Utilities ---
    setCanvasSize (w, h)
    {
        this.canvas.width = w;
        this.canvas.height = h;
    }

    destroy ()
    {
        this.canvas.removeEventListener ('mousedown', this._onMouseDown);
        this.canvas.removeEventListener ('mousemove', this._onMouseMove);
        this.canvas.removeEventListener ('mouseup', this._onMouseUp);
        this.canvas.removeEventListener ('mouseleave', this._onMouseUp);
        this.canvas.removeEventListener ('wheel', this._onWheel);
        this.canvas.removeEventListener ('contextmenu', () => {});
    }
}
