#include "RuneFader.h"

namespace gravel
{

RuneFader::RuneFader (juce::Slider& control,
                      const juce::String& title,
                      bool isPrimary,
                      juce::Colour railColour,
                      juce::Colour handleColour,
                      juce::Colour arrowColour)
    : slider (control),
      label (std::move (title)),
      primary (isPrimary),
      rail (railColour),
      handle (handleColour),
      arrow (arrowColour),
      slopeArrow (juce::Colour (0xff4caf50).withAlpha (0.55f)),
      jitterArrow (juce::Colour (0xffab47bc).withAlpha (0.55f))
{
    slider.addListener (this);
    slider.setWantsKeyboardFocus (false);
    setWantsKeyboardFocus (false);
    setMouseCursor (primary ? juce::MouseCursor::LeftRightResizeCursor : juce::MouseCursor::UpDownResizeCursor);
}

RuneFader::~RuneFader()
{
    slider.removeListener (this);
}

void RuneFader::setTargetModulation (float amount)
{
    targetModulation = juce::jlimit (-1.0f, 1.0f, amount);
}

void RuneFader::setAppliedModulation (float amount)
{
    appliedModulation = juce::jlimit (-1.0f, 1.0f, amount);
}

void RuneFader::setSlopeModulation (float amount)
{
    slopeTarget = juce::jlimit (-1.0f, 1.0f, amount);
}

void RuneFader::setJitterModulation (float amount)
{
    jitterTarget = juce::jlimit (-1.0f, 1.0f, amount);
}

void RuneFader::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    const auto value = static_cast<float> (slider.getValue());
    const auto normalised = static_cast<float> (slider.valueToProportionOfLength (value));
    const auto lineWidth = primary ? 2.0f : 1.4f;

    if (primary)
    {
        const auto railLeft = bounds.getX() + 18.0f;
        const auto railRight = bounds.getRight() - 18.0f;
        const auto y = bounds.getCentreY() + 7.0f;
        const auto handleX = juce::jmap (normalised, railLeft, railRight);

        static constexpr auto kPrimaryHandleHalf = 16.0f;

        g.setColour (rail.withAlpha (0.80f));
        g.drawLine (railLeft, y, railRight, y, lineWidth);
        g.drawLine (railLeft, y - kPrimaryHandleHalf, railLeft, y + kPrimaryHandleHalf, lineWidth);
        g.drawLine (railRight, y - kPrimaryHandleHalf, railRight, y + kPrimaryHandleHalf, lineWidth);

        g.setColour (handle);
        g.drawLine (handleX, y - kPrimaryHandleHalf, handleX, y + kPrimaryHandleHalf, lineWidth);
        g.fillEllipse (handleX - lineWidth, y - lineWidth, lineWidth * 2.0f, lineWidth * 2.0f);

        // Orange arrow: target modulation (draggable)
        smoothedTarget += (targetModulation - smoothedTarget) * 0.18f;
        paintHorizontalOrangeArrow (g, handleX, railLeft, railRight, y);

        // White arrow: real-time envelope result (read-only)
        smoothedApplied += (appliedModulation - smoothedApplied) * 0.18f;
        paintHorizontalWhiteArrow (g, handleX, railLeft, railRight, y);

        // Green arrow: slope modulation
        slopeSmoothed += (slopeTarget - slopeSmoothed) * 0.18f;
        paintHorizontalGreenArrow (g, railLeft, railRight, y);

        // Purple arrow: jitter modulation
        jitterSmoothed += (jitterTarget - jitterSmoothed) * 0.18f;
        paintHorizontalPurpleArrow (g, railLeft, railRight, y);

        g.setColour (rail.withAlpha (0.76f));
        g.setFont (13.0f);
        g.drawFittedText (label.toUpperCase(), getLocalBounds().removeFromTop (18), juce::Justification::centred, 1);
        return;
    }

