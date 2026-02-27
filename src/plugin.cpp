#include "plugin.hpp"

Plugin* pluginInstance;

void init(Plugin* p) {
	pluginInstance = p;

	p->addModel(modelDroneVoice);
	p->addModel(modelFMVoice);
	p->addModel(modelSubtractiveVoice);
	p->addModel(modelDualFilter);
}
