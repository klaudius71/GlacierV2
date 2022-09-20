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
	TransformComponent* transform = nullptr;
};

#endif _ROTATING_SCRIPT