    const auto railTop = bounds.getY() + 30.0f;
    const auto railBottom = bounds.getBottom() - 16.0f;
    const auto x = bounds.getCentreX();
    const auto handleY = juce::jmap (normalised, railBottom, railTop);

    static constexpr auto kHandleHalf = 19.0f;

    g.setColour (rail.withAlpha (0.54f));
    g.drawLine (x, railTop, x, railBottom, lineWidth);

    g.setColour (handle);
    g.drawLine (x - kHandleHalf, railTop, x + kHandleHalf, railTop, lineWidth);
    g.drawLine (x - kHandleHalf, railBottom, x + kHandleHalf, railBottom, lineWidth);

    g.setColour (rail);
    g.drawLine (x - kHandleHalf, handleY, x + kHandleHalf, handleY, lineWidth);
    g.fillEllipse (x - lineWidth, handleY - lineWidth, lineWidth * 2.0f, lineWidth * 2.0f);

    // Orange arrow: target modulation (draggable)
    smoothedTarget += (targetModulation - smoothedTarget) * 0.18f;
    paintOrangeArrow (g, x, railTop, railBottom);

    // White arrow: real-time envelope result (read-only)
    smoothedApplied += (appliedModulation - smoothedApplied) * 0.18f;
    paintWhiteArrow (g, x, railTop, railBottom);

    // Green arrow: slope modulation
    slopeSmoothed += (slopeTarget - slopeSmoothed) * 0.18f;
    paintGreenArrow (g, x, railTop, railBottom);

    // Purple arrow: jitter modulation
    jitterSmoothed += (jitterTarget - jitterSmoothed) * 0.18f;
    paintPurpleArrow (g, x, railTop, railBottom);

    g.setColour (rail.withAlpha (0.76f));
    g.setFont (11.0f);
    g.drawFittedText (label.toUpperCase(), getLocalBounds().removeFromBottom (14), juce::Justification::centred, 1);
}

void RuneFader::paintOrangeArrow (juce::Graphics& g, float x, float railTop, float railBottom)
{
    if (std::abs (smoothedTarget) <= 0.001f)
        return;

    const auto railHeight = railBottom - railTop;
    static constexpr auto kHandleHalf = 19.0f;
    const auto spread = kHandleHalf * 2.0f / 3.0f;
    const auto alpha = juce::jmap (std::abs (smoothedTarget), 0.0f, 1.0f, 0.25f, 0.65f);

    g.setColour (arrow.withAlpha (alpha));

    juce::Path path;
    if (smoothedTarget < 0.0f)
    {
        const auto tipY = railTop + std::abs (smoothedTarget) * railHeight;
        path.startNewSubPath (x - spread, railTop);
        path.lineTo (x, tipY);
        path.lineTo (x + spread, railTop);
    }
    else
    {
        const auto tipY = railBottom - smoothedTarget * railHeight;
        path.startNewSubPath (x - spread, railBottom);
        path.lineTo (x, tipY);
        path.lineTo (x + spread, railBottom);
    }

    g.strokePath (path, juce::PathStrokeType (1.8f));
}

void RuneFader::paintWhiteArrow (juce::Graphics& g, float x, float railTop, float railBottom)
{
    if (std::abs (smoothedApplied) <= 0.001f)
        return;

    const auto railHeight = railBottom - railTop;
    static constexpr auto kHandleHalf = 19.0f;
    const auto spread = kHandleHalf * 2.0f / 3.0f;

    g.setColour (rail.withAlpha (0.70f));

    juce::Path path;
    if (smoothedApplied < 0.0f)
    {
        const auto tipY = railTop + std::abs (smoothedApplied) * railHeight;
        path.startNewSubPath (x - spread, railTop);
        path.lineTo (x, tipY);
        path.lineTo (x + spread, railTop);
    }
    else
    {
        const auto tipY = railBottom - smoothedApplied * railHeight;
        path.startNewSubPath (x - spread, railBottom);
        path.lineTo (x, tipY);
        path.lineTo (x + spread, railBottom);
    }

    g.strokePath (path, juce::PathStrokeType (1.2f));
}

