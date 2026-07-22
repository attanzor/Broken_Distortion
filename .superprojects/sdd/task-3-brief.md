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

