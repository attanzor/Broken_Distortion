#include "SpectralGridComponent.h"

namespace
{
const auto voidColour = juce::Colour (0xffd5d0b8);
const auto emberColour = juce::Colour (0xffef8b48);
const auto groundColour = juce::Colour (0xff11100e);

constexpr float kLineTolerance = 6.0f;
constexpr float kIntersectionTolerance = 10.0f;
constexpr float kPushMargin = 0.05f;
constexpr float kHandleDotRadius = 3.0f;

constexpr std::array<float, 3> kLayerAlphas = { 0.55f, 0.70f, 0.85f };
}

SpectralGridComponent::SpectralGridComponent()
{
    setWantsKeyboardFocus (false);
    setMouseClickGrabsKeyboardFocus (false);
}

void SpectralGridComponent::resized()
{
    recalcGridBounds();
}

void SpectralGridComponent::recalcGridBounds()
{
    const auto b = getLocalBounds();
    const auto size = juce::jmin (b.getWidth(), b.getHeight());
    gridBounds = {
        b.getCentreX() - size / 2,
        b.getCentreY() - size / 2,
        size,
        size
    };
}

float SpectralGridComponent::getCellLeft (int band) const
{
    const auto gb = gridBounds.toFloat();
    if (band == 0) return gb.getX();
    if (band == 1) return gb.getX() + vLinePos[0] * gb.getWidth();
    return gb.getX() + vLinePos[1] * gb.getWidth();
}

float SpectralGridComponent::getCellRight (int band) const
{
    const auto gb = gridBounds.toFloat();
    if (band == 0) return gb.getX() + vLinePos[0] * gb.getWidth();
    if (band == 1) return gb.getX() + vLinePos[1] * gb.getWidth();
    return gb.getRight();
}

float SpectralGridComponent::getCellTop (int layer) const
{
    const auto gb = gridBounds.toFloat();
    if (layer == 0) return gb.getY();
    if (layer == 1) return gb.getY() + hLinePos[0] * gb.getHeight();
    return gb.getY() + hLinePos[1] * gb.getHeight();
}

float SpectralGridComponent::getCellBottom (int layer) const
{
    const auto gb = gridBounds.toFloat();
    if (layer == 0) return gb.getY() + hLinePos[0] * gb.getHeight();
    if (layer == 1) return gb.getY() + hLinePos[1] * gb.getHeight();
    return gb.getBottom();
}

