#ifndef _SANDBOX
#define _SANDBOX

#include "GlacierAPI.h"

class Sandbox : public Glacier::Application
{
public:
	Sandbox(const int screen_width, const int screen_height);
	~Sandbox() = default;

	virtual void LoadResources() override;
};

#endif