#pragma once

#include "PluginProcessor.h"

#include "ui/SpectralGridComponent.h"
#include "gravel/RuneFader.h"
#include "gravel/SeedField.h"
#include "gravel/FloatFader.h"
#include "gravel/ModulationPopup.h"

class BrokenAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                                              private juce::Timer
{
public:
    explicit BrokenAudioProcessorEditor (BrokenAudioProcessor&);
    ~BrokenAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    bool keyPressed (const juce::KeyPress& key) override;

private:
    std::unique_ptr<juce::ComponentTraverser> createKeyboardFocusTraverser() override;
    void timerCallback() override;
    void mouseDown (const juce::MouseEvent&) override;

    void onSelectedCellChanged (int cell);
    void closeModulationPopup();
    void showModulationPopup (int paramIndex);

    BrokenAudioProcessor& audioProcessor;

    // Global mod amount (vertical, right of matrix)
    juce::Slider globalModSlider;
    std::unique_ptr<gravel::RuneFader> globalModFader;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> globalModAttachment;

    // Top bar
    juce::Slider seedSlider;
    gravel::SeedField seedField;
    juce::TextButton randomizeButton { "NEW FIELD" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> seedAttachment;

    // Possibility
    std::unique_ptr<gravel::FloatFader> possibilityFader;

    // Spectral grid
    SpectralGridComponent spectralGrid;

    // Selected cell morph fader (big, middle column)
    juce::Slider selectedCellMorphSlider;
    std::unique_ptr<gravel::RuneFader> selectedCellMorphFader;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> selectedCellMorphAttachment;

    // 9 small cell morph sliders (right column)
    std::array<juce::Slider, 9> cellMorphSliders;
    std::array<std::unique_ptr<gravel::RuneFader>, 9> cellMorphFaders;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 9> cellMorphAttachments;
    std::array<juce::Slider, 9> cellDriveSliders;
    std::array<std::unique_ptr<gravel::RuneFader>, 9> cellDriveFaders;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 9> cellDriveAttachments;

    // Pre-EQ sliders (3 stages x 3 params)
    std::array<juce::Slider, 3> preEqFreqSliders;
    std::array<juce::Slider, 3> preEqQSliders;
    std::array<juce::Slider, 3> preEqGainSliders;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 3> preEqFreqAttachments;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 3> preEqQAttachments;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 3> preEqGainAttachments;
    std::array<std::unique_ptr<gravel::RuneFader>, 3> preEqFreqFaders;
    std::array<std::unique_ptr<gravel::RuneFader>, 3> preEqQFaders;
    std::array<std::unique_ptr<gravel::RuneFader>, 3> preEqGainFaders;

    // Threshold faders
    juce::Slider threshLowSlider, threshHighSlider;
    std::unique_ptr<gravel::RuneFader> threshLowFader, threshHighFader;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> threshLowAttachment, threshHighAttachment;

    // Drive
    juce::Slider driveSlider;
    std::unique_ptr<gravel::RuneFader> driveFader;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;

    // Global envelope multipliers
    juce::Slider envRiseSlider, envFallSlider, envGainSlider;
    std::unique_ptr<gravel::RuneFader> envRiseFader, envFallFader, envGainFader;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> envRiseAttachment, envFallAttachment, envGainAttachment;

    // Slope / Jitter global multipliers
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

    // Global params
    juce::Slider postEqDepthSlider, crossFreqLowSlider, crossFreqMidSlider;
    juce::Slider bandLowLevelSlider, bandMidLevelSlider, bandHighLevelSlider;
    juce::Slider mixSlider, outputSlider;
    juce::Slider hysteresisLowSlider, hysteresisMidSlider, hysteresisHighSlider;
    juce::TextButton eqBypassButton { "EQ" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postEqDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crossFreqLowAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crossFreqMidAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bandLowLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bandMidLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bandHighLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hysteresisLowAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hysteresisMidAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hysteresisHighAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBypassAttachment;
    std::unique_ptr<gravel::RuneFader> postEqDepthFader;
    std::unique_ptr<gravel::RuneFader> crossFreqLowFader;
    std::unique_ptr<gravel::RuneFader> crossFreqMidFader;
    std::unique_ptr<gravel::RuneFader> bandLowLevelFader;
    std::unique_ptr<gravel::RuneFader> bandMidLevelFader;
    std::unique_ptr<gravel::RuneFader> bandHighLevelFader;
    std::unique_ptr<gravel::RuneFader> mixFader;
    std::unique_ptr<gravel::RuneFader> outputFader;
    std::unique_ptr<gravel::RuneFader> hysteresisLowFader, hysteresisMidFader, hysteresisHighFader;

    // Cell shape data for 9 cells
    std::array<std::array<float, ::dsp::SpectralGridProcessor::kWaveTableSize>, 9> cellShapeData {};
    std::array<float, 9> lastCellActivity {};
    float lastThreshLow = -1.0f;
    float lastThreshHigh = -1.0f;
    float lastXOverLow = -1.0f;
    float lastXOverHigh = -1.0f;

    // Modulation popup
    std::unique_ptr<gravel::ModulationPopup> modulationPopup;
    int modulationPopupIndex = -1;

    std::vector<std::pair<juce::Rectangle<int>, juce::String>> sectionLabels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BrokenAudioProcessorEditor)
};
