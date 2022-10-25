#ifndef _SCENE_ATT
#define _SCENE_ATT

#include "Scene.h"

class SceneAtt
{
	friend class EditorLayer;

	static SceneGraph& GetSceneGraph(Scene& scn) { return scn.scn_graph; };
};

#endif _SCENE_ATT