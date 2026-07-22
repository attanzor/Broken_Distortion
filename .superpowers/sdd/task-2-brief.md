# Task 2: Implement SpectralGridProcessor DSP Core

## Context

Task 1 created the SpectralGridProcessor skeleton with compilable stubs. This task fills in all the real DSP processing. The class lives at:
- Header: `Source/dsp/SpectralGridProcessor.h`
- Impl: `Source/dsp/SpectralGridProcessor.cpp`

## What to Implement

### 1. `rebuildBaseCurves()` — Full wavetable curve generation

Replace the stub that fills all curves with linear values. Use the same 8 curves as the original plugin (in `PluginProcessor.cpp:fillWaveShapeBuffer`):

```cpp
void SpectralGridProcessor::rebuildBaseCurves()
{
    for (int curve = 0; curve < kNumBaseCurves; ++curve)
    {
        for (int s = 0; s < kWaveTableSize; ++s)
        {
            const auto x = (static_cast<float> (s) / static_cast<float> (kWaveTableSize - 1)) * 2.0f - 1.0f;
            float value = 0.0f;

            switch (curve)
            {
                case 0: value = x; break;  // Linear
                case 1: value = std::tanh (x * 2.2f) / std::tanh (2.2f); break;  // Tanh soft
                case 2: value = juce::jlimit (-1.0f, 1.0f, x * 3.0f); break;    // Hard clip
                case 3: value = std::atan (x * 4.0f) / std::atan (4.0f); break; // Arctan
                case 4: value = std::sin (x * juce::MathConstants<float>::halfPi); break; // Sine
                case 5: value = std::copysign ((1.0f - std::exp (-3.0f * std::abs (x))) / (1.0f - std::exp (-3.0f)), x); break; // Exponential
                case 6: value = (2.0f * x) / (1.0f + std::abs (x)); break; // Rational
                case 7: value = 4.0f * x * x * x - 3.0f * x; break; // Chebyshev
            }

            baseCurves[curve][s] = juce::jlimit (-1.0f, 1.0f, value);
        }
    }
}
```

### 2. `rebuildCellPermutations()` — Per-cell permutation from seed

Each cell gets a deterministic permutation of the 8 base curves, derived from seed + cell index. Use `juce::Random` with a unique seed per cell.

