#include "plugin.hpp"

struct DualFilter : Module {
	enum ParamId {
		FREQ_1_PARAM,
		RES_1_PARAM,
		FREQ_2_PARAM,
		RES_2_PARAM,
		MODE_PARAM,
		LINK_PARAM,
		DRIVE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		IN_1_INPUT,
		IN_2_INPUT,
		FREQ_CV_1_INPUT,
		FREQ_CV_2_INPUT,
		RES_CV_1_INPUT,
		RES_CV_2_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUT_1_OUTPUT,
		OUT_2_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LINK_LIGHT,
		LIGHTS_LEN
	};

	DualFilter() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		configParam(FREQ_1_PARAM, 0.f, 1.f, 0.5f, "Frequency 1", " Hz", std::pow(2.f, 10.f), 20.f);
		configParam(RES_1_PARAM, 0.f, 1.f, 0.f, "Resonance 1", "%", 0.f, 100.f);
		configParam(FREQ_2_PARAM, -1.f, 1.f, 0.f, "Frequency 2 / Offset", " oct", 0.f, 2.f);
		configParam(RES_2_PARAM, -0.5f, 0.5f, 0.f, "Resonance 2 / Offset", "%", 0.f, 100.f);
		configSwitch(MODE_PARAM, 0.f, 1.f, 0.f, "Mode", {"Lowpass", "Bandpass"});
		configButton(LINK_PARAM, "Link Channels");
		configParam(DRIVE_PARAM, 0.f, 1.f, 0.f, "Drive", "%", 0.f, 100.f);

		configInput(IN_1_INPUT, "Audio 1");
		configInput(IN_2_INPUT, "Audio 2");
		configInput(FREQ_CV_1_INPUT, "Freq CV 1");
		configInput(FREQ_CV_2_INPUT, "Freq CV 2");
		configInput(RES_CV_1_INPUT, "Res CV 1");
		configInput(RES_CV_2_INPUT, "Res CV 2");

		configOutput(OUT_1_OUTPUT, "Audio 1");
		configOutput(OUT_2_OUTPUT, "Audio 2");
	}

	void process(const ProcessArgs& args) override {
		// TODO: Implement DSP
	}
};

struct DualFilterWidget : ModuleWidget {
	DualFilterWidget(DualFilter* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/DualFilter.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		float x1 = 22.f;
		float x2 = 52.f;
		float y = 40.f;
		float ySpacing = 30.f;

		// Channel 1
		addParam(createParamCentered<RoundBlackKnob>(Vec(x1, y), module, DualFilter::FREQ_1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(Vec(x2, y), module, DualFilter::RES_1_PARAM));

		// Channel 2
		y += ySpacing;
		addParam(createParamCentered<RoundBlackKnob>(Vec(x1, y), module, DualFilter::FREQ_2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(Vec(x2, y), module, DualFilter::RES_2_PARAM));

		// Global
		y += ySpacing;
		addParam(createParamCentered<CKSS>(Vec(x1, y), module, DualFilter::MODE_PARAM));
		addParam(createParamCentered<VCVButton>(Vec(x2, y), module, DualFilter::LINK_PARAM));
		addChild(createLightCentered<MediumLight<GreenLight>>(Vec(x2, y - 10.f), module, DualFilter::LINK_LIGHT));

		y += ySpacing;
		addParam(createParamCentered<RoundBlackKnob>(Vec(37.f, y), module, DualFilter::DRIVE_PARAM));

		// Inputs
		y += ySpacing + 10.f;
		addInput(createInputCentered<PJ301MPort>(Vec(x1, y), module, DualFilter::IN_1_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(x2, y), module, DualFilter::IN_2_INPUT));

		y += ySpacing;
		addInput(createInputCentered<PJ301MPort>(Vec(15.f, y), module, DualFilter::FREQ_CV_1_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(35.f, y), module, DualFilter::FREQ_CV_2_INPUT));
		addInput(createInputCentered<PJ301MPort>(Vec(55.f, y), module, DualFilter::RES_CV_1_INPUT));

		y += ySpacing;
		addInput(createInputCentered<PJ301MPort>(Vec(15.f, y), module, DualFilter::RES_CV_2_INPUT));

		// Outputs
		addOutput(createOutputCentered<PJ301MPort>(Vec(35.f, y), module, DualFilter::OUT_1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(Vec(55.f, y), module, DualFilter::OUT_2_OUTPUT));
	}
};

Model* modelDualFilter = createModel<DualFilter, DualFilterWidget>("DualFilter");
