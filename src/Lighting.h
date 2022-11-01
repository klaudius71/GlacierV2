#ifndef _LIGHTING
#define _LIGHTING

#define DIR_SHADOW_MAP_SIZE 2048

class Scene;
struct DirectionalLightComponent;
struct CameraComponent;

class Lighting
{
	static void SetBuffers(const GLuint& DirLight_ubo, const GLuint& lightspace_ubo);
	static void UpdateBuffers(const Scene& curr_scene);

	static void RenderSceneShadows(Scene* const curr_scene, const CameraComponent& cam);

	static GLuint DirLight_ubo;
	static GLuint LightspaceMatrices_ubo;

	static GLuint DirShadow_fbo;
	static GLuint DirShadow_tex;

	static const DirectionalLightComponent default_dir_light;

	friend class SceneManager;
	friend class ShaderLoader;
	friend class Renderer;
};

#endif _LIGHTING