#ifndef _RENDERER_2D_ATT
#define _RENDERER_2D_ATT

#include "Renderer2D.h"

class Renderer2DAtt
{
	friend class Glacier;

	static void Initialize() { Renderer2D::Initialize(); }
	static void Terminate() { Renderer2D::Terminate(); }
};

#endif _RENDERER_2D_ATT
