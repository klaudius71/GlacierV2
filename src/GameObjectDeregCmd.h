#ifndef _GAMEOBJECT_DEREG_CMD
#define _GAMEOBJECT_DEREG_CMD

#include "Command.h"

class GameObject;

class GameObjectDeregCmd : public Command
{
public:
	GameObjectDeregCmd(GameObject* const obj);
	GameObjectDeregCmd(const GameObjectDeregCmd&) = delete;
	GameObjectDeregCmd& operator=(const GameObjectDeregCmd&) = delete;
	GameObjectDeregCmd(GameObjectDeregCmd&&) = delete;
	GameObjectDeregCmd& operator=(GameObjectDeregCmd&&) = delete;
	~GameObjectDeregCmd() = default;

	virtual void Execute() override;

private:
	GameObject* obj;
};

#endif _GAMEOBJECT_DEREG_CMD
