#ifndef _ROTATING_SCRIPT
#define _ROTATING_SCRIPT

#include "GlacierAPI.h"

class RotatingScript : public Script
{
public:
	RotatingScript(const glm::vec3& rot_direction = glm::vec3(0.0f, 1.0f, 0.0f));
	~RotatingScript() = default;

	virtual void OnCreate() override;
	virtual void OnTick() override;

private:
	TransformComponent* transform = nullptr;
	glm::vec3 rot_direction;
};

#endif _ROTATING_SCRIPT