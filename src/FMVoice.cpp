#include "plugin.hpp"

struct FMVoice : Module {
	enum ParamId {
		CARRIER_TUNE_COARSE_PARAM,
		CARRIER_TUNE_FINE_PARAM,
		MOD_RATE_COARSE_PARAM,
		MOD_RATE_FINE_PARAM,
		MOD_DEPTH_PARAM,
		MODE_PARAM,
		ATTACK_PARAM,
		RELEASE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		VOCT_INPUT,
		GATE_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	FMVoice() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		configParam(CARRIER_TUNE_COARSE_PARAM, -4.f, 4.f, 0.f, "Carrier Tune (Coarse)", " oct");
		configParam(CARRIER_TUNE_FINE_PARAM, -0.5f, 0.5f, 0.f, "Carrier Tune (Fine)", " semitones", 0.f, 100.f);
		configParam(MOD_RATE_COARSE_PARAM, 0.05f, 5000.f, 100.f, "Modulator Rate (Coarse)", " Hz");
		configParam(MOD_RATE_FINE_PARAM, -1.f, 1.f, 0.f, "Modulator Rate (Fine)", " oct");
		configParam(MOD_DEPTH_PARAM, 0.f, 1.f, 0.f, "Mod Depth", "%", 0.f, 100.f);
		configSwitch(MODE_PARAM, 0.f, 2.f, 1.f, "Mode", {"AM", "FM", "AM+FM"});
		configParam(ATTACK_PARAM, 0.005f, 5.f, 0.01f, "Attack", " s");
		configParam(RELEASE_PARAM, 0.005f, 10.f, 0.5f, "Release", " s");

		configInput(VOCT_INPUT, "V/Oct (Carrier)");
		configInput(GATE_INPUT, "Gate");

		configOutput(OUT_OUTPUT, "Audio");
	}

	void process(const ProcessArgs& args) override {
		// TODO: Implement DSP
	}
};

struct FMVoiceWidget : ModuleWidget {
	FMVoiceWidget(FMVoice* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/FMVoice.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		float x1 = 22.f;
		float x2 = 48.f;
		float y = 40.f;
		float ySpacing = 32.f;

		addParam(createParamCentered<RoundBlackKnob>(Vec(x1, y), module, FMVoice::CARRIER_TUNE_COARSE_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(x2, y), module, FMVoice::CARRIER_TUNE_FINE_PARAM));

		y += ySpacing;
		addParam(createParamCentered<RoundBlackKnob>(Vec(x1, y), module, FMVoice::MOD_RATE_COARSE_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(x2, y), module, FMVoice::MOD_RATE_FINE_PARAM));

		y += ySpacing;
		addParam(createParamCentered<RoundBlackKnob>(Vec(x1, y), module, FMVoice::MOD_DEPTH_PARAM));
		addParam(createParamCentered<CKSSThree>(Vec(x2, y), module, FMVoice::MODE_PARAM));

		y += ySpacing;
		addParam(createParamCentered<RoundBlackKnob>(Vec(x1, y), module, FMVoice::ATTACK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(Vec(x2, y), module, FMVoice::RELEASE_PARAM));

		// Inputs
		y += ySpacing + 10.f;
		addInput(createInputCentered<PJ301MPort>(Vec(x1, y), module, FMVoice::VOCT_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(x2, y), module, FMVoice::GATE_INPUT));

		// Output
		y += ySpacing;
		addOutput(createOutputCentered<PJ301MPort>(Vec(35.f, y), module, FMVoice::OUT_OUTPUT));
	}
};

Model* modelFMVoice = createModel<FMVoice, FMVoiceWidget>("FMVoice");
