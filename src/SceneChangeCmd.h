#ifndef _SCENE_CHANGE_CMD
#define _SCENE_CHANGE_CMD

#include "Command.h"

class Scene;

class SceneChangeCmd : public Command
{
public:
	SceneChangeCmd() = default;
	SceneChangeCmd(const SceneChangeCmd&) = delete;
	SceneChangeCmd& operator=(const SceneChangeCmd&) = delete;
	~SceneChangeCmd() = default;

	void SetScene(Scene* const new_scene);

	virtual void Execute() override;

private:
	Scene* scn = nullptr;
};

#endif _SCENE_CHANGE_CMD
