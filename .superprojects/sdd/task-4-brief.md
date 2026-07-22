### Task 4: PluginProcessor — global params + DSP pipeline

**Files:**
- Modify: `Source/PluginProcessor.h`, `Source/PluginProcessor.cpp`

- [ ] **Step 1: Add per-parameter state arrays to PluginProcessor.h**

```cpp
    std::array<float, kNumMappedParams> slopeFilterStates {};
    std::array<float, kNumMappedParams> jitterPhases {};
```

- [ ] **Step 2: Add 6 global APVTS params in createParameterLayout()**

After the env_gain parameter:
```cpp
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("slope_speed_mult", "Slope Speed Mult",
        juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f, 2.0f), 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("slope_spike_mult", "Slope Spike Mult",
        juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f, 2.0f), 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("slope_gain", "Slope Gain",
        juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f, 2.0f), 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("jitter_speed_mult", "Jitter Speed Mult",
        juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f, 2.0f), 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("jitter_spike_mult", "Jitter Spike Mult",
        juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f, 2.0f), 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("jitter_gain", "Jitter Gain",
        juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f, 2.0f), 1.0f));
```

- [ ] **Step 3: Add parameter listeners in constructor**

```cpp
    for (const auto* id : { "slope_speed_mult", "slope_spike_mult", "slope_gain",
                             "jitter_speed_mult", "jitter_spike_mult", "jitter_gain" })
        parameters.addParameterListener (id, this);
```

Same in destructor with `removeParameterListener`.

- [ ] **Step 4: Load globals and compute slope/jitter in processBlock()**

At top of processBlock, add:
```cpp
    const auto globalSlopeSpeedMult = parameters.getRawParameterValue ("slope_speed_mult")->load();
    const auto globalSlopeSpikeMult = parameters.getRawParameterValue ("slope_spike_mult")->load();
    const auto globalSlopeGain      = parameters.getRawParameterValue ("slope_gain")->load();
    const auto globalJitterSpeedMult = parameters.getRawParameterValue ("jitter_speed_mult")->load();
    const auto globalJitterSpikeMult = parameters.getRawParameterValue ("jitter_spike_mult")->load();
    const auto globalJitterGain      = parameters.getRawParameterValue ("jitter_gain")->load();
```

Before the per-sample loop, add:
```cpp
    float prevInputLevel = 0.0f;
```

Inside per-sample loop, after `inputLevel /= numChannels;`, add:
```cpp
        // Slope — signed derivative of input level
        const auto slopeRaw = inputLevel - prevInputLevel;
        prevInputLevel = inputLevel;

        for (int i = 0; i < kNumMappedParams; ++i)
        {
            auto& mod = modulationStates[static_cast<size_t> (i)];

            // Slope smoothing (one-pole) using the per-param filter state
            const auto speedMs = juce::jmax (0.001f, mod.slopeSpeed.load() * globalSlopeSpeedMult);
            const auto coeff = speedMs < 0.5f ? 1.0f : std::exp (-1000.0f / static_cast<float> (getSampleRate() * speedMs * 0.001f));
            slopeFilterStates[static_cast<size_t> (i)] += (slopeRaw - slopeFilterStates[static_cast<size_t> (i)]) * coeff;
            const auto slopeState = slopeFilterStates[static_cast<size_t> (i)];

            // Spikyness pow
            const auto spike = juce::jmax (0.01f, mod.slopeSpikyness.load() * globalSlopeSpikeMult);
            const auto absSlope = std::abs (slopeState);
            const auto shaped = std::pow (absSlope, spike);
            const auto slopeValue = (slopeState >= 0.0f ? shaped : -shaped);
            mod.slopeApplied.store (slopeValue * mod.slopeAmount.load() * globalSlopeGain);

            // Jitter random walk
            auto& jPhase = jitterPhases[static_cast<size_t> (i)];
            jPhase += mod.jitterSpeed.load() * globalJitterSpeedMult * static_cast<float> (1.0 / getSampleRate());
            const auto jRaw = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 50 + 500), jPhase) * 2.0f - 1.0f;
            const auto jSpike = juce::jmax (0.01f, mod.jitterSpikyness.load() * globalJitterSpikeMult);
            const auto jAbs = std::abs (jRaw);
            const auto jShaped = std::pow (jAbs, jSpike);
            const auto jitterValue = (jRaw >= 0.0f ? jShaped : -jShaped);
            mod.jitterApplied.store (jitterValue * mod.jitterAmount.load() * globalJitterGain);
        }
```

- [ ] **Step 5: Modify final modulation stacking to include slope + jitter**

Change the `appliedModulation` computation in the per-block loop (after the per-sample loop):
```cpp
        auto slopeApplied = mod.slopeApplied.load();
        auto jitterApplied = mod.jitterApplied.load();
        if (! mod.slopeEnabled.load()) slopeApplied = 0.0f;
        if (! mod.jitterEnabled.load()) jitterApplied = 0.0f;
        const auto applied = juce::jlimit (-1.0f, 1.0f,
            envNorm * mod.envGain.load() * globalGainMul * effectiveMod
            + slopeApplied + jitterApplied);
        mod.appliedModulation.store (applied);
```

- [ ] **Step 6: Reset slope/jitter state in prepareToPlay()**

```cpp
    slopeFilterStates = {};
    jitterPhases = {};
```

---

