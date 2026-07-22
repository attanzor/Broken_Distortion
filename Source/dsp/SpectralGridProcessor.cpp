#include "SpectralGridProcessor.h"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace
{
    float normToFreq (float norm) { return 20.0f * std::pow (20000.0f / 20.0f, norm); }
    float freqToNorm (float freq) { return std::log (freq / 20.0f) / std::log (20000.0f / 20.0f); }
}

namespace dsp
{

SpectralGridProcessor::SpectralGridProcessor()
{
    for (auto& perm : cellPermutations)
        std::iota (perm.begin(), perm.end(), 0);
}

void SpectralGridProcessor::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;

    juce::dsp::ProcessSpec spec {
        sampleRate,
        static_cast<juce::uint32> (samplesPerBlock),
        static_cast<juce::uint32> (2)
    };

    for (auto& stage : preEqStages)
    {
        stage.filter.state = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
            sampleRate, stage.freqHz, stage.q, juce::Decibels::decibelsToGain (stage.gainDb));
        stage.filter.prepare (spec);
        stage.filter.reset();
    }

    for (auto& stage : postEqStages)
    {
        stage.filter.state = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
            sampleRate, stage.freqHz, stage.q, juce::Decibels::decibelsToGain (stage.gainDb));
        stage.filter.prepare (spec);
        stage.filter.reset();
    }

    crossoverLow.prepare (spec);
    crossoverLow.setType (juce::dsp::LinkwitzRileyFilter<float>::Type::lowpass);
    crossoverLow.setCutoffFrequency (static_cast<float> (crossFreqLow));

    crossoverMid.prepare (spec);
    crossoverMid.setType (juce::dsp::LinkwitzRileyFilter<float>::Type::allpass);
    crossoverMid.setCutoffFrequency (static_cast<float> (crossFreqLow));

    crossoverHigh.prepare (spec);
    crossoverHigh.setType (juce::dsp::LinkwitzRileyFilter<float>::Type::highpass);
    crossoverHigh.setCutoffFrequency (static_cast<float> (crossFreqMid));

    for (auto& band : fftTimeDomain)
        for (auto& buf : band)
            buf.assign (static_cast<size_t> (kFftSize), 0.0f);

    for (auto& band : fftFreqDomain)
        for (auto& buf : band)
            buf.assign (static_cast<size_t> (kFftSize), std::complex<float> (0.0f, 0.0f));

    const auto maxOsBlockSize = static_cast<size_t> (samplesPerBlock) * 4; // 4x oversampling
    const auto outBufSize = maxOsBlockSize + static_cast<size_t> (kFftSize);

    for (auto& band : fftOutputBuffer)
        for (auto& buf : band)
            buf.assign (outBufSize, 0.0f);

    for (int i = 0; i < kFftSize; ++i)
        hannWindow[i] = 0.5f * (1.0f - std::cos (2.0f * juce::MathConstants<float>::pi * static_cast<float> (i) / static_cast<float> (kFftSize - 1)));

    for (auto& band : fftWritePos)
        band.fill (0);

    for (auto& band : fftOlaOffset)
        band.fill (0);

    for (auto& band : prevNormMag)
        for (auto& ch : band)
            ch.fill (0.0f);

    rebuildBaseCurves();
    rebuildCellPermutations();

    for (int i = 0; i < kNumCells; ++i)
        updateCellWavetable (i);
}

void SpectralGridProcessor::reset()
{
    for (auto& stage : preEqStages)
        stage.filter.reset();

    for (auto& stage : postEqStages)
        stage.filter.reset();

    crossoverLow.reset();
    crossoverMid.reset();
    crossoverHigh.reset();

    for (auto& band : fftWritePos)
        band.fill (0);

    for (auto& band : fftOlaOffset)
        band.fill (0);

    for (auto& band : fftTimeDomain)
        for (auto& buf : band)
            std::fill (buf.begin(), buf.end(), 0.0f);

    for (auto& band : fftFreqDomain)
        for (auto& buf : band)
            std::fill (buf.begin(), buf.end(), std::complex<float> (0.0f, 0.0f));

    for (auto& band : fftOutputBuffer)
        for (auto& buf : band)
            std::fill (buf.begin(), buf.end(), 0.0f);

    for (auto& band : prevNormMag)
        for (auto& ch : band)
            ch.fill (0.0f);
}

