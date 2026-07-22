#include "PluginEditor.h"

namespace
{
const auto voidColour = juce::Colour (0xffd5d0b8);
const auto emberColour = juce::Colour (0xffef8b48);
const auto groundColour = juce::Colour (0xff11100e);

float normToFreq (float norm)
{
    return 20.0f * std::pow (20000.0f / 20.0f, norm);
}

float freqToNorm (float freq)
{
    return std::log (freq / 20.0f) / std::log (20000.0f / 20.0f);
}

static constexpr std::array<const char*, 9> kCellParamIDs = {
    "cell_lq_shape", "cell_lm_shape", "cell_ll_shape",
    "cell_mq_shape", "cell_mm_shape", "cell_ml_shape",
    "cell_hq_shape", "cell_hm_shape", "cell_hl_shape"
};

static constexpr std::array<const char*, 9> kCellDriveParamIDs = {
    "cell_lq_drive", "cell_lm_drive", "cell_ll_drive",
    "cell_mq_drive", "cell_mm_drive", "cell_ml_drive",
    "cell_hq_drive", "cell_hm_drive", "cell_hl_drive"
};

static constexpr std::array<const char*, 3> kPreEqFreqIDs = {
    "pre_lowshelf_freq", "pre_bell_freq", "pre_highshelf_freq"
};
static constexpr std::array<const char*, 3> kPreEqQIDs = {
    "pre_lowshelf_q", "pre_bell_q", "pre_highshelf_q"
};
static constexpr std::array<const char*, 3> kPreEqGainIDs = {
    "pre_lowshelf_gain", "pre_bell_gain", "pre_highshelf_gain"
};

static constexpr std::array<const char*, 3> kPreEqFreqLabels = { "LOW F", "MID F", "HI F" };
static constexpr std::array<const char*, 3> kPreEqQLabels =    { "LOW Q", "MID Q", "HI Q" };
static constexpr std::array<const char*, 3> kPreEqGainLabels = { "LOW G", "MID G", "HI G" };

static constexpr int kNumGlobalParams = 6;

struct GlobalParamDef
{
    const char* paramID;
    const char* label;
    int modIndex;
};

static constexpr std::array<GlobalParamDef, kNumGlobalParams> kGlobalParams = { {
    { "post_eq_depth",  "P.EQ DPTH", 9 },
    { "cross_freq_low", "XOVR LOW",  10 },
    { "cross_freq_mid", "XOVR MID",  11 },
    { "band_low_level", "LOW LVL",   12 },
    { "band_mid_level", "MID LVL",   13 },
    { "band_high_level","HI LVL",    14 },
} };
}

