#ifndef _ENTITY_DEREG_CMD
#define _ENTITY_DEREG_CMD

#include "Command.h"

class Entity;

class EntityDeregCmd : public Command
{
public:
	EntityDeregCmd(Entity* const obj);
	EntityDeregCmd(const EntityDeregCmd&) = delete;
	EntityDeregCmd& operator=(const EntityDeregCmd&) = delete;
	EntityDeregCmd(EntityDeregCmd&&) = delete;
	EntityDeregCmd& operator=(EntityDeregCmd&&) = delete;
	~EntityDeregCmd() = default;

	virtual void Execute() override;

private:
	Entity* obj;
};

#endif _ENTITY_DEREG_CMD
