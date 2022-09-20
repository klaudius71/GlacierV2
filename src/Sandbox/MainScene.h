#ifndef _MAIN_SCENE
#define _MAIN_SCENE

#include "GlacierAPI.h"

class MainScene : public Scene
{
public:
	MainScene() = default;
	~MainScene() = default;

	virtual void InitializeScene() override;
	virtual void EndScene() override;

private:
	GameObject* level_script = nullptr;
	GameObject* camera = nullptr;
	GameObject* dir_light = nullptr;
	GameObject* terrain = nullptr;
	std::vector<GameObject*> cubes;
};

#endif _MAIN_SCENE