BrokenAudioProcessorEditor::BrokenAudioProcessorEditor (
    BrokenAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setWantsKeyboardFocus (true);
    setMouseClickGrabsKeyboardFocus (false);

    // Top bar: seed field and randomize button
    seedField.onSeedCommitted = [this] (int value)
    {
        seedSlider.setValue (value, juce::sendNotificationSync);
    };
    addAndMakeVisible (seedField);

    randomizeButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    randomizeButton.setColour (juce::TextButton::buttonOnColourId, emberColour.withAlpha (0.20f));
    randomizeButton.setColour (juce::TextButton::textColourOffId, voidColour.withAlpha (0.82f));
    randomizeButton.setColour (juce::TextButton::textColourOnId, emberColour);
    randomizeButton.onClick = [this]
    {
        auto nextSeed = juce::Random::getSystemRandom().nextInt (1000000);
        if (nextSeed == juce::roundToInt (seedSlider.getValue()))
            nextSeed = (nextSeed + 1) % 1000000;

        seedSlider.setValue (nextSeed, juce::sendNotificationSync);
        seedField.setSeed (nextSeed);
    };
    addAndMakeVisible (randomizeButton);

    // Possibility fader
    possibilityFader = std::make_unique<gravel::FloatFader> ("Possibility");
    possibilityFader->onValueChanged = [this] (float norm)
    {
        auto* param = audioProcessor.parameters.getParameter ("possibility");
        if (param != nullptr)
            param->setValueNotifyingHost (norm);
        seedField.finishEditing();
    };
    addAndMakeVisible (*possibilityFader);

    // Global mod amount (vertical, right of matrix)
    globalModSlider.setSliderStyle (juce::Slider::LinearVertical);
    globalModSlider.setRange (0.0, 1.0, 0.001);
    globalModSlider.setSkewFactorFromMidPoint (0.25);
    globalModSlider.setValue (0.25, juce::dontSendNotification);
    globalModSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    globalModFader = std::make_unique<gravel::RuneFader> (
        globalModSlider, "", false,
        voidColour.withAlpha (0.3f), emberColour.withMultipliedSaturation (0.3f));
    globalModFader->setWantsKeyboardFocus (false);
    globalModFader->onInteraction = [this] { seedField.finishEditing(); };
    addAndMakeVisible (*globalModFader);
    globalModAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "global_mod_amount", globalModSlider);

    // Spectral grid
    addAndMakeVisible (spectralGrid);
    spectralGrid.onCellClicked = [this] (int cell)
    {
        onSelectedCellChanged (cell);
    };

    // --- Middle column: selected cell morph fader (horizontal) ---
    selectedCellMorphFader = std::make_unique<gravel::RuneFader> (
        selectedCellMorphSlider, "CELL MORPH", true,
        juce::Colour (0xffd5d0b8), juce::Colour (0xffef8b48));
    selectedCellMorphFader->onInteraction = [this] { seedField.finishEditing(); };
    selectedCellMorphFader->onModulationDragged = [this] (float modAmount)
    {
        const auto cell = spectralGrid.getSelectedCell();
        if (cell >= 0)
        {
            auto& mod = audioProcessor.getModulationState (15 + cell);
            mod.manualModulation.store (modAmount);
        }
    };
    selectedCellMorphFader->onSlopeDragged = [this] (float amount)
    {
        const auto cell = spectralGrid.getSelectedCell();
        if (cell >= 0)
            audioProcessor.getModulationState (15 + cell).slopeAmount.store (amount);
    };
    selectedCellMorphFader->onJitterDragged = [this] (float amount)
    {
        const auto cell = spectralGrid.getSelectedCell();
        if (cell >= 0)
            audioProcessor.getModulationState (15 + cell).jitterAmount.store (amount);
    };
    selectedCellMorphFader->onCtrlClick = [this]
    {
        const auto cell = spectralGrid.getSelectedCell();
        if (cell >= 0)
            showModulationPopup (15 + cell);
    };
    addAndMakeVisible (*selectedCellMorphFader);

    // --- Right column: 9 small cell morph faders (horizontal, no labels) ---
    for (int i = 0; i < 9; ++i)
    {
        const auto idx = static_cast<size_t> (i);
        auto& slider = cellMorphSliders[idx];
        slider.setSliderStyle (juce::Slider::LinearHorizontal);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);

        cellMorphFaders[idx] = std::make_unique<gravel::RuneFader> (
            slider, "", true, juce::Colour (0xffd5d0b8), juce::Colour (0xffef8b48));
        cellMorphFaders[idx]->setWantsKeyboardFocus (false);
        cellMorphFaders[idx]->onInteraction = [this] { seedField.finishEditing(); };
        cellMorphFaders[idx]->onModulationDragged = [this, modIdx = 15 + i] (float modAmount)
        {
            auto& mod = audioProcessor.getModulationState (modIdx);
            mod.manualModulation.store (modAmount);
        };
        cellMorphFaders[idx]->onSlopeDragged = [this, modIdx = 15 + i] (float amount)
        {
            audioProcessor.getModulationState (modIdx).slopeAmount.store (amount);
        };
        cellMorphFaders[idx]->onJitterDragged = [this, modIdx = 15 + i] (float amount)
        {
            audioProcessor.getModulationState (modIdx).jitterAmount.store (amount);
        };
        cellMorphFaders[idx]->onCtrlClick = [this, modIdx = 15 + i] { showModulationPopup (modIdx); };
        addAndMakeVisible (*cellMorphFaders[idx]);

        cellMorphAttachments[idx] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            audioProcessor.parameters, kCellParamIDs[idx], slider);

        auto& cellDriveSlider = cellDriveSliders[idx];
        cellDriveSlider.setSliderStyle (juce::Slider::LinearHorizontal);
        cellDriveSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        cellDriveFaders[idx] = std::make_unique<gravel::RuneFader> (
            cellDriveSlider, "", true,
            voidColour.withAlpha (0.3f), emberColour.withMultipliedSaturation (0.3f));
        cellDriveFaders[idx]->setWantsKeyboardFocus (false);
        cellDriveFaders[idx]->onInteraction = [this] { seedField.finishEditing(); };
        cellDriveFaders[idx]->onModulationDragged = [this, modIdx = 29 + i] (float modAmount)
        {
            auto& mod = audioProcessor.getModulationState (modIdx);
            mod.manualModulation.store (modAmount);
        };
        cellDriveFaders[idx]->onSlopeDragged = [this, modIdx = 29 + i] (float amount)
        {
            audioProcessor.getModulationState (modIdx).slopeAmount.store (amount);
        };
        cellDriveFaders[idx]->onJitterDragged = [this, modIdx = 29 + i] (float amount)
        {
            audioProcessor.getModulationState (modIdx).jitterAmount.store (amount);
        };
        cellDriveFaders[idx]->onCtrlClick = [this, modIdx = 29 + i] { showModulationPopup (modIdx); };
        addAndMakeVisible (*cellDriveFaders[idx]);
        cellDriveAttachments[idx] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            audioProcessor.parameters, kCellDriveParamIDs[idx], cellDriveSlider);
    }

    // --- Pre-EQ faders (horizontal) ---
    for (int i = 0; i < 3; ++i)
    {
        const auto idx = static_cast<size_t> (i);

        preEqFreqFaders[idx] = std::make_unique<gravel::RuneFader> (
            preEqFreqSliders[idx], kPreEqFreqLabels[idx], true);
        preEqFreqFaders[idx]->onInteraction = [this] { seedField.finishEditing(); };
        preEqFreqFaders[idx]->onModulationDragged = [this, modIdx = i] (float modAmount)
        {
            auto& mod = audioProcessor.getModulationState (modIdx);
            mod.manualModulation.store (modAmount);
        };
        preEqFreqFaders[idx]->onSlopeDragged = [this, modIdx = i] (float amount)
        {
            audioProcessor.getModulationState (modIdx).slopeAmount.store (amount);
        };
        preEqFreqFaders[idx]->onJitterDragged = [this, modIdx = i] (float amount)
        {
            audioProcessor.getModulationState (modIdx).jitterAmount.store (amount);
        };
        preEqFreqFaders[idx]->onCtrlClick = [this, modIdx = i] { showModulationPopup (modIdx); };
        addAndMakeVisible (*preEqFreqFaders[idx]);

        preEqQFaders[idx] = std::make_unique<gravel::RuneFader> (
            preEqQSliders[idx], kPreEqQLabels[idx], true);
        preEqQFaders[idx]->onInteraction = [this] { seedField.finishEditing(); };
        preEqQFaders[idx]->onModulationDragged = [this, modIdx = 3 + i] (float modAmount)
        {
            auto& mod = audioProcessor.getModulationState (modIdx);
            mod.manualModulation.store (modAmount);
        };
        preEqQFaders[idx]->onSlopeDragged = [this, modIdx = 3 + i] (float amount)
        {
            audioProcessor.getModulationState (modIdx).slopeAmount.store (amount);
        };
        preEqQFaders[idx]->onJitterDragged = [this, modIdx = 3 + i] (float amount)
        {
            audioProcessor.getModulationState (modIdx).jitterAmount.store (amount);
        };
        preEqQFaders[idx]->onCtrlClick = [this, modIdx = 3 + i] { showModulationPopup (modIdx); };
        addAndMakeVisible (*preEqQFaders[idx]);

        preEqGainFaders[idx] = std::make_unique<gravel::RuneFader> (
            preEqGainSliders[idx], kPreEqGainLabels[idx], true);
        preEqGainFaders[idx]->onInteraction = [this] { seedField.finishEditing(); };
        preEqGainFaders[idx]->onModulationDragged = [this, modIdx = 6 + i] (float modAmount)
        {
            auto& mod = audioProcessor.getModulationState (modIdx);
            mod.manualModulation.store (modAmount);
        };
        preEqGainFaders[idx]->onSlopeDragged = [this, modIdx = 6 + i] (float amount)
        {
            audioProcessor.getModulationState (modIdx).slopeAmount.store (amount);
        };
        preEqGainFaders[idx]->onJitterDragged = [this, modIdx = 6 + i] (float amount)
        {
            audioProcessor.getModulationState (modIdx).jitterAmount.store (amount);
        };
        preEqGainFaders[idx]->onCtrlClick = [this, modIdx = 6 + i] { showModulationPopup (modIdx); };
        addAndMakeVisible (*preEqGainFaders[idx]);

        preEqFreqAttachments[idx] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            audioProcessor.parameters, kPreEqFreqIDs[idx], preEqFreqSliders[idx]);
        preEqQAttachments[idx] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            audioProcessor.parameters, kPreEqQIDs[idx], preEqQSliders[idx]);
        preEqGainAttachments[idx] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            audioProcessor.parameters, kPreEqGainIDs[idx], preEqGainSliders[idx]);
    }

    // --- Global faders ---
    auto setupGlobalFader = [&] (juce::Slider& slider, std::unique_ptr<gravel::RuneFader>& fader,
                                 const char* label, int modIdx)
    {
        slider.setSliderStyle (juce::Slider::LinearHorizontal);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);

        fader = std::make_unique<gravel::RuneFader> (slider, label, true,
            voidColour.withAlpha (0.3f), emberColour.withMultipliedSaturation (0.3f));
        fader->onInteraction = [this] { seedField.finishEditing(); };
        fader->onModulationDragged = [this, modIdx] (float modAmount)
        {
            auto& mod = audioProcessor.getModulationState (modIdx);
            mod.manualModulation.store (modAmount);
        };
        fader->onSlopeDragged = [this, modIdx] (float amount)
        {
            audioProcessor.getModulationState (modIdx).slopeAmount.store (amount);
        };
        fader->onJitterDragged = [this, modIdx] (float amount)
        {
            audioProcessor.getModulationState (modIdx).jitterAmount.store (amount);
        };
        fader->onCtrlClick = [this, modIdx] { showModulationPopup (modIdx); };
        addAndMakeVisible (*fader);
    };

    for (int i = 0; i < kNumGlobalParams; ++i)
    {
        const auto& g = kGlobalParams[static_cast<size_t> (i)];
        juce::Slider* slider = nullptr;
        std::unique_ptr<gravel::RuneFader>* fader = nullptr;

        if (i == 0)      { slider = &postEqDepthSlider;   fader = &postEqDepthFader;  }
        else if (i == 1) { slider = &crossFreqLowSlider;  fader = &crossFreqLowFader; }
        else if (i == 2) { slider = &crossFreqMidSlider;  fader = &crossFreqMidFader; }
        else if (i == 3) { slider = &bandLowLevelSlider;  fader = &bandLowLevelFader; }
        else if (i == 4) { slider = &bandMidLevelSlider;  fader = &bandMidLevelFader; }
        else if (i == 5) { slider = &bandHighLevelSlider; fader = &bandHighLevelFader; }

        if (slider != nullptr && fader != nullptr)
        {
            setupGlobalFader (*slider, *fader, g.label, g.modIndex);
        }
    }

    // Threshold faders
    setupGlobalFader (threshLowSlider, threshLowFader, "THRESH L", 26);
    setupGlobalFader (threshHighSlider, threshHighFader, "THRESH H", 27);

    // Drive (vertical)
    driveSlider.setSliderStyle (juce::Slider::LinearVertical);
    driveSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    driveFader = std::make_unique<gravel::RuneFader> (driveSlider, "DRIVE", false,
        voidColour.withAlpha (0.3f), emberColour.withMultipliedSaturation (0.3f));
    driveFader->onInteraction = [this] { seedField.finishEditing(); };
    driveFader->onModulationDragged = [this] (float modAmount)
    {
        auto& mod = audioProcessor.getModulationState (28);
        mod.manualModulation.store (modAmount);
    };
    driveFader->onSlopeDragged = [this] (float amount)
    {
        audioProcessor.getModulationState (28).slopeAmount.store (amount);
    };
    driveFader->onJitterDragged = [this] (float amount)
    {
        audioProcessor.getModulationState (28).jitterAmount.store (amount);
    };
    driveFader->onCtrlClick = [this] { showModulationPopup (28); };
    addAndMakeVisible (*driveFader);
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "drive", driveSlider);

    // Mix + Output (vertical, together with drive and global_mod in the strip)
    auto setupVerticalFader = [&] (juce::Slider& slider, std::unique_ptr<gravel::RuneFader>& fader,
                                    const char* label, int modIdx, const juce::String& paramID)
    {
        slider.setSliderStyle (juce::Slider::LinearVertical);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        fader = std::make_unique<gravel::RuneFader> (slider, label, false,
            voidColour.withAlpha (0.3f), emberColour.withMultipliedSaturation (0.3f));
        fader->onInteraction = [this] { seedField.finishEditing(); };
        fader->onModulationDragged = [this, modIdx] (float modAmount)
        {
            audioProcessor.getModulationState (modIdx).manualModulation.store (modAmount);
        };
        fader->onSlopeDragged = [this, modIdx] (float amount)
        {
            audioProcessor.getModulationState (modIdx).slopeAmount.store (amount);
        };
        fader->onJitterDragged = [this, modIdx] (float amount)
        {
            audioProcessor.getModulationState (modIdx).jitterAmount.store (amount);
        };
        fader->onCtrlClick = [this, modIdx] { showModulationPopup (modIdx); };
        addAndMakeVisible (*fader);
    };
    setupVerticalFader (mixSlider, mixFader, "MIX", 24, "mix");
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "mix", mixSlider);
    setupVerticalFader (outputSlider, outputFader, "OUT", 25, "output");
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "output", outputSlider);

    // Hysteresis faders (modulatable)
    setupGlobalFader (hysteresisLowSlider, hysteresisLowFader, "HYST L", 41);
    setupGlobalFader (hysteresisMidSlider, hysteresisMidFader, "HYST M", 42);
    setupGlobalFader (hysteresisHighSlider, hysteresisHighFader, "HYST H", 43);

    // Global envelope multipliers (modulatable)
    setupGlobalFader (envRiseSlider, envRiseFader, "ENV RISE", 38);
    setupGlobalFader (envFallSlider, envFallFader, "ENV FALL", 39);
    setupGlobalFader (envGainSlider, envGainFader, "ENV GAIN", 40);
    envRiseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "env_rise", envRiseSlider);
    envFallAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "env_fall", envFallSlider);
    envGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "env_gain", envGainSlider);

    setupGlobalFader (slopeSpeedMultSlider, slopeSpeedMultFader, "SLP SPD", 44);
    setupGlobalFader (slopeSpikeMultSlider, slopeSpikeMultFader, "SLP SPK", 45);
    setupGlobalFader (slopeGainSlider, slopeGainFader, "SLP GAIN", 46);
    setupGlobalFader (jitterSpeedMultSlider, jitterSpeedMultFader, "JIT SPD", 47);
    setupGlobalFader (jitterSpikeMultSlider, jitterSpikeMultFader, "JIT SPK", 48);
    setupGlobalFader (jitterGainSlider, jitterGainFader, "JIT GAIN", 49);

    slopeSpeedMultAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "slope_speed_mult", slopeSpeedMultSlider);
    slopeSpikeMultAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "slope_spike_mult", slopeSpikeMultSlider);
    slopeGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "slope_gain", slopeGainSlider);
    jitterSpeedMultAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "jitter_speed_mult", jitterSpeedMultSlider);
    jitterSpikeMultAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "jitter_spike_mult", jitterSpikeMultSlider);
    jitterGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "jitter_gain", jitterGainSlider);

    eqBypassButton.setClickingTogglesState (true);
    eqBypassButton.setColour (juce::TextButton::buttonColourId, voidColour.withAlpha (0.15f));
    eqBypassButton.setColour (juce::TextButton::buttonOnColourId, emberColour.withAlpha (0.4f));
    eqBypassButton.setColour (juce::TextButton::textColourOffId, voidColour.withAlpha (0.6f));
    eqBypassButton.setColour (juce::TextButton::textColourOnId, emberColour);
    addAndMakeVisible (eqBypassButton);

    postEqDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "post_eq_depth", postEqDepthSlider);
    crossFreqLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "cross_freq_low", crossFreqLowSlider);
    crossFreqMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "cross_freq_mid", crossFreqMidSlider);
    bandLowLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "band_low_level", bandLowLevelSlider);
    bandMidLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "band_mid_level", bandMidLevelSlider);
    bandHighLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "band_high_level", bandHighLevelSlider);
    threshLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "thresh_low", threshLowSlider);
    threshHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "thresh_high", threshHighSlider);
    hysteresisLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "hysteresis_low", hysteresisLowSlider);
    hysteresisMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "hysteresis_mid", hysteresisMidSlider);
    hysteresisHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "hysteresis_high", hysteresisHighSlider);

    eqBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        audioProcessor.parameters, "eq_bypass", eqBypassButton);

    seedAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, "seed", seedSlider);

    // Wire grid line dragging
    spectralGrid.onVerticalLineDragged = [this] (int lineIndex, float normPos)
    {
        const auto effLow = audioProcessor.spectralGrid.getCrossoverNorm (0);
        const auto rawNorm = (lineIndex == 0)
            ? std::sqrt (normPos)
            : (normPos - effLow) / (1.0f - effLow);
        const auto rawHz = normToFreq (rawNorm);
        auto* param = audioProcessor.parameters.getParameter (
            lineIndex == 0 ? "cross_freq_low" : "cross_freq_mid");
        param->setValueNotifyingHost (param->convertTo0to1 (rawHz));
    };

    spectralGrid.onHorizontalLineDragged = [this] (int lineIndex, float normPos)
    {
        const auto effLow = audioProcessor.spectralGrid.getThresholdLine (0);
        const auto rawVal = (lineIndex == 0)
            ? std::sqrt (normPos)
            : (normPos - effLow) / (1.0f - effLow);
        auto* param = audioProcessor.parameters.getParameter (
            lineIndex == 0 ? "thresh_low" : "thresh_high");
        param->setValueNotifyingHost (rawVal);
    };

    spectralGrid.setVerticalLine (0, audioProcessor.spectralGrid.getCrossoverNorm (0));
    spectralGrid.setVerticalLine (1, audioProcessor.spectralGrid.getCrossoverNorm (1));
    spectralGrid.setHorizontalLine (0, audioProcessor.spectralGrid.getThresholdLine (0));
    spectralGrid.setHorizontalLine (1, audioProcessor.spectralGrid.getThresholdLine (1));

    // Initialize state
    seedField.setSeed (juce::roundToInt (seedSlider.getValue()));
    onSelectedCellChanged (0);
    startTimerHz (60);
    setSize (1024, 720);
}

