# Task 4: SpectralGridComponent — Report

## Status: Complete

## Files Created
- `Source/ui/SpectralGridComponent.h` — Class declaration matching spec (113 lines)
- `Source/ui/SpectralGridComponent.cpp` — Full implementation (274 lines)

## Files Modified
- `CMakeLists.txt` — Added both `Source/ui/SpectralGridComponent.{cpp,h}`

## Implementation Summary

### Header (`SpectralGridComponent.h`)
Exact class from brief, including:
- `CellInfo` struct with `shapeData` (513‑float wavetable) and `bounds`
- `DragTarget` enum covering lines, intersections, and cells
- Public API: set/get lines, set shape data, analyzer data, selection, callbacks
- Private helpers: `recalcGridBounds()`, `drawShape()`, `getCellLeft/Right/Top/Bottom()`

### Source (`SpectralGridComponent.cpp`)

**Anonymous namespace** defines:
- `voidColour` (#d5d0b8), `emberColour` (#ef8b48), `groundColour` (#11100e)
- `kLineTolerance` (6px), `kIntersectionTolerance` (10px), `kPushMargin` (0.05)
- `kLayerAlphas` — {0.55, 0.70, 0.85} for quiet/moderate/loud layers

**`paint()`** draws in order:
1. FFT analyzer bars — thin vertical lines, alpha 0.03→0.12 based on magnitude
2. Cell wavetable shapes — `strokePath` with `emberColour` at layer‑appropriate alpha
3. Grid lines — 2 vertical + 2 horizontal, `voidColour.withAlpha(0.55)`, 1px
4. Handle dots — filled 3px circles at endpoints & intersections, `emberColour.withAlpha(0.70)`
5. Selected cell border — `emberColour.withAlpha(0.40)` 1px rect

**Mouse interaction:**
- **mouseDown**: hit‑tests intersections → vLines → hLines → cells (fall through)
- **mouseDrag**: moves target with push constraint (lines maintain 0.05 margin, can't cross)
- **mouseUp**: fires `onCellClicked`, `onVerticalLineDragged`, or `onHorizontalLineDragged`
- Intersection drags move both lines simultaneously

## Build Verification

```
cmake --build build --target GravelPossibilityDistortion_All
```

All targets compiled and installed successfully (VST3, AU, Standalone). No warnings from SpectralGridComponent.
