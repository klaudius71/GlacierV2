#ifndef _RENDERER_ATT
#define _RENDERER_ATT

#include "Renderer.h"
#include "Application.h"

class RendererAtt
{
	friend class Glacier::Application;
	friend class SceneManager;
	friend class Window;
	friend class EditorLayer;

	static void Initialize() { Renderer::Initialize(); }
	static void RenderScene(Scene& scn) { Renderer::RenderScene(scn); }
	static void UpdateViewportSize(const int& width, const int& height) { Renderer::UpdateViewportSize(width, height); }
	static void Terminate() { Renderer::Terminate(); }
};

#endif _RENDERER_ATT