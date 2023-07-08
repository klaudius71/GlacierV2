#ifndef _PREFAB
#define _PREFAB

#include "GlacierCore.h"

class Scene;

struct GLACIER_API Prefab
{
	virtual GameObject ApplyPrefab(Scene& scn) = 0;
};

#endif _PREFAB