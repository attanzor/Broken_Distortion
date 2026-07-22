# Task 7: Wire Modulation to SpectralGrid Params

## Context

The modulation system computes `appliedModulation` for all 26 params correctly in `processBlock`, but only `mix` and `output` actually use the modulated value via `readParam`. All other spectralGrid params (pre-EQ, crossovers, cell shapes, etc.) only update when `parameterChanged` fires — missing envelope modulation.

This task adds a loop in `processBlock` that reads the modulated value for each param and forwards it to `spectralGrid`.

## File to Modify

- `Source/PluginProcessor.cpp`

## What to Do

In `processBlock()`, after the modulation computation loop and before the oversampler processing, add code to forward the modulated values to spectralGrid:

```cpp
// After:
//   const auto effMixParam = readParam (24);
//   const auto effOutputParam = readParam (25);
// But before:
//   juce::dsp::AudioBlock<float> outputBlock (buffer);

// Forward modulated values to spectralGrid
auto modulatedVal = [&](int index) -> float
{
    const auto base = parameters.getRawParameterValue (mappedParamIds[index])->load();
    const auto modAmount = modulationStates[static_cast<size_t> (index)].appliedModulation.load();
    return juce::jlimit (0.0f, 1.0f, base + modAmount);
};

// Pre-EQ (indices 0-8)
spectralGrid.setPreEqFreq (0, modulatedVal (0));
spectralGrid.setPreEqQ (0, modulatedVal (1));
spectralGrid.setPreEqGain (0, modulatedVal (2));

spectralGrid.setPreEqFreq (1, modulatedVal (3));
spectralGrid.setPreEqQ (1, modulatedVal (4));
spectralGrid.setPreEqGain (1, modulatedVal (5));

spectralGrid.setPreEqFreq (2, modulatedVal (6));
spectralGrid.setPreEqQ (2, modulatedVal (7));
spectralGrid.setPreEqGain (2, modulatedVal (8));

// Post-EQ depth (index 9)
spectralGrid.setPostEqDepth (modulatedVal (9));

// Crossover frequencies (indices 10-11)
spectralGrid.setCrossoverFreq (0, modulatedVal (10));
spectralGrid.setCrossoverFreq (1, modulatedVal (11));

// Band levels (indices 12-14)
spectralGrid.setBandLevel (0, modulatedVal (12));
spectralGrid.setBandLevel (1, modulatedVal (13));
spectralGrid.setBandLevel (2, modulatedVal (14));

// Cell shapes (indices 15-23)
for (int cell = 0; cell < 9; ++cell)
    spectralGrid.setCellShape (cell, modulatedVal (15 + cell));
```

Note: `spectralGrid.setCrossoverFreq()` and `spectralGrid.setThresholdLine()` accept frequency Hz and 0-1 values respectively. The `modulatedVal` returns 0-1 normalized values. The crossover frequencies need to be converted from normalized to Hz. But `readParam` returns the APVTS raw value which is already in Hz (not normalized 0-1) — because the APVTS stores the actual parameter value.

Wait — `readParam` uses `getRawParameterValue` which returns the actual value (20-20000 for freqs, -24-24 for gains, etc.), NOT a 0-1 normalized value. The modulation is applied as `base + modAmount` where modAmount is -1 to 1. This means for frequency params (20-20000), the modulation range is too narrow. This is actually a pre-existing design issue with the modulation system — it assumes all params are 0-1.

For now, just use `parameters.getRawParameterValue(mappedParamIds[index])->load()` directly (not modulated) for non-0-1 params, and `readParam` for 0-1 params. This keeps the current behavior for non-modulated params while enabling modulation on cell shapes, band levels, post-eq depth, mix, and output (which are all 0-1).

Alternatively, a simpler approach: just apply modulation to the 0-1 params (indices 9, 12-25) and pass raw values for the rest:

```cpp
// Params with meaningful 0-1 ranges (indices 9, 12-25)
const int modulatedIndices[] = { 9, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };
for (int i : modulatedIndices)
{
    const auto base = parameters.getRawParameterValue (mappedParamIds[i])->load();
    const auto modAmount = modulationStates[i].appliedModulation.load();
    const auto val = juce::jlimit (0.0f, 1.0f, base + modAmount);
    // route based on index...
}
```

Actually, the simplest correct approach: just use `readParam` for everything (it already applies modulation correctly to the raw value). For frequency params (20-20000), the modulation offset of ±1 is small relative to the range but that's fine — it won't cause issues.

Keep it simple:

```cpp
auto forwardModulated = [&](int index, auto&& setter)
{
    setter (readParam (index));
};
```

But `readParam` returns float, and `setPreEqFreq` takes `(int, float)`, `setPreEqGain` takes `(int, float)` where gain is in dB, etc. The `readParam` adds the modulation offset to the base value — so for gain (-24 to 24), adding a ±1 modulation moves it by ±1 dB which is subtle but usable.

Just call the spectralGrid setters with the modulated values for all 26 params using the simple `readParam` approach. If the modulation range is too small for frequency/gain params, that's a refinement for later.

## Verification

```bash
cmake --build build --target GravelPossibilityDistortion_All
```

## Report

Write to: `.superpowers/sdd/task-7-report.md`
