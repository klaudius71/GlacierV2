#include "gpch.h"
#include "CommandBroker.h"
#include "Command.h"

CommandBroker::~CommandBroker()
{
	while (!cmds.empty())
	{
		delete cmds.front();
		cmds.pop();
	}
}

void CommandBroker::Enqueue(Command* const cmd)
{
	cmds.emplace(cmd);
}

void CommandBroker::DequeueAndExecuteAll()
{
	while(!cmds.empty())
	{
		Command* const cmd = cmds.front();
		cmd->Execute();
		cmds.pop();
		delete cmd;
	}
}
