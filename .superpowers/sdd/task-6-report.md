# Task 6 Report: 3-Column Editor Layout

**Status**: Complete

## Files Modified

- `Source/PluginEditor.h` — Full rewrite
- `Source/PluginEditor.cpp` — Full rewrite

## What Changed

### PluginEditor.h
- Removed: `effectSliders`, `effectFaders`, `effectAttachments` (arrays of 4), `currentShapeData`, `shapeDisplayBounds`
- Added includes: `"ui/SpectralGridComponent.h"`, `"ui/CellStripPanel.h"`
- Added:
  - `SpectralGridComponent spectralGrid` (left column)
  - `CellStripPanel cellStripPanel` (right column)
  - `selectedCellMorphSlider` + attachment (re-assignable per cell)
  - `preEqFreq/Q/GainSliders` arrays (3 stages × 3 params)
  - `crossFreqLow/MidSlider`, `bandLow/Mid/HighLevelSlider`, `postEqDepthSlider`, `mixSlider`, `outputSlider`
  - `cellShapeData[9][513]` for all 9 cells
  - `mouseDown` override (detects ctrl+click on morph slider to show modulation popup)
  - `onSelectedCellChanged(int)` helper

### PluginEditor.cpp
- **Constructor** (lines 13–159):
  - Kept seed field, randomize button, possibility fader with original setup
  - Created spectralGrid with `onCellClicked` → cellStripPanel sync
  - Created cellStripPanel with `onCellClicked` → spectralGrid sync
  - Created all 9 pre-EQ sliders (freq, Q, gain per stage) and attached to `pre_lowshelf_*`, `pre_bell_*`, `pre_highshelf_*` params
  - Created all 8 global sliders attached to `cross_freq_low/mid`, `band_*_level`, `post_eq_depth`, `mix`, `output`
  - Wired `spectralGrid.onVerticalLineDragged` → log-maps normalized pos to Hz → sets crossover param via `setValueNotifyingHost`
  - Synced initial grid line positions from crossover param values
  - Called `onSelectedCellChanged(0)` for initial state

- **`resized()`** (lines 195–252):
  - Top bar (42px): seedField + randomizeButton
  - Bottom bar (50px): possibilityFader
  - 3 equal-width columns, each reduced by 4px margins
  - Middle column: morph slider (80px), PRE-EQ label (14px), 3 stage rows (44px each with label + 3 sliders), GLOBAL label (14px), 8 param rows (26px each)
  - Pre-EQ rows: 50px label + 3 equal-width slider areas (freq, Q, gain)

- **`onSelectedCellChanged(int cell)`** (lines 183–193):
  - Updates spectralGrid and cellStripPanel selected cell
  - Re-attaches `selectedCellMorphAttachment` to the correct `cell_XX_shape` param ID

- **`mouseDown`** (lines 171–179):
  - Detects ctrl+click on `selectedCellMorphSlider` → shows modulation popup for cell param (indices 15–23 in mappedParamIds)

- **`timerCallback()`** (lines 256–264):
  - Updates all 9 cell shape data arrays from processor
  - Pushes each to spectralGrid and cellStripPanel
  - Updates seedField and repaints

- **`paint()`** (lines 140–192):
  - Original background, border, title, "SEED" label
  - Adds "PRE-EQ", "L.SHELF"/"BELL"/"H.SHELF", "GLOBAL" labels painted onto middle column

- **`showModulationPopup` / `closeModulationPopup`**: Kept with simplified naming ("Cell Morph")

- **Window size**: `1024 × 600` (was 840×540)

## Build Verification

```
cmake --build build --target GravelPossibilityDistortion_All
```

All 4 targets built cleanly: SharedCode, VST3, Standalone, AU. No errors or warnings.
