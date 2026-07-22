#pragma once

#include <JuceHeader.h>

#include <array>
#include <atomic>
#include <complex>
#include <vector>

namespace dsp
{

class SpectralGridProcessor final
{
public:
    static constexpr int kNumBands = 3;
    static constexpr int kNumLayers = 3;
    static constexpr int kNumCells = kNumBands * kNumLayers; // 9
    static constexpr int kWaveTableSize = 513;
    static constexpr int kNumBaseCurves = 9;
    static constexpr int kFftOrder = 10; // 1024
    static constexpr int kFftSize = 1 << kFftOrder;
    static constexpr int kHopSize = kFftSize / 4;
    static constexpr int kFftHalfBins = kFftSize / 2 + 1;

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
    float getCrossoverNorm (int bandIndex) const;
    void setBandLevel (int bandIndex, float level);
    void setCellShape (int cellIndex, float morph);
    void setCellDrive (int cellIndex, float drive);
    void setCellBias (int cellIndex, float bias);
    void setThresholdLine (int lineIndex, float value);
    float getThresholdLine (int lineIndex) const;
    float getRawThreshold (int lineIndex) const;
    float getRawCrossNorm (int bandIndex) const;
    void setSeed (uint32_t seed);
    void setEqBypass (bool bypass);
    void setBypassWaveshapers (bool bypass);
    void setBypassAmplitudeSplit (bool bypass);
    void setBypassFrequencySplit (bool bypass);
    std::array<float, kWaveTableSize> getCellShapeData (int cellIndex) const;
    const std::array<float, kNumCells>& getCellActivity() const { return cellActivity; }
    void setHysteresisDepth (int band, float depth);

private:
    struct EQStage
    {
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter;
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

    std::array<WaveTable, kNumBaseCurves> baseCurves;
    std::array<std::array<int, kNumBaseCurves>, 9> cellPermutations;

    uint32_t currentSeed = 1024;
    double sampleRate = 48000.0;

    static constexpr int kMaxChannels = 2;

    juce::dsp::FFT fft { kFftOrder };
    std::array<std::array<std::vector<float>, kMaxChannels>, 3> fftTimeDomain;
    std::array<std::array<std::vector<std::complex<float>>, kMaxChannels>, 3> fftFreqDomain;
    std::array<std::array<std::vector<float>, kMaxChannels>, 3> fftOutputBuffer;
    std::array<float, kFftSize> hannWindow {};
    std::array<std::array<size_t, kMaxChannels>, 3> fftWritePos {};
    std::array<std::array<size_t, kMaxChannels>, 3> fftOlaOffset {};

    bool eqBypassed = false;
    bool bypassWaveshapers = false;
    bool bypassAmplitudeSplit = false;
    bool bypassFrequencySplit = false;
    std::atomic<float> thresholdLow { 0.33f };
    std::atomic<float> thresholdHigh { 0.66f };
    float rawThresholdLow = 0.33f;
    float rawThresholdHigh = 0.66f;
    float rawCrossLowNorm = 0.392f;
    float rawCrossHighNorm = 0.7255f;
    std::array<float, kNumCells> cellActivity {};

    std::array<float, kNumCells> cellDrives { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    std::array<float, kNumCells> cellBiases {};
    std::array<float, kNumBands> hysteresisDepth {};
    std::array<std::array<std::array<float, kFftHalfBins>, kMaxChannels>, kNumBands> prevNormMag {};

    void rebuildBaseCurves();
    void rebuildCellPermutations();
    void updateCellWavetable (int cellIndex);
    float lookupWaveShape (const WaveTable& table, float input) const;
    void applyEq (juce::AudioBuffer<float>& buffer, const std::array<EQStage, 3>& stages);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralGridProcessor)
};

} // namespace dsp
