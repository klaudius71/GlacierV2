#ifndef _PREFAB
#define _PREFAB

class Scene;

struct Prefab
{
	virtual GameObject ApplyPrefab(Scene& scn) = 0;
};

#endif _PREFAB