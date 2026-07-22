cmake_minimum_required(VERSION 3.22)

project(Broken VERSION 0.1.0)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.12
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(Broken
    COMPANY_NAME "Fledgeling Creations"
    BUNDLE_ID "com.fledgeling.broken"
    PLUGIN_MANUFACTURER_CODE Fled
    PLUGIN_CODE Brkn
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "Broken"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        AU_COPY_DIR "/Library/Audio/Plug-Ins/Components"
        VST3_COPY_DIR "/Library/Audio/Plug-Ins/VST3"
)

target_sources(Broken PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/gravel/PossibilitySpace.cpp
    Source/gravel/PossibilitySpace.h
    Source/gravel/ParameterSpace.cpp
    Source/gravel/ParameterSpace.h
    Source/gravel/RuneFader.cpp
    Source/gravel/RuneFader.h
    Source/gravel/SeedField.cpp
    Source/gravel/SeedField.h
    Source/gravel/ModulationState.h
    Source/gravel/EnvelopeFollower.h
    Source/gravel/ModulationPopup.cpp
    Source/gravel/ModulationPopup.h
    Source/gravel/FloatFader.cpp
    Source/gravel/FloatFader.h
    Source/dsp/SpectralGridProcessor.cpp
    Source/dsp/SpectralGridProcessor.h
    Source/ui/SpectralGridComponent.cpp
    Source/ui/SpectralGridComponent.h
    Source/ui/CellStripPanel.cpp
    Source/ui/CellStripPanel.h
    Source/tests/TestUtilities.cpp
    Source/tests/TestUtilities.h
    Source/tests/CrossoverSumTest.cpp
    Source/tests/FftRoundTripTest.cpp
    Source/tests/NoiseDeterminismTest.cpp
    Source/tests/ParameterMapTest.cpp
)

juce_generate_juce_header(Broken)

target_compile_definitions(Broken PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_UNIT_TESTS=1
)

target_link_libraries(Broken PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

juce_add_console_app(GravelTest
    COMPANY_NAME "Fledgeling Creations"
    BUNDLE_ID "com.fledgeling.test"
    NEEDS_CURL FALSE
    NEEDS_WEB_BROWSER FALSE
)
juce_generate_juce_header(GravelTest)
target_sources(GravelTest PRIVATE
    Source/tests/ConsoleTest.cpp
    Source/dsp/SpectralGridProcessor.cpp
    Source/dsp/SpectralGridProcessor.h
    Source/gravel/PossibilitySpace.cpp
    Source/gravel/PossibilitySpace.h
)
target_compile_definitions(GravelTest PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_UNIT_TESTS=1
)
target_link_libraries(GravelTest PRIVATE
    juce::juce_core
    juce::juce_dsp
)
