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
        juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (blockSize), 1 };

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

        // White noise — RMS of sum ≈ RMS of input
        {
            beginTest (juce::String ("noise RMS — ") + label);

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
                sumBuf.setSample (0, s, lowBuf.getSample (0, s) + midBuf.getSample (0, s) + highBuf.getSample (0, s));

            auto inputRms = getRMS (input);
            auto sumRms = getRMS (sumBuf);

            expectWithinAbsoluteError (inputRms, sumRms, 0.02f,
                juce::String ("RMS mismatch: input=") + juce::String (inputRms) + " sum=" + juce::String (sumRms));
        }

        // Multi-sine — per-band energy distribution
        {
            beginTest (juce::String ("band energy — ") + label);

            juce::AudioBuffer<float> input (1, blockSize);
            fillMultiSine (input, { 100.0f, 1500.0f, 8000.0f }, static_cast<float> (sampleRate), blockSize);

            juce::AudioBuffer<float> lowBuf (1, blockSize);
            juce::AudioBuffer<float> midBuf (1, blockSize);
            juce::AudioBuffer<float> highBuf (1, blockSize);
            lowBuf.copyFrom (0, 0, input, 0, 0, blockSize);
            midBuf.copyFrom (0, 0, input, 0, 0, blockSize);
            highBuf.copyFrom (0, 0, input, 0, 0, blockSize);

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
                sumBuf.setSample (0, s, lowBuf.getSample (0, s) + midBuf.getSample (0, s) + highBuf.getSample (0, s));

            auto inputRms = getRMS (input);
            auto sumRms = getRMS (sumBuf);

            expectWithinAbsoluteError (inputRms, sumRms, 0.02f,
                juce::String ("Sum RMS mismatch: input=") + juce::String (inputRms) + " sum=" + juce::String (sumRms));

            expect (getRMS (lowBuf) > 0.001f, "Low band has no energy");
            expect (getRMS (midBuf) > 0.001f, "Mid band has no energy");
            expect (getRMS (highBuf) > 0.001f, "High band has no energy");
        }
    }
};

static CrossoverSumTest crossoverSumTest;
