#ifndef _SCRIPT_ATT
#define _SCRIPT_ATT

#include "Script.h"

class ScriptAtt
{
	friend class GameObject;

	static void SetGameObject(Script* const scrpt, GameObject* const obj) { scrpt->SetGameObject(obj); }
};

#endif _SCRIPT_ATT
