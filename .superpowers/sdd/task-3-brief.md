# Task 3: Update PluginProcessor and Parameter Layout

## Context

Task 2 completed the SpectralGridProcessor DSP module. Now we wire it into the PluginProcessor, replacing the old single-wavetable approach with the new multiband architecture.

## Files to Modify

- `Source/PluginProcessor.h`
- `Source/PluginProcessor.cpp`

## What to Do

### 1. Update `PluginProcessor.h`

**Remove:**
- `static constexpr int kNumWaveShapes = 8;`
- `struct WaveShapeEntry` (and the `WaveTable` typedef)
- `waveShapes`, `shapeOrders`, `orderedShapeIndices`, `currentShapeBuffer` member arrays
- `rebuildWaveTable()`, `updateCurrentShapeBuffer()`, `fillWaveShapeBuffer()`, `lookupWaveShape()` method declarations
- The `WaveTable` usings/typedefs

**Change:**
- `kNumMappedParams` from 4 to 26

**Add:**
- `#include "dsp/SpectralGridProcessor.h"`
- `dsp::SpectralGridProcessor spectralGrid;` member
- `std::array<float, dsp::SpectralGridProcessor::kWaveTableSize> getCellShapeData (int cellIndex) const;` accessor
- Keep `oversampler`, `dryBuffer`, `modulationStates`, `envelopeFollowers` but change modulationStates/envelopeFollowers to size `kNumMappedParams` (26)

### 2. Update `PluginProcessor.cpp`

**Constructor changes:**
- Remove old `possibilitySpace.mapParameters({...})` with 4 entries
- Map 26 parameters to noise field indices. Params are:
  - `pre_lowshelf_freq`(2), `pre_lowshelf_q`(3), `pre_lowshelf_gain`(4)
  - `pre_bell_freq`(5), `pre_bell_q`(6), `pre_bell_gain`(7)
  - `pre_highshelf_freq`(8), `pre_highshelf_q`(9), `pre_highshelf_gain`(10)
  - `post_eq_depth`(11)
  - `cross_freq_low`(12), `cross_freq_mid`(13)
  - `band_low_level`(14), `band_mid_level`(15), `band_high_level`(16)
  - `cell_lq_shape`(17), `cell_mq_shape`(18), `cell_hq_shape`(19)
  - `cell_lm_shape`(20), `cell_mm_shape`(21), `cell_hm_shape`(22)
  - `cell_ll_shape`(23), `cell_ml_shape`(24), `cell_hl_shape`(25)
  - `mix`(26), `output`(27)
- Add parameter listeners for all new params
- Remove old wave table rebuild init code, replace with `spectralGrid.setSeed(1024)`
- Keep seed/possibility listeners

**`createParameterLayout()` changes:**
- Keep `seed` (int, 0-999999, 1024), `possibility` (float, 0-1, 0)
- Add pre-EQ params (9 floats with NormalisableRange):
  - pre_lowshelf_freq/q/gain: 20-20000Hz (log), 0.1-10, -24-+24dB
  - pre_bell_freq/q/gain: same ranges
  - pre_highshelf_freq/q/gain: same ranges
  - Use `juce::NormalisableRange<float>(min, max, 0.0f, 0.3f)` for frequency to get log-like mapping
- Add `post_eq_depth` (0-1, 1.0)
- Add `cross_freq_low`, `cross_freq_mid` (20-20000Hz, log, defaults 300 and 3000)
- Add `band_low_level`, `band_mid_level`, `band_high_level` (0-1, 1.0)
- Add 9 cell shape params: `cell_lq/mq/hq/lm/mm/hm/ll/ml/hl_shape` (0-1, 0.5)
- Add `mix` (0-1, 1.0), `output` (0-1, 0.75)
- Remove old `drive` and `shape` params

**`prepareToPlay()` changes:**
- Add `spectralGrid.prepare(sampleRate, samplesPerBlock)`
- Add FFT latency to `setLatencySamples()`
- Remove old oversampler init from here (keep in processBlock)

**`processBlock()` changes:**
- Remove old wave table lookup machinery
- Keep envelope follower processing (same logic, extended to 26 params)
- The oversampler wraps `spectralGrid.processBlock()` instead of old lookup
- After oversampling: `spectralGrid.processBlock(osBuffer)` in the oversampled space
- Dry/wet mix stays the same

**`parameterChanged()` changes:**
- Deeply refactored: instead of just `drive`/`mix`/`output`/`shape`, route each parameter to the appropriate `spectralGrid.set*()` method
- Seed → `spectralGrid.setSeed()`
- `pre_*` params → `spectralGrid.setPreEqFreq/Q/Gain()` based on the stage
- `cross_freq_*` → `spectralGrid.setCrossoverFreq()`
- `band_*_level` → `spectralGrid.setBandLevel()`
- `cell_*_shape` → `spectralGrid.setCellShape()` (map cell ID string to cell index)
- `post_eq_depth` → `spectralGrid.setPostEqDepth()`
- `mix`/`output` → handle directly
- `possibility` → `possibilitySpace.handleParameterChange()`

**Remove:** `fillWaveShapeBuffer`, `lookupWaveShape`, `updateCurrentShapeBuffer`, `rebuildWaveTable`, `getCurrentShapeData` methods entirely

**Replace `getCurrentShapeData()`** with `getCellShapeData(int cellIndex)` that delegates to `spectralGrid.getCellShapeData(cellIndex)`

**Destructor changes:** Remove old parameter listener removals for drive/shape

## Verification

```bash
cmake --build build --target GravelPossibilityDistortion_All
```

Must compile cleanly (editor will be broken until Task 6, but processor alone must compile).

## Report

Write to: `.superpowers/sdd/task-3-report.md`
