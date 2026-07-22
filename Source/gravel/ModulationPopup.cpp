#include "ModulationPopup.h"

namespace gravel
{

namespace
{
constexpr auto kPanelWidth  = 360;
constexpr auto kPanelHeight = 540;
constexpr auto kRowHeight   = 28;
constexpr auto kLabelWidth  = 70;
constexpr auto kPad         = 8;
constexpr auto kSliderH     = 20;
constexpr auto kToggleH     = 18;

class MiniFaderLNF final : public juce::LookAndFeel_V4
{
public:
    MiniFaderLNF (juce::Colour r, juce::Colour h, juce::Colour a)
        : rail (r), handleColour (h), accent (a) {}

    void drawLinearSlider (juce::Graphics& g, int x, int y, int w, int h,
                           float sliderPos, float, float,
                           const juce::Slider::SliderStyle, juce::Slider&) override
    {
        auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (2.0f, 0.0f);
        auto cy = bounds.getCentreY();
        auto lx = bounds.getX();
        auto rx = bounds.getRight();
        static constexpr auto capHalf = 4.0f;
        static constexpr auto handleHalf = 6.0f;

        g.setColour (rail.withAlpha (0.35f));
        g.drawLine (lx, cy, rx, cy, 1.2f);

        g.setColour (handleColour);
        g.drawLine (lx, cy - capHalf, lx, cy + capHalf, 1.2f);
        g.drawLine (rx, cy - capHalf, rx, cy + capHalf, 1.2f);

        g.setColour (accent);
        g.drawLine (sliderPos, cy - handleHalf, sliderPos, cy + handleHalf, 1.5f);
        g.fillEllipse (sliderPos - 1.2f, cy - 1.2f, 2.4f, 2.4f);
    }

private:
    juce::Colour rail, handleColour, accent;
};

juce::ToggleButton* setupToggle (juce::ToggleButton& tb, const juce::String& text,
                                  juce::Colour textCol, juce::Colour accentCol, bool initial)
{
    tb.setButtonText (text);
    tb.setToggleState (initial, juce::dontSendNotification);
    tb.setColour (juce::ToggleButton::textColourId, textCol);
    tb.setColour (juce::ToggleButton::tickColourId, accentCol);
    tb.setColour (juce::ToggleButton::tickDisabledColourId, textCol.withAlpha (0.3f));
    return &tb;
}
}

ModulationPopup::ModulationPopup (const juce::String& paramName, ModulationState& stateRef)
    : state (stateRef)
{
    auto setupSlider = [&] (juce::Slider& s, float v, float minVal, float maxVal, float skewMidpoint = 0.0f)
    {
        s.setSliderStyle (juce::Slider::LinearHorizontal);
        s.setRange (minVal, maxVal, 0.01f);
        if (skewMidpoint > 0.0f)
            s.setSkewFactorFromMidPoint (skewMidpoint);
        s.setValue (v, juce::dontSendNotification);
        s.setTextBoxStyle (juce::Slider::NoTextBox, 0, 0, 0);
        s.setWantsKeyboardFocus (false);
    };

    auto setupLabel = [&] (juce::Label& l, const juce::String& t)
    {
        l.setText (t, juce::dontSendNotification);
        l.setColour (juce::Label::textColourId, text);
        l.setFont (juce::Font (juce::FontOptions (11.0f)));
        l.setJustificationType (juce::Justification::centredLeft);
        l.setWantsKeyboardFocus (false);
    };

    auto setupHeader = [&] (juce::Label& l, const juce::String& t)
    {
        l.setText (t, juce::dontSendNotification);
        l.setFont (juce::Font (juce::FontOptions (12.0f)).boldened());
        l.setColour (juce::Label::textColourId, accent);
    };

    auto applyLNF = [&] (juce::Slider& s)
    {
        s.setLookAndFeel (new MiniFaderLNF (text, text, accent));
    };

    titleLabel.setText (paramName.toUpperCase(), juce::dontSendNotification);
    titleLabel.setColour (juce::Label::textColourId, accent);
    titleLabel.setFont (juce::Font (juce::FontOptions (15.0f)).boldened());
    titleLabel.setJustificationType (juce::Justification::centred);

    // --- Envelope section ---
    setupHeader (envHeader, "ENVELOPE");

    setupLabel (envRiseLabel, "RISE");
    setupSlider (envRiseSlider, state.envRise.load(), 1.0f, 1000.0f, 30.0f);
    applyLNF (envRiseSlider);
    envRiseSlider.onValueChange = [this] { state.envRise.store (static_cast<float> (envRiseSlider.getValue())); };

    setupLabel (envFallLabel, "FALL");
    setupSlider (envFallSlider, state.envFall.load(), 1.0f, 1000.0f, 30.0f);
    applyLNF (envFallSlider);
    envFallSlider.onValueChange = [this] { state.envFall.store (static_cast<float> (envFallSlider.getValue())); };

    setupLabel (envGainLabel, "GAIN");
    setupSlider (envGainSlider, state.envGain.load(), 0.0f, 10.0f);
    applyLNF (envGainSlider);
    envGainSlider.onValueChange = [this] { state.envGain.store (static_cast<float> (envGainSlider.getValue())); };

    setupHeader (envExploreLabel, "EXPLORE");
    setupToggle (envValueToggle, "Value", text, accent, state.explEnabled.load());
    envValueToggle.onStateChange = [this] { state.explEnabled.store (envValueToggle.getToggleState()); };
    setupToggle (envModAmountToggle, "Mod Amount", text, accent, state.envModAmountExplEnabled.load());
    envModAmountToggle.onStateChange = [this] { state.envModAmountExplEnabled.store (envModAmountToggle.getToggleState()); };
    setupToggle (envSettingsToggle, "Settings", text, accent, state.explModEnabled.load());
    envSettingsToggle.onStateChange = [this] { state.explModEnabled.store (envSettingsToggle.getToggleState()); };

    // --- Jitter section (before slope) ---
    setupHeader (jitterHeader, "JITTER");

    setupLabel (jitterSpeedLabel, "SPEED");
    setupSlider (jitterSpeedSlider, state.jitterSpeed.load(), 0.0f, 20000.0f, 5.0f);
    applyLNF (jitterSpeedSlider);
    jitterSpeedSlider.onValueChange = [this] { state.jitterSpeed.store (static_cast<float> (jitterSpeedSlider.getValue())); };

    setupLabel (jitterSpikeLabel, "SPIKE");
    setupSlider (jitterSpikeSlider, state.jitterSpikyness.load(), 0.3f, 10.0f);
    applyLNF (jitterSpikeSlider);
    jitterSpikeSlider.onValueChange = [this] { state.jitterSpikyness.store (static_cast<float> (jitterSpikeSlider.getValue())); };

    setupToggle (jitterToggle, "JITTER", text, accent, state.jitterEnabled.load());
    jitterToggle.onStateChange = [this] { state.jitterEnabled.store (jitterToggle.getToggleState()); };

    setupHeader (jitterExploreLabel, "EXPLORE");
    setupToggle (jitterModAmountToggle, "Mod Amount", text, accent, state.jitterModAmountExplEnabled.load());
    jitterModAmountToggle.onStateChange = [this] { state.jitterModAmountExplEnabled.store (jitterModAmountToggle.getToggleState()); };
    setupToggle (jitterSettingsToggle, "Settings", text, accent, state.jitterExplModEnabled.load());
    jitterSettingsToggle.onStateChange = [this] { state.jitterExplModEnabled.store (jitterSettingsToggle.getToggleState()); };

    // --- Slope section ---
    setupHeader (slopeHeader, "SLOPE");

    setupLabel (slopeSpeedLabel, "SPEED");
    setupSlider (slopeSpeedSlider, state.slopeSpeed.load(), 0.0f, 500.0f, 5.0f);
    applyLNF (slopeSpeedSlider);
    slopeSpeedSlider.onValueChange = [this] { state.slopeSpeed.store (static_cast<float> (slopeSpeedSlider.getValue())); };

    setupLabel (slopeSpikeLabel, "SPIKE");
    setupSlider (slopeSpikeSlider, state.slopeSpikyness.load(), 0.3f, 10.0f);
    applyLNF (slopeSpikeSlider);
    slopeSpikeSlider.onValueChange = [this] { state.slopeSpikyness.store (static_cast<float> (slopeSpikeSlider.getValue())); };

    setupToggle (slopeToggle, "SLOPE", text, accent, state.slopeEnabled.load());
    slopeToggle.onStateChange = [this] { state.slopeEnabled.store (slopeToggle.getToggleState()); };

    setupHeader (slopeExploreLabel, "EXPLORE");
    setupToggle (slopeModAmountToggle, "Mod Amount", text, accent, state.slopeModAmountExplEnabled.load());
    slopeModAmountToggle.onStateChange = [this] { state.slopeModAmountExplEnabled.store (slopeModAmountToggle.getToggleState()); };
    setupToggle (slopeSettingsToggle, "Settings", text, accent, state.slopeExplModEnabled.load());
    slopeSettingsToggle.onStateChange = [this] { state.slopeExplModEnabled.store (slopeSettingsToggle.getToggleState()); };

    addAndMakeVisible (titleLabel);
    addAndMakeVisible (envHeader);
    addAndMakeVisible (envRiseLabel);
    addAndMakeVisible (envRiseSlider);
    addAndMakeVisible (envFallLabel);
    addAndMakeVisible (envFallSlider);
    addAndMakeVisible (envGainLabel);
    addAndMakeVisible (envGainSlider);
    addAndMakeVisible (envExploreLabel);
    addAndMakeVisible (envValueToggle);
    addAndMakeVisible (envModAmountToggle);
    addAndMakeVisible (envSettingsToggle);
    addAndMakeVisible (jitterHeader);
    addAndMakeVisible (jitterSpeedLabel);
    addAndMakeVisible (jitterSpeedSlider);
    addAndMakeVisible (jitterSpikeLabel);
    addAndMakeVisible (jitterSpikeSlider);
    addAndMakeVisible (jitterToggle);
    addAndMakeVisible (jitterExploreLabel);
    addAndMakeVisible (jitterModAmountToggle);
    addAndMakeVisible (jitterSettingsToggle);
    addAndMakeVisible (slopeHeader);
    addAndMakeVisible (slopeSpeedLabel);
    addAndMakeVisible (slopeSpeedSlider);
    addAndMakeVisible (slopeSpikeLabel);
    addAndMakeVisible (slopeSpikeSlider);
    addAndMakeVisible (slopeToggle);
    addAndMakeVisible (slopeExploreLabel);
    addAndMakeVisible (slopeModAmountToggle);
    addAndMakeVisible (slopeSettingsToggle);

    setSize (kPanelWidth, kPanelHeight);
}

ModulationPopup::~ModulationPopup()
{
    envRiseSlider.setLookAndFeel (nullptr);
    envFallSlider.setLookAndFeel (nullptr);
    envGainSlider.setLookAndFeel (nullptr);
    jitterSpeedSlider.setLookAndFeel (nullptr);
    jitterSpikeSlider.setLookAndFeel (nullptr);
    slopeSpeedSlider.setLookAndFeel (nullptr);
    slopeSpikeSlider.setLookAndFeel (nullptr);
}

void ModulationPopup::setColours (juce::Colour overlayColour,
                                   juce::Colour panelBgColour,
                                   juce::Colour textColour,
                                   juce::Colour accentColour)
{
    overlay = overlayColour;
    panelBg = panelBgColour;
    text = textColour;
    accent = accentColour;
}

juce::Rectangle<float> ModulationPopup::getPanelBounds() const
{
    auto bounds = getLocalBounds().toFloat();
    const auto pw = juce::jmin (static_cast<float> (kPanelWidth), bounds.getWidth() - kPad * 2);
    const auto ph = static_cast<float> (kPanelHeight);
    const auto panelX = (bounds.getWidth() - pw) * 0.5f;
    const auto panelY = (bounds.getHeight() - ph) * 0.5f;
    return { panelX, panelY, pw, ph };
}

void ModulationPopup::mouseDown (const juce::MouseEvent& event)
{
    if (! getPanelBounds().contains (event.position))
    {
        if (onClose)
            onClose();
    }
}

void ModulationPopup::paint (juce::Graphics& g)
{
    g.setColour (overlay);
    g.fillAll();

    auto panelRect = getPanelBounds();
    g.setColour (panelBg);
    g.fillRoundedRectangle (panelRect, 6.0f);
    g.setColour (text.withAlpha (0.25f));
    g.drawRoundedRectangle (panelRect, 6.0f, 1.0f);
}

void ModulationPopup::resized()
{
    auto bounds = getLocalBounds();
    const auto pw = juce::jmin (kPanelWidth, bounds.getWidth() - kPad * 2);
    const auto ph = kPanelHeight;
    const auto panelX = (bounds.getWidth() - pw) / 2;
    const auto panelY = (bounds.getHeight() - ph) / 2;

    auto panel = juce::Rectangle<int> (panelX, panelY, pw, ph).reduced (kPad);
    auto sw = pw - kLabelWidth * 2 - kPad * 2;

    auto layoutSlider = [&] (juce::Rectangle<int>& section, juce::Label& label, juce::Slider& slider)
    {
        label.setBounds (section.removeFromTop (kRowHeight).withTrimmedLeft (kLabelWidth));
        slider.setBounds (label.getBounds().removeFromLeft (sw).withHeight (kSliderH).translated (0, (kRowHeight - kSliderH) / 2));
    };

    auto layoutToggle = [&] (juce::Rectangle<int>& area, juce::ToggleButton& tb)
    {
        tb.setBounds (area.removeFromTop (kToggleH).withSizeKeepingCentre (140, kToggleH));
    };

    titleLabel.setBounds (panel.removeFromTop (24));

    // --- Envelope section ---
    envHeader.setBounds (panel.removeFromTop (14));
    auto envSliders = panel.removeFromTop (kRowHeight * 3);
    layoutSlider (envSliders, envRiseLabel, envRiseSlider);
    layoutSlider (envSliders, envFallLabel, envFallSlider);
    layoutSlider (envSliders, envGainLabel, envGainSlider);

    envExploreLabel.setBounds (panel.removeFromTop (14));
    layoutToggle (panel, envValueToggle);
    layoutToggle (panel, envModAmountToggle);
    layoutToggle (panel, envSettingsToggle);

    panel.removeFromTop (kPad);

    // --- Jitter section ---
    jitterHeader.setBounds (panel.removeFromTop (14));
    auto jitterSliders = panel.removeFromTop (kRowHeight * 2);
    layoutSlider (jitterSliders, jitterSpeedLabel, jitterSpeedSlider);
    layoutSlider (jitterSliders, jitterSpikeLabel, jitterSpikeSlider);

    layoutToggle (panel, jitterToggle);
    jitterExploreLabel.setBounds (panel.removeFromTop (14));
    layoutToggle (panel, jitterModAmountToggle);
    layoutToggle (panel, jitterSettingsToggle);

    panel.removeFromTop (kPad);

    // --- Slope section ---
    slopeHeader.setBounds (panel.removeFromTop (14));
    auto slopeSliders = panel.removeFromTop (kRowHeight * 2);
    layoutSlider (slopeSliders, slopeSpeedLabel, slopeSpeedSlider);
    layoutSlider (slopeSliders, slopeSpikeLabel, slopeSpikeSlider);

    layoutToggle (panel, slopeToggle);
    slopeExploreLabel.setBounds (panel.removeFromTop (14));
    layoutToggle (panel, slopeModAmountToggle);
    layoutToggle (panel, slopeSettingsToggle);
}

} // namespace gravel
