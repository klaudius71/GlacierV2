#include "gpch.h"
#include "GameObjectDeregCmd.h"
#include "GameObjectAtt.h"
#include "SceneManager.h"

GameObjectDeregCmd::GameObjectDeregCmd(GameObject* const obj)
	: obj(obj)
{
}

void GameObjectDeregCmd::Execute()
{
	GameObjectAtt::deregister_from_scene(obj);
}
