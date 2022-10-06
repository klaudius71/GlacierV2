#ifndef _RENDERER_ATT
#define _RENDERER_ATT

#include "Renderer.h"

class RendererAtt
{
	friend class SceneManager;

	static void RenderScene(Scene& scn) { Renderer::RenderScene(scn); }
};

#endif _RENDERER_ATT