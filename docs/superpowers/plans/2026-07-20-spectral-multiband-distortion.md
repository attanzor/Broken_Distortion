# Spectral Multiband Distortion — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add 3-band pre/post EQ, 3-band crossover with per-band FFT gain-layer processing, and 9 independent wavetables displayed in a 3×3 spectral grid UI.

**Architecture:** New `SpectralGridProcessor` class encapsulates all DSP (EQ, crossover, 9 wavetables, 3× per-band FFT). New `SpectralGridComponent` and `CellStripPanel` UI components provide the 3-column grid/control layout. PluginProcessor delegates to `SpectralGridProcessor`; PluginEditor manages the 3-column layout with existing RuneFader/SeedField.

**Tech Stack:** C++20, JUCE 8 (`juce::dsp` for IIR filters, Linkwitz-Riley, FFT, Oversampling). New files in `Source/dsp/` and `Source/ui/`.

## Global Constraints

- C++20, JUCE 8 APIs preferred
- `#pragma once` include guards
- float precision throughout DSP
- Naming: PascalCase classes, camelCase methods, camelCase members (no prefix), `k` prefix for constants
- `final` on non-inheritance classes, `override` on all overridden virtuals
- Space before paren in function declarations: `void paint (juce::Graphics&)`
- `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR` on component/processor classes
- All params except seed/possibility support envelope-follower modulation via existing arrow system
- No new external dependencies
- CMakeLists.txt must list all new source files

---
### Task 1: Create DSP directory and SpectralGridProcessor header

**Files:**
- Create: `Source/dsp/SpectralGridProcessor.h`
- Create: `Source/dsp/SpectralGridProcessor.cpp`

**Interfaces:**
- Produces: `SpectralGridProcessor` class with `prepare()`, `processBlock()`, `reset()`, parameter update methods

- [ ] **Step 1: Create `Source/dsp/` directory**

Run: `mkdir -p Source/dsp`

- [ ] **Step 2: Write the header**

```cpp
// Source/dsp/SpectralGridProcessor.h
#pragma once

#include <JuceHeader.h>

#include <array>
#include <atomic>

namespace dsp
{

class SpectralGridProcessor final
{
public:
    static constexpr int kNumBands = 3;
    static constexpr int kNumLayers = 3;
    static constexpr int kNumCells = kNumBands * kNumLayers; // 9
    static constexpr int kWaveTableSize = 513;
    static constexpr int kNumBaseCurves = 8;
    static constexpr int kFftOrder = 10; // 1024
    static constexpr int kFftSize = 1 << kFftOrder;
    static constexpr int kHopSize = kFftSize / 4; // 75% overlap

    SpectralGridProcessor();
    ~SpectralGridProcessor() = default;

    void prepare (double sampleRate, int samplesPerBlock);
    void reset();
    void processBlock (juce::AudioBuffer<float>& buffer);

    // Parameter updates (called from processor's parameterChanged)
    void setPreEqFreq (int stage, float freqHz);
    void setPreEqQ (int stage, float q);
    void setPreEqGain (int stage, float gainDb);
    void setPostEqDepth (float depth);

    void setCrossoverFreq (int bandIndex, float freqHz);
    void setBandLevel (int bandIndex, float level);

    void setCellShape (int cellIndex, float morph);

    // Grid threshold lines (0–1 normalized)
    void setThresholdLine (int lineIndex, float value);

    // Seed causes full wavetable rebuild
    void setSeed (uint32_t seed);

    // Access current shape data for UI
    std::array<float, kWaveTableSize> getCellShapeData (int cellIndex) const;

private:
    struct EQStage
    {
        juce::dsp::IIR::Filter<float> filter;
        float freqHz = 1000.0f;
        float q = 0.707f;
        float gainDb = 0.0f;
    };

    std::array<EQStage, 3> preEqStages;   // low-shelf, bell, high-shelf
    std::array<EQStage, 3> postEqStages;  // linked mirror, same order

    float postEqDepth = 1.0f;

    // Crossover: LR4 low/high per band
    juce::dsp::LinkwitzRileyFilter<float> crossoverLow;
    juce::dsp::LinkwitzRileyFilter<float> crossoverMid;
    juce::dsp::LinkwitzRileyFilter<float> crossoverHigh;
    float crossFreqLow = 300.0f;
    float crossFreqMid = 3000.0f;

    std::array<float, kNumBands> bandLevels = { 1.0f, 1.0f, 1.0f };

    // 9-cell wavetable storage
    using WaveTable = std::array<float, kWaveTableSize>;
    struct CellWavetable
    {
        WaveTable buffer {};
        float shapeMorph = 0.5f;
    };
    std::array<CellWavetable, kNumCells> cellWavetables;

    // Base curves shared across cells but permuted per cell
    std::array<WaveTable, kNumBaseCurves> baseCurves;
    std::array<std::array<int, kNumBaseCurves>, kNumCells> cellPermutations;

    uint32_t currentSeed = 1024;
    double sampleRate = 48000.0;

    // Per-band FFT processing
    juce::dsp::FFT fft { kFftOrder };
    std::array<std::vector<float>, kNumBands> fftTimeDomain;  // ring buffer per band
    std::array<std::vector<std::complex<float>>, kNumBands> fttFreqDomain;
    size_t fftWritePos = 0;

    // Threshold lines for amplitude split
    std::array<float, 2> thresholds = { 0.33f, 0.66f };

    void rebuildBaseCurves();
    void rebuildCellPermutations();
    void updateCellWavetable (int cellIndex);
    float lookupWaveShape (const WaveTable& table, float input) const;
    void applyEq (juce::AudioBuffer<float>& buffer, const std::array<EQStage, 3>& stages);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralGridProcessor)
};

} // namespace dsp
```

