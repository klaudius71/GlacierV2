#ifndef _GAMEOBJECT_ATT
#define _GAMEOBJECT_ATT

#include "GameObject.h"

class GameObjectAtt
{
	friend class GameObjectRegCmd;
	friend class GameObjectDeregCmd;

	static void register_to_scene(GameObject* const obj) { obj->register_to_scene(); }
	static void deregister_from_scene(GameObject* const obj) { obj->deregister_from_scene(); }
};

#endif // !_GAMEOBJECT_ATT
