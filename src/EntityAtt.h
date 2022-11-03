#ifndef _GAMEOBJECT_ATT
#define _GAMEOBJECT_ATT

#include "Entity.h"

class EntityAtt
{
	friend class EntityRegCmd;
	friend class EntityDeregCmd;
	friend class SceneGraph;

	static void register_to_scene(Entity* const obj) { obj->register_to_scene(); }
	static void deregister_from_scene(Entity* const obj) { obj->deregister_from_scene(); }
	static void update_transform(Entity& obj) { obj.update_transform(); }
	static std::list<GameObject>& GetChildren(Entity& obj) { return obj.children; }
	static SceneGraphRef& GetSceneGraphRef(Entity& obj) { return obj.GetSceneGraphRef(); }
	static void SetSceneGraphRef(Entity& obj, SceneGraphRef ref) { obj.SetSceneGraphRef(ref); }
	static std::list<GameObject>::const_iterator GetAsChildRef(Entity& obj) { return obj.GetAsChildRef(); }
	static void SetAsChildRef(Entity& obj, std::list<GameObject>::const_iterator& ref) { obj.SetAsChildRef(ref); }
	static void EraseChild(Entity& obj, std::list<GameObject>::const_iterator ref) { obj.EraseChild(ref); }
};

#endif // !_GAMEOBJECT_ATT
