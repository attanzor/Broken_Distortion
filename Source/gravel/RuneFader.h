#pragma once

#include <JuceHeader.h>

#include <functional>

namespace gravel
{

class RuneFader final : public juce::Component,
                         private juce::Slider::Listener
{
public:
    RuneFader (juce::Slider& slider,
               const juce::String& title,
               bool isPrimary = false,
               juce::Colour railColour   = juce::Colour (0xffd5d0b8),
               juce::Colour handleColour = juce::Colour (0xffef8b48),
                juce::Colour arrowColour  = juce::Colour (0xffef8b48).withAlpha (0.55f));

    ~RuneFader() override;

    void paint (juce::Graphics&) override;
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;

    void setRailColour (juce::Colour colour)   { rail = colour; repaint(); }
    void setHandleColour (juce::Colour colour)  { handle = colour; repaint(); }
    juce::Colour getRailColour() const          { return rail; }
    juce::Colour getHandleColour() const        { return handle; }

    // Orange arrow: the target modulation amount (-1 to 1)
    void setTargetModulation (float amount);
    float getTargetModulation() const { return targetModulation; }

    // White arrow: the real-time envelope-scaled result (-1 to 1)
    void setAppliedModulation (float amount);
    float getAppliedModulation() const { return appliedModulation; }

    // Green arrow (slope)
    void setSlopeModulation (float amount);
    float getSlopeModulation() const { return slopeTarget; }

    // Purple arrow (jitter)
    void setJitterModulation (float amount);
    float getJitterModulation() const { return jitterTarget; }

    void setArrowColour (juce::Colour c) { arrow = c; repaint(); }
    void setSlopeArrowColour (juce::Colour c) { slopeArrow = c; repaint(); }
    void setJitterArrowColour (juce::Colour c) { jitterArrow = c; repaint(); }

    std::function<void()> onInteraction;
    std::function<void()> onCtrlClick;
    std::function<void(float modulationAmount)> onModulationDragged;
    std::function<void(float)> onSlopeDragged;
    std::function<void(float)> onJitterDragged;

private:
    void sliderValueChanged (juce::Slider*) override;
    void setValueFromMouse (juce::Point<float> position, bool shiftHeld = false);
    void paintOrangeArrow (juce::Graphics& g, float x, float railTop, float railBottom);
    void paintWhiteArrow (juce::Graphics& g, float x, float railTop, float railBottom);
    void paintGreenArrow (juce::Graphics& g, float x, float railTop, float railBottom);
    void paintPurpleArrow (juce::Graphics& g, float x, float railTop, float railBottom);
    void paintHorizontalOrangeArrow (juce::Graphics& g, float /*handleX*/, float railLeft, float railRight, float y);
    void paintHorizontalWhiteArrow (juce::Graphics& g, float /*handleX*/, float railLeft, float railRight, float y);
    void paintHorizontalGreenArrow (juce::Graphics& g, float railLeft, float railRight, float y);
    void paintHorizontalPurpleArrow (juce::Graphics& g, float railLeft, float railRight, float y);

    static constexpr float kCmdSensitivity = 2.0f; // full rail / kCmdSensitivity = full ±1

    juce::Slider& slider;
    juce::String label;
    bool primary;
    juce::Colour rail;
    juce::Colour handle;
    juce::Colour arrow;

    float targetModulation = 0.0f;
    float smoothedTarget = 0.0f;
    float appliedModulation = 0.0f;
    float smoothedApplied = 0.0f;

    bool arrowDragActive = false;
    float dragStartMod = 0.0f;

    float slopeTarget = 0.0f;
    float slopeSmoothed = 0.0f;
    float jitterTarget = 0.0f;
    float jitterSmoothed = 0.0f;
    bool slopeDragActive = false;
    bool jitterDragActive = false;
    float slopeDragStartMod = 0.0f;
    float jitterDragStartMod = 0.0f;
    juce::Colour slopeArrow;
    juce::Colour jitterArrow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RuneFader)
};

} // namespace gravel
