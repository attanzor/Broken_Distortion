# Task 6: Update PluginEditor with 3-Column Layout

## Context

This is the biggest UI task — wiring everything together into the final 3-column layout. The left column has the SpectralGridComponent, the middle column has the selected cell controls and global params, the right column has the CellStripPanel. The existing bottom exploration fader (possibility) and seed field are kept.

## Files to Modify

- `Source/PluginEditor.h` — class declaration
- `Source/PluginEditor.cpp` — full implementation

## Current State

The editor has been minimally patched to compile (Task 3). The current editor has:
- `seedSlider`, `possibilitySlider`
- 4 `effectSliders` (old drive/mix/output/shape — mix and output kept)
- `seedField`, `randomizeButton`
- `possibilityFader` (RuneFader)
- 4 `effectFaders`
- `currentShapeData` (now expects `getCellShapeData(0)`)
- `shapeDisplayBounds`

## New Requirements

### Header Changes

**Remove:**
- `effectSliders` (array of 4) — replaced by many individual sliders
- `effectFaders` (array of 4 RuneFaders)
- `effectAttachments` (array of 4)
- `currentShapeData` single array — replace with 9-cell data
- `shapeDisplayBounds`
- `closeModulationPopup`, `showModulationPopup`, `modulationPopup`, `modulationPopupIndex` — keep for now but simplify

**Add includes:**
- `#include "ui/SpectralGridComponent.h"`
- `#include "ui/CellStripPanel.h"`

**Add members:**
```cpp
SpectralGridComponent spectralGrid;
CellStripPanel cellStripPanel;

// Selected cell morph slider (large)
juce::Slider selectedCellMorphSlider;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> selectedCellMorphAttachment;

// Pre-EQ sliders (3 stages × 3 params)
std::array<juce::Slider, 3> preEqFreqSliders;
std::array<juce::Slider, 3> preEqQSliders;
std::array<juce::Slider, 3> preEqGainSliders;
std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 3> preEqFreqAttachments;
std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 3> preEqQAttachments;
std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 3> preEqGainAttachments;

// Global params
juce::Slider postEqDepthSlider, crossFreqLowSlider, crossFreqMidSlider;
juce::Slider bandLowLevelSlider, bandMidLevelSlider, bandHighLevelSlider;
juce::Slider mixSlider, outputSlider;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postEqDepthAttachment;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crossFreqLowAttachment;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crossFreqMidAttachment;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bandLowLevelAttachment;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bandMidLevelAttachment;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bandHighLevelAttachment;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;

// Cell data for 9 cells
std::array<std::array<float, 513>, 9> cellShapeData {};

// Keep existing:
juce::Slider seedSlider;
gravel::SeedField seedField;
juce::TextButton randomizeButton;
std::unique_ptr<gravel::RuneFader> possibilityFader;
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> seedAttachment, possibilityAttachment;
```

### Constructor

1. Add spectralGrid (left column):
```cpp
addAndMakeVisible (spectralGrid);
spectralGrid.onCellClicked = [this] (int cell)
{
    cellStripPanel.setSelectedCell (cell);
    onSelectedCellChanged (cell);
};
```

2. Add cellStripPanel (right column):
```cpp
addAndMakeVisible (cellStripPanel);
cellStripPanel.onCellClicked = [this] (int cell)
{
    spectralGrid.setSelectedCell (cell);
    onSelectedCellChanged (cell);
};
```

3. Create selected cell morph slider:
```cpp
selectedCellMorphSlider.setSliderStyle (juce::Slider::LinearHorizontal);
selectedCellMorphSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
selectedCellMorphAttachment = std::make_unique<...> (..., "cell_lq_shape", selectedCellMorphSlider);
addAndMakeVisible (selectedCellMorphSlider);
```

4. Create 9 pre-EQ sliders (3 freqs, 3 Qs, 3 gains). Style: LinearHorizontal, NoTextBox, very thin.

5. Create global param sliders.

6. Attach all sliders to their parameter IDs.

7. Wire up grid line dragging:
```cpp
spectralGrid.onVerticalLineDragged = [this] (int lineIndex, float normPos)
{
    // Convert normalized position to frequency (20-20000 Hz log)
    const auto freqHz = 20.0f * std::pow (20000.0f / 20.0f, normPos);
    auto* param = audioProcessor.parameters.getParameter (
        lineIndex == 0 ? "cross_freq_low" : "cross_freq_mid");
    param->setValueNotifyingHost (param->convertTo0to1 (freqHz));
};
```

