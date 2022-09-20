#ifndef _GAMEOBJECT_REG_CMD
#define _GAMEOBJECT_REG_CMD

#include "Command.h"

class GameObject;

class GameObjectRegCmd : public Command
{
public:
	GameObjectRegCmd(GameObject* const obj);
	GameObjectRegCmd(const GameObjectRegCmd&) = delete;
	GameObjectRegCmd& operator=(const GameObjectRegCmd&) = delete;
	GameObjectRegCmd(GameObjectRegCmd&&) = delete;
	GameObjectRegCmd& operator=(GameObjectRegCmd&&) = delete;
	~GameObjectRegCmd() = default;

	virtual void Execute() override;

private:
	GameObject* obj;
};

#endif _GAMEOBJECT_REG_CMD
