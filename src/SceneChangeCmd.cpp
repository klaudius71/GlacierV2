#include "gpch.h"
#include "SceneChangeCmd.h"
#include "SceneManagerAtt.h"

void SceneChangeCmd::SetScene(Scene* const new_scene)
{
	scn = new_scene;
}

void SceneChangeCmd::Execute()
{
	SceneManagerAtt::SceneChange::ChangeScene(scn);
}
