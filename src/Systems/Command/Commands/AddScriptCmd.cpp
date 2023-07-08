#include "gpch.h"
#include "AddScriptCmd.h"
#include "Entity.h"

AddScriptCmd::AddScriptCmd(Entity* const obj, Script* const script)
	: obj(obj), script(script)
{
}

void AddScriptCmd::Execute()
{
	obj->EmplaceComponent<ScriptComponent>(script);
}
