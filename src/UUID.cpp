#include "gpch.h"
#include "UUID.h"

namespace Tools
{
	UUID* UUID::instance = nullptr;

	void UUID::Initialize()
	{
		assert(!instance && "UUID already initialized!");
		instance = new UUID;
	}
	void UUID::Terminate()
	{
		assert(instance && "UUID not initialized!");
		delete instance;
	}

	UUID::UUID()
		: rand_dev(), engine(rand_dev()), unif_distri(), uuids()
	{
	}

	uint64_t UUID::genUUID()
	{
		uint64_t uuid;
		while (uuids.find((uuid = unif_distri(engine))) != uuids.cend());
		uuids.emplace(uuid);
		return uuid;
	}
}
