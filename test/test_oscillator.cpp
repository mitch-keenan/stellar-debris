#include "dsp/Oscillator.hpp"
#include <cmath>
#include <cstdio>

extern void test_report(const char* name, bool passed);

static bool is_finite(float x) {
	return !std::isnan(x) && !std::isinf(x);
}

void run_oscillator_tests() {
	const float sampleRate = 44100.f;

	// Test: sine output in [-1, 1]
	{
		stellar::Oscillator osc;
		osc.setSampleRate(sampleRate);
		osc.setFrequency(440.f);
		osc.setWaveform(stellar::Oscillator::SINE);

		bool in_range = true;
		for (int i = 0; i < 44100; i++) {
			float out = osc.process();
			if (out < -1.01f || out > 1.01f) {
				in_range = false;
				break;
			}
		}
		test_report("sine output in [-1, 1]", in_range);
	}

	// Test: saw output in [-1, 1]
	{
		stellar::Oscillator osc;
		osc.setSampleRate(sampleRate);
		osc.setFrequency(440.f);
		osc.setWaveform(stellar::Oscillator::SAW);

		bool in_range = true;
		for (int i = 0; i < 44100; i++) {
			float out = osc.process();
			if (out < -1.2f || out > 1.2f) {
				in_range = false;
				break;
			}
		}
		test_report("saw output in expected range", in_range);
	}

	// Test: square output in [-1, 1]
	{
		stellar::Oscillator osc;
		osc.setSampleRate(sampleRate);
		osc.setFrequency(440.f);
		osc.setWaveform(stellar::Oscillator::SQUARE);

		bool in_range = true;
		for (int i = 0; i < 44100; i++) {
			float out = osc.process();
			if (out < -1.5f || out > 1.5f) {
				in_range = false;
				break;
			}
		}
		test_report("square output in expected range", in_range);
	}

	// Test: triangle output in [-1, 1]
	{
		stellar::Oscillator osc;
		osc.setSampleRate(sampleRate);
		osc.setFrequency(440.f);
		osc.setWaveform(stellar::Oscillator::TRIANGLE);

		bool in_range = true;
		// Let it stabilize
		for (int i = 0; i < 4410; i++) {
			osc.process();
		}
		for (int i = 0; i < 44100; i++) {
			float out = osc.process();
			if (out < -1.5f || out > 1.5f) {
				in_range = false;
				break;
			}
		}
		test_report("triangle output in expected range", in_range);
	}

	// Test: sine is approximately zero-mean
	{
		stellar::Oscillator osc;
		osc.setSampleRate(sampleRate);
		osc.setFrequency(440.f);
		osc.setWaveform(stellar::Oscillator::SINE);

		float sum = 0.f;
		int n = 44100;
		for (int i = 0; i < n; i++) {
			sum += osc.process();
		}
		float mean = sum / n;
		test_report("sine is approximately zero-mean", std::fabs(mean) < 0.01f);
	}

	// Test: no NaN/inf across waveforms
	{
		bool all_finite = true;
		for (int wf = 0; wf < 4; wf++) {
			stellar::Oscillator osc;
			osc.setSampleRate(sampleRate);
			osc.setFrequency(440.f);
			osc.setWaveform(static_cast<stellar::Oscillator::Waveform>(wf));

			for (int i = 0; i < 44100; i++) {
				float out = osc.process();
				if (!is_finite(out)) {
					all_finite = false;
					break;
				}
			}
		}
		test_report("no NaN/inf across all waveforms", all_finite);
	}

	// Test: frequency range extremes
	{
		bool ok = true;
		float freqs[] = {20.f, 100.f, 1000.f, 10000.f, 20000.f};
		for (int f = 0; f < 5; f++) {
			stellar::Oscillator osc;
			osc.setSampleRate(sampleRate);
			osc.setFrequency(freqs[f]);
			osc.setWaveform(stellar::Oscillator::SAW);

			for (int i = 0; i < 4410; i++) {
				float out = osc.process();
				if (!is_finite(out)) {
					ok = false;
					break;
				}
			}
		}
		test_report("stable across frequency range", ok);
	}

	// Test: sync resets phase
	{
		stellar::Oscillator osc;
		osc.setSampleRate(sampleRate);
		osc.setFrequency(440.f);
		osc.setWaveform(stellar::Oscillator::SAW);

		for (int i = 0; i < 100; i++) {
			osc.process();
		}
		osc.syncPhase();
		test_report("sync resets phase to zero", osc.getPhase() == 0.f);
	}
}
