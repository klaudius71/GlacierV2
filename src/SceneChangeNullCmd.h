#ifndef _SCENE_CHANGE_NULL_CMD
#define _SCENE_CHANGE_NULL_CMD

#include "SceneChangeCmd.h"

class SceneChangeNullCmd : public SceneChangeCmd
{
public:
	SceneChangeNullCmd() = default;
	SceneChangeNullCmd(const SceneChangeNullCmd&) = delete;
	SceneChangeNullCmd& operator=(const SceneChangeNullCmd&) = delete;
	~SceneChangeNullCmd() = default;

	virtual void Execute() override;
};

#endif _SCENE_CHANGE_NULL_CMD
