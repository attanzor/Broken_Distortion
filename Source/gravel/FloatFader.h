#pragma once

#include <JuceHeader.h>

#include <functional>

namespace gravel
{

class FloatFader final : public juce::Component
{
public:
    FloatFader (const juce::String& labelText,
                juce::Colour railColour   = juce::Colour (0xffd5d0b8),
                juce::Colour handleColour = juce::Colour (0xffef8b48));
    ~FloatFader() override = default;

    void paint (juce::Graphics&) override;
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;

    void setValue (float normalized);
    float getValue() const { return value; }

    std::function<void(float normalized)> onValueChanged;

private:
    juce::String label;
    juce::Colour rail;
    juce::Colour handle;
    float value = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FloatFader)
};

} // namespace gravel
