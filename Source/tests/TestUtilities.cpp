#include "TestUtilities.h"

#include <cmath>
#include <random>

void fillSine (juce::AudioBuffer<float>& buffer, float freq, float sampleRate, int numSamples)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        for (int s = 0; s < numSamples; ++s)
            buffer.setSample (ch, s, std::sin (2.0f * juce::MathConstants<float>::pi * freq * static_cast<float> (s) / sampleRate));
}

void fillMultiSine (juce::AudioBuffer<float>& buffer, const std::vector<float>& freqs, float sampleRate, int numSamples)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        for (int s = 0; s < numSamples; ++s)
        {
            auto val = 0.0f;
            for (auto f : freqs)
                val += std::sin (2.0f * juce::MathConstants<float>::pi * f * static_cast<float> (s) / sampleRate);
            buffer.setSample (ch, s, val / static_cast<float> (freqs.size()));
        }
    }
}

void fillNoise (juce::AudioBuffer<float>& buffer, int seed)
{
    std::mt19937 rng (static_cast<unsigned> (seed));
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
        for (int s = 0; s < expected.getNumSamples(); ++s)
            if (std::abs (expected.getSample (ch, s) - actual.getSample (ch, s)) > tolerance)
                return false;

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
        for (int s = 0; s < numSamples; ++s)
            if (std::abs (input.getSample (ch, s) - output.getSample (ch, latencySamples + s)) > tolerance)
                return false;

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
            sumSq += static_cast<double> (val) * static_cast<double> (val);
            ++count;
        }
    return (count > 0) ? static_cast<float> (std::sqrt (sumSq / static_cast<double> (count))) : 0.0f;
}
