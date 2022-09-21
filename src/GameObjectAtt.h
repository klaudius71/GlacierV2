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
	static void update_transform(GameObject* const obj) { obj->update_transform(); }
	static std::vector<GameObject*>& GetChildren(GameObject* const obj) { return obj->children; }
};

#endif // !_GAMEOBJECT_ATT
