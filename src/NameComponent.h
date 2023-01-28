#ifndef _NAME_COMPONENT
#define _NAME_COMPONENT

struct NameComponent
{
	std::string name;
	size_t id;

	NameComponent(const std::string& name);
	NameComponent(std::string&& name);
	NameComponent(NameComponent&& o);
	NameComponent& operator=(NameComponent&& o);
};

#endif