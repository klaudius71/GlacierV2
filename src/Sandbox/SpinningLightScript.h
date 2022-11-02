#include "GlacierAPI.h"

class SpinningLightScript : public Script
{
public:
	SpinningLightScript();
	~SpinningLightScript() = default;

	virtual void OnSceneEnter() override;
	virtual void OnUpdate() override;

private:
	glm::vec3* dir = nullptr;
};