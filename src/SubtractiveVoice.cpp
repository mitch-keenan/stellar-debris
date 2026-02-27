#include "plugin.hpp"

struct SubtractiveVoice : Module {
	enum ParamId {
		// VCO A
		TUNE_COARSE_A_PARAM,
		TUNE_FINE_A_PARAM,
		FM_AMOUNT_A_PARAM,
		PWM_AMOUNT_A_PARAM,
		WAVE_SHAPE_A_PARAM,
		// VCO B
		TUNE_COARSE_B_PARAM,
		TUNE_FINE_B_PARAM,
		FM_AMOUNT_B_PARAM,
		PWM_AMOUNT_B_PARAM,
		WAVE_SHAPE_B_PARAM,
		// Shared
		MIX_AB_PARAM,
		ATTACK_PARAM,
		DECAY_PARAM,
		SUSTAIN_PARAM,
		RELEASE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		VOCT_A_INPUT,
		VOCT_B_INPUT,
		FM_A_INPUT,
		FM_B_INPUT,
		PWM_A_INPUT,
		PWM_B_INPUT,
		SYNC_A_INPUT,
		GATE_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		VCO_A_SIN_OUTPUT,
		VCO_A_SAW_OUTPUT,
		VCO_A_SQR_OUTPUT,
		VCO_A_TRI_OUTPUT,
		VCO_B_SIN_OUTPUT,
		VCO_B_SAW_OUTPUT,
		VCO_B_SQR_OUTPUT,
		VCO_B_TRI_OUTPUT,
		OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	bool normalizeVoctAtoB = true;

	SubtractiveVoice() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		// VCO A
		configParam(TUNE_COARSE_A_PARAM, -4.f, 4.f, 0.f, "VCO A Tune (Coarse)", " oct");
		configParam(TUNE_FINE_A_PARAM, -0.5f, 0.5f, 0.f, "VCO A Tune (Fine)", " semitones", 0.f, 100.f);
		configParam(FM_AMOUNT_A_PARAM, 0.f, 1.f, 0.f, "VCO A FM Amount", "%", 0.f, 100.f);
		configParam(PWM_AMOUNT_A_PARAM, 0.f, 1.f, 0.f, "VCO A PWM Amount", "%", 0.f, 100.f);
		configParam(WAVE_SHAPE_A_PARAM, 0.f, 1.f, 0.f, "VCO A Wave Shape", "%", 0.f, 100.f);

		// VCO B
		configParam(TUNE_COARSE_B_PARAM, -4.f, 4.f, 0.f, "VCO B Tune (Coarse)", " oct");
		configParam(TUNE_FINE_B_PARAM, -0.5f, 0.5f, 0.f, "VCO B Tune (Fine)", " semitones", 0.f, 100.f);
		configParam(FM_AMOUNT_B_PARAM, 0.f, 1.f, 0.f, "VCO B FM Amount", "%", 0.f, 100.f);
		configParam(PWM_AMOUNT_B_PARAM, 0.f, 1.f, 0.f, "VCO B PWM Amount", "%", 0.f, 100.f);
		configParam(WAVE_SHAPE_B_PARAM, 0.f, 1.f, 0.f, "VCO B Wave Shape", "%", 0.f, 100.f);

		// Shared
		configParam(MIX_AB_PARAM, 0.f, 1.f, 0.5f, "Mix A/B", "%", 0.f, 100.f);
		configParam(ATTACK_PARAM, 0.005f, 5.f, 0.01f, "Attack", " s");
		configParam(DECAY_PARAM, 0.005f, 5.f, 0.3f, "Decay", " s");
		configParam(SUSTAIN_PARAM, 0.f, 1.f, 0.5f, "Sustain", "%", 0.f, 100.f);
		configParam(RELEASE_PARAM, 0.005f, 10.f, 0.5f, "Release", " s");

		// Inputs
		configInput(VOCT_A_INPUT, "V/Oct A");
		configInput(VOCT_B_INPUT, "V/Oct B");
		configInput(FM_A_INPUT, "FM A");
		configInput(FM_B_INPUT, "FM B");
		configInput(PWM_A_INPUT, "PWM A");
		configInput(PWM_B_INPUT, "PWM B");
		configInput(SYNC_A_INPUT, "Sync A");
		configInput(GATE_INPUT, "Gate");

		// Outputs
		configOutput(VCO_A_SIN_OUTPUT, "VCO A Sine");
		configOutput(VCO_A_SAW_OUTPUT, "VCO A Saw");
		configOutput(VCO_A_SQR_OUTPUT, "VCO A Square");
		configOutput(VCO_A_TRI_OUTPUT, "VCO A Triangle");
		configOutput(VCO_B_SIN_OUTPUT, "VCO B Sine");
		configOutput(VCO_B_SAW_OUTPUT, "VCO B Saw");
		configOutput(VCO_B_SQR_OUTPUT, "VCO B Square");
		configOutput(VCO_B_TRI_OUTPUT, "VCO B Triangle");
		configOutput(OUT_OUTPUT, "Audio");
	}

