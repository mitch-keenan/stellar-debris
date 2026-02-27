#pragma once
#include <cmath>
#include <algorithm>

namespace stellar {

class PolivoksFilter {
public:
	enum Mode {
		LOWPASS,
		BANDPASS
	};

	PolivoksFilter() = default;

	void setSampleRate(float sr) {
		sampleRate = sr;
	}

	void setCutoff(float hz) {
		cutoff = std::max(20.f, std::min(18000.f, hz));
	}

	void setResonance(float r) {
		// 0..1 range, where 1 approaches self-oscillation
		resonance = std::max(0.f, std::min(1.f, r));
	}

	void setMode(Mode m) {
		mode = m;
	}

	float process(float input) {
		// 12dB/oct SVF (state variable filter)
		// Polivoks character: soft clipping on state variables
		float f = 2.f * sinf(M_PI * cutoff / sampleRate);
		float damp = 2.f * (1.f - resonance * 0.95f);

		float hp = input - lp - damp * bp;
		bp += f * hp;
		lp += f * bp;

		// Polivoks-style soft saturation on state variables
		bp = tanhClip(bp * 0.5f) * 2.f;
		lp = tanhClip(lp * 0.5f) * 2.f;

		switch (mode) {
			case LOWPASS:
				return lp;
			case BANDPASS:
				return bp;
			default:
				return lp;
		}
	}

	void reset() {
		lp = 0.f;
		bp = 0.f;
	}

private:
	static float tanhClip(float x) {
		// Fast tanh approximation for saturation
		if (x > 3.f) return 1.f;
		if (x < -3.f) return -1.f;
		float x2 = x * x;
		return x * (27.f + x2) / (27.f + 9.f * x2);
	}

	float sampleRate = 44100.f;
	float cutoff = 1000.f;
	float resonance = 0.f;
	float lp = 0.f;
	float bp = 0.f;
	Mode mode = LOWPASS;
};

} // namespace stellar
