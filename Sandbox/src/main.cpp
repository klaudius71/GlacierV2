#include <stdio.h>
#include "Sandbox.h"
#include "WindowDirectX.h"

struct MemMarker {
	~MemMarker()
	{
	}
} mem_marker;

int main()
{
#ifdef _SHOW_CONSOLE
	AllocConsole();
	FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;
	freopen_s(&fpstdin, "CONIN$", "r", stdin);
	freopen_s(&fpstdout, "CONOUT$", "w", stdout);
	freopen_s(&fpstderr, "CONOUT$", "w", stderr);
#endif

	Sandbox* sandbox = new Sandbox(1920, 1080);
	sandbox->Run();
	delete sandbox;
	return 0;
}