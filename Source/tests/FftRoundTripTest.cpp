#include "TestUtilities.h"

#include <JuceHeader.h>

#include <complex>
#include <cmath>

class FftRoundTripTest final : public juce::UnitTest
{
public:
    FftRoundTripTest() : juce::UnitTest ("FFT Round Trip", "Gravel DSP") {}

    void runTest() override
    {
        testColaProperty();
        testFftIdentity();
        testOverlapAddRoundTrip();
    }

private:
    void testColaProperty()
    {
        beginTest ("Hann window COLA");

        constexpr int fftSize = 1024;
        constexpr int hopSize = fftSize / 4;

        std::array<float, fftSize> window;
        for (int i = 0; i < fftSize; ++i)
            window[static_cast<size_t> (i)] = 0.5f * (1.0f - std::cos (2.0f * juce::MathConstants<float>::pi * static_cast<float> (i) / static_cast<float> (fftSize - 1)));

        constexpr int testLen = fftSize * 4;
        std::vector<float> sum (static_cast<size_t> (testLen), 0.0f);
        for (int pos = 0; pos < testLen; pos += hopSize)
            for (int i = 0; i < fftSize; ++i)
                if (pos + i < testLen)
                    sum[static_cast<size_t> (pos + i)] += window[static_cast<size_t> (i)];

        float minVal = sum[static_cast<size_t> (hopSize)];
        float maxVal = sum[static_cast<size_t> (hopSize)];
        for (size_t i = static_cast<size_t> (hopSize); i < sum.size() - static_cast<size_t> (hopSize); ++i)
        {
            minVal = std::min (minVal, sum[i]);
            maxVal = std::max (maxVal, sum[i]);
        }

        expectWithinAbsoluteError (maxVal, minVal, 1e-5f,
            juce::String ("Sum not constant: max=") + juce::String (maxVal) + " min=" + juce::String (minVal));
    }

    void testFftIdentity()
    {
        beginTest ("FFT -> IFFT identity");

        constexpr int fftOrder = 10;
        constexpr int fftSize = 1 << fftOrder;

        juce::dsp::FFT fft (fftOrder);

        std::array<std::complex<float>, fftSize> data;
        for (int i = 0; i < fftSize; ++i)
            data[static_cast<size_t> (i)] = std::sin (2.0f * juce::MathConstants<float>::pi * 5.0f * static_cast<float> (i) / static_cast<float> (fftSize))
                                          + 0.5f * std::sin (2.0f * juce::MathConstants<float>::pi * 17.0f * static_cast<float> (i) / static_cast<float> (fftSize));

        auto original = data;

        fft.perform (data.data(), data.data(), false);
        fft.perform (data.data(), data.data(), true);

        float maxDiff = 0.0f;
        float scale = 1.0f / static_cast<float> (fftSize);
        for (size_t i = 0; i < fftSize; ++i)
        {
            auto diff = std::abs (data[i].real() * scale - original[i].real());
            maxDiff = std::max (maxDiff, diff);
        }

        expect (maxDiff < 1e-5f, juce::String ("FFT -> IFFT maxDiff=") + juce::String (maxDiff));
    }

    void testOverlapAddRoundTrip()
    {
        beginTest ("Overlap-add round-trip");

        constexpr int fftOrder = 10;
        constexpr int fftSize = 1 << fftOrder;
        constexpr int hopSize = fftSize / 4;
        constexpr int numBlocks = 8;

        juce::dsp::FFT fft (fftOrder);

        std::array<float, fftSize> window;
        for (int i = 0; i < fftSize; ++i)
            window[static_cast<size_t> (i)] = 0.5f * (1.0f - std::cos (2.0f * juce::MathConstants<float>::pi * static_cast<float> (i) / static_cast<float> (fftSize - 1)));

        const int totalSamples = hopSize * numBlocks;
        std::vector<float> input (static_cast<size_t> (totalSamples));
        for (int i = 0; i < totalSamples; ++i)
            input[static_cast<size_t> (i)] = std::sin (2.0f * juce::MathConstants<float>::pi * 100.0f * static_cast<float> (i) / 48000.0f);

        std::vector<float> output (static_cast<size_t> (totalSamples), 0.0f);
        std::vector<float> ringBuf (static_cast<size_t> (fftSize), 0.0f);
        size_t writePos = 0;

        for (int block = 0; block < numBlocks; ++block)
        {
            for (int i = 0; i < hopSize; ++i)
                ringBuf[writePos++] = input[static_cast<size_t> (block * hopSize + i)];

            if (writePos >= static_cast<size_t> (hopSize))
            {
                std::array<std::complex<float>, fftSize> frame;
                for (int i = 0; i < fftSize; ++i)
                    frame[static_cast<size_t> (i)] = ringBuf[static_cast<size_t> (i)] * window[static_cast<size_t> (i)];

                fft.perform (frame.data(), frame.data(), false);
                fft.perform (frame.data(), frame.data(), true);

                for (int i = 0; i < fftSize; ++i)
                {
                    auto idx = static_cast<size_t> (block * hopSize + i);
                    if (idx < output.size())
                        output[idx] += frame[static_cast<size_t> (i)].real() * window[static_cast<size_t> (i)];
                }

                for (int i = 0; i < fftSize - hopSize; ++i)
                    ringBuf[static_cast<size_t> (i)] = ringBuf[static_cast<size_t> (i + hopSize)];

                writePos -= hopSize;
            }
        }

        const int steadyStateStart = hopSize * 2;
        const int compareLen = totalSamples - steadyStateStart;

        float maxDiff = 0.0f;
        for (int i = 0; i < compareLen; ++i)
        {
            auto idx = static_cast<size_t> (steadyStateStart + i);
            auto diff = std::abs (input[idx] - output[idx]);
            maxDiff = std::max (maxDiff, diff);
        }

        expect (maxDiff < 1e-2f, juce::String ("Overlap-add maxDiff=") + juce::String (maxDiff));
    }
};

static FftRoundTripTest fftRoundTripTest;
