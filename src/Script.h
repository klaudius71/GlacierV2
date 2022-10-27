#ifndef _SCRIPT
#define _SCRIPT

#include "SceneGraph.h"

class Scene;

class Script
{
public:
	Script(const std::string& name = "");
	virtual ~Script() = default;

	virtual void OnCreate() = 0;
	virtual void OnTick() = 0;
	virtual void OnDestroy() {};

	virtual void OnScreenResize(const int& width, const int& height) { width; height; };

	GameObjectRef CreateGameObject(std::string name);
	GameObjectRef CreateGameObject(std::string name, GameObjectRef parent, bool keep_world = false);
	void DestroyGameObject(GameObjectRef go);

	GameObjectRef& GetGameObject();
	Scene& GetCurrentScene();

	const std::string& GetName() const;

private:
	GameObjectRef obj;
	std::string script_name;

	void SetGameObject(GameObjectRef& new_obj);

	static void ExecuteAllOnCreate(const Scene& scn);
	static void ExecuteAllOnTick(const Scene& curr_scene);
	static void ExecuteAllOnDestroy(const Scene& scn);
	static void ExecuteAllOnScreenResize(const Scene& scn, const int& width, const int& height);

	friend class ScriptAtt;
	friend class SceneManager;
};

#endif _SCRIPT