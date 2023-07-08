#ifndef _UUID
#define _UUID

#include "GlacierCore.h"
#include <random>

namespace Tools
{
	class GLACIER_API UUID
	{
	private:
		static UUID* instance;
		static UUID& Instance()
		{
			assert(instance && "UUID not initialized!");
			return *instance;
		}
		UUID();
		UUID(const UUID&) = delete;
		UUID& operator=(const UUID&) = delete;
		~UUID() = default;
	
		std::random_device rand_dev;
		std::mt19937_64 engine;
		std::uniform_int_distribution<uint64_t> unif_distri;

		std::unordered_set<uint64_t> uuids;

		uint64_t genUUID();

		static void Initialize();
		static void Terminate();

		friend class UUIDAtt;

	public:
		static const uint64_t GenUUID() { return Instance().genUUID(); }
	};
}

#endif _UUID