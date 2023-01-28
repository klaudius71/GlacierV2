#ifndef _SCRIPT_COMPONENT
#define _SCRIPT_COMPONENT

class Script;

struct ScriptComponent
{
	Script* script = nullptr;

	ScriptComponent(Script* script);
};

#endif