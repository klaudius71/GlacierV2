#ifndef _COMMAND
#define _COMMAND

class Command
{
public:
	Command() = default;
	Command(const Command&) = default;
	Command& operator=(const Command&) = default;
	Command(Command&&) = default;
	Command& operator=(Command&&) = default;
	virtual ~Command() = default;

	virtual void Execute() = 0;
};

#endif _COMMAND