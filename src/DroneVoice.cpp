#include "plugin.hpp"

struct DroneVoice : Module {
	enum ParamId {
		OSC1_TUNE_COARSE_PARAM,
		OSC2_TUNE_COARSE_PARAM,
		OSC3_TUNE_COARSE_PARAM,
		OSC4_TUNE_COARSE_PARAM,
		OSC5_TUNE_COARSE_PARAM,
		OSC1_TUNE_FINE_PARAM,
		OSC2_TUNE_FINE_PARAM,
		OSC3_TUNE_FINE_PARAM,
		OSC4_TUNE_FINE_PARAM,
		OSC5_TUNE_FINE_PARAM,
		OSC1_MOD_AMOUNT_PARAM,
		OSC2_MOD_AMOUNT_PARAM,
		OSC3_MOD_AMOUNT_PARAM,
		OSC4_MOD_AMOUNT_PARAM,
		OSC5_MOD_AMOUNT_PARAM,
		VOLT_PARAM,
		ATTACK_PARAM,
		RELEASE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		GATE_INPUT,
		PITCH_OFFSET_INPUT,
		MOD_CV_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	DroneVoice() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		for (int i = 0; i < 5; i++) {
			std::string label = "Osc " + std::to_string(i + 1);
			configParam(OSC1_TUNE_COARSE_PARAM + i, -2.f, 2.f, 0.f, label + " Tune (Coarse)", " oct");
			configParam(OSC1_TUNE_FINE_PARAM + i, -0.5f, 0.5f, 0.f, label + " Tune (Fine)", " semitones", 0.f, 100.f);
			configParam(OSC1_MOD_AMOUNT_PARAM + i, 0.f, 1.f, 0.f, label + " Mod Amount", "%", 0.f, 100.f);
		}

		configParam(VOLT_PARAM, 0.f, 1.f, 0.f, "Volt", "%", 0.f, 100.f);
		configParam(ATTACK_PARAM, 0.005f, 5.f, 0.01f, "Attack", " s");
		configParam(RELEASE_PARAM, 0.005f, 10.f, 0.5f, "Release", " s");

		configInput(GATE_INPUT, "Gate");
		configInput(PITCH_OFFSET_INPUT, "Pitch Offset CV");
		configInput(MOD_CV_INPUT, "Mod CV");

		configOutput(OUT_OUTPUT, "Audio");
	}

	void process(const ProcessArgs& args) override {
		// TODO: Implement DSP
	}
};

struct DroneVoiceWidget : ModuleWidget {
	DroneVoiceWidget(DroneVoice* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/DroneVoice.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		// Params — positions are placeholders for panel layout
		float yStart = 30.f;
		float xStart = 15.f;
		float xSpacing = 18.f;
		float ySpacing = 28.f;

		for (int i = 0; i < 5; i++) {
			float x = xStart + i * xSpacing;
			addParam(createParamCentered<RoundBlackKnob>(Vec(x, yStart), module, DroneVoice::OSC1_TUNE_COARSE_PARAM + i));
			addParam(createParamCentered<RoundSmallBlackKnob>(Vec(x, yStart + ySpacing), module, DroneVoice::OSC1_TUNE_FINE_PARAM + i));
			addParam(createParamCentered<RoundSmallBlackKnob>(Vec(x, yStart + 2 * ySpacing), module, DroneVoice::OSC1_MOD_AMOUNT_PARAM + i));
		}

		float yBottom = yStart + 3 * ySpacing;
		addParam(createParamCentered<RoundBlackKnob>(Vec(xStart, yBottom), module, DroneVoice::VOLT_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(Vec(xStart + xSpacing, yBottom), module, DroneVoice::ATTACK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(Vec(xStart + 2 * xSpacing, yBottom), module, DroneVoice::RELEASE_PARAM));

		// Inputs
		float yIO = yBottom + ySpacing + 10.f;
		addInput(createInputCentered<PJ301MPort>(Vec(xStart, yIO), module, DroneVoice::GATE_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(xStart + xSpacing, yIO), module, DroneVoice::PITCH_OFFSET_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(xStart + 2 * xSpacing, yIO), module, DroneVoice::MOD_CV_INPUT));

		// Output
		addOutput(createOutputCentered<PJ301MPort>(Vec(xStart + 4 * xSpacing, yIO), module, DroneVoice::OUT_OUTPUT));
	}
};

Model* modelDroneVoice = createModel<DroneVoice, DroneVoiceWidget>("DroneVoice");
