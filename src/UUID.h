#ifndef _UUID
#define _UUID

#include <random>

namespace Tools
{
	class UUID
	{
	private:
		static UUID& Instance();
		UUID();
		UUID(const UUID&) = delete;
		UUID& operator=(const UUID&) = delete;
		~UUID() = default;
	
		std::random_device rand_dev;
		std::mt19937_64 engine;
		std::uniform_int_distribution<uint64_t> unif_distri;

		std::unordered_set<uint64_t> uuids;

		uint64_t genUUID();

	public:
		static const uint64_t GenUUID() { return Instance().genUUID(); }
	};
}

#endif _UUID