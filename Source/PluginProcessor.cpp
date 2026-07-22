#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr const char* cellBiasIDs[] = {
    "cell_lq_bias", "cell_lm_bias", "cell_ll_bias",
    "cell_mq_bias", "cell_mm_bias", "cell_ml_bias",
    "cell_hq_bias", "cell_hm_bias", "cell_hl_bias"
};

static constexpr const char* mappedParamIds[] = {
    "pre_lowshelf_freq",
    "pre_lowshelf_q",
    "pre_lowshelf_gain",
    "pre_bell_freq",
    "pre_bell_q",
    "pre_bell_gain",
    "pre_highshelf_freq",
    "pre_highshelf_q",
    "pre_highshelf_gain",
    "post_eq_depth",
    "cross_freq_low",
    "cross_freq_mid",
    "band_low_level",
    "band_mid_level",
    "band_high_level",
    "cell_lq_shape",
    "cell_mq_shape",
    "cell_hq_shape",
    "cell_lm_shape",
    "cell_mm_shape",
    "cell_hm_shape",
    "cell_ll_shape",
    "cell_ml_shape",
    "cell_hl_shape",
    "mix",
    "output",
    "thresh_low",
    "thresh_high",
    "drive",
    "cell_lq_drive",
    "cell_lm_drive",
    "cell_ll_drive",
    "cell_mq_drive",
    "cell_mm_drive",
    "cell_ml_drive",
    "cell_hq_drive",
    "cell_hm_drive",
    "cell_hl_drive",
    "env_rise",
    "env_fall",
    "env_gain",
    "hysteresis_low",
    "hysteresis_mid",
    "hysteresis_high",
    "slope_speed_mult",
    "slope_spike_mult",
    "slope_gain",
    "jitter_speed_mult",
    "jitter_spike_mult",
    "jitter_gain"
};