	json_t* dataToJson() override {
		json_t* rootJ = json_object();
		json_object_set_new(rootJ, "normalizeVoctAtoB", json_boolean(normalizeVoctAtoB));
		return rootJ;
	}

	void dataFromJson(json_t* rootJ) override {
		json_t* normJ = json_object_get(rootJ, "normalizeVoctAtoB");
		if (normJ)
			normalizeVoctAtoB = json_boolean_value(normJ);
	}

	void process(const ProcessArgs& args) override {
		// TODO: Implement DSP
	}
};

struct SubtractiveVoiceWidget : ModuleWidget {
	SubtractiveVoiceWidget(SubtractiveVoice* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/SubtractiveVoice.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		float colA = 25.f;
		float colB = 95.f;
		float y = 35.f;
		float ySpacing = 24.f;

		// VCO A column
		addParam(createParamCentered<RoundBlackKnob>(Vec(colA, y), module, SubtractiveVoice::TUNE_COARSE_A_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(colA + 25.f, y), module, SubtractiveVoice::TUNE_FINE_A_PARAM));
		y += ySpacing;
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(colA, y), module, SubtractiveVoice::FM_AMOUNT_A_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(colA + 25.f, y), module, SubtractiveVoice::PWM_AMOUNT_A_PARAM));
		y += ySpacing;
		addParam(createParamCentered<RoundBlackKnob>(Vec(colA, y), module, SubtractiveVoice::WAVE_SHAPE_A_PARAM));

		// VCO B column
		y = 35.f;
		addParam(createParamCentered<RoundBlackKnob>(Vec(colB, y), module, SubtractiveVoice::TUNE_COARSE_B_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(colB + 25.f, y), module, SubtractiveVoice::TUNE_FINE_B_PARAM));
		y += ySpacing;
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(colB, y), module, SubtractiveVoice::FM_AMOUNT_B_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(colB + 25.f, y), module, SubtractiveVoice::PWM_AMOUNT_B_PARAM));
		y += ySpacing;
		addParam(createParamCentered<RoundBlackKnob>(Vec(colB, y), module, SubtractiveVoice::WAVE_SHAPE_B_PARAM));

		// Shared controls
		y += ySpacing + 10.f;
		float xMid = 60.f;
		addParam(createParamCentered<RoundBlackKnob>(Vec(xMid, y), module, SubtractiveVoice::MIX_AB_PARAM));
		y += ySpacing;
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(25.f, y), module, SubtractiveVoice::ATTACK_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(50.f, y), module, SubtractiveVoice::DECAY_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(75.f, y), module, SubtractiveVoice::SUSTAIN_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(Vec(100.f, y), module, SubtractiveVoice::RELEASE_PARAM));

		// Inputs
		y += ySpacing + 10.f;
		addInput(createInputCentered<PJ301MPort>(Vec(15.f, y), module, SubtractiveVoice::VOCT_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(35.f, y), module, SubtractiveVoice::FM_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(55.f, y), module, SubtractiveVoice::PWM_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(75.f, y), module, SubtractiveVoice::SYNC_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(95.f, y), module, SubtractiveVoice::GATE_INPUT));

		y += ySpacing;
		addInput(createInputCentered<PJ301MPort>(Vec(15.f, y), module, SubtractiveVoice::VOCT_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(35.f, y), module, SubtractiveVoice::FM_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(55.f, y), module, SubtractiveVoice::PWM_B_INPUT));

		// Outputs
		y += ySpacing + 5.f;
		addOutput(createOutputCentered<PJ301MPort>(Vec(15.f, y), module, SubtractiveVoice::VCO_A_SIN_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(Vec(35.f, y), module, SubtractiveVoice::VCO_A_SAW_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(Vec(55.f, y), module, SubtractiveVoice::VCO_A_SQR_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(Vec(75.f, y), module, SubtractiveVoice::VCO_A_TRI_OUTPUT));

		y += ySpacing;
		addOutput(createOutputCentered<PJ301MPort>(Vec(15.f, y), module, SubtractiveVoice::VCO_B_SIN_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(Vec(35.f, y), module, SubtractiveVoice::VCO_B_SAW_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(Vec(55.f, y), module, SubtractiveVoice::VCO_B_SQR_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(Vec(75.f, y), module, SubtractiveVoice::VCO_B_TRI_OUTPUT));

		addOutput(createOutputCentered<PJ301MPort>(Vec(105.f, y), module, SubtractiveVoice::OUT_OUTPUT));
	}

	void appendContextMenu(Menu* menu) override {
		SubtractiveVoice* module = dynamic_cast<SubtractiveVoice*>(this->module);
		if (!module) return;

		menu->addChild(new MenuSeparator);
		menu->addChild(createBoolPtrMenuItem("Normalize V/Oct A to B", "", &module->normalizeVoctAtoB));
	}
};

Model* modelSubtractiveVoice = createModel<SubtractiveVoice, SubtractiveVoiceWidget>("SubtractiveVoice");
