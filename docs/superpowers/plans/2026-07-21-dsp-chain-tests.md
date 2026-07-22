# DSP Chain Tests — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Add JUCE UnitTest-based tests for the Gravel DSP chain, compiled into the plugin binary.

**Architecture:** 4 test files + 1 shared utility file under `Source/tests/`, added to `target_sources()` in CMakeLists.txt. Tests auto-register with JUCE's UnitTest system and run from AudioPluginHost's test menu.

**Tech Stack:** C++20, JUCE 8 UnitTest, no external test framework.

## Global Constraints

- Tests compile into the plugin binary (no separate test target)
- Don't touch `Source/gravel/` — the reusable library stays clean
- Test files in `Source/tests/`
- No new dependencies

---

### Task 1: TestUtilities — shared helpers

**Files:**
- Create: `Source/tests/TestUtilities.h`
- Create: `Source/tests/TestUtilities.cpp`

**Interfaces:**
- Produces: `fillSine()`, `fillMultiSine()`, `fillNoise()`, `fillImpulse()`, `bufferMatches()`, `bufferMatchesWithLatency()`, `getRMS()`

- [ ] **Step 1: Write TestUtilities.h**

```cpp
#pragma once

#include <JuceHeader.h>

#include <vector>

void fillSine (juce::AudioBuffer<float>& buffer, float freq, float sampleRate, int numSamples);
void fillMultiSine (juce::AudioBuffer<float>& buffer, const std::vector<float>& freqs, float sampleRate, int numSamples);
void fillNoise (juce::AudioBuffer<float>& buffer, int seed);
void fillImpulse (juce::AudioBuffer<float>& buffer, int sampleIndex);

bool bufferMatches (const juce::AudioBuffer<float>& expected,
                    const juce::AudioBuffer<float>& actual,
                    float tolerance);

bool bufferMatchesWithLatency (const juce::AudioBuffer<float>& input,
                               const juce::AudioBuffer<float>& output,
                               int latencySamples,
                               float tolerance);

float getRMS (const juce::AudioBuffer<float>& buffer);
```

- [ ] **Step 2: Write TestUtilities.cpp**

```cpp
#include "TestUtilities.h"

#include <cmath>
#include <random>

void fillSine (juce::AudioBuffer<float>& buffer, float freq, float sampleRate, int numSamples)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        for (int s = 0; s < numSamples; ++s)
            buffer.setSample (ch, s, std::sin (2.0f * juce::MathConstants<float>::pi * freq * static_cast<float>(s) / sampleRate));
}

void fillMultiSine (juce::AudioBuffer<float>& buffer, const std::vector<float>& freqs, float sampleRate, int numSamples)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        for (int s = 0; s < numSamples; ++s)
        {
            auto val = 0.0f;
            for (auto f : freqs)
                val += std::sin (2.0f * juce::MathConstants<float>::pi * f * static_cast<float>(s) / sampleRate);
            buffer.setSample (ch, s, val / static_cast<float>(freqs.size()));
        }
    }
}

void fillNoise (juce::AudioBuffer<float>& buffer, int seed)
{
    std::mt19937 rng (static_cast<unsigned>(seed));
    std::uniform_real_distribution<float> dist (-1.0f, 1.0f);
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        for (int s = 0; s < buffer.getNumSamples(); ++s)
            buffer.setSample (ch, s, dist (rng));
}

void fillImpulse (juce::AudioBuffer<float>& buffer, int sampleIndex)
{
    buffer.clear();
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.setSample (ch, sampleIndex, 1.0f);
}

bool bufferMatches (const juce::AudioBuffer<float>& expected,
                    const juce::AudioBuffer<float>& actual,
                    float tolerance)
{
    if (expected.getNumSamples() != actual.getNumSamples())
        return false;
    if (expected.getNumChannels() != actual.getNumChannels())
        return false;

    for (int ch = 0; ch < expected.getNumChannels(); ++ch)
    {
        for (int s = 0; s < expected.getNumSamples(); ++s)
        {
            auto diff = std::abs (expected.getSample (ch, s) - actual.getSample (ch, s));
            if (diff > tolerance)
                return false;
        }
    }
    return true;
}

bool bufferMatchesWithLatency (const juce::AudioBuffer<float>& input,
                               const juce::AudioBuffer<float>& output,
                               int latencySamples,
                               float tolerance)
{
    auto numSamples = input.getNumSamples();
    if (output.getNumSamples() < latencySamples + numSamples)
        return false;

    for (int ch = 0; ch < input.getNumChannels(); ++ch)
    {
        for (int s = 0; s < numSamples; ++s)
        {
            auto outIdx = s + latencySamples;
            auto diff = std::abs (input.getSample (ch, s) - output.getSample (ch, outIdx));
            if (diff > tolerance)
                return false;
        }
    }
    return true;
}

float getRMS (const juce::AudioBuffer<float>& buffer)
{
    auto sumSq = 0.0;
    auto count = 0;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        for (int s = 0; s < buffer.getNumSamples(); ++s)
        {
            auto val = buffer.getSample (ch, s);
            sumSq += val * val;
            ++count;
        }
    return (count > 0) ? std::sqrt (sumSq / static_cast<double>(count)) : 0.0f;
}
```