```cpp
void SpectralGridProcessor::rebuildCellPermutations()
{
    for (int cell = 0; cell < kNumCells; ++cell)
    {
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

### 3. `updateCellWavetable(int)` — Morph interpolation

Blend between 2 of the 8 base curves based on the cell's shape morph parameter, using the cell's permutation ordering.

```cpp
void SpectralGridProcessor::updateCellWavetable (int cellIndex)
{
    if (cellIndex < 0 || cellIndex >= kNumCells)
        return;

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

### 4. `setSeed(uint32_t)` — Full rebuild

```cpp
void SpectralGridProcessor::setSeed (uint32_t seed)
{
    currentSeed = seed;
    rebuildBaseCurves();
    rebuildCellPermutations();
    for (int i = 0; i < kNumCells; ++i)
        updateCellWavetable (i);
}
```

### 5. `setCellShape(int, float)` — Also trigger wavetable update

```cpp
void SpectralGridProcessor::setCellShape (int cellIndex, float morph)
{
    if (cellIndex >= 0 && cellIndex < kNumCells)
    {
        cellWavetables[cellIndex].shapeMorph = morph;
        updateCellWavetable (cellIndex);
    }
}
```

### 6. EQ Parameter Setters — Update filter coefficients

`setPreEqFreq`, `setPreEqQ`, `setPreEqGain` must recalculate the IIR coefficients. Stage 0 = low-shelf, stage 1 = bell (peak), stage 2 = high-shelf. Post-EQ mirrors with inverted gain and applies `postEqDepth`.

```cpp
void SpectralGridProcessor::setPreEqFreq (int stage, float freqHz)
{
    if (stage < 0 || stage >= 3) return;
    preEqStages[stage].freqHz = freqHz;
    postEqStages[stage].freqHz = freqHz;

    auto makeCoeffs = [&](int s, float gain) -> juce::dsp::IIR::Coefficients<float>::Ptr
    {
        const auto f = (s == 0) ? preEqStages[0].freqHz : (s == 1) ? preEqStages[1].freqHz : preEqStages[2].freqHz;
        const auto q = (s == 0) ? preEqStages[0].q : (s == 1) ? preEqStages[1].q : preEqStages[2].q;
        if (s == 0) return juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
        if (s == 1) return juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
        return juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
    };

    *preEqStages[stage].filter.coefficients = *makeCoeffs (stage, preEqStages[stage].gainDb);
    *postEqStages[stage].filter.coefficients = *makeCoeffs (stage, -postEqStages[stage].gainDb * postEqDepth);
}
```

`setPreEqQ` and `setPreEqGain` follow the same pattern (update stored value, recalc coeffs, mirror to post-EQ with inverted gain).

`setPostEqDepth` just stores the value and triggers a recalc of all post-EQ coefficients.

### 7. `setCrossoverFreq(int, float)` — With push constraint

```cpp
void SpectralGridProcessor::setCrossoverFreq (int bandIndex, float freqHz)
{
    if (bandIndex == 0) crossFreqLow = freqHz;
    else if (bandIndex == 1) crossFreqMid = freqHz;

    crossFreqLow = std::min (crossFreqLow, crossFreqMid * 0.9f);
    crossFreqMid = std::max (crossFreqMid, crossFreqLow * 1.1f);

    crossoverLow.setCutoffFrequency (static_cast<float> (crossFreqLow));
    crossoverMid.setCutoffFrequency (static_cast<float> (crossFreqLow));
    crossoverHigh.setCutoffFrequency (static_cast<float> (crossFreqMid));
}
```

### 8. `processBlock()` — Full signal chain

The main processing pipeline:

1. **Pre-EQ**: Apply `applyEq(buffer, preEqStages)` to the input buffer
2. **Crossover**: Create 3 band buffers, process each sample through the LR filters to separate into low/mid/high, apply per-band levels
3. **Per-band FFT + gain-layer processing**: 
   - Each band gets its own FFT ring buffer
   - When enough samples accumulate (hop size), window+FFT, process per-bin using thresholds to blend between 3 gain-layer wavetables, IFFT, overlap-add
4. **Sum bands**: Add all 3 band buffers back to the main buffer
5. **Post-EQ**: Apply `applyEq(buffer, postEqStages)`

Key decisions for the FFT processing:
- Use Hann window for both analysis and synthesis (COLA)
- Blend weight per bin: compare bin magnitude to threshold[0] and threshold[1]
- Below threshold[0]: use cell (bandIndex*3 + 0) 100%
- Between thresholds: crossfade cell (bandIndex*3 + 0) and (bandIndex*3 + 1)
- Above threshold[1]: crossfade cell (bandIndex*3 + 1) and (bandIndex*3 + 2)
- Apply bin magnitude through the wavetable to determine drive amount

### 9. `applyEq()` — Already implemented in skeleton, keep as-is

### 10. `lookupWaveShape()` — Already implemented, keep as-is

### 11. `setThresholdLine(int, float)` — With push constraint

Ensure thresholds don't cross:
```cpp
void SpectralGridProcessor::setThresholdLine (int lineIndex, float value)
{
    if (lineIndex >= 0 && lineIndex < 2)
    {
        thresholds[lineIndex] = juce::jlimit (0.0f, 1.0f, value);
        if (thresholds[0] >= thresholds[1])
            thresholds[1] = std::min (1.0f, thresholds[0] + 0.05f);
    }
}
```

## Verification

After implementing, run:
```bash
cmake --build build --target GravelPossibilityDistortion_All
```

The build must succeed.

## Report

Write report to: `.superpowers/sdd/task-2-report.md`
Include: what was implemented, any challenges, build result, concerns.

## No Git

Skip all git operations.
