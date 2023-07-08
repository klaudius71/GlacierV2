#ifndef _LOGGER_ATT
#define _LOGGER_ATT

#include "Logger.h"
#include "Application.h"

class LoggerAtt
{
	friend class Glacier::Application;

	static void Initialize() { Logger::Initialize(); }
	static void Terminate() { Logger::Terminate(); }
};

#endif