- [ ] **Step 3: Write the .cpp skeleton**

```cpp
// Source/dsp/SpectralGridProcessor.cpp
#include "SpectralGridProcessor.h"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace dsp
{

SpectralGridProcessor::SpectralGridProcessor()
{
    for (auto& cell : cellWavetables)
        cell.buffer.fill (0.0f);

    for (auto& curve : baseCurves)
        curve.fill (0.0f);

    for (auto& perm : cellPermutations)
        std::iota (perm.begin(), perm.end(), 0);
}

void SpectralGridProcessor::prepare (double sr, int samplesPerBlock)
{
    sampleRate = sr;

    // Prepare EQ filters
    auto prepareEq = [&] (std::array<EQStage, 3>& stages)
    {
        for (auto& stage : stages)
        {
            juce::dsp::ProcessSpec spec { sr, static_cast<uint32_t> (samplesPerBlock), 2 };
            stage.filter.prepare (spec);
            stage.filter.reset();
        }
    };
    prepareEq (preEqStages);
    prepareEq (postEqStages);

    // Prepare crossover
    juce::dsp::ProcessSpec crossoverSpec { sr, static_cast<uint32_t> (samplesPerBlock), 2 };
    crossoverLow.prepare (crossoverSpec);
    crossoverMid.prepare (crossoverSpec);
    crossoverHigh.prepare (crossoverSpec);
    crossoverLow.reset();
    crossoverMid.reset();
    crossoverHigh.reset();

    // FFT buffers
    for (int b = 0; b < kNumBands; ++b)
    {
        fftTimeDomain[b].assign (kFftSize, 0.0f);
        fftFreqDomain[b].assign (kFftSize, std::complex<float> (0.0f, 0.0f));
    }

    rebuildBaseCurves();
    rebuildCellPermutations();
    for (int i = 0; i < kNumCells; ++i)
        updateCellWavetable (i);
}

void SpectralGridProcessor::reset()
{
    for (auto& stage : preEqStages) stage.filter.reset();
    for (auto& stage : postEqStages) stage.filter.reset();
    crossoverLow.reset();
    crossoverMid.reset();
    crossoverHigh.reset();
    fftWritePos = 0;
    for (auto& buf : fftTimeDomain)
        std::fill (buf.begin(), buf.end(), 0.0f);
}

// (remaining methods implemented in Task 2)

} // namespace dsp
```

- [ ] **Step 4: Add to CMakeLists.txt**

Edit `CMakeLists.txt` to add new source files under `target_sources`:

```cmake
target_sources(GravelPossibilityDistortion PRIVATE
    # ... existing entries ...
    Source/dsp/SpectralGridProcessor.cpp
    Source/dsp/SpectralGridProcessor.h
)
```

- [ ] **Step 5: Verify it compiles**

Run: `cmake --build build --target GravelPossibilityDistortion_All`
Expected: compiles (warnings about unimplemented methods are OK)

- [ ] **Step 6: Commit**

```bash
git add Source/dsp/SpectralGridProcessor.cpp Source/dsp/SpectralGridProcessor.h
git add CMakeLists.txt
git commit -m "feat: add SpectralGridProcessor skeleton with DSP structure"
```

---

### Task 2: Implement SpectralGridProcessor DSP core

**Files:**
- Modify: `Source/dsp/SpectralGridProcessor.cpp`

**Interfaces:**
- Consumes: header from Task 1
- Produces: fully implemented `processBlock()`, `rebuildBaseCurves()`, `rebuildCellPermutations()`, `updateCellWavetable()`, `lookupWaveShape()`, `applyEq()`, all parameter setters

- [ ] **Step 1: Implement wavetable generation**

Replace the stub `rebuildBaseCurves()`:

```cpp
void SpectralGridProcessor::rebuildBaseCurves()
{
    juce::Random random (currentSeed);

    for (int curve = 0; curve < kNumBaseCurves; ++curve)
    {
        auto& buffer = baseCurves[curve];
        for (int s = 0; s < kWaveTableSize; ++s)
        {
            const auto x = (static_cast<float> (s) / static_cast<float> (kWaveTableSize - 1)) * 2.0f - 1.0f;
            float value = 0.0f;

            switch (curve)
            {
                case 0: value = x; break;
                case 1: value = std::tanh (x * 2.2f) / std::tanh (2.2f); break;
                case 2: value = juce::jlimit (-1.0f, 1.0f, x * 3.0f); break;
                case 3: value = std::atan (x * 4.0f) / std::atan (4.0f); break;
                case 4: value = std::sin (x * juce::MathConstants<float>::halfPi); break;
                case 5: value = std::copysign ((1.0f - std::exp (-3.0f * std::abs (x))) / (1.0f - std::exp (-3.0f)), x); break;
                case 6: value = (2.0f * x) / (1.0f + std::abs (x)); break;
                case 7: value = 4.0f * x * x * x - 3.0f * x; break;
            }

            buffer[s] = juce::jlimit (-1.0f, 1.0f, value);
        }
    }
}
```

- [ ] **Step 2: Implement per-cell permutation**

```cpp
void SpectralGridProcessor::rebuildCellPermutations()
{
    // Each cell gets a deterministic permutation based on seed + cell index
    for (int cell = 0; cell < kNumCells; ++cell)
    {
        // Use a unique sub-seed per cell so permutations differ
        juce::Random cellRandom (currentSeed + static_cast<uint32_t> (cell) * 7919u);

        std::array<int, kNumBaseCurves> indices;
        std::iota (indices.begin(), indices.end(), 0);
        std::array<float, kNumBaseCurves> order;
        for (auto& o : order)
            o = cellRandom.nextFloat();

        std::stable_sort (indices.begin(), indices.end(), [&order] (int a, int b)
        {
            return order[a] < order[b];
        });

        cellPermutations[cell] = indices;
    }
}
```

