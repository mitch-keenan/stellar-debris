#pragma once
#include <cmath>
#include <algorithm>

namespace stellar {

class Oscillator {
public:
	enum Waveform {
		SINE,
		SAW,
		SQUARE,
		TRIANGLE
	};

	Oscillator() = default;

	void setSampleRate(float sr) {
		sampleRate = sr;
	}

	void setFrequency(float hz) {
		frequency = hz;
	}

	void setPulseWidth(float pw) {
		pulseWidth = std::max(0.01f, std::min(0.99f, pw));
	}

	void setWaveform(Waveform wf) {
		waveform = wf;
	}

	void syncPhase() {
		phase = 0.f;
	}

	float process() {
		float dt = frequency / sampleRate;
		float out = 0.f;

		switch (waveform) {
			case SINE:
				out = sinf(2.f * M_PI * phase);
				break;
			case SAW:
				out = 2.f * phase - 1.f;
				out -= polyBLEP(phase, dt);
				break;
			case SQUARE: {
				out = (phase < pulseWidth) ? 1.f : -1.f;
				out += polyBLEP(phase, dt);
				out -= polyBLEP(fmodf(phase + 1.f - pulseWidth, 1.f), dt);
				break;
			}
			case TRIANGLE: {
				// Integrated square -> triangle via leaky integrator
				float sq = (phase < 0.5f) ? 1.f : -1.f;
				sq += polyBLEP(phase, dt);
				sq -= polyBLEP(fmodf(phase + 0.5f, 1.f), dt);
				// Leaky integration
				triState += 4.f * dt * sq;
				// DC offset correction via slow leak
				triState *= 0.999f;
				out = triState;
				break;
			}
		}

		// Advance phase
		phase += dt;
		if (phase >= 1.f) {
			phase -= 1.f;
		}

		return out;
	}

	float getPhase() const { return phase; }

	// Get output for a specific waveform without advancing phase
	float getSample(Waveform wf) const {
		float dt = frequency / sampleRate;
		switch (wf) {
			case SINE:
				return sinf(2.f * M_PI * phase);
			case SAW: {
				float s = 2.f * phase - 1.f;
				s -= polyBLEP(phase, dt);
				return s;
			}
			case SQUARE: {
				float s = (phase < pulseWidth) ? 1.f : -1.f;
				s += polyBLEP(phase, dt);
				s -= polyBLEP(fmodf(phase + 1.f - pulseWidth, 1.f), dt);
				return s;
			}
			case TRIANGLE:
				return triState;
			default:
				return 0.f;
		}
	}

	void reset() {
		phase = 0.f;
		triState = 0.f;
	}

private:
	static float polyBLEP(float t, float dt) {
		if (dt <= 0.f) return 0.f;
		if (t < dt) {
			t /= dt;
			return t + t - t * t - 1.f;
		} else if (t > 1.f - dt) {
			t = (t - 1.f) / dt;
			return t * t + t + t + 1.f;
		}
		return 0.f;
	}

	float sampleRate = 44100.f;
	float frequency = 440.f;
	float phase = 0.f;
	float pulseWidth = 0.5f;
	float triState = 0.f;
	Waveform waveform = SINE;
};

} // namespace stellar
