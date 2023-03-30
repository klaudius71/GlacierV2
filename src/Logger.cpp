#include "gpch.h"
#include "Logger.h"

Logger* Logger::instance = nullptr;

void Logger::log(const char* const key, const int value)
{
	ints[key] = value;
}
void Logger::log(const char* const key, const float value)
{
	floats[key] = value;
}
void Logger::log(const char* const key, const double value)
{
	doubles[key] = value;
}
void Logger::log(const char* const key, const std::string& value)
{
	strings[key] = value;
}

const int& Logger::getInt(const char* const key)
{
	return ints[key];
}
const int& Logger::getInt(const char* const key, const int default_value)
{
	auto it = ints.find(key);
	return it != ints.cend() ? it->second : default_value;
}
const float& Logger::getFloat(const char* const key)
{
	return floats[key];
}
const float& Logger::getFloat(const char* const key, const float default_value)
{
	auto it = floats.find(key);
	return it != floats.cend() ? it->second : default_value;
}
const double& Logger::getDouble(const char* const key)
{
	return doubles[key];
}
const double& Logger::getDouble(const char* const key, const double default_value)
{
	auto it = doubles.find(key);
	return it != doubles.cend() ? it->second : default_value;
}
const std::string& Logger::getString(const char* const key)
{
	return strings[key];
}
const std::string& Logger::getString(const char* const key, const std::string& default_value)
{
	auto it = strings.find(key);
	return it != strings.cend() ? it->second : default_value;
}

void Logger::Initialize()
{
	assert(!instance && "Logger already initialized!");
	instance = new Logger;
}
void Logger::Terminate()
{
	assert(instance && "Logger not initialized!");
	delete instance;
}