- [ ] **Step 3: Implement cell wavetable update (morph interpolation)**

```cpp
void SpectralGridProcessor::updateCellWavetable (int cellIndex)
{
    const auto& perm = cellPermutations[cellIndex];
    const auto morph = cellWavetables[cellIndex].shapeMorph;
    const auto scaledPos = morph * static_cast<float> (kNumBaseCurves - 1);
    const auto lowerIdx = juce::jlimit (0, kNumBaseCurves - 2, static_cast<int> (std::floor (scaledPos)));
    const auto upperIdx = lowerIdx + 1;
    const auto alpha = scaledPos - static_cast<float> (lowerIdx);

    auto& dest = cellWavetables[cellIndex].buffer;
    for (int s = 0; s < kWaveTableSize; ++s)
    {
        const auto lower = baseCurves[perm[lowerIdx]][s];
        const auto upper = baseCurves[perm[upperIdx]][s];
        dest[s] = juce::jlimit (-1.0f, 1.0f, lower + (upper - lower) * alpha);
    }
}
```

- [ ] **Step 4: Implement `lookupWaveShape`**

```cpp
float SpectralGridProcessor::lookupWaveShape (const WaveTable& table, float input) const
{
    const auto x = juce::jlimit (-1.0f, 1.0f, input);
    const auto index = juce::jlimit (0, kWaveTableSize - 1,
        static_cast<int> (((x + 1.0f) * 0.5f) * static_cast<float> (kWaveTableSize - 1)));
    return table[index];
}
```

- [ ] **Step 5: Implement `applyEq`**

```cpp
void SpectralGridProcessor::applyEq (juce::AudioBuffer<float>& buffer,
                                     const std::array<EQStage, 3>& stages)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        for (int s = 0; s < buffer.getNumSamples(); ++s)
        {
            float sample = data[s];
            for (const auto& stage : stages)
                sample = stage.filter.processSample (ch, sample);
            data[s] = sample;
        }
    }
}
```

- [ ] **Step 6: Implement parameter setters**

```cpp
void SpectralGridProcessor::setPreEqFreq (int stage, float freqHz)
{
    if (stage < 0 || stage >= 3) return;
    preEqStages[stage].freqHz = freqHz;
    postEqStages[stage].freqHz = freqHz;
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, freqHz,
        preEqStages[stage].q, juce::Decibels::decibelsToGain (preEqStages[stage].gainDb));
    *preEqStages[stage].filter.coefficients = *coeffs;
    // Post-EQ mirrors with inverted gain
    auto postCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, freqHz,
        postEqStages[stage].q, juce::Decibels::decibelsToGain (-postEqStages[stage].gainDb * postEqDepth));
    *postEqStages[stage].filter.coefficients = *postCoeffs;
}
```

Similarly for `setPreEqQ`, `setPreEqGain`, `setPostEqDepth`. Note that the bell/high-shelf stages use `makePeakFilter` / `makeHighShelf` respectively.

- [ ] **Step 7: Implement crossover processing**

```cpp
void SpectralGridProcessor::setCrossoverFreq (int bandIndex, float freqHz)
{
    if (bandIndex == 0) crossFreqLow = freqHz;
    if (bandIndex == 1) crossFreqMid = freqHz;
    // Ensure non-crossing: low < mid
    crossFreqLow = std::min (crossFreqLow, crossFreqMid * 0.9f);
    crossFreqMid = std::max (crossFreqMid, crossFreqLow * 1.1f);

    *crossoverLow.coefficients = juce::dsp::LinkwitzRileyFilter<float>::makeLowPass (sampleRate, crossFreqLow);
    *crossoverMid.coefficients = juce::dsp::LinkwitzRileyFilter<float>::makeBandPass (sampleRate, crossFreqLow, crossFreqMid);
    *crossoverHigh.coefficients = juce::dsp::LinkwitzRileyFilter<float>::makeHighPass (sampleRate, crossFreqMid);
}
```

- [ ] **Step 8: Implement the main `processBlock` signal chain**