void RuneFader::mouseDown (const juce::MouseEvent& event)
{
    if (event.mods.isCtrlDown())
    {
        if (onCtrlClick)
            onCtrlClick();
        return;
    }

    if (onInteraction)
        onInteraction();

    if (event.mods.isShiftDown())
    {
        slopeDragActive = true;
        slopeDragStartMod = slopeTarget;
        repaint();
        return;
    }
    if (event.mods.isAltDown())
    {
        jitterDragActive = true;
        jitterDragStartMod = jitterTarget;
        repaint();
        return;
    }

    if (event.mods.isCommandDown())
    {
        arrowDragActive = true;
        dragStartMod = targetModulation;
        return;
    }

    arrowDragActive = false;
    setValueFromMouse (event.position, event.mods.isShiftDown());
}

void RuneFader::mouseDrag (const juce::MouseEvent& event)
{
    if (slopeDragActive)
    {
        if (primary)
        {
            const auto railWidth = static_cast<float> (getWidth() - 36);
            const auto mod = slopeDragStartMod + event.getDistanceFromDragStartX() / (railWidth / kCmdSensitivity);
            slopeTarget = juce::jlimit (-1.0f, 1.0f, mod);
        }
        else
        {
            const auto railHeight = static_cast<float> (getHeight() - 46);
            const auto mod = slopeDragStartMod - event.getDistanceFromDragStartY() / (railHeight / kCmdSensitivity);
            slopeTarget = juce::jlimit (-1.0f, 1.0f, mod);
        }
        if (std::abs (slopeTarget) <= 0.01f) slopeTarget = 0.0f;
        if (onSlopeDragged) onSlopeDragged (slopeTarget);
        repaint();
        return;
    }
    if (jitterDragActive)
    {
        if (primary)
        {
            const auto railWidth = static_cast<float> (getWidth() - 36);
            const auto mod = jitterDragStartMod + event.getDistanceFromDragStartX() / (railWidth / kCmdSensitivity);
            jitterTarget = juce::jlimit (-1.0f, 1.0f, mod);
        }
        else
        {
            const auto railHeight = static_cast<float> (getHeight() - 46);
            const auto mod = jitterDragStartMod - event.getDistanceFromDragStartY() / (railHeight / kCmdSensitivity);
            jitterTarget = juce::jlimit (-1.0f, 1.0f, mod);
        }
        if (std::abs (jitterTarget) <= 0.01f) jitterTarget = 0.0f;
        if (onJitterDragged) onJitterDragged (jitterTarget);
        repaint();
        return;
    }

    if (! arrowDragActive)
    {
        setValueFromMouse (event.position, event.mods.isShiftDown());
        return;
    }

    if (primary)
    {
        const auto railWidth = static_cast<float> (getWidth() - 36);
        const auto mod = juce::jlimit (-1.0f, 1.0f,
            dragStartMod + (event.getDistanceFromDragStartX() / (railWidth / kCmdSensitivity)));
        targetModulation = std::abs (mod) > 0.01f ? mod : 0.0f;

        if (onModulationDragged)
            onModulationDragged (targetModulation);

        repaint();
    }
    else
    {
        const auto railHeight = static_cast<float> (getHeight() - 46);
        const auto mod = juce::jlimit (-1.0f, 1.0f,
            dragStartMod + (-event.getDistanceFromDragStartY() / (railHeight / kCmdSensitivity)));
        targetModulation = std::abs (mod) > 0.01f ? mod : 0.0f;

        if (onModulationDragged)
            onModulationDragged (targetModulation);

        repaint();
    }
}

void RuneFader::mouseUp (const juce::MouseEvent&)
{
    if (slopeDragActive)
    {
        slopeDragActive = false;
        if (std::abs (slopeTarget) <= 0.01f)
        {
            slopeTarget = 0.0f;
            if (onSlopeDragged) onSlopeDragged (0.0f);
            repaint();
        }
        return;
    }
    if (jitterDragActive)
    {
        jitterDragActive = false;
        if (std::abs (jitterTarget) <= 0.01f)
        {
            jitterTarget = 0.0f;
            if (onJitterDragged) onJitterDragged (0.0f);
            repaint();
        }
        return;
    }
    arrowDragActive = false;
}

