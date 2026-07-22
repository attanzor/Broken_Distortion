#pragma once

#include <JuceHeader.h>

#include "dsp/SpectralGridProcessor.h"
#include "gravel/PossibilitySpace.h"
#include "gravel/ParameterSpace.h"
#include "gravel/ModulationState.h"
#include "gravel/EnvelopeFollower.h"

class BrokenAudioProcessor final : public juce::AudioProcessor,
                                                        private juce::AudioProcessorValueTreeState::Listener
{
public:
    BrokenAudioProcessor();
    ~BrokenAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

    static constexpr int kNumMappedParams = 50;

    std::array<float, ::dsp::SpectralGridProcessor::kWaveTableSize> getCellShapeData (int cellIndex) const;

    gravel::ModulationState& getModulationState (int index);
    void syncModulationToSpace (int index);
    void recomputeExplorationModulation();

    juce::AudioProcessorValueTreeState parameters;
    ::dsp::SpectralGridProcessor spectralGrid;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;

    gravel::ParameterSpace possibilitySpace { parameters };

    juce::dsp::Oversampling<float> oversampler { 2, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, true };
    juce::AudioBuffer<float> dryBuffer;

    std::array<gravel::ModulationState, kNumMappedParams> modulationStates;
    std::array<gravel::EnvelopeFollower, kNumMappedParams> envelopeFollowers;

    std::array<float, kNumMappedParams> slopeFilterStates {};
    std::array<float, kNumMappedParams> jitterPhases {};
    std::array<float, kNumMappedParams> paramSmoothed {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BrokenAudioProcessor)
};