- [ ] **Step 3: Verify compilation**
  (Done after all tasks are added to CMakeLists.txt)

---

### Task 2: CrossoverSumTest

**Files:**
- Create: `Source/tests/CrossoverSumTest.cpp`

**Interfaces:**
- Consumes: `fillNoise`, `fillMultiSine`, `getRMS` from TestUtilities
- Tests: `juce::dsp::LinkwitzRileyFilter<float>` lowpass + allpass + highpass sum

- [ ] **Step 1: Write CrossoverSumTest.cpp**

```cpp
#include "TestUtilities.h"

#include <JuceHeader.h>

#include <cmath>

class CrossoverSumTest final : public juce::UnitTest
{
public:
    CrossoverSumTest() : juce::UnitTest ("Crossover Sum", "Gravel DSP") {}

    void runTest() override
    {
        const auto sampleRate = 48000.0;
        const int blockSize = 4096;

        runTestWithFreqs (sampleRate, blockSize, 300.0f, 3000.0f, "default freqs");
        runTestWithFreqs (sampleRate, blockSize, 20.0f, 30.0f, "min spread");
        runTestWithFreqs (sampleRate, blockSize, 8000.0f, 19000.0f, "max spread");
    }

private:
    void runTestWithFreqs (double sampleRate, int blockSize, float crossLow, float crossMid, const char* label)
    {
        juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32>(blockSize), 1 };

        juce::dsp::LinkwitzRileyFilter<float> lowpass;
        juce::dsp::LinkwitzRileyFilter<float> allpass;
        juce::dsp::LinkwitzRileyFilter<float> highpass;

        lowpass.prepare (spec);
        allpass.prepare (spec);
        highpass.prepare (spec);

        lowpass.setType (juce::dsp::LinkwitzRileyFilter<float>::Type::lowpass);
        lowpass.setCutoffFrequency (crossLow);
        allpass.setType (juce::dsp::LinkwitzRileyFilter<float>::Type::allpass);
        allpass.setCutoffFrequency (crossLow);
        highpass.setType (juce::dsp::LinkwitzRileyFilter<float>::Type::highpass);
        highpass.setCutoffFrequency (crossMid);

        // Test 1: White noise — RMS of sum ≈ RMS of input
        {
            beginTest ("noise RMS — " + juce::String (label));

            juce::AudioBuffer<float> input (1, blockSize);
            fillNoise (input, 42);

            juce::AudioBuffer<float> lowBuf (1, blockSize);
            juce::AudioBuffer<float> midBuf (1, blockSize);
            juce::AudioBuffer<float> highBuf (1, blockSize);
            lowBuf.copyFrom (0, 0, input, 0, 0, blockSize);
            midBuf.copyFrom (0, 0, input, 0, 0, blockSize);
            highBuf.copyFrom (0, 0, input, 0, 0, blockSize);

            {
                juce::dsp::AudioBlock<float> block (lowBuf);
                juce::dsp::ProcessContextReplacing<float> ctx (block);
                lowpass.process (ctx);
            }
            {
                juce::dsp::AudioBlock<float> block (midBuf);
                juce::dsp::ProcessContextReplacing<float> ctx (block);
                allpass.process (ctx);
            }
            {
                juce::dsp::AudioBlock<float> block (highBuf);
                juce::dsp::ProcessContextReplacing<float> ctx (block);
                highpass.process (ctx);
            }

            // mid = allpass - lowpass - highpass
            for (int s = 0; s < blockSize; ++s)
            {
                auto low = lowBuf.getSample (0, s);
                auto mid = midBuf.getSample (0, s);
                auto high = highBuf.getSample (0, s);
                midBuf.setSample (0, s, mid - low - high);
            }

            // Sum bands
            juce::AudioBuffer<float> sumBuf (1, blockSize);
            sumBuf.clear();
            for (int s = 0; s < blockSize; ++s)
            {
                auto sum = lowBuf.getSample (0, s) + midBuf.getSample (0, s) + highBuf.getSample (0, s);
                sumBuf.setSample (0, s, sum);
            }

            auto inputRms = getRMS (input);
            auto sumRms = getRMS (sumBuf);

            expectWithinAbsoluteError (inputRms, sumRms, 0.02f,
                "RMS mismatch: input=" + juce::String (inputRms) + " sum=" + juce::String (sumRms));
        }

        // Test 2: Multi-sine — per-band energy distribution
        {
            beginTest ("band energy distribution — " + juce::String (label));

            juce::AudioBuffer<float> input (1, blockSize);
            fillMultiSine (input, { 100.0f, 1500.0f, 8000.0f }, static_cast<float>(sampleRate), blockSize);

            juce::AudioBuffer<float> lowBuf (1, blockSize);
            juce::AudioBuffer<float> midBuf (1, blockSize);
            juce::AudioBuffer<float> highBuf (1, blockSize);
            lowBuf.copyFrom (0, 0, input, 0, 0, blockSize);
            midBuf.copyFrom (0, 0, input, 0, 0, blockSize);
            highBuf.copyFrom (0, 0, input, 0, 0, blockSize);

            // Reset filters
            lowpass.reset();
            allpass.reset();
            highpass.reset();

            {
                juce::dsp::AudioBlock<float> block (lowBuf);
                juce::dsp::ProcessContextReplacing<float> ctx (block);
                lowpass.process (ctx);
            }
            {
                juce::dsp::AudioBlock<float> block (midBuf);
                juce::dsp::ProcessContextReplacing<float> ctx (block);
                allpass.process (ctx);
            }
            {
                juce::dsp::AudioBlock<float> block (highBuf);
                juce::dsp::ProcessContextReplacing<float> ctx (block);
                highpass.process (ctx);
            }

            for (int s = 0; s < blockSize; ++s)
            {
                auto low = lowBuf.getSample (0, s);
                auto mid = midBuf.getSample (0, s);
                auto high = highBuf.getSample (0, s);
                midBuf.setSample (0, s, mid - low - high);
            }

            juce::AudioBuffer<float> sumBuf (1, blockSize);
            sumBuf.clear();
            for (int s = 0; s < blockSize; ++s)
            {
                auto sum = lowBuf.getSample (0, s) + midBuf.getSample (0, s) + highBuf.getSample (0, s);
                sumBuf.setSample (0, s, sum);
            }

            auto inputRms = getRMS (input);
            auto sumRms = getRMS (sumBuf);
            auto lowRms = getRMS (lowBuf);
            auto midRms = getRMS (midBuf);
            auto highRms = getRMS (highBuf);

            expectWithinAbsoluteError (inputRms, sumRms, 0.02f,
                "Sum RMS mismatch: input=" + juce::String (inputRms) + " sum=" + juce::String (sumRms));

            // Verify each band has non-trivial energy (sines are spread across bands)
            expect (lowRms > 0.001f, "Low band has no energy");
            expect (midRms > 0.001f, "Mid band has no energy");
            expect (highRms > 0.001f, "High band has no energy");
        }
    }
};

static CrossoverSumTest crossoverSumTest;
```

