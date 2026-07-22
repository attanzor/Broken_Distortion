# Task 4: Create SpectralGridComponent UI

## Context

This is the main visual component — the 3×3 tic-tac-toe grid overlaying a real-time spectrum analyzer. It shows 9 wavetable shapes (one per freq-band × gain-layer combination) with draggable vertical (crossover) and horizontal (amplitude threshold) lines.

## Files to Create

- `Source/ui/SpectralGridComponent.h`
- `Source/ui/SpectralGridComponent.cpp`

## Files to Modify

- `CMakeLists.txt` (add both files)

## Design

The component follows the existing plugin visual language:
- Background: #11100e (groundColour)
- Lines: #d5d0b8 (voidColour) at various alpha levels
- Accent: #ef8b48 (emberColour)
- 1px thin lines, minimal chrome, no filled backgrounds

### Layout

The grid fills the component bounds with padding. The grid area is square (fit to the smaller of width/height). Inside:
- 2 vertical draggable lines (crossover frequencies) at normalized positions 0.33 and 0.66
- 2 horizontal draggable lines (amplitude thresholds) at normalized positions 0.33 and 0.66
- 9 cells are the rectangles bounded by these lines
- Each cell draws its wavetable shape as a thin SVG-style path
- An FFT analyzer draws thin vertical bars behind the grid

### Interaction

- Drag a vertical line → moves the crossover frequency (with push constraint)
- Drag a horizontal line → moves the threshold (with push constraint)
- Drag an intersection → moves both lines
- Click a cell → selects it (highlights border), fires `onCellClicked(cellIndex)`

### Header

```cpp
#pragma once

#include <JuceHeader.h>

#include <array>
#include <functional>
#include <vector>

class SpectralGridComponent final : public juce::Component
{
public:
    static constexpr int kNumBands = 3;
    static constexpr int kNumLayers = 3;
    static constexpr int kNumCells = kNumBands * kNumLayers;
    static constexpr int kWaveTableSize = 513;

    SpectralGridComponent();

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;

    void setCellShapeData (int cellIndex, const std::array<float, kWaveTableSize>& data);
    void setSelectedCell (int cellIndex);
    int getSelectedCell() const { return selectedCell; }

    void setVerticalLine (int index, float normalizedPos);
    void setHorizontalLine (int index, float normalizedPos);
    float getVerticalLine (int index) const;
    float getHorizontalLine (int index) const;

    void setAnalyzerData (const std::vector<float>& magnitudes);

    std::function<void (int cellIndex)> onCellClicked;
    std::function<void (int lineIndex, float normalizedPos)> onVerticalLineDragged;
    std::function<void (int lineIndex, float normalizedPos)> onHorizontalLineDragged;

private:
    struct CellInfo
    {
        std::array<float, kWaveTableSize> shapeData {};
        juce::Rectangle<float> bounds;
    };

    std::array<CellInfo, kNumCells> cells;
    int selectedCell = -1;

    std::array<float, 2> vLinePos = { 0.33f, 0.66f };
    std::array<float, 2> hLinePos = { 0.33f, 0.66f };

    enum class DragTarget { None, VLine0, VLine1, HLine0, HLine1, Intersection00, Intersection01, Intersection10, Intersection11, Cell };
    DragTarget dragTarget = DragTarget::None;
    int dragCellIndex = -1;
    float dragStartX = 0, dragStartY = 0;

    std::vector<float> analyzerData;
    juce::Rectangle<int> gridBounds;

    void recalcGridBounds();
    void drawShape (juce::Graphics& g, juce::Rectangle<float> bounds, const std::array<float, kWaveTableSize>& data, float alpha);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralGridComponent)
};
```

### Implementation Notes

**`paint()` method:**
1. Draw FFT analyzer: thin vertical bars filling the grid area, use `emberColour.withAlpha(0.12)` fading to `emberColour.withAlpha(0.03)`
2. Draw 9 cell wavetable shapes: for each cell compute its bounds from the 4 lines, draw the shape path in `emberColour.withAlpha(alpha)` where alpha varies by layer (quiet=0.55, moderate=0.7, loud=0.85)
3. Draw grid lines: 2 vertical (at vLinePos), 2 horizontal (at hLinePos) in `voidColour.withAlpha(0.55)`, 1px width
4. Draw handle dots at line endpoints and intersections: small filled circles in `emberColour.withAlpha(0.7)`
5. Draw selected cell border: `emberColour.withAlpha(0.4)` 1px outline

**`resized()`:** Recalculate `gridBounds` — a square centered in the component.

**Hit testing:** 6px tolerance for lines, 10px for intersections.

**Push constraint:** vertical lines maintain `vLinePos[0] < vLinePos[1]` with 0.05 margin. Same for horizontal.

**Line-to-frequency mapping:**
The normalized vLinePos (0-1 in grid area) maps to frequency range (20-20000Hz logarithmically) via the editor passing the value to the processor. The component just stores [0,1] normalized positions.

**Shape drawing:**
For each cell, find the grid area rectangle, normalize the wavetable data (513 floats, range -1 to 1) into that rectangle, draw as a path with `strokePath`.

## Verification

```bash
cmake --build build --target GravelPossibilityDistortion_All
```

Must compile (editor integration will fail until Task 6 but the component itself must compile).

## Report

Write to: `.superpowers/sdd/task-4-report.md`
