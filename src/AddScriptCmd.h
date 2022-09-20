#ifndef _ADD_SCRIPT_CMD
#define _ADD_SCRIPT_CMD

#include "Command.h"

class GameObject;
class Script;

class AddScriptCmd : public Command
{
public:
	AddScriptCmd() = delete;
	AddScriptCmd(GameObject* const obj, Script* const script);
	AddScriptCmd(const AddScriptCmd&) = delete;
	AddScriptCmd& operator=(const AddScriptCmd&) = delete;
	AddScriptCmd(AddScriptCmd&&) = delete;
	AddScriptCmd& operator=(AddScriptCmd&&) = delete;
	~AddScriptCmd() = default;

	virtual void Execute() override;

private:
	GameObject* obj;
	Script* script;
};

#endif