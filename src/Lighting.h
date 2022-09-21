#ifndef _LIGHTING
#define _LIGHTING

class Scene;
struct DirectionalLightComponent;

class Lighting
{
	static void SetBuffers(const GLuint& DirLight_ubo);
	static void UpdateBuffers(const Scene& curr_scene);

	static GLuint DirLight_ubo;

	static const DirectionalLightComponent default_dir_light;

	friend class SceneManager;
	friend class ShaderLoader;
};

#endif _LIGHTING