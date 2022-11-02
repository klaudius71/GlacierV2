#ifndef _EMPLACE_COMPONENT_COMMAND
#define _EMPLACE_COMPONENT_COMMAND

#include "gpch.h"
#include "Command.h"
#include "GameObject.h"

template<class T, typename ...Args>
class EmplaceComponentCommand : public Command
{
public:
	EmplaceComponentCommand() = delete;
	EmplaceComponentCommand(Entity* const obj, Args&&... args)
		: obj(obj), component(std::forward<Args>(args)...)
	{}
	EmplaceComponentCommand(const EmplaceComponentCommand&) = delete;
	EmplaceComponentCommand& operator=(const EmplaceComponentCommand&) = delete;
	EmplaceComponentCommand(EmplaceComponentCommand&&) = delete;
	EmplaceComponentCommand& operator=(EmplaceComponentCommand&&) = delete;
	~EmplaceComponentCommand() = default;

	virtual void Execute() override
	{
		obj->EmplaceComponent<T>(std::move(component));
	}

private:
	Entity* obj;
	T component;
};

#endif _EMPLACE_COMPONENT_COMMAND