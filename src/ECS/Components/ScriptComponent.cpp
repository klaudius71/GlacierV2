#include "gpch.h"
#include "ScriptComponent.h"

ScriptComponent::ScriptComponent(Script* const script)
	: script(script)
{
	assert(script);
}