void SpectralGridProcessor::processBlock (juce::AudioBuffer<float>& buffer)
{
    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();

    if (numSamples == 0)
        return;

    cellActivity.fill (0.0f);

    // 1. Pre-EQ
    if (! eqBypassed)
        applyEq (buffer, preEqStages);

    if (bypassFrequencySplit)
    {
        const auto& table = cellWavetables[4].buffer;
        for (int ch = 0; ch < numChannels; ++ch)
            for (int s = 0; s < numSamples; ++s)
                buffer.setSample (ch, s, lookupWaveShape (table, buffer.getSample (ch, s)));
    }
    else
    {
    // 2. Crossover — create 3 band buffers
    juce::AudioBuffer<float> lowBand (numChannels, numSamples);
    juce::AudioBuffer<float> midBand (numChannels, numSamples);
    juce::AudioBuffer<float> highBand (numChannels, numSamples);

    for (int ch = 0; ch < numChannels; ++ch)
    {
        lowBand.copyFrom (ch, 0, buffer, ch, 0, numSamples);
        midBand.copyFrom (ch, 0, buffer, ch, 0, numSamples);
        highBand.copyFrom (ch, 0, buffer, ch, 0, numSamples);
    }

    {
        juce::dsp::AudioBlock<float> block (lowBand);
        juce::dsp::ProcessContextReplacing<float> ctx (block);
        crossoverLow.process (ctx);
    }
    {
        juce::dsp::AudioBlock<float> block (midBand);
        juce::dsp::ProcessContextReplacing<float> ctx (block);
        crossoverMid.process (ctx);
    }
    {
        juce::dsp::AudioBlock<float> block (highBand);
        juce::dsp::ProcessContextReplacing<float> ctx (block);
        crossoverHigh.process (ctx);
    }

    for (int ch = 0; ch < numChannels; ++ch)
    {
        for (int s = 0; s < numSamples; ++s)
        {
            const auto low = lowBand.getSample (ch, s);
            const auto mid = midBand.getSample (ch, s);
            const auto high = highBand.getSample (ch, s);
            midBand.setSample (ch, s, mid - low - high);
            lowBand.setSample (ch, s, low * bandLevels[0]);
            midBand.setSample (ch, s, midBand.getSample (ch, s) * bandLevels[1]);
            highBand.setSample (ch, s, high * bandLevels[2]);
        }
    }

    // 3. Per-band processing
    std::array<juce::AudioBuffer<float>, 3> bandBuffers = {
        std::move (lowBand), std::move (midBand), std::move (highBand)
    };

    if (bypassAmplitudeSplit)
    {
        for (int band = 0; band < kNumBands; ++band)
        {
            const auto& table = cellWavetables[band * 3 + 1].buffer;
            auto& bandBuf = bandBuffers[band];
            for (int ch = 0; ch < numChannels; ++ch)
                for (int s = 0; s < numSamples; ++s)
                    bandBuf.setSample (ch, s, lookupWaveShape (table, bandBuf.getSample (ch, s) + cellBiases[band * 3 + 1]));
        }
    }
    else
    {
    constexpr float kMagReference = static_cast<float> (kFftSize) * 0.25f; // Hann window: full-scale sine -> N/4

    for (int band = 0; band < kNumBands; ++band)
    {
        auto& bandBuf = bandBuffers[band];

        if (bypassWaveshapers)
            continue;   // pass band through unchanged (linear crossover+sum test)

        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto& ringBuf = fftTimeDomain[band][ch];
            auto& outBuf = fftOutputBuffer[band][ch];
            auto& writePos = fftWritePos[band][ch];
            auto& olaOffset = fftOlaOffset[band][ch];

            for (int s = 0; s < numSamples; ++s)
            {
                ringBuf[writePos] = bandBuf.getSample (ch, s);
                ++writePos;

                if (writePos >= static_cast<size_t> (kFftSize))
                {
                    std::array<std::complex<float>, kFftSize> frame;
                    for (int i = 0; i < kFftSize; ++i)
                        frame[i] = ringBuf[i] * hannWindow[i];

                    fft.perform (frame.data(), frame.data(), false);

                    for (int bin = 0; bin <= kFftSize / 2; ++bin)
                    {
                        auto& c = frame[static_cast<size_t> (bin)];
                        const auto mag = std::abs (c);

                        float scale;
                        if (bypassWaveshapers)
                        {
                            scale = 1.0f;
                        }
                        else
                        {
                            const auto normMag = mag / kMagReference;

                            const auto tl = thresholdLow.load();
                            const auto th = thresholdHigh.load();

                            float w0, w1, w2;
                            if (normMag < tl)
                            {
                                w0 = 1.0f; w1 = 0.0f; w2 = 0.0f;
                            }
                            else if (normMag < th)
                            {
                                const auto t = (normMag - tl) / (th - tl + 1e-10f);
                                w0 = 1.0f - t; w1 = t; w2 = 0.0f;
                            }
                            else
                            {
                            const auto t = std::min (1.0f, (normMag - th) / (1.0f - th + 1e-10f));
                            w0 = 0.0f; w1 = 1.0f - t; w2 = t;
                        }

                        const auto hyst = hysteresisDepth[band];
                        if (hyst > 0.0f)
                        {
                            auto& prev = prevNormMag[band][ch][static_cast<size_t> (bin)];
                            if (normMag > prev)
                            {
                                w0 = std::max (0.0f, w0 - hyst);
                                w2 = std::min (1.0f, w2 + hyst);
                            }
                            else if (normMag < prev)
                            {
                                w2 = std::max (0.0f, w2 - hyst);
                                w0 = std::min (1.0f, w0 + hyst);
                            }
                            const auto sum = w0 + w1 + w2;
                            w0 /= sum; w1 /= sum; w2 /= sum;
                            prev = normMag;
                        }

                        auto shapedNorm = 0.0f;
                            shapedNorm += w0 * lookupWaveShape (cellWavetables[band * 3 + 0].buffer, normMag * cellDrives[band * 3 + 0] + cellBiases[band * 3 + 0]);
                            shapedNorm += w1 * lookupWaveShape (cellWavetables[band * 3 + 1].buffer, normMag * cellDrives[band * 3 + 1] + cellBiases[band * 3 + 1]);
                            shapedNorm += w2 * lookupWaveShape (cellWavetables[band * 3 + 2].buffer, normMag * cellDrives[band * 3 + 2] + cellBiases[band * 3 + 2]);

                            cellActivity[static_cast<size_t> (band * 3 + 0)] += w0 * normMag;
                            cellActivity[static_cast<size_t> (band * 3 + 1)] += w1 * normMag;
                            cellActivity[static_cast<size_t> (band * 3 + 2)] += w2 * normMag;

                            scale = (mag > 1e-10f) ? shapedNorm / normMag : 0.0f;
                        }

                        c *= scale;

                        if (bin > 0 && bin < kFftSize / 2)
                            frame[static_cast<size_t> (kFftSize - bin)] = std::conj (c);
                    }

                    fft.perform (frame.data(), frame.data(), true);

                    // OLA: each frame's output is placed at olaOffset, which advances
                    // by kHopSize per frame so overlapping frames are staggered correctly.
                    for (int i = 0; i < kFftSize; ++i)
                        outBuf[olaOffset + static_cast<size_t> (i)] += frame[static_cast<size_t> (i)].real() * hannWindow[i];

                    olaOffset += static_cast<size_t> (kHopSize);

                    for (int i = 0; i < kFftSize - kHopSize; ++i)
                        ringBuf[static_cast<size_t> (i)] = ringBuf[static_cast<size_t> (i + kHopSize)];

                    writePos -= kHopSize;
                }
            }

            // Emit the oldest numSamples (fully overlapped at steady state).
            for (int s = 0; s < numSamples; ++s)
                bandBuf.setSample (ch, s, outBuf[static_cast<size_t> (s)]);

            // Shift left by numSamples, zero tail.
            for (int i = numSamples; i < static_cast<int> (outBuf.size()); ++i)
                outBuf[static_cast<size_t> (i - numSamples)] = outBuf[static_cast<size_t> (i)];

            for (int i = static_cast<int> (outBuf.size()) - numSamples; i < static_cast<int> (outBuf.size()); ++i)
                outBuf[static_cast<size_t> (i)] = 0.0f;

            // Adjust OLA offset: samples consumed from front
            if (olaOffset >= static_cast<size_t> (numSamples))
                olaOffset -= static_cast<size_t> (numSamples);
            else
                olaOffset = 0;
        }
    }
    } // end bypassAmplitudeSplit else

    // Normalize cell activity per-band
    for (int band = 0; band < kNumBands; ++band)
    {
        auto maxVal = 0.0f;
        for (int lay = 0; lay < kNumLayers; ++lay)
            maxVal = juce::jmax (maxVal, cellActivity[static_cast<size_t> (band * kNumLayers + lay)]);
        if (maxVal > 0.001f)
            for (int lay = 0; lay < kNumLayers; ++lay)
                cellActivity[static_cast<size_t> (band * kNumLayers + lay)] /= maxVal;
    }

    // 4. Sum bands back to main buffer
    buffer.clear();
    for (int ch = 0; ch < numChannels; ++ch)
    {
        for (int s = 0; s < numSamples; ++s)
        {
            auto sum = 0.0f;
            for (int band = 0; band < kNumBands; ++band)
                sum += bandBuffers[band].getSample (ch, s);
            buffer.setSample (ch, s, sum);
        }
    }
    } // end bypassFrequencySplit else

    // 5. Post-EQ
    if (! eqBypassed)
        applyEq (buffer, postEqStages);
}