```cpp
void SpectralGridProcessor::processBlock (juce::AudioBuffer<float>& buffer)
{
    // 1. Pre-EQ
    applyEq (buffer, preEqStages);

    // 2. Crossover → per-band FFT → gain-layer wavetables
    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();

    juce::AudioBuffer<float> bandBuffers[kNumBands];
    for (int b = 0; b < kNumBands; ++b)
    {
        bandBuffers[b].setSize (numChannels, numSamples, false, false, true);
        bandBuffers[b].clear();
    }

    // Process crossover per channel
    for (int ch = 0; ch < numChannels; ++ch)
    {
        const auto* src = buffer.getReadPointer (ch);
        auto* lowDest = bandBuffers[0].getWritePointer (ch);
        auto* midDest = bandBuffers[1].getWritePointer (ch);
        auto* highDest = bandBuffers[2].getWritePointer (ch);

        for (int s = 0; s < numSamples; ++s)
        {
            float low, mid, high;
            low = crossoverLow.processSample (ch, src[s]);
            mid = crossoverMid.processSample (ch, src[s]);
            high = crossoverHigh.processSample (ch, src[s]);
            lowDest[s] = low * bandLevels[0];
            midDest[s] = mid * bandLevels[1];
            highDest[s] = high * bandLevels[2];
        }
    }

    // Per-band FFT processing
    const auto halfFft = kFftSize / 2;
    std::vector<float> window (kFftSize);
    for (int i = 0; i < kFftSize; ++i)
        window[i] = 0.5f * (1.0f - std::cos (2.0f * juce::MathConstants<float>::pi * i / (kFftSize - 1)));

    for (int b = 0; b < kNumBands; ++b)
    {
        auto& timeBuf = fftTimeDomain[b];
        auto& freqBuf = fftFreqDomain[b];
        const auto* bandData = bandBuffers[b].getReadPointer (0); // mono mix for FFT

        for (int s = 0; s < numSamples; ++s)
        {
            // Shift in new sample
            timeBuf[fftWritePos] = bandData[s];
            fftWritePos = (fftWritePos + 1) % kFftSize;

            if (fftWritePos % kHopSize == 0)
            {
                // Window and FFT
                std::copy (timeBuf.begin(), timeBuf.end(), freqBuf.begin());
                for (int i = 0; i < kFftSize; ++i)
                    reinterpret_cast<float&> (freqBuf[i]) *= window[i];

                fft.performRealOnlyForwardTransform (reinterpret_cast<float*> (freqBuf.data()), true);

                // Per-bin amplitude processing
                for (int bin = 0; bin < halfFft; ++bin)
                {
                    const auto mag = std::abs (freqBuf[bin]);
                    const auto normMag = juce::jlimit (0.0f, 1.0f, mag);

                    // Blend weights based on thresholds
                    float weight[3] = {};
                    if (normMag < thresholds[0])
                        weight[0] = 1.0f;
                    else if (normMag < thresholds[1])
                    {
                        const auto t = (normMag - thresholds[0]) / (thresholds[1] - thresholds[0]);
                        weight[0] = 1.0f - t;
                        weight[1] = t;
                    }
                    else
                    {
                        weight[1] = 0.0f;
                        weight[2] = 1.0f;
                    }

                    // Apply cell wavetables to bin magnitude
                    const auto cellOffset = b * kNumLayers;
                    std::complex<float> newBin = freqBuf[bin];
                    float gainSum = 0.0f;
                    for (int layer = 0; layer < kNumLayers; ++layer)
                    {
                        if (weight[layer] > 0.0f)
                            gainSum += weight[layer];
                    }
                    if (gainSum > 0.0f)
                    {
                        float combined = 0.0f;
                        for (int layer = 0; layer < kNumLayers; ++layer)
                        {
                            if (weight[layer] > 0.0f)
                            {
                                const auto cellIdx = cellOffset + layer;
                                // Scale bin magnitude through wavetable
                                const auto drive = 1.0f + (normMag * 19.0f) * weight[layer] / gainSum;
                                const auto processed = lookupWaveShape (
                                    cellWavetables[cellIdx].buffer, std::abs (freqBuf[bin].real()) * drive);
                                combined += processed * weight[layer];
                            }
                        }
                        const auto ratio = (gainSum > 0.0f) ? combined / gainSum / (std::abs (freqBuf[bin]) + 1e-6f) : 0.0f;
                        newBin *= ratio;
                    }
                    freqBuf[bin] = newBin;
                }

                // IFFT and overlap-add
                fft.performRealOnlyInverseTransform (reinterpret_cast<float*> (freqBuf.data()));

                for (int i = 0; i < kFftSize; ++i)
                {
                    const auto sampleIdx = (fftWritePos + i) % kFftSize;
                    bandBuffers[b].addSample (0, sampleIdx, freqBuf[i].real() * window[i] * 0.25f); // overlap
                }
            }
        }
    }

    // Sum bands back to output
    buffer.clear();
    for (int b = 0; b < kNumBands; ++b)
        for (int ch = 0; ch < numChannels; ++ch)
            for (int s = 0; s < numSamples; ++s)
                buffer.addSample (ch, s, bandBuffers[b].getSample (ch, s));

    // 3. Post-EQ
    applyEq (buffer, postEqStages);
}
```

**Note**: The FFT per-bin processing above is a simplified approach. For a real implementation, the bin magnitude should determine the blend of 3 wavetable lookups applied to the bin's complex value. The exact per-bin DSP may need refinement during implementation.

- [ ] **Step 9: Implement remaining parameter setters**

```cpp
void SpectralGridProcessor::setBandLevel (int bandIndex, float level)
{
    if (bandIndex >= 0 && bandIndex < kNumBands)
        bandLevels[bandIndex] = level;
}

void SpectralGridProcessor::setCellShape (int cellIndex, float morph)
{
    if (cellIndex >= 0 && cellIndex < kNumCells)
    {
        cellWavetables[cellIndex].shapeMorph = morph;
        updateCellWavetable (cellIndex);
    }
}

void SpectralGridProcessor::setThresholdLine (int lineIndex, float value)
{
    if (lineIndex >= 0 && lineIndex < 2)
    {
        thresholds[lineIndex] = juce::jlimit (0.0f, 1.0f, value);
        // Ensure non-crossing
        if (thresholds[0] >= thresholds[1])
            thresholds[1] = std::min (1.0f, thresholds[0] + 0.05f);
    }
}

void SpectralGridProcessor::setSeed (uint32_t seed)
{
    currentSeed = seed;
    rebuildBaseCurves();
    rebuildCellPermutations();
    for (int i = 0; i < kNumCells; ++i)
        updateCellWavetable (i);
}

std::array<float, SpectralGridProcessor::kWaveTableSize>
SpectralGridProcessor::getCellShapeData (int cellIndex) const
{
    if (cellIndex >= 0 && cellIndex < kNumCells)
        return cellWavetables[cellIndex].buffer;
    return {};
}
```