BrokenAudioProcessor::BrokenAudioProcessor()
    : AudioProcessor (BusesProperties().withInput ("Input", juce::AudioChannelSet::stereo(), true)
                                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, "STATE", createParameterLayout()),
      possibilitySpace (parameters)
{
    possibilitySpace.mapParameters ({
        { "pre_lowshelf_freq",  2 },
        { "pre_lowshelf_q",     3 },
        { "pre_lowshelf_gain",  4 },
        { "pre_bell_freq",      5 },
        { "pre_bell_q",         6 },
        { "pre_bell_gain",      7 },
        { "pre_highshelf_freq", 8 },
        { "pre_highshelf_q",    9 },
        { "pre_highshelf_gain", 10 },
        { "post_eq_depth",      11 },
        { "cross_freq_low",     12 },
        { "cross_freq_mid",     13 },
        { "band_low_level",     14 },
        { "band_mid_level",     15 },
        { "band_high_level",    16 },
        { "cell_lq_shape",      17 },
        { "cell_mq_shape",      18 },
        { "cell_hq_shape",      19 },
        { "cell_lm_shape",      20 },
        { "cell_mm_shape",      21 },
        { "cell_hm_shape",      22 },
        { "cell_ll_shape",      23 },
        { "cell_ml_shape",      24 },
        { "cell_hl_shape",      25 },
        { "mix",                26 },
        { "output",             27 },
        { "thresh_low",         28 },
        { "thresh_high",        29 },
        { "cell_lq_drive",      30 },
        { "cell_lm_drive",      31 },
        { "cell_ll_drive",      32 },
        { "cell_mq_drive",      33 },
        { "cell_mm_drive",      34 },
        { "cell_ml_drive",      35 },
        { "cell_hq_drive",      36 },
        { "cell_hm_drive",      37 },
        { "cell_hl_drive",      38 },
        { "env_rise",           39 },
        { "env_fall",           40 },
        { "env_gain",           41 },
        { "hysteresis_low",     42 },
        { "hysteresis_mid",     43 },
        { "hysteresis_high",    44 },
        { "slope_speed_mult",   45 },
        { "slope_spike_mult",   46 },
        { "slope_gain",         47 },
        { "jitter_speed_mult",  48 },
        { "jitter_spike_mult",  49 },
        { "jitter_gain",        50 },
    });

    parameters.addParameterListener ("seed", this);
    parameters.addParameterListener ("possibility", this);
    parameters.addParameterListener ("pre_lowshelf_freq", this);
    parameters.addParameterListener ("pre_lowshelf_q", this);
    parameters.addParameterListener ("pre_lowshelf_gain", this);
    parameters.addParameterListener ("pre_bell_freq", this);
    parameters.addParameterListener ("pre_bell_q", this);
    parameters.addParameterListener ("pre_bell_gain", this);
    parameters.addParameterListener ("pre_highshelf_freq", this);
    parameters.addParameterListener ("pre_highshelf_q", this);
    parameters.addParameterListener ("pre_highshelf_gain", this);
    parameters.addParameterListener ("post_eq_depth", this);
    parameters.addParameterListener ("cross_freq_low", this);
    parameters.addParameterListener ("cross_freq_mid", this);
    parameters.addParameterListener ("band_low_level", this);
    parameters.addParameterListener ("band_mid_level", this);
    parameters.addParameterListener ("band_high_level", this);
    parameters.addParameterListener ("cell_lq_shape", this);
    parameters.addParameterListener ("cell_mq_shape", this);
    parameters.addParameterListener ("cell_hq_shape", this);
    parameters.addParameterListener ("cell_lm_shape", this);
    parameters.addParameterListener ("cell_mm_shape", this);
    parameters.addParameterListener ("cell_hm_shape", this);
    parameters.addParameterListener ("cell_ll_shape", this);
    parameters.addParameterListener ("cell_ml_shape", this);
    parameters.addParameterListener ("cell_hl_shape", this);
    parameters.addParameterListener ("mix", this);
    parameters.addParameterListener ("output", this);
    parameters.addParameterListener ("eq_bypass", this);
    parameters.addParameterListener ("bypass_waveshapers", this);
    parameters.addParameterListener ("bypass_amp_split", this);
    parameters.addParameterListener ("bypass_freq_split", this);
    parameters.addParameterListener ("thresh_low", this);
    parameters.addParameterListener ("thresh_high", this);
    parameters.addParameterListener ("drive", this);
    for (const auto* id : { "cell_lq_drive", "cell_lm_drive", "cell_ll_drive",
                            "cell_mq_drive", "cell_mm_drive", "cell_ml_drive",
                            "cell_hq_drive", "cell_hm_drive", "cell_hl_drive" })
        parameters.addParameterListener (id, this);

    for (const auto* id : { "slope_speed_mult", "slope_spike_mult", "slope_gain",
                             "jitter_speed_mult", "jitter_spike_mult", "jitter_gain" })
        parameters.addParameterListener (id, this);

    for (int i : { 9, 12, 13, 14, 24, 25 })
    {
        modulationStates[static_cast<size_t> (i)].modulationAmount.store (0.0f);
        modulationStates[static_cast<size_t> (i)].manualModulation.store (0.0f);
    }
    // Enable exploration by default for all params except explicitly disabled ones
    for (int i = 0; i < kNumMappedParams; ++i)
    {
        if (i == 28) continue; // drive
        modulationStates[static_cast<size_t> (i)].explEnabled.store (true);
    }
    // Drive: non-explorable, no manual modulation by default
    modulationStates[static_cast<size_t> (28)].explEnabled.store (false);
    modulationStates[static_cast<size_t> (28)].modulationAmount.store (0.0f);
    modulationStates[static_cast<size_t> (28)].manualModulation.store (0.0f);

    for (int i = 0; i < kNumMappedParams; ++i)
    {
        if (i == 9 || (i >= 12 && i <= 14) || i == 24 || i == 25 || i == 28)
            continue;
        modulationStates[static_cast<size_t> (i)].explModEnabled.store (true);
        modulationStates[static_cast<size_t> (i)].envModAmountExplEnabled.store (true);
        modulationStates[static_cast<size_t> (i)].slopeExplModEnabled.store (true);
        modulationStates[static_cast<size_t> (i)].jitterExplModEnabled.store (true);
        modulationStates[static_cast<size_t> (i)].slopeModAmountExplEnabled.store (true);
        modulationStates[static_cast<size_t> (i)].jitterModAmountExplEnabled.store (true);
    }

    for (int i = 0; i < kNumMappedParams; ++i)
        possibilitySpace.setExploration (
            mappedParamIds[static_cast<size_t> (i)],
            modulationStates[static_cast<size_t> (i)].explMin.load(),
            modulationStates[static_cast<size_t> (i)].explMax.load(),
            modulationStates[static_cast<size_t> (i)].explEnabled.load());

    spectralGrid.setSeed (1024);
}

