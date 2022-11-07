#ifndef _TEXTURE_ATT
#define _TEXTURE_ATT

#include "Texture.h"

class TextureAtt
{
	friend class TextureLoader;

	static void LoadGPUData(Texture& tex) { tex.load_gpu_data(); }
};

#endif _TEXTURE_ATT