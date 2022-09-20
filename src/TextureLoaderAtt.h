#ifndef _TEXTURE_LOADER_ATT
#define _TEXTURE_LOADER_ATT

#include "TextureLoader.h"

class TextureLoaderAtt
{
	friend class Glacier;

	static void WaitForThreadsAndLoadGPUData() { TextureLoader::WaitForThreadsAndLoadGPUData(); }
	static void Terminate() { TextureLoader::Terminate(); }
};

#endif _TEXTURE_LOADER_ATT