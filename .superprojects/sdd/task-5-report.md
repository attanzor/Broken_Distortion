# Task 5 Report: PluginEditor — global faders + timer sync

## What was implemented

1. **PluginEditor.h**: Added 18 member variables — 6 `juce::Slider`, 6 `std::unique_ptr<gravel::RuneFader>`, and 6 `std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>` for the 6 slope/jitter global param faders.

2. **PluginEditor.cpp — Constructor**: Created a `setupSlopeJitterFader` lambda (same pattern as `setupEnvFader`) and used it to create faders for SLP SPD, SLP SPK, SLP GAIN, JIT SPD, JIT SPK, JIT GAIN. Added 6 APVTS `SliderAttachment`s for `slope_speed_mult`, `slope_spike_mult`, `slope_gain`, `jitter_speed_mult`, `jitter_spike_mult`, `jitter_gain`.

3. **PluginEditor.cpp — resized()**: Added layout for all 6 slope/jitter faders in the left column, after the env faders (30px each, reduced 4px).

4. **PluginEditor.cpp — timerCallback()**: Added `fader->setSlopeModulation(mod.slopeApplied.load())` and `fader->setJitterModulation(mod.jitterApplied.load())` inside the `syncFader` lambda, so every modulated fader gets green/purple arrow synced every frame.

5. **PluginEditor.cpp — onSlopeDragged/onJitterDragged**: Added these callbacks on every fader that had `onModulationDragged`:
   - `selectedCellMorphFader` (modIdx = 15 + cell)
   - 9 `cellMorphFaders` (modIdx = 15 + i)
   - 9 `cellDriveFaders` (modIdx = 29 + i)
   - 3 `preEqFreqFaders` (modIdx = i)
   - 3 `preEqQFaders` (modIdx = 3 + i)
   - 3 `preEqGainFaders` (modIdx = 6 + i)
   - All `setupGlobalFader`-created faders (post_eq_depth, cross_freq_low/mid, band_low/mid/high, mix, output — plus thresh_low, thresh_high via same lambda)
   - `driveFader` (modIdx = 28)

## Files changed

- `Source/PluginEditor.h` (+12 lines)
- `Source/PluginEditor.cpp` (+112 lines)

## Self-review findings

- All changes follow existing patterns (lambda-based fader creation, `syncFader` lambda pattern in timer, `onModulationDragged` callback style).
- The slope/jitter faders don't have `onModulationDragged`, `onCtrlClick`, or arrow sync (they're unmodulated global params, like env faders).
- Build compiles cleanly with no warnings.

## Concerns

None.
