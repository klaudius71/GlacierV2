#include "gpch.h"
#include "Script.h"
#include "Components.h"
#include "SceneManager.h"
#include "Scene.h"

Script::Script(const std::string& name)
	: script_name(name)
{
}

void Script::SetGameObject(GameObject& new_obj)
{
	obj = new_obj;
}

GameObject Script::CreateGameObject(std::string name)
{
	return GetCurrentScene().CreateGameObject(std::move(name));
}
GameObject Script::CreateGameObject(std::string name, GameObject parent, bool keep_world)
{
	return GetCurrentScene().CreateGameObject(std::move(name), parent, keep_world);
}

void Script::DestroyGameObject(GameObject go)
{
	GetCurrentScene().DestroyGameObject(go);
}

GameObject& Script::GetGameObject()
{
	return obj;
}

Scene& Script::GetCurrentScene()
{
	return *SceneManager::GetCurrentScene();
}

const std::string& Script::GetScriptName() const
{
	return script_name;
}

void Script::ExecuteAllOnSceneEnter(const Scene& scn)
{
	auto script_view = scn.GetRegistry().view<ScriptComponent>();
	for (auto&& [entity, script] : script_view.each())
		script.script->OnSceneEnter();
}
void Script::ExecuteAllOnUpdate(const Scene& scn)
{
	auto script_view = scn.GetRegistry().view<ScriptComponent>();
	for (auto&& [entity, script] : script_view.each())
		script.script->OnUpdate();
}
void Script::ExecuteAllOnSceneExit(const Scene& scn)
{
	auto script_view = scn.GetRegistry().view<ScriptComponent>();
	for (auto&& [entity, script] : script_view.each())
	{
		script.script->OnSceneExit();
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
