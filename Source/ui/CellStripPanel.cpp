#include "CellStripPanel.h"

namespace
{
    const auto emberColour = juce::Colour (0xffef8b48);
    constexpr auto kGroupGap = 4;
    constexpr float kLayerAlphas[] = { 0.55f, 0.70f, 0.85f };
}

CellStripPanel::CellStripPanel()
{
    setWantsKeyboardFocus (false);
    setMouseClickGrabsKeyboardFocus (false);

    for (int i = 0; i < kNumCells; ++i)
    {
        auto& cell = miniCells[static_cast<size_t> (i)];
        cell.morphSlider = std::make_unique<juce::Slider> (juce::Slider::LinearHorizontal, juce::Slider::NoTextBox);
        cell.morphSlider->setRange (0.0, 1.0, 0.01);
        cell.morphSlider->setValue (0.5);
        cell.morphSlider->setTextBoxIsEditable (false);
        cell.morphSlider->setWantsKeyboardFocus (false);
        cell.morphSlider->setMouseClickGrabsKeyboardFocus (false);
        addAndMakeVisible (cell.morphSlider.get());
    }
}

void CellStripPanel::resized()
{
    const auto totalHeight = getHeight();
    const auto gaps = kGroupGap * 2;
    const auto availableHeight = totalHeight - gaps;
    const auto stripHeight = availableHeight / static_cast<float> (kNumCells);
    const auto width = getWidth();

    constexpr auto kShapePortion = 5.0f / 6.0f;
    constexpr auto kSliderPortion = 1.0f / 6.0f;

    for (int i = 0; i < kNumCells; ++i)
    {
        auto& cell = miniCells[static_cast<size_t> (i)];
        const auto groupOffset = (i / 3) * kGroupGap;
        const auto y = static_cast<int> (static_cast<float> (i) * stripHeight + static_cast<float> (groupOffset));
        const auto stripH = static_cast<int> (stripHeight);

        cell.bounds = { 0, y, width, stripH };
        cell.shapeBounds = { 0, y, width, static_cast<int> (stripHeight * kShapePortion) };
        cell.sliderBounds = { 2, y + static_cast<int> (stripHeight * kShapePortion), width - 4, static_cast<int> (stripHeight * kSliderPortion) };

        cell.morphSlider->setBounds (cell.sliderBounds);
    }
}

void CellStripPanel::paint (juce::Graphics& g)
{
    for (int i = 0; i < kNumCells; ++i)
    {
        const auto& cell = miniCells[static_cast<size_t> (i)];
        const auto layer = i / 3;
        const auto alpha = kLayerAlphas[static_cast<size_t> (layer)];

        drawShape (g, cell.shapeBounds, cell.shapeData, alpha);
    }

    if (selectedCell >= 0 && selectedCell < kNumCells)
    {
        const auto& cell = miniCells[static_cast<size_t> (selectedCell)];
        g.setColour (emberColour.withAlpha (0.40f));
        g.drawRect (cell.shapeBounds, 1);
    }
}

void CellStripPanel::mouseDown (const juce::MouseEvent& event)
{
    const auto pos = event.getPosition();
    for (int i = 0; i < kNumCells; ++i)
    {
        if (miniCells[static_cast<size_t> (i)].shapeBounds.contains (pos))
        {
            selectedCell = i;
            repaint();
            if (onCellClicked)
                onCellClicked (i);
            return;
        }
    }
}

void CellStripPanel::setCellShapeData (int cellIndex, const std::array<float, kWaveTableSize>& data)
{
    if (cellIndex >= 0 && cellIndex < kNumCells)
    {
        miniCells[static_cast<size_t> (cellIndex)].shapeData = data;
        repaint();
    }
}

void CellStripPanel::setSelectedCell (int cellIndex)
{
    selectedCell = cellIndex;
    repaint();
}

juce::Slider& CellStripPanel::getMorphSlider (int cellIndex)
{
    jassert (cellIndex >= 0 && cellIndex < kNumCells);
    return *miniCells[static_cast<size_t> (cellIndex)].morphSlider;
}

void CellStripPanel::drawShape (juce::Graphics& g, juce::Rectangle<int> bounds,
                                const std::array<float, kWaveTableSize>& data, float alpha)
{
    if (bounds.isEmpty())
        return;

    const auto rect = bounds.toFloat();
    juce::Path path;
    for (int i = 0; i < kWaveTableSize; ++i)
    {
        const auto x = rect.getX() + (static_cast<float> (i) / static_cast<float> (kWaveTableSize - 1)) * rect.getWidth();
        const auto y = rect.getCentreY() - (data[static_cast<size_t> (i)] * 0.5f * rect.getHeight());
        if (i == 0)
            path.startNewSubPath (x, y);
        else
            path.lineTo (x, y);
    }
    g.setColour (juce::Colour (0xffef8b48).withAlpha (alpha));
    g.strokePath (path, juce::PathStrokeType (1.2f));
}
