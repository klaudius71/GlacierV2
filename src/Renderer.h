#ifndef _RENDERER
#define _RENDERER

class Scene;
struct CameraComponent;

class Renderer
{
private:
	static void UpdateCameraData(const CameraComponent& camera);

public:
	static void RenderScene(Scene* const scn);
};

#endif _RENDERER