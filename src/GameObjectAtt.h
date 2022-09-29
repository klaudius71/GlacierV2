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
	static GameObjectRef& GetParent(GameObject& obj) { return obj.GetParent(); }
	static std::list<GameObjectRef>& GetChildren(GameObject& obj) { return obj.GetChildren(); }
	static SceneGraphRef& GetSceneGraphRef(GameObject& obj) { return obj.GetSceneGraphRef(); }
	static void SetSceneGraphRef(GameObject& obj, SceneGraphRef ref) { obj.SetSceneGraphRef(ref); }
	static std::list<GameObjectRef>::const_iterator GetAsChildRef(GameObject& obj) { return obj.GetAsChildRef(); }
	static void SetAsChildRef(GameObject& obj, std::list<GameObjectRef>::const_iterator& ref) { obj.SetAsChildRef(ref); }
	static void EraseChild(GameObject& obj, std::list<GameObjectRef>::const_iterator ref) { obj.EraseChild(ref); }
};

#endif // !_GAMEOBJECT_ATT