void SpectralGridProcessor::setPreEqFreq (int stage, float freqHz)
{
    if (stage < 0 || stage >= 3) return;
    preEqStages[stage].freqHz = juce::jlimit (20.0f, 20000.0f, freqHz);
    postEqStages[stage].freqHz = freqHz;

    auto makeCoeffs = [&](int s, float gain) -> juce::dsp::IIR::Coefficients<float>::Ptr
    {
        const auto f = (s == 0) ? preEqStages[0].freqHz : (s == 1) ? preEqStages[1].freqHz : preEqStages[2].freqHz;
        const auto q = (s == 0) ? preEqStages[0].q : (s == 1) ? preEqStages[1].q : preEqStages[2].q;
        if (s == 0) return juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
        if (s == 1) return juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
        return juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
    };

    *preEqStages[stage].filter.state = *makeCoeffs (stage, preEqStages[stage].gainDb);
    *postEqStages[stage].filter.state = *makeCoeffs (stage, -postEqStages[stage].gainDb * postEqDepth);
}

void SpectralGridProcessor::setPreEqQ (int stage, float q)
{
    if (stage < 0 || stage >= 3) return;
    preEqStages[stage].q = juce::jlimit (0.05f, 20.0f, q);
    postEqStages[stage].q = q;

    auto makeCoeffs = [&](int s, float gain) -> juce::dsp::IIR::Coefficients<float>::Ptr
    {
        const auto f = (s == 0) ? preEqStages[0].freqHz : (s == 1) ? preEqStages[1].freqHz : preEqStages[2].freqHz;
        const auto qv = (s == 0) ? preEqStages[0].q : (s == 1) ? preEqStages[1].q : preEqStages[2].q;
        if (s == 0) return juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, f, qv, juce::Decibels::decibelsToGain (gain));
        if (s == 1) return juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, f, qv, juce::Decibels::decibelsToGain (gain));
        return juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, f, qv, juce::Decibels::decibelsToGain (gain));
    };

    *preEqStages[stage].filter.state = *makeCoeffs (stage, preEqStages[stage].gainDb);
    *postEqStages[stage].filter.state = *makeCoeffs (stage, -postEqStages[stage].gainDb * postEqDepth);
}

