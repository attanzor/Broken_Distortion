#pragma once

#include <JuceHeader.h>

#include <cstdint>
#include <limits>

namespace gravel
{

class SmoothNoise final
{
public:
    static float sample (uint32_t seed, uint32_t channel, float position) noexcept;

private:
    static uint32_t hash (uint32_t value) noexcept;
    static float valueAt (uint32_t seed, uint32_t channel, int latticePoint) noexcept;
};

class Space final
{
public:
    static float valueFor (uint32_t seed, uint32_t parameterIndex, float position) noexcept;
};

} // namespace gravel
