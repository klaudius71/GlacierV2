#include <stdio.h>
#include "Sandbox.h"

int main()
{
	Sandbox* sandbox = new Sandbox(1920, 1080);
	sandbox->Run();
	delete sandbox;
	return 0;
}