void SpectralGridProcessor::setPreEqGain (int stage, float gainDb)
{
    if (stage < 0 || stage >= 3) return;
    preEqStages[stage].gainDb = juce::jlimit (-30.0f, 30.0f, gainDb);
    postEqStages[stage].gainDb = gainDb;

    auto makeCoeffs = [&](int s, float gain) -> juce::dsp::IIR::Coefficients<float>::Ptr
    {
        const auto f = (s == 0) ? preEqStages[0].freqHz : (s == 1) ? preEqStages[1].freqHz : preEqStages[2].freqHz;
        const auto q = (s == 0) ? preEqStages[0].q : (s == 1) ? preEqStages[1].q : preEqStages[2].q;
        if (s == 0) return juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
        if (s == 1) return juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
        return juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
    };

    *preEqStages[stage].filter.state = *makeCoeffs (stage, preEqStages[stage].gainDb);
    *postEqStages[stage].filter.state = *makeCoeffs (stage, -postEqStages[stage].gainDb * postEqDepth);
}

void SpectralGridProcessor::setPostEqDepth (float depth)
{
    postEqDepth = juce::jlimit (0.0f, 1.0f, depth);

    auto makeCoeffs = [&](int s, float gain) -> juce::dsp::IIR::Coefficients<float>::Ptr
    {
        const auto f = (s == 0) ? postEqStages[0].freqHz : (s == 1) ? postEqStages[1].freqHz : postEqStages[2].freqHz;
        const auto q = (s == 0) ? postEqStages[0].q : (s == 1) ? postEqStages[1].q : postEqStages[2].q;
        if (s == 0) return juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
        if (s == 1) return juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
        return juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, f, q, juce::Decibels::decibelsToGain (gain));
    };

    for (int s = 0; s < 3; ++s)
        *postEqStages[s].filter.state = *makeCoeffs (s, -postEqStages[s].gainDb * postEqDepth);
}

