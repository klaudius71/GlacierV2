#include "gpch.h"
#include "GameObjectRegCmd.h"
#include "GameObjectAtt.h"
#include "SceneManager.h"

GameObjectRegCmd::GameObjectRegCmd(GameObject* const obj)
	: obj(obj)
{
}

void GameObjectRegCmd::Execute()
{
	GameObjectAtt::register_to_scene(obj);
}
