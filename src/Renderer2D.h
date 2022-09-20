#ifndef _RENDERER_2D
#define _RENDERER_2D

#include "Components.h"

class Renderer2D
{
private:
	static Renderer2D* instance;
	Renderer2D();
	~Renderer2D() = default;

	glm::mat4 proj;
	glm::mat4 view;

	static void Initialize();
	static void Terminate();

	friend class Renderer2DAtt;

public:
	static void RenderComponents(entt::registry& registry);
};

#endif _RENDERER_2D