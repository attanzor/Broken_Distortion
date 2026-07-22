#include "ParameterSpace.h"

namespace gravel
{

ParameterSpace::ParameterSpace (juce::AudioProcessorValueTreeState& a,
                                const juce::String& sID,
                                const juce::String& pID)
    : apvts (a), seedID (sID), possibilityID (pID) {}

void ParameterSpace::mapParameter (const juce::String& paramID, uint32_t fieldIndex)
{
    for (const auto& m : mappings)
        if (m.paramID == paramID)
            return;

    SpaceMapping mapping;
    mapping.paramID = paramID;
    mapping.fieldIndex = fieldIndex;
    mappings.push_back (mapping);
}

void ParameterSpace::mapParameters (std::initializer_list<SpaceMapping> newMappings)
{
    for (const auto& mapping : newMappings)
    {
        for (const auto& m : mappings)
            if (m.paramID == mapping.paramID)
                goto next;

        mappings.push_back (mapping);

        next:;
    }
}

void ParameterSpace::setExploration (const juce::String& paramID, float explMin, float explMax, bool explEnabled)
{
    for (auto& m : mappings)
    {
        if (m.paramID == paramID)
        {
            m.explMin = explMin;
            m.explMax = explMax;
            m.explEnabled = explEnabled;
            return;
        }
    }
}

bool ParameterSpace::isManaged (const juce::String& paramID) const
{
    for (const auto& m : mappings)
        if (m.paramID == paramID)
            return true;

    return false;
}

bool ParameterSpace::isSeedOrPossibility (const juce::String& paramID) const
{
    return paramID == seedID || paramID == possibilityID;
}

bool ParameterSpace::handleParameterChange (const juce::String& paramID, float)
{
    if (! isSeedOrPossibility (paramID))
        return false;

    if (isUpdating.exchange (true))
        return true;

    if (onBeforeUpdate)
        onBeforeUpdate();

    updateAll();

    if (onAfterUpdate)
        onAfterUpdate();

    isUpdating = false;
    return true;
}

void ParameterSpace::updateAll()
{
    auto* seedParam = apvts.getRawParameterValue (seedID);
    auto* possibilityParam = apvts.getRawParameterValue (possibilityID);

    if (seedParam == nullptr || possibilityParam == nullptr)
        return;

    const auto seed = static_cast<uint32_t> (std::round (seedParam->load()));
    const auto possibility = juce::jlimit (0.0f, 1.0f, possibilityParam->load());

    for (const auto& mapping : mappings)
    {
        if (! mapping.explEnabled)
            continue;

        auto* param = apvts.getParameter (mapping.paramID);
        if (param == nullptr)
            continue;

        float normalized = Space::valueFor (seed, mapping.fieldIndex, possibility);
        normalized = mapping.explMin + (mapping.explMax - mapping.explMin) * normalized;

        param->setValueNotifyingHost (juce::jlimit (0.0f, 1.0f, normalized));
    }
}

} // namespace gravel
