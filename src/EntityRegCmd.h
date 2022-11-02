#ifndef _ENTITY_REG_CMD
#define _ENTITY_REG_CMD

#include "Command.h"

class Entity;

class EntityRegCmd : public Command
{
public:
	EntityRegCmd(Entity* const obj);
	EntityRegCmd(const EntityRegCmd&) = delete;
	EntityRegCmd& operator=(const EntityRegCmd&) = delete;
	EntityRegCmd(EntityRegCmd&&) = delete;
	EntityRegCmd& operator=(EntityRegCmd&&) = delete;
	~EntityRegCmd() = default;

	virtual void Execute() override;

private:
	Entity* obj;
};

#endif _ENTITY_REG_CMD
