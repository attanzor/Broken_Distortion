#pragma once

#include <JuceHeader.h>

#include <functional>

namespace gravel
{

class SeedField final : public juce::Component
{
public:
    SeedField (int maxDigits = 6,
               int minValue = 0,
               int maxValue = 999999,
               juce::Colour bgColour    = juce::Colour (0xff11100e),
               juce::Colour borderColour = juce::Colour (0xffd5d0b8).withAlpha (0.45f),
               juce::Colour textColour  = juce::Colour (0xffef8b48));

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent&) override;
    bool keyPressed (const juce::KeyPress&) override;
    void focusLost (FocusChangeType) override;

    void setSeed (int seed);
    void finishEditing();
    void cancelEditing();

    std::function<void(int)> onSeedCommitted;

private:
    void commitValue();

    juce::String seedText;
    juce::String editBuffer;
    bool isEditing = false;
    int maxDigits;
    int minValue;
    int maxValue;
    juce::Colour bg;
    juce::Colour border;
    juce::Colour text;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SeedField)
};

} // namespace gravel