---

### Task 3: FftRoundTripTest

**Files:**
- Create: `Source/tests/FftRoundTripTest.cpp`

- [ ] **Step 1: Write FftRoundTripTest.cpp**

```cpp
#include "TestUtilities.h"

#include <JuceHeader.h>

#include <complex>
#include <cmath>

class FftRoundTripTest final : public juce::UnitTest
{
public:
    FftRoundTripTest() : juce::UnitTest ("FFT Round Trip", "Gravel DSP") {}

    void runTest() override
    {
        // Test Hann window COLA property: overlapping windows sum to unity
        testColaProperty();

        // Test FFT identity: FFT → IFFT reconstructs original signal
        testFftIdentity();

        // Test overlap-add: Hann window → FFT → IFFT → overlap-add reconstructs original
        testOverlapAddRoundTrip();
    }

private:
    void testColaProperty()
    {
        beginTest ("Hann window COLA");

        constexpr int fftSize = 1024;
        constexpr int hopSize = fftSize / 4;

        std::array<float, fftSize> window;
        for (int i = 0; i < fftSize; ++i)
            window[i] = 0.5f * (1.0f - std::cos (2.0f * juce::MathConstants<float>::pi * static_cast<float>(i) / static_cast<float>(fftSize - 1)));

        // Sum overlapping windows: for each position, sum of all window contributions
        constexpr int testLen = fftSize * 4;
        std::vector<float> sum (testLen, 0.0f);
        for (int pos = 0; pos < testLen; pos += hopSize)
            for (int i = 0; i < fftSize; ++i)
                if (pos + i < testLen)
                    sum[static_cast<size_t>(pos + i)] += window[i];

        // COLA means the sum should be constant (except near edges)
        float minVal = sum[hopSize];
        float maxVal = sum[hopSize];
        for (size_t i = static_cast<size_t>(hopSize); i < sum.size() - static_cast<size_t>(hopSize); ++i)
        {
            minVal = std::min (minVal, sum[i]);
            maxVal = std::max (maxVal, sum[i]);
        }

        expectWithinAbsoluteError (maxVal, minVal, 1e-5f,
            "Sum not constant: max=" + juce::String (maxVal) + " min=" + juce::String (minVal));
    }

    void testFftIdentity()
    {
        beginTest ("FFT → IFFT identity");

        constexpr int fftOrder = 10;
        constexpr int fftSize = 1 << fftOrder;

        juce::dsp::FFT fft (fftOrder);

        std::array<std::complex<float>, fftSize> data;

        // Fill with a mix of frequencies
        for (int i = 0; i < fftSize; ++i)
            data[static_cast<size_t>(i)] = std::sin (2.0f * juce::MathConstants<float>::pi * 5.0f * static_cast<float>(i) / static_cast<float>(fftSize))
                                          + 0.5f * std::sin (2.0f * juce::MathConstants<float>::pi * 17.0f * static_cast<float>(i) / static_cast<float>(fftSize));

        auto original = data;

        fft.perform (data.data(), data.data(), false);
        fft.perform (data.data(), data.data(), true);

        float maxDiff = 0.0f;
        float scale = 1.0f / static_cast<float>(fftSize);
        for (size_t i = 0; i < fftSize; ++i)
        {
            auto diff = std::abs (data[i].real() * scale - original[i].real());
            maxDiff = std::max (maxDiff, diff);
        }

        expect (maxDiff < 1e-5f, "FFT → IFFT identity failed, maxDiff=" + juce::String (maxDiff));
    }

    void testOverlapAddRoundTrip()
    {
        beginTest ("Overlap-add round-trip");

        constexpr int fftOrder = 10;
        constexpr int fftSize = 1 << fftOrder;
        constexpr int hopSize = fftSize / 4;
        constexpr int numBlocks = 8;

        juce::dsp::FFT fft (fftOrder);

        std::array<float, fftSize> window;
        for (int i = 0; i < fftSize; ++i)
            window[i] = 0.5f * (1.0f - std::cos (2.0f * juce::MathConstants<float>::pi * static_cast<float>(i) / static_cast<float>(fftSize - 1)));

        // Generate input signal
        const int totalSamples = hopSize * numBlocks;
        std::vector<float> input (static_cast<size_t>(totalSamples));
        for (int i = 0; i < totalSamples; ++i)
            input[static_cast<size_t>(i)] = std::sin (2.0f * juce::MathConstants<float>::pi * 100.0f * static_cast<float>(i) / 48000.0f);

        std::vector<float> output (static_cast<size_t>(totalSamples), 0.0f);
        std::vector<float> ringBuf (static_cast<size_t>(fftSize), 0.0f);
        size_t writePos = 0;

        for (int block = 0; block < numBlocks; ++block)
        {
            for (int i = 0; i < hopSize; ++i)
                ringBuf[writePos++] = input[static_cast<size_t>(block * hopSize + i)];

            if (writePos >= static_cast<size_t>(hopSize))
            {
                std::array<std::complex<float>, fftSize> frame;
                for (int i = 0; i < fftSize; ++i)
                    frame[i] = ringBuf[static_cast<size_t>(i)] * window[i];

                fft.perform (frame.data(), frame.data(), false);
                // No bin modification (identity)
                fft.perform (frame.data(), frame.data(), true);

                for (int i = 0; i < fftSize; ++i)
                {
                    auto idx = static_cast<size_t>(block * hopSize + i);
                    if (idx < output.size())
                        output[idx] += frame[i].real() * window[i];
                }

                // Shift ring buffer
                for (int i = 0; i < fftSize - hopSize; ++i)
                    ringBuf[static_cast<size_t>(i)] = ringBuf[static_cast<size_t>(i + hopSize)];

                writePos -= hopSize;
            }
        }

        // Compare output to input (accounting for processing delay at start)
        const int steadyStateStart = hopSize * 2; // skip initial transient
        const int compareLen = totalSamples - steadyStateStart;

        float maxDiff = 0.0f;
        for (int i = 0; i < compareLen; ++i)
        {
            auto inIdx = static_cast<size_t>(steadyStateStart + i);
            auto diff = std::abs (input[inIdx] - output[inIdx]);
            maxDiff = std::max (maxDiff, diff);
        }

        expect (maxDiff < 1e-2f, "Overlap-add round-trip failed, maxDiff=" + juce::String (maxDiff));
    }
};

static FftRoundTripTest fftRoundTripTest;
```

