#include "FloatFader.h"

namespace gravel
{

FloatFader::FloatFader (const juce::String& labelText,
                        juce::Colour railColour,
                        juce::Colour handleColour)
    : label (labelText), rail (railColour), handle (handleColour)
{
    setWantsKeyboardFocus (false);
}

void FloatFader::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    const auto railLeft = bounds.getX() + 18.0f;
    const auto railRight = bounds.getRight() - 18.0f;
    const auto y = bounds.getCentreY() + 7.0f;

    g.setColour (rail.withAlpha (0.80f));
    g.drawLine (railLeft, y, railRight, y, 2.0f);
    g.drawLine (railLeft, y - 16.0f, railLeft, y + 16.0f, 2.0f);
    g.drawLine (railRight, y - 16.0f, railRight, y + 16.0f, 2.0f);

    const auto handleX = juce::jmap (value, 0.0f, 1.0f, railLeft, railRight);
    g.setColour (handle);
    g.drawLine (handleX, y - 16.0f, handleX, y + 16.0f, 2.0f);
    g.fillEllipse (handleX - 2.0f, y - 2.0f, 4.0f, 4.0f);

    g.setColour (rail.withAlpha (0.76f));
    g.setFont (13.0f);
    g.drawFittedText (label.toUpperCase(), getLocalBounds().removeFromTop (18), juce::Justification::centred, 1);
}

void FloatFader::setValue (float normalized)
{
    value = juce::jlimit (0.0f, 1.0f, normalized);
    repaint();
}

void FloatFader::mouseDown (const juce::MouseEvent& event)
{
    const auto railLeft = 18.0f;
    const auto railRight = static_cast<float> (getWidth() - 18);
    const auto prop = juce::jlimit (0.0f, 1.0f, (event.position.x - railLeft) / (railRight - railLeft));
    value = prop;
    if (onValueChanged)
        onValueChanged (prop);
    repaint();
}

void FloatFader::mouseDrag (const juce::MouseEvent& event)
{
    const auto railLeft = 18.0f;
    const auto railRight = static_cast<float> (getWidth() - 18);
    const auto prop = juce::jlimit (0.0f, 1.0f, (event.position.x - railLeft) / (railRight - railLeft));
    value = prop;
    if (onValueChanged)
        onValueChanged (prop);
    repaint();
}

} // namespace gravel
