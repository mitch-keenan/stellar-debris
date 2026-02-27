#pragma once
#include <cmath>
#include <algorithm>

namespace stellar {

class ADSREnvelope {
public:
	ADSREnvelope() = default;

	void setSampleRate(float sr) {
		sampleRate = sr;
	}

	void setAttack(float seconds) {
		attackTime = std::max(0.001f, seconds);
	}

	void setDecay(float seconds) {
		decayTime = std::max(0.001f, seconds);
	}

	void setSustain(float level) {
		sustainLevel = std::max(0.f, std::min(1.f, level));
	}

	void setRelease(float seconds) {
		releaseTime = std::max(0.001f, seconds);
	}

	float process(bool gate) {
		if (gate && !lastGate) {
			stage = STAGE_ATTACK;
		} else if (!gate && lastGate) {
			if (stage != STAGE_IDLE) {
				stage = STAGE_RELEASE;
			}
		}
		lastGate = gate;

		switch (stage) {
			case STAGE_ATTACK: {
				float rate = 1.f / (attackTime * sampleRate);
				env += rate;
				if (env >= 1.f) {
					env = 1.f;
					stage = STAGE_DECAY;
				}
				break;
			}
			case STAGE_DECAY: {
				float rate = 1.f / (decayTime * sampleRate);
				env -= rate;
				if (env <= sustainLevel) {
					env = sustainLevel;
					stage = STAGE_SUSTAIN;
				}
				break;
			}
			case STAGE_SUSTAIN:
				env = sustainLevel;
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
		lastGate = false;
	}

	float getOutput() const { return env; }

private:
	float sampleRate = 44100.f;
	float attackTime = 0.01f;
	float decayTime = 0.3f;
	float sustainLevel = 0.5f;
	float releaseTime = 0.5f;
	float env = 0.f;
	bool lastGate = false;

	enum Stage {
		STAGE_IDLE,
		STAGE_ATTACK,
		STAGE_DECAY,
		STAGE_SUSTAIN,
		STAGE_RELEASE
	};
	Stage stage = STAGE_IDLE;
};

} // namespace stellar
