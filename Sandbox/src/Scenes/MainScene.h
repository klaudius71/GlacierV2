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
};

#endif _MAIN_SCENE