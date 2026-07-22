#pragma once

#include <JuceHeader.h>

#include <vector>

void fillSine (juce::AudioBuffer<float>& buffer, float freq, float sampleRate, int numSamples);
void fillMultiSine (juce::AudioBuffer<float>& buffer, const std::vector<float>& freqs, float sampleRate, int numSamples);
void fillNoise (juce::AudioBuffer<float>& buffer, int seed);
void fillImpulse (juce::AudioBuffer<float>& buffer, int sampleIndex);

bool bufferMatches (const juce::AudioBuffer<float>& expected,
                    const juce::AudioBuffer<float>& actual,
                    float tolerance);

bool bufferMatchesWithLatency (const juce::AudioBuffer<float>& input,
                               const juce::AudioBuffer<float>& output,
                               int latencySamples,
                               float tolerance);

float getRMS (const juce::AudioBuffer<float>& buffer);
