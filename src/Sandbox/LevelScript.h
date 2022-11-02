#include "GlacierAPI.h"

class LevelScript : public Script
{
public:
	LevelScript();
	~LevelScript() = default;

	virtual void OnSceneEnter() override;
	virtual void OnUpdate() override;
	virtual void OnSceneExit() override;

private:
	GameObject cube2;
	int frame_time_index;
	std::vector<float> frame_time_data;
};