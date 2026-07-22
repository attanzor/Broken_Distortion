# Task 1 Report: DSP Directory and SpectralGridProcessor Skeleton

## Files Created

- `Source/dsp/SpectralGridProcessor.h` — exact class from the brief with `dsp` namespace
- `Source/dsp/SpectralGridProcessor.cpp` — skeleton implementations

## Files Modified

- `CMakeLists.txt` — added both new files to `target_sources`

## Compilation

Build succeeded (`cmake --build build --target GravelPossibilityDistortion_All`) with 12 warnings:

- **-Wshadow (1)**: Parameter `sampleRate` shadows member `sampleRate` — expected given the brief specifies both names
- **-Wsign-conversion (11)**: Implicit `int`→`size_t` conversions in stub setters/constructors — trivial, matching project's own patterns

None of these are errors or would affect runtime behavior.

## Design Concerns

1. **`crossoverMid` type**: Currently set to `allpass` at `crossFreqLow`. The actual 3-band crossover implementation in `processBlock` will need to determine whether `crossoverMid` is used in the dual-output cascaded scheme (where `crossoverLow` + `crossoverHigh` can produce all 3 bands) or as a standalone filter. The `LinkwitzRileyFilter::processSample(int, float, float&, float&)` dual-output overload exists and can give both lowpass and highpass simultaneously — this is the natural approach for a 3-way LR crossover.

2. **`applyEq` uses `const_cast`**: The `IIR::Filter::process()` requires a non-const `ProcessContext`, so `const_cast` was necessary to call it from a const-qualified EQ stage reference. This is correct for JUCE's filter API but worth noting. A future refactor could store iterators instead.

3. **Pre/post EQ filter updates not triggered by setters**: The stub setters (`setPreEqFreq`, `setPreEqQ`, `setPreEqGain`) only store the value without recalculating IIR coefficients. The full implementation will need to call `filter.coefficients = Coefficients::makePeakFilter(...)` after each change.

## Status

DONE

Files: `Source/dsp/SpectralGridProcessor.h`, `Source/dsp/SpectralGridProcessor.cpp`, `CMakeLists.txt`
