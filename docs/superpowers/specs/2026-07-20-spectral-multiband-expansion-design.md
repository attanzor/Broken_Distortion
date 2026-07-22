# Spectral Multiband Distortion — Design Doc

## Signal Chain

```
Input
  → Pre-EQ (low-shelf → bell → high-shelf)
  → Oversample ↑4x (2×2, half-band polyphase IIR)
    → Linkwitz-Riley Crossover → 3 bands (low/mid/high)
      → Each band: FFT per-bin amplitude analysis
        → 3 gain-layer wavetables blended by per-bin amplitude
        → 9 cells = 3 freq bands × 3 gain layers, each independent
      → Sum bands
    → Post-EQ (linked mirror of pre-EQ, depth 0–1)
  → Oversample ↓4x
  → Dry/Wet mix
  → Output gain
```

Signal flow for a single sample:
1. Pre-emphasis EQ shapes the spectrum
2. Crossover splits into 3 frequency bands (LR4 filters, sum to unity)
3. For each band, FFT analysis determines per-bin amplitude
4. Per bin, the amplitude relative to 2 threshold lines blends between
   3 gain-layer wavetables — each of the 9 (3×3) combinations has an
   independent wavetable with its own shape morph
5. Bands are summed back
6. Post-EQ restores spectral balance (linked mirror of pre-EQ)
7. Dry/wet mix and output level

Latency: oversampler (fixed) + FFT frame (~5.3ms at 48kHz) compensated via
`setLatencySamples()`.

## 1. Pre / Post EQ

Three stages in series, each with three parameters:

| Parameter | ID | Range | Mapping |
|---|---|---|---|
| Low Shelf freq | `pre_lowshelf_freq` | 20–20000 | logarithmic |
| Low Shelf Q | `pre_lowshelf_q` | 0.1–10 | linear |
| Low Shelf gain | `pre_lowshelf_gain` | -24–+24 dB | linear |
| Bell freq | `pre_bell_freq` | 20–20000 | logarithmic |
| Bell Q | `pre_bell_q` | 0.1–10 | linear |
| Bell gain | `pre_bell_gain` | -24–+24 dB | linear |
| High Shelf freq | `pre_highshelf_freq` | 20–20000 | logarithmic |
| High Shelf Q | `pre_highshelf_q` | 0.1–10 | linear |
| High Shelf gain | `pre_highshelf_gain` | -24–+24 dB | linear |

Post-EQ mirrors the same 9 parameters with inverted gain. A single control
determines how much of the mirror is applied:

| Parameter | ID | Range |
|---|---|---|
| Post-EQ depth | `post_eq_depth` | 0–1 |

At depth = 0 the post-EQ is flat (no compensation). At depth = 1 the mirror
is exact.

## 2. Crossover & Per-Band Level

Two crossover frequencies split the spectrum into three bands:

| Parameter | ID | Range |
|---|---|---|
| Crossover Low→Mid | `cross_freq_low` | 20–20000 Hz |
| Crossover Mid→High | `cross_freq_mid` | 20–20000 Hz |

Implemented with 4th-order Linkwitz-Riley filters (24 dB/oct). The bands
sum to unity gain. Crossover frequencies cannot cross — dragging one pushes
the adjacent.

Per-band output level controls:

| Parameter | ID | Range |
|---|---|---|
| Low band level | `band_low_level` | 0–1 |
| Mid band level | `band_mid_level` | 0–1 |
| High band level | `band_high_level` | 0–1 |

## 3. FFT & 9-Cell Wavetable Processing

After the crossover, each frequency band enters independent FFT-based
gain-layer processing.

**FFT analysis**: 1024-sample window, 75% overlap (hop = 256 samples).
Hann window. At 48 kHz input (192 kHz oversampled), this is ~5.3 ms
latency. Each band gets its own FFT analysis.

**9 independent wavetables** — one per cell in the 3×3 grid:

