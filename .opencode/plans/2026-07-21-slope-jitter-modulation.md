# Slope & Jitter Modulation Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax.

**Goal:** Add Slope and Jitter modulation sources to every fader, with per-fader popup controls, green/purple arrow indicators, and global 0-2 multipliers.

**Architecture:** Additive stack — slope (input level derivative, smoothed) and jitter (smooth noise random walk) produce independent values that sum with the existing modulation pipeline. Each source has its own stored amount (set by modifier+drag), display arrow, popup settings, and global gain multiplier.

**Tech Stack:** C++20, JUCE 8, atomics for cross-thread state, SmoothNoise for jitter generation.

---

### Task 1: ModulationState — new atomic fields

**Files:**
- Modify: `Source/gravel/ModulationState.h`

**Interfaces:**
- Consumes: existing `ModulationState` struct pattern
- Produces: 8 new `std::atomic<float>` fields: `slopeSpeed`, `slopeSpikyness`, `slopeAmount`, `slopeApplied`, `jitterSpeed`, `jitterSpikyness`, `jitterAmount`, `jitterApplied`; updated `toValueTree()`/`fromValueTree()`; enable toggles `slopeEnabled`, `jitterEnabled`

- [ ] **Step 1: Add the 8 new atomic fields + 2 enable toggles**

After line 18 (`explModEnabled`), add:
```cpp
    std::atomic<float> slopeSpeed    { 10.0f };
    std::atomic<float> slopeSpikyness { 1.0f };
    std::atomic<float> slopeAmount   { 0.0f };
    std::atomic<float> slopeApplied  { 0.0f };
    std::atomic<bool>  slopeEnabled  { true };
    std::atomic<float> jitterSpeed   { 1.0f };
    std::atomic<float> jitterSpikyness { 1.0f };
    std::atomic<float> jitterAmount  { 0.0f };
    std::atomic<float> jitterApplied { 0.0f };
    std::atomic<bool>  jitterEnabled { true };
```

- [ ] **Step 2: Update `toValueTree()`**

Add after `tree.setProperty("explModEnabled", ...)`:
```cpp
        tree.setProperty ("slopeSpeed",    slopeSpeed.load(),    nullptr);
        tree.setProperty ("slopeSpikyness", slopeSpikyness.load(), nullptr);
        tree.setProperty ("slopeAmount",   slopeAmount.load(),   nullptr);
        tree.setProperty ("slopeEnabled",  slopeEnabled.load(),  nullptr);
        tree.setProperty ("jitterSpeed",   jitterSpeed.load(),   nullptr);
        tree.setProperty ("jitterSpikyness", jitterSpikyness.load(), nullptr);
        tree.setProperty ("jitterAmount",  jitterAmount.load(),  nullptr);
        tree.setProperty ("jitterEnabled", jitterEnabled.load(), nullptr);
```

- [ ] **Step 3: Update `fromValueTree()`**

Add after `explModEnabled.store(...)`:
```cpp
        slopeSpeed.store   (tree.getProperty ("slopeSpeed", 10.0f));
        slopeSpikyness.store (tree.getProperty ("slopeSpikyness", 1.0f));
        slopeAmount.store  (tree.getProperty ("slopeAmount", 0.0f));
        slopeEnabled.store (static_cast<bool> (tree.getProperty ("slopeEnabled", true)));
        jitterSpeed.store  (tree.getProperty ("jitterSpeed", 1.0f));
        jitterSpikyness.store (tree.getProperty ("jitterSpikyness", 1.0f));
        jitterAmount.store (tree.getProperty ("jitterAmount", 0.0f));
        jitterEnabled.store (static_cast<bool> (tree.getProperty ("jitterEnabled", true)));
```

---

### Task 2: RuneFader — green/purple arrows + modifier drag

**Files:**
- Modify: `Source/gravel/RuneFader.h`, `Source/gravel/RuneFader.cpp`

**Interfaces:**
- Consumes: `ModulationState` fields (no direct coupling — stores own float amounts like `targetModulation`)
- Produces: `setSlopeModulation(float)`, `setJitterModulation(float)`, `onSlopeDragged`, `onJitterDragged` callbacks; green/purple arrow colors; 3 drag modes based on modifier keys

- [ ] **Step 1: Add new members to RuneFader.h**

Add after existing `targetModulation` / `appliedModulation` / `arrow` members:

