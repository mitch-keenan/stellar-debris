#pragma once
#include <cmath>
#include <algorithm>

namespace stellar {

class AREnvelope {
public:
	AREnvelope() = default;

	void setSampleRate(float sr) {
		sampleRate = sr;
	}

	void setAttack(float seconds) {
		attackTime = std::max(0.001f, seconds);
	}

	void setRelease(float seconds) {
		releaseTime = std::max(0.001f, seconds);
	}

	float process(bool gate) {
		if (gate) {
			if (stage == STAGE_IDLE || stage == STAGE_RELEASE) {
				stage = STAGE_ATTACK;
			}
		} else {
			if (stage == STAGE_ATTACK || stage == STAGE_SUSTAIN) {
				stage = STAGE_RELEASE;
			}
		}

		switch (stage) {
			case STAGE_ATTACK: {
				float rate = 1.f / (attackTime * sampleRate);
				env += rate;
				if (env >= 1.f) {
					env = 1.f;
					stage = STAGE_SUSTAIN;
				}
				break;
			}
			case STAGE_SUSTAIN:
				env = 1.f;
				break;
			case STAGE_RELEASE: {
				float rate = 1.f / (releaseTime * sampleRate);
				env -= rate;
				if (env <= 0.f) {
					env = 0.f;
					stage = STAGE_IDLE;
				}
				break;
			}
			case STAGE_IDLE:
			default:
				env = 0.f;
				break;
		}

		return env;
	}

	void reset() {
		env = 0.f;
		stage = STAGE_IDLE;
	}

	float getOutput() const { return env; }

private:
	float sampleRate = 44100.f;
	float attackTime = 0.01f;
	float releaseTime = 0.5f;
	float env = 0.f;

	enum Stage {
		STAGE_IDLE,
		STAGE_ATTACK,
		STAGE_SUSTAIN,
		STAGE_RELEASE
	};
	Stage stage = STAGE_IDLE;
};

} // namespace stellar