| Cell | ID | Purpose |
|---|---|---|
| Low / Quiet | `cell_lq_shape` | Low freq, quiet amplitude |
| Mid / Quiet | `cell_mq_shape` | Mid freq, quiet amplitude |
| High / Quiet | `cell_hq_shape` | High freq, quiet amplitude |
| Low / Moderate | `cell_lm_shape` | Low freq, moderate amplitude |
| Mid / Moderate | `cell_mm_shape` | Mid freq, moderate amplitude |
| High / Moderate | `cell_hm_shape` | High freq, moderate amplitude |
| Low / Loud | `cell_ll_shape` | Low freq, loud amplitude |
| Mid / Loud | `cell_ml_shape` | Mid freq, loud amplitude |
| High / Loud | `cell_hl_shape` | High freq, loud amplitude |

Each shape morph is 0–1, interpolating between 8 seed-defined base curves
(the existing 8-curve system, shared globally).

**Per-bin amplitude blending**: Within each frequency band, each FFT bin's
magnitude is compared to two threshold lines (the horizontal lines on the
grid). Based on where the magnitude falls:
- Below lower threshold → 100% quiet-layer wavetable for this band
- Between thresholds → crossfade quiet↔moderate (6 dB overlap zone)
- Above upper threshold → crossfade moderate↔loud (6 dB overlap zone)

The two threshold lines map directly to the horizontal grid lines and are
stored as 0–1 normalized values.

**Overlap-add**: The processed FFT frames are overlap-added back into the
time-domain signal per band, then all 3 bands are summed.

## 4. Retained Parameters

| Parameter | ID | Range | Notes |
|---|---|---|---|
| Seed | `seed` | 0–999999 | Still defines 8 base wavetable curves |
| Possibility | `possibility` | 0–1 | Navigates the noise field |
| Mix | `mix` | 0–1 | Dry/wet after distortion |
| Output | `output` | 0–1 | Master output level |

The old `drive` and `shape` params are replaced — `drive` by the pre-EQ,
`shape` by the 9 per-cell shape morphs.

## 5. Parameter Summary (28 total)

| Index | ID | Purpose |
|---|---|---|
| 0 | `seed` | Noise field seed |
| 1 | `possibility` | Noise field position |
| 2–10 | `pre_lowshelf_freq/q/gain`, `pre_bell_f/q/g`, `pre_highshelf_f/q/g` | Pre-EQ (9) |
| 11 | `post_eq_depth` | Post-EQ mirror depth |
| 12 | `cross_freq_low` | Crossover low→mid |
| 13 | `cross_freq_mid` | Crossover mid→high |
| 14–16 | `band_low/mid/high_level` | Per-band output levels (3) |
| 17 | `cell_lq_shape` | Low/Quiet wavetable morph |
| 18 | `cell_mq_shape` | Mid/Quiet wavetable morph |
| 19 | `cell_hq_shape` | High/Quiet wavetable morph |
| 20 | `cell_lm_shape` | Low/Moderate wavetable morph |
| 21 | `cell_mm_shape` | Mid/Moderate wavetable morph |
| 22 | `cell_hm_shape` | High/Moderate wavetable morph |
| 23 | `cell_ll_shape` | Low/Loud wavetable morph |
| 24 | `cell_ml_shape` | Mid/Loud wavetable morph |
| 25 | `cell_hl_shape` | High/Loud wavetable morph |
| 26 | `mix` | Dry/wet |
| 27 | `output` | Master level |

All params except `seed`/`possibility` are mapped to noise field indices
and support envelope-follower modulation (arrow system).

## 6. DSP Architecture Notes

- **Oversampling wraps everything**: The existing `juce::dsp::Oversampling`
  (2×, 2 stages = 4×) wraps the entire chain: pre-EQ → crossover →
  per-band FFT + cell wavetables → sum → post-EQ. This keeps latency
  compensation clean.

- **FFT at oversampled rate**: At 4× oversampled rate (192 kHz from 48 kHz
  input), a 1024-sample FFT frame is ~5.3 ms. FFT runs inside the
  oversampled block to avoid double-buffering.

- **Per-band FFT**: Each of the 3 frequency bands gets its own FFT analysis
  (so quiet bins in the low band don't compete with loud bins in the high
  band). This means 3× FFT processing — acceptable given the small frame
  size and the fact this is not real-time critical in distortion context.

