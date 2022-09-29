#ifndef _SCRIPT
#define _SCRIPT

#include "SceneGraph.h"

class Scene;

class Script
{
public:
	Script() = default;
	virtual ~Script() = default;

	virtual void OnCreate() = 0;
	virtual void OnTick() = 0;
	virtual void OnDestroy() {};

	//template<class T>
	//T& GetComponent()
	//{
	//	return obj->GetComponent<T>();
	//}

	GameObjectRef obj;
private:
	void SetGameObject(GameObjectRef& new_obj);

	static void ExecuteAllOnCreate(const Scene& scn);
	static void ExecuteAllOnTick(const Scene& curr_scene);
	static void ExecuteAllOnDestroy(const Scene& scn);

	friend class ScriptAtt;
	friend class SceneManager;
};

#endif _SCRIPT