BrokenAudioProcessorEditor::~BrokenAudioProcessorEditor()
{
    selectedCellMorphSlider.removeMouseListener (this);
    modulationPopup = nullptr;
}

void BrokenAudioProcessorEditor::onSelectedCellChanged (int cell)
{
    spectralGrid.setSelectedCell (cell);

    selectedCellMorphAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.parameters, juce::String (kCellParamIDs[static_cast<size_t> (cell)]), selectedCellMorphSlider);
}

void BrokenAudioProcessorEditor::showModulationPopup (int paramIndex)
{
    modulationPopupIndex = paramIndex;
    auto& mod = audioProcessor.getModulationState (paramIndex);
    modulationPopup = std::make_unique<gravel::ModulationPopup> ("", mod);

    modulationPopup->onClose = [this] { closeModulationPopup(); };
    modulationPopup->setSize (getWidth(), getHeight());
    addAndMakeVisible (modulationPopup.get());
    modulationPopup->toFront (true);
}

void BrokenAudioProcessorEditor::closeModulationPopup()
{
    if (modulationPopup != nullptr)
    {
        const auto idx = modulationPopupIndex;
        removeChildComponent (modulationPopup.get());
        modulationPopup = nullptr;
        modulationPopupIndex = -1;

        if (idx >= 0)
            audioProcessor.syncModulationToSpace (idx);
    }
}