---

### Task 4: NoiseDeterminismTest

**Files:**
- Create: `Source/tests/NoiseDeterminismTest.cpp`

- [ ] **Step 1: Write NoiseDeterminismTest.cpp**

```cpp
#include <JuceHeader.h>

#include "Source/gravel/PossibilitySpace.h"

class NoiseDeterminismTest final : public juce::UnitTest
{
public:
    NoiseDeterminismTest() : juce::UnitTest ("Noise Determinism", "Gravel DSP") {}

    void runTest() override
    {
        beginTest ("SmoothNoise same seed = same sample");

        auto a = gravel::SmoothNoise::sample (42, 0, 0.5f);
        auto b = gravel::SmoothNoise::sample (42, 0, 0.5f);
        expectEquals (a, b, "Same seed produced different values");

        beginTest ("SmoothNoise different seed = different sample");

        auto c = gravel::SmoothNoise::sample (99, 0, 0.5f);
        expect (a != c, "Different seeds produced same value");

        beginTest ("SmoothNoise different position = different sample");

        auto d = gravel::SmoothNoise::sample (42, 0, 0.7f);
        expect (a != d, "Different positions produced same value");

        beginTest ("Space valueFor range");

        for (uint32_t seed = 0; seed < 100; ++seed)
        {
            for (uint32_t field = 0; field < 5; ++field)
            {
                auto val = gravel::Space::valueFor (seed, field, 0.5f);
                expect (val >= 0.0f && val <= 1.0f,
                    "valueFor out of range: " + juce::String (val));
            }
        }

        beginTest ("Space valueFor determinism");

        auto e = gravel::Space::valueFor (42, 1, 0.3f);
        auto f = gravel::Space::valueFor (42, 1, 0.3f);
        expectEquals (e, f, "Same params produced different values");

        beginTest ("Space valueFor different field = different value");

        auto g = gravel::Space::valueFor (42, 2, 0.3f);
        expect (e != g, "Different fields produced same value");
    }
};

static NoiseDeterminismTest noiseDeterminismTest;
```