void RuneFader::paintGreenArrow (juce::Graphics& g, float x, float railTop, float railBottom)
{
    if (std::abs (slopeSmoothed) <= 0.001f)
        return;

    const auto railHeight = railBottom - railTop;
    static constexpr auto kHandleHalf = 19.0f;
    const auto spread = kHandleHalf * 2.0f / 3.0f;
    const auto alpha = juce::jmap (std::abs (slopeSmoothed), 0.0f, 1.0f, 0.25f, 0.65f);

    g.setColour (slopeArrow.withAlpha (alpha));

    juce::Path path;
    if (slopeSmoothed < 0.0f)
    {
        const auto tipY = railTop + std::abs (slopeSmoothed) * railHeight;
        path.startNewSubPath (x - spread, railTop);
        path.lineTo (x, tipY);
        path.lineTo (x + spread, railTop);
    }
    else
    {
        const auto tipY = railBottom - slopeSmoothed * railHeight;
        path.startNewSubPath (x - spread, railBottom);
        path.lineTo (x, tipY);
        path.lineTo (x + spread, railBottom);
    }

    g.strokePath (path, juce::PathStrokeType (1.8f));
}

void RuneFader::paintPurpleArrow (juce::Graphics& g, float x, float railTop, float railBottom)
{
    if (std::abs (jitterSmoothed) <= 0.001f)
        return;

    const auto railHeight = railBottom - railTop;
    static constexpr auto kHandleHalf = 19.0f;
    const auto spread = kHandleHalf * 2.0f / 3.0f;
    const auto alpha = juce::jmap (std::abs (jitterSmoothed), 0.0f, 1.0f, 0.25f, 0.65f);

    g.setColour (jitterArrow.withAlpha (alpha));

    juce::Path path;
    if (jitterSmoothed < 0.0f)
    {
        const auto tipY = railTop + std::abs (jitterSmoothed) * railHeight;
        path.startNewSubPath (x - spread, railTop);
        path.lineTo (x, tipY);
        path.lineTo (x + spread, railTop);
    }
    else
    {
        const auto tipY = railBottom - jitterSmoothed * railHeight;
        path.startNewSubPath (x - spread, railBottom);
        path.lineTo (x, tipY);
        path.lineTo (x + spread, railBottom);
    }

    g.strokePath (path, juce::PathStrokeType (1.8f));
}

void RuneFader::paintHorizontalGreenArrow (juce::Graphics& g, float railLeft, float railRight, float y)
{
    if (std::abs (slopeSmoothed) <= 0.001f)
        return;

    const auto railWidth = railRight - railLeft;
    static constexpr auto kSpread = 10.667f;
    const auto alpha = juce::jmap (std::abs (slopeSmoothed), 0.0f, 1.0f, 0.25f, 0.65f);

    g.setColour (slopeArrow.withAlpha (alpha));

    juce::Path path;
    if (slopeSmoothed > 0.0f)
    {
        const auto tipX = railLeft + slopeSmoothed * railWidth;
        path.startNewSubPath (railLeft, y - kSpread);
        path.lineTo (tipX, y);
        path.lineTo (railLeft, y + kSpread);
    }
    else
    {
        const auto tipX = railRight + slopeSmoothed * railWidth;
        path.startNewSubPath (railRight, y - kSpread);
        path.lineTo (tipX, y);
        path.lineTo (railRight, y + kSpread);
    }

    g.strokePath (path, juce::PathStrokeType (1.8f));
}

