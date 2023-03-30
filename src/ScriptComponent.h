#ifndef _SCRIPT_COMPONENT
#define _SCRIPT_COMPONENT

#include "GlacierCore.h"

class Script;

struct GLACIER_API ScriptComponent
{
	Script* script = nullptr;

	ScriptComponent(Script* const script);
};

#endif