void BrokenAudioProcessorEditor::mouseDown (const juce::MouseEvent& event)
{
    if (event.eventComponent == &selectedCellMorphSlider && event.mods.isCtrlDown())
    {
        const auto cell = spectralGrid.getSelectedCell();
        if (cell >= 0)
            showModulationPopup (15 + cell);
    }
}

void BrokenAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (groundColour);
    const auto bounds = getLocalBounds();

    g.setColour (voidColour.withAlpha (0.18f));
    g.drawRect (bounds.reduced (14), 1);

    g.setColour (voidColour.withAlpha (0.80f));
    g.setFont (18.0f);
    g.drawText ("BROKEN", 27, 18, 340, 22, juce::Justification::left);
    g.setFont (11.0f);
    g.setColour (voidColour.withAlpha (0.48f));
    g.drawText ("SEED SELECTS THE FIELD", getWidth() - 190 - 27, 22, 190, 18, juce::Justification::right);
    g.drawText ("SEED", 32, 70, 76, 18, juce::Justification::left);

    g.setColour (voidColour.withAlpha (0.50f));
    g.setFont (11.0f);
    for (const auto& [rect, text] : sectionLabels)
        g.drawText (text, rect, juce::Justification::left);
}

void BrokenAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced (12);

    auto topBar = bounds.removeFromTop (42);
    seedField.setBounds (topBar.removeFromLeft (140).reduced (4));
    randomizeButton.setBounds (topBar.removeFromLeft (110).reduced (4));

    auto bottomBar = bounds.removeFromBottom (50);
    if (possibilityFader != nullptr)
        possibilityFader->setBounds (bottomBar);

    auto columnWidth = bounds.getWidth() / 3;
    auto leftCol = bounds.removeFromLeft (columnWidth).reduced (4);
    auto midCol = bounds.removeFromLeft (columnWidth).reduced (4);
    auto rightCol = bounds.reduced (4);

    sectionLabels.clear();

    // ===== LEFT COLUMN: grid + processing pairs =====
    static constexpr int kProcH = 24;
    const int procTotal = 3 * kProcH;
    auto procArea = leftCol.removeFromBottom (procTotal);
    spectralGrid.setBounds (leftCol);

    // Row 1: thresh low / high
    {
        auto row = procArea.removeFromTop (kProcH);
        auto half = row.getWidth() / 2;
        if (threshLowFader != nullptr) threshLowFader->setBounds (row.removeFromLeft (half));
        if (threshHighFader != nullptr) threshHighFader->setBounds (row);
    }
    // Row 2: cross low / mid
    {
        auto row = procArea.removeFromTop (kProcH);
        auto half = row.getWidth() / 2;
        if (crossFreqLowFader != nullptr) crossFreqLowFader->setBounds (row.removeFromLeft (half));
        if (crossFreqMidFader != nullptr) crossFreqMidFader->setBounds (row);
    }
    // Row 3: hyst low/mid/high
    {
        auto row = procArea.removeFromTop (kProcH);
        auto third = row.getWidth() / 3;
        if (hysteresisLowFader != nullptr) hysteresisLowFader->setBounds (row.removeFromLeft (third));
        if (hysteresisMidFader != nullptr) hysteresisMidFader->setBounds (row.removeFromLeft (third));
        if (hysteresisHighFader != nullptr) hysteresisHighFader->setBounds (row);
    }

    // ===== MIDDLE COLUMN: 4 vertical faders, morph, post-eq, bands, groups =====
    static constexpr int kVertFaderH = 90;
    auto vertRow = midCol.removeFromTop (kVertFaderH).withSizeKeepingCentre (120, kVertFaderH);
    auto vW = vertRow.getWidth() / 4;
    auto placeVert = [&] (std::unique_ptr<gravel::RuneFader>& f)
    {
        auto r = vertRow.removeFromLeft (vW).reduced (2, 0);
        if (f != nullptr) f->setBounds (r);
    };
    placeVert (mixFader);
    placeVert (outputFader);
    placeVert (driveFader);
    placeVert (globalModFader);

    auto morphArea = midCol.removeFromTop (38);
    if (selectedCellMorphFader != nullptr)
        selectedCellMorphFader->setBounds (morphArea.reduced (4));

    static constexpr int kMidFaderH = 26;

    // Post-EQ depth
    midCol.removeFromTop (6);
    if (postEqDepthFader != nullptr)
        postEqDepthFader->setBounds (midCol.removeFromTop (kMidFaderH));

    // Band levels (3 side by side)
    {
        auto row = midCol.removeFromTop (kMidFaderH);
        auto third = row.getWidth() / 3;
        if (bandLowLevelFader != nullptr) bandLowLevelFader->setBounds (row.removeFromLeft (third));
        if (bandMidLevelFader != nullptr) bandMidLevelFader->setBounds (row.removeFromLeft (third));
        if (bandHighLevelFader != nullptr) bandHighLevelFader->setBounds (row);
    }

    auto addSection = [&] (const juce::String& label)
    {
        midCol.removeFromTop (6);
        auto labelR = midCol.removeFromTop (14);
        sectionLabels.emplace_back (labelR, label);
    };
    auto placeGroup = [&] (std::unique_ptr<gravel::RuneFader>& gain,
                            std::unique_ptr<gravel::RuneFader>& a,
                            std::unique_ptr<gravel::RuneFader>& b)
    {
        if (gain != nullptr) gain->setBounds (midCol.removeFromTop (kMidFaderH));
        auto row = midCol.removeFromTop (kMidFaderH);
        auto half = row.getWidth() / 2;
        if (a != nullptr) a->setBounds (row.removeFromLeft (half));
        if (b != nullptr) b->setBounds (row);
    };

    addSection ("ENVELOPE");
    placeGroup (envGainFader, envRiseFader, envFallFader);

    addSection ("SLOPE");
    placeGroup (slopeGainFader, slopeSpeedMultFader, slopeSpikeMultFader);

    addSection ("JITTER");
    placeGroup (jitterGainFader, jitterSpeedMultFader, jitterSpikeMultFader);

    // ===== RIGHT COLUMN: Pre-EQ + morph/drive rows =====
    auto eqBtnRow = rightCol.removeFromTop (18);
    eqBypassButton.setBounds (eqBtnRow.removeFromLeft (30).reduced (0, 3));
    auto preEqLabelR = rightCol.removeFromTop (14);
    sectionLabels.emplace_back (preEqLabelR, "PRE-EQ");

    for (int i = 0; i < 3; ++i)
    {
        auto row = rightCol.removeFromTop (32);
        auto third = row.getWidth() / 3;
        if (preEqFreqFaders[static_cast<size_t> (i)] != nullptr)
            preEqFreqFaders[static_cast<size_t> (i)]->setBounds (row.removeFromLeft (third));
        if (preEqQFaders[static_cast<size_t> (i)] != nullptr)
            preEqQFaders[static_cast<size_t> (i)]->setBounds (row.removeFromLeft (third));
        if (preEqGainFaders[static_cast<size_t> (i)] != nullptr)
            preEqGainFaders[static_cast<size_t> (i)]->setBounds (row);
    }

    rightCol.removeFromTop (8);

    // Morph + Drive rows (compacted)
    static constexpr int kRightRowH = 44;
    for (int i = 0; i < 9; ++i)
    {
        const auto idx = static_cast<size_t> (i);
        auto area = rightCol.removeFromTop (kRightRowH).reduced (2, 1);
        auto leftHalf = area.removeFromLeft (area.getWidth() / 2);
        if (cellMorphFaders[idx] != nullptr)
            cellMorphFaders[idx]->setBounds (leftHalf.reduced (1, 0));
        if (cellDriveFaders[idx] != nullptr)
            cellDriveFaders[idx]->setBounds (area.reduced (1, 0));
    }

    if (modulationPopup != nullptr)
        modulationPopup->setBounds (getLocalBounds());
}

