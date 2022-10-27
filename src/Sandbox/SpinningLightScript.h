#include "GlacierAPI.h"

class SpinningLightScript : public Script
{
public:
	SpinningLightScript();
	~SpinningLightScript() = default;

	virtual void OnCreate() override;
	virtual void OnTick() override;

private:
	glm::vec3* dir = nullptr;
};