void RuneFader::paintHorizontalPurpleArrow (juce::Graphics& g, float railLeft, float railRight, float y)
{
    if (std::abs (jitterSmoothed) <= 0.001f)
        return;

    const auto railWidth = railRight - railLeft;
    static constexpr auto kSpread = 10.667f;
    const auto alpha = juce::jmap (std::abs (jitterSmoothed), 0.0f, 1.0f, 0.25f, 0.65f);

    g.setColour (jitterArrow.withAlpha (alpha));

    juce::Path path;
    if (jitterSmoothed > 0.0f)
    {
        const auto tipX = railLeft + jitterSmoothed * railWidth;
        path.startNewSubPath (railLeft, y - kSpread);
        path.lineTo (tipX, y);
        path.lineTo (railLeft, y + kSpread);
    }
    else
    {
        const auto tipX = railRight + jitterSmoothed * railWidth;
        path.startNewSubPath (railRight, y - kSpread);
        path.lineTo (tipX, y);
        path.lineTo (railRight, y + kSpread);
    }

    g.strokePath (path, juce::PathStrokeType (1.8f));
}

void RuneFader::paintHorizontalOrangeArrow (juce::Graphics& g, float /*handleX*/, float railLeft, float railRight, float y)
{
    if (std::abs (smoothedTarget) <= 0.001f)
        return;

    const auto railWidth = railRight - railLeft;
    static constexpr auto kSpread = 10.667f;
    const auto alpha = juce::jmap (std::abs (smoothedTarget), 0.0f, 1.0f, 0.25f, 0.65f);

    g.setColour (arrow.withAlpha (alpha));

    juce::Path path;
    if (smoothedTarget > 0.0f)
    {
        const auto tipX = railLeft + smoothedTarget * railWidth;
        path.startNewSubPath (railLeft, y - kSpread);
        path.lineTo (tipX, y);
        path.lineTo (railLeft, y + kSpread);
    }
    else
    {
        const auto tipX = railRight + smoothedTarget * railWidth;
        path.startNewSubPath (railRight, y - kSpread);
        path.lineTo (tipX, y);
        path.lineTo (railRight, y + kSpread);
    }

    g.strokePath (path, juce::PathStrokeType (1.8f));
}

void RuneFader::paintHorizontalWhiteArrow (juce::Graphics& g, float /*handleX*/, float railLeft, float railRight, float y)
{
    if (std::abs (smoothedApplied) <= 0.001f)
        return;

    const auto railWidth = railRight - railLeft;
    static constexpr auto kSpread = 10.667f;

    g.setColour (rail.withAlpha (0.70f));

    juce::Path path;
    if (smoothedApplied > 0.0f)
    {
        const auto tipX = railLeft + smoothedApplied * railWidth;
        path.startNewSubPath (railLeft, y - kSpread);
        path.lineTo (tipX, y);
        path.lineTo (railLeft, y + kSpread);
    }
    else
    {
        const auto tipX = railRight + smoothedApplied * railWidth;
        path.startNewSubPath (railRight, y - kSpread);
        path.lineTo (tipX, y);
        path.lineTo (railRight, y + kSpread);
    }

    g.strokePath (path, juce::PathStrokeType (1.2f));
}

void RuneFader::setValueFromMouse (juce::Point<float> position, bool shiftHeld)
{
    const auto rawProportion = primary
        ? (position.x - 18.0f) / static_cast<float> (getWidth() - 36)
        : (static_cast<float> (getHeight() - 16) - position.y) / static_cast<float> (getHeight() - 46);

    float proportion;
    if (shiftHeld)
    {
        const auto current = static_cast<float> (slider.valueToProportionOfLength (slider.getValue()));
        const auto delta = (rawProportion - 0.5f) * 0.1f;
        proportion = juce::jlimit (0.0f, 1.0f, current + delta);
    }
    else
    {
        proportion = juce::jlimit (0.0f, 1.0f, rawProportion);
    }

    slider.setValue (slider.proportionOfLengthToValue (proportion), juce::sendNotificationSync);
}

void RuneFader::sliderValueChanged (juce::Slider*)
{
    repaint();
}

} // namespace gravel