void BrokenAudioProcessorEditor::timerCallback()
{
    for (int i = 0; i < 9; ++i)
    {
        const auto idx = static_cast<size_t> (i);
        const auto newData = audioProcessor.getCellShapeData (i);
        if (newData != cellShapeData[idx])
        {
            cellShapeData[idx] = newData;
            spectralGrid.setCellShapeData (i, newData);
        }
    }

    const auto newActivity = audioProcessor.spectralGrid.getCellActivity();
    if (newActivity != lastCellActivity)
    {
        lastCellActivity = newActivity;
        spectralGrid.setCellActivity (newActivity);
    }

    const auto tl = audioProcessor.spectralGrid.getThresholdLine (0);
    const auto th = audioProcessor.spectralGrid.getThresholdLine (1);
    const auto xl = audioProcessor.spectralGrid.getCrossoverNorm (0);
    const auto xh = audioProcessor.spectralGrid.getCrossoverNorm (1);

    if (std::abs (tl - lastThreshLow) > 0.001f || std::abs (th - lastThreshHigh) > 0.001f)
    {
        lastThreshLow = tl;
        lastThreshHigh = th;
        spectralGrid.setHorizontalLine (0, tl);
        spectralGrid.setHorizontalLine (1, th);
    }

    if (std::abs (xl - lastXOverLow) > 0.001f || std::abs (xh - lastXOverHigh) > 0.001f)
    {
        lastXOverLow = xl;
        lastXOverHigh = xh;
        spectralGrid.setVerticalLine (0, xl);
        spectralGrid.setVerticalLine (1, xh);
    }

    auto updateFaderColor = [] (gravel::RuneFader* fader, const gravel::ModulationState& mod)
    {
        if (fader == nullptr) return;
        const auto enabled = mod.explEnabled.load();
        if (fader->getRailColour().getAlpha() == (enabled ? 1.0f : 0.3f))
            return;
        if (enabled)
        {
            fader->setRailColour (juce::Colour (0xffd5d0b8));
            fader->setHandleColour (juce::Colour (0xffef8b48));
        }
        else
        {
            fader->setRailColour (juce::Colour (0xffd5d0b8).withAlpha (0.3f));
            fader->setHandleColour (juce::Colour (0xffef8b48).withMultipliedSaturation (0.3f));
        }
    };

    // Sync arrow state and colors for cell morph fader
    const auto cell = spectralGrid.getSelectedCell();
    if (cell >= 0 && selectedCellMorphFader != nullptr)
    {
        const auto& mod = audioProcessor.getModulationState (15 + cell);
        selectedCellMorphFader->setTargetModulation (mod.modulationAmount.load());
        selectedCellMorphFader->setAppliedModulation (mod.appliedModulation.load());
        updateFaderColor (selectedCellMorphFader.get(), mod);
    }

    // Pre-EQ and global faders
    auto syncFader = [&] (gravel::RuneFader* fader, int modIdx)
    {
        if (fader == nullptr) return;
        const auto& mod = audioProcessor.getModulationState (modIdx);
        const auto ma = mod.modulationAmount.load();
        const auto aa = mod.appliedModulation.load();
        const auto sa = mod.slopeAmount.load();
        const auto ja = mod.jitterAmount.load();
        bool anyChanged = false;
        anyChanged |= std::abs (fader->getTargetModulation() - ma) > 0.001f;
        anyChanged |= std::abs (fader->getAppliedModulation() - aa) > 0.001f;
        anyChanged |= std::abs (fader->getSlopeModulation() - sa) > 0.001f;
        anyChanged |= std::abs (fader->getJitterModulation() - ja) > 0.001f;
        fader->setTargetModulation (ma);
        fader->setAppliedModulation (aa);
        fader->setSlopeModulation (sa);
        fader->setJitterModulation (ja);
        updateFaderColor (fader, mod);
        if (anyChanged)
            fader->repaint();
    };

    for (int i = 0; i < 3; ++i)
    {
        syncFader (preEqFreqFaders[static_cast<size_t> (i)].get(), i);
        syncFader (preEqQFaders[static_cast<size_t> (i)].get(), 3 + i);
        syncFader (preEqGainFaders[static_cast<size_t> (i)].get(), 6 + i);
    }

    for (int i = 0; i < kNumGlobalParams; ++i)
    {
        const auto& g = kGlobalParams[static_cast<size_t> (i)];
        std::unique_ptr<gravel::RuneFader>* fader = nullptr;
        if (i == 0)      fader = &postEqDepthFader;
        else if (i == 1) fader = &crossFreqLowFader;
        else if (i == 2) fader = &crossFreqMidFader;
        else if (i == 3) fader = &bandLowLevelFader;
        else if (i == 4) fader = &bandMidLevelFader;
        else if (i == 5) fader = &bandHighLevelFader;

        if (fader != nullptr && fader->get() != nullptr)
            syncFader (fader->get(), g.modIndex);
    }

    syncFader (threshLowFader.get(), 26);
    syncFader (threshHighFader.get(), 27);
    syncFader (driveFader.get(), 28);
    syncFader (mixFader.get(), 24);
    syncFader (outputFader.get(), 25);
    syncFader (envRiseFader.get(), 38);
    syncFader (envFallFader.get(), 39);
    syncFader (envGainFader.get(), 40);
    syncFader (hysteresisLowFader.get(), 41);
    syncFader (hysteresisMidFader.get(), 42);
    syncFader (hysteresisHighFader.get(), 43);
    syncFader (slopeSpeedMultFader.get(), 44);
    syncFader (slopeSpikeMultFader.get(), 45);
    syncFader (slopeGainFader.get(), 46);
    syncFader (jitterSpeedMultFader.get(), 47);
    syncFader (jitterSpikeMultFader.get(), 48);
    syncFader (jitterGainFader.get(), 49);

    // 9 cell morph faders
    for (int i = 0; i < 9; ++i)
    {
        const auto idx = static_cast<size_t> (i);
        if (cellMorphFaders[idx] != nullptr)
            syncFader (cellMorphFaders[idx].get(), 15 + i);
        if (cellDriveFaders[idx] != nullptr)
            syncFader (cellDriveFaders[idx].get(), 29 + i);
    }

    seedField.setSeed (juce::roundToInt (seedSlider.getValue()));

    if (possibilityFader != nullptr)
        possibilityFader->setValue (audioProcessor.parameters.getRawParameterValue ("possibility")->load());
}

