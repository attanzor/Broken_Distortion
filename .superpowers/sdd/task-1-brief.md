# Task 1: Create DSP directory and SpectralGridProcessor skeleton

## Context

This is the first task in the spectral multiband distortion expansion. It creates the DSP module that will house the new multiband processing chain. The plan is at `docs/superpowers/plans/2026-07-20-spectral-multiband-distortion.md`, the design spec is at `docs/superpowers/specs/2026-07-20-spectral-multiband-expansion-design.md`.

## Files to Create

- `Source/dsp/SpectralGridProcessor.h`
- `Source/dsp/SpectralGridProcessor.cpp`

## Files to Modify

- `CMakeLists.txt` (add new source files to target_sources)

## Requirements

1. Create `Source/dsp/` directory under `/Users/budi/Documents/Code/Gravel/Source/dsp/`
2. Write the header `SpectralGridProcessor.h` with the class in `dsp` namespace:

```cpp
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
    static constexpr int kHopSize = kFftSize / 4;

    SpectralGridProcessor();
    ~SpectralGridProcessor() = default;

    void prepare (double sampleRate, int samplesPerBlock);
    void reset();
    void processBlock (juce::AudioBuffer<float>& buffer);

    void setPreEqFreq (int stage, float freqHz);
    void setPreEqQ (int stage, float q);
    void setPreEqGain (int stage, float gainDb);
    void setPostEqDepth (float depth);
    void setCrossoverFreq (int bandIndex, float freqHz);
    void setBandLevel (int bandIndex, float level);
    void setCellShape (int cellIndex, float morph);
    void setThresholdLine (int lineIndex, float value);
    void setSeed (uint32_t seed);
    std::array<float, kWaveTableSize> getCellShapeData (int cellIndex) const;

private:
    struct EQStage
    {
        juce::dsp::IIR::Filter<float> filter;
        float freqHz = 1000.0f;
        float q = 0.707f;
        float gainDb = 0.0f;
    };

    std::array<EQStage, 3> preEqStages;
    std::array<EQStage, 3> postEqStages;
    float postEqDepth = 1.0f;

    juce::dsp::LinkwitzRileyFilter<float> crossoverLow;
    juce::dsp::LinkwitzRileyFilter<float> crossoverMid;
    juce::dsp::LinkwitzRileyFilter<float> crossoverHigh;
    float crossFreqLow = 300.0f;
    float crossFreqMid = 3000.0f;

    std::array<float, 3> bandLevels = { 1.0f, 1.0f, 1.0f };

    using WaveTable = std::array<float, kWaveTableSize>;
    struct CellWavetable
    {
        WaveTable buffer {};
        float shapeMorph = 0.5f;
    };
    std::array<CellWavetable, 9> cellWavetables;

    std::array<WaveTable, 8> baseCurves;
    std::array<std::array<int, 8>, 9> cellPermutations;

    uint32_t currentSeed = 1024;
    double sampleRate = 48000.0;

    juce::dsp::FFT fft { kFftOrder };
    std::array<std::vector<float>, 3> fftTimeDomain;
    std::array<std::vector<std::complex<float>>, 3> fftFreqDomain;
    size_t fftWritePos = 0;

    std::array<float, 2> thresholds = { 0.33f, 0.66f };

    void rebuildBaseCurves();
    void rebuildCellPermutations();
    void updateCellWavetable (int cellIndex);
    float lookupWaveShape (const WaveTable& table, float input) const;
    void applyEq (juce::AudioBuffer<float>& buffer, const std::array<EQStage, 3>& stages);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralGridProcessor)
};
```

3. Write the .cpp with skeleton implementations:
   - Constructor: initialize buffers, fill cellPermutations with identity sequences
   - `prepare()`: set sampleRate, prepare EQ filters (ProcessSpec with given sampleRate/samplesPerBlock/2 channels), prepare crossover filters, allocate FFT buffers, call rebuildBaseCurves/rebuildCellPermutations/updateCellWavetable for all cells
   - `reset()`: reset all filters and FFT state
   - Keep other methods as stubs (method body can be empty or minimal)

4. The .cpp skeleton must compile — all JUCE types must be properly used:
   - `juce::dsp::ProcessSpec` for filter preparation
   - `juce::dsp::IIR::Filter<float>` with `prepare()` and `reset()`
   - `juce::dsp::LinkwitzRileyFilter<float>` with `prepare()` and `reset()`
   - `juce::dsp::FFT` with `kFftOrder`
   - `juce::AudioBuffer<float>` in signatures
   - Include `<JuceHeader.h>` and use `<array>`, `<vector>`, `<complex>`, `<algorithm>`, `<cmath>`, `<numeric>`

5. Add both files to `CMakeLists.txt` under `target_sources`

## Coding Conventions

- `#pragma once` in headers
- C++20, JUCE 8 APIs
- `final` on class, `override` on overridden virtuals where applicable
- Space before paren in declarations: `void prepare (double, int)`
- CamelCase for class names, camelCase for methods and members
- No `m_` or `m` prefix on member variables

## Report

After completing, write a brief report to:
`.superpowers/sdd/task-1-report.md`

Include: files created/modified, any issues encountered, verification that the project compiles.

## No Git

This project has no git repository. Skip all git-related steps (commits, git add, etc.).
