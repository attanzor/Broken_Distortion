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

