#ifndef _MODEL_LOADER_ATT
#define _MODEL_LOADER_ATT

#include "ModelLoader.h"

class ModelLoaderAtt
{
	friend class Glacier;

	static void WaitForThreadsAndLoadGPUData() { ModelLoader::WaitForThreadsAndLoadGPUData(); }
	static void Terminate() { ModelLoader::Terminate(); }
};

#endif _MODEL_LOADER_ATT