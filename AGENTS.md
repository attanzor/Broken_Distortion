# Gravel Possibility Distortion — Project Instructions

## Project overview

This is a JUCE 8 audio plugin (AU, VST3, Standalone) built with CMake. It
generates seed-defined noise fields to modulate audio. The core DSP and UI
components live in `Source/gravel/` — a reusable library that can be copied
into any future JUCE plugin project.

All agents should understand the domain before making changes.

## Code conventions

- **Language**: C++20. Use JUCE 8 APIs where possible.
- **Naming**:
  - Classes: `PascalCase` (e.g. `SmoothNoise`, `RuneFader`).
  - Methods/functions: `camelCase` (e.g. `rebuildWaveTable()`).
  - Member variables: `camelCase` with no prefix (e.g. `seedSlider`). Avoid `m_` or `m` prefixes.
  - Constants: `k` prefix + PascalCase (e.g. `kWaveTableSize`).
  - Namespaces: `snake_case` (e.g. `gravel`).
- **Style**:
  - Space before opening parenthesis in function declarations/definitions: `void paint (juce::Graphics&)`.
  - `final` on classes not intended for inheritance.
  - `override` on all overridden virtuals.
  - Use `auto` judiciously. Prefer explicit types in public interfaces.
  - Include `JuceHeader.h` via `#include <JuceHeader.h>` in headers that need JUCE types.
  - Use `#pragma once` for include guards.
  - Always use `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR` for component/processor classes.
- **Memory**: Prefer `std::unique_ptr` over raw `new`/`delete`. Avoid JUCE's
  `ScopedPointer` (deprecated in JUCE 8).

## Gravel library (`Source/gravel/`)

This directory is the reusable core. To use in a new project:

1. Copy the entire `Source/gravel/` folder into your `Source/`.
2. Add all `.cpp` and `.h` files from it to your `target_sources()`.
3. Create a `gravel::ParameterSpace` member in your processor and call
   `mapParameters()` with `{ paramID, fieldIndex }` pairs.
4. Create `gravel::RuneFader` and `gravel::SeedField` components in your editor.

The library is designed with zero internal dependencies on plugin-specific
code. Colour schemes in `RuneFader` and `SeedField` are configurable via
constructor parameters.

### Available components

| Class | File | Purpose |
|---|---|---|
| `gravel::SmoothNoise` | `PossibilitySpace.h/.cpp` | Deterministic value noise with quintic interpolation |
| `gravel::Space` | `PossibilitySpace.h/.cpp` | Three-octave noise field, normalized per parameter |
| `gravel::ParameterSpace` | `ParameterSpace.h/.cpp` | Bridges noise space to APVTS; manages parameter slaving |
| `gravel::RuneFader` | `RuneFader.h/.cpp` | Visual fader (horizontal or vertical) with custom colours |
| `gravel::SeedField` | `SeedField.h/.cpp` | Editable seed display (click to edit, keyboard input) |

### Using `ParameterSpace`

```cpp
// In your processor constructor:
possibilitySpace.mapParameters ({
    { "drive",  0 },   // paramID, fieldIndex
    { "mix",    1 },
    { "output", 2 },
});

// In your processor's parameterChanged:
void parameterChanged (const String& id, float val) override
{
    if (possibilitySpace.handleParameterChange (id, val))
        return;  // seed or possibility was handled

    if (id == "myParam")
        doSomething();
}
```

### Adding a new parameter to the space

1. Add the parameter to `createParameterLayout()`.
2. Call `possibilitySpace.mapParameter ("newParam", fieldIndex)`.
3. Create a `RuneFader` in the editor (or let the space drive it invisibly).

The field index selects which independent noise channel drives the parameter.
Every seed produces a unique landscape for each field index.

## Build system

- CMake 3.22+. JUCE is fetched via `FetchContent`, tag `8.0.12`.
- Build artifacts are auto-copied to `/Library/Audio/Plug-Ins/` (macOS).
- Do not add new dependencies without justification.

## Build and deploy

After any source or CMake change, rebuild the full AU/VST3 install target so
the plugin can be tested directly in a DAW:

    cmake --build build --target GravelPossibilityDistortion_All

This builds all formats (AU, VST3, Standalone) and copies them to
`/Library/Audio/Plug-Ins/Components` and
`/Library/Audio/Plug-Ins/VST3`.

Always target `_All` rather than a single format. The AU and VST3 bundles must
both be refreshed to cover testing in different DAWs. Do not redirect install
paths to build-only or user-local folders unless the user explicitly requests
it.

## Audio / DSP conventions

- Process in `float` precision. Use `juce::dsp` for oversampling, filters, etc.
- Parameter IDs use `snake_case` strings (e.g. `"seed"`, `"possibility"`).
- The `processBlock` output must be latency-compensated when oversampling is active.

## Testing

- No test framework is set up yet. Test via the plugin host (e.g. a DAW or
  JUCE's AudioPluginHost). Do not introduce a test framework without discussion.

## Git

- Commit messages: concise imperative mood, no trailing period, max 72 chars.
- Keep commits focused on a single logical change.
