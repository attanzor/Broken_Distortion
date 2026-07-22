# DSP Chain Tests — Design

## Goal

Add JUCE `UnitTest`-based tests for the Gravel DSP chain, compiled into the plugin binary and runnable from AudioPluginHost's test menu.

## File Structure

```
Source/tests/
├── TestUtilities.h
├── TestUtilities.cpp
├── CrossoverSumTest.cpp
├── FftRoundTripTest.cpp
├── NoiseDeterminismTest.cpp
└── ParameterMapTest.cpp
```

All test files are added to `target_sources()` in `CMakeLists.txt`. No changes to `Source/gravel/` (reusable library stays clean).

## Shared Test Utilities (`TestUtilities.h/.cpp`)

| Helper | Signature | Purpose |
|---|---|---|
| `fillSine` | `(AudioBuffer<float>&, freq, sampleRate, numSamples)` | Pure sine tone |
| `fillMultiSine` | `(AudioBuffer<float>&, vector<float> freqs, sampleRate, numSamples)` | Sum of sines spanning crossover region |
| `fillNoise` | `(AudioBuffer<float>&, int seed)` | Uniform white noise |
| `fillImpulse` | `(AudioBuffer<float>&, int sampleIndex)` | Single 1.0 sample, rest 0 |
| `bufferMatches` | `(const AudioBuffer<float>& expected, const AudioBuffer<float>& actual, float tolerance)` | Element-wise compare, logs first mismatch |
| `bufferMatchesWithLatency` | `(input, output, int expectedLatencySamples, float tolerance)` | Same, shifts expected by latency |
| `getRMS` | `(const AudioBuffer<float>&)` | RMS level |

Tolerance defaults: `1e-3f` for band-sum/FFT tests, `1e-6f` for noise determinism.

## Test Cases

### CrossoverSumTest

Verifies that `lowBand + midBand + highBand = original` within tolerance.

- **Setup**: `LinkwitzRileyFilter<float>` (lowpass, highpass, allpass), default 300/3000 Hz
- **Stimulus**: White noise + multi-sine (100 + 1500 + 5000 Hz)
- **Assertion**: `allpass(lowFreq) - low - high` → mid band; `low + mid + high ≈ input` sample-by-sample at `1e-3f`
- **Variations**:
  - Default crossover freqs
  - Minimum spread (clamped near boundary)
  - Maximum spread
  - Mono and stereo buffers
  - Impulse response (no DC buildup)

### FftRoundTripTest

Verifies Hann window → FFT → identity bin processing → IFFT → overlap-add reconstructs the input.

- **Setup**: `SpectralGridProcessor` with identity wavetables (all 9 cells set to flat), band levels = 1.0, EQ bypassed
- **Stimulus**: Bin-centered sine, inter-bin sine, multi-sine, white noise
- **Assertion**: Output matches input after 256-sample FFT latency at `1e-3f`
- **Edge cases**: DC bin, near-zero amplitude

### NoiseDeterminismTest

- **Setup**: `gravel::SmoothNoise`, `gravel::Space`
- **Assertion**: Same seed + same position → same sample; different seed → different sample; `Space::valueFor` range ∈ [0, 1]

### ParameterMapTest

- **Setup**: `gravel::ParameterSpace` with mapped parameter
- **Assertion**: Same seed + possibility → same mapped value; values within `[explMin, explMax]`

## Running

Tests are registered with JUCE's `UnitTest` system. Run from AudioPluginHost: **Tests > Run All Tests**. Results print to the host's log window.
