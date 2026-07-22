#include <JuceHeader.h>

#include "../gravel/PossibilitySpace.h"

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
            for (uint32_t field = 0; field < 5; ++field)
            {
                auto val = gravel::Space::valueFor (seed, field, 0.5f);
                expect (val >= 0.0f && val <= 1.0f,
                    juce::String ("valueFor out of range: ") + juce::String (val));
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
