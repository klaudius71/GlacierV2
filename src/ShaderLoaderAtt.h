#ifndef _SHADER_LOADER_ATT
#define _SHADER_LOADER_ATT

#include "ShaderLoader.h"

class ShaderLoaderAtt
{
	friend class Glacier;

	static void Initialize() { ShaderLoader::Instance(); }
	static void Terminate() { ShaderLoader::Terminate(); }
};

#endif _SHADER_LOADER_ATT