- **Crossover filters**: `juce::dsp::LinkwitzRileyFilter<4>` (4th order).
  Three filters create low, band-pass, and high sections that sum to unity.

- **Wavetable structure**: Keep `kWaveTableSize = 513` and 8 base curves.
  Each of the 9 cells gets its **own independent permutation** of the 8
  curves, derived from the seed. This means a morph position of 0.5 in
  cell (low,quiet) selects a completely different curve than 0.5 in
  (mid,loud). The seed deterministically shuffles all 9 cell orderings
  independently using the same hash function as `SmoothNoise`.

- **Envelope follower**: Keep the existing `gravel::EnvelopeFollower`
  system. The FFT analysis handles the gain-layer blending, but the
  arrow/modulation system still works on all scalar params.

- **Memory**: 9 wavetables × 513 floats × 2 (one current, one rebuild
  buffer) ≈ 37 KB — negligible.

## 7. UI Layout

Three-column layout matching the existing minimal visual language (dark
background, thin lines, void/ember colours):

```
┌──────────────────────────────────────────────────────────────────┐
│  GRAVEL / POSSIBILITY SPACE           SEED: [____] [NEW FIELD]  │
├────────────────┬──────────────────────┬─────────────────────────┤
│                │                      │                         │
│  Spectral Grid │  Selected Cell:      │  Cell Controls          │
│  (3×3 tic-tac- │  Low / Moderate      │                         │
│   toe overlay  │  ┌────────────────┐  │  ┌────┐ ┌────┐ ┌────┐  │
│   on live FFT) │  │ shape morph    │  │  │ w1 │ │ w2 │ │ w3 │  │
│                │  │  (large fader)  │  │  └────┘ └────┘ └────┘  │
│  Drag lines to │  └────────────────┘  │  ┌────┐ ┌────┐ ┌────┐  │
│  reshape grid  │  Pre-EQ section     │  │ w4 │ │ w5 │ │ w6 │  │
│  Click cell to │  [faders]            │  └────┘ └────┘ └────┘  │
│  select        │  Global section     │  ┌────┐ ┌────┐ ┌────┐  │
│                │  [faders]            │  │ w7 │ │ w8 │ │ w9 │  │
│                │                      │  └────┘ └────┘ └────┘  │
├────────────────┴──────────────────────┴─────────────────────────┤
│  EXPLORATION  ════════════⬤════════════════════════════════     │
└──────────────────────────────────────────────────────────────────┘
```

- **Left**: 3×3 spectral grid overlaying real-time FFT analyzer.
  Each cell draws the current wavetable shape for that freq×gain
  combination. 2 vertical lines = crossover frequencies (draggable with
  push). 2 horizontal lines = amplitude thresholds (draggable).
  Click a cell to select it.

- **Middle**: Controls for the selected grid cell:
  - Large shape-morph fader (RuneFader style, scaled up)
  - Pre-EQ section (9 faders, grouped by band)
  - Global section (post-EQ depth, crossovers, band levels, mix, output)

- **Right**: All 9 cell mini-strips, each showing:
  - Wavetable shape thumbnail
  - Thin morph fader (~1/5 normal RuneFader height)
  - Active cell highlighted

- **Bottom**: Full-width exploration fader (possibility).

Existing visual language preserved throughout: thin 1px lines, no filled
backgrounds, #11100e ground, #d5d0b8 void, #ef8b48 ember accents.

## 8. Cell-to-Grid Mapping

The 9 cells in the 3×3 grid:

| \ | **Quiet** | **Moderate** | **Loud** |
|---|---|---|---|
| **Low** | cell_lq_shape | cell_lm_shape | cell_ll_shape |
| **Mid** | cell_mq_shape | cell_mm_shape | cell_ml_shape |
| **High** | cell_hq_shape | cell_hm_shape | cell_hl_shape |

Each cell is an independent wavetable with its own shape morph (0–1),
interpolating the same 8 seed-defined base curves. The grid lines
determine:
- **Vertical** (columns): the crossover frequencies dividing the spectrum
- **Horizontal** (rows): the amplitude thresholds dividing quiet/moderate/loud

When a cell is selected, the middle column shows its shape morph fader
at full size plus all global params.
