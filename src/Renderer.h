#ifndef _RENDERER
#define _RENDERER

#include "Framebuffer.h"

class Scene;
struct CameraComponent;

class Renderer
{
private:
	static Renderer* instance;
	Renderer();
	~Renderer() = default;

	Framebuffer framebuffer;

	static void RenderScene(Scene& scn);

	static void UpdateCameraData(const CameraComponent& camera);

	static void RenderUnlit(Scene& scn);
	static void RenderLit(Scene& scn);
	static void RenderSkybox(Scene& scn);

	friend class RendererAtt;
};

#endif _RENDERER