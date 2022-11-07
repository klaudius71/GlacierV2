#ifndef _MODEL_ATT
#define _MODEL_ATT

#include "Model.h"

class ModelAtt
{
	friend class ModelLoader;

	static void LoadGPUData(Model& mod) { mod.load_gpu_data(); }
};

#endif _MODEL_ATT