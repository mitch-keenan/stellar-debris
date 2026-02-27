#include "dsp/AREnvelope.hpp"
#include <cmath>
#include <cstdio>

extern void test_report(const char* name, bool passed);

static bool is_finite(float x) {
	return !std::isnan(x) && !std::isinf(x);
}

void run_ar_envelope_tests() {
	// Test: idle state outputs zero
	{
		stellar::AREnvelope env;
		env.setSampleRate(44100.f);
		float out = env.process(false);
		test_report("idle outputs zero", out == 0.f);
	}

	// Test: attack ramps up
	{
		stellar::AREnvelope env;
		env.setSampleRate(44100.f);
		env.setAttack(0.01f); // 10ms
		env.setRelease(0.5f);

		// Process with gate high for attack duration
		float last = 0.f;
		bool ramps_up = true;
		for (int i = 0; i < 441; i++) { // 10ms at 44100
			float out = env.process(true);
			if (out < last - 0.0001f) {
				ramps_up = false;
				break;
			}
			last = out;
		}
		test_report("attack ramps up", ramps_up && last > 0.5f);
	}

	// Test: sustains at peak while gate high
	{
		stellar::AREnvelope env;
		env.setSampleRate(44100.f);
		env.setAttack(0.005f);
		env.setRelease(0.5f);

		// Complete attack
		for (int i = 0; i < 1000; i++) {
			env.process(true);
		}

		// Should sustain at 1.0
		float out = env.process(true);
		test_report("sustains at peak", std::fabs(out - 1.f) < 0.001f);
	}

	// Test: release ramps down
	{
		stellar::AREnvelope env;
		env.setSampleRate(44100.f);
		env.setAttack(0.005f);
		env.setRelease(0.1f);

		// Complete attack
		for (int i = 0; i < 1000; i++) {
			env.process(true);
		}

		// Release
		float last = 1.f;
		bool ramps_down = true;
		for (int i = 0; i < 4410; i++) { // 100ms
			float out = env.process(false);
			if (out > last + 0.0001f) {
				ramps_down = false;
				break;
			}
			last = out;
		}
		test_report("release ramps down", ramps_down && last < 0.5f);
	}

	// Test: no NaN/inf
	{
		stellar::AREnvelope env;
		env.setSampleRate(44100.f);
		env.setAttack(0.001f);
		env.setRelease(0.001f);

		bool all_finite = true;
		for (int i = 0; i < 10000; i++) {
			float out = env.process(i < 2000);
			if (!is_finite(out)) {
				all_finite = false;
				break;
			}
		}
		test_report("no NaN/inf in output", all_finite);
	}

	// Test: output stays in [0, 1]
	{
		stellar::AREnvelope env;
		env.setSampleRate(44100.f);
		env.setAttack(0.005f);
		env.setRelease(0.01f);

		bool in_range = true;
		for (int i = 0; i < 10000; i++) {
			bool gate = (i % 3000) < 1500;
			float out = env.process(gate);
			if (out < -0.001f || out > 1.001f) {
				in_range = false;
				break;
			}
		}
		test_report("output in [0, 1] range", in_range);
	}
}
