#include "GlacierAPI.h"

class BSphereTestingScript : public Script
{
public:
	BSphereTestingScript(GameObject mesh);
	~BSphereTestingScript() = default;

	//virtual void OnSceneEnter() override;
	virtual void OnUpdate(float dt) override;

private:
	GameObject mesh;
};