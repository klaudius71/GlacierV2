#ifndef _INPUT
#define _INPUT

#include "GlacierCore.h"
#include "KeyCodes.h"

class GLACIER_API Input
{
public:
	static bool GetKeyDown(GLACIER_KEY key);
	static bool GetMouseButtonState(GLACIER_MOUSE btn);
	static const glm::f64vec2& GetMousePosition();
	static const glm::vec2& GetMouseDeltaPosition();

private:
	static glm::f64vec2 curr_mouse_position;
	static glm::f64vec2 prev_mouse_position;
	static glm::vec2 mouse_delta;

	static void ProcessMouseData();

	friend class InputAtt;
};

#endif _INPUT