void SpectralGridComponent::paint (juce::Graphics& g)
{
    g.fillAll (groundColour);

    const auto gb = gridBounds.toFloat();

    // 1. FFT analyzer bars
    if (!analyzerData.empty())
    {
        const auto numBars = static_cast<int> (analyzerData.size());
        const auto barWidth = gb.getWidth() / static_cast<float> (numBars);

        for (int i = 0; i < numBars; ++i)
        {
            const auto magnitude = juce::jlimit (0.0f, 1.0f, analyzerData[static_cast<size_t> (i)]);
            const auto barHeight = magnitude * gb.getHeight();
            const auto x = gb.getX() + static_cast<float> (i) * barWidth;

            const auto alpha = juce::jmap (magnitude, 0.0f, 1.0f, 0.03f, 0.12f);
            g.setColour (emberColour.withAlpha (alpha));
            g.drawVerticalLine (static_cast<int> (x + barWidth * 0.5f),
                                gb.getBottom() - barHeight,
                                gb.getBottom());
        }
    }

    // 2. Draw cell wavetable shapes
    for (int cellIdx = 0; cellIdx < kNumCells; ++cellIdx)
    {
        const auto band = cellIdx % kNumBands;
        const auto layer = cellIdx / kNumBands;

        const auto bounds = juce::Rectangle<float> {
            getCellLeft (band),
            getCellTop (layer),
            getCellRight (band) - getCellLeft (band),
            getCellBottom (layer) - getCellTop (layer)
        };

        cells[static_cast<size_t> (cellIdx)].bounds = bounds;
        drawShape (g, bounds, cells[static_cast<size_t> (cellIdx)].shapeData, kLayerAlphas[static_cast<size_t> (layer)]);
    }

    // 2b. Cell activity glow overlay
    for (int cellIdx = 0; cellIdx < kNumCells; ++cellIdx)
    {
        const auto activity = cellActivity[static_cast<size_t> (cellIdx)];
        if (activity > 0.01f)
        {
            g.setColour (emberColour.withAlpha (activity * 0.20f));
            g.fillRect (cells[static_cast<size_t> (cellIdx)].bounds);
        }
    }

    // 3. Draw grid lines
    g.setColour (voidColour.withAlpha (0.55f));

    for (int i = 0; i < 2; ++i)
    {
        const auto x = gb.getX() + vLinePos[static_cast<size_t> (i)] * gb.getWidth();
        g.drawLine (x, gb.getY(), x, gb.getBottom(), 1.0f);
    }

    for (int i = 0; i < 2; ++i)
    {
        const auto y = gb.getY() + hLinePos[static_cast<size_t> (i)] * gb.getHeight();
        g.drawLine (gb.getX(), y, gb.getRight(), y, 1.0f);
    }

    // 4. Handle dots at endpoints and intersections
    const auto dotColour = emberColour.withAlpha (0.70f);
    for (int vi = 0; vi < 2; ++vi)
    {
        const auto x = gb.getX() + vLinePos[static_cast<size_t> (vi)] * gb.getWidth();

        g.setColour (dotColour);
        g.fillEllipse (x - kHandleDotRadius, gb.getY() - kHandleDotRadius,
                       kHandleDotRadius * 2.0f, kHandleDotRadius * 2.0f);
        g.fillEllipse (x - kHandleDotRadius, gb.getBottom() - kHandleDotRadius,
                       kHandleDotRadius * 2.0f, kHandleDotRadius * 2.0f);

        for (int hi = 0; hi < 2; ++hi)
        {
            const auto y = gb.getY() + hLinePos[static_cast<size_t> (hi)] * gb.getHeight();
            g.fillEllipse (x - kHandleDotRadius, y - kHandleDotRadius,
                           kHandleDotRadius * 2.0f, kHandleDotRadius * 2.0f);
        }
    }

    // 5. Selected cell border
    if (selectedCell >= 0 && selectedCell < kNumCells)
    {
        const auto& cell = cells[static_cast<size_t> (selectedCell)];
        g.setColour (emberColour.withAlpha (0.40f));
        g.drawRect (cell.bounds, 1.0f);
    }
}

void SpectralGridComponent::drawShape (juce::Graphics& g, juce::Rectangle<float> bounds, const std::array<float, kWaveTableSize>& data, float alpha)
{
    if (bounds.isEmpty())
        return;

    juce::Path path;
    const auto centreY = bounds.getCentreY();
    const auto halfH = bounds.getHeight() * 0.5f;

    for (int i = 0; i < kWaveTableSize; ++i)
    {
        const auto x = bounds.getX() + (static_cast<float> (i) / static_cast<float> (kWaveTableSize - 1)) * bounds.getWidth();
        const auto y = centreY - (data[static_cast<size_t> (i)] * halfH);

        if (i == 0)
            path.startNewSubPath (x, y);
        else
            path.lineTo (x, y);
    }

    g.setColour (emberColour.withAlpha (alpha));
    g.strokePath (path, juce::PathStrokeType (1.0f));
}