BrokenAudioProcessor::~BrokenAudioProcessor()
{
    parameters.removeParameterListener ("seed", this);
    parameters.removeParameterListener ("possibility", this);
    parameters.removeParameterListener ("pre_lowshelf_freq", this);
    parameters.removeParameterListener ("pre_lowshelf_q", this);
    parameters.removeParameterListener ("pre_lowshelf_gain", this);
    parameters.removeParameterListener ("pre_bell_freq", this);
    parameters.removeParameterListener ("pre_bell_q", this);
    parameters.removeParameterListener ("pre_bell_gain", this);
    parameters.removeParameterListener ("pre_highshelf_freq", this);
    parameters.removeParameterListener ("pre_highshelf_q", this);
    parameters.removeParameterListener ("pre_highshelf_gain", this);
    parameters.removeParameterListener ("post_eq_depth", this);
    parameters.removeParameterListener ("cross_freq_low", this);
    parameters.removeParameterListener ("cross_freq_mid", this);
    parameters.removeParameterListener ("band_low_level", this);
    parameters.removeParameterListener ("band_mid_level", this);
    parameters.removeParameterListener ("band_high_level", this);
    parameters.removeParameterListener ("cell_lq_shape", this);
    parameters.removeParameterListener ("cell_mq_shape", this);
    parameters.removeParameterListener ("cell_hq_shape", this);
    parameters.removeParameterListener ("cell_lm_shape", this);
    parameters.removeParameterListener ("cell_mm_shape", this);
    parameters.removeParameterListener ("cell_hm_shape", this);
    parameters.removeParameterListener ("cell_ll_shape", this);
    parameters.removeParameterListener ("cell_ml_shape", this);
    parameters.removeParameterListener ("cell_hl_shape", this);
    parameters.removeParameterListener ("mix", this);
    parameters.removeParameterListener ("output", this);
    parameters.removeParameterListener ("eq_bypass", this);
    parameters.removeParameterListener ("bypass_waveshapers", this);
    parameters.removeParameterListener ("bypass_amp_split", this);
    parameters.removeParameterListener ("bypass_freq_split", this);
    parameters.removeParameterListener ("thresh_low", this);
    parameters.removeParameterListener ("thresh_high", this);
    parameters.removeParameterListener ("drive", this);
    for (const auto* id : { "cell_lq_drive", "cell_lm_drive", "cell_ll_drive",
                            "cell_mq_drive", "cell_mm_drive", "cell_ml_drive",
                            "cell_hq_drive", "cell_hm_drive", "cell_hl_drive" })
        parameters.removeParameterListener (id, this);

    for (const auto* id : { "slope_speed_mult", "slope_spike_mult", "slope_gain",
                             "jitter_speed_mult", "jitter_spike_mult", "jitter_gain" })
        parameters.removeParameterListener (id, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout BrokenAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> controls;

    controls.push_back (std::make_unique<juce::AudioParameterInt> ("seed", "Seed", 0, 999999, 1024));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("possibility", "Possibility", 0.0f, 1.0f, 0.0f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_lowshelf_freq", "Pre Lowshelf Freq",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.3f), 300.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_lowshelf_q", "Pre Lowshelf Q",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.0f, 1.0f), 0.707f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_lowshelf_gain", "Pre Lowshelf Gain",
        -24.0f, 24.0f, 0.0f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_bell_freq", "Pre Bell Freq",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.3f), 1000.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_bell_q", "Pre Bell Q",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.0f, 1.0f), 0.707f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_bell_gain", "Pre Bell Gain",
        -24.0f, 24.0f, 0.0f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_highshelf_freq", "Pre Highshelf Freq",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.3f), 3000.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_highshelf_q", "Pre Highshelf Q",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.0f, 1.0f), 0.707f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("pre_highshelf_gain", "Pre Highshelf Gain",
        -24.0f, 24.0f, 0.0f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("post_eq_depth", "Post EQ Depth",
        0.0f, 1.0f, 1.0f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cross_freq_low", "Cross Freq Low",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.3f), 300.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cross_freq_mid", "Cross Freq Mid",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.3f), 3000.0f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("band_low_level", "Band Low Level",
        0.0f, 1.0f, 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("band_mid_level", "Band Mid Level",
        0.0f, 1.0f, 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("band_high_level", "Band High Level",
        0.0f, 1.0f, 1.0f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cell_lq_shape", "Cell LQ Shape",
        0.0f, 1.0f, 0.5f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cell_mq_shape", "Cell MQ Shape",
        0.0f, 1.0f, 0.5f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cell_hq_shape", "Cell HQ Shape",
        0.0f, 1.0f, 0.5f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cell_lm_shape", "Cell LM Shape",
        0.0f, 1.0f, 0.5f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cell_mm_shape", "Cell MM Shape",
        0.0f, 1.0f, 0.5f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cell_hm_shape", "Cell HM Shape",
        0.0f, 1.0f, 0.5f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cell_ll_shape", "Cell LL Shape",
        0.0f, 1.0f, 0.5f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cell_ml_shape", "Cell ML Shape",
        0.0f, 1.0f, 0.5f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("cell_hl_shape", "Cell HL Shape",
        0.0f, 1.0f, 0.5f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("mix", "Mix",
        0.0f, 1.0f, 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("drive", "Drive",
        juce::NormalisableRange<float> (0.0f, 20.0f, 0.01f, 0.3f), 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("output", "Output",
        0.0f, 1.0f, 0.75f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("hysteresis_low", "Hysteresis Low", 0.0f, 1.0f, 0.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("hysteresis_mid", "Hysteresis Mid", 0.0f, 1.0f, 0.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("hysteresis_high", "Hysteresis High", 0.0f, 1.0f, 0.0f));

    controls.push_back (std::make_unique<juce::AudioParameterBool> ("eq_bypass", "EQ Bypass", false));

    controls.push_back (std::make_unique<juce::AudioParameterBool> ("bypass_waveshapers", "Bypass Waveshapers", false));
    controls.push_back (std::make_unique<juce::AudioParameterBool> ("bypass_amp_split", "Bypass Amp Split", false));
    controls.push_back (std::make_unique<juce::AudioParameterBool> ("bypass_freq_split", "Bypass Freq Split", false));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("thresh_low", "Threshold Low",
        0.0f, 1.0f, 0.33f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("thresh_high", "Threshold High",
        0.0f, 1.0f, 0.66f));

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("env_rise", "Env Rise",
        juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f, 2.0f), 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("env_fall", "Env Fall",
        juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f, 2.0f), 1.0f));
    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("env_gain", "Env Gain",
        juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f, 2.0f), 1.0f));

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

    static constexpr const char* cellDriveIDs[] = {
        "cell_lq_drive", "cell_lm_drive", "cell_ll_drive",
        "cell_mq_drive", "cell_mm_drive", "cell_ml_drive",
        "cell_hq_drive", "cell_hm_drive", "cell_hl_drive"
    };
    for (int i = 0; i < 9; ++i)
    {
        controls.push_back (std::make_unique<juce::AudioParameterFloat> (cellDriveIDs[i], cellDriveIDs[i],
            juce::NormalisableRange<float> (1.0f, 10.0f, 0.01f, 0.315f), 1.0f));
    }

    for (int i = 0; i < 9; ++i)
    {
        controls.push_back (std::make_unique<juce::AudioParameterFloat> (cellBiasIDs[i], cellBiasIDs[i],
            -1.0f, 1.0f, 0.0f));
    }

    controls.push_back (std::make_unique<juce::AudioParameterFloat> ("global_mod_amount", "Mod Amount",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 2.0f), 0.25f));

    return { controls.begin(), controls.end() };
}

gravel::ModulationState& BrokenAudioProcessor::getModulationState (int index)
{
    return modulationStates[static_cast<size_t> (juce::jlimit (0, kNumMappedParams - 1, index))];
}

void BrokenAudioProcessor::syncModulationToSpace (int index)
{
    const auto i = static_cast<size_t> (juce::jlimit (0, kNumMappedParams - 1, index));
    const auto& mod = modulationStates[i];
    possibilitySpace.setExploration (mappedParamIds[i], mod.explMin.load(), mod.explMax.load(), mod.explEnabled.load());
}

void BrokenAudioProcessor::parameterChanged (const juce::String& parameterID, float val)
{
    if (parameterID == "seed")
    {
        spectralGrid.setSeed (static_cast<uint32_t> (std::round (val)));
        possibilitySpace.handleParameterChange (parameterID, {});
        recomputeExplorationModulation();
        return;
    }

    if (parameterID == "possibility")
        recomputeExplorationModulation();

    if (possibilitySpace.handleParameterChange (parameterID, {}))
        return;

    if      (parameterID == "pre_lowshelf_freq")    spectralGrid.setPreEqFreq (0, val);
    else if (parameterID == "pre_lowshelf_q")       spectralGrid.setPreEqQ (0, val);
    else if (parameterID == "pre_lowshelf_gain")    spectralGrid.setPreEqGain (0, val);
    else if (parameterID == "pre_bell_freq")        spectralGrid.setPreEqFreq (1, val);
    else if (parameterID == "pre_bell_q")           spectralGrid.setPreEqQ (1, val);
    else if (parameterID == "pre_bell_gain")        spectralGrid.setPreEqGain (1, val);
    else if (parameterID == "pre_highshelf_freq")   spectralGrid.setPreEqFreq (2, val);
    else if (parameterID == "pre_highshelf_q")      spectralGrid.setPreEqQ (2, val);
    else if (parameterID == "pre_highshelf_gain")   spectralGrid.setPreEqGain (2, val);
    else if (parameterID == "post_eq_depth")        spectralGrid.setPostEqDepth (val);
    else if (parameterID == "cross_freq_low")       spectralGrid.setCrossoverFreq (0, val);
    else if (parameterID == "cross_freq_mid")       spectralGrid.setCrossoverFreq (1, val);
    else if (parameterID == "band_low_level")       spectralGrid.setBandLevel (0, val);
    else if (parameterID == "band_mid_level")       spectralGrid.setBandLevel (1, val);
    else if (parameterID == "band_high_level")      spectralGrid.setBandLevel (2, val);
    else if (parameterID == "eq_bypass") spectralGrid.setEqBypass (val >= 0.5f);
    else if (parameterID == "bypass_waveshapers") spectralGrid.setBypassWaveshapers (val >= 0.5f);
    else if (parameterID == "bypass_amp_split")   spectralGrid.setBypassAmplitudeSplit (val >= 0.5f);
    else if (parameterID == "bypass_freq_split")  spectralGrid.setBypassFrequencySplit (val >= 0.5f);
    else if (parameterID == "cell_lq_shape")        spectralGrid.setCellShape (0, val);
    else if (parameterID == "cell_lm_shape")        spectralGrid.setCellShape (1, val);
    else if (parameterID == "cell_ll_shape")        spectralGrid.setCellShape (2, val);
    else if (parameterID == "cell_mq_shape")        spectralGrid.setCellShape (3, val);
    else if (parameterID == "cell_mm_shape")        spectralGrid.setCellShape (4, val);
    else if (parameterID == "cell_ml_shape")        spectralGrid.setCellShape (5, val);
    else if (parameterID == "cell_hq_shape")        spectralGrid.setCellShape (6, val);
    else if (parameterID == "cell_hm_shape")        spectralGrid.setCellShape (7, val);
    else if (parameterID == "cell_hl_shape")        spectralGrid.setCellShape (8, val);
    else if (parameterID == "thresh_low")            spectralGrid.setThresholdLine (0, val);
    else if (parameterID == "thresh_high")           spectralGrid.setThresholdLine (1, val);
    else if (parameterID == "cell_lq_drive")         spectralGrid.setCellDrive (0, val);
    else if (parameterID == "cell_lm_drive")         spectralGrid.setCellDrive (1, val);
    else if (parameterID == "cell_ll_drive")         spectralGrid.setCellDrive (2, val);
    else if (parameterID == "cell_mq_drive")         spectralGrid.setCellDrive (3, val);
    else if (parameterID == "cell_mm_drive")         spectralGrid.setCellDrive (4, val);
    else if (parameterID == "cell_ml_drive")         spectralGrid.setCellDrive (5, val);
    else if (parameterID == "cell_hq_drive")         spectralGrid.setCellDrive (6, val);
    else if (parameterID == "cell_hm_drive")         spectralGrid.setCellDrive (7, val);
    else if (parameterID == "cell_hl_drive")         spectralGrid.setCellDrive (8, val);
}

void BrokenAudioProcessor::recomputeExplorationModulation()
{
    const auto seed = static_cast<uint32_t> (std::round (parameters.getRawParameterValue ("seed")->load()));
    const auto possibility = parameters.getRawParameterValue ("possibility")->load();

    for (int i = 0; i < kNumMappedParams; ++i)
    {
        auto& mod = modulationStates[static_cast<size_t> (i)];
        if (mod.explModEnabled.load() && i != 28)
        {
            const auto nvRise = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 7 + 200), possibility * 3.0f);
            const auto nvFall = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 7 + 300), possibility * 3.0f);
            mod.envRise.store (juce::jmap (nvRise, 0.0f, 1.0f, 1.0f, 1000.0f));
            mod.envFall.store (juce::jmap (nvFall, 0.0f, 1.0f, 1.0f, 1000.0f));
        }

        if (mod.slopeExplModEnabled.load())
        {
            const auto nvSpeed = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 7 + 400), possibility * 3.0f);
            const auto nvSpike = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 7 + 500), possibility * 3.0f);
            mod.slopeSpeed.store (juce::jmap (nvSpeed, 0.0f, 1.0f, 0.0f, 500.0f));
            mod.slopeSpikyness.store (juce::jmap (nvSpike, 0.0f, 1.0f, 0.3f, 10.0f));
        }

        if (mod.jitterExplModEnabled.load())
        {
            const auto nvSpeed = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 7 + 600), possibility * 3.0f);
            const auto nvSpike = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 7 + 700), possibility * 3.0f);
            mod.jitterSpeed.store (juce::jmap (nvSpeed, 0.0f, 1.0f, 0.0f, 20000.0f));
            mod.jitterSpikyness.store (juce::jmap (nvSpike, 0.0f, 1.0f, 0.3f, 10.0f));
        }

        if (mod.slopeModAmountExplEnabled.load())
        {
            const auto nv = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 7 + 800), possibility * 3.0f);
            mod.slopeAmount.store ((nv - 0.5f) * 2.0f);
        }

        if (mod.jitterModAmountExplEnabled.load())
        {
            const auto nv = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 7 + 900), possibility * 3.0f);
            mod.jitterAmount.store ((nv - 0.5f) * 2.0f);
        }
    }
}

void BrokenAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto osFactor = static_cast<int> (oversampler.getOversamplingFactor());
    spectralGrid.prepare (sampleRate * osFactor, samplesPerBlock * osFactor);

    for (auto& f : envelopeFollowers)
        f.prepare (sampleRate);

    oversampler.initProcessing (static_cast<size_t> (samplesPerBlock));
    oversampler.reset();
    dryBuffer.setSize (getTotalNumOutputChannels(), samplesPerBlock);
    dryBuffer.clear();
    setLatencySamples (juce::roundToInt (oversampler.getLatencyInSamples()) + ::dsp::SpectralGridProcessor::kFftSize / 4);

    slopeFilterStates = {};
    jitterPhases = {};

    for (int i = 0; i < kNumMappedParams; ++i)
        paramSmoothed[static_cast<size_t> (i)] = parameters.getRawParameterValue (mappedParamIds[i])->load();

    recomputeExplorationModulation();
}

void BrokenAudioProcessor::releaseResources()
{
    dryBuffer.setSize (0, 0);
}

bool BrokenAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void BrokenAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    const auto numSamples = buffer.getNumSamples();
    dryBuffer.makeCopyOf (buffer, true);

    static constexpr float kEnvReference = 0.5f;
    const auto seed = static_cast<uint32_t> (std::round (parameters.getRawParameterValue ("seed")->load()));
    const auto possibility = parameters.getRawParameterValue ("possibility")->load();

    auto readParam = [&] (int index) -> float
    {
        const auto base = parameters.getRawParameterValue (mappedParamIds[index])->load();
        auto& smooth = paramSmoothed[static_cast<size_t> (index)];
        smooth += (base - smooth) * 0.3f;

        const auto modAmount = modulationStates[static_cast<size_t> (index)].appliedModulation.load();
        if (std::abs (modAmount) < 0.001f)
            return smooth;

        auto* param = parameters.getParameter (mappedParamIds[index]);
        if (param == nullptr)
            return smooth;

        const auto normBase = param->convertTo0to1 (smooth);
        const auto modulated = juce::jlimit (0.0f, 1.0f, normBase + modAmount * 0.5f);
        return param->convertFrom0to1 (modulated);
    };

    const auto globalRiseMul = readParam (38);
    const auto globalFallMul = readParam (39);
    const auto globalGainMul = readParam (40);
    const auto globalSlopeSpeedMult = readParam (44);
    const auto globalSlopeSpikeMult = readParam (45);
    const auto globalSlopeGain      = readParam (46);
    const auto globalJitterSpeedMult = readParam (47);
    const auto globalJitterSpikeMult = readParam (48);
    const auto globalJitterGain      = readParam (49);
    const auto globalModAmount       = parameters.getRawParameterValue ("global_mod_amount")->load();

    float prevInputLevel = 0.0f;
    const auto invSampleRate = static_cast<float> (1.0 / getSampleRate());

    struct ParamLocal
    {
        float slopeSpeed;
        float slopeSpikyness;
        float slopeAmount;
        float slopeCoeff;
        float jitterSpeed;
        float jitterSpikyness;
        float jitterAmount;
    };
    std::array<ParamLocal, kNumMappedParams> paramLocal;

    for (int i = 0; i < kNumMappedParams; ++i)
    {
        auto& mod = modulationStates[static_cast<size_t> (i)];
        auto& pl = paramLocal[static_cast<size_t> (i)];
        pl.slopeSpeed = mod.slopeSpeed.load();
        pl.slopeSpikyness = mod.slopeSpikyness.load();
        pl.slopeAmount = mod.slopeAmount.load();
        pl.jitterSpeed = mod.jitterSpeed.load();
        pl.jitterSpikyness = mod.jitterSpikyness.load();
        pl.jitterAmount = mod.jitterAmount.load();

        const auto speedMs = juce::jmax (0.001f, pl.slopeSpeed * globalSlopeSpeedMult);
        pl.slopeCoeff = speedMs < 0.5f ? 1.0f : std::exp (-1000.0f / static_cast<float> (getSampleRate() * speedMs * 0.001f));

        envelopeFollowers[static_cast<size_t> (i)].setCoefficients (
            mod.envRise.load() * globalRiseMul, mod.envFall.load() * globalFallMul);
    }

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float inputLevel = 0.0f;
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            inputLevel += std::abs (dryBuffer.getSample (ch, sample));
        inputLevel /= static_cast<float> (buffer.getNumChannels());

        const auto slopeRaw = inputLevel - prevInputLevel;
        prevInputLevel = inputLevel;

        for (int i = 0; i < kNumMappedParams; ++i)
        {
            const auto& pl = paramLocal[static_cast<size_t> (i)];

            slopeFilterStates[static_cast<size_t> (i)] += (slopeRaw - slopeFilterStates[static_cast<size_t> (i)]) * pl.slopeCoeff;

            jitterPhases[static_cast<size_t> (i)] += pl.jitterSpeed * globalJitterSpeedMult * invSampleRate;

            envelopeFollowers[static_cast<size_t> (i)].process (inputLevel);
        }
    }

    for (int i = 0; i < kNumMappedParams; ++i)
    {
        const auto& pl = paramLocal[static_cast<size_t> (i)];
        auto& mod = modulationStates[static_cast<size_t> (i)];

        const auto slopeState = slopeFilterStates[static_cast<size_t> (i)];
        const auto spike = juce::jmax (0.01f, pl.slopeSpikyness * globalSlopeSpikeMult);
        const auto absSlope = std::abs (slopeState);
        const auto shaped = std::pow (absSlope, spike);
        const auto slopeValue = (slopeState >= 0.0f ? shaped : -shaped);
        mod.slopeApplied.store (slopeValue * pl.slopeAmount * globalSlopeGain);

        const auto jRaw = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 50 + 500), jitterPhases[static_cast<size_t> (i)]) * 2.0f - 1.0f;
        const auto jSpike = juce::jmax (0.01f, pl.jitterSpikyness * globalJitterSpikeMult);
        const auto jAbs = std::abs (jRaw);
        const auto jShaped = std::pow (jAbs, jSpike);
        const auto jitterValue = (jRaw >= 0.0f ? jShaped : -jShaped);
        mod.jitterApplied.store (jitterValue * pl.jitterAmount * globalJitterGain);
    }

    for (int i = 0; i < kNumMappedParams; ++i)
    {
        auto& mod = modulationStates[static_cast<size_t> (i)];

        auto manualMod = mod.manualModulation.load();
        float effectiveMod;

        if (mod.envModAmountExplEnabled.load() && i != 28)
        {
            const auto nv = gravel::SmoothNoise::sample (seed, static_cast<uint32_t> (i * 7 + 100), possibility * 3.0f);
            const auto noiseMod = (nv - 0.5f) * 2.0f;
            if (std::abs (manualMod) > 0.001f)
                effectiveMod = juce::jlimit (-1.0f, 1.0f, manualMod * noiseMod * 2.0f);
            else
                effectiveMod = noiseMod;
        }
        else
        {
            effectiveMod = manualMod;
        }

        mod.modulationAmount.store (effectiveMod);

        const auto envRaw = envelopeFollowers[static_cast<size_t> (i)].getEnvelope();
        const auto envNorm = juce::jlimit (0.0f, 1.0f, envRaw / kEnvReference);
        auto slopeApplied = mod.slopeApplied.load();
        auto jitterApplied = mod.jitterApplied.load();
        if (! mod.slopeEnabled.load()) slopeApplied = 0.0f;
        if (! mod.jitterEnabled.load()) jitterApplied = 0.0f;
        const auto applied = juce::jlimit (-1.0f, 1.0f,
            envNorm * mod.envGain.load() * globalGainMul * effectiveMod
            + slopeApplied + jitterApplied);
        mod.appliedModulation.store (applied * globalModAmount);
    }

    const auto effMixParam = readParam (24);
    const auto effOutputParam = readParam (25);

    // Forward modulated values to spectralGrid
    spectralGrid.setPreEqFreq (0, readParam (0));
    spectralGrid.setPreEqQ (0, readParam (1));
    spectralGrid.setPreEqGain (0, readParam (2));
    spectralGrid.setPreEqFreq (1, readParam (3));
    spectralGrid.setPreEqQ (1, readParam (4));
    spectralGrid.setPreEqGain (1, readParam (5));
    spectralGrid.setPreEqFreq (2, readParam (6));
    spectralGrid.setPreEqQ (2, readParam (7));
    spectralGrid.setPreEqGain (2, readParam (8));
    spectralGrid.setPostEqDepth (readParam (9));
    spectralGrid.setCrossoverFreq (0, readParam (10));
    spectralGrid.setCrossoverFreq (1, readParam (11));
    spectralGrid.setBandLevel (0, readParam (12));
    spectralGrid.setBandLevel (1, readParam (13));
    spectralGrid.setBandLevel (2, readParam (14));
    for (int cell = 0; cell < 9; ++cell)
        spectralGrid.setCellShape (cell, readParam (15 + cell));
    for (int cell = 0; cell < 9; ++cell)
        spectralGrid.setCellDrive (cell, readParam (29 + cell));
    for (int cell = 0; cell < 9; ++cell)
        spectralGrid.setCellBias (cell, parameters.getRawParameterValue (cellBiasIDs[cell])->load());

    spectralGrid.setThresholdLine (0, readParam (26));
    spectralGrid.setThresholdLine (1, readParam (27));
    spectralGrid.setHysteresisDepth (0, readParam (41));
    spectralGrid.setHysteresisDepth (1, readParam (42));
    spectralGrid.setHysteresisDepth (2, readParam (43));

    juce::dsp::AudioBlock<float> outputBlock (buffer);
    auto oversampledBlock = oversampler.processSamplesUp (outputBlock);

    {
        const auto osChannels = static_cast<int> (oversampledBlock.getNumChannels());
        const auto osSamples = static_cast<int> (oversampledBlock.getNumSamples());

        std::vector<float*> channelPtrs (static_cast<size_t> (osChannels));
        for (int ch = 0; ch < osChannels; ++ch)
            channelPtrs[static_cast<size_t> (ch)] = oversampledBlock.getChannelPointer (static_cast<size_t> (ch));

        juce::AudioBuffer<float> osBuffer (channelPtrs.data(), osChannels, osSamples);
        const auto drive = parameters.getRawParameterValue ("drive")->load();
        if (std::abs (drive - 1.0f) > 0.001f)
            osBuffer.applyGain (drive);
        spectralGrid.processBlock (osBuffer);
    }

    oversampler.processSamplesDown (outputBlock);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        for (int sample = 0; sample < numSamples; ++sample)
            buffer.setSample (channel, sample, (dryBuffer.getSample (channel, sample) * (1.0f - effMixParam)
                                               + buffer.getSample (channel, sample) * effMixParam) * juce::jlimit (0.0f, 1.0f, effOutputParam));
}

