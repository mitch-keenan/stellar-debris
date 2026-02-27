#include "dsp/ADSREnvelope.hpp"
#include <cmath>
#include <cstdio>

extern void test_report(const char* name, bool passed);

static bool is_finite(float x) {
	return !std::isnan(x) && !std::isinf(x);
}

void run_adsr_envelope_tests() {
	// Test: idle outputs zero
	{
		stellar::ADSREnvelope env;
		env.setSampleRate(44100.f);
		float out = env.process(false);
		test_report("idle outputs zero", out == 0.f);
	}

	// Test: attack ramps to 1.0
	{
		stellar::ADSREnvelope env;
		env.setSampleRate(44100.f);
		env.setAttack(0.01f);
		env.setDecay(0.1f);
		env.setSustain(0.5f);
		env.setRelease(0.5f);

		float last = 0.f;
		bool ramps_up = true;
		// Gate on for attack phase
		for (int i = 0; i < 441; i++) {
			float out = env.process(true);
			if (out < last - 0.0001f) {
				ramps_up = false;
				break;
			}
			last = out;
		}
		test_report("attack ramps up", ramps_up && last > 0.5f);
	}

	// Test: decay to sustain level
	{
		stellar::ADSREnvelope env;
		env.setSampleRate(44100.f);
		env.setAttack(0.005f);
		env.setDecay(0.05f);
		env.setSustain(0.4f);
		env.setRelease(0.5f);

		// Complete attack + decay
		for (int i = 0; i < 5000; i++) {
			env.process(true);
		}

		float out = env.process(true);
		test_report("decays to sustain level", std::fabs(out - 0.4f) < 0.05f);
	}

	// Test: release from sustain to zero
	{
		stellar::ADSREnvelope env;
		env.setSampleRate(44100.f);
		env.setAttack(0.005f);
		env.setDecay(0.05f);
		env.setSustain(0.5f);
		env.setRelease(0.1f);

		// Complete ADSR to sustain
		for (int i = 0; i < 5000; i++) {
			env.process(true);
		}

		// Release
		float last = env.process(false);
		bool ramps_down = true;
		for (int i = 0; i < 5000; i++) {
			float out = env.process(false);
			if (out > last + 0.0001f) {
				ramps_down = false;
				break;
			}
			last = out;
		}
		test_report("release ramps down", ramps_down && last < 0.1f);
	}

	// Test: no NaN/inf
	{
		stellar::ADSREnvelope env;
		env.setSampleRate(44100.f);
		env.setAttack(0.001f);
		env.setDecay(0.001f);
		env.setSustain(0.5f);
		env.setRelease(0.001f);

		bool all_finite = true;
		for (int i = 0; i < 20000; i++) {
			bool gate = (i % 5000) < 2500;
			float out = env.process(gate);
			if (!is_finite(out)) {
				all_finite = false;
				break;
			}
		}
		test_report("no NaN/inf in output", all_finite);
	}

	// Test: output stays in [0, 1]
	{
		stellar::ADSREnvelope env;
		env.setSampleRate(44100.f);
		env.setAttack(0.005f);
		env.setDecay(0.05f);
		env.setSustain(0.7f);
		env.setRelease(0.05f);

		bool in_range = true;
		for (int i = 0; i < 20000; i++) {
			bool gate = (i % 5000) < 2500;
			float out = env.process(gate);
			if (out < -0.001f || out > 1.001f) {
				in_range = false;
				break;
			}
		}
		test_report("output in [0, 1] range", in_range);
	}
}
