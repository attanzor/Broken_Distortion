# Task 3: ModulationPopup — SLOPE/JITTER sections

## What was implemented

Added SLOPE and JITTER sections to the ModulationPopup fader settings popup.

## Files changed

- `Source/gravel/ModulationPopup.h` — Added member controls (labels, sliders, toggles) for slope and jitter sections.
- `Source/gravel/ModulationPopup.cpp` — Changed `kPanelHeight` from 420 to 640; added constructor wiring for slope/jitter controls; added destructor cleanup for new LNF pointers; added layout in `resized()`.

## Self-review findings

- All member variables match the brief exactly.
- Constructor code follows the existing pattern (setupLabel, setupSlider, applyLNF, onValueChange).
- Destructor cleans up `setLookAndFeel(nullptr)` for all new sliders.
- Layout in `resized()` mirrors the existing env/expl section pattern with proper padding.
- Jitter speed/spike ranges match slope (assumed — brief only specified slope ranges explicitly).
- No build attempted — this is UI code with no test framework.

## Concerns

- Jitter speed/spike ranges (0–500, skew 5.0 / 0.3–10) are assumed identical to slope since the brief didn't specify otherwise.