```cpp
    void setSlopeModulation (float amount);
    float getSlopeModulation() const { return slopeTarget; }
    void setJitterModulation (float amount);
    float getJitterModulation() const { return jitterTarget; }

    void setSlopeArrowColour (juce::Colour c) { slopeArrow = c; repaint(); }
    void setJitterArrowColour (juce::Colour c) { jitterArrow = c; repaint(); }

    std::function<void(float)> onSlopeDragged;
    std::function<void(float)> onJitterDragged;

    // in private section, alongside existing modulation members:
    float slopeTarget = 0.0f;
    float slopeSmoothed = 0.0f;
    float jitterTarget = 0.0f;
    float jitterSmoothed = 0.0f;
    bool slopeDragActive = false;
    bool jitterDragActive = false;
    juce::Colour slopeArrow;
    juce::Colour jitterArrow;

    void paintGreenArrow (juce::Graphics& g, float x, float railTop, float railBottom);
    void paintPurpleArrow (juce::Graphics& g, float x, float railTop, float railBottom);
    void paintHorizontalGreenArrow (juce::Graphics& g, float railLeft, float railRight, float y);
    void paintHorizontalPurpleArrow (juce::Graphics& g, float railLeft, float railRight, float y);
```

Initialize `slopeArrow` and `jitterArrow` in the constructor initializer list:
```cpp
      slopeArrow (juce::Colour (0xff4caf50).withAlpha (0.55f)),
      jitterArrow (juce::Colour (0xffab47bc).withAlpha (0.55f)),
```

- [ ] **Step 2: Implement setter methods in RuneFader.cpp**

```cpp
void RuneFader::setSlopeModulation (float amount)
{
    slopeTarget = juce::jlimit (-1.0f, 1.0f, amount);
}

void RuneFader::setJitterModulation (float amount)
{
    jitterTarget = juce::jlimit (-1.0f, 1.0f, amount);
}
```

- [ ] **Step 3: Modify mouseDown() to check modifier keys**

In both primary and secondary branches, before the existing edge-zone logic, add:
```cpp
    if (event.mods.isShiftDown())
    {
        slopeDragActive = true;
        dragDirectionNegative = (relativeX < 0.5f);  // or relativeY
        slopeTarget = 0.0f;
        repaint();
        return;
    }
    if (event.mods.isAltDown())
    {
        jitterDragActive = true;
        dragDirectionNegative = (relativeX < 0.5f);
        jitterTarget = 0.0f;
        repaint();
        return;
    }
```

- [ ] **Step 4: Modify mouseDrag() to handle 3 active flags**

```cpp
    if (slopeDragActive)
    {
        // compute slopeTarget from drag distance (same formula as orange arrow)
        if (onSlopeDragged) onSlopeDragged(slopeTarget);
        repaint();
        return;
    }
    if (jitterDragActive)
    {
        // compute jitterTarget from drag distance
        if (onJitterDragged) onJitterDragged(jitterTarget);
        repaint();
        return;
    }
    // existing arrowDragActive logic...
```

- [ ] **Step 5: Modify mouseUp()**

```cpp
    if (slopeDragActive)
    {
        slopeDragActive = false;
        if (std::abs(slopeTarget) <= 0.01f)
        {
            slopeTarget = 0.0f;
            if (onSlopeDragged) onSlopeDragged(0.0f);
            repaint();
        }
        return;
    }
    if (jitterDragActive)
    {
        jitterDragActive = false;
        if (std::abs(jitterTarget) <= 0.01f)
        {
            jitterTarget = 0.0f;
            if (onJitterDragged) onJitterDragged(0.0f);
            repaint();
        }
        return;
    }
    // existing arrowDragActive logic...
```

- [ ] **Step 6: Add painting calls in paint()**

After white arrow painting, for primary:
```cpp
    slopeSmoothed += (slopeTarget - slopeSmoothed) * 0.18f;
    paintHorizontalGreenArrow(g, railLeft, railRight, y);
    jitterSmoothed += (jitterTarget - jitterSmoothed) * 0.18f;
    paintHorizontalPurpleArrow(g, railLeft, railRight, y);
```

Same for secondary (vertical) with paintGreenArrow/paintPurpleArrow.

- [ ] **Step 7: Implement green/purple paint methods**

Copy `paintHorizontalOrangeArrow` and `paintOrangeArrow` verbatim, replacing `arrow` with `slopeArrow`/`jitterArrow` and renaming.

---

### Task 3: ModulationPopup — SLOPE/JITTER sections

**Files:**
- Modify: `Source/gravel/ModulationPopup.h`, `Source/gravel/ModulationPopup.cpp`

- [ ] **Step 1: Add member controls to ModulationPopup.h**

