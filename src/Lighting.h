#ifndef _LIGHTING
#define _LIGHTING

class Scene;

class Lighting
{
	static void SetBuffers(const GLuint& DirLight_ubo);
	static void UpdateBuffers(const Scene& curr_scene);

	static GLuint DirLight_ubo;

	friend class SceneManager;
	friend class ShaderLoader;
};

#endif _LIGHTING