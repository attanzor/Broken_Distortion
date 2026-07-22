# Task 4 Report: PluginProcessor — global params + DSP pipeline

## What was implemented

Added Slope and Jitter modulation sources to the plugin processor:

1. **PluginProcessor.h**: Added `slopeFilterStates` and `jitterPhases` arrays (both `std::array<float, kNumMappedParams>`) as private members.

2. **createParameterLayout()**: Added 6 global APVTS float params — `slope_speed_mult`, `slope_spike_mult`, `slope_gain`, `jitter_speed_mult`, `jitter_spike_mult`, `jitter_gain` — all 0..2 range, default 1, with NormalisableRange skew 2.0.

3. **Parameter listeners**: Added `addParameterListener` / `removeParameterListener` for all 6 new params in constructor/destructor.

4. **processBlock()**:
   - Loads 6 global multiplier values at top
   - Added `prevInputLevel` tracking before per-sample loop
   - Slope: signed derivative of input level → one-pole smoothing per-param → spikyness pow → multiplied by slopeAmount and globalSlopeGain → stored in `mod.slopeApplied`
   - Jitter: phase accumulation per-param → SmoothNoise sample mapped to [-1,1] → spikyness pow → multiplied by jitterAmount and globalJitterGain → stored in `mod.jitterApplied`
   - Final stacking: slopeApplied and jitterApplied added to env-based modulation, respecting slopeEnabled/jitterEnabled toggles

5. **prepareToPlay()**: Both arrays zero-initialized on reset.

6. **Build**: Compiles and links cleanly, installs to /Library/Audio/Plug-Ins/.

## Files changed

- `Source/PluginProcessor.h` — added 2 member arrays (lines 63-64)
- `Source/PluginProcessor.cpp` — 6 edits across createParameterLayout, constructor, destructor, processBlock, prepareToPlay

## Self-review findings

All 6 steps from the brief are implemented exactly as specified:
- Array sizes match `kNumMappedParams` (38)
- SmoothNoise channels for jitter use `i * 50 + 500` to avoid overlap with existing noise channels
- Envelope follower processing still happens in the same loop (after slope/jitter compute)
- Lazy eval: `slopeApplied` and `jitterApplied` computed per-sample, last-sample values used in block-level stacking

## Concerns

None.