std::array<float, ::dsp::SpectralGridProcessor::kWaveTableSize> BrokenAudioProcessor::getCellShapeData (int cellIndex) const
{
    return spectralGrid.getCellShapeData (cellIndex);
}

juce::AudioProcessorEditor* BrokenAudioProcessor::createEditor()
{
    return new BrokenAudioProcessorEditor (*this);
}

void BrokenAudioProcessor::getStateInformation (juce::MemoryBlock& destination)
{
    if (auto state = parameters.copyState(); state.isValid())
    {
        auto modTree = juce::ValueTree ("modulation");
        for (int i = 0; i < kNumMappedParams; ++i)
            modTree.addChild (modulationStates[static_cast<size_t> (i)].toValueTree (mappedParamIds[i]), i, nullptr);
        state.addChild (modTree, -1, nullptr);
        copyXmlToBinary (*state.createXml(), destination);
    }
}

void BrokenAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
    {
        if (xml->hasTagName (parameters.state.getType()))
        {
            auto state = juce::ValueTree::fromXml (*xml);
            if (auto modTree = state.getChildWithName ("modulation"); modTree.isValid())
            {
                for (int i = 0; i < kNumMappedParams && i < modTree.getNumChildren(); ++i)
                {
                    modulationStates[static_cast<size_t> (i)].fromValueTree (modTree.getChild (i));
                    possibilitySpace.setExploration (
                        mappedParamIds[i],
                        modulationStates[static_cast<size_t> (i)].explMin.load(),
                        modulationStates[static_cast<size_t> (i)].explMax.load(),
                        modulationStates[static_cast<size_t> (i)].explEnabled.load());
                }
            }
            parameters.replaceState (state);
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BrokenAudioProcessor();
}
