#include "gpch.h"
#include "ScriptComponent.h"

ScriptComponent::ScriptComponent(Script* script)
	: script(script)
{
	assert(script);
}