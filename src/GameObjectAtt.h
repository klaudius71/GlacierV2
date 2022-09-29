#ifndef _GAMEOBJECT_ATT
#define _GAMEOBJECT_ATT

#include "GameObject.h"

class GameObjectAtt
{
	friend class GameObjectRegCmd;
	friend class GameObjectDeregCmd;
	friend class SceneGraph;

	static void register_to_scene(GameObject* const obj) { obj->register_to_scene(); }
	static void deregister_from_scene(GameObject* const obj) { obj->deregister_from_scene(); }
	static void update_transform(GameObject& obj) { obj.update_transform(); }
	static std::vector<GameObjectRef>& GetChildren(GameObject& obj) { return obj.GetChildren(); }
	static SceneGraphRef& GetSceneGraphRef(GameObject& obj) { return obj.GetSceneGraphRef(); }
	static void SetSceneGraphRef(GameObject& obj, SceneGraphRef ref) { obj.SetSceneGraphRef(ref); }
};

#endif // !_GAMEOBJECT_ATT
