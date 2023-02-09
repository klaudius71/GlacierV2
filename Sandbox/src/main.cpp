#include <stdio.h>
#include "Sandbox.h"

struct MemMarker {
	~MemMarker()
	{
	}
} mem_marker;

int main()
{
	Sandbox* sandbox = new Sandbox(1600, 900);
	sandbox->Run();
	delete sandbox;
	return 0;
}