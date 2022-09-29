#ifndef _LIGHTING
#define _LIGHTING

class Scene;
struct DirectionalLightComponent;
struct CameraComponent;

class Lighting
{
	static void SetBuffers(const GLuint& DirLight_ubo);
	static void UpdateBuffers(const Scene& curr_scene);

	static void RenderSceneShadows(Scene& curr_scene, const CameraComponent& cam);

	static GLuint DirLight_ubo;
	static GLuint DirShadow_fbo;

	static const DirectionalLightComponent default_dir_light;

	friend class SceneManager;
	friend class ShaderLoader;
	friend class Renderer;
};

#endif _LIGHTING