#include "gpch.h"
#include "EntityDeregCmd.h"
#include "EntityAtt.h"
#include "SceneManager.h"

EntityDeregCmd::EntityDeregCmd(Entity* const obj)
	: obj(obj)
{
}

void EntityDeregCmd::Execute()
{
	EntityAtt::deregister_from_scene(obj);
}