void SpectralGridComponent::mouseDown (const juce::MouseEvent& event)
{
    const auto pos = event.getPosition().toFloat();
    const auto gb = gridBounds.toFloat();

    dragTarget = DragTarget::None;
    dragCellIndex = -1;
    dragStartX = pos.x;
    dragStartY = pos.y;

    if (!gb.contains (pos))
        return;

    for (int vi = 0; vi < 2; ++vi)
    {
        const auto x = gb.getX() + vLinePos[static_cast<size_t> (vi)] * gb.getWidth();
        for (int hi = 0; hi < 2; ++hi)
        {
            const auto y = gb.getY() + hLinePos[static_cast<size_t> (hi)] * gb.getHeight();
            if (std::abs (pos.x - x) < kIntersectionTolerance && std::abs (pos.y - y) < kIntersectionTolerance)
            {
                dragTarget = static_cast<DragTarget> (static_cast<int> (DragTarget::Intersection00) + vi * 2 + hi);
                return;
            }
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        const auto x = gb.getX() + vLinePos[static_cast<size_t> (i)] * gb.getWidth();
        if (std::abs (pos.x - x) < kLineTolerance && pos.y >= gb.getY() && pos.y <= gb.getBottom())
        {
            dragTarget = (i == 0) ? DragTarget::VLine0 : DragTarget::VLine1;
            return;
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        const auto y = gb.getY() + hLinePos[static_cast<size_t> (i)] * gb.getHeight();
        if (std::abs (pos.y - y) < kLineTolerance && pos.x >= gb.getX() && pos.x <= gb.getRight())
        {
            dragTarget = (i == 0) ? DragTarget::HLine0 : DragTarget::HLine1;
            return;
        }
    }

    for (int cellIdx = 0; cellIdx < kNumCells; ++cellIdx)
    {
        const auto band = cellIdx % kNumBands;
        const auto layer = cellIdx / kNumBands;

        const auto cellBounds = juce::Rectangle<float> {
            getCellLeft (band),
            getCellTop (layer),
            getCellRight (band) - getCellLeft (band),
            getCellBottom (layer) - getCellTop (layer)
        };

        if (cellBounds.contains (pos))
        {
            dragTarget = DragTarget::Cell;
            dragCellIndex = cellIdx;
            selectedCell = cellIdx;
            repaint();
            return;
        }
    }
}

void SpectralGridComponent::mouseDrag (const juce::MouseEvent& event)
{
    const auto pos = event.getPosition().toFloat();
    const auto gb = gridBounds.toFloat();

    if (dragTarget == DragTarget::None)
        return;

    const auto deltaX = (pos.x - dragStartX) / gb.getWidth();
    const auto deltaY = (pos.y - dragStartY) / gb.getHeight();

    if (dragTarget == DragTarget::VLine0 || dragTarget == DragTarget::VLine1)
    {
        const auto idx = (dragTarget == DragTarget::VLine0) ? 0 : 1;
        auto newPos = vLinePos[static_cast<size_t> (idx)] + deltaX;
        newPos = juce::jlimit (0.0f, 1.0f, newPos);

        if (idx == 0)
            newPos = juce::jmin (newPos, vLinePos[1] - kPushMargin);
        else
            newPos = juce::jmax (newPos, vLinePos[0] + kPushMargin);

        vLinePos[static_cast<size_t> (idx)] = newPos;
        dragStartX = pos.x;
        repaint();
    }
    else if (dragTarget == DragTarget::HLine0 || dragTarget == DragTarget::HLine1)
    {
        const auto idx = (dragTarget == DragTarget::HLine0) ? 0 : 1;
        auto newPos = hLinePos[static_cast<size_t> (idx)] + deltaY;
        newPos = juce::jlimit (0.0f, 1.0f, newPos);

        if (idx == 0)
            newPos = juce::jmin (newPos, hLinePos[1] - kPushMargin);
        else
            newPos = juce::jmax (newPos, hLinePos[0] + kPushMargin);

        hLinePos[static_cast<size_t> (idx)] = newPos;
        dragStartY = pos.y;
        repaint();
    }
    else if (dragTarget >= DragTarget::Intersection00 && dragTarget <= DragTarget::Intersection11)
    {
        const int offset = static_cast<int> (dragTarget) - static_cast<int> (DragTarget::Intersection00);
        const auto vi = offset / 2;
        const auto hi = offset % 2;

        auto newV = vLinePos[static_cast<size_t> (vi)] + deltaX;
        auto newH = hLinePos[static_cast<size_t> (hi)] + deltaY;

        if (vi == 0)
            newV = juce::jmin (newV, vLinePos[1] - kPushMargin);
        else
            newV = juce::jmax (newV, vLinePos[0] + kPushMargin);

        if (hi == 0)
            newH = juce::jmin (newH, hLinePos[1] - kPushMargin);
        else
            newH = juce::jmax (newH, hLinePos[0] + kPushMargin);

        vLinePos[static_cast<size_t> (vi)] = juce::jlimit (0.0f, 1.0f, newV);
        hLinePos[static_cast<size_t> (hi)] = juce::jlimit (0.0f, 1.0f, newH);
        dragStartX = pos.x;
        dragStartY = pos.y;
        repaint();
    }
}

void SpectralGridComponent::mouseUp (const juce::MouseEvent&)
{
    if (dragTarget == DragTarget::Cell && dragCellIndex >= 0)
    {
        if (onCellClicked)
            onCellClicked (dragCellIndex);
    }
    else if (dragTarget == DragTarget::VLine0 || dragTarget == DragTarget::VLine1)
    {
        const auto idx = (dragTarget == DragTarget::VLine0) ? 0 : 1;
        if (onVerticalLineDragged)
            onVerticalLineDragged (idx, vLinePos[static_cast<size_t> (idx)]);
    }
    else if (dragTarget == DragTarget::HLine0 || dragTarget == DragTarget::HLine1)
    {
        const auto idx = (dragTarget == DragTarget::HLine0) ? 0 : 1;
        if (onHorizontalLineDragged)
            onHorizontalLineDragged (idx, hLinePos[static_cast<size_t> (idx)]);
    }
    else if (dragTarget >= DragTarget::Intersection00 && dragTarget <= DragTarget::Intersection11)
    {
        const int offset = static_cast<int> (dragTarget) - static_cast<int> (DragTarget::Intersection00);
        const auto vi = offset / 2;
        const auto hi = offset % 2;

        if (onVerticalLineDragged)
            onVerticalLineDragged (vi, vLinePos[static_cast<size_t> (vi)]);
        if (onHorizontalLineDragged)
            onHorizontalLineDragged (hi, hLinePos[static_cast<size_t> (hi)]);
    }

    dragTarget = DragTarget::None;
    dragCellIndex = -1;
}

void SpectralGridComponent::setCellShapeData (int cellIndex, const std::array<float, kWaveTableSize>& data)
{
    if (cellIndex >= 0 && cellIndex < kNumCells)
    {
        cells[static_cast<size_t> (cellIndex)].shapeData = data;
        repaint();
    }
}

void SpectralGridComponent::setSelectedCell (int cellIndex)
{
    selectedCell = cellIndex;
    repaint();
}

void SpectralGridComponent::setVerticalLine (int index, float normalizedPos)
{
    if (index >= 0 && index < 2)
    {
        auto clamped = juce::jlimit (0.0f, 1.0f, normalizedPos);
        if (index == 0)
            clamped = juce::jmin (clamped, vLinePos[1] - kPushMargin);
        else
            clamped = juce::jmax (clamped, vLinePos[0] + kPushMargin);

        vLinePos[static_cast<size_t> (index)] = clamped;
        repaint();
    }
}

void SpectralGridComponent::setHorizontalLine (int index, float normalizedPos)
{
    if (index >= 0 && index < 2)
    {
        auto clamped = juce::jlimit (0.0f, 1.0f, normalizedPos);
        if (index == 0)
            clamped = juce::jmin (clamped, hLinePos[1] - kPushMargin);
        else
            clamped = juce::jmax (clamped, hLinePos[0] + kPushMargin);

        hLinePos[static_cast<size_t> (index)] = clamped;
        repaint();
    }
}

float SpectralGridComponent::getVerticalLine (int index) const
{
    if (index >= 0 && index < 2)
        return vLinePos[static_cast<size_t> (index)];

    return 0.0f;
}

float SpectralGridComponent::getHorizontalLine (int index) const
{
    if (index >= 0 && index < 2)
        return hLinePos[static_cast<size_t> (index)];

    return 0.0f;
}

void SpectralGridComponent::setAnalyzerData (const std::vector<float>& magnitudes)
{
    analyzerData = magnitudes;
    repaint();
}

void SpectralGridComponent::setCellActivity (const std::array<float, kNumCells>& activity)
{
    // activity is in DSP order (band*major: [b0L0, b0L1, b0L2, b1L0, ...])
    // grid displays in row-major order (layer*major: [b0L0, b1L0, b2L0, b0L1, ...])
    for (int dspIdx = 0; dspIdx < kNumCells; ++dspIdx)
    {
        const auto band  = dspIdx / kNumBands;
        const auto layer = dspIdx % kNumBands;
        const auto gridIdx = layer * kNumBands + band;
        cellActivity[static_cast<size_t> (gridIdx)] = activity[static_cast<size_t> (dspIdx)];
    }
    repaint();
}
