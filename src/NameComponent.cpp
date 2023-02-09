#include "gpch.h"
#include "NameComponent.h"
#include "UUID.h"

NameComponent::NameComponent(const std::string& name)
	: name(name), id(Tools::UUID::GenUUID())
{
}
NameComponent::NameComponent(std::string&& name)
	: name(std::move(name)), id(Tools::UUID::GenUUID())
{}
NameComponent::NameComponent(NameComponent&& o)
	: name(std::move(o.name)), id(o.id)
{ o.id = (size_t)-1; }
NameComponent& NameComponent::operator=(NameComponent&& o)
{
	name = std::move(o.name);
	id = o.id;

	o.id = (size_t)-1;

	return *this;
}