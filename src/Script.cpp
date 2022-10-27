#include "gpch.h"
#include "Script.h"
#include "Components.h"
#include "SceneManager.h"
#include "Scene.h"

Script::Script(const std::string& name)
	: script_name(name)
{
}

void Script::SetGameObject(GameObjectRef& new_obj)
{
	obj = new_obj;
}

GameObjectRef Script::CreateGameObject(std::string name)
{
	return GetCurrentScene().CreateGameObject(std::move(name));
}
GameObjectRef Script::CreateGameObject(std::string name, GameObjectRef parent, bool keep_world)
{
	return GetCurrentScene().CreateGameObject(std::move(name), parent, keep_world);
}

void Script::DestroyGameObject(GameObjectRef go)
{
	GetCurrentScene().DestroyGameObject(go);
}

GameObjectRef& Script::GetGameObject()
{
	return obj;
}

Scene& Script::GetCurrentScene()
{
	return *SceneManager::GetCurrentScene();
}

const std::string& Script::GetName() const
{
	return script_name;
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
void Script::ExecuteAllOnScreenResize(const Scene& scn, const int& width, const int& height)
{
	auto script_view = scn.GetRegistry().view<ScriptComponent>();
	for (auto&& [entity, script] : script_view.each())
	{
		script.script->OnScreenResize(width, height);
	}
}
