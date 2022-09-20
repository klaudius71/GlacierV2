#ifndef _COMMAND_BROKER
#define _COMMAND_BROKER

class Command;

class CommandBroker
{
public:
	CommandBroker() = default;
	CommandBroker(const CommandBroker&) = delete;
	CommandBroker& operator=(const CommandBroker&) = delete;
	CommandBroker(CommandBroker&&) = delete;
	CommandBroker& operator=(CommandBroker&&) = delete;
	~CommandBroker();

	void Enqueue(Command* const cmd);
	void DequeueAndExecuteAll();

private:
	std::queue<Command*> cmds;
};

#endif _COMMAND_BROKER