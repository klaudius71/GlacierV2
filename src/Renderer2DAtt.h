#ifndef _RENDERER_2D_ATT
#define _RENDERER_2D_ATT

#include "Renderer2D.h"
#include "Application.h"

class Renderer2DAtt
{
	friend class Glacier::Application;
	friend class SceneManager;
	friend class Window;
	friend class EditorLayer;

	static void Initialize() { Renderer2D::Initialize(); }
	static void RenderComponents(Scene& scn) { Renderer2D::RenderComponents(scn); }
	static void UpdateViewportSize(const int& width, const int& height) { Renderer2D::UpdateViewportSize(width, height); }
	static void Terminate() { Renderer2D::Terminate(); }
};

#endif _RENDERER_2D_ATT
