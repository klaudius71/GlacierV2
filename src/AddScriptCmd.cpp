#include "gpch.h"
#include "AddScriptCmd.h"
#include "GameObject.h"

AddScriptCmd::AddScriptCmd(GameObject* const obj, Script* const script)
	: obj(obj), script(script)
{
}

void AddScriptCmd::Execute()
{
	obj->EmplaceComponent<ScriptComponent>(script);
}
