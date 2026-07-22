# Task 3: Update PluginProcessor and Parameter Layout — Report

**Status:** Complete — builds cleanly (all formats: VST3, AU, Standalone)

## Changes Made

### `Source/PluginProcessor.h`

- **Removed:** `kWaveTableSize`, `kNumWaveShapes`, `WaveShapeEntry` struct, `WaveTable` typedef, all old wavetable member arrays (`waveShapes`, `shapeOrders`, `orderedShapeIndices`, `currentShapeBuffer`), and old method declarations (`rebuildWaveTable`, `updateCurrentShapeBuffer`, `fillWaveShapeBuffer`, `lookupWaveShape`)
- **Added:** `#include "dsp/SpectralGridProcessor.h"`, `::dsp::SpectralGridProcessor spectralGrid` member
- **Changed:** `kNumMappedParams` from 4 → 26; replaced `getCurrentShapeData()` with `getCellShapeData(int cellIndex)`
- **Kept:** `oversampler`, `dryBuffer`, `modulationStates[26]`, `envelopeFollowers[26]`
- **Note:** Uses `::dsp::` prefix to disambiguate from `juce::dsp` (JUCE's `using namespace juce;` in JuceHeader.h causes namespace collision)

### `Source/PluginProcessor.cpp`

**Constructor:**
- Removed old `mapParameters({...})` with `drive`/`mix`/`output`/`shape`
- Mapped 26 parameters to noise field indices 2–27 (seed 0, possibility 1 are implicit)
- Added `spectralGrid.setSeed(1024)` replacing old `rebuildWaveTable()`
- Removed `"shape"` parameter listener

**`createParameterLayout()`:**
- 28 controls total: `seed` (int), `possibility` (float), 9 pre-EQ params (3 stages × freq/q/gain), `post_eq_depth`, 2 crossover freqs, 3 band levels, 9 cell shapes (`cell_*_shape`), `mix`, `output`
- Frequency params use `NormalisableRange<float>(20, 20000, 0, 0.3f)` for log-like mapping

**`parameterChanged()`:**
- Routes `seed` → `spectralGrid.setSeed()`
- Routes `pre_*` params to `setPreEqFreq/Q/Gain(stage, val)` (stages 0=lowshelf, 1=bell, 2=highshelf)
- Routes `post_eq_depth` → `setPostEqDepth()`
- Routes `cross_freq_*` → `setCrossoverFreq()`
- Routes `band_*_level` → `setBandLevel()`
- Routes `cell_*_shape` → `setCellShape()` (see cell index mapping below)
- `mix`/`output` used directly in dry/wet mix (no spectralGrid setter needed)
- `possibility` handled by `possibilitySpace.handleParameterChange()` → triggers `updateAll()` which automates all mapped params

**Cell index mapping:**
| Param ID | Cell Index |
|---|---|
| `cell_lq_shape` | 0 (band=0, layer=0) |
| `cell_lm_shape` | 1 (band=0, layer=1) |
| `cell_ll_shape` | 2 (band=0, layer=2) |
| `cell_mq_shape` | 3 (band=1, layer=0) |
| `cell_mm_shape` | 4 (band=1, layer=1) |
| `cell_ml_shape` | 5 (band=1, layer=2) |
| `cell_hq_shape` | 6 (band=2, layer=0) |
| `cell_hm_shape` | 7 (band=2, layer=1) |
| `cell_hl_shape` | 8 (band=2, layer=2) |

**`prepareToPlay()`:**
- Added `spectralGrid.prepare(sampleRate, samplesPerBlock)`
- FFT latency: `setLatencySamples(oversamplerLatency + kFftSize / 4)` (= 256 samples)

**`processBlock()`:**
- Kept envelope follower processing (now 26 channels) and modulation computation
- Oversampler wraps `spectralGrid.processBlock()` via a non-owning `AudioBuffer` wrapper around the oversampled `AudioBlock` channel pointers
- Dry/wet mix formula unchanged, reads `mix`(index 24) and `output`(index 25) with envelope modulation via `readParam()`

**Removed methods:**
- `fillWaveShapeBuffer`, `lookupWaveShape`, `updateCurrentShapeBuffer`, `rebuildWaveTable` — all deleted

**`getCellShapeData(int)`** replaces `getCurrentShapeData()` — delegates to `spectralGrid.getCellShapeData(cellIndex)`

**State serialization:**
- Updated `getStateInformation`/`setStateInformation` to use the 26-element `mappedParamIds` array (file-level `static constexpr`)

### `Source/PluginEditor.h` / `Source/PluginEditor.cpp`

- Minimal changes to allow processor build: fixed `kWaveTableSize` reference → `::dsp::SpectralGridProcessor::kWaveTableSize`, replaced `getCurrentShapeData()` → `getCellShapeData(0)`
- Editor is expected to be non-functional until Task 6

## Verification

```bash
$ cmake --build build --target GravelPossibilityDistortion_All
# → Build succeeded: VST3, AU, Standalone all compiled and installed
```

## Files Modified

| File | Changes |
|---|---|
| `Source/PluginProcessor.h` | Major refactor: added SpectralGridProcessor, removed wavetable code |
| `Source/PluginProcessor.cpp` | New parameter layout, processBlock, parameterChanged; removed old DSP |
| `Source/PluginEditor.h` | Minimal: fixed kWaveTableSize reference |
| `Source/PluginEditor.cpp` | Minimal: replaced getCurrentShapeData calls |

## Notes

- `::dsp::` prefix required because JuceHeader.h injects `using namespace juce;` which brings `juce::dsp` into scope
- All 26 mapped params receive envelope modulation tracking, but only `mix` and `output` apply it at runtime (other params are set via `parameterChanged` → spectralGrid setters)