void SpectralGridProcessor::setCrossoverFreq (int bandIndex, float freqHz)
{
    freqHz = juce::jlimit (20.0f, 20000.0f, freqHz);
    const auto rawNorm = freqToNorm (freqHz);

    if (bandIndex == 0)
        rawCrossLowNorm = rawNorm;
    else
        rawCrossHighNorm = rawNorm;

    const auto effLowNorm = rawCrossLowNorm * rawCrossLowNorm;
    const auto effHighNorm = effLowNorm + rawCrossHighNorm * (1.0f - effLowNorm);

    crossFreqLow = normToFreq (effLowNorm);
    crossFreqMid = normToFreq (effHighNorm);

    crossoverLow.setCutoffFrequency (crossFreqLow);
    crossoverMid.setCutoffFrequency (crossFreqLow);
    crossoverHigh.setCutoffFrequency (crossFreqMid);
}

void SpectralGridProcessor::setBandLevel (int bandIndex, float level)
{
    if (bandIndex >= 0 && bandIndex < 3)
        bandLevels[bandIndex] = juce::jlimit (0.0f, 1.0f, level);
}

void SpectralGridProcessor::setCellShape (int cellIndex, float morph)
{
    if (cellIndex >= 0 && cellIndex < kNumCells)
    {
        cellWavetables[cellIndex].shapeMorph = juce::jlimit (0.0f, 1.0f, morph);
        updateCellWavetable (cellIndex);
    }
}

void SpectralGridProcessor::setCellDrive (int cellIndex, float drive)
{
    if (cellIndex >= 0 && cellIndex < kNumCells)
        cellDrives[cellIndex] = juce::jmax (1.0f, drive);
}

void SpectralGridProcessor::setCellBias (int cellIndex, float bias)
{
    if (cellIndex >= 0 && cellIndex < kNumCells)
        cellBiases[cellIndex] = juce::jlimit (-1.0f, 1.0f, bias);
}

void SpectralGridProcessor::setThresholdLine (int lineIndex, float value)
{
    value = juce::jlimit (0.0f, 1.0f, value);

    if (lineIndex == 0)
        rawThresholdLow = value;
    else
        rawThresholdHigh = value;

    const auto effLow = rawThresholdLow * rawThresholdLow;
    const auto effHigh = effLow + rawThresholdHigh * (1.0f - effLow);

    thresholdLow.store (effLow);
    thresholdHigh.store (effHigh);
}

float SpectralGridProcessor::getThresholdLine (int lineIndex) const
{
    if (lineIndex == 0) return thresholdLow.load();
    if (lineIndex == 1) return thresholdHigh.load();
    return 0.0f;
}

float SpectralGridProcessor::getCrossoverNorm (int bandIndex) const
{
    const auto effLowNorm = rawCrossLowNorm * rawCrossLowNorm;
    const auto effHighNorm = effLowNorm + rawCrossHighNorm * (1.0f - effLowNorm);
    return (bandIndex == 0) ? effLowNorm : effHighNorm;
}

float SpectralGridProcessor::getRawThreshold (int lineIndex) const
{
    return (lineIndex == 0) ? rawThresholdLow : rawThresholdHigh;
}

float SpectralGridProcessor::getRawCrossNorm (int bandIndex) const
{
    return (bandIndex == 0) ? rawCrossLowNorm : rawCrossHighNorm;
}

void SpectralGridProcessor::setSeed (uint32_t seed)
{
    currentSeed = seed;
    rebuildBaseCurves();
    rebuildCellPermutations();
    for (int i = 0; i < kNumCells; ++i)
        updateCellWavetable (i);
}

void SpectralGridProcessor::setEqBypass (bool bypass)
{
    eqBypassed = bypass;
}

void SpectralGridProcessor::setHysteresisDepth (int band, float depth)
{
    if (band >= 0 && band < kNumBands)
        hysteresisDepth[static_cast<size_t> (band)] = juce::jlimit (0.0f, 1.0f, depth);
}

