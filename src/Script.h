#ifndef _SCRIPT
#define _SCRIPT

#include "SceneGraph.h"

class Scene;

class Script
{
public:
	Script(const std::string& name = "Unnamed Script");
	virtual ~Script() = default;

	virtual void OnSceneEnter() {};
	virtual void OnUpdate(float dt) { UNREFERENCED_PARAMETER(dt); };
	virtual void OnSceneExit() {};

	// Engine events
	virtual void OnScreenResize(const int& width, const int& height) { UNREFERENCED_PARAMETER(width); UNREFERENCED_PARAMETER(height); };


	GameObject CreateGameObject(const std::string& name);
	GameObject CreateGameObject(const std::string& name, GameObject parent);
	void DestroyGameObject(GameObject go);

	GameObject& GetGameObject();
	Scene& GetCurrentScene();

	const std::string& GetScriptName() const;

private:
	GameObject obj;
	std::string script_name;

	void SetGameObject(GameObject& new_obj);

	static void ExecuteAllOnSceneEnter(const Scene& scn);
	static void ExecuteAllOnUpdate(const Scene& curr_scene);
	static void ExecuteAllOnSceneExit(const Scene& scn);
	static void ExecuteAllOnScreenResize(const Scene& scn, const int& width, const int& height);

	friend class ScriptAtt;
	friend class SceneManager;
};

#endif _SCRIPT