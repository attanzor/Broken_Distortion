#include <JuceHeader.h>
#include "../dsp/SpectralGridProcessor.h"

#include <cmath>
#include <cstdio>

static float rms (const juce::AudioBuffer<float>& b)
{
    double s = 0;
    int n = 0;
    for (int ch = 0; ch < b.getNumChannels(); ++ch)
        for (int i = 0; i < b.getNumSamples(); ++i, ++n)
            s += (double) b.getSample (ch, i) * b.getSample (ch, i);
    return std::sqrt (s / std::max (1, n));
}

static float peak (const juce::AudioBuffer<float>& b)
{
    float p = 0;
    for (int ch = 0; ch < b.getNumChannels(); ++ch)
        for (int i = 0; i < b.getNumSamples(); ++i)
            p = std::max (p, std::abs (b.getSample (ch, i)));
    return p;
}

static void run (const char* label, ::dsp::SpectralGridProcessor& proc,
                 bool bypassFreq, bool bypassAmp, bool bypassShape,
                 float freq, float amp, int numBlocks)
{
    proc.setBypassFrequencySplit (bypassFreq);
    proc.setBypassAmplitudeSplit (bypassAmp);
    proc.setBypassWaveshapers (bypassShape);
    proc.setEqBypass (true);
    proc.setSeed (1024);

    const double sr = 48000.0;
    const int block = 256;
    proc.prepare (sr * 4, block * 4);
    proc.reset();

    juce::AudioBuffer<float> out (2, 0);
    double phase = 0.0;
    const double phaseInc = 2.0 * juce::MathConstants<double>::pi * freq / (sr * 4.0);

    for (int i = 0; i < numBlocks; ++i)
    {
        juce::AudioBuffer<float> buf (2, block * 4);
        for (int ch = 0; ch < 2; ++ch)
            for (int s = 0; s < block * 4; ++s, phase += phaseInc)
                buf.setSample (ch, s, amp * std::sin (phase));

        proc.processBlock (buf);

        const int old = out.getNumSamples();
        out.setSize (2, old + buf.getNumSamples(), true);
        for (int ch = 0; ch < 2; ++ch)
            out.copyFrom (ch, old, buf, ch, 0, buf.getNumSamples());
    }

    const int warmup = 4096;
    const int off = std::min (warmup, out.getNumSamples());
    juce::AudioBuffer<float> steady (2, out.getNumSamples() - off);
    for (int ch = 0; ch < 2; ++ch)
        steady.copyFrom (ch, 0, out, ch, off, out.getNumSamples() - off);

    std::printf ("%-34s inRMS=%.4f inPeak=%.3f  outRMS=%.4f outPeak=%.3f\n",
                 label, amp * 0.7071f, amp,
                 rms (steady), peak (steady));

    if (bypassFreq == false && bypassShape == true && amp == 0.1f && std::abs (freq - 1000.0f) < 1.0f)
    {
        std::printf ("   all samples > 0.12 (1.2x peak) in crossover+sum:\n");
        int count = 0;
        for (int i = 0; i < steady.getNumSamples() && count < 10; ++i)
            if (std::abs (steady.getSample (0, i)) > 0.12f)
            {
                std::printf ("    [%d] block=%d in-block=%d val=%+.4f\n",
                             i + off, (i + off) / 1024, (i + off) % 1024, steady.getSample (0, i));
                ++count;
            }
    }
}

int main()
{
    ::dsp::SpectralGridProcessor proc; (void)0;
    constexpr int blocks = 200;

    // ---- Direct crossover isolation test ----
    {
        const double sr = 48000.0;
        const int N = 4096;
        juce::dsp::ProcessSpec spec { sr, (juce::uint32) N, 1 };
        juce::dsp::LinkwitzRileyFilter<float> lp, ap, hp;
        lp.prepare (spec); ap.prepare (spec); hp.prepare (spec);
        lp.setType (juce::dsp::LinkwitzRileyFilter<float>::Type::lowpass);  lp.setCutoffFrequency (300.0f);
        ap.setType (juce::dsp::LinkwitzRileyFilter<float>::Type::allpass);  ap.setCutoffFrequency (300.0f);
        hp.setType (juce::dsp::LinkwitzRileyFilter<float>::Type::highpass); hp.setCutoffFrequency (3000.0f);

        juce::AudioBuffer<float> ib (1, N), lb (1, N), mb (1, N), hb (1, N);
        for (int i = 0; i < N; ++i) ib.setSample (0, i, 0.5f * std::sin (2.0 * juce::MathConstants<double>::pi * 1000.0 * i / sr));
        lb.copyFrom (0, 0, ib, 0, 0, N); mb.copyFrom (0, 0, ib, 0, 0, N); hb.copyFrom (0, 0, ib, 0, 0, N);
        { juce::dsp::AudioBlock<float> b (lb); juce::dsp::ProcessContextReplacing<float> c (b); lp.process (c); }
        { juce::dsp::AudioBlock<float> b (mb); juce::dsp::ProcessContextReplacing<float> c (b); ap.process (c); }
        { juce::dsp::AudioBlock<float> b (hb); juce::dsp::ProcessContextReplacing<float> c (b); hp.process (c); }
        juce::AudioBuffer<float> sum (1, N); sum.clear();
        for (int i = 0; i < N; ++i) sum.setSample (0, i, lb.getSample(0,i) + (mb.getSample(0,i) - lb.getSample(0,i) - hb.getSample(0,i)) + hb.getSample(0,i));

        auto p = [] (const juce::AudioBuffer<float>& b, int off)
        {
            float pk = 0; for (int i = off; i < b.getNumSamples(); ++i) pk = std::max (pk, std::abs (b.getSample (0, i)));
            return pk;
        };
        std::printf ("crossover direct: inPeak=%.4f sumPeak=%.4f  | low=%.4f mid=%.4f high=%.4f\n",
                     peak (ib), p (sum, 2048), p (lb, 2048), p (mb, 2048), p (hb, 2048));
        std::printf ("   sum[2048..2052]: %.4f %.4f %.4f %.4f %.4f\n",
                     sum.getSample(0,2048), sum.getSample(0,2049), sum.getSample(0,2050), sum.getSample(0,2051), sum.getSample(0,2052));
        std::printf ("   in [2048..2052]: %.4f %.4f %.4f %.4f %.4f\n\n",
                     ib.getSample(0,2048), ib.getSample(0,2049), ib.getSample(0,2050), ib.getSample(0,2051), ib.getSample(0,2052));
    }


    for (float amp : { 0.1f, 0.5f, 0.9f })
    {
        std::printf ("--- amp=%.2f, 1kHz sine ---\n", amp);
        run ("bypass_freq (single shaper)",   proc, true,  false, false, 1000.0f, amp, blocks);
        run ("freq + bypass_shape (FFT id)",  proc, false, false, true,  1000.0f, amp, blocks);
        run ("freq + bypass_amp (td shaper)", proc, false, true,  false, 1000.0f, amp, blocks);
        run ("freq + amp + shape (full)",      proc, false, false, false, 1000.0f, amp, blocks);
        run ("freq + amp + shape, 100Hz",      proc, false, false, false, 100.0f,  amp, blocks);
        run ("freq + amp + shape, 5kHz",       proc, false, false, false, 5000.0f, amp, blocks);
        std::printf ("\n");
    }
    return 0;
}
