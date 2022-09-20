#ifndef _ROTATING_SCRIPT
#define _ROTATING_SCRIPT

#include "GlacierAPI.h"

class RotatingScript : public Script
{
public:
	RotatingScript() = default;
	~RotatingScript() = default;

	virtual void OnCreate() override;
	virtual void OnTick() override;

private:
	glm::mat4* transform = nullptr;
};

#endif _ROTATING_SCRIPT