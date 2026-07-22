#include <JuceHeader.h>

#include "../gravel/ParameterSpace.h"

// Minimal AudioProcessor needed to create an AudioProcessorValueTreeState in JUCE 8
class TestProcessor final : public juce::AudioProcessor
{
public:
    TestProcessor() : AudioProcessor (BusesProperties()) {}

    void prepareToPlay (double, int) override {}
    void releaseResources() override {}
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override {}
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }
    const juce::String getName() const override { return "Test"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}
    void getStateInformation (juce::MemoryBlock&) override {}
    void setStateInformation (const void*, int) override {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestProcessor)
};

class ParameterMapTest final : public juce::UnitTest
{
public:
    ParameterMapTest() : juce::UnitTest ("Parameter Map", "Gravel DSP") {}

    void runTest() override
    {
        beginTest ("ParameterSpace mapping and determinism");

        TestProcessor testProc;

        juce::AudioProcessorValueTreeState::ParameterLayout layout;
        layout.add (std::make_unique<juce::AudioParameterFloat> ("seed", "Seed", 0.0f, 1.0f, 0.5f));
        layout.add (std::make_unique<juce::AudioParameterFloat> ("possibility", "Possibility", 0.0f, 1.0f, 0.5f));
        layout.add (std::make_unique<juce::AudioParameterFloat> ("test_param", "Test", 0.0f, 1.0f, 0.0f));

        juce::AudioProcessorValueTreeState apvts (testProc, nullptr, "test", std::move (layout));

        gravel::ParameterSpace space (apvts);
        space.mapParameter ("test_param", 0);

        *apvts.getRawParameterValue ("seed") = 0.25f;
        *apvts.getRawParameterValue ("possibility") = 0.5f;
        space.updateAll();
        auto val1 = apvts.getRawParameterValue ("test_param")->load();

        *apvts.getRawParameterValue ("seed") = 0.25f;
        *apvts.getRawParameterValue ("possibility") = 0.5f;
        space.updateAll();
        auto val2 = apvts.getRawParameterValue ("test_param")->load();

        expectEquals (val1, val2, "Same seed+possibility produced different values");

        *apvts.getRawParameterValue ("seed") = 0.75f;
        *apvts.getRawParameterValue ("possibility") = 0.5f;
        space.updateAll();
        auto val3 = apvts.getRawParameterValue ("test_param")->load();

        expect (val1 != val3, "Different seeds produced same value");

        expect (val1 >= 0.0f && val1 <= 1.0f, "Value out of range");
        expect (val3 >= 0.0f && val3 <= 1.0f, "Value out of range");
    }
};

static ParameterMapTest parameterMapTest;
