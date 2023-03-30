#ifndef _UUID_ATT
#define _UUID_ATT

#include "UUID.h"
#include "Application.h"

namespace Tools {
	
	class UUIDAtt
	{
		friend class Glacier::Application;

		static void Initialize() { UUID::Initialize(); }
		static void Terminate() { UUID::Terminate(); }
	};
}

#endif