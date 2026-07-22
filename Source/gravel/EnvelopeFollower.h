#pragma once

#include <JuceHeader.h>

#include <cmath>

namespace gravel
{

class EnvelopeFollower final
{
public:
    void prepare (double sr) noexcept
    {
        sampleRate = sr;
        reset();
    }

    void reset() noexcept
    {
        envelope = 0.0f;
    }

    float process (float input) noexcept
    {
        const auto rectified = std::abs (input);
        const auto coeff = rectified > envelope ? riseCoeff : fallCoeff;
        envelope = rectified + coeff * (envelope - rectified);
        return envelope;
    }

    void setCoefficients (float riseMs, float fallMs) noexcept
    {
        riseCoeff = std::exp (-1000.0f / static_cast<float> (sampleRate * static_cast<double> (juce::jmax (1.0f, riseMs))));
        fallCoeff = std::exp (-1000.0f / static_cast<float> (sampleRate * static_cast<double> (juce::jmax (1.0f, fallMs))));
    }

    float getEnvelope() const noexcept { return envelope; }

private:
    double sampleRate = 44100.0;
    float envelope = 0.0f;
    float riseCoeff = 0.0f;
    float fallCoeff = 0.0f;
};

} // namespace gravel
