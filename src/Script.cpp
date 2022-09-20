#include "gpch.h"
#include "Script.h"
#include "GameObject.h"
#include "Scene.h"
#include "Components.h"

void Script::SetGameObject(GameObject* const new_obj)
{
	obj = new_obj;
}

void Script::ExecuteAllOnCreate(const Scene& scn)
{
	auto script_view = scn.GetRegistry().view<ScriptComponent>();
	for (auto&& [entity, script] : script_view.each())
		script.script->OnCreate();
}
void Script::ExecuteAllOnTick(const Scene& scn)
{
	auto script_view = scn.GetRegistry().view<ScriptComponent>();
	for (auto&& [entity, script] : script_view.each())
		script.script->OnTick();
}
void Script::ExecuteAllOnDestroy(const Scene& scn)
{
	auto script_view = scn.GetRegistry().view<ScriptComponent>();
	for (auto&& [entity, script] : script_view.each())
	{
		script.script->OnDestroy();
		delete script.script;
	}
}
