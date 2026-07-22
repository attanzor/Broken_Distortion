# Gravel Possibility Distortion

A JUCE control-system prototype for a seed-defined possibility space.

- **Seed**: selects a deterministic set of five smooth noise fields.
- **Possibility**: scrolls continuously through that set, moving every rune control together.
- **Rune 1–5**: readouts for the generated values; they are dummy controls for this first stage.

The audio path currently passes input through unchanged. The reusable control layer lives in the `possibility::SmoothNoise` and `possibility::Space` classes in the processor source.

## Build

Configure with CMake 3.22 or later, then build the default `GravelPossibilityDistortion` target. On the first configure, CMake downloads JUCE 8.0.12 through `FetchContent`.

Every build produces AU and VST3 artifacts and copies them to the macOS system plug-in folders:

- `/Library/Audio/Plug-Ins/Components`
- `/Library/Audio/Plug-Ins/VST3`

This copy may require an account with permission to write to `/Library`.