void SpectralGridProcessor::setBypassWaveshapers (bool bypass)      { bypassWaveshapers = bypass; }
void SpectralGridProcessor::setBypassAmplitudeSplit (bool bypass)   { bypassAmplitudeSplit = bypass; }
void SpectralGridProcessor::setBypassFrequencySplit (bool bypass)   { bypassFrequencySplit = bypass; }

std::array<float, SpectralGridProcessor::kWaveTableSize> SpectralGridProcessor::getCellShapeData (int cellIndex) const
{
    if (cellIndex >= 0 && cellIndex < kNumCells)
        return cellWavetables[cellIndex].buffer;

    return {};
}

void SpectralGridProcessor::rebuildBaseCurves()
{
    for (int curve = 0; curve < kNumBaseCurves; ++curve)
    {
        for (int s = 0; s < kWaveTableSize; ++s)
        {
            const auto x = (static_cast<float> (s) / static_cast<float> (kWaveTableSize - 1)) * 2.0f - 1.0f;
            float value = 0.0f;

            switch (curve)
            {
                case 0: value = x; break;
                case 1: value = std::tanh (x * 2.2f) / std::tanh (2.2f); break;
                case 2: value = juce::jlimit (-1.0f, 1.0f, x * 3.0f); break;
                case 3: value = std::atan (x * 4.0f) / std::atan (4.0f); break;
                case 4: value = std::sin (x * juce::MathConstants<float>::halfPi); break;
                case 5: value = std::copysign ((1.0f - std::exp (-3.0f * std::abs (x))) / (1.0f - std::exp (-3.0f)), x); break;
                case 6: value = (2.0f * x) / (1.0f + std::abs (x)); break;
                case 7: value = 4.0f * x * x * x - 3.0f * x; break;
                case 8: value = std::copysign (x * x, x); break; // square-law
            }

            baseCurves[curve][s] = juce::jlimit (-1.0f, 1.0f, value);
        }
    }
}

void SpectralGridProcessor::rebuildCellPermutations()
{
    for (int cell = 0; cell < kNumCells; ++cell)
    {
        juce::Random cellRandom (currentSeed + static_cast<uint32_t> (cell) * 7919u);

        std::array<int, kNumBaseCurves> indices;
        std::iota (indices.begin(), indices.end(), 0);
        std::array<float, kNumBaseCurves> order;
        for (auto& o : order)
            o = cellRandom.nextFloat();

        std::stable_sort (indices.begin(), indices.end(), [&order] (int a, int b)
        {
            return order[a] < order[b];
        });

        cellPermutations[cell] = indices;
    }
}

void SpectralGridProcessor::updateCellWavetable (int cellIndex)
{
    if (cellIndex < 0 || cellIndex >= kNumCells)
        return;

    const auto& perm = cellPermutations[cellIndex];
    const auto morph = cellWavetables[cellIndex].shapeMorph;
    const auto scaledPos = morph * static_cast<float> (kNumBaseCurves - 1);
    const auto lowerIdx = juce::jlimit (0, kNumBaseCurves - 2, static_cast<int> (std::floor (scaledPos)));
    const auto upperIdx = lowerIdx + 1;
    const auto alpha = scaledPos - static_cast<float> (lowerIdx);

    auto& dest = cellWavetables[cellIndex].buffer;
    for (int s = 0; s < kWaveTableSize; ++s)
    {
        const auto lower = baseCurves[perm[lowerIdx]][s];
        const auto upper = baseCurves[perm[upperIdx]][s];
        dest[s] = juce::jlimit (-1.0f, 1.0f, lower + (upper - lower) * alpha);
    }
}

float SpectralGridProcessor::lookupWaveShape (const WaveTable& table, float input) const
{
    const auto x = juce::jlimit (-1.0f, 1.0f, input);
    const auto index = static_cast<int> (((x + 1.0f) * 0.5f) * static_cast<float> (kWaveTableSize - 1));
    return table[juce::jlimit (0, kWaveTableSize - 1, index)];
}

void SpectralGridProcessor::applyEq (juce::AudioBuffer<float>& buffer, const std::array<EQStage, 3>& stages)
{
    juce::dsp::AudioBlock<float> block (buffer);

    for (const auto& stage : stages)
    {
        if (stage.filter.state != nullptr)
        {
            juce::dsp::ProcessContextReplacing<float> context (block);
            const_cast<juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>&> (stage.filter).process (context);
        }
    }
}

} // namespace dsp