8. Keep seed field, randomize button, possibility fader setup as before.

9. Start timer at 60Hz, set size to 1024x600.

### `onSelectedCellChanged(int cell)` helper method

When a cell is selected:
1. Re-attach `selectedCellMorphAttachment` to the correct cell param ID
2. Update the selected cell in both spectralGrid and cellStripPanel
3. Repaint

### `resized()`

Three-column layout:
```
[seed/randomize bar at top, 40px]
[Left: spectral grid      | Middle: controls      | Right: cell strip]
[                                          | selected cell morph      ]
[                                          | pre-EQ (9 faders)        ]
[                                          | global (8 faders)        ]
[Bottom: possibility fader, 50px]
```

Let me be more specific:

```cpp
void GravelPossibilityDistortionAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced (12);

    // Top bar: seed
    auto topBar = bounds.removeFromTop (42);

    // Bottom: possibility
    auto bottomBar = bounds.removeFromBottom (50);
    possibilityFader->setBounds (bottomBar);

    // Three columns
    auto columnWidth = bounds.getWidth() / 3;
    auto leftCol = bounds.removeFromLeft (columnWidth).reduced (4);
    auto midCol = bounds.removeFromLeft (columnWidth).reduced (4);
    auto rightCol = bounds.reduced (4);

    spectralGrid.setBounds (leftCol);

    // Middle column layout
    auto midArea = midCol;
    auto morphArea = midArea.removeFromTop (80);
    selectedCellMorphSlider.setBounds (morphArea.reduced (4));

    // Pre-EQ group
    auto eqLabel = midArea.removeFromTop (14); // "PRE-EQ" text
    for (int i = 0; i < 3; ++i)
    {
        auto row = midArea.removeFromTop (44);
        // row: [label] [freq slider] [Q slider] [gain slider]
        // Use a sub-layout
    }

    // Global group
    auto globalLabel = midArea.removeFromTop (14); // "GLOBAL" text
    // ... crossovers, band levels, mix, output

    cellStripPanel.setBounds (rightCol);
}
```

For the slider arrangement in the middle column, use a simple vertical stack of parameter rows. Each row is ~26px high with a label and slider. Group: label "PRE-EQ" then 9 rows (3 stages × 3 params), then "GLOBAL" then 8 rows.

### Timer

In `timerCallback()`, update all 9 cell shape data arrays:
```cpp
for (int i = 0; i < 9; ++i)
{
    cellShapeData[i] = audioProcessor.getCellShapeData (i);
    spectralGrid.setCellShapeData (i, cellShapeData[i]);
    cellStripPanel.setCellShapeData (i, cellShapeData[i]);
}
```

Also keep the existing seedField update, modulation arrow state update, and repaint.

### `onSelectedCellChanged(int cell)`

```cpp
void GravelPossibilityDistortionAudioProcessorEditor::onSelectedCellChanged (int cell)
{
    spectralGrid.setSelectedCell (cell);
    cellStripPanel.setSelectedCell (cell);

    const std::array<const char*, 9> cellIDs = {
        "cell_lq_shape", "cell_lm_shape", "cell_ll_shape",
        "cell_mq_shape", "cell_mm_shape", "cell_ml_shape",
        "cell_hq_shape", "cell_hm_shape", "cell_hl_shape"
    };

    selectedCellMorphAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, cellIDs[cell], selectedCellMorphSlider);
}
```

### Line-to-frequency conversion

For the grid, the vertical lines use normalized positions (0-1). The crossover frequencies use a log mapping:
```cpp
static float normToFreq (float norm)
{
    return 20.0f * std::pow (20000.0f / 20.0f, norm);
}
static float freqToNorm (float freq)
{
    return std::log (freq / 20.0f) / std::log (20000.0f / 20.0f);
}
```

In the constructor, sync the grid lines to the initial crossover values:
```cpp
spectralGrid.setVerticalLine (0, freqToNorm (parameters.getRawParameterValue ("cross_freq_low")->load()));
spectralGrid.setVerticalLine (1, freqToNorm (parameters.getRawParameterValue ("cross_freq_mid")->load()));
```

### Modulation popup

Keep the existing `ModulationPopup` system but simplified. Since we have ~26 params now, show the popup only for the selected cell's morph slider (ctrl-click on the large morph fader).

## Verification

```bash
cmake --build build --target GravelPossibilityDistortion_All
```

Must compile cleanly and all 4 targets must build.

## Report

Write to: `.superpowers/sdd/task-6-report.md`
