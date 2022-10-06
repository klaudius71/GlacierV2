#ifndef _RENDERER_2D_ATT
#define _RENDERER_2D_ATT

#include "Renderer2D.h"

class Renderer2DAtt
{
	friend class Glacier;
	friend class SceneManager;
	friend class Window;

	static void Initialize() { Renderer2D::Initialize(); }
	static void RenderComponents(Scene& scn) { Renderer2D::RenderComponents(scn); }
	static void UpdateScreenSize(const int& width, const int& height) { Renderer2D::UpdateScreenSize(width, height); }
	static void Terminate() { Renderer2D::Terminate(); }
};

#endif _RENDERER_2D_ATT