```cpp
    juce::Label slopeHeader, slopeSpeedLabel, slopeSpikeLabel;
    juce::Slider slopeSpeedSlider, slopeSpikeSlider;
    juce::ToggleButton slopeToggle;
    juce::Label jitterHeader, jitterSpeedLabel, jitterSpikeLabel;
    juce::Slider jitterSpeedSlider, jitterSpikeSlider;
    juce::ToggleButton jitterToggle;
```

Change `kPanelHeight` from `420` to `640`.

- [ ] **Step 2: Constructor — wire slope/jitter sliders**

After existing `explModToggle` setup, add:

```cpp
    slopeHeader.setText ("SLOPE", juce::dontSendNotification);
    slopeHeader.setFont (juce::Font (juce::FontOptions (12.0f)).boldened());
    slopeHeader.setColour (juce::Label::textColourId, accent);
    addAndMakeVisible (slopeHeader);

    setupLabel (slopeSpeedLabel, "SPEED");
    setupSlider (slopeSpeedSlider, state.slopeSpeed.load(), 0.0f, 500.0f, 5.0f);
    applyLNF (slopeSpeedSlider);
    slopeSpeedSlider.onValueChange = [this] { state.slopeSpeed.store (static_cast<float> (slopeSpeedSlider.getValue())); };

    setupLabel (slopeSpikeLabel, "SPIKE");
    setupSlider (slopeSpikeSlider, state.slopeSpikyness.load(), 0.3f, 10.0f);
    applyLNF (slopeSpikeSlider);
    slopeSpikeSlider.onValueChange = [this] { state.slopeSpikyness.store (static_cast<float> (slopeSpikeSlider.getValue())); };

    slopeToggle.setButtonText ("SLOPE");
    slopeToggle.setToggleState (state.slopeEnabled.load(), juce::dontSendNotification);
    slopeToggle.setColour (juce::ToggleButton::textColourId, text);
    slopeToggle.setColour (juce::ToggleButton::tickColourId, accent);
    slopeToggle.setColour (juce::ToggleButton::tickDisabledColourId, text.withAlpha (0.3f));
    slopeToggle.onStateChange = [this] { state.slopeEnabled.store (slopeToggle.getToggleState()); };
    addAndMakeVisible (slopeToggle);

    // Repeat for jitter section (jitterHeader, jitterSpeedLabel, jitterSpikeLabel,
    // jitterSpeedSlider, jitterSpikeSlider, jitterToggle)
    // Use state.jitterSpeed.load(), state.jitterSpikyness.load(), state.jitterEnabled.load()
```

- [ ] **Step 3: Destructor — clean up LNF for new sliders**

```cpp
    slopeSpeedSlider.setLookAndFeel (nullptr);
    slopeSpikeSlider.setLookAndFeel (nullptr);
    jitterSpeedSlider.setLookAndFeel (nullptr);
    jitterSpikeSlider.setLookAndFeel (nullptr);
```

- [ ] **Step 4: Layout in resized()**

After `explModToggle` layout, add:

```cpp
    panel.removeFromTop (kPad);

    slopeHeader.setBounds (panel.removeFromTop (20));
    slopeSpeedLabel.setBounds (panel.removeFromTop (kRowHeight).withTrimmedLeft (kLabelWidth));
    slopeSpeedSlider.setBounds (slopeSpeedLabel.getBounds().removeFromLeft (pw - kLabelWidth * 2 - kPad * 2).withHeight (kSliderH).translated (0, (kRowHeight - kSliderH) / 2));
    slopeSpikeLabel.setBounds (panel.removeFromTop (kRowHeight).withTrimmedLeft (kLabelWidth));
    slopeSpikeSlider.setBounds (slopeSpikeLabel.getBounds().removeFromLeft (pw - kLabelWidth * 2 - kPad * 2).withHeight (kSliderH).translated (0, (kRowHeight - kSliderH) / 2));
    panel.removeFromTop (kPad / 2);
    slopeToggle.setBounds (panel.removeFromTop (24).withSizeKeepingCentre (120, 24));

    panel.removeFromTop (kPad);

    jitterHeader.setBounds (panel.removeFromTop (20));
    jitterSpeedLabel.setBounds (panel.removeFromTop (kRowHeight).withTrimmedLeft (kLabelWidth));
    jitterSpeedSlider.setBounds (jitterSpeedLabel.getBounds().removeFromLeft (pw - kLabelWidth * 2 - kPad * 2).withHeight (kSliderH).translated (0, (kRowHeight - kSliderH) / 2));
    jitterSpikeLabel.setBounds (panel.removeFromTop (kRowHeight).withTrimmedLeft (kLabelWidth));
    jitterSpikeSlider.setBounds (jitterSpikeLabel.getBounds().removeFromLeft (pw - kLabelWidth * 2 - kPad * 2).withHeight (kSliderH).translated (0, (kRowHeight - kSliderH) / 2));
    panel.removeFromTop (kPad / 2);
    jitterToggle.setBounds (panel.removeFromTop (24).withSizeKeepingCentre (120, 24));
```

