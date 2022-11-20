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

	Framebuffer main_framebuffer;

	static void Initialize();
	static void Terminate();

	static void UpdateViewportSize(const int& width, const int& height);
	static void UpdateCameraData(const CameraComponent& camera);

	static void RenderScene(Scene& scn);

	static void RenderLit(Scene& scn);
	static void RenderSkinned(Scene& scn);
	static void RenderUnlit(Scene& scn);
	static void RenderSkybox(Scene& scn);

	friend class RendererAtt;

public:
	static const Framebuffer& GetMainFramebuffer();
};

#endif _RENDERER