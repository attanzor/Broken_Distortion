#pragma once

#include <JuceHeader.h>

#include <atomic>

namespace gravel
{

struct ModulationState final
{
    std::atomic<float> envRise   { 100.0f };
    std::atomic<float> envFall   { 100.0f };
    std::atomic<float> envGain   { 5.0f };
    std::atomic<float> explMin   { 0.0f };
    std::atomic<float> explMax   { 1.0f };
    std::atomic<bool>  explEnabled { true };
    std::atomic<float> modulationAmount { 0.0f };
    std::atomic<float> manualModulation { 0.0f };
    std::atomic<float> appliedModulation { 0.0f };
    std::atomic<bool>  explModEnabled { false };
    std::atomic<float> slopeSpeed    { 10.0f };
    std::atomic<float> slopeSpikyness { 1.0f };
    std::atomic<float> slopeAmount   { 0.0f };
    std::atomic<float> slopeApplied  { 0.0f };
    std::atomic<bool>  slopeEnabled  { true };
    std::atomic<float> jitterSpeed   { 1.0f };
    std::atomic<float> jitterSpikyness { 1.0f };
    std::atomic<float> jitterAmount  { 0.0f };
    std::atomic<float> jitterApplied { 0.0f };
    std::atomic<bool>  jitterEnabled { true };
    std::atomic<bool>  slopeExplModEnabled { false };
    std::atomic<bool>  jitterExplModEnabled { false };
    std::atomic<bool>  envModAmountExplEnabled { false };
    std::atomic<bool>  slopeModAmountExplEnabled { false };
    std::atomic<bool>  jitterModAmountExplEnabled { false };

    juce::ValueTree toValueTree (const juce::String& name) const
    {
        juce::ValueTree tree (name);
        tree.setProperty ("envRise",   envRise.load(),   nullptr);
        tree.setProperty ("envFall",   envFall.load(),   nullptr);
        // ponytail: envGain not serialized — always defaults to max (10.0f)
        tree.setProperty ("explMin",   explMin.load(),   nullptr);
        tree.setProperty ("explMax",   explMax.load(),   nullptr);
        tree.setProperty ("explEnabled", explEnabled.load(), nullptr);
        tree.setProperty ("modulationAmount", modulationAmount.load(), nullptr);
        tree.setProperty ("explModEnabled", explModEnabled.load(), nullptr);
        tree.setProperty ("slopeSpeed",    slopeSpeed.load(),    nullptr);
        tree.setProperty ("slopeSpikyness", slopeSpikyness.load(), nullptr);
        tree.setProperty ("slopeAmount",   slopeAmount.load(),   nullptr);
        tree.setProperty ("slopeEnabled",  slopeEnabled.load(),  nullptr);
        tree.setProperty ("jitterSpeed",   jitterSpeed.load(),   nullptr);
        tree.setProperty ("jitterSpikyness", jitterSpikyness.load(), nullptr);
        tree.setProperty ("jitterAmount",  jitterAmount.load(),  nullptr);
        tree.setProperty ("jitterEnabled", jitterEnabled.load(), nullptr);
        tree.setProperty ("slopeExplModEnabled", slopeExplModEnabled.load(), nullptr);
        tree.setProperty ("jitterExplModEnabled", jitterExplModEnabled.load(), nullptr);
        tree.setProperty ("envModAmountExplEnabled", envModAmountExplEnabled.load(), nullptr);
        tree.setProperty ("slopeModAmountExplEnabled", slopeModAmountExplEnabled.load(), nullptr);
        tree.setProperty ("jitterModAmountExplEnabled", jitterModAmountExplEnabled.load(), nullptr);
        return tree;
    }

    void fromValueTree (const juce::ValueTree& tree)
    {
        envRise.store (tree.getProperty ("envRise", 100.0f));
        envFall.store (tree.getProperty ("envFall", 100.0f));
        // ponytail: envGain not serialized — always defaults to max (10.0f)
        explMin.store (tree.getProperty ("explMin", 0.0f));
        explMax.store (tree.getProperty ("explMax", 1.0f));
        explEnabled.store (static_cast<bool> (tree.getProperty ("explEnabled", true)));
        modulationAmount.store (tree.getProperty ("modulationAmount", 0.0f));
        explModEnabled.store (static_cast<bool> (tree.getProperty ("explModEnabled", true)));
        slopeSpeed.store   (tree.getProperty ("slopeSpeed", 10.0f));
        slopeSpikyness.store (tree.getProperty ("slopeSpikyness", 1.0f));
        slopeAmount.store  (tree.getProperty ("slopeAmount", 0.0f));
        slopeEnabled.store (static_cast<bool> (tree.getProperty ("slopeEnabled", true)));
        jitterSpeed.store  (tree.getProperty ("jitterSpeed", 1.0f));
        jitterSpikyness.store (tree.getProperty ("jitterSpikyness", 1.0f));
        jitterAmount.store (tree.getProperty ("jitterAmount", 0.0f));
        jitterEnabled.store (static_cast<bool> (tree.getProperty ("jitterEnabled", true)));
        slopeExplModEnabled.store (static_cast<bool> (tree.getProperty ("slopeExplModEnabled", false)));
        jitterExplModEnabled.store (static_cast<bool> (tree.getProperty ("jitterExplModEnabled", false)));
        envModAmountExplEnabled.store (static_cast<bool> (tree.getProperty ("envModAmountExplEnabled", false)));
        slopeModAmountExplEnabled.store (static_cast<bool> (tree.getProperty ("slopeModAmountExplEnabled", false)));
        jitterModAmountExplEnabled.store (static_cast<bool> (tree.getProperty ("jitterModAmountExplEnabled", false)));
    }
};

} // namespace gravel