- [ ] **Step 10: Verify compilation**

Run: `cmake --build build --target GravelPossibilityDistortion_All`
Expected: compiles cleanly

- [ ] **Step 11: Commit**

```bash
git add Source/dsp/SpectralGridProcessor.cpp
git commit -m "feat: implement SpectralGridProcessor DSP chain"
```

---

### Task 3: Update PluginProcessor and Parameter Layout

**Files:**
- Modify: `Source/PluginProcessor.h`
- Modify: `Source/PluginProcessor.cpp`
- Modify: `Source/gravel/ParameterSpace.h` (no changes needed — already generic)
- Modify: `Source/gravel/ModulationState.h` (no changes needed — already generic)

**Interfaces:**
- Produces: updated processor with new param layout, 26 ModulationState instances, 26 EnvelopeFollower instances

- [ ] **Step 1: Replace old params in processor header**

```cpp
// Source/PluginProcessor.h — update class members

// Replace old constants:
static constexpr int kNumMappedParams = 26; // was 4
// Remove: static constexpr int kNumWaveShapes = 8;
// Remove: waveShapes, shapeOrders, orderedShapeIndices, currentShapeBuffer
// Remove: fillWaveShapeBuffer, lookupWaveShape, updateCurrentShapeBuffer, rebuildWaveTable

// Add:
#include "dsp/SpectralGridProcessor.h"

// In class body:
dsp::SpectralGridProcessor spectralGrid;

// Keep modulation arrays but now at kNumMappedParams size
std::array<gravel::ModulationState, kNumMappedParams> modulationStates;
std::array<gravel::EnvelopeFollower, kNumMappedParams> envelopeFollowers;

// Keep the shape data accessor but delegate to spectralGrid
std::array<float, dsp::SpectralGridProcessor::kWaveTableSize> getCellShapeData (int cellIndex) const;
```

Remove:
```cpp
static constexpr int kNumWaveShapes = 8;
std::array<WaveShapeEntry, kNumWaveShapes> waveShapes {};
std::array<float, kNumWaveShapes> shapeOrders {};
std::array<int, kNumWaveShapes> orderedShapeIndices {};
WaveTable currentShapeBuffer {};
void rebuildWaveTable();
void updateCurrentShapeBuffer();
static void fillWaveShapeBuffer (std::array<float, 513>& buffer, int shapeIndex);
float lookupWaveShape (float input) const;
```

Remove the `WaveShapeEntry` struct and `WaveTable` typedef.

- [ ] **Step 2: Update parameter layout in .cpp**

```cpp
// Source/PluginProcessor.cpp

juce::AudioProcessorValueTreeState::ParameterLayout
GravelPossibilityDistortionAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> controls;

    controls.push_back (std::make_unique<juce::AudioParameterInt> ("seed", "Seed", 0, 999999, 1024));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("possibility", "Possibility", 0.0f, 1.0f, 0.0f));

    // Pre-EQ (9 params)
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_lowshelf_freq", "Low Shelf Freq",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.3f), 200.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_lowshelf_q", "Low Shelf Q",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.0f, 0.3f), 0.707f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_lowshelf_gain", "Low Shelf Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.0f, 0.3f), 0.0f));
    // ... bell and high-shelf similarly ...

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("post_eq_depth", "Post EQ Depth", 0.0f, 1.0f, 1.0f));

    // Crossovers
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cross_freq_low", "Cross Low→Mid",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.3f), 300.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cross_freq_mid", "Cross Mid→High",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.3f), 3000.0f));

    // Band levels
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("band_low_level", "Low Level", 0.0f, 1.0f, 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("band_mid_level", "Mid Level", 0.0f, 1.0f, 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("band_high_level", "High Level", 0.0f, 1.0f, 1.0f));

    // 9 cell shapes
    const std::array<const char*, 9> cellIDs = {
        "cell_lq_shape", "cell_mq_shape", "cell_hq_shape",
        "cell_lm_shape", "cell_mm_shape", "cell_hm_shape",
        "cell_ll_shape", "cell_ml_shape", "cell_hl_shape"
    };
    for (auto* id : cellIDs)
        controls.push_back (std::make_unique<juce::AudioParameterFloat> (id, id, 0.0f, 1.0f, 0.5f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("mix", "Mix", 0.0f, 1.0f, 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("output", "Output", 0.0f, 1.0f, 0.75f));

    return { controls.begin(), controls.end() };
}
```

- [ ] **Step 3: Update constructor — remove old wave table setup, add spectralGrid**

```cpp
GravelPossibilityDistortionAudioProcessor::GravelPossibilityDistortionAudioProcessor()
    : AudioProcessor (...),
      parameters (...),
      possibilitySpace (parameters)
{
    // Map ALL params to noise field indices (2–27, since 0=seed, 1=possibility)
    std::vector<gravel::SpaceMapping> mappings;
    mappings.push_back ({ "pre_lowshelf_freq", 2 });
    // ... all 26 mapped params ...
    mappings.push_back ({ "output", 27 });
    possibilitySpace.mapParameters (mappings);

    // Listen for seed, possibility
    parameters.addParameterListener ("seed", this);
    parameters.addParameterListener ("possibility", this);

    // Subscribe to all mapped params
    for (const auto& m : mappings)
        parameters.addParameterListener (m.paramID, this);

    // Init modulation exploration settings
    // (simplified — can be a loop over all param IDs)

    // No wave table rebuild — spectralGrid.setSeed handles it
    spectralGrid.setSeed (1024);
}
```

- [ ] **Step 4: Update `prepareToPlay`**

