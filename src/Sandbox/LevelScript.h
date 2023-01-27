#include "GlacierAPI.h"

class LevelScript : public Script
{
public:
	LevelScript();
	~LevelScript() = default;

	virtual void OnSceneEnter() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnSceneExit() override;

private:
	GameObject cube2;

	int frames;
	float update, physics, draw, three, two, animation, frame_time;
};