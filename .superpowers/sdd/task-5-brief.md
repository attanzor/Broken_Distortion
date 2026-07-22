# Task 5: Create CellStripPanel Component

## Context

Right-column panel showing 9 mini wavetable strips. Each strip displays a shape thumbnail and a thin morph fader. Click a cell to select it.

## Files to Create

- `Source/ui/CellStripPanel.h`
- `Source/ui/CellStripPanel.cpp`

## Files to Modify

- `CMakeLists.txt`

## Header

```cpp
#pragma once

#include <JuceHeader.h>

#include <array>
#include <functional>

class CellStripPanel final : public juce::Component
{
public:
    static constexpr int kNumCells = 9;
    static constexpr int kWaveTableSize = 513;

    CellStripPanel();

    void paint (juce::Graphics&) override;
    void resized() override;

    void setCellShapeData (int cellIndex, const std::array<float, kWaveTableSize>& data);
    void setSelectedCell (int cellIndex);
    int getSelectedCell() const { return selectedCell; }

    juce::Slider& getMorphSlider (int cellIndex);

    std::function<void (int cellIndex)> onCellClicked;

private:
    struct MiniCell
    {
        std::array<float, kWaveTableSize> shapeData {};
        std::unique_ptr<juce::Slider> morphSlider;
        juce::Rectangle<int> bounds;
        juce::Rectangle<int> shapeBounds;
        juce::Rectangle<int> sliderBounds;
    };

    std::array<MiniCell, kNumCells> miniCells;
    int selectedCell = -1;

    void drawShape (juce::Graphics& g, juce::Rectangle<int> bounds,
                    const std::array<float, kWaveTableSize>& data, float alpha);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CellStripPanel)
};
```

## Implementation

### Constructor
Create 9 sliders, each horizontal, range 0-1, value 0.5. Style them to be very thin (1/5 normal Slider height). Use a custom LookAndFeel or just set slider text box size to 0 and thumb size small.

### `resized()`
Divide the component height by 9 for each strip. Each strip has:
- Top 50px: shape thumbnail area
- Bottom 10px: thin horizontal fader
All 9 strips fill the height vertically with small gaps between groups (quiet band → moderate band → loud band).

### `paint()`
For each cell, draw the wavetable shape path in its shapeBounds rect. Use `emberColour` with alpha varying by layer (0.55 for quiet, 0.70 for moderate, 0.85 for loud). If selected, draw a highlighted border in `emberColour.withAlpha(0.4)`.

### `mouseDown()`
Hit test which cell's shapeBounds was clicked, fire `onCellClicked(cellIndex)`, set selected cell.

### `setCellShapeData(int, data)`
Store the shape data for repainting.

### `setSelectedCell(int)`
Update selected cell index, repaint.

### `getMorphSlider(int)`
Return reference to the slider for attachment.

### `drawShape()`
Draw the 513-float data as a path in the given rectangle:
```cpp
void CellStripPanel::drawShape (juce::Graphics& g, juce::Rectangle<int> bounds,
                                const std::array<float, kWaveTableSize>& data, float alpha)
{
    const auto rect = bounds.toFloat();
    juce::Path path;
    for (int i = 0; i < kWaveTableSize; ++i)
    {
        const auto x = rect.getX() + (static_cast<float>(i) / static_cast<float>(kWaveTableSize - 1)) * rect.getWidth();
        const auto y = rect.getCentreY() - (data[i] * 0.5f * rect.getHeight());
        if (i == 0)
            path.startNewSubPath (x, y);
        else
            path.lineTo (x, y);
    }
    g.setColour (juce::Colour (0xffef8b48).withAlpha (alpha));
    g.strokePath (path, juce::PathStrokeType (1.2f));
}
```

### Visual style
- Background: transparent
- Shape color: `juce::Colour(0xffef8b48)` (emberColour) varying alpha
- Selected border: same colour alpha 0.4
- Slider: very thin track, small thumb (or use RuneFader style)

## Verification

```bash
cmake --build build --target GravelPossibilityDistortion_All
```

Must compile (editor linking will fail until Task 6 — expected).

## Report

Write to: `.superpowers/sdd/task-5-report.md`