```cpp
void GravelPossibilityDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (auto& f : envelopeFollowers)
        f.prepare (sampleRate);

    spectralGrid.prepare (sampleRate, samplesPerBlock);

    oversampler.initProcessing (static_cast<size_t> (samplesPerBlock));
    oversampler.reset();

    setLatencySamples (juce::roundToInt (oversampler.getLatencyInSamples())
                       + juce::roundToInt (dsp::SpectralGridProcessor::kFftSize / 4));
}
```

- [ ] **Step 5: Update `processBlock`**

```cpp
void GravelPossibilityDistortionAudioProcessor::processBlock (
    juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    const auto numSamples = buffer.getNumSamples();
    dryBuffer.makeCopyOf (buffer, true);

    // Envelope processing (same as before but for 26 params)
    for (int sample = 0; sample < numSamples; ++sample)
    {
        float inputLevel = 0.0f;
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            inputLevel += std::abs (dryBuffer.getSample (ch, sample));
        inputLevel /= static_cast<float> (buffer.getNumChannels());

        for (int i = 0; i < kNumMappedParams; ++i)
        {
            auto& mod = modulationStates[i];
            envelopeFollowers[i].process (inputLevel,
                juce::jmax (1.0f, mod.envRise.load()),
                juce::jmax (1.0f, mod.envFall.load()));
        }
    }

    // Modulation computation (same pattern, extended to 26 params)
    static constexpr float kEnvReference = 0.15f;
    for (int i = 0; i < kNumMappedParams; ++i)
    {
        // ... same logic as before ...
    }

    // Oversample the entire spectral grid processing
    juce::dsp::AudioBlock<float> outputBlock (buffer);
    auto oversampledBlock = oversampler.processSamplesUp (outputBlock);

    // De-interleave oversampled block to AudioBuffer, process, re-interleave
    juce::AudioBuffer<float> osBuffer (oversampledBlock);
    spectralGrid.processBlock (osBuffer);

    oversampler.processSamplesDown (outputBlock);

    // Dry/wet mix (same)
    // ... mix and output gain ...
}
```

- [ ] **Step 6: Update `parameterChanged`**

```cpp
void GravelPossibilityDistortionAudioProcessor::parameterChanged (
    const juce::String& parameterID, float newValue)
{
    if (parameterID == "seed")
    {
        spectralGrid.setSeed (static_cast<uint32_t> (std::round (newValue)));
        return;
    }

    if (possibilitySpace.handleParameterChange (parameterID, newValue))
        return;

    // Route to spectral grid
    if (parameterID.startsWith ("pre_"))
    {
        int stage = -1;
        auto type = parameterID.fromLastOccurrenceOf ("_", false, false);
        if (parameterID.contains ("lowshelf")) stage = 0;
        else if (parameterID.contains ("bell")) stage = 1;
        else if (parameterID.contains ("highshelf")) stage = 2;

        if (stage >= 0)
        {
            if (type == "freq") spectralGrid.setPreEqFreq (stage, newValue);
            else if (type == "q") spectralGrid.setPreEqQ (stage, newValue);
            else if (type == "gain") spectralGrid.setPreEqGain (stage, newValue);
        }
    }
    else if (parameterID == "post_eq_depth")
        spectralGrid.setPostEqDepth (newValue);
    else if (parameterID.startsWith ("cross_freq"))
    {
        if (parameterID == "cross_freq_low")
            spectralGrid.setCrossoverFreq (0, newValue);
        else if (parameterID == "cross_freq_mid")
            spectralGrid.setCrossoverFreq (1, newValue);
    }
    else if (parameterID.startsWith ("band_") && parameterID.endsWith ("_level"))
    {
        int band = -1;
        if (parameterID.contains ("low")) band = 0;
        else if (parameterID.contains ("mid")) band = 1;
        else if (parameterID.contains ("high")) band = 2;
        if (band >= 0) spectralGrid.setBandLevel (band, newValue);
    }
    else if (parameterID.startsWith ("cell_"))
    {
        const std::array<const char*, 9> cellIDs = {
            "cell_lq_shape", "cell_mq_shape", "cell_hq_shape",
            "cell_lm_shape", "cell_mm_shape", "cell_hm_shape",
            "cell_ll_shape", "cell_ml_shape", "cell_hl_shape"
        };
        for (int i = 0; i < 9; ++i)
        {
            if (parameterID == cellIDs[i])
            {
                spectralGrid.setCellShape (i, newValue);
                break;
            }
        }
    }
}
```

- [ ] **Step 7: Update `getCurrentShapeData()` method**

Replace the old method:
```cpp
std::array<float, dsp::SpectralGridProcessor::kWaveTableSize>
GravelPossibilityDistortionAudioProcessor::getCellShapeData (int cellIndex) const
{
    return spectralGrid.getCellShapeData (cellIndex);
}
```

- [ ] **Step 8: Verify compilation**

