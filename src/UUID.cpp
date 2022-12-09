#include "gpch.h"
#include "UUID.h"

namespace Tools
{
	UUID& UUID::Instance()
	{
		static UUID instance;
		return instance;
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