---

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

### Task 5: PluginEditor — global faders + timer sync

**Files:**
- Modify: `Source/PluginEditor.h`, `Source/PluginEditor.cpp`

- [ ] **Step 1: Add member variables to PluginEditor.h**

```cpp
    juce::Slider slopeSpeedMultSlider, slopeSpikeMultSlider, slopeGainSlider;
    juce::Slider jitterSpeedMultSlider, jitterSpikeMultSlider, jitterGainSlider;
    std::unique_ptr<gravel::RuneFader> slopeSpeedMultFader, slopeSpikeMultFader, slopeGainFader;
    std::unique_ptr<gravel::RuneFader> jitterSpeedMultFader, jitterSpikeMultFader, jitterGainFader;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> slopeSpeedMultAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> slopeSpikeMultAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> slopeGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> jitterSpeedMultAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> jitterSpikeMultAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> jitterGainAttachment;
```

- [ ] **Step 2: Create global faders in constructor**

After existing env fader setup:
```cpp
    auto setupSlopeJitterFader = [&] (juce::Slider& slider, std::unique_ptr<gravel::RuneFader>& fader, const char* label)
    {
        slider.setSliderStyle (juce::Slider::LinearHorizontal);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        fader = std::make_unique<gravel::RuneFader> (slider, label, true,
            voidColour.withAlpha (0.3f), emberColour.withMultipliedSaturation (0.3f));
        fader->onInteraction = [this] { seedField.finishEditing(); };
        addAndMakeVisible (*fader);
    };
    setupSlopeJitterFader (slopeSpeedMultSlider, slopeSpeedMultFader, "SLP SPD");
    setupSlopeJitterFader (slopeSpikeMultSlider, slopeSpikeMultFader, "SLP SPK");
    setupSlopeJitterFader (slopeGainSlider, slopeGainFader, "SLP GAIN");
    setupSlopeJitterFader (jitterSpeedMultSlider, jitterSpeedMultFader, "JIT SPD");
    setupSlopeJitterFader (jitterSpikeMultSlider, jitterSpikeMultFader, "JIT SPK");
    setupSlopeJitterFader (jitterGainSlider, jitterGainFader, "JIT GAIN");
```

Then add attachments:
```cpp
    slopeSpeedMultAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "slope_speed_mult", slopeSpeedMultSlider);
    // ... repeat for all 6
```

- [ ] **Step 3: Layout in resized()**

After env fader layout in left column:
```cpp
    if (slopeSpeedMultFader)
        slopeSpeedMultFader->setBounds (gridFaderArea.removeFromTop (30).reduced (4));
    if (slopeSpikeMultFader)
        slopeSpikeMultFader->setBounds (gridFaderArea.removeFromTop (30).reduced (4));
    if (slopeGainFader)
        slopeGainFader->setBounds (gridFaderArea.removeFromTop (30).reduced (4));
    if (jitterSpeedMultFader)
        jitterSpeedMultFader->setBounds (gridFaderArea.removeFromTop (30).reduced (4));
    if (jitterSpikeMultFader)
        jitterSpikeMultFader->setBounds (gridFaderArea.removeFromTop (30).reduced (4));
    if (jitterGainFader)
        jitterGainFader->setBounds (gridFaderArea.removeFromTop (30).reduced (4));
```

- [ ] **Step 4: Add green/purple arrow sync in timerCallback()**

Inside the `syncFader` lambda or alongside it, add for each fader that shows modulation:
```cpp
        fader->setSlopeModulation (mod.slopeApplied.load());
        fader->setJitterModulation (mod.jitterApplied.load());
```

Add these calls everywhere `setTargetModulation` / `setAppliedModulation` is called.

- [ ] **Step 5: Wire onSlopeDragged / onJitterDragged on every fader**

For each fader with `onModulationDragged`, add:
```cpp
    fader->onSlopeDragged = [this, modIdx] (float amount)
    {
        audioProcessor.getModulationState (modIdx).slopeAmount.store (amount);
    };
    fader->onJitterDragged = [this, modIdx] (float amount)
    {
        audioProcessor.getModulationState (modIdx).jitterAmount.store (amount);
    };
```

---

### Task 6: Build and verify

- [ ] **Step 1:** `cmake --build build --target GravelPossibilityDistortion_All`
- [ ] **Step 2:** Confirm clean build, no warnings.
