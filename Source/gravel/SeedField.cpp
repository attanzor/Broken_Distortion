#include "SeedField.h"

namespace gravel
{

SeedField::SeedField (int maxD, int minV, int maxV,
                      juce::Colour bgColour, juce::Colour borderColour, juce::Colour textColour)
    : maxDigits (maxD), minValue (minV), maxValue (maxV),
      bg (bgColour), border (borderColour), text (textColour)
{
    setWantsKeyboardFocus (true);
    setMouseClickGrabsKeyboardFocus (true);
    setInterceptsMouseClicks (true, true);
}

void SeedField::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour (bg);
    g.fillRoundedRectangle (bounds, 2.0f);
    g.setColour (border);
    g.drawRoundedRectangle (bounds, 2.0f, 1.0f);
    g.setColour (text);
    g.setFont (14.0f);

    const auto textToDraw = isEditing ? editBuffer : seedText;
    g.drawText (textToDraw, bounds.reduced (6, 0), juce::Justification::centred);
}

void SeedField::resized()
{
}

void SeedField::mouseDown (const juce::MouseEvent&)
{
    if (! isEditing)
    {
        editBuffer = seedText;
        isEditing = true;
        grabKeyboardFocus();
        repaint();
    }
}

bool SeedField::keyPressed (const juce::KeyPress& key)
{
    if (! isEditing)
        return false;

    if (key.getKeyCode() == juce::KeyPress::returnKey)
    {
        commitValue();
        return true;
    }

    if (key.getKeyCode() == juce::KeyPress::escapeKey)
    {
        cancelEditing();
        return true;
    }

    if (key.getKeyCode() == juce::KeyPress::backspaceKey || key.getKeyCode() == juce::KeyPress::deleteKey)
    {
        editBuffer = editBuffer.substring (0, juce::jmax (0, editBuffer.length() - 1));
        repaint();
        return true;
    }

    const auto character = key.getTextCharacter();
    if (character >= '0' && character <= '9' && editBuffer.length() < maxDigits)
    {
        editBuffer += character;
        repaint();
        return true;
    }

    return false;
}

void SeedField::focusLost (FocusChangeType)
{
    if (isEditing)
        commitValue();
}

void SeedField::setSeed (int seed)
{
    seedText = juce::String (seed);
    if (! isEditing)
        repaint();
}

void SeedField::finishEditing()
{
    if (isEditing)
        commitValue();
}

void SeedField::cancelEditing()
{
    if (isEditing)
    {
        isEditing = false;
        editBuffer.clear();
        repaint();
    }
}

void SeedField::commitValue()
{
    if (! isEditing)
        return;

    const auto clamped = juce::jlimit (minValue, maxValue, editBuffer.getIntValue());
    editBuffer = juce::String (clamped);
    seedText = editBuffer;
    isEditing = false;
    repaint();

    if (onSeedCommitted)
        onSeedCommitted (clamped);
}

} // namespace gravel
