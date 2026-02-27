#include "dsp/PolivoksFilter.hpp"
#include <cmath>
#include <cstdio>

extern void test_report(const char* name, bool passed);

static bool is_finite(float x) {
	return !std::isnan(x) && !std::isinf(x);
}

void run_polivoks_filter_tests() {
	const float sampleRate = 44100.f;

	// Test: silence in, silence out
	{
		stellar::PolivoksFilter filt;
		filt.setSampleRate(sampleRate);
		filt.setCutoff(1000.f);
		filt.setResonance(0.f);

		float out = 0.f;
		for (int i = 0; i < 1000; i++) {
			out = filt.process(0.f);
		}
		test_report("silence in gives silence out", std::fabs(out) < 0.001f);
	}

	// Test: lowpass attenuates high frequencies
	{
		stellar::PolivoksFilter filt;
		filt.setSampleRate(sampleRate);
		filt.setCutoff(200.f);
		filt.setResonance(0.f);
		filt.setMode(stellar::PolivoksFilter::LOWPASS);

		// Feed a high-frequency signal (5kHz sine)
		float sumAbs = 0.f;
		int n = 4410;
		for (int i = 0; i < n; i++) {
			float input = sinf(2.f * M_PI * 5000.f * i / sampleRate);
			float out = filt.process(input);
			sumAbs += std::fabs(out);
		}
		float avgLevel = sumAbs / n;
		// Should be significantly attenuated
		test_report("lowpass attenuates high freq", avgLevel < 0.2f);
	}

	// Test: lowpass passes low frequencies
	{
		stellar::PolivoksFilter filt;
		filt.setSampleRate(sampleRate);
		filt.setCutoff(5000.f);
		filt.setResonance(0.f);
		filt.setMode(stellar::PolivoksFilter::LOWPASS);

		// Feed a low-frequency signal (100Hz sine)
		float sumAbs = 0.f;
		int n = 4410;
		// Let filter stabilize
		for (int i = 0; i < 4410; i++) {
			float input = sinf(2.f * M_PI * 100.f * i / sampleRate);
			filt.process(input);
		}
		for (int i = 0; i < n; i++) {
			float input = sinf(2.f * M_PI * 100.f * (i + 4410) / sampleRate);
			float out = filt.process(input);
			sumAbs += std::fabs(out);
		}
		float avgLevel = sumAbs / n;
		// Should pass through with reasonable level
		test_report("lowpass passes low freq", avgLevel > 0.3f);
	}

	// Test: bandpass mode works
	{
		stellar::PolivoksFilter filt;
		filt.setSampleRate(sampleRate);
		filt.setCutoff(1000.f);
		filt.setResonance(0.5f);
		filt.setMode(stellar::PolivoksFilter::BANDPASS);

		// Feed broadband noise-like signal
		float sumAbs = 0.f;
		int n = 4410;
		for (int i = 0; i < n; i++) {
			float input = sinf(2.f * M_PI * 1000.f * i / sampleRate);
			float out = filt.process(input);
			sumAbs += std::fabs(out);
		}
		float avgLevel = sumAbs / n;
		test_report("bandpass produces output at cutoff freq", avgLevel > 0.05f);
	}

	// Test: resonance increases peak
	{
		stellar::PolivoksFilter filtLow, filtHigh;
		filtLow.setSampleRate(sampleRate);
		filtLow.setCutoff(1000.f);
		filtLow.setResonance(0.1f);
		filtLow.setMode(stellar::PolivoksFilter::LOWPASS);

		filtHigh.setSampleRate(sampleRate);
		filtHigh.setCutoff(1000.f);
		filtHigh.setResonance(0.9f);
		filtHigh.setMode(stellar::PolivoksFilter::LOWPASS);

		// Let both filters stabilize
		for (int i = 0; i < 44100; i++) {
			float input = sinf(2.f * M_PI * 1000.f * i / sampleRate);
			filtLow.process(input);
			filtHigh.process(input);
		}

		// Measure steady-state amplitude
		float peakLow = 0.f, peakHigh = 0.f;
		int n = 4410;
		for (int i = 0; i < n; i++) {
			float input = sinf(2.f * M_PI * 1000.f * (i + 44100) / sampleRate);
			peakLow = std::max(peakLow, std::fabs(filtLow.process(input)));
			peakHigh = std::max(peakHigh, std::fabs(filtHigh.process(input)));
		}
		test_report("resonance increases level at cutoff", peakHigh > peakLow);
	}

	// Test: no NaN/inf
	{
		stellar::PolivoksFilter filt;
		filt.setSampleRate(sampleRate);
		filt.setCutoff(1000.f);
		filt.setResonance(0.9f);

		bool all_finite = true;
		for (int i = 0; i < 44100; i++) {
			float input = sinf(2.f * M_PI * 440.f * i / sampleRate);
			float out = filt.process(input);
			if (!is_finite(out)) {
				all_finite = false;
				break;
			}
		}
		test_report("no NaN/inf in output", all_finite);
	}

	// Test: cutoff range extremes
	{
		bool ok = true;
		float cutoffs[] = {20.f, 100.f, 1000.f, 10000.f, 18000.f};
		for (int c = 0; c < 5; c++) {
			stellar::PolivoksFilter filt;
			filt.setSampleRate(sampleRate);
			filt.setCutoff(cutoffs[c]);
			filt.setResonance(0.5f);

			for (int i = 0; i < 4410; i++) {
				float input = sinf(2.f * M_PI * 440.f * i / sampleRate);
				float out = filt.process(input);
				if (!is_finite(out)) {
					ok = false;
					break;
				}
			}
		}
		test_report("stable across cutoff range", ok);
	}
}
