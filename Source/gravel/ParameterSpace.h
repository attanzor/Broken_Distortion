#pragma once

#include "PossibilitySpace.h"

#include <JuceHeader.h>

#include <functional>
#include <vector>

namespace gravel
{

struct SpaceMapping final
{
    juce::String paramID;
    uint32_t fieldIndex;
    float explMin = 0.0f;
    float explMax = 1.0f;
    bool explEnabled = true;
};

class ParameterSpace final
{
public:
    ParameterSpace (juce::AudioProcessorValueTreeState& apvts,
                    const juce::String& seedParamID = "seed",
                    const juce::String& possibilityParamID = "possibility");

    void mapParameter (const juce::String& paramID, uint32_t fieldIndex);
    void mapParameters (std::initializer_list<SpaceMapping> mappings);
    void setExploration (const juce::String& paramID, float explMin, float explMax, bool explEnabled);
    bool isManaged (const juce::String& paramID) const;
    bool isSeedOrPossibility (const juce::String& paramID) const;

    bool handleParameterChange (const juce::String& paramID, float newValue);

    void updateAll();

    std::function<void()> onBeforeUpdate;
    std::function<void()> onAfterUpdate;

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::String seedID;
    juce::String possibilityID;
    std::vector<SpaceMapping> mappings;
    std::atomic<bool> isUpdating { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSpace)
};

} // namespace gravel
