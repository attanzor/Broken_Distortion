#pragma once

#include "ModulationState.h"

#include <JuceHeader.h>

#include <functional>

namespace gravel
{

class ModulationPopup final : public juce::Component
{
public:
    ModulationPopup (const juce::String& paramName, ModulationState& stateRef);
    ~ModulationPopup() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent&) override;

    std::function<void()> onClose;

    void setColours (juce::Colour overlayColour,
                     juce::Colour panelBg,
                     juce::Colour textColour,
                     juce::Colour accentColour);

private:
    juce::Rectangle<float> getPanelBounds() const;

    ModulationState& state;

    juce::Label titleLabel;

    // Envelope section
    juce::Label envHeader;
    juce::Slider envRiseSlider, envFallSlider, envGainSlider;
    juce::Label envRiseLabel, envFallLabel, envGainLabel;
    juce::Label envExploreLabel;
    juce::ToggleButton envValueToggle;
    juce::ToggleButton envModAmountToggle;
    juce::ToggleButton envSettingsToggle;

    // Jitter section (before slope per user request)
    juce::Label jitterHeader;
    juce::Slider jitterSpeedSlider, jitterSpikeSlider;
    juce::Label jitterSpeedLabel, jitterSpikeLabel;
    juce::ToggleButton jitterToggle;
    juce::Label jitterExploreLabel;
    juce::ToggleButton jitterModAmountToggle;
    juce::ToggleButton jitterSettingsToggle;

    // Slope section
    juce::Label slopeHeader;
    juce::Slider slopeSpeedSlider, slopeSpikeSlider;
    juce::Label slopeSpeedLabel, slopeSpikeLabel;
    juce::ToggleButton slopeToggle;
    juce::Label slopeExploreLabel;
    juce::ToggleButton slopeModAmountToggle;
    juce::ToggleButton slopeSettingsToggle;

    juce::Colour overlay { 0xbb11100e };
    juce::Colour panelBg  { 0xff1a1815 };
    juce::Colour text     { 0xffd5d0b8 };
    juce::Colour accent   { 0xffef8b48 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationPopup)
};

} // namespace gravel
