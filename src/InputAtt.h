#ifndef _INPUT_ATT
#define _INPUT_ATT

#include "Input.h"

class InputAtt
{
	friend class Glacier::Application;

	static void ProcessMouseData() { Input::ProcessMouseData(); }
};

#endif _INPUT_ATT