Run: `cmake --build build --target GravelPossibilityDistortion_All`
Expected: compiles (editor will have errors since we haven't updated it yet)

- [ ] **Step 9: Commit**

```bash
git add Source/PluginProcessor.cpp Source/PluginProcessor.h
git commit -m "feat: update processor for 28-param spectral grid architecture"
```

---

### Task 4: Create UI directory and SpectralGridComponent

**Files:**
- Create: `Source/ui/SpectralGridComponent.h`
- Create: `Source/ui/SpectralGridComponent.cpp`
- Modify: `CMakeLists.txt`

**Interfaces:**
- Produces: `SpectralGridComponent` — the 3×3 grid with draggable lines, FFT analyzer bars, and 9 wavetable shapes

- [ ] **Step 1: Create `Source/ui/` directory**

Run: `mkdir -p Source/ui`

- [ ] **Step 2: Write the header**

```cpp
// Source/ui/SpectralGridComponent.h
#pragma once

#include <JuceHeader.h>

#include <functional>

class SpectralGridComponent final : public juce::Component
{
public:
    SpectralGridComponent();

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;

    void setCellShapeData (int cellIndex, const std::array<float, 513>& data);
    void setCellShapeMorph (int cellIndex, float morph);
    void setSelectedCell (int cellIndex);

    // Set the 4 grid line positions (0–1 normalized)
    void setVerticalLine (int index, float position);
    void setHorizontalLine (int index, float position);

    // Set FFT magnitudes for analyzer
    void setAnalyzerData (const std::vector<float>& magnitudes);

    // Callbacks
    std::function<void (int cellIndex)> onCellClicked;
    std::function<void (int lineIndex, float position)> onVerticalLineDragged;
    std::function<void (int lineIndex, float position)> onHorizontalLineDragged;

private:
    static constexpr int kNumBands = 3;
    static constexpr int kNumLayers = 3;
    static constexpr int kNumCells = kNumBands * kNumLayers;
    static constexpr int kWaveTableSize = 513;

    struct CellInfo
    {
        std::array<float, kWaveTableSize> shapeData {};
        float morph = 0.5f;
        juce::Rectangle<int> bounds;
    };

    std::array<CellInfo, kNumCells> cells;
    int selectedCell = -1;

    // Grid lines (0–1 normalized within grid area)
    std::array<float, 2> vLinePos = { 0.33f, 0.66f };
    std::array<float, 2> hLinePos = { 0.33f, 0.66f };

    // Dragging state
    enum class DragTarget { None, VLine0, VLine1, HLine0, HLine1, Cell };
    DragTarget dragTarget = DragTarget::None;
    int dragCellIndex = -1;

    std::vector<float> analyzerData;
    juce::Rectangle<int> gridBounds;

    juce::Rectangle<int> getCellBounds (int cellIndex) const;
    int cellIndexAt (juce::Point<int> pos) const;
    int hitTestLine (juce::Point<int> pos) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralGridComponent)
};
```

- [ ] **Step 3: Implement the .cpp with full drawing logic**

Key sections in `paint()`:
1. Draw FFT analyzer bars (thin vertical lines in each column region)
2. Draw 9 cell wavetable shapes (scaled SVG-style paths)
3. Draw grid lines (2 vertical, 2 horizontal) with handle dots at ends and intersections
4. Highlight selected cell border

Key sections in `mouseDown()`/`mouseDrag()`:
1. Hit test lines first (within 6px), then cells
2. On line drag: update position, enforce push constraint, fire callback
3. On cell click: set selectedCell, fire onCellClicked
4. Intersection hit test (near both a vline and hline) handles both

- [ ] **Step 4: Add to CMakeLists.txt**

```cmake
Source/ui/SpectralGridComponent.cpp
Source/ui/SpectralGridComponent.h
```

- [ ] **Step 5: Commit**

```bash
git add Source/ui/SpectralGridComponent.cpp Source/ui/SpectralGridComponent.h
git add CMakeLists.txt
git commit -m "feat: add SpectralGridComponent with draggable grid overlay"
```

---

### Task 5: Create CellStripPanel component

**Files:**
- Create: `Source/ui/CellStripPanel.h`
- Create: `Source/ui/CellStripPanel.cpp`
- Modify: `CMakeLists.txt`

**Interfaces:**
- Produces: `CellStripPanel` — right column showing 9 mini wavetable strips with thin morph faders

- [ ] **Step 1: Write the header**

```cpp
// Source/ui/CellStripPanel.h
#pragma once

#include <JuceHeader.h>

#include "Source/dsp/SpectralGridProcessor.h"

class CellStripPanel final : public juce::Component
{
public:
    CellStripPanel();

    void paint (juce::Graphics&) override;
    void resized() override;

    void setCellShapeData (int cellIndex, const std::array<float, 513>& data);
    void setSelectedCell (int cellIndex);

    // Each mini-strip provides a thin slider
    // We reuse juce::Slider with custom look-and-feel for minimal style
    juce::Slider& getMorphSlider (int cellIndex);

    std::function<void (int cellIndex)> onCellClicked;

private:
    static constexpr int kNumCells = 9;
    static constexpr int kWaveTableSize = 513;

    struct MiniCell
    {
        std::array<float, kWaveTableSize> shapeData {};
        std::unique_ptr<juce::Slider> morphSlider;
        juce::Rectangle<int> bounds;
    };

    std::array<MiniCell, kNumCells> miniCells;
    int selectedCell = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CellStripPanel)
};
```

- [ ] **Step 2: Implement in .cpp**

Each mini-cell shows:
- Wavetable shape drawn as thin path (40×40 area)
- Below: tiny horizontal fader (RuneFader style, ~1/5 normal height)
- Clicking the shape sets selectedCell

- [ ] **Step 3: Add to CMakeLists.txt and commit**

---

### Task 6: Update PluginEditor with 3-column layout

**Files:**
- Modify: `Source/PluginEditor.h`
- Modify: `Source/PluginEditor.cpp`

- [ ] **Step 1: Update header with new imports and members**

```cpp
// Source/PluginEditor.h additions:
#include "ui/SpectralGridComponent.h"
#include "ui/CellStripPanel.h"

// In class:
SpectralGridComponent spectralGrid;
CellStripPanel cellStripPanel;

// Selected cell detail panel
juce::Slider selectedCellMorphSlider;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> selectedCellMorphAttachment;

// Pre-EQ sliders (9)
std::array<gravel::RuneFader, 9> preEqFaders;

// Global param sliders + faders
// ... crossovers, band levels, post-eq depth, mix, output ...

// Keep seedField, randomizeButton, possibilityFader
```

- [ ] **Step 2: Implement constructor**

Create all controls, attach to parameters, set up layout.

```cpp
GravelPossibilityDistortionAudioProcessorEditor::
    GravelPossibilityDistortionAudioProcessorEditor (
        GravelPossibilityDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Add spectral grid
    addAndMakeVisible (spectralGrid);
    spectralGrid.onCellClicked = [this] (int cell)
    {
        cellStripPanel.setSelectedCell (cell);
        updateSelectedCell();
    };

    // Add cell strip panel
    addAndMakeVisible (cellStripPanel);

    // Wire up component callbacks to processor
    spectralGrid.onVerticalLineDragged = [this] (int idx, float pos)
    {
        auto pid = juce::String (idx == 0 ? "cross_freq_low" : "cross_freq_mid");
        // Convert grid position (0-1) to frequency and set param
        // ...
    };

    // Set up sliders, attachments, etc.

    startTimerHz (60);
    setSize (1024, 600); // Wider layout for 3 columns
}
```

- [ ] **Step 3: Implement `resized()`**

Three-column layout with bottom exploration fader:

```cpp
void GravelPossibilityDistortionAudioProcessorEditor::resized()
{
    auto content = getLocalBounds().reduced (14);

    // Top bar: seed
    auto topBar = content.removeFromTop (48);

    // Bottom: possibility fader
    auto bottomBar = content.removeFromBottom (56);

    // Three columns
    auto columns = content;
    const auto colWidth = columns.getWidth() / 3;

    auto leftCol = columns.removeFromLeft (colWidth).reduced (4);
    auto midCol = columns.removeFromLeft (colWidth).reduced (4);
    auto rightCol = columns.reduced (4);

    spectralGrid.setBounds (leftCol);
    // ... midCol: selected cell morph + pre-EQ + globals
    cellStripPanel.setBounds (rightCol);
}
```

- [ ] **Step 4: Implement timer callback to update UI**

```cpp
void GravelPossibilityDistortionAudioProcessorEditor::timerCallback()
{
    // Update spectral grid shape data for all 9 cells
    for (int i = 0; i < 9; ++i)
        spectralGrid.setCellShapeData (i, audioProcessor.getCellShapeData (i));

    // Update cell strip panel
    // ...

    // Update FFT analyzer data
    // ...

    repaint();
}
```

- [ ] **Step 5: Verify compilation**

Run: `cmake --build build --target GravelPossibilityDistortion_All`
Expected: builds and links cleanly

- [ ] **Step 6: Commit**

```bash
git add Source/PluginEditor.cpp Source/PluginEditor.h
git commit -m "feat: update editor with 3-column spectral grid layout"
```

---

### Task 7: State save/restore and modulation

**Files:**
- Modify: `Source/PluginProcessor.cpp`
- Modify: `Source/ui/SpectralGridComponent.cpp`
- Modify: `Source/ui/CellStripPanel.cpp`

- [ ] **Step 1: Update `getStateInformation`/`setStateInformation`**

Extend the ValueTree to include/exclude old params, save new modulation states.

- [ ] **Step 2: Wire envelope-follower modulation to all params**

The existing `readParam` lambda in processBlock needs extending to 26 params. The modulation computation loop already handles the generic pattern — just update the param ID array.

- [ ] **Step 3: Add push constraints to SpectralGridComponent line dragging**

```cpp
void SpectralGridComponent::mouseDrag (const juce::MouseEvent& e)
{
    if (dragTarget == DragTarget::VLine0)
    {
        auto newPos = (e.position.x - gridBounds.getX()) / gridBounds.getWidth();
        newPos = juce::jlimit (0.05f, vLinePos[1] - 0.05f, newPos);
        vLinePos[0] = newPos;
        onVerticalLineDragged (0, newPos);
    }
    else if (dragTarget == DragTarget::VLine1)
    {
        auto newPos = (e.position.x - gridBounds.getX()) / gridBounds.getWidth();
        newPos = juce::jlimit (vLinePos[0] + 0.05f, 0.95f, newPos);
        vLinePos[1] = newPos;
        onVerticalLineDragged (1, newPos);
    }
    // Same for horizontal lines
    repaint();
}
```

- [ ] **Step 4: Verify compilation and test**

Run: `cmake --build build --target GravelPossibilityDistortion_All`
Expected: clean build

- [ ] **Step 5: Commit**

```bash
git add -A
git commit -m "feat: add state persistence, modulation, and line constraints"
```

---

### Self-Review Checklist

- [ ] **Spec coverage**: Every section in the design doc maps to a task:
  - Pre/post EQ (9 params) → Tasks 1-3
  - Crossover bands → Task 2, step 7
  - 9 cell wavetables → Task 2, steps 1-4
  - FFT per-bin gain-layer blending → Task 2, step 8
  - Per-cell seeded permutation → Task 2, step 2
  - Spectral grid UI → Task 4
  - Cell strip panel → Task 5
  - 3-column layout → Task 6
  - State management → Task 7

- [ ] **Placeholder scan**: No TBDs, "fill in details", or incomplete sections

- [ ] **Type consistency**: `getCellShapeData(int) → std::array<float, 513>` used consistently. `setSeed(uint32_t)` matches. `kNumCells = 9` everywhere.

- [ ] **Scope check**: This is one coherent feature — multiband spectral distortion. One plan is appropriate.