---

### Task 5: ParameterMapTest

**Files:**
- Create: `Source/tests/ParameterMapTest.cpp`

- [ ] **Step 1: Write ParameterMapTest.cpp**

```cpp
#include <JuceHeader.h>

#include "Source/gravel/ParameterSpace.h"

class ParameterMapTest final : public juce::UnitTest
{
public:
    ParameterMapTest() : juce::UnitTest ("Parameter Map", "Gravel DSP") {}

    void runTest() override
    {
        beginTest ("ParameterSpace mapping and determinism");

        juce::AudioProcessorValueTreeState apvts (
            nullptr, nullptr, "test",
            { std::make_unique<juce::AudioParameterFloat> ("seed", "Seed", 0.0f, 1.0f, 0.5f),
              std::make_unique<juce::AudioParameterFloat> ("possibility", "Possibility", 0.0f, 1.0f, 0.5f),
              std::make_unique<juce::AudioParameterFloat> ("test_param", "Test", 0.0f, 1.0f, 0.0f) });

        gravel::ParameterSpace space (apvts);
        space.mapParameter ("test_param", 0);

        // Same seed, same possibility → same value
        *apvts.getRawParameterValue ("seed") = 0.25f;
        *apvts.getRawParameterValue ("possibility") = 0.5f;
        space.updateAll();
        auto val1 = apvts.getRawParameterValue ("test_param")->load();

        *apvts.getRawParameterValue ("seed") = 0.25f;
        *apvts.getRawParameterValue ("possibility") = 0.5f;
        space.updateAll();
        auto val2 = apvts.getRawParameterValue ("test_param")->load();

        expectEquals (val1, val2, "Same seed+possibility produced different values");

        // Different seed → different value
        *apvts.getRawParameterValue ("seed") = 0.75f;
        *apvts.getRawParameterValue ("possibility") = 0.5f;
        space.updateAll();
        auto val3 = apvts.getRawParameterValue ("test_param")->load();

        expect (val1 != val3, "Different seeds produced same value");

        // Value stays in [0, 1]
        expect (val1 >= 0.0f && val1 <= 1.0f, "Value out of range: " + juce::String (val1));
        expect (val3 >= 0.0f && val3 <= 1.0f, "Value out of range: " + juce::String (val3));
    }
};

static ParameterMapTest parameterMapTest;
```

---

### Task 6: Update CMakeLists.txt

**Files:**
- Modify: `CMakeLists.txt` — add test sources

- [ ] **Step 1: Add test sources to target_sources**

```cmake
target_sources(GravelPossibilityDistortion PRIVATE
    # ... existing sources ...
    Source/tests/TestUtilities.cpp
    Source/tests/TestUtilities.h
    Source/tests/CrossoverSumTest.cpp
    Source/tests/FftRoundTripTest.cpp
    Source/tests/NoiseDeterminismTest.cpp
    Source/tests/ParameterMapTest.cpp
)
```

- [ ] **Step 2: Add JUCE_UNIT_TESTS compile definition**

```cmake
target_compile_definitions(GravelPossibilityDistortion PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_UNIT_TESTS=1
)
```

- [ ] **Step 3: Build and verify**

```bash
cmake --build build --target GravelPossibilityDistortion_All
```
