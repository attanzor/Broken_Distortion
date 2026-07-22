#include "PossibilitySpace.h"

namespace gravel
{

uint32_t SmoothNoise::hash (uint32_t value) noexcept
{
    value ^= value >> 16;
    value *= 0x7feb352dU;
    value ^= value >> 15;
    value *= 0x846ca68bU;
    value ^= value >> 16;
    return value;
}

float SmoothNoise::valueAt (uint32_t seed, uint32_t channel, int latticePoint) noexcept
{
    const auto point = static_cast<uint32_t> (latticePoint);
    return static_cast<float> (hash (seed ^ (channel * 0x9e3779b9U) ^ (point * 0x85ebca6bU)))
         / static_cast<float> (std::numeric_limits<uint32_t>::max());
}

float SmoothNoise::sample (uint32_t seed, uint32_t channel, float position) noexcept
{
    const auto left = static_cast<int> (std::floor (position));
    const auto fraction = position - static_cast<float> (left);
    const auto blend = fraction * fraction * fraction * (fraction * (fraction * 6.0f - 15.0f) + 10.0f);
    return juce::jmap (blend, valueAt (seed, channel, left), valueAt (seed, channel, left + 1));
}

float Space::valueFor (uint32_t seed, uint32_t parameterIndex, float position) noexcept
{
    const auto zoom = 3.0f;
    auto evaluateAt = [=] (float samplePosition) noexcept {
        const auto a = SmoothNoise::sample (seed, parameterIndex, samplePosition * zoom);
        const auto b = SmoothNoise::sample (seed + 0x68bc21ebU, parameterIndex, samplePosition * zoom * 2.17f);
        const auto c = SmoothNoise::sample (seed + 0x02e5be93U, parameterIndex, samplePosition * zoom * 4.71f);
        return juce::jlimit (0.0f, 1.0f, a * 0.58f + b * 0.29f + c * 0.13f);
    };

    constexpr int numSamples = 33;
    float minValue = 1.0f;
    float maxValue = 0.0f;

    for (int sample = 0; sample <= numSamples; ++sample)
    {
        const auto samplePosition = static_cast<float> (sample) / static_cast<float> (numSamples);
        const auto sampleValue = evaluateAt (samplePosition);
        minValue = juce::jmin (minValue, sampleValue);
        maxValue = juce::jmax (maxValue, sampleValue);
    }

    if (maxValue <= minValue)
        return 0.5f;

    const auto rawValue = evaluateAt (position);
    return juce::jlimit (0.0f, 1.0f, (rawValue - minValue) / (maxValue - minValue));
}

} // namespace gravel
