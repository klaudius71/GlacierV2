#include "gpch.h"
#include "EntityRegCmd.h"
#include "EntityAtt.h"
#include "SceneManager.h"

EntityRegCmd::EntityRegCmd(Entity* const obj)
	: obj(obj)
{
}

void EntityRegCmd::Execute()
{
	EntityAtt::register_to_scene(obj);
}
