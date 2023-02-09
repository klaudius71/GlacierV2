#ifndef _FPS_CHARACTER_CONTROLLER_SCRIPT
#define _FPS_CHARACTER_CONTROLLER_SCRIPT

#include "GlacierAPI.h"

class FPSCharacterControllerScript : public Script
{
public:
	FPSCharacterControllerScript();
	~FPSCharacterControllerScript() = default;

	virtual void OnUpdate(float dt) override;

private:
	glm::vec3 camRot{ 0.0f, glm::pi<float>(), 0.0f };
};

#endif