std::unique_ptr<juce::ComponentTraverser> BrokenAudioProcessorEditor::createKeyboardFocusTraverser()
{
    struct EmptyTraverser final : juce::KeyboardFocusTraverser
    {
        juce::Component* getDefaultComponent (juce::Component*) override { return nullptr; }
        juce::Component* getNextComponent (juce::Component*) override { return nullptr; }
        juce::Component* getPreviousComponent (juce::Component*) override { return nullptr; }
        std::vector<juce::Component*> getAllComponents (juce::Component*) override { return {}; }
    };

    return std::make_unique<EmptyTraverser>();
}

bool BrokenAudioProcessorEditor::keyPressed (const juce::KeyPress& key)
{
    if (key == juce::KeyPress ('t', juce::ModifierKeys::commandModifier, 0)
        || key == juce::KeyPress ('t', juce::ModifierKeys::ctrlModifier, 0))
    {
        juce::UnitTestRunner runner;
        runner.setAssertOnFailure (false);
        runner.runAllTests();

        int passes = 0, failures = 0;
        for (int i = 0; i < runner.getNumResults(); ++i)
        {
            if (auto* r = runner.getResult (i))
            {
                passes += r->passes;
                failures += r->failures;
            }
        }

        juce::AlertWindow::showMessageBoxAsync (juce::MessageBoxIconType::InfoIcon,
            "Unit Tests",
            juce::String (passes) + " passed, "
            + juce::String (failures) + " failed",
            "OK");
        return true;